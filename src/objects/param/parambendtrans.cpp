#include <objects/param/parambendtrans.hpp>

#include "functions/details/fvbase.hpp"

namespace objects
{
  namespace details
  {

    template <typename T, int IDX, bool NORMAL>
    struct bend_trans_t
    {
      bend_trans_t(const IParamSurface &child, const T& k, const T &alpha)
	:m_child(child), m_k(k), m_alpha(alpha)
	{}


      template <typename X>
      typename core::arith::ArithTraits<typename X::value_type>::vec_t 
      transformed_normal(const X &j, 
			 const typename core::arith::ArithTraits<typename X::value_type>::vec_t &c_normal) const
	{
	  X j_inv(3,3);
	  typename X::value_type det(  j[0][0]*(j[1][1]*j[2][2] - j[1][2]*j[2][1]) 
				     + j[0][1]*(j[1][2]*j[2][0] - j[1][0]*j[2][2])
				     + j[0][2]*(j[1][0]*j[2][1] - j[1][1]*j[2][0]));
	  
	  if(0 <= enclosure_cast<core::arith::interval>(det))
	    return typename core::arith::ArithTraits<typename X::value_type>::vec_t(0);
	  // Fehler
	  j_inv[0][0] = j[2][2]*j[1][1] - j[2][1]*j[1][2];
	  j_inv[0][1] = j[2][1]*j[0][2] - j[2][2]*j[0][1];
	  j_inv[0][2] = j[1][2]*j[0][1] - j[1][1]*j[0][2];
	  
	  j_inv[1][0] = j[2][0]*j[1][2] - j[2][2]*j[1][0];
	  j_inv[1][1] = j[2][2]*j[0][0] - j[2][0]*j[0][2];
	  j_inv[1][2] = j[1][0]*j[0][2] - j[1][2]*j[0][0];
	  
	  j_inv[2][0] = j[2][1]*j[1][0] - j[2][0]*j[1][1];
	  j_inv[2][1] = j[2][1]*j[0][1] - j[2][1]*j[0][0];
	  j_inv[2][2] = j[1][1]*j[0][0] - j[1][0]*j[0][1];
	  j_inv = j_inv * det;

	  return typename core::arith::ArithTraits<typename X::value_type>::vec_t(det*trans(j_inv)*c_normal);
	}

      template <typename X>
      typename boost::enable_if_c<IDX == -1, X>::type rval(const X& r) const 
	{
	  return r;
	}

      template <typename X>
      typename boost::disable_if_c<IDX == -1, typename X::value_type>::type rval(const X& r) const 
	{
	  return r[IDX];
	}


      template <typename X, typename Y>
      typename boost::mpl::if_c<IDX == -1, X, typename X::value_type>::type
      eval(const X& args, const Y& k, const Y &alpha) const
	{
//	  X org(NORMAL ? 6 : 3, forced_cast<typename X::value_type>(core::arith::interval(1,-1)));
	  X org;
	  if(NORMAL && m_child.p_normals()) {
	    // std::cout << (*m_child.p_normals())(args) << std::endl;
	    //std::cout << (*(m_child.p_normals()))(args) << std::endl;
	    org = (*(m_child.p_normals()))(args);
	  }
	  else
	    org = m_child.p_fun()(args);
	  //std::cout << "Org " << org << std::endl;

	  typename X::value_type gamma(k*org[2]);
	  typename X::value_type kinv(1.0/k);
	  typename X::value_type cosalpha(cos(alpha));
	  typename X::value_type sinalpha(sin(alpha));
	  typename X::value_type r(org[0]*cosalpha + org[1]*sinalpha);
	  if(IDX < 2) {
	    typename X::value_type R(kinv - (kinv-r)*cos(gamma));
	    if(IDX == -1 || IDX == 0)
	      org[0] += (R-r)*cosalpha;
	    if(IDX == -1 || IDX == 1)
	      org[1] += (R-r)*sinalpha;
	  }
	  if(IDX == -1 || IDX ==2)
	    org[2] = (kinv-r)*sin(gamma);

	  if(size(org) == 6) {
	    typename core::arith::ArithTraits<typename X::value_type>::mat_t j(3,3);
	    typename X::value_type cosgamma(cos(gamma));
	    typename X::value_type singamma(sin(gamma));

	    // Ableitung X/x,y,z
	    j[0][0] = 1.0 + sqr(cosalpha)*(cosgamma - 1.0);
	    j[0][1] = sinalpha*(cosgamma - 1.0)*cosalpha;
	    j[0][2] = singamma*(1.0 - k*r)*cosalpha;
	    
	    // Ableitung Y/x,y,z
	    j[1][0] = sinalpha*cosalpha*(cosgamma - 1.0);
	    j[1][1] = 1 + sqr(sinalpha)*(cosgamma - 1.0);
	    j[1][2] = singamma*sinalpha - (1.0 - k*r);
	    
	    // Ableitung Z/x,y,z
	    j[2][0] = -cosalpha*singamma;
	    j[2][1] = -sinalpha*singamma;
	    j[2][2] = cosgamma*(1.0 - r*k);

	    X tn(transformed_normal(j, org[mtl::irange(3,6)]));
	    org[mtl::irange(3,6)] = size(tn) ? tn : 
	      X(3, forced_cast<typename X::value_type>(core::arith::interval(1, -1)));
	  }

	  return rval(org);
	}

