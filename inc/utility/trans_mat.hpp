/*!
**@file   trans_mat.hpp
**@author Stefan Kiel
**@date   Mon Jul  5 11:37:44 2010
**
**@brief  Tools for creating transformation matrices
**
**
*/

#ifndef __TRANS_MAT_HPP__
#define __TRANS_MAT_HPP__

#include <core/arithcore.hpp>

namespace utility 
{

  /*! 
  ** Generic template for creating transformation matrices using all supported
  *  arithmetics. The returned transformation matrix is in homogenous coordinates.
  ** 
  ** @param axis rotation axis
  ** @param angle rotation angle
  ** @param translation translation vector
  ** 
  ** @return transformation matrix
  */
  template<typename T>
  typename core::arith::ArithTraits<T>::mat_t trans_mat(
    const typename core::arith::ArithTraits<T>::vec_t &axis, const T& angle, 
    const typename core::arith::ArithTraits<T>::vec_t &translation);
  
  // overload for reals
  core::arith::rmatrix trans_mat(core::arith::rvector axis, core::arith::mreal angle, 
				 const core::arith::rvector &translation);

  /** 
   * Generic template for creating a rotation matrix using all supported arithmetics.
   * The returned matrix is a 3x3 matrix.
   * 
   * @param axis rotation axis
   * @param angle rotation angle
   * 
   * @return 
   */
  template<typename T>
  typename core::arith::ArithTraits<T>::mat_t trans_mat(
    const typename core::arith::ArithTraits<T>::vec_t &axis, const T& angle);
  
  core::arith::rmatrix trans_mat(core::arith::rvector axis, core::arith::mreal angle);

  /** 
   * Generic template for creating a 4x4 transformation matrix from a 3x3 rotation matrix and
   * a translation axis
   * 
   * @param rot rotation matrix
   * @param trans translation axis
   * 
   * @return 4x4 transformation matrix in homogenous coordinates
   */
  template<typename T>
  T trans_mat(const T& rot, const typename 
	      core::arith::ArithTraits<typename core::arith::ArithTraits<T>::base_t>::vec_t &trans);

#include "trans_mat.inl"
}


#endif /*__TRANS_MAT_HPP__*/
