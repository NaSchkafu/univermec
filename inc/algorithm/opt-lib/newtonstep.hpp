/**
 * @file   newtonstep.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:27:43 2009
 *
 * @brief  Header for multidim. newton step
 *
 *
 */


#ifndef __NEWTONSTEP_HPP__
#define __NEWTONSTEP_HPP__ __NEWTONSTEP_HPP__

namespace functions 
{
  class IFunction;
}


#include <core/arithcore.hpp>
#include "types.hpp"

namespace GlobOpt {

/*!
** Performs one step of the newton iteration
** Uses the Gauss-Seidel iteration if the matrix is irregular or a hull method
** if we have a regular matrix
**
** @param box box for evaluation
** @param functions vector containing the function set (i.e. forming a
** R^n->R^n function)
** @param empty flag which is set if the resulting box is empty
** @param gaps this vector is filled with gaps which occured inside box
** @param nwt pointer to an ivector, where the result of the Newton-Opterator
*  should stored
*
** @return new box
*/
  core::arith::ivector newtonStep(const core::arith::ivector &box, const core::arith::rvector &expPoint,
				  const FLst &functions, bool &empty,
				  std::vector<Gap> &gaps, core::arith::ivector *nwt = 0);

/*!
** Performs one step of the newton iteration
** Uses the newton iteration if the matrix is irregular or a hull method
** if we have a regular matrix
**
** @param box box for evaluation
** @param functions vector containing the function set (i.e. forming a
** R^n->R^n function)
** @param empty flag which is set if the resulting box is empty
** @param gaps this vector is filled with gaps which occured inside box
** @param nwt pointer to an ivector, where the result of the Newton-Opterator
*  should stored
*
** @return new box
*/
  // core::arith::ivector newtonStep(const core::arith::ivector &box, const core::arith::rvector &expPoint,
  // 				  const FLst &functions, bool &empty,
  // 				  std::vector<Gap> &gaps, core::arith::ivector *nwt = 0);
}

#endif
