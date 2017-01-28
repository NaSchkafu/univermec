
#include "convexpolyhedron.inl"

namespace objects
{
  namespace details
  {
    mreal max(const mreal &a, const mreal &b)
    {
      return (a > b) ? a : b;
    }

    interval max(const interval &a, const interval &b)
    {
      return interval(std::max(inf(a), inf(b)), std::max(sup(a), sup(b)));
    }

    aaf max(const aaf &a, const aaf &b)
    {
      // FixMe: ???
      return lossless_cast<aaf>(max(enclosure_cast<interval>(a),
				    enclosure_cast<interval>(b)));
    }

    taylormodel max(const taylormodel &a, const taylormodel &b)
    {
      return lossless_cast<taylormodel>(max(enclosure_cast<interval>(a),
					    enclosure_cast<interval>(b)));
    }
  }
}

