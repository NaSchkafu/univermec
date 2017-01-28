#ifndef __BCCONSTRAINTS_HPP__
#define __BCCONSTRAINTS_HPP__

#include "functions/contractors/bccontract.hpp"
#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt 
{
  /// Performs box consistency using the constraints
  struct bc_const_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	for(auto cit(env.ineq_constraints->begin()); cit != env.ineq_constraints->end(); ++cit) {
	  functions::contractors::BCContract bc(*cit);
	  if(bc.prune(*env.cbox, core::arith::interval(-core::arith::infinity(), 0.0)))
	    return true;
	}
	return false;
      }

    virtual std::string name() const
      {
	return "BC Const";
      }
  };
}


#endif /*__BCCONSTRAINTS_HPP__*/
