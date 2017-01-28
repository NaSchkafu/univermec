#ifndef __PARAMSURFACE_HPP__
#define __PARAMSURFACE_HPP__

#include "iparamsurface.hpp"

namespace objects 
{
  class ParamSurface : public IParamSurface
  {
  public:
    /** 
     * Ctor
     * 
     * @param param parametric function
     * @param cf implicit function
     * @param domain parametric domain
     */
    ParamSurface(const functions::IVFunction &param, const functions::IFunction &cf, 
		 const core::arith::ivector &domain);

    /** 
     * Ctor
     * 
     * @param param parametric function
       @param pnf parametric normal function (FIXME: ownership transfer!)
     * @param cf implicit function
     * @param domain parametric domain
     * 
     */
    ParamSurface(const functions::IVFunction &param, const functions::IVFunction &pnf,
		 const functions::IFunction &cf, const core::arith::ivector &domain);

    virtual ~ParamSurface();

  private:
    // IGeoObj
    virtual const functions::IFunction& cf_() const;
    virtual IGeoObj* clone_() const;
    virtual unsigned dim_() const;
    virtual const functions::IVFunction* normals_() const;

    // IParamSurface
    virtual unsigned d_dim_() const;
    virtual core::arith::ivector domain_() const;
    virtual const functions::IVFunction& p_fun_() const;
    virtual const functions::IVFunction* p_normals_() const;

  private:
    /// Parametric functions
    const functions::IVFunction& m_param;
    const functions::IVFunction* m_pnf;
    /// Characteristic (Implicit Function)
    const functions::IFunction& m_cf;
    std::unique_ptr<functions::IVFunction> m_nf;
    /// parameter domain
    core::arith::ivector m_domain;
  };
}

#endif /*__PARAMSURFACE_HPP__*/
