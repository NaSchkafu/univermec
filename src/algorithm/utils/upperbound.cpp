/**
 * @file   upperbound.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:31:55 2009
 *
 * @brief  Impl. of UpperBound class
 *
 *
 */


#include <algorithm/utils/upperbound.hpp>

namespace algorithm
{
  namespace utils
  {
    template <typename PTYPE>
    void UpperBoundImpl<PTYPE>::sync(UpperBoundImpl<PTYPE> &other)
    {
      if(other.m_uBound > m_uBound) {
        other.m_uBound = m_uBound;
        other.m_uPoint = m_uPoint;
        other.m_validPoint = m_validPoint;
      }
      else {
        m_uBound = other.m_uBound;
        m_uPoint = other.m_uPoint;
        m_validPoint = other.m_validPoint;
      }
    }

    template <typename PTYPE>
    bool UpperBoundImpl<PTYPE>::update(const PTYPE &point,
				       const core::arith::mreal &ub)
    {
      if(ub < m_uBound) {
//	std::cout << "Update UpperBound von " << m_uBound << " auf " << ub
//		  << " gefunden bei Box " << point << std::endl;
        m_uBound = ub;
        m_uPoint = point;
        m_validPoint = true;
        return true;
      }
      return false;
    }

    template <typename PTYPE>
    bool UpperBoundImpl<PTYPE>::update(const core::arith::mreal &ub)
    {
      if(ub < m_uBound) {
	m_uBound = ub;
	m_validPoint = false;
	return true;
      }
      return false;
    }

    template <typename PTYPE>
    UpperBoundImpl<PTYPE>::UpperBoundImpl(const core::arith::mreal &uBound)
      :m_uBound(uBound), m_validPoint(false)
    {
    }

    template <typename PTYPE>
    UpperBoundImpl<PTYPE>::UpperBoundImpl(const core::arith::mreal &uBound, const PTYPE &uPoint)
      :m_uBound(uBound), m_uPoint(uPoint), m_validPoint(true)
    {
    }

    template <typename PTYPE>
    UpperBoundImpl<PTYPE>::UpperBoundImpl()
      :m_uBound(core::arith::infinity()), m_validPoint(false)
    {  }

    template class UpperBoundImpl<core::arith::rvector>;
    template class UpperBoundImpl<core::arith::ivector>;
  }

}


