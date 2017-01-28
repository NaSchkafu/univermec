#ifndef __ICSGNODEFACTORY_HPP__
#define __ICSGNODEFACTORY_HPP__

#include <core/arithcore.hpp>

namespace objects
{

  class ICSGNode;
  class ICSGTransform;
  class ICSGLeaf;
  class ICSGPolyLeaf;
  template<class X>
  class INonConvexPolyhedron;
  class IGeoObj;

  /// interface for CSG node factories
  class ICSGNodeFactory
  {
  public:
    virtual ~ICSGNodeFactory() = default;

    /*!
    ** Creates a new leaf node for a geometric object
    **
    ** @param obj the leaf
    **
    ** @return new CSG node representing the leadf
    */
    ICSGLeaf* leaf(const IGeoObj &leaf) const
      {
        return leaf_(leaf);
      }

    /*!
    ** Creates an Inversion CSG node
    **
    ** @param n node to invert
    **
    ** @return CSG node representing the inversion node
    */
    ICSGNode* invert(ICSGNode *n) const
      {
        return invert_(n);
      }

    /*!
    ** Creates an Union CSG Node
    **
    ** @param n1 node 1
    ** @param n2 node 2
    **
    ** @return union node for n1 and n2
    */
    ICSGNode* unite(ICSGNode *n1, ICSGNode *n2) const
      {
        return unite_(n1, n2);
      }

    /*!
    ** Creates an Intersection CSG node
    **
    ** @param n1 node 1
    ** @param n2 node 2
    **
    ** @return intersection node for n1 and n2
    */
    ICSGNode* intersect(ICSGNode *n1, ICSGNode *n2) const
      {
        return intersect_(n1, n2);
      }

    /*!
    ** Create a transformation CSG node
    **
    ** @param n node
    ** @param m transformation matrix
    **
    ** @return transformation node for n
    */
    ICSGTransform* transform(ICSGNode *n, const core::arith::imatrix &m, 
			     const core::arith::imatrix &im, const core::arith::ivector &t, 
			     const core::arith::ivector &it) const
      {
        return transform_(n, m, im, t, it);
      }

    ICSGTransform* transform(ICSGNode *n, const core::arith::rmatrix &m,  
			     const core::arith::rmatrix &im, const core::arith::rvector &t, 
			     const core::arith::rvector &it) const
      {
        return transform_(n, m, im, t, it);
      }

  private:
    virtual ICSGLeaf* leaf_(const objects::IGeoObj &obj) const = 0;
    virtual ICSGNode* invert_(ICSGNode *n) const = 0;
    virtual ICSGNode* unite_(ICSGNode *n1, ICSGNode *n2) const = 0;
    virtual ICSGNode* intersect_(ICSGNode *n1, ICSGNode *n2) const = 0;
    virtual ICSGTransform* transform_(ICSGNode *n, const core::arith::imatrix &m, 
				const core::arith::imatrix &im, const core::arith::ivector &t, 
				const core::arith::ivector &it) const = 0;
    virtual ICSGTransform* transform_(ICSGNode *n, const core::arith::rmatrix &m,
                                const core::arith::rmatrix &im, const core::arith::rvector &t,
                                const core::arith::rvector &it) const = 0;
  };
}

#endif /*__ICSGNODEFACTORY_HPP__*/

