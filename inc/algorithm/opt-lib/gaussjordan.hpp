/**
 * @file   gaussjordan.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:13:28 2009
 * 
 * @brief  Header for Gauss-Jordan-Algorithm
 * 
 * 
 */


#ifndef __GAUSJORDAN_HPP__
#define __GAUSJORDAN_HPP__ __GAUSJORDAN_HPP__

#include <core/arithcore.hpp>

namespace GlobOpt {

 /*!
 ** Performs the Gauss-Jordan algorithm
 ** (LHS*X = RHS is solved)
 **
 ** @param lhs left hand side
 ** @param rhs right hand side
 **
 ** After successful completion the lhs should contain the identity
 ** matrix and rhs the solution of the system (=X)
 ** Note: You can use this algorithm for calculating the inverse of a
 ** non singular matrix. Just use the identity matrix as starting
 ** argument for rhs.
 **
 ** @return true if successful
 */
  bool gjelim(core::arith::rmatrix &mat, core::arith::rmatrix &rhs);

  /*!
  ** Calculates the inverse matrix of m
  **
  ** @param m matrix
  ** @param sucess flag, set to true if the matrix m is regular, false
  ** if m is singular
  **
  ** @return if sucess is true the inverse m^-1, otherwise a matrix
  ** which can be used for getting a diagonal dominant system (for
  ** preconditioning, See Hansen 5.11)
  */
  core::arith::rmatrix invertMatrix(const core::arith::rmatrix &m, bool &sucess);

}
#endif
