#ifndef __PARAMBENDTRANS_HPP__
#define __PARAMBENDTRANS_HPP__

#include "../iparamtrans.hpp"


namespace objects 
{
  namespace details 
  {
    /// Bending transformation for parametric surfaces
    /**
     * 
     * 
     */
    template<typename T>
    class ParamBendTrans : public IParamTrans
    {
    public:
      /** 
       * Ctor
       * 
       * @param child surface to transform
       * @param k curvature of circular section
       * @param alpha bending angle
       * 
       */
      ParamBendTrans(IParamSurface *child, const T& k, const T &alpha);
      virtual ~ParamBendTrans();

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

#endif /*__PARAMBENDTRANS_HPP__*/
