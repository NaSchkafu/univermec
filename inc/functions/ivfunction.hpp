#ifndef __IVFUNCTION_HPP__
#define __IVFUNCTION_HPP__

#include <core/arithcore.hpp>
/// Namespace for mathematical functions
/*!
* This namespace provides interfaces for representing mathematical functions \f$f: \Rz^n \to \Rz^m\f$
* based on the notion of a function represention object (FRO). An FRO characterizes a function by a
* set of inclusion functions for different arithmetics and a set of features (e.g., for derivatives).
* 
* Inside UniVerMeC an FRO is represented by the interface @ref IVFunction. Derivatives are 
* represented by @ref IDerivative and accessible through the @ref IFFeatures interface which
* models the feature set of a function.
* 
* To actually construct a function the use of template functions inside @ref functions::details
* is recommened. If this is not possible it is possible to implement all necessary interface
* manually. Refer to @ref functions::details::FLinChainRule  for an example.
* 
* For more details on the theoretical foundation and implemenation refer to Sect. 4.4 in \cite thesis.
*/
namespace functions 
{

  class IFFeatures;

  /// Interface representing a mathematical function \f$f: \Rz^n \to \Rz^m\f$
  /*!
  ** 
  * This interface is used for representing a mathematical function \f$f: \Rz^n \to \Rz^m\f$ inside 
  * the framework. The concrete function is characterized by a set of inclusion functions which 
  * contains exactly one inclusion function for each supported arithmetic of the framework.
  * Additionally, access to the [IFFeature interface](@ref IFFeatures) representing the feature
  * set of the function is possible. This set contains, for example, the derivatives of function
  * if they are available.
  * 
  * Algorithms on higher layers can generally rely on the defined
  * operations. All functions inherent from this interface either
  * directly or indirectly
  */
  class IVFunction 
  {
  public:
    /*! 
    ** Evaluates the function using interval arithmetic
    ** 
    ** @param args interval vector containing the input parameters
    ** 
    ** @return interval enclosure of the range
    */
    inline core::arith::ivector operator()(const core::arith::ivector& args) const
      {
        return calc_v_(args);
      }

    /*! 
    ** Evaluates the function using the mreal type
    ** 
    ** @param args floating-point vector containing the input arguments
    ** 
    ** @return (approximate) result 
    */
    inline core::arith::rvector operator()(const core::arith::rvector &args) const
      {
        return calc_v_(args);
      }

    /*! 
    ** Evaluates the function using affine arithmetic
    ** 
    ** @param args affine vector containing the input arguments
    ** 
    ** @return affine enclosure of the range
    */
    inline core::arith::avector operator()(const core::arith::avector &args) const 
      {
	return calc_v_(args);
      }

    /** 
     * Evaluated the function using Taylor models
     * 
     * @param args vector of Taylor models containg the input arguments
     * 
     * @return Taylor model enclosing the range
     */
    inline core::arith::tvector operator()(const core::arith::tvector &args) const 
      {
	return calc_v_(args);
      }


    /** 
     * Dimension of the domain
     * 
     * 
     * @return dimension
     */
    inline unsigned dim() const 
      {
	return dim_();
      }

    /** 
     * Dimension of the codomain
     * (number of member functions)
     * 
     * 
     * @return codomain's dimension
     */
    inline unsigned cdim() const
      {
	return cdim_();
      }

    /** 
     * Polymorphic clone operation
     * 
     * 
     * @return deep copy of the function
     */
    inline IVFunction* clone() const
      {
	return clone_();
      }

    /** 
     * Textual represention of the function
     * 
     * 
     * @return textual representation
     */
    inline std::string text() const
      {
	return text_();
      }

    /** 
     * Returns a reference to the @ref IFFeatures interface of this function
     * representing the feature set of the function.
     * 
     * @return reference to utility interface
     */
    inline const IFFeatures& features() const 
      {
	return features_();
      }


    virtual ~IVFunction() {}

  private:
    virtual core::arith::ivector calc_v_(const core::arith::ivector &args) const = 0;
    virtual core::arith::avector calc_v_(const core::arith::avector &args) const = 0;
    virtual core::arith::rvector calc_v_(const core::arith::rvector &args) const = 0;
    virtual core::arith::tvector calc_v_(const core::arith::tvector &args) const = 0;

    virtual unsigned dim_() const = 0;
    virtual unsigned cdim_() const = 0;
    virtual IVFunction* clone_() const = 0;
    virtual std::string text_() const = 0;

    virtual const IFFeatures& features_() const = 0;
  };
}

#endif /*__IVFUNCTION_HPP__*/
