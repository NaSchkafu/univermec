#include <utility/utility.hpp>


using namespace core::arith;

namespace utility
{
  ivector unit_box(unsigned dim)
  {
    core::arith::ivector v(dim, interval(0.0, 1.0));
    return v;
  }

  unsigned max_dim(const ivector &box)
  {
    unsigned dim = 0;
    core::arith::mreal width(0.0);
    for(unsigned i = 0; i < size(box); i++)
      if(width < wid(box[i])) {
        width = wid(box[i]);
        dim = i;
      }
    return dim;
  }

  boost::array<interval,2> split(const interval &iv, mreal point)
  {
    assert(point <= iv && "Split failed. Point is not inside iv!");
    boost::array<interval, 2> r = { { interval(inf(iv), point), interval(point, sup(iv)) } };
    return r;
  }

  boost::array<ivector,2> split(const ivector &box, unsigned dim, mreal point)
  {
    ivector v1(box), v2(box);
    const boost::array<interval, 2> siv(split(box[dim], point));
    v1[dim] = siv[0];
    v2[dim] = siv[1];
    boost::array<ivector,2> r = { { v1, v2 } };
    return r;
  }

//     mreal w(const core::arith::ivector &X)
//     {
//       mreal width(0.0);
//       for(unsigned i = 0; i < size(X); i++)
//         width = std::max(width, w(X[i]));
//       return width;
//     }

  mreal box_volume(const core::arith::ivector &X)
  {
    mreal vl = 1.0;
    unsigned dim = size(X);
    assert(dim && "volume of non existant box requested!");
    for(unsigned i = 0; i < dim; i++)
      vl *= wid(X[i]);
    return vl;
  }

  imatrix iv_identity(unsigned size)
  {
    imatrix m(size, size);
    set_to_zero(m);
    for(unsigned i = 0; i < size; i++)
      m[i][i] = interval(1,1);
    return m;
  }

  // core::arith::ivector operator|(const core::arith::ivector &v1, const core::arith::ivector &v2)
  // {
  //   assert(size(v1) == size(v2));
  //   unsigned dim = size(v1);
  //   ivector r(dim);
  //   for(unsigned i = 0; i < dim; i++) {
  //     r[i] = v1[i] | v2[i];
  //   }
  //   return r;
  // }

  bool normalize(core::arith::ivector &v)
  {
    core::arith::interval l(length(v));
    if(0 <= l)
      return false;
    v /= l;
    return true;
  }

}



