/**
 * @file   globoptobserver.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:22:48 2009
 * 
 * @brief  Header for the observer class
 * 
 * 
 */


#ifndef __GLOBOPTOBSERVER_HPP__
#define __GLOBOPTOBSERVER_HPP__ __GLOBOPTOBSERVER_HPP__

#include <core/arithcore.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace GlobOpt {

  class OptBox;

  /**
   * PB_CERTAINLY_FEASIBLE: problem is feasible
   * PB_INFEASIBLE: problem is infeasible
   * PB_EPSP_FEASIBLE: problem is assumed to be feasible with the epsP criterion
   * 
   */
  enum GlobOptProblemType { PB_CERTAINLY_FEASIBLE, PB_EPSP_FEASIBLE, PB_INFEASIBLE, 
			    PB_POSSIBLY_FEASIBLE };
  
  /**
   * RP_CERTAINLY_FEASIBLEL: feasibility of returned point has been proven
   * RP_EPSP_FEASIBLE: feasiblity is assumed under the epsP criterion
   * 
   */
  enum GlobOptPointType { RP_CERTAINLY_FEASIBLE, RP_EPSP_FEASIBLE };
    
  /**
   * RE_SOLVED: Problem has been solved
   * RE_USER_ABORT: solver has been cancelled by an user request
   * RE_MAX_IT: maximum number of iterations exceeded 
   * RE_INTERNAL_ERROR: an internal error in the solver occured
   */
  enum GlobOptSolverReturn { RE_SOLVED, RE_USER_ABORT, RE_MAX_IT, RE_INTERNAL_ERROR }; 

  struct GlobOptStatistic
  {
    GlobOptStatistic()
      :discJohn(0), discLin(0), discHCUBound(0), discHCConst(0), discBCUBound(0), discFeas(0),
       discInfeas(0), discBCConst(0), reJohn(0), reLin(0), reHCUBound(0), reHCConst(0), 
       reBCUBound(0), reBCConst(0)
    { 
#ifdef GLOBOPT_PERFMON
      perfmonOn = true;
#else
      perfmonOn = false;
#endif
	}
    
    /// discared by John Condition
    unsigned int discJohn;
    /// discared by lin of obj
    unsigned int discLin;
    /// discarded by HC on f < uBound
    unsigned int discHCUBound;
    /// discared by HC on constraints
    unsigned int discHCConst;
    /// discarded by BC on f < uBound
    unsigned int discBCUBound;
    /// discared by stricly feasible test
    unsigned int discFeas;
    /// discarded by infeas test
    unsigned int discInfeas;
    /// discarded by BC on constraints
    unsigned int discBCConst;

    /// true if reduced monitoring is enabled
    bool perfmonOn;
    /// reduced by John Condition
    unsigned int reJohn;
    /// reduced by lin of obj
    unsigned int reLin;
    /// reduced by HC on f < uBound
    unsigned int reHCUBound;
    /// reduced by HC on constraints
    unsigned int reHCConst;
    /// reduced by BC on f < uBound
    unsigned int reBCUBound;
    /// reduced by BC on constraints
    unsigned int reBCConst;
    
    void sync(const GlobOptStatistic &other);
  };

  std::ostream& operator<<(std::ostream &os, const GlobOptStatistic &stats);
  
  /// status structure for GlobOptAlgo
  struct GlobOptStatus {
    GlobOptStatus()
      :hasUPoint(false), phase2(false)
      { }
    
    /// feasible point for upper bound on global minimum
    core::arith::rvector uPoint;
    
    /// upper bound value
    core::arith::mreal uBound;

    /// true if uPoint is valid
    bool hasUPoint;

    /// box processed
    core::arith::ivector box;
    
    /// no of boxes in the box manager list
    unsigned int boxes;
    
    /// no of boxes which satisfies the first criteria
    unsigned int finalBoxes;

    /// indicates that algorithm is in phase 2
    bool phase2;

    /// statiscal data
    GlobOptStatistic statistic;
  };
  
  /// result structure for GlobOptAlgo
  struct GlobOptResult {
    GlobOptResult(const boost::ptr_vector<OptBox> &final)
      :finalLst(final)
      {}

    /// solver exit status
    GlobOptSolverReturn solverReturn;
    
    /// feasibility of the problem
    GlobOptProblemType problemType;

    /// feasibility of returned point
    GlobOptPointType pointType;
    
    /// point for the global minimum
    core::arith::rvector point;

    /// value at point (global minimum)
    core::arith::mreal minimum;
    
    /// interval which contains the global minimum
    core::arith::interval minInterval;
    
    /// list of final boxes
    const boost::ptr_vector<OptBox>& finalLst;

    /// point for upper bound on global minimum
    core::arith::rvector uPoint;

    /// the upper bound
    core::arith::mreal uBound;
    
    /// true if uPoint and uBound are valid
    bool hasUPoint;

    /// statistical data
    GlobOptStatistic statistic;

    /// number of iterations
    unsigned int iterations;
  };

  std::ostream& operator<<(std::ostream &os, const GlobOptResult &result);

  /// interface for status callback
  /**
   * Interface for classes which want to observe the status of the
   * global optimization algorithm
   * The notify function is called before every iteration 
   */
  class GlobOptObserver {
  public:
    /** 
     * Called before every iteration
     * 
     * @param status algorithm status
     * 
     * @return return true to proceed or false to cancel the algorithm
     */
    virtual bool notify(const GlobOptStatus &status) = 0;
    virtual ~GlobOptObserver()  { }
  };

};

#endif
