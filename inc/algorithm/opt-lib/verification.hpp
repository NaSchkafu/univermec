/**
 * @file   verification.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:32:44 2009
 * 
 * @brief  Header for verification procedure
 * 
 * 
 */


#ifndef __VERIFICATION_HPP__
#define __VERIFICATION_HPP__ __VERIFICATION_HPP__

#include <iostream>
#include "types.hpp"


namespace GlobOpt {

  /**
   * Result type for verificationStep
   * UNKNOWN: no statement
   * UNIQUE_ZERO: existence of an unique zero was proven
   * ZERO: existence of a uero was proven
   * INTERNAL_ERROR: an error occured
   */
  enum VERIFICATION_RESULT { UNKNOWN_GAP, UNKNOWN_EMPTY, UNKNOWN, NLGS_UNIQUE_ZERO,
			     NLGS_ZERO, INTERNAL_ERROR, VSTEP };

  /// type for epsilon-inflation callback
  typedef void(*BLOWFunc)(core::arith::ivector&, const core::arith::mreal &eps, const FLst &lst);

  /// resulttype for the verification step
  struct VerficationResult {
    VERIFICATION_RESULT newtonResult;
    core::arith::ivector newtonBox;
    VERIFICATION_RESULT mirandaResult;
  };

  /** 
   * Tries to verifiy a zero near point
   * 
   * @param point point where a zero is assumed
   * @param lst the NLGS
   * @param b epsilon-inflation callback
   * 
   * @return verification result
   */
  VerficationResult verificationStep(const core::arith::rvector &point,
				     const FLst &lst, BLOWFunc b);
  /** 
   * Tries to verifiy a zero near/in box
   * 
   * @param point box in which a zero is assumed
   * @param lst the NLGS
   * @param b epsilon-inflation callback
   * 
   * @return verification result
   */
  VerficationResult verificationStep(const core::arith::ivector &box,
				     const FLst &lst, BLOWFunc b);

  std::ostream& operator<<(std::ostream &os, const VerficationResult &result);
  std::ostream& operator<<(std::ostream &os, VERIFICATION_RESULT result);

}


#endif
