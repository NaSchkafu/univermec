#ifndef __MIDPOINT_HPP__
#define __MIDPOINT_HPP__

#include "../optenv.hpp"
#include "../optbox.hpp"
#include "../stratele.hpp"

namespace GlobOpt 
{
  /// Performs the midpoint test on the element using the bound on OptBox
  struct mid_point_t : public strat_ele_t 
  {
    virtual bool apply(opt_worker_state_t &env) const
      {
	return inf(env.cbox->bound()) > env.ubound();
      }

    virtual std::string name() const
      {
	return "Mid-point test";
      }
  };
}

#endif /*__MIDPOINT_HPP__*/
