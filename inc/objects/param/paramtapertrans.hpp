#ifndef __PARAMTAPERTRANS_HPP__
#define __PARAMTAPERTRANS_HPP__

#include "../iparamtrans.hpp"


namespace objects 
{
  namespace details 
  {
    /// Tapering transformation for parametric surfaces
    /**
     * 
     * 
     */
    template<typename T>
    class ParamTaperTrans : public IParamTrans
    {
    public:
      /** 
       * Ctor
       * 
       * @param child surface to transform
       * @param kx tapering factor x
       * @param ky tapering factor y
       * @param sz scaling z axis
       * 
       */
      ParamTaperTrans(IParamSurface *child, const T& kx, const T &ky, const T& sz);
      virtual ~ParamTaperTrans();

    private:
      // IParamSurface
      virtual const IParamSurface& child_() const;
      virtual unsigned d_dim_() const;
      virtual core::arith::ivector domain_() const;
      virtual const functions::IVFunction& p_fun_() const;
      virtual const functions::IVFunction* p_normals_() const;

      // IGeoObj
      virtual const functions::IFunction& cf_() const;
      virtual IGeoObj* clone_() const;
      virtual unsigned dim_() const;
      virtual const functions::IVFunction* normals_() const;

    private:
      std::unique_ptr<IParamSurface> m_child;
      std::unique_ptr<functions::IVFunction> m_pf;
      std::unique_ptr<functions::IVFunction> m_npf; 
      std::unique_ptr<functions::IFunction> m_cf;
    };
    
      
  }
}

#endif /*__PARAMTAPERTRANS_HPP__*/
