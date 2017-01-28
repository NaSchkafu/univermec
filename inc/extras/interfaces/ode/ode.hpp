#ifndef __ODE_HPP__
#define __ODE_HPP__

#include "stable.h"

extern "C" void ode_(void (*fcn)(double*,double*,double*),
                    int*,double*,double*,double*,double*,double*,
                    int*,double*,int*);

namespace ivp 
{
  class IIVP;
}

namespace extras
{
  namespace interfaces
  {
    /**
     * Solves an initial value problem using ode
     * (This function is not reentrant!)
     *
     * @param ivp
     * @param param
     */
    void solve_ivp_ode(const ivp::IIVP &ivp, const core::arith::mreal &stop);
  }
}


#endif /*__ODE_HPP__*/

