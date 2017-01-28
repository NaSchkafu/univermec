#ifndef __ILIEENCL_HPP__
#define __ILIEENCL_HPP__

#include "../iencl.hpp"

namespace functions 
{
  namespace encl 
  {
    /// Class for providing Implicit Linear Interval Estimations of a function
    /**
     * An implicit linear interval estimation is a thick hyperplane enclosing the zeros
     * of an implicit function.
     * Formally: Given is a function f:R^n -> R, a box X
     * Let further F the level set  of f with F = { x | f(x) = 0 }
     * Then for all x \in (X \cap F) we have:
     * 0 \in L(x)
     * whereas L(x) is an ILIE for f over X.
     *
     * The created ILIEs also support the ICustomConsistency implemented through Hansen's
     * hull consisteny technique. The functor type is Hyperplane_F provided
     * through the hyperplanefwd.hpp header.
     */
    class ILIEEncl : public IEncl
    {
    public:
      /** 
       * Ctor
       * 
       * @param f implicit function which should be linearized
       * 
       */
      explicit ILIEEncl(const IFunction& f);

      /// dtor
      virtual ~ILIEEncl();

      /** 
       * Calculates an ILIE for an affine box and an enclosure
       * 
       * @param affBox affine box
       * @param encl enclosure
       * @param affErr error symbols describing linear dependency on input
       * 
       * @return function describing ILIE or 0
       */
      static functions::IFunction* calc_ilie(const core::arith::avector &affBox,
					     const core::arith::aaf &encl,
					     const core::arith::aaf::error_comb_t &affErr);

    private:
      // IEncl implementation
      virtual bool is_global_() const;
      virtual IFunction* encl_(const::core::arith::ivector &box) const;
      virtual IFunction* encl_() const;

    private:
      const IFunction& m_F;
    };
  }
}

#endif /*__ILIEENCL_HPP__*/
