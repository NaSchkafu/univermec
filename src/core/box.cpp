#include <core/box.hpp>

namespace core 
{
  Box* Box::bisect(unsigned dir, const arith::interval &p)
  {
    arith::ivector r(m_x);
    r[dir] = arith::interval(sup(p), sup(m_x[dir]));
    m_x[dir] = arith::interval(inf(m_x[dir]), inf(p));
    return new Box(r);
  }

  Box::~Box()
  {}
}
