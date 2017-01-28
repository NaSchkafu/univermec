#ifndef __PARAMLEAF_HPP__
#define __PARAMLEAF_HPP__

#include "../iparamtrans.hpp"

namespace objects
{
  namespace details
  {
    /// This is a wrapper for parametric surfaces that act as leafs in a transformation tree
    class ParamLeaf : public IParamTrans
    {
    public:
      explicit ParamLeaf(const IParamSurface &leaf);
      virtual ~ParamLeaf() {}

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
      const IParamSurface& m_leaf;
    };
  }
}

#endif /*__PARAMLEAF_HPP__*/
