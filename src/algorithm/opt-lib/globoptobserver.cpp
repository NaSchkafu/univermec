/**
 * @file   globoptobserver.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:22:32 2009
 *
 * @brief  Implementation file for the observer
 *
 *
 */


#include <algorithm/opt-lib/globoptobserver.hpp>

namespace GlobOpt {
  void GlobOptStatistic::sync(const GlobOptStatistic &other)
  {
    discJohn += other.discJohn;
    discLin += other.discLin;
    discHCUBound += other.discHCUBound;
    discHCConst += other.discHCConst;
    discHCUBound += other.discBCUBound;
    discFeas += other.discFeas;
    discInfeas += other.discInfeas;
    discBCConst += other.discBCConst;
    if(perfmonOn) {
      reJohn += other.reJohn;
      reLin += other.reLin;
      reHCUBound += other.reHCUBound;
      reHCConst += other.reHCConst;
      reBCUBound += other.reBCUBound;
      reBCConst += other.discBCConst;
    }
  }

  std::ostream& operator<<(std::ostream &os, const GlobOptStatistic &stats)
  {
    os << "discJohn: " << stats.discJohn << std::endl;
    os << "discLin: " << stats.discLin << std::endl;
    os << "discHCUBound: " << stats.discHCUBound << std::endl;
    os << "discBCUBound: " << stats.discBCUBound << std::endl;
    os << "discFeas: " << stats.discFeas << std::endl;
    os << "discInfeas: " << stats.discInfeas << std::endl;
    os << "discBCConst: " << stats.discBCConst << std::endl;

    os << "reJohn: " << stats.reJohn << std::endl;
    os << "reLin: " << stats.reLin << std::endl;
    os << "reHCUBound: " << stats.reHCUBound << std::endl;
    os << "reBCUBound: " << stats.reBCUBound << std::endl;
    os << "reHCConst: " << stats.reHCConst << std::endl;
    os << "reBCConst: " << stats.reBCConst << std::endl;
    return os;
  }

  std::ostream& operator<<(std::ostream &os, const GlobOptResult &result)
  {
    os << "opt-lib has been ";
    switch(result.solverReturn) {
    case RE_SOLVED:
      os << "successful finished.";
      break;
    case RE_USER_ABORT:
      os << "aborted by the user.";
      break;
    case RE_MAX_IT:
      os << "aborted due to reaching the maximum number of iterations.";
      break;
    case RE_INTERNAL_ERROR:
      os << "terminated by an internal error!" << std::endl;
      return os;
    }
    os << std::endl << "The problem is ";
    switch(result.problemType) {
    case PB_CERTAINLY_FEASIBLE:
      os << "certainly feasible.";
      break;
    case PB_EPSP_FEASIBLE:
      os << "assumed to be feasible (epsilon_g criterion).";
      break;
    case PB_INFEASIBLE:
      os << "infeasible.";
      break;
    case PB_POSSIBLY_FEASIBLE:
      os << "possibily feasible.";
      break;
    }
    os << std::endl << "The candidate point " << result.point << " is ";
    switch(result.pointType) {
    case RP_CERTAINLY_FEASIBLE:
      os << "certainly feasible.";
      break;
    case RP_EPSP_FEASIBLE:
      os << "assumed to be feasible (eps_g criterion).";
      break;
    }
    os << " and the objective bound there " << result.minimum << std::endl;
    if(result.hasUPoint)
      os << result.uBound << "is  rigorous upper bound on the global minimum to be found at " << result.uPoint;
    else
      os << "No rigorous upper bound on the global minimum found!";
    os << std::endl << "****************************************************************" << std::endl;
    os << "Rigorous bound for the global minimum is " << result.minInterval 
       << " Enclosure Width: " << wid(result.minInterval) << std::endl;
    os << "Number of potential global minimizers " << result.finalLst.size() << std::endl;
    os << "Required iterations " << result.iterations << std::endl;
    os << "****************************************************************" << std::endl;
    return os;
  }

}
