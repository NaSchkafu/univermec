#include <objects/csg/standardcsgnodefactory.hpp>

#include "functions/ifunction.hpp"

#include <objects/csg/standardcsgops.hpp>
#include <objects/csg/csgnodeimpl.hpp>

namespace objects 
{
  namespace details 
  {


    ICSGLeaf* StandardCSGNodeFactory::leaf_(const objects::IGeoObj &f) const
    {
      CSGLeafImpl *n = new CSGLeafImpl(f);
      return n;
    }

    ICSGNode* StandardCSGNodeFactory::invert_(ICSGNode *n) const 
    {
      // CSGNodeImpl *n1 = new CSGNodeImpl(1,1);
      // n1->set_cf(new functions::details::FBase<CSGStandardInversion>(new CSGStandardInversion(*n)));
      // n1->add(n);
      // return n1;
      assert(!"Not impl");
      return 0;
    }
    
    ICSGNode* StandardCSGNodeFactory::unite_(ICSGNode *n1, ICSGNode *n2) const
    {
      assert(!"Not impl!");
	  return 0;
    }
    
    ICSGNode* StandardCSGNodeFactory::intersect_(ICSGNode *n1, ICSGNode *n2) const 
    {
      assert(!"Not impl!");  
	  return 0;
    }
    
    
    ICSGTransform* StandardCSGNodeFactory::transform_(ICSGNode *n, const core::arith::rmatrix &m,
						      const core::arith::rmatrix &im, const core::arith::rvector &t,
						      const core::arith::rvector &it) const
    {
      
      CSGTransformImpl<core::arith::mreal> *n1 = new CSGTransformImpl<core::arith::mreal>(1,1);
      n1->add(n);
      n1->set_t_s(m, im, t, it);
      return n1;
    }
    
    ICSGTransform* StandardCSGNodeFactory::transform_(ICSGNode *n, const core::arith::imatrix &m, 
						      const core::arith::imatrix &im, const core::arith::ivector &t, 
						      const core::arith::ivector &it) const    
    {
      // Hier fehlt set_in_cf_
      CSGTransformImpl<core::arith::interval> *n1 = new CSGTransformImpl<core::arith::interval>(1,1);
      n1->add(n);
      n1->set_t_s(m, im , t, it);
      return n1;
    }

  }
}
