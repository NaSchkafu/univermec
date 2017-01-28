/**
 * @file   globoptalgo.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 15:05:28 2009
 *
 * @brief  Header for the global optimization algorithm
 *
 *
 */


#ifndef __GLOBOPTALGO_HPP__
#define __GLOBOPTALGO_HPP__ __GLOBOPTALGO_HPP__

//#include "function.hpp"
#include "optbox.hpp"
#include "globoptobserver.hpp"
#include "ioptstrategy.hpp"

// wg. inline Template
//#include "upperbound.hpp"
#include "helper.hpp"

namespace functions
{
  class IFunction;
}

namespace decomp
{
  template <typename T>
  class IBoxListMgr;
}

namespace algorithm
{
  namespace utils
  {
    template <typename T>
    class UpperBoundImpl;
    typedef UpperBoundImpl<core::arith::rvector> UpperBound;
  }
}

namespace GlobOpt {

  typedef std::vector<boost::reference_wrapper<functions::IFunction> > FLst;

  class BoxListMgr;

  struct local_opt_env_t;
  struct global_state_t;
  
  /// proximity queries using global optimization
  /**
   * Implementation of an algorithm for proximity queries using global
   * optimization
   *
   */
  class GlobOptAlgo {
  public:
    /// type of final list
    typedef std::deque<OptBox*> FinalLst;

    /**
     * Constructor
     *
     * @param objF objective function
     * @param constraints  list of inequality constraints
     * @param strategy strategy engine
     * @param maxIt maximum number of iterations
     *
     */
    GlobOptAlgo(const functions::IFunction &objF, const FLst& constraints,
		IOptStrategy &strat, unsigned int maxIt);

    /// destructor
    ~GlobOptAlgo();

    /*!
    ** Starts the global optimization algorithm
    **
    ** @param startBox initial box for the search
    ** @param intPoint feasible(!) starting point for interior point method
    */
    void start(const core::arith::ivector &startBox, const core::arith::rvector &intPoint = core::arith::rvector(1));

    /**
     * Registers an observer
     * The notify method of observer is called in every algorithm
     * iteration
     * In the multithreaded version only the master thread calls
     * the notify method
     *
     * @param observer reference to the observer
     */
    void registerObserver(GlobOptObserver& observer);

    /**
     * Gets the algorithm result
     *
     *
     * @return result
     */
    const GlobOptResult& result() const;

  private:
    /// enum for the termination criterion
    //enum TERM_RES { ADDED_BOX = 0, REDUCED_BOX = 1, NOTHING = 2 };

    /// enum for algorithm status
    enum ALG_STATUS { RUNNING, FINISHED, USER_ABORT, MAX_IT_EXCEEDED };

    /// 
    enum THREAD_STATUS { T_PENDING, T_NEW_BOXES, T_NEXT_PHASE  };

#ifdef _OPENMP
    enum PHASE2_STATE {PENDING, NEW_BOXES, OK };
#endif

    /**
     * Starts the actual algorithm
     *
     */
    void work();

    /**
     * Finds the global minimum value
     *
     * @param minF reference to mreal for storing lower bound
     * @param maxF reference to mreal for storing upper bound
     * @param lst list for search
     * @param uBound current upper bound
     *
     * @return true if uBound.point() is contained in a box in lst
     */
    bool minInterval(core::arith::mreal &minF, core::arith::mreal &maxF,
		     FinalLst &lst, const algorithm::utils::UpperBound &uBound);

    /** 
     * Applies a list of optimization stratgies
     * 
     * @param lst 
     * @param state 
     * @param env 
     * 
     * @return 
     */
    TERM_RESULT apply_strats(const IOptStrategy::strat_ele_lst_t &lst,
			     opt_worker_state_t &state,
			     local_opt_env_t &env,
			     IOptStrategy::phase_config_t &bmgr);

    TERM_RESULT apply_strats(const IOptStrategy::gpu_strat_ele_lst_t &lst,
			     opt_worker_state_t &state,
			     IOptStrategy::phase_config_t &lphase,
			     local_opt_env_t &env,
			     global_state_t &global_state);

    void request_boxes(opt_worker_state_t &state,
		       IOptStrategy::phase_config_t &lphase,
		       local_opt_env_t &local_env,
		       global_state_t &global_state,
		       unsigned number = 1,
		       std::deque<std::pair<OptBox*, bool> > *ret =  nullptr);
    
      
  private:
    /// objective function
    const functions::IFunction& m_obj;

    /// list of constraints
    const FLst& m_constraints;

    /// the initial search box
    std::unique_ptr<OptBox> m_initBox;

    /// list of boxes
    boost::ptr_vector<OptBox> m_finalLst;

    /// list of observers
    std::vector<boost::reference_wrapper<GlobOptObserver> > m_observers;

    /// current status
    GlobOptStatus m_status;

    /// result of algorithm
    GlobOptResult m_result;

    /// current status
    ALG_STATUS m_alg_status;

    /// threshold for applying a newton step to the fritz-john condition
    core::arith::mreal m_epsilonNwt;

    /// numer of final boxes
    unsigned int m_finalCounter;

    /// number of maximum iterations
    unsigned int m_maxIt;

    /// starting point for Interior Point optimizer
    core::arith::rvector m_intPoint;

    /// strategy engine
    IOptStrategy &m_strategy; 

    /// UpperBound inside finalLst
    bool m_ub_inside;
  };

#include "globoptalgo.inl"

}

#endif
