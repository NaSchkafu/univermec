#ifndef __FUNCTIONHELPER_HPP__
#define __FUNCTIONHELPER_HPP__

#include <core/arithcore.hpp>

namespace functions 
{
  class IFunction;
  class IVFunction;

  /// Construcs the Jacobian \f$J_f(args)\f$
   /** 
   * Construcs the Jacobian \f$J_f(args)\f$
   * f needs to be one times differentiable (through IDerivative)
   * 
   * @param f function
   * @param args arguments
   * @param values pointer to vector for function values of f
   * @param forward use forward differentation
   * 
   * @return Jacobian matrix
   */
  template <typename T>
  typename core::arith::ArithTraits<typename T::value_type>::mat_t
  jacobian(const IVFunction &f, const T& args, T* values = nullptr, bool forward = false);

  
  /// Construcs the Hessian \f$H_f(args)\f$
  /** 
   * Construcs the Hessian \f$H_f(args)\f$
   * f needs to be two times differentiable (through IDerivative)
   * 
   * @param f function
   * @param args arguments
   * 
   * @return Hessian matrix 
   */
   template <typename T>
   typename core::arith::ArithTraits<typename T::value_type>::mat_t
   hessian(const IFunction &f, const T& args);


  /// Constructs a new function \f$g: \Rz^n \to Rz\f$ for a function f
  /*!
  ** Constructs a new function \f$g: \Rz^n \to Rz\f$ for a function f
  *  with \f$g(x) :=  f( x_{start}, \ldots, x_{end}  )\f$  
  **
  ** @param f function to wrap
  ** @param start startindex of the arguments of f
  ** @param end endindex of the argument of f
  ** @param dim dimension n of the input vector of g function
  */
  functions::IFunction* arg_wrapper(const functions::IFunction &f, unsigned int start, 
				    unsigned int end, unsigned int dim);

  /// Creates a new interval-valued constant function
  /** 
   * Creates a new interval-valued function \f$f: \Rz^n \to \Rz\f$ with 
   * \f$f(x) = c\f$.
   * 
   * @param c constant
   * @param dim dimension n
   * 
   * @return pointer to function
   */
  functions::IFunction* constant_func(const core::arith::interval &c, unsigned dim);

  /// Creates a new constant function
  /** 
   * Creates a new function \f$f: \Rz^n \to \Rz\f$ with 
   * \f$f(x) = c\f$.
   * 
   * @param c constant
   * @param dim dimension
   * 
   * @return pointer to function
   */
  functions::IFunction* constant_func(const core::arith::mreal &c, unsigned dim);


}

#endif /*__FUNCTIONHELPER_HPP__*/
