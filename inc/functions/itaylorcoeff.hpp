#ifndef __ITAYLORCOEFF_HPP__
#define __ITAYLORCOEFF_HPP__

#include <vector>
#include <core/arithcore.hpp>

namespace functions
{
  /// Interface for calculating Taylor coeffients of a function
  /**
   * This interface is used to calculate the Taylor coefficient of
   * an ODE the right-hand side of which is represented by the 
   * @ref IVFunction instance to which this @ITaylorCoeff instance
   * is associated. Additionally, the Taylor coefficients of the
   * variational equation can be derived.
   */
  class ITaylorCoeff 
  {
  public:
    /** 
     * Returns the Taylor coefficients of the ODE's solution.
     * 
     * @param args arguments arguments to the right hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with n entries for the coefficients of the member functions \f$f_1, \ldots, f_m\f$
     */
    std::vector<core::arith::rvector> taylor_coeff_ode(const core::arith::rvector &args, unsigned order) const
      {
	return taylor_coeff_ode_(args, order);
      }

    /** 
     * Returns the Taylor coefficients of the ODE's solution.
     * 
     * @param args arguments arguments to the-right hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with m entries for the coefficients of the member functions \f$f_1, \ldots, f_m\f$
     */
    std::vector<core::arith::ivector> taylor_coeff_ode(const core::arith::ivector &args, unsigned order) const
      {
	return taylor_coeff_ode_(args, order);
      }

    /** 
     * Returns the Taylor coefficients of the ODE's solution.
     * 
     * @param args arguments arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with m entries for the coefficients of the member functions \f$f_1, \ldots, f_m\f$
     */
    std::vector<core::arith::avector> taylor_coeff_ode(const core::arith::avector &args, unsigned order) const
      {
	return taylor_coeff_ode_(args, order);
      }

    /** 
     * Returns the Taylor coefficients of the ODE's solution.
     * 
     * @param args arguments arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with m entries for the coefficients of the member functions \f$f_1, \ldots, f_m\f$
     */
    std::vector<core::arith::tvector> taylor_coeff_ode(const core::arith::tvector &args, unsigned order) const
      {
	return taylor_coeff_ode_(args, order);
      }

    /** 
     * Taylor coeffients of the variational equation of an ODE
     * 
     * @param args arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with n entries containing a vector with m entries
     */
    std::vector<std::vector<core::arith::rvector> > taylor_coeff_ode_df(const core::arith::rvector &args, unsigned order) const
      {
	return taylor_coeff_ode_df_(args, order);
      }

    /** 
     * Taylor coeffients of the variational equation of an ODE
     * 
     * @param args arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with n entries containing a vector with m entries
     */
    std::vector<std::vector<core::arith::ivector> > taylor_coeff_ode_df(const core::arith::ivector &args, unsigned order) const
      {
	return taylor_coeff_ode_df_(args, order);
      }

    /** 
     * Taylor coeffients of the variational equation of an ODE
     * 
     * @param args arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with n entries containing a vector with m entries
     */
    std::vector<std::vector<core::arith::avector> > taylor_coeff_ode_df(const core::arith::avector &args, unsigned order) const
      {
	return taylor_coeff_ode_df_(args, order);
      }

    /** 
     * Taylor coeffients of the variational equation of an ODE
     * 
     * @param args arguments to the right-hand side
     * @param order order maximum order of Taylor expansion
     * 
     * @return vector with n entries containing a vector with m entries
     */
    std::vector<std::vector<core::arith::tvector> > taylor_coeff_ode_df(const core::arith::tvector &args, unsigned order) const
      {
	return taylor_coeff_ode_df_(args, order);
      }



  private:
    virtual std::vector<core::arith::rvector> taylor_coeff_ode_(const core::arith::rvector &args, unsigned order) const = 0;
    virtual std::vector<core::arith::ivector> taylor_coeff_ode_(const core::arith::ivector &args, unsigned order) const = 0;
    virtual std::vector<core::arith::avector> taylor_coeff_ode_(const core::arith::avector &args, unsigned order) const = 0;
    virtual std::vector<core::arith::tvector> taylor_coeff_ode_(const core::arith::tvector &args, unsigned order) const = 0;

    virtual std::vector<std::vector<core::arith::rvector> > taylor_coeff_ode_df_(const core::arith::rvector &args, unsigned order) const = 0;
    virtual std::vector<std::vector<core::arith::ivector> > taylor_coeff_ode_df_(const core::arith::ivector &args, unsigned order) const = 0;
    virtual std::vector<std::vector<core::arith::avector> > taylor_coeff_ode_df_(const core::arith::avector &args, unsigned order) const = 0;
    virtual std::vector<std::vector<core::arith::tvector> > taylor_coeff_ode_df_(const core::arith::tvector &args, unsigned order) const = 0;

  };
}

#endif /*__ITAYLORCOEFF_HPP__*/
