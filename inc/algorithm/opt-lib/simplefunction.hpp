/*!
**@file   simplefunction.hpp
**@author Stefan Kiel
**
**@brief  This file contains various functions and function patterns
**which may be helpful for defining new functions
**
**
*/


#ifndef __SIMPLEFUNCTION_HPP__
#define __SIMPLEFUNCTION_HPP__ __SIMPLEFUNCTION_HPP__


#include "helper.hpp"

namespace functions
{
  class IFunction;
}

namespace GlobOpt
{


// func obj which returns a constant interval

// /// obsolete
// typedef __ConstantFunction __ZeroFunction;

// /// func obj which returns a point interval containing 1
// struct __OneFunction {
//   core::arith::interval operator()(const core::arith::ivector &) const
//     {
//       return core::arith::interval(1,1);
//     }
// };

// /// func obj which adds a multiplier based on an argument to a function
// struct __Multiplier {
//   __Multiplier(int sizeX, int cNo, const functions::IFunction &G)
//     :_sizeX(sizeX), _cNo(cNo), _G(G)
//     {}

//   core::arith::interval operator()(const core::arith::ivector & args) const
//     {
//       return args[_sizeX + 1 +_cNo]*_G(MT::copyVec(args,_sizeX));
//     }

//   const int _sizeX;
//   const int _cNo;
//   const functions::IFunction& _G;
// };

// /// func obj which acts as argument wrapper
// struct __ArgumentWrapper {
//   __ArgumentWrapper(int sizeX, const functions::IFunction &C)
//     :_sizeX(sizeX), _C(C)
//   {}

//   core::arith::interval operator()(const core::arith::ivector &args) const
//   {
//       core::arith::interval temp;
//      return _C(MT::copyVec(args, _sizeX));
//   }

//   const int _sizeX;
//   const functions::IFunction& _C;

// };

// /// func objs which acts as addition between functions
// struct __Adder {
//   __Adder(int sizeX, const functions::IFunction& F, const std::vector<boost::shared_ptr<functions::IFunction> > constraints)
//     :_sizeX(sizeX), _F(F), _constraints(constraints)
//   {}

//   core::arith::interval operator()(const core::arith::ivector &args) const
//   {
//     core::arith::ivector x(MT::copyVec(args, _sizeX));

//     core::arith::interval result(args[_sizeX + 1]*_F(x));


//     for(std::vector<boost::shared_ptr<functions::IFunction> >::const_iterator cIt = _constraints.begin();
//         cIt != _constraints.end(); ++cIt)
//         result += /*args[sizeX + i + 2]**/((**cIt)(args)); //TODO: Multiplikation HIER???
//     return result;
//   }

//   const int _sizeX;
//   const functions::IFunction& _F;
//   const std::vector<boost::shared_ptr<functions::IFunction> > _constraints;
// };

/// A template class for a simple function
/*!
** Use the template parameter F for adding a functional object
** with the actual implementation of the function
** the operator() of F should take a core::arith::ivector as argument
** Be careful with the constructors which set the dimension to 0
**
*/
// template<class F>
// class SimpleFunction : public functions::IFunction {
// public:

//   /*!
//   ** Constructor
//   **
//   ** @param function pointer to the function (pointer is stored in a
//   ** shared_ptr and is freed if this object and all copies are destroyed)
//   ** @param dim dimension of this function
//   **
//   ** @return
//   */
//   SimpleFunction(F* function, unsigned int dim)
//     :_F(function), _dim(dim)
//     {}


//   /*!
//   ** Constructor
//   ** Note: If you use this constructor, the template class F needs a
//   ** standard constructor or you will get a compiler error
//   **
//   ** @param dim dimension
//   **
//   */
//   SimpleFunction(unsigned int dim)
//     :_F(new F()), _dim(dim)
//   { }

//   // copy constructor
//   SimpleFunction(const SimpleFunction &old)
//     :Function(), _F(old._F), _dim(old.dim())
//   {}

//   virtual ~SimpleFunction() { }

//   virtual unsigned int dim() const { return _dim; }

// protected:
//   inline core::arith::interval calc(const core::arith::ivector &args) const
//   {
//     return (*_F)(args);
//   }

//   inline core::arith::interval calc(const core::arith::rvector &args) const
//   {
//     return (*_F)(static_cast<core::arith::ivector>(args));
//   }

// private:
//   boost::shared_ptr<F> _F;
//   const unsigned int _dim;

// };

/// a function which returns only zero
/**
 * Function which returns zero. Used for impl. of ConstantFunction
 *
 */
  // class ZeroFunction : public IFunction {
  // public:
  //   /**
  //    * Constructor
  //    *
  //    * @param dim dimension of the function
  //    */
  //   ZeroFunction(unsigned int dim)
  //     :m_dim(dim)
  //     {
  //     }

  //   virtual unsigned int dim() const { return m_dim; }

  // private:
  //   virtual core::arith::interval calc(const core::arith::ivector &) const { return core::arith::interval(0,0); }
  //   virtual core::arith::interval calc(const core::arith::rvector &) const { return core::arith::interval(0,0); }
  //   // C2Function members
  //   virtual const C1Function& getGradF(unsigned int ) const { return *this; }


  // private:
  //   const unsigned int m_dim;
  // };



/// a constant as function
/*!
** a derivable constant function (mostly used for gradient members
** of a C2Function)
**
*/
// class ConstantFunction : public C2Function {
// public:
//   ConstantFunction(const core::arith::interval &c, unsigned int dim)
//     :_c(c), _dim(dim), _zero(dim) { }
//   virtual ~ConstantFunction() { }

//   virtual unsigned int dim() const { return _dim; }
//   virtual std::string text() const;

// private:
//   virtual core::arith::interval calc(const core::arith::ivector &) const { return _c; }
//   virtual core::arith::interval calc(const core::arith::rvector &) const { return _c; }
//   // C2Function members
//   virtual const C1Function& getGradF(unsigned int ) const { return _zero; }


// private:
//   const core::arith::interval _c;
//   const unsigned int _dim;
//   const ZeroFunction _zero;
// };

// /// an argument wrapper for functions
// /*!
// ** Wraps the argument of a function
// ** The function F is called with args(start, end)
// ** So F.dim() == end - start is required!
// */
// class ArgWrapper : public Function {
// public:


//   virtual unsigned int dim() const { return _dim; }
//   virtual std::string text() const;

// protected:
//   virtual core::arith::interval calc(const core::arith::ivector &args) const;
//   virtual core::arith::interval calc(const core::arith::rvector &args) const;

// private:
//   const Function& _F;
//   const unsigned int _start;
//   const unsigned int _end;
//   const unsigned int _dim;

// };


  /*!
  ** Constructor
  **
  ** @param F function to wrap
  ** @param start startindex of the arguments of F
  ** @param end endindex of the argument of F
  ** @param dim dimension of the inpitvector of THIS function
  */
  // functions::IFunction* arg_wrapper(const functions::IFunction &f, unsigned int start, unsigned int end, 
  // 				    unsigned int dim);

  /// Function which adds a multiplier based on an argument to an existing function
  functions::IFunction* multi_func(unsigned sizeX, unsigned cNo, 
				   const functions::IFunction &G, unsigned dim);

}


#endif
