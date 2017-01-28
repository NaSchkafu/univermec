#include "ivp/details/ivpimpl.hpp"

#include "functions/ivfunction.hpp"

namespace ivp
{
  namespace details
  {
    IVPImpl::IVPImpl(const functions::IVFunction &rhs, const core::arith::ivector &p,
		     const functions::IVFunction &u, const core::arith::mreal &t0,
		     const core::arith::ivector &x0, const core::arith::ivector &u0)
      :m_rhs(rhs), m_p(p), m_u(&u), m_t0(t0), m_x0(x0), m_u0(u0)
    {

      assert(mtl::size(m_x0) + mtl::size(m_p) + m_u->cdim() == rhs.dim() &&
	     "Dimensions of initial values and right hand side do not match!");
      assert(mtl::size(m_u0) == m_u->cdim() && "Dimensions of u(t) and u0 do not match!");
    }

    const functions::IVFunction& IVPImpl::rhs_() const
    {
      return m_rhs;
    }
    
    std::tuple<unsigned, unsigned, unsigned> IVPImpl::structure_() const
    {
      return std::make_tuple(mtl::size(m_x0), mtl::size(m_p), m_u->cdim());
    }

    const core::arith::ivector& IVPImpl::p_() const
    {
      return m_p;
    }

    const functions::IVFunction& IVPImpl::u_() const
    {
      return *m_u;
    }

    core::arith::mreal IVPImpl::t0_() const
    {
      return m_t0;
    }

    core::arith::ivector IVPImpl::x0_() const
    {
      return m_x0;
    }

    core::arith::ivector IVPImpl::u0_() const
    {
      return m_u0;
    }

    IIVP* IVPImpl::clone_() const
    {
      return new IVPImpl(m_rhs, m_p, *m_u, m_t0, m_x0, m_u0);
    }
  }
}
