#ifndef __IFUNCTION_HPP__
#define __IFUNCTION_HPP__

#include <core/arithcore.hpp>
#include "ivfunction.hpp"

namespace functions
{
  class IFFeatures;

  /// Convenience for representing scalar-valued mathematical functions \f$f: \Rz^n \to \Rz\f$.
  /**
   * This interface is a specialization of @ref IVFunction for scalar-valued functions.
   * Note, that the interface hides the `operator()` members of @ref IVFunction. An explicit cast
   * is necessary to access the inherited members. 
   * 
   */
  class IFunction : public IVFunction
  {
  public:
    // Function eval

    /*! 
    ** Evaluates the function using interval arithmetic
    ** 
    ** @param args interval vector containing the input parameters
    ** 
    ** @return interval enclosure of the range
    */
    inline core::arith::interval operator()(const core::arith::ivector& args) const
      {
        return calc_(args);
      }

    /*! 
    ** Evaluates the function using standard floating point arithmetic (IEEE754)
    ** 
    ** @param args floating point vector containing the input parameters
    ** 
    ** @return (approximate) result 
    */
    inline core::arith::mreal operator()(const core::arith::rvector &args) const
      {
        return calc_(args);
      }

    /*! 
    ** Evaluates the function using affine arithmetic
    ** 
    ** @param args affine vector containing the input parameters
    ** 
    ** @return affine enclosure of the range
    */
    inline core::arith::aaf operator()(const core::arith::avector &args) const 
      {
	return calc_(args);
      }

    /** 
     * 
     * 
     * 
     * @return 
     */
    inline core::arith::taylormodel operator()(const core::arith::tvector &args) const 
      {
	return calc_(args);
      }

    /*! 
    ** Clone function for copying the object
    * (Polymorphic copy ctor)
    ** 
    ** 
    ** @return copy of this object
    */
    inline IFunction* clone() const
      {
	return clone_();
      }

    virtual ~IFunction() { }

  protected:
    IFunction()  { }
    

  private:
    virtual core::arith::interval calc_(const core::arith::ivector &args) const = 0;
    virtual core::arith::aaf calc_(const core::arith::avector &args) const = 0;
    virtual core::arith::mreal calc_(const core::arith::rvector &args) const = 0;
    virtual core::arith::taylormodel calc_(const core::arith::tvector &args) const = 0;

    virtual IFunction* clone_() const = 0;

  private:
    // no cctor
    IFunction(const IFunction &);
    // no assign
    IFunction& operator=(const IFunction &);
  };

  /// Clones a given function with its polymorphic clone operation
  /** 
   * Clones a given function with its polymorphic clone operation
   * 
   * @param f function f
   * 
   * @return deep copy
   */
  inline IFunction* new_clone(const IFunction &f)
  {
    return f.clone();
  }

  /// Clones a given function with its polymorphic clone operation
  /** 
   * Clones a given function with its polymorphic clone operation
   * 
   * @param f function f
   * 
   * @return deep copy
   */
  inline IFunction* new_clone(const IFunction* f)
  {
    return f->clone();
  }
}
#endif /*__IFUNCTION_HPP__*/
