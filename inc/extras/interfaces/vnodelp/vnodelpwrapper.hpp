#ifndef __VNODELPWRAPPER_HPP__
#define __VNODELPWRAPPER_HPP__

#include <core/arithcore.hpp>
#include <vector>

namespace ivp 
{
  class IIVP;
}

namespace extras
{
  namespace interfaces 
  {
    struct ivp_callback_t
    {

      virtual void operator()(core::arith::ivector &v, const core::arith::interval &cur_t,
			      const core::arith::interval &stop_t) const = 0;
    };

    /** 
     * Solves an IVP problem using VNODE-LP
     * 
     * @param ivp IVP problem to solve
     * @param stop stop time for integreation
     * @param start start time for integration
     * @param os output stream for data
     * @param output_step constant step size (for output or changing control u(t))
     * @param cb callback function is called after completing  control step
     * @param cparams set to true if parameters of the problems change
     *
     * You need to set cparams to true iff you use time dependent
     * parameters u(t). In this case, you have to set output_step
     * equal to the step size of your measurements of u(t).
     *  
     * The output_set parameter does not influence VNODE-LPs
     * internal step size control. It is only used for getting
     * equidistant points for plotting and for the cb and cparams
     * options. 
     * For this it subdivides the integration intervals into
     * subintervals i=1...n 
     * start_1 = start 
     * stop_1 = start + output_step 
     * start_i+1 = stop_i 
     * stop_i+1 = stop_i + output_step 
     * and reports the results for these subintervals to os.
     * 
     * @return enclosure of solution at time stop, empty vector if an
     * error occurred
     */
    core::arith::ivector solve_ivp_vnodelp(const ivp::IIVP &ivp, const core::arith::mreal &stop, 
					   std::vector<core::arith::ivector> &sresults,
					   std::ostream &os = std::cout,
					   core::arith::mreal output_step = -1.0, const ivp_callback_t *cb = 0, 
					   bool cparams = false);
  }
}

#endif /*__VNODELPWRAPPER_HPP__*/
