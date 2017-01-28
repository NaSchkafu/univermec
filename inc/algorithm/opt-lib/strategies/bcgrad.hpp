#ifndef __BCGRAD_HPP__
#define __BCGRAD_HPP__

#include "functions/contractors/bccontract.hpp"
#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"
#include "functions/iderivative.hpp"
#include "functions/iffeatures.hpp"

namespace GlobOpt 
{
  /// Performs box consistency using the constraints
  struct bc_grad_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	unsigned dim(env.obj->dim());
	const functions::IDerivative& dobj = *env.obj->features().derivative();
	for(unsigned i = 1; i <= dim; i++) {
	  functions::contractors::BCContract bc(dobj.grad(i));  
	  if(bc.prune(*env.cbox, core::arith::interval(0, 0.0)))
	    return true;
	}
	return false;
      }

    virtual std::string name() const
      {
	return "BC Grad";
      }
  };
}


#endif /*__BCGRAD_HPP__*/
