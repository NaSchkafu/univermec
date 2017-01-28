#include <objects/param/paramleaf.hpp>

namespace objects 
{
  namespace details
  {
    ParamLeaf::ParamLeaf(const IParamSurface &leaf)
      :m_leaf(leaf)
    {}

    const IParamSurface& ParamLeaf::child_() const
    {
      return m_leaf;
    }

    unsigned ParamLeaf::d_dim_() const
    {
      return m_leaf.d_dim();
    }

    core::arith::ivector ParamLeaf::domain_() const
    {
      return m_leaf.domain();
    }

    const functions::IVFunction& ParamLeaf::p_fun_() const
    {
      return m_leaf.p_fun();
    }
      // IGeoObj
    const functions::IFunction& ParamLeaf::cf_() const
    {
      return m_leaf.cf();
    }

    IGeoObj* ParamLeaf::clone_() const
    {
      return new ParamLeaf(m_leaf);
    }

    unsigned ParamLeaf::dim_() const
    {
      return m_leaf.dim();
    }

    const functions::IVFunction* ParamLeaf::p_normals_() const
    {
      return m_leaf.p_normals();
    }

    const functions::IVFunction* ParamLeaf::normals_() const
    {
      return m_leaf.normals();
    }

  }
}
