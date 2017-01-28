#ifndef __IIVP_HPP__
#define __IIVP_HPP__

#include <tuple>
#include <core/arithcore.hpp>

namespace functions
{
  class IVFunction;
}

namespace ivp 
{
  /// Representation of an initial value problem
  /** 
   * An initial value problem in \univermec is defined as
   * \f$\dot{x}(t, p, u(t)) = f(x(t), p, u(t))\f$ where x(t) is the
   * IVP's solution, p are parameters and u(t) are time-dependent
   * parameters. Furthermore, IIVP stores information of the initial
   * values \f$x(t_0) = x_0, u(t_0) = u_0\f$ and the starting time
   * \f$t_0\f$.
   *
   * \univermec only support autonomous systems.
   *
   */
  class IIVP 
  {
  public:
    IIVP() {}
    virtual ~IIVP() {}

    /** 
     * Gets the right-hand-side of the IVP problem
     * 
     * 
     * @return reference to right-hand-side
     */
    const functions::IVFunction& rhs() const 
      {
	return rhs_();
      }

    /** 
     * Returns a tuple describing the structure of rhs
     * 1. Dimension of x
     * 2. Dimension of p
     * 3. Dimension of u(t)
     * 
     * @return tuple describing the structure
     */
    std::tuple<unsigned, unsigned, unsigned> structure() const
      {
	return structure_();
      }

    /** 
     * Gets the parameter p of the problem
     * 
     * 
     * @return reference to problem parameters
     */
    const core::arith::ivector& p() const
      {
	return p_();
      }

    /** 
     * Gets the function describing the time-dependent parameters u(t)
     * 
     * 
     * @return reference to function describing u(t)
     */
    const functions::IVFunction& u() const
      {
	return u_();
      }

    /** 
     * Gets the starting time \f$t_0\f$
     * 
     * 
     * @return starting time t_0
     */
    core::arith::mreal t0() const
      {
	return t0_();
      }
    
    /** 
     * Gets \f$x(t_0) = x_0\f$
     * 
     * 
     * @return starting value \f$x_0\f$
     */
    core::arith::ivector x0() const
      {
	return x0_();
      }

    /** 
     * Gets \f$u(t_0) = u_0\f$
     * 
     * 
     * @return starting parameters \f$u_0\f$
     */
    core::arith::ivector u0() const
      {
	return u0_();
      }

    /** 
     * Polymorphics copy
     * 
     * 
     * @return copy of *this
     */
    IIVP* clone() const
      {
	return clone_();
      }

  private:
    virtual const functions::IVFunction& rhs_() const = 0;
    virtual std::tuple<unsigned, unsigned, unsigned> structure_() const  = 0;
    virtual const core::arith::ivector& p_() const = 0;
    virtual const functions::IVFunction& u_() const = 0;
    virtual core::arith::mreal t0_() const = 0;
    virtual core::arith::ivector x0_() const = 0;
    virtual core::arith::ivector u0_() const = 0;
    virtual IIVP* clone_() const = 0;
    
  private:
    IIVP(const IIVP&);
    IIVP& operator=(const IIVP&);
  };

  class ModifiableIIVP : public IIVP
  {
  public:
    void set_p(const core::arith::ivector &p)
      {
	set_p_(p);
      }

    void set_u(const functions::IVFunction &u)
      {
	set_u_(u);
      }
    
    void set_x0(const core::arith::interval &x0)
      {
	set_x0_(x0);
      }

  private:
    virtual void set_p_(const core::arith::ivector &p) = 0;
    virtual void set_u_(const functions::IVFunction &u) = 0;
    virtual void set_x0_(const core::arith::interval &x0) = 0;

  private:
    ModifiableIIVP(const ModifiableIIVP& other);
    ModifiableIIVP& operator=(const ModifiableIIVP& other);
  };
}

#endif /*__IIVP_HPP__*/
