#include <utility/trans_mat.hpp>
#include <utility/utility.hpp>

using namespace core::arith;

namespace utility
{

  rmatrix trans_mat(rvector axis, mreal angle, const rvector &translation)
  {
    if(!utility::normalize(axis)) {
      assert(!"Could not normalize axis for rotation!");
      // TODO: Exception
    }

    mreal co(cos(angle * (d_pi() / 180.0)));
    mreal si(sin(angle * (d_pi() / 180.0)));

    rmatrix m(4,4);
    m = 1.0;
    m[0][0] = sqr(axis[0]) + co * (1 - sqr(axis[0]));
    m[1][1] = sqr(axis[1]) + co * (1 - sqr(axis[1]));
    m[2][2] = sqr(axis[2]) + co * (1 - sqr(axis[2]));
    m[0][1] = axis[0] * axis[1] * (1 - co) - axis[2] * si;
    m[0][2] = axis[0] * axis[2] * (1 - co) + axis[1] * si;
    m[1][0] = axis[0] * axis[1] * (1 - co) + axis[2] * si;
    m[1][2] = axis[1] * axis[2] * (1 - co) - axis[0] * si;
    m[2][0] = axis[0] * axis[2] * (1 - co) - axis[1] * si;
    m[2][1] = axis[1] * axis[2] * (1 - co) + axis[0] * si;
    for(unsigned i = 0; i < 3; i++)
      m[i][3] = translation[i];
    return m;
  }


  rmatrix trans_mat(rvector axis, mreal angle)
  {
    if(!normalize(axis)) {
      assert(!"Could not normalize axis for rotation!");
      // TODO: Exception
    }

    mreal co(cos(angle * (d_pi() / 180.0)));
    mreal si(sin(angle * (d_pi() / 180.0)));

    rmatrix m(3,3);
    m = 1.0;
    m[0][0] = sqr(axis[0]) + co * (1 - sqr(axis[0]));
    m[1][1] = sqr(axis[1]) + co * (1 - sqr(axis[1]));
    m[2][2] = sqr(axis[2]) + co * (1 - sqr(axis[2]));
    m[0][1] = axis[0] * axis[1] * (1 - co) - axis[2] * si;
    m[0][2] = axis[0] * axis[2] * (1 - co) + axis[1] * si;
    m[1][0] = axis[0] * axis[1] * (1 - co) + axis[2] * si;
    m[1][2] = axis[1] * axis[2] * (1 - co) - axis[0] * si;
    m[2][0] = axis[0] * axis[2] * (1 - co) - axis[1] * si;
    m[2][1] = axis[1] * axis[2] * (1 - co) + axis[0] * si;
    // for(unsigned i = 0; i < 3; i++)
    //   m[i][3] = translation[i];
    return m;
  }
/*
  core::arith::rmatrix trans_mat(const core::arith::rmatrix &rot, const core::arith::rvector &trans)
  {
    assert(num_cols(rot) == 3 && num_rows(rot) == 3 && size(trans) == 3);
    rmatrix m(4,4);
    m  = 1.0;
    for(unsigned i = 0; i < 3; i++) {
      for(unsigned j = 0; j < 3; j++)
	m[i][j] = rot[i][j];
      m[i][3] = translation[i];
    }
    return m;
  }
*/
}

