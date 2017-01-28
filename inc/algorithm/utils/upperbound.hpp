/**
 * @file   upperbound.hpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:32:18 2009
 *
 * @brief  Header for UpperBound class
 *
 *
 */


#ifndef __UPPERBOUND_HPP__
#define __UPPERBOUND_HPP__ __UPPERBOUND_HPP__

#include <core/arithcore.hpp>

namespace algorithm
{
  namespace utils
  {
    /// class for managing the upper bound in the global optimization algorithm
    template <typename PTYPE>
    class UpperBoundImpl {
    public:
      /**
       * Constructor
       *
       * @param uBound initial ubound
       */
      explicit UpperBoundImpl(const core::arith::mreal &uBound);

      /**
       * Constructor
       *
       * @param uBound initial ubound
       * @param uPoint feasible point where uBound has been acquired
       */
      UpperBoundImpl(const core::arith::mreal &uBound, const PTYPE &m_uPoint);

      /**
       * Constructor
       *
       */
      UpperBoundImpl();

      /**
       * Gets the upper bound on the global minimum
       *
       * @return upper bound
       */
      inline const core::arith::mreal& bound() const { return m_uBound; }

      /**
       * Convenience method for bound()
       *
       *
       * @return upper bound
       */
      inline const core::arith::mreal& operator()() const{ return m_uBound; }


      /**
       * Get a point where sup(F(uPoint()) == uBound() holds
       * only valid if hasUPoint() is true
       *
       * @return point
       */
      inline const PTYPE& point() const { return m_uPoint; }

      /**
       * Checks if a point for the current uBound is known
       *
       *
       * @return
       */
      inline bool hasPoint() const { return m_validPoint; }

      /**
       * Syncs the two upper bound object
       *
       *
       * @param other other object
       */
      void sync(UpperBoundImpl<PTYPE> &other);

      /**
       * Tries to update upper bound
       *
       * @param point feasible(!) point
       *
       * @return true if the upper bound has been improved
       */
      bool update(const PTYPE &ubpoint,
                  const core::arith::mreal &value);

      bool update(const core::arith::mreal &value);

    private:
      core::arith::mreal m_uBound;
      PTYPE m_uPoint;
      bool m_validPoint;
    };

    typedef UpperBoundImpl<core::arith::rvector> UpperBound;
    typedef UpperBoundImpl<core::arith::ivector> UpperBoundIV;
  }
}

#endif
