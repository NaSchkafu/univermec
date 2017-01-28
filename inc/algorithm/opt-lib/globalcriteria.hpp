/**
 * @file   globalcriteria.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:16:51 2009
 * 
 * @brief  Header for criteria
 * 
 * 
 */


#ifndef __GLOBALCRITERIA_HPP__
#define __GLOBALCRITERIA_HPP__ __GLOBALCRITERIA_HPP__

#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
}

namespace GlobOpt 
{
  


/// Global stopping criteria for the GlobOpt Library
/*!
** This is the actual implementation of the global criteria.
** DO NOT USE THIS CLASS DIRECTLY. REFER TO THE SINGLETON
** SingleGlobalCriteria
**
*/
class DONOTUSEGlobalCriteria {
private:
  DONOTUSEGlobalCriteria();
  static DONOTUSEGlobalCriteria m_instance;


public:
  static DONOTUSEGlobalCriteria& Instance()
    {
      return m_instance;
    }


  /*!
  ** Checks if the interval satisfies the width criterion
  **
  **
  ** @param i the interval
  **
  ** @return true if w(i) < epsilon
  */
  bool checkInterval(const core::arith::interval &i) const;

  /*!
  ** Checks if the result of the function evaluation satisfies the
  ** epsilon criterion
  **
  ** @param i function result
  **
  ** @return true if MT::w <= epsilon
  */
  bool checkFunction(const core::arith::interval &i) const;

  /*!
  ** Checks if the evaluation of F at the args satiesfies the epsilon criterion
  **
  ** @param F function
  ** @param args arguments
  **
  ** @return true if MT::w(F(args)) <= epsilon
  */
  bool checkFunction(const core::arith::ivector &args, const functions::IFunction& F) const;

  bool checkInEqConstraint(const core::arith::ivector &args, const functions::IFunction &P) const;

  bool checkInEqConstraint(const core::arith::interval &i) const;

  bool checkBox(const core::arith::ivector &box) const;

  // non const
  /*!
  ** Set the maximum width
  **
  ** @param epsX
  */
  void setEpsX(const core::arith::mreal &epsX);

  /** 
   * Set maximum width of obj. value evaluation
   * 
   * @param epsF 
   */
  void setEpsF(const core::arith::mreal &epsF);

  /** 
   * Sets maximum constraint violation
   * 
   * @param epsP 
   */
  void setEpsP(const core::arith::mreal &epsP);

  /*!
  ** Checks wether the box has been sufficiently reduced in a step of
  ** the algorithm
  ** (see Hansen 11.7)
  **
  ** @param newBox box after the step
  ** @param oldBox box before the step
  **
  ** @return true if the progress criterion is satisfied
  */
  bool hasReduced(const core::arith::ivector &newBox, const core::arith::ivector &oldBox);

  /*!
  ** Checks if a number should be treated as zero
  ** (returns abs(n) < m_epsZero)
  **
  ** @param n mreal number
  **
  ** @return true if the number can be set to zero
  */
  bool isZero(const core::arith::mreal &n) const;

  /*!
  ** Checks if an interval should be treated as zero
  **
  ** @param i interval
  **
  ** @return true if i should be treated as zero
  */
  bool isZero(const core::arith::interval &i) const;

  /** 
   * Gets maximum width
   * 
   * 
   * @return max. width
   */
  core::arith::mreal epsX() const { return m_epsilonX; }

  /** 
   * Gets maximum constraint violation
   * 
   * 
   * @return max. violation
   */
  core::arith::mreal epsP() const { return m_epsilonP; }

  /** 
   * Gets maximum width on objective function
   * 
   * 
   * @return max. width on obj. function
   */
  core::arith::mreal epsF() const { return m_epsilonF; }

protected:
  /// 1 ulp
//  core::arith::mreal m_ulp;

  /// epsilon for interval width
  core::arith::mreal m_epsilonX;

  /// epsiolon for interval width of functione evalution
  core::arith::mreal m_epsilonF;

  /// epsilon for interval with of constraint evaluation
  core::arith::mreal m_epsilonP;

  /// epsilon for determining zero
  core::arith::mreal m_epsZero;

};

typedef DONOTUSEGlobalCriteria GC;

}


#endif

