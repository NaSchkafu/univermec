#ifndef __EPSDISTANCE_HPP__
#define __EPSDISTANCE_HPP__

#include <core/arithcore.hpp>

namespace decomp 
{
  class IIVTree;  
}

namespace algorithm 
{
  class IDistObserver;
}

namespace epsdistance 
{ 
  /**
   * NORMALS_NONE: Do not use normals
   * NORMALS_CONE: Checks whether \f$x_1-x_2\f$ lies in the normal cone
   * \f$n_2\f$ and \f$x_2-x_1\f$ lies in \f$n_1\f$.
   * NORMAL_CROSS: Checks whether \f$-(n_1 \cdot n_2) \geq 0\f$ and
   * \f$0 \in n_1 \times n_2\f$.
   */
  enum normal_test_t { NORMALS_NONE = 0, NORMALS_CONE = 1, NORMALS_CROSS=2 };

  /** 
   * Performs a distance computation between two (aligned )interval tree
   * 
   * @param t1 root of tree 1
   * @param t2 root of tree 2
   * @param eps desired accurarcy
   * @param ubound known ubound, use infinity() if no ubound is known!
   * @param pre_split maximum level for uniform pre split, ignore if ubound != infinity()
   * @param ntest type of normal test to use
   * @param obs pointer to a list of observers
   * 
   * @return enclosure of the distance
   */
  core::arith::interval eps_distance(decomp::IIVTree &t1, decomp::IIVTree &t2, 
				     const core::arith::mreal &eps, core::arith::mreal ubound,
				     unsigned pre_split, normal_test_t ntest,
				     unsigned threads, const algorithm::IDistObserver *obs);  
}

#endif /*__EPSDISTANCE_HPP__*/
