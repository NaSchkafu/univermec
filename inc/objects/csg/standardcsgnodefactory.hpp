#ifndef __STANDARDCSGFACTORY_HPP__
#define __STANDARDCSGFACTORY_HPP__

#include "../icsgnodefactory.hpp"

namespace objects 
{
  namespace details 
  {
    class StandardCSGNodeFactory : public ICSGNodeFactory
    {
    private:
      virtual ICSGLeaf* leaf_(const objects::IGeoObj &n) const;
      virtual ICSGNode* invert_(ICSGNode *n) const;
      virtual ICSGNode* unite_(ICSGNode *n1, ICSGNode *n2) const;
      virtual ICSGNode* intersect_(ICSGNode *n1, ICSGNode *n2) const;
      virtual ICSGTransform* transform_(ICSGNode *n, const core::arith::imatrix &m, 
					const core::arith::imatrix &im, const core::arith::ivector &t, 
					const core::arith::ivector &it) const;
      virtual ICSGTransform* transform_(ICSGNode *n, const core::arith::rmatrix &m,
					const core::arith::rmatrix &im, const core::arith::rvector &t,
					const core::arith::rvector &it) const;
      // virtual ICSGPolyLeaf* poly_leaf_(objects::INonConvexPolyhedron<core::arith::mreal> *poly) const;
      // virtual ICSGPolyLeaf* poly_leaf_(objects::INonConvexPolyhedron<core::arith::interval> *poly) const;
    };
  }
}

#endif /*__STANDARDCSGFACTORY_HPP__*/
