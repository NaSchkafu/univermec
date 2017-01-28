/**
 * @file   iderivative.hpp
 * @author Stefan Kiel <kiel@inf.uni-due.de>
 * @date   Tue Jul 20 13:37:36 2010
 * 
 * @brief  
 * 
 * 
 */

#ifndef __IDERIVATIVE_HPP__
#define __IDERIVATIVE_HPP__

#include <core/arithcore.hpp>
#include "islope.hpp"

namespace functions
{
  class IFunction;
  class IVFunction;

  /// Interface for representing derivatives of a functions
  /**
   * Derivatives of a [function](@ref IVFunction) are represented
   * inside the framework by adding this interface to the [feature
   * set](@ref IFFeatures) of the function.  
   * 
   * For a function \f$f: \Rz^n \to \Rz^m\f$ IDerivative provides a
   * IVFunction instance representing each directional gradient member
   * of f through the @ref grad member function. Additionally, each
   * row of the Jacobian \f$J_f\f$ can be retrieved in form of an IVFunction
   * representation by the @ref grad_fun member. Because the Jacobian can not
   * be represented with IVFunction since it supports only vectror-valued functions,
   * IDerivative allows to evaluate an inclusion function for \f$J_f\f$ for each
   * supported arithmetic by using the @ref jacobian member. 
   *
   * Since the returned IVFunction instances can have a feature set of their own, they
   * can also provide access to IDerivative. In this way, higher order derivatives can
   * be represented inside the framework.
   * 
   * Example:
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * IFunction* f = functions::parser::parse_string("pow(x0,3)+x1");
   * core::arith::imatrix jacobian(f->features().derivative()->jacobian(i_args)); // Interval Jacobian of f
   * core::arith::amatrix hessian(f->features().derivative()->grad_fun().features().derivative()->jacobian(a_args)); // Affine Hessian of f
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   * Because the IVFunction instances returned by IDerivative model full-fledged function
   * representation objects, it is even possible to use different arithmetics for derivative
   * evaluation as shown in example.
   * 
   * For details refer to \cite thesis Sect. 4.4. 
   */
  class IDerivative : public ISlope 
  {
  public:
    /** 
     * Gets the directional derivative
     * This is the derivative in direction dir of the member function mem
     * 
     * @param dir derivative direction
     * @param mem member function
     * 
     * @return reference to IFunction representing the derivative
     */
    const IFunction& grad(unsigned dir, unsigned mem = 0) const 
      {
	return grad_(dir, mem);
      }

    /** 
     * Evaluates the gradient of a member function, returning function value and all gradient values
     * 
     * @param args box for evaluation
     * @param member member function of the original f
     * 
     * @return vector containg all gradient values, the first entry contains the function value
     */
    inline core::arith::ivector grad(const core::arith::ivector &args, unsigned member = 0) const
      {
	return grad_(args, member);
      }

    /** 
     * Evaluates the gradient of a member function, returning function value and all gradient values
     * 
     * @param args affine vector for evaluation
     * @param member member function of the original f
     * 
     * @return vector containg all gradient values, the first entry contains the function value
     */
    inline core::arith::avector grad(const core::arith::avector &args, unsigned member = 0) const
      {
	return grad_(args, member);
      }

    /** 
     * Evaluates the gradient of a member function, returning function value and all gradient values
     * 
     * @param args Taylor model vector for evaluation
     * @param member member function of the original f
     * 
     * @return vector containg all gradient values, the first entry contains the function value
     */
    inline core::arith::tvector grad(const core::arith::tvector &args, unsigned member = 0) const
      {
	return grad_(args, member);
      }

    /** 
     * Evaluates the gradient of a member function, returning function value and all gradient values
     * 
     * @param args point for evaluation
     * @param member member function of the original f
     * 
     * @return vector containg all gradient values, the first entry contains the function value
     */
    inline core::arith::rvector grad(const core::arith::rvector &args, unsigned member = 0) const
      {
	return grad_(args, member);
      }


    /** 
     * Evaluates the Jacobian of the function
     * (The first column of the Jacobian are the values of the
     * original function!)
     * 
     * @param args arguments
     * 
     * @return Jacobian matrix
     */
    inline core::arith::rmatrix jacobian(const core::arith::rvector &args) const
      {
	return jacobian_(args);
      }

    /** 
     * Evaluates the Jacobian of the function
     * (The first column of the Jacobian are the values of the
     * original function!)
     * 
     * @param args arguments
     * 
     * @return Jacobian matrix
     */
    inline core::arith::imatrix jacobian(const core::arith::ivector &args) const
      {
	return jacobian_(args);	
      }

    /** 
     * Evaluates the Jacobian of the function
     * (The first column of the Jacobian are the values of the
     * original function!)
     * 
     * @param args arguments
     * 
     * @return Jacobian matrix
     */
    inline core::arith::amatrix jacobian(const core::arith::avector &args) const
      {
	return jacobian_(args);	
      }

    /** 
     * Evaluates the Jacobian of the function
     * (The first column of the Jacobian are the values of the
     * original function!)     
     * 
     * @param args arguments
     * 
     * @return Jacobian matrix
     */
    inline core::arith::tmatrix jacobian(const core::arith::tvector &args) const
      {
	return jacobian_(args);	
      }

    /** 
     * Returns the gradient function of a member
     * The gradient function of the i-th member of a function f: R^n
     * \to R^m is a function g: R^n \to R^n+1. 
     * 
     * The 0-th member of g is the i-th member of f.
     * The first to n-th members are the directional derivatives of the
     * i-th member of f. 
     * 
     * @param member number of member
     * 
     * @return gradient function
     */
    inline const IVFunction& grad_fun(unsigned member = 0) const
      {
	return grad_fun_(member);
      }

    virtual ~IDerivative()
      {}

  private:
    virtual const IFunction& grad_(unsigned direction, unsigned member) const = 0;
    virtual core::arith::ivector grad_(const core::arith::ivector &args, unsigned member) const = 0;
    virtual core::arith::avector grad_(const core::arith::avector &args, unsigned member) const = 0;
    virtual core::arith::tvector grad_(const core::arith::tvector &args, unsigned member) const = 0;
    virtual core::arith::rvector grad_(const core::arith::rvector &args, unsigned member) const = 0;

    virtual core::arith::rmatrix jacobian_(const core::arith::rvector &args) const = 0;
    virtual core::arith::imatrix jacobian_(const core::arith::ivector &args) const = 0;
    virtual core::arith::amatrix jacobian_(const core::arith::avector &args) const = 0;
    virtual core::arith::tmatrix jacobian_(const core::arith::tvector &args) const = 0;

    virtual const IVFunction& grad_fun_(unsigned member) const = 0;
  };
}

#endif /*__IDERIVATIVE_HPP__*/
