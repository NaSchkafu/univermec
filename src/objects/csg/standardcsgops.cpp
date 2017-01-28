#include <objects/csg/standardcsgops.hpp>

namespace objects
{
  namespace details
  {
    CSGStandardInversion::CSGStandardInversion(const ICSGNode &node)
      :m_node(node)
    {
    }

    unsigned CSGStandardInversion::dim() const
    {
      return m_node.dim();
    }

    std::string CSGStandardInversion::text() const
    {
      std::stringstream sstream;
      sstream << "StandardCSG Inversion Node";
      return sstream.str();
    }

    CSGStandardUnion::CSGStandardUnion(const ICSGNode &node1, const ICSGNode &node2)
      :m_node1(node1), m_node2(node2)
    {
      //TODO: Exception!
      assert(m_node1.dim() == m_node2.dim() && "Dimensions of nodes are not matching in CSG Union");
    }

    unsigned CSGStandardUnion::dim() const
    {
      return m_node1.dim();
    }

    std::string CSGStandardUnion::text() const
    {
      std::stringstream sstream;
      sstream << "CSG Union Node";
      return sstream.str();
    }

    // core::arith::rmatrix CSGStandardTransform<core::arith::mreal>::rot() const
    // {
    //   return m_m;
    // }

    // core::arith::rmatrix CSGStandardTransform<core::arith::mreal>::i_rot() const
    // {
    //   return m_im;
    // }

    // core::arith::rvector CSGStandardTransform<core::arith::mreal>::trans() const
    // {
    //   return m_t;
    // }


    // core::arith::rvector CSGStandardTransform<core::arith::mreal>::i_trans() const
    // {
    //   return m_it;
    // }

    // std::string CSGStandardTransform<core::arith::mreal>::text() const
    // {
    //   std::stringstream sstream;
    //   sstream << "StandardCSG Transformation Node with Real Transformation Matrix ";
    //   sstream << m_m;
    //   return sstream.str();
    // }

    // CSGStandardTransform<core::arith::mreal>::CSGStandardTransform(
    //   const ICSGNode &n, const core::arith::rmatrix &m, const core::arith::rmatrix &im,
    //   const core::arith::rvector &t, const core::arith::rvector &it)
    //   :m_node(n), m_m(m), m_im(im), m_t(t), m_it(it)
    // {
    // }

    // unsigned CSGStandardTransform<core::arith::mreal>::dim() const
    // {
    //   return m_node.dim();
    // }

    core::arith::mreal CSGStandardIntersection::operator()(const core::arith::rvector &) const
    {
      assert(!"Set theoretic CSG operations cannot be evaluated using a real type!");
      // TODO: Exception Handling
      return 0;
    }

    core::arith::mreal CSGStandardUnion::operator()(const core::arith::rvector &) const
    {
      assert(!"Set theoretic CSG operations cannot be evaluated using a real type!");
      // TODO: Exception Handling
      return 0;
    }
  }
}
