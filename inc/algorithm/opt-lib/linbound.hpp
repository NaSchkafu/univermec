/*!
**@file   linbound.hpp
**@author Stefan Kiel
**@date   Wed Feb 11 21:25:17 2009
**
**@brief  Header that Provides a method for linearizing a C1Function F with a taylor expansion
** and solving the inequality F(x) <= value for a box x
**
**
*/


#ifndef __LINBOUND_HPP__
#define __LINBOUND_HPP__ __LINBOUND_HPP__

#include <core/arithcore.hpp>
#include "types.hpp"

namespace functions
{
  class IFunction;  
}

namespace GlobOpt {

  /*!
  ** Tries to reduce a box with the inequality F(box) <= uBound
  ** using linearized function based on a Taylor expansion of the original
  ** function
  **
  ** @param x expansion point for taylor
  ** @param box the box (which is eventually reduced)
  ** @param gaps vector for saving found gaps
  ** @param F the function
  *  @param uBound  upper bound
  ** @return true if the box has been deleted, otherwise false
  */
  bool linBound(const core::arith::rvector &x, core::arith::ivector &box, std::vector<Gap> &gaps,
                const functions::IFunction &F, const core::arith::mreal &uBound);
}

#endif
