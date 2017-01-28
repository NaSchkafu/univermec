/**
 * @file   helper.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:23:32 2009
 *
 * @brief  Header for helper functions
 *
 *
 */


#ifndef __HELPER_HPP__
#define __HELPER_HPP__ __HELPER_HPP__

#include <core/arithcore.hpp>
#include <vector>

/// namespace for Math Tools
namespace MT {
  /*!
  ** Return the width of the given interval
  **
  ** @param X interval
  **
  ** @return width of the interval
  */
  //inline core::arith::mreal w(const core::arith::interval &X) { return core::arith::sup(X) - core::arith::inf(X); }

  /*!
  ** Return the middle of a given interval
  **
  ** @param X interval
  **
  ** @return midpoint of X
  */
  inline core::arith::mreal m(const core::arith::interval &X) { return (core::arith::inf(X) + core::arith::sup(X))/2; }

  /*!
  ** Split a interval in half
  **
  ** @param X interval to split
  ** @param P point where to split
  ** Note: The result is not defined if P is not element of X
  **
  ** @return vector containing two intervals
  */
  std::vector<core::arith::interval> split(const core::arith::interval &X, const core::arith::mreal &P);

  /*!
  ** Convience method for splitting an interval in half
  **
  ** @param X interval to split
  **
  ** @return vector containing two intervals
  */
  inline std::vector<core::arith::interval> split(const core::arith::interval &X)
  {
    return split(X, mid(X));
  }

  /*!
  ** Splits an ivector (box) along a given axis in two halves
  **
  ** @param v the box
  ** @param direction the axis
  ** @param first true for the first new box, false for the second box
  **
  ** @return the new box according to the first flag
  */
  core::arith::ivector split(const core::arith::ivector &v, int direction, bool first);

  /*!
  ** Splits an ivector (box) along a given axis and a given point on this axis
  **
  ** @param v the box
  ** @param p point on the axis
  ** @param direction the axis
  ** @param first true for the first new box, false for the second box
  **
  ** @return the new box according to the first flag
  */
  core::arith::ivector split(const core::arith::ivector &v, const core::arith::mreal &p, int direction, bool first);

  /*!
  ** Returns the maximal diameter of this box
  **
  ** @param v the box
  **
  ** @return max diameter (width of the box)
  */
  core::arith::mreal w(const core::arith::ivector &v);

  /*!
  ** Returns the index of widthest dimension of a box
  **
  ** @param v the box
  **
  ** @return index i of max(w(X[i])) or -1 if all boxes have width 0
  */
  int maxDim(const core::arith::ivector &v);

  /*!
  ** Solves U + V*t <= 0
  ** A arbitrary inequality can be brought in this form by using a taylor
  ** expansion (see Hansen 6.2)
  **
  ** @param U interval U
  ** @param V interval V
  **
  ** @return vector containg one or two intervals
  */
  std::vector<core::arith::interval> solveInEq(const core::arith::interval& U, const core::arith::interval &V);

  /*!
  ** Returns an interval which contains X1 and X2 and also the gap which existed between X1/2
  **
  ** @param X1 interval 1
  ** @param X2 interval 2
  **
  ** @return interval containing both and the gap between
  */
  core::arith::interval unite(const core::arith::interval &X1, const core::arith::interval &X2);

  /*!
  ** Checks if the i-th row of the matrix m is zero
  **
  ** @param m the matrix
  ** @param i row number
  **
  ** @return true if all entries are zero
  */
  bool isRowZero(const core::arith::rmatrix &m, int i);

  /*!
  ** Swaps the rows
  **
  ** @param m the matrix
  ** @param row1 row 1
  ** @param row2 row 2
  */
  void swapRows(core::arith::rmatrix&m, int row1, int row2);

  /*!
  ** Creates a new vector object of the same type and copies the data from the source
  **
  ** @param source source vector
  ** @param from starting index (defaults to 1)
  ** @param to stopping index
  **
  ** @return new vector v with Lb(v) = 1 and Ub(v) = to - from + 1
  */
  template<class T>
  T copyVec(const T& source, unsigned int to, unsigned int from = 1);

  /*!
  ** Calculates the length of a vector (||2)
  **
  ** @param v (cxsc) vector
  **
  ** @return interval enclosure of the length
  */
  template<class T>
  core::arith::interval iLength(const T &v);

  /*!
  ** Calculates the length of a vector (||2)
  **
  ** @param v (cxsc) vector
  **
  ** @return (rounded) length
  */
  template<class T>
  core::arith::interval rLength(const T &v);

  /*!
  ** Checks if the point p lies inside the box b
  ** NOTE: the dimensions of p and b must match
  ** This functions matches regardless of Lb/Ub
  **
  ** @param p the point
  ** @param b the box
  **
  ** @return true if p is inside p
  */
  bool inside(const core::arith::rvector &p, const core::arith::ivector &box);

  /*!
  ** Casts a double array to a core::arith::rvector
  **
  ** @param n double array
  ** @param size number of array entries
  **
  ** @return core::arith::rvector
  */
  inline core::arith::rvector doubleToVec(const double *n, unsigned int size);

  /*!
  ** Returns the sum of the component widths
  **
  ** @param x box
  **
  ** @return sum
  */
  core::arith::mreal totalWidth(const core::arith::ivector &x);

  /*!
  ** Checks if the vector is empty
  **
  ** @param v vector
  **
  ** @return true if IsEmpty is true for any component of v, false otherwise
  */
  bool IsEmpty(const core::arith::ivector &v);

  /*!
  ** Checks if two intervals overlap
  **
  ** @param i1 interval 1
  ** @param i2 interval 2
  **
  ** @return true if i1 and i2 overlap, false otherwise
  */
  bool overlap(const core::arith::interval &i1, const core::arith::interval &i2);

  core::arith::imatrix iIdentity (int size);

  bool isSpecialValue(const core::arith::interval &i);


  #include "helper.inl"


}
#endif
