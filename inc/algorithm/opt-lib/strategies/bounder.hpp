#ifndef __BOUNDER_HPP__
#define __BOUNDER_HPP__

#include "functions/ifunction.hpp"

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt
{
  struct bounder_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	env.cbox->bound() = (*env.obj)(env.cbox->ref_X());
	return false;
      }
    
    virtual std::string name() const
      {
	return "Bounder";
      }
  };
}

#endif /*__BOUNDER_HPP__*/
