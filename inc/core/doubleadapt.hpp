#ifndef __DOUBLEADAPT_HPP__
#define __DOUBLEADAPT_HPP__

#include <cmath>

namespace core
{
  namespace arith
  {

    inline bool is_infinity(double r)
    {
#ifdef _WIN32
		return _finite(r);
#else
		return std::isinf(r);
#endif
    }

    // TODO: unterscheiden!
    inline bool is_quiet_NaN(double r)
    {
#ifdef _WIN32
		return _isnan(r);
#else
		return std::isnan(r);
#endif
    }

    inline bool is_signaling_NaN(double r)
    {
 #ifdef _WIN32
		return _isnan(r);
#else
		return std::isnan(r);
#endif
    }

    inline bool is_nan(double r)
    {
#ifdef _WIN32
		return _isnan(r);
#else
		return std::isnan(r);
#endif
    }

    inline double d_pi()
    {
      return M_PI;
    }
  }
}


#endif /*__DOUBLEADAPT_HPP__*/
