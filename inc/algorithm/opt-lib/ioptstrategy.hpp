#ifndef __IOPTSTRATEGY_HPP__
#define __IOPTSTRATEGY_HPP__

#include <vector>
#include <core/arithcore.hpp>

namespace functions
{
  class IFunction;
}

namespace decomp
{
  template <typename T>
  class IBoxListMgr;

  template <typename T>
  class IMultiSection;
}

namespace GlobOpt 
{

  struct opt_worker_state_t;
  struct strat_ele_t;
  struct gpu_strat_ele_t;
  class OptBox;

  /**
   * Result of termination criterion
   * 
   * NONE: No criterion satisfied
   * SOLUTION: add to final list, restart main loop
   * REDUCED: significantly reduced, add to working list and restart loop
   * DISCARDED: Box has been discarded
   * 
   */
  enum TERM_RESULT { NONE, SOLUTION, REDUCED, DISCARDED, TMP_REMOVED };

  /// type of termination criterion
  /**
   * Note: This is standard C function pointer and not a member
   * function pointer. Normally, it is declared static in concrete strategy
   * classes and may depend on static members of these classes.
   * (See documenetation of the classes implementing IOptStrategy)
   */
  typedef TERM_RESULT (*f_term_t)(const opt_worker_state_t&);

  /// Phases of one iteration of the optimization algorithm
  enum OPT_PHASE { OPT_A, OPT_INFEAS, OPT_B, OPT_FEAS, OPT_C, OPT_STRICT_FEAS, OPT_D };

  /// customizes the strategy of the global optimization algorithm
  class IOptStrategy 
  {
  public:
    /// list of optimization strategy elements
    typedef std::vector<const strat_ele_t*> strat_ele_lst_t;

    typedef std::vector<const gpu_strat_ele_t*> gpu_strat_ele_lst_t;

    /// defines a configuration for an optimization phase
    struct phase_config_t 
    {
      phase_config_t()
	:section(0)
	{}

      /// optimization strategy elements to be applied on the solution list
      strat_ele_lst_t sol_strats;
      /// optimization strategy elements to be applied on the temporary solution list
      strat_ele_lst_t tmp_strats;
      /// minimum width for subdivision in the next phase
      core::arith::mreal min_width;
      /// maximum number of iterations to call next_phase again
      unsigned max_it;
      /// strategies for the splitting step
      strat_ele_lst_t psplit;
      /// strategies for optimizer step a
      strat_ele_lst_t pa;
      /// strategies for optimizer step b
      strat_ele_lst_t pb;
      /// strategies for optimizer step c
      strat_ele_lst_t pc;
      /// strategies for optimizer step d
      strat_ele_lst_t pd;
      /// strategies for optimizer step "box possibly infeasible"
      strat_ele_lst_t pinfeas;
      /// strategies for optimizer step "box feasible"
      strat_ele_lst_t pfeas;
      /// strategies for optimizer step "box strictly feasible"
      strat_ele_lst_t psfeas;
      /// pointer to termination criterion
      f_term_t term_crit;
      /// multisection strategy for the next phase (maybe zero if no strategy change is intended)
      decomp::IMultiSection<OptBox>* section;
      /// GPU Split elements
      gpu_strat_ele_lst_t gpu_psplit;
    };

    /// Workaround Multi Threading
    struct global_config_t 
    {
      global_config_t()
	:bmgr(0)
	{}

      /// box list manager for the next phase (maybe zero if no strategy change is intended)
      decomp::IBoxListMgr<OptBox>* bmgr;
      /// splits the initial box before starting the algorithm (without using psplit contractors)
      unsigned pre_split;
    };

    IOptStrategy() {}
    virtual ~IOptStrategy() {}

    /** 
     * Switches to the phase in the strategy
     * 
     * @param state current state of optimizer (note: cbox is invalid!)
     * @param tmps number of elements on temporary list
     * @param sols number of elements on final solution list
     * @param its number of iterations
     * 
     * @return configuration
     */
    inline phase_config_t next_phase(opt_worker_state_t &state, unsigned tmps, 
				     unsigned sols, unsigned its)
      {
	return next_phase_(state, tmps, sols, its);
      }

    /** 
     * Gets the first phase configuration of the strategy
     * 
     * @param state worker state (note: cbox is the initial box)
     * 
     * @return configuration
     */
    inline phase_config_t first_phase(opt_worker_state_t &state) 
      {
	return first_phase_(state);
      }

    inline global_config_t global_config(opt_worker_state_t &state)
      {
	return global_config_(state);
      }

    /** 
     * Gets the configuration for the final phase
     * Note: tmp_strats is only considered if the algorithm is aborted due to maximum iterations 
     * exceeded; sol_strats is called on all boxes; no other strategies are applied in this phase
     * 
     * @param state worker state (note: cbox is invalid)
     * @param tmps number of elements on temporary list
     * @param sols number of elements on final solution list
     * @param its number of iterations
     * 
     * @return configuration
     */
    inline phase_config_t final_phase(opt_worker_state_t &state, unsigned tmps, 
				      unsigned sols, unsigned its)
      {
	return final_phase_(state, tmps, sols, its);
      }

    inline IOptStrategy* clone() const 
      {
	return clone_();
      }

  private:
    virtual phase_config_t next_phase_(opt_worker_state_t &state, unsigned tmps, 
				       unsigned sols, unsigned its) = 0;
    virtual phase_config_t final_phase_(opt_worker_state_t &state, unsigned tmps, 
				       unsigned sols, unsigned its) = 0;
    virtual global_config_t global_config_(opt_worker_state_t &state) = 0;
    virtual phase_config_t first_phase_(opt_worker_state_t &state) = 0;
    virtual IOptStrategy* clone_() const = 0;

  private:
    IOptStrategy(const IOptStrategy&);
    IOptStrategy& operator=(const IOptStrategy&);
  };
}

#endif /*__IOPTSTRATEGY_HPP__*/
