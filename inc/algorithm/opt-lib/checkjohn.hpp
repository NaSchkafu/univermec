/**
 * @file   checkjohn.hpp
 * @author Stefan Kiel 
 * @date   Mon May 11 14:09:56 2009
 * 
 * @brief  Header for test of Fritz-John-Cond.
 * 
 * 
 */


#ifndef __CHECKJOHN_HPP__
#define __CHECKJOHN_HPP__ __CHECKJOHN_HPP__

namespace functions
{
  class IFunction; 
}

namespace GlobOpt 
{

  class OptBox;

  /** 
   * Checks the John Conditions
   * 
   * @param b box to check
   * @param obj objective function
   * 
   * @return true if b has been deleted, false otherwise -> in this case b has been eventually
   * updated with new bounds
   */
  bool checkJohnCond(OptBox &b, const functions::IFunction &obj);
}


#endif
