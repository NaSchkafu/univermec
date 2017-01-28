/**
 * @file   general.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:15:17 2009
 * 
 * @brief  Header for general helper routines
 * 
 * 
 */


#ifndef __GENERAL_HPP__
#define __GENERAL_HPP__ __GENERAL_HPP__

#include <core/arithcore.hpp>

#include "globalcriteria.hpp"
#include "helper.hpp"

namespace GlobOpt {
  /*!
  ** Create an argument vector for the ith grad function according to the scheme
  ** which is used in the multidim. taylor expansion of a function
  ** This scheme allows that some of the parameters can be evaluated as real number,
  ** thus getting a sharper bound
  **
  **
  ** @param x expansion point of the function
  ** @param box box in which is x contained
  ** @param i index of gradient function
  **
  ** @return argument for the i-th gradient function
  */
  core::arith::ivector createGradArg(const core::arith::rvector &x, const core::arith::ivector &box, int i);

  /// functional object for deleting pointers
  struct delFunc {
    template<class T>
    void operator()(T *p) { if(p) delete p; }
    template<class T>
    void operator()(T **p) { if(p && *p) delete *p; p = 0; }
  };

  /*!
  ** Calculates the interval jacobian for the given box and the given point
  **
  ** @param box box containing the point
  ** @param x point
  ** @param functions list of functions (note that all functions must
  ** have the same dimension)
  ** Note: The template parameter should only used for vectors of
  ** C1Function(s) or C2Function(s)
  ** @param setZero set to true if you want to treat very small
  ** intervals as zero (according to the S_GC class)
  ** @param err flag that indicates a special value in the matrix (Inf, NaN), if this flag is set
  * initially the function continues after an occurence of a special value
  ** @return interval jacobian
  */
  template<class CXLST>
  core::arith::imatrix jacobian(const core::arith::ivector &box, const core::arith::rvector &x,
				const CXLST &functions, bool setZero, bool &err);
  
  /*!
** Evaluate the function vector
**
** @param functions the vector of functions
** @param box box for evaulation
**
** (Note: only use with vectors of Function/C1Function/C2Function and
** core::arith::ivector/core::arith::rvector as template types)
**
** @return box containing the function evaluation
*/
  template<class FLST, class VECT>
  core::arith::ivector evalFunctions(const FLST &functions, const VECT &box);

  /** 
   * Checks if a box feasible
   * 
   * @param b box
   * @param constraints constraints for the box
   * 
   * @return true if b is feasible
   */
  template<class FLST, class B>
  bool isFeasible(const B &b, const FLST &constraints);
  
  
  #include "general.inl"

}

#endif
