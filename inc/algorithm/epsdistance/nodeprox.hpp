#ifndef __NODEPROX_HPP__
#define __NODEPROX_HPP__

#include "decomp/iivtree.hpp"

namespace distance
{
  /** 
   * Computes the distance between two treenodes
   * 
   * @param n1 node 1
   * @param n2 node 2
   * 
   * @return 
   */
  core::arith::interval node_distance(const decomp::IIVTreeNode &n1, const decomp::IIVTreeNode &n2);
}

#endif /*__NODEPROX_HPP__*/
