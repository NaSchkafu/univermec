#include <objects/param/paramlintrans.hpp>

#include "functions/details/flinchainrule.hpp"
#include "functions/details/fvbase.hpp"
#include "objects/details/implnormals.hpp"

namespace objects
{
  namespace details 
  {
    template <typename T, bool NORMAL>
    struct lin_trans_t
    {
      lin_trans_t(const IParamSurface &child,
		  const typename core::arith::ArithTraits<T>::mat_t &r,
		  const typename core::arith::ArithTraits<T>::vec_t &t)
	:m_child(child), m_r(r), m_t(t)
	{
	  assert(!NORMAL || m_child.normals() && "lin_trans_t in normal mode requested by surface does not"
		 " provide normals!");
	}

      template <typename X>
      X operator()(const X& args) const
	{
	  // std::cout << "Input " << args << std::endl;
	  // std::cout << "t " << m_t << std::endl;
	  // std::cout << "r " << m_r << std::endl;
	  // std::cout << "Erg 1 " << m_child.p_fun()(args) << std::endl;
	  // std::cout << "Erg 2 " << X(m_r*m_child.p_fun()(args)) << std::endl;
	  // std::cout << "Erg 3 " << X(m_r*m_child.p_fun()(args) + m_t) << std::endl;
	  // Normal Mode
	  if(NORMAL) {
	    unsigned dim(size(args)+1);
	    X xn((*m_child.p_normals())(args));
	    xn[mtl::irange(0,dim)] = X(m_r*xn[mtl::irange(0, dim)] + m_t);
	    if(size(xn) > dim)
	      xn[mtl::irange(dim, 2*dim)] = X(m_r*xn[mtl::irange(dim, 2*dim)] + m_t);
	    return xn;
	  }

	  return X(m_r*m_child.p_fun()(args) + m_t);
	  // std::cout << "Output " << x << std::endl;
	  //return x;
	}

      // Interval Transformation soll mit Realpunkt ausgewertet werden!
      core::arith::rvector operator()(const core::arith::rvector& args) const
	{
	  return core::arith::rvector(forced_cast<core::arith::rmatrix>(m_r)*m_child.p_fun()(args) 
				      + forced_cast<core::arith::rvector>(m_t));
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
	  sstr << "Linear transformation M*" << m_child.p_fun().text()
	       << "+t" << " (with M=" << m_r << " and t = " << m_t;
	  return sstr.str();
	}

      typename core::arith::ArithTraits<T>::mat_t m_r;
      typename core::arith::ArithTraits<T>::vec_t m_t;
      const IParamSurface& m_child;
    };

    template <typename T>
    ParamLinTrans<T>::ParamLinTrans(IParamSurface *child, 
				    const typename core::arith::ArithTraits<T>::mat_t &r, 
				    const typename core::arith::ArithTraits<T>::mat_t &ir,
				    const typename core::arith::ArithTraits<T>::vec_t &t,
				    const typename core::arith::ArithTraits<T>::vec_t &it)
      :m_child(child)
    {
      assert(m_child && "Linear transformation for non existing child requested!");
      m_cf.reset(new cf_fun_t(m_child->cf(), ir, it));
      //std::vector<functions::IFunction*> members;
      m_pf.reset(new pf_fun_t(new lin_trans_t<T, false>(*child, r, t)));
      m_npf.reset(new pnf_fun_t(new lin_trans_t<T, true>(*child, r, t)));
    }

    template <typename T>
    ParamLinTrans<T>::~ParamLinTrans() {}

    template <typename T>
    void ParamLinTrans<T>::set_t_s(const typename core::arith::ArithTraits<T>::mat_t &r, 
				   const typename core::arith::ArithTraits<T>::mat_t &ir,
				   const typename core::arith::ArithTraits<T>::vec_t &t,
				   const typename core::arith::ArithTraits<T>::vec_t &it)
    {
      m_cf.reset(new cf_fun_t(m_child->cf(), ir, it));
      m_nf.reset(details::normal_fun(*m_cf.get()));
      std::vector<functions::IFunction*> members;
      m_pf.reset(new pf_fun_t(new lin_trans_t<T, false>(*m_child, r, t)));
      m_npf.reset(new pnf_fun_t(new lin_trans_t<T, true>(*m_child, r, t)));
    }

    template <typename T>
    const IParamSurface& ParamLinTrans<T>::child_() const
    {
      return *m_child;
    }

    template <typename T>
    core::arith::ivector ParamLinTrans<T>::domain_() const
    {
      return m_child->domain();
    }

    template <typename T>
    unsigned ParamLinTrans<T>::d_dim_() const
    {
      return m_child->d_dim();
    }

    template <typename T>
    const functions::IFunction& ParamLinTrans<T>::cf_() const
    {
      return *m_cf;
    }

    template <typename T>
    const functions::IVFunction* ParamLinTrans<T>::normals_() const
    {
      return m_nf.get();
    }

    template <typename T>
    unsigned ParamLinTrans<T>::dim_() const
    {
      return m_cf->dim();
    }

    template <typename T>
    IGeoObj* ParamLinTrans<T>::clone_() const
    {
      return new ParamLinTrans<T>(dynamic_cast<IParamSurface*>(m_child->clone()), m_pf->get_f().m_r,
				  m_cf->get_m(), m_pf->get_f().m_t, m_cf->get_c());
    }

    template <typename T>
    const functions::IVFunction& ParamLinTrans<T>::p_fun_() const
    {
      return *m_pf;
    }

    template <typename T>
    const functions::IVFunction* ParamLinTrans<T>::p_normals_() const
    {
      return m_npf.get();
    }

    template class ParamLinTrans<core::arith::interval>;
    template class ParamLinTrans<core::arith::mreal>;
  }
}
