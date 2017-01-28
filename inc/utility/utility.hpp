#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <core/arithcore.hpp>

// TODO C11
#include <boost/array.hpp>

namespace utility
{
  /*!
  ** Creates a unit box for the dimension
  **
  ** @param dim dimension of the box
  **
  ** @return unit bxo in form of a interval vector
  */
  core::arith::ivector unit_box(unsigned dim);

  /*!
  ** Gets the maximum length side
  **
  ** @param box the box
  **
  ** @return side with maximum length
  */
  unsigned max_dim(const core::arith::ivector &box);

  /*!
  ** Splits an interval
  **
  ** @param iv interval to split
  ** @param point split point
  **
  ** @return array containg two interval i1,i2 with i1 union i2 = iv and i1 intersect i2 = point
  */
  boost::array<core::arith::interval, 2> split(const core::arith::interval &iv,
                                               core::arith::mreal point);

  /*!
  ** Splits a box
  **
  ** @param box box to split
  ** @param dim axis for split
  ** @param point split point
  **
  ** @return array containing the resulting boxes
  */
  boost::array<core::arith::ivector,2> split(const core::arith::ivector &box,
                                             unsigned dim, core::arith::mreal point);

//     /*!
//     ** Gets the width of the widthest component of X
//     **
//     ** @param X box
//     **
//     ** @return maximum width
//     */
//     core::arith::mreal w(const core::arith::ivector &X);

  /*!
  ** Calculates the volume of the box
  **
  ** @param X box
  **
  ** @return volume of X
  */
  core::arith::mreal box_volume(const core::arith::ivector &X);

  /*!
  ** Returns a NxN interval identity matrix
  **
  ** @param size N
  **
  ** @return identity matrix
  */
  core::arith::imatrix iv_identity(unsigned size);

  // core::arith::ivector operator|(const core::arith::ivector &v1, const core::arith::ivector &v2);

  /**
   * Length of vector v
   *
   * @param v vector
   *
   * @return length of vector
   */
  template<typename T>
  typename core::arith::ArithTraits<T>::base_t length(const T &v);

  /**
   * Normalizes a vector
   *
   * @param v vector to normalize
   *
   * @return true if normalization was successful
   */
  template<typename T>
  bool normalize(T &v);

  bool normalize(core::arith::ivector &v);

#include "utility.inl"

}


#endif /*__UTILITY_HPP__*/
