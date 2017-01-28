#ifndef __IVPIMPL_HPP__
#define __IVPIMPL_HPP__

#include "../iivp.hpp"

namespace ivp 
{
  namespace details 
  {
    class IVPImpl : public IIVP 
    {
    public:
      IVPImpl(const functions::IVFunction &rhs, const core::arith::ivector &p,
	      const functions::IVFunction &u, const core::arith::mreal &t0,
	      const core::arith::ivector &x0, const core::arith::ivector &u0);
      virtual ~IVPImpl() {}
 
    private:
      const functions::IVFunction& rhs_() const;
      std::tuple<unsigned, unsigned, unsigned> structure_() const;
      const core::arith::ivector& p_() const;
      const functions::IVFunction& u_() const;
      core::arith::mreal t0_() const;
      core::arith::ivector x0_() const;
      core::arith::ivector u0_() const;
      IIVP* clone_() const;

    private:
      const functions::IVFunction &m_rhs;
      core::arith::ivector m_p;
      const functions::IVFunction *m_u;
      core::arith::mreal m_t0;
      core::arith::ivector m_x0;
      core::arith::ivector m_u0;
    };
  }
}

#endif /*__IVPIMPL_HPP__*/
