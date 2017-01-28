#ifndef __VODE_HPP__
#define __VODE_HPP__

#include "stable.h"

extern "C" void dvode_(	void (*fcn)(int*,double*,double*,double*,double*,int*),//F
			int*,//NEQ
		     	double*,//Y
			double*,//T
			double*,//TOUT
			int*, //ITOL 1 or 2
			double*,//RTOL
			double*,//ATOL
                    	int*,//ITASK
                    	int*,//ISTATE
                    	int*,//IOPT
			double*,//RWORK
                    	int*,//LRW
                    	int*,//IWORK
                    	int*,//LIW
			void (*jac)(double*,double*,double*),//JAC
                    	int*,//MF
                    	double*,//RPAR
                    	int* //IPAR
			);

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
    void solve_ivp_vode(const ivp::IIVP &ivp, const core::arith::mreal &stop, std::ostream &os=std::cout, const core::arith::mreal &output_step=1);
  }
}


#endif /*__ODE_HPP__*/

