#ifndef __BCUBOUND_HPP__
#define __BCUBOUND_HPP__

#include "functions/contractors/bccontract.hpp"
#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt 
{
  /// Performs the midpoint test on the element using the bound on OptBox
  struct bc_ubound_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	if(env.ubound() < core::arith::infinity()) {
	  functions::contractors::BCContract bc(*env.obj);
	  return bc.prune(*env.cbox, core::arith::interval(-core::arith::infinity(), env.ubound()));
	}
	return false;
      }

    virtual std::string name() const
      {
	return "BC UBound";
      }
  };
}


#endif /*__BCUBOUND_HPP__*/
