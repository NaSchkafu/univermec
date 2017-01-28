#ifndef __OPTENV_HPP__
#define __OPTENV_HPP__

namespace functions
{
  class IFunction;
}

#include "ioptstrategy.hpp"
#include "types.hpp"
#include "algorithm/utils/upperbound.hpp"

namespace GlobOpt
{
  //struct local_opt_env_t;

  /// Public informations about a state of a worker thread
  /**
   * The worker state data structure is used for the communication
   * between the optimizer, optimizer strategy elements and the optimizer
   * strategy engine.
   * 
   * Responsibility:
   * ubound: Updated by strategy elements
   * cbox: Current box, added and moved by optimizer; reduced by strategy element
   * term_crit: 
   * org_obj: Original objective function, added by optimizer
   * org_ineq_constraits: Original inequality contraints, added by optimizer
   * org_eq_constraints: Original equality constraints, added by optimizer
   * 
   * obj: Enclosure of org_obj, initialized by optimizer and eventually updated by strategy engine
   * ineq_constraints: Enclosure of ineq_constraints, initialized by optimizer and eventually updated by strategy engine
   * eq_constraints: Enclosure of eq_constraints, initialized by optimizer and eventually updated by strategy engine
   */
  struct opt_worker_state_t
  {
    opt_worker_state_t(const functions::IFunction &obj, const FLst &ineq_constraints,
		       const FLst& eq_constraints)
      :org_obj(obj), org_eq_constraints(eq_constraints), org_ineq_constraints(ineq_constraints) 
       //local_state(nullptr)
      {
	this->obj = &org_obj;
	this->ineq_constraints = &ineq_constraints;
	this->eq_constraints = &eq_constraints;
      }

    ///  Upper bound
    algorithm::utils::UpperBound ubound;
    /// Current Box
    OptBox* cbox;
    /// objective function
    const functions::IFunction &org_obj;
    /// Equality constraints
    const FLst org_eq_constraints;
    /// Inequality constraints
    const FLst org_ineq_constraints;
    /// enclosure of objective function
    const functions::IFunction *obj;
    /// enclosure of inequality constraints
    const FLst* ineq_constraints;
    /// enclosure of equality constraints
    const FLst* eq_constraints;
    /// Stores Contractor discards
    std::map<std::string, unsigned> m_discards;
    /// Local worker state
//    local_opt_env_t *local_state;
  };

}

#endif /*__OPTENV_HPP__*/
