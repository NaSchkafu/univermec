#ifndef __MREALADAPT_HPP__
#define __MREALADAPT_HPP__

#include <cmath>
#include <core/arithcore.hpp>

namespace core
{
  namespace arith
  {

    inline bool is_infinity(const mreal& r)
    {
#ifdef _WIN32
      return _finite(to_double(r));
#else
      return std::isinf(to_double(r));
#endif
    }

    // TODO: unterscheiden!
    inline bool is_quiet_NaN(const mreal& r)
    {
#ifdef _WIN32
      return _isnan(to_double(r));
#else
      return std::isnan(to_double(r));
#endif
    }

    inline bool is_signaling_NaN(const mreal& r)
    {
 #ifdef _WIN32
      return _isnan(to_double(r));
#else
      return std::isnan(to_double(r));
#endif
    }

    inline bool is_nan(const mreal& r)
    {
#ifdef _WIN32
      return _isnan(to_double(r));
#else
      return std::isnan(to_double(r));
#endif
    }

    // inline bool is_special(const mreal &r) 
    // {
    //   return is_infinity(r) || is_nan(r);
    // }

    inline mreal m_pi()
    {
      return M_PI;
    }

    inline mreal mreal_pi()
    {
      return M_PI;
    }
  }
}


#endif /*__MREALADAPT_HPP__*/
