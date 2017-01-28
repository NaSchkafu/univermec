#include <objects/param/paramtapertrans.hpp>

#include "functions/details/fvbase.hpp"

namespace objects
{
  namespace details
  {
    template <typename T, int IDX, bool NORMAL>
    struct taper_trans_t
    {
      taper_trans_t(const IParamSurface &child, const T& kx, const T &ky, const T& sz)
        :m_child(child), m_kx(kx), m_ky(ky), m_sz(sz)
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
      eval(const X& args, const Y& kx, const Y &ky, const Y& sz) const
        {
//        X org(NORMAL ? 6 : 3, forced_cast<typename X::value_type>(core::arith::interval(1,-1)));
          X org(NORMAL &&  m_child.p_normals() ?  (*m_child.p_normals())(args) : m_child.p_fun()(args));

          typename X::value_type fxz((kx/sz)*org[2] + 1.0);
          typename X::value_type fyz((ky/sz)*org[2] + 1.0);



          if(size(org) == 6) {
	    typename core::arith::ArithTraits<typename X::value_type>::mat_t j(3,3);
            // Ableitung X/x,y,z
            j[0][0] = fxz;
            j[0][1] = 0;
            j[0][2] = (kx/sz)*org[0];

            // Ableitung Y/x,y,z
            j[1][0] = 0;
            j[1][1] = fxz;
            j[1][2] = (ky/sz)*org[1];

            // Ableitung Z/x,y,z
            j[2][0] = 0;
            j[2][1] = 0;
            j[2][2] = 1;

            X tn(transformed_normal(j, org[mtl::irange(3,6)]));
            org[mtl::irange(3,6)] = size(tn) ? tn :
              X(3, forced_cast<typename X::value_type>(core::arith::interval(1, -1)));
          }

          org[0] *= fxz;
          org[1] *= fyz;

          return rval(org);
        }

      template <typename X>
      typename boost::mpl::if_c<IDX == -1, X, typename X::value_type>::type
      operator()(const X& args) const
        {
          return eval(args, core::arith::interval(m_kx), core::arith::interval(m_ky), core::arith::interval(m_sz));
        }

      typename boost::mpl::if_c<IDX == -1, core::arith::rvector, core::arith::mreal>::type
      operator()(const core::arith::rvector &args) const
        {
          return eval(args, forced_cast<core::arith::mreal>(m_kx), 
		      forced_cast<core::arith::mreal>(m_ky), forced_cast<core::arith::mreal>(m_sz));
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
               << " (with kx=" << m_kx << ", ky = " << m_ky << " scaling_z=" << m_sz << ")";
          return sstr.str();
        }


      T m_kx;
      T m_ky;
      T m_sz;
      const IParamSurface& m_child;
    };


    template <typename T>
    ParamTaperTrans<T>::ParamTaperTrans(IParamSurface *child, const T& kx, const T &ky, const T& sz)
      :m_child(child)
    {
      // std::vector<functions::IFunction*> members;
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 0, false> >
      //                   (new taper_trans_t<T,0,false>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 1, false> >
      //                   (new taper_trans_t<T,1,false>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 2, false> >
      //                   (new taper_trans_t<T,2, false>(*child, kx, ky, sz)));
      m_pf.reset(new functions::details::FVBase<taper_trans_t<T, -1, false> >
                 (new taper_trans_t<T,-1, false>(*child, kx, ky, sz)));

      // std::vector<functions::IFunction*> nmembers;
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 0, true> >
      //                   (new taper_trans_t<T,0,true>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 1, true> >
      //                   (new taper_trans_t<T,1,true>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 2, true> >
      //                   (new taper_trans_t<T,2, true>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 3, true> >
      //                   (new taper_trans_t<T,3,true>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 4, true> >
      //                   (new taper_trans_t<T,4,true>(*child, kx, ky, sz)));
      // members.push_back(new functions::details::FBase<taper_trans_t<T, 5, true> >
      //                   (new taper_trans_t<T,5, true>(*child, kx, ky, sz)));
      m_npf.reset(new functions::details::FVBase<taper_trans_t<T, -1, true> >
                  (new taper_trans_t<T, -1, true>(*child, kx, ky, sz)));
    }

    template <typename T>
    ParamTaperTrans<T>::~ParamTaperTrans() {}

    template <typename T>
    const IParamSurface& ParamTaperTrans<T>::child_() const
    {
      return *m_child;
    }

    template <typename T>
    core::arith::ivector ParamTaperTrans<T>::domain_() const
    {
      return m_child->domain();
    }

    template <typename T>
    unsigned ParamTaperTrans<T>::d_dim_() const
    {
      return m_child->d_dim();
    }

    template <typename T>
    const functions::IFunction& ParamTaperTrans<T>::cf_() const
    {
      std::cerr << "cf() not implemented for ParametricBending, returning child's cf instead"
                << std::endl;
      return m_child->cf();

    }

    template <typename T>
    unsigned ParamTaperTrans<T>::dim_() const
    {
      return 0;
      //return m_cf->dim();
    }

    template <typename T>
    IGeoObj* ParamTaperTrans<T>::clone_() const
    {
      typedef functions::details::FVBase<taper_trans_t<T, -1, false> > fun_t;
      auto pf(dynamic_cast<fun_t*>(m_pf.get()));
      if(pf)
        return new ParamTaperTrans<T>(dynamic_cast<IParamSurface*>(m_child->clone()),
				      pf->get_f().m_kx, pf->get_f().m_ky, pf->get_f().m_sz);
      return (IGeoObj*)0;
    }

    template <typename T>
    const functions::IVFunction& ParamTaperTrans<T>::p_fun_() const
    {
      return *m_pf;
    }

    template <typename T>
    const functions::IVFunction* ParamTaperTrans<T>::p_normals_() const
    {
      return m_npf.get();
    }

    template <typename T>
    const functions::IVFunction* ParamTaperTrans<T>::normals_() const
    {
      return nullptr;
    }

    template class ParamTaperTrans<core::arith::interval>;
    template class ParamTaperTrans<core::arith::mreal>;
  }
}