      template <typename X>
      typename boost::mpl::if_c<IDX == -1, X, typename X::value_type>::type 
      operator()(const X& args) const
	{
	  return eval(args, core::arith::interval(m_k), core::arith::interval(m_alpha));
	}

      typename boost::mpl::if_c<IDX == -1, core::arith::rvector, core::arith::mreal>::type
      operator()(const core::arith::rvector &args) const 
	{
	  return eval(args, forced_cast<core::arith::mreal>(m_k), forced_cast<core::arith::mreal>(m_alpha));
	}

      unsigned dim() const
	{
	  return m_child.p_fun().dim();
	}

      unsigned cdim() const
	{
	  if(NORMAL)
	    return 2*(dim()+1);
	  return dim()+1;
	}

      std::string text() const
	{
	  std::stringstream sstr;
	  sstr << "Bending transformation " << m_child.p_fun().text()
	       << " (with k=" << m_k << " and alpha = " << m_alpha << ")";
	  return sstr.str();
	}


      T m_k;
      T m_alpha;
      const IParamSurface& m_child;
    };

    template <typename T>
    ParamBendTrans<T>::ParamBendTrans(IParamSurface *child, const T& k, const T &alpha)
      :m_child(child)
    {
      // std::vector<functions::IFunction*> members;
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 0, false> >
      // 			(new bend_trans_t<T,0,false>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 1, false> >
      // 			(new bend_trans_t<T,1,false>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 2, false> >
      // 			(new bend_trans_t<T,2, false>(*child, k, alpha)));
      m_pf.reset(new functions::details::FVBase<bend_trans_t<T, -1, false> >
		 (new bend_trans_t<T,-1, false>(*child, k, alpha)));

      // std::vector<functions::IFunction*> nmembers;
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 0, true> >
      // 			(new bend_trans_t<T,0,true>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 1, true> >
      // 			(new bend_trans_t<T,1,true>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 2, true> >
      // 			(new bend_trans_t<T,2, true>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 3, true> >
      // 			(new bend_trans_t<T,3,true>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 4, true> >
      // 			(new bend_trans_t<T,4,true>(*child, k, alpha)));
      // members.push_back(new functions::details::FBase<bend_trans_t<T, 5, true> >
      // 			(new bend_trans_t<T,5, true>(*child, k, alpha)));
      m_npf.reset(new functions::details::FVBase<bend_trans_t<T, -1, true> >
		  (new bend_trans_t<T, -1, true>(*child, k, alpha)));
    }

    template <typename T>
    ParamBendTrans<T>::~ParamBendTrans() {}

    template <typename T>
    const IParamSurface& ParamBendTrans<T>::child_() const
    {
      return *m_child;
    }

    template <typename T>
    core::arith::ivector ParamBendTrans<T>::domain_() const
    {
      return m_child->domain();
    }

    template <typename T>
    unsigned ParamBendTrans<T>::d_dim_() const
    {
      return m_child->d_dim();
    }

    template <typename T>
    const functions::IFunction& ParamBendTrans<T>::cf_() const
    {
      std::cerr << "cf() not implemented for ParametricBending, returning child's cf instead" 
		<< std::endl;
      return m_child->cf();
      
    }

    template <typename T>
    unsigned ParamBendTrans<T>::dim_() const
    {
      //return 0;
      return m_cf->dim();
    }

    template <typename T>
    IGeoObj* ParamBendTrans<T>::clone_() const
    {
      typedef functions::details::FVBase<bend_trans_t<T, -1, false> > fun_t;
      auto pf(dynamic_cast<fun_t*>(m_pf.get()));
      if(pf)
	return new ParamBendTrans<T>(dynamic_cast<IParamSurface*>(m_child->clone()), 
				     pf->get_f().m_k, pf->get_f().m_alpha);
      return (IGeoObj*)0;
    }

    template <typename T>
    const functions::IVFunction& ParamBendTrans<T>::p_fun_() const
    {
      return *m_pf;
    }

    template <typename T>
    const functions::IVFunction* ParamBendTrans<T>::p_normals_() const
    {
      return m_npf.get();
    }

    template <typename T>
    const functions::IVFunction* ParamBendTrans<T>::normals_() const
    {
      std::cerr << "cf() not implemented for ParametricBending, returning child's cf instead" 
		<< std::endl;
      return m_child->normals();
    }

    template class ParamBendTrans<core::arith::interval>;
    template class ParamBendTrans<core::arith::mreal>;
  }
}
