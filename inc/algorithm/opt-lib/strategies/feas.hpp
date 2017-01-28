#ifndef __FEAS_HPP__
#define __FEAS_HPP__

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt 
{
  /// Performs the midpoint test on the element using the bound on OptBox
  struct feas_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	return env.cbox->updateFeas() == FEAS_INFEAS;
      }

    virtual std::string name() const
      {
	return "Feasibility test";
      }
  };
}


#endif /*__FEAS_HPP__*/
