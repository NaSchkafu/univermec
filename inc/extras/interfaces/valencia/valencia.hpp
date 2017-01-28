#ifndef __VALENCIA_HPP__
#define __VALENCIA_HPP__

#include "stable.h"

namespace ivp 
{
  class IIVP;
}

namespace extras
{
  namespace interfaces
  {
    /**
     * Solves an initial value problem using ValEncIA-IVP
     * (This function is not reentrant!)
     *
     * @param ivp
     * @param param
     */
    void solve_ivp_valencia(const ivp::IIVP &ivp, const core::arith::mreal &stop,
                const core::arith::mreal &step, const std::string &file);
  }
}


#endif /*__VALENCIA_HPP__*/

