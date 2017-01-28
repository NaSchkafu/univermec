#ifndef __NEWTON_HPP__
#define __NEWTON_HPP__

#include <vector>
#include <core/arithcore.hpp>

namespace functions {
  namespace contractors
  {
    namespace details
    {
      /** 
       ** Performs a newton step
      ** (Precondition is: F has no root at xn if F' has a root in x)
       * 
       * @param xn expansion point
       * @param fxn function at xn
       * @param dfx gradient at x
       * 
       * @return 
       */
      std::vector<core::arith::interval> stepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fxn,
						   const core::arith::interval &dfxn, const core::arith::interval &x);

      /*!
      ** Performs a simple newton step
      ** (Precondition is: F' has no roots in x)
      *
      ** @param x interval
      ** @param xn expansion point of the newton step
      ** @param F function
      ** @param intersect true for intersecting the result with x
      **
      ** @return (hopefully) reduced intervals
      */
      core::arith::interval simpleStepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fxn,
					    const core::arith::interval &dfx);

      /*!
      ** Performs a newton step
      ** (Precondition is: F' has at least one root in x and F has no
      ** root at xn)
      **
      ** @param x interval
      ** @param xn expansion point
      ** @param F function
      *  @param intersect true for intersecting the result with x
      **
      ** @return list with (hopefully) reduced intervals
      */
      std::vector<core::arith::interval> extStepNwt1D(const core::arith::mreal &xn, const core::arith::interval &fxn,
						      const core::arith::interval &dfxn, const core::arith::interval &x);
    }
  }
}


#endif /*__NEWTON_HPP__*/

