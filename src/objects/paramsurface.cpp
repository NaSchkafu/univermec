#include <objects/paramsurface.hpp>

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"

#include "objects/details/implnormals.hpp"
#include "objects/details/paramnormals.hpp"

namespace objects 
{
  ParamSurface::ParamSurface(const functions::IVFunction &param, const functions::IFunction &cf, 
	       const core::arith::ivector &domain)
    :m_param(param), m_pnf(details::p_normal_fun(param)), m_cf(cf), m_nf(details::normal_fun(cf)),
     m_domain(domain)
  {}

  ParamSurface::ParamSurface(const functions::IVFunction &param, const functions::IVFunction &pnf,
			     const functions::IFunction &cf, const core::arith::ivector &domain)
    :m_param(param), m_pnf(&pnf), m_cf(cf), m_nf(details::normal_fun(cf)),
     m_domain(domain)
  {  }

  ParamSurface::~ParamSurface() {}

  // IGeoObj
  const functions::IFunction& ParamSurface::cf_() const
  {
    return m_cf;
  }
  
  IGeoObj* ParamSurface::clone_() const
  {
    return new ParamSurface(m_param, m_cf, m_domain);
  }
  
  unsigned ParamSurface::dim_() const
  {
    return m_cf.dim();
  }

  const functions::IVFunction* ParamSurface::normals_() const
  {
    return m_nf.get();
    //return m_cf.normals();
  }

  // IParamSurface
  unsigned ParamSurface::d_dim_() const
  {
    return m_param.dim();
  }
  
  core::arith::ivector ParamSurface::domain_() const
  {
    return m_domain;
  }
  
  const functions::IVFunction& ParamSurface::p_fun_() const
  {
    return m_param;
  }

  const functions::IVFunction* ParamSurface::p_normals_() const
  {
    return m_pnf;
  }
}
