/**
 * @file   linesearch.hpp
 * @author Stefan Kiel <fluxy@lucy>
 * @date   Mon May 11 14:26:26 2009
 * 
 * @brief  Header of linesearch procedure
 * 
 * 
 */


#ifndef __LINESEARCH_HPP__
#define __LINESEARCH_HPP__ __LINESEARCH_HPP__

#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
}

namespace GlobOpt 
{
  class OptBox;

  /** 
   * Performs a linesearch for finding a point with a low upperbound on the global
   * minimum
   * 
   * @param b box for the search 
   * @param initBox the initial box (used by starting the algorithm)
   * @param F objective function
   * @param ubPoint this vector is set to the found point
   * @param proximityQuery indicates if the modified linesearch for proximity queries should be used
   * 
   * @return true if a linesearch was successful and ubPoint has been set, otherwise false
   */
  bool lineSearch(const OptBox &b, const core::arith::ivector &initBox, 
		  const functions::IFunction &F, core::arith::rvector &ubPoint, bool proximityQuery);
}


#endif
