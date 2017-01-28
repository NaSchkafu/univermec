dnl Adapter file for creating fadbad::Op template specialization
dnl automatically for all registered arithmetics.
#ifndef __FADBADADAPT_HPP__
#define __FADBADADAPT_HPP__

#include "core/arithcore.hpp"

divert(-1)
define(`CREATE_FADBAD_TEMPLATE_SPECIALIZATION',`
  /// specialization of the fadbad Op template for CLASS_NAME
  template<>
  struct Op<core::arith::`'CLASS_NAME> 
  {
    typedef core::arith::`'CLASS_NAME Base;
    static Base myInteger(const int i) { return Base(i); }
    static int myZero() { return 0; }
    static int myOne() { return 1;}
    static int myTwo() { return 2; }
    static Base myPI() { return core::arith::`'CLASS_NAME`'_pi(); }
    static Base myPos(const Base& x) { return +x; }
    static Base myNeg(const Base& x) { return -x; }
    template <typename U> static Base & myCadd(Base& x, const U& y) { return x+=y; }
    template <typename U> static Base& myCsub(Base& x, const U& y) { return x-=y; }
    template <typename U> static Base& myCmul(Base& x, const U& y) { return x*=y; }
    template <typename U> static Base& myCdiv(Base& x, const U& y) { return x/=y; }
    static Base myInv(const Base& x) { return myOne()/x; }
    static Base mySqr(const Base& x) { return sqr(x); }
    template <typename X, typename Y>
    static Base myPow(const X& x, const Y& y) { return pow(x,y); }
    static Base mySqrt(const Base& x) { return sqrt(x); }
    static Base myLog(const Base& x) { return log(x); }
    static Base myExp(const Base& x) { return exp(x); }
    static Base mySin(const Base& x) { return sin(x); }
    static Base myCos(const Base& x) {  return cos(x); }
    static Base myTan(const Base& x) {  return tan(x); }
    static Base myAsin(const Base& x) { return asin(x); }
    static Base myAcos(const Base& x) { return acos(x); }
    static Base myAtan(const Base& x) { return atan(x); }
    static bool myEq(const Base& x, const Base& y) { return x==y; }
    static bool myNe(const Base& x, const Base& y) { assert(!"Fix ME!"); return x == y; }
    static bool myLt(const Base& x, const Base& y) { assert(!"Fix ME!"); return x == y; }
    static bool myLe(const Base& x, const Base& y) { assert(!"Fix ME!"); return x == y; }
    static bool myGt(const Base& x, const Base& y) { assert(!"Fix ME!"); return x == y; }
    static bool myGe(const Base& x, const Base& y) { assert(!"Fix ME!"); return x == y; }
  };
')

divert(0)
namespace fadbad
{
CREATE_ADAPTERS(`CREATE_FADBAD_TEMPLATE_SPECIALIZATION')dnl
} //namespace fadbad


#endif /*__FADBADADAPT_HPP__*/

