#ifndef __UUBOUND_HPP__
#define __UUBOUND_HPP__

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt 
{

  struct u_ubound_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	if(env.cbox->feasible() || env.cbox->feasible(mid(env.cbox->ref_X()))) {
	  env.ubound.update(mid(env.cbox->ref_X()), sup((*env.obj)(core::arith::ivector(mid(env.cbox->ref_X())))));
	}
	return false;
      }

    virtual std::string name() const
      {
	return "Update uBound";
      }
  };
  
}

#endif /*__UUBOUND_HPP__*/
