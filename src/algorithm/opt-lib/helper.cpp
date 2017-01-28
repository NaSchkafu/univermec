/**
 * @file   helper.cpp
 * @author Stefan Kiel
 * @date   Mon May 11 14:23:15 2009
 *
 * @brief  Impl. of various helper functions
 *
 *
 */


#include <algorithm/opt-lib/helper.hpp>
#include <algorithm/opt-lib/exception.hpp>
#include <core/cxsccomp.hpp>

namespace MT {
  std::vector<core::arith::interval> split(const core::arith::interval &X, const core::arith::mreal &p)
  {
    std::vector<core::arith::interval> v;
    v.push_back(core::arith::interval(inf(X), p));
    v.push_back(core::arith::interval(p, sup(X)));
    return v;
  }

  core::arith::ivector split(const core::arith::ivector &v, const core::arith::mreal &p, int direction, bool first)
  {
    core::arith::ivector r(v);
    if(first)
      r[direction] = core::arith::interval(inf(r[direction]), p);
    else
      r[direction] = core::arith::interval(p, sup(r[direction]));
    return r;
  }

  core::arith::ivector split(const core::arith::ivector &v, int direction, bool first)
  {
    return split(v, mid(v[direction]), direction, first);
  }



  core::arith::mreal w(const core::arith::ivector &v)
  {
    core::arith::mreal ub = Ub(v);
    core::arith::mreal r(0.0);
    for(int i = Lb(v); i < ub; i++) {
      if(core::arith::wid(v[i]) > r) {
        r = core::arith::wid(v[i]);
      }
    }
    return r;
  }

  int maxDim(const core::arith::ivector &v)
  {
    int ub = Ub(v);
    core::arith::mreal r(0.0);
    int i = -1;
    for(int j = Lb(v); j <= ub; j++) {
      if(core::arith::wid(v[j]) > r) {
        r = core::arith::wid(v[j]);
        i = j;
      }
    }
    return i;
  }


  std::vector<core::arith::interval> solveInEq(const core::arith::interval& U, const core::arith::interval &V)
  {
    // see Hansen 6.2.4
    core::arith::mreal a = inf(U), b = sup(U);
    core::arith::mreal c = inf(V), d = sup(V);
    std::vector<core::arith::interval> result;
    if(a <= 0 && d < 0)
      result.push_back(core::arith::interval(-a/d, core::arith::infinity()));
    else if(a > 0 && c < 0 && d <= 0)
      result.push_back(core::arith::interval(-a/c, core::arith::infinity()));
    else if(a <= 0 && c <= 0  && 0 <= d)
      result.push_back(core::arith::interval(-core::arith::infinity(), core::arith::infinity()));
//      result.push_back(core::arith::interval(-core::arith::infinity()));
    else if(a > 0 && c < 0 && 0 < d) {
      result.push_back(core::arith::interval(-core::arith::infinity(), -a/d));
      result.push_back(core::arith::interval(-a/c, core::arith::infinity()));
    }
    else if(a <= 0 && c > 0)
      result.push_back(core::arith::interval(-core::arith::infinity(), -a/c));
    else if(a > 0 && c >= 0 && d > 0)
      result.push_back(core::arith::interval(-core::arith::infinity(), -a/d));
    else if(a > 0 && (c == d) && c == 0) { // leere Lsg hier ausgeben?
      result.push_back(core::arith::interval(-core::arith::infinity(), -core::arith::infinity()));
      result.push_back(core::arith::interval(core::arith::infinity(), core::arith::infinity()));
    }
    else {
      //TODO: alter exception
      THROW(GlobOptException, "illegal condition!");
    }
    return result;
  }

  core::arith::interval unite(const core::arith::interval &X1, const core::arith::interval &X2)
  {
    if(sup(X1) <= inf(X2))
      return core::arith::interval(inf(X1), sup(X2));
    return core::arith::interval(inf(X2), sup(X1));
  }

  bool isRowZero(const core::arith::rmatrix &m, int i)
  {
    for(int j = core::arith::Lb(m, 2); j <= core::arith::Ub(m, 2); j++)
      if(m[i][j] != 0)
        return false;
    return true;
  }

  void switchRows(core::arith::rmatrix& m, int row1, int row2)
  {
    //DONE_C: Neuimplementierung für mtl.
    //Geht bestimmt noch schoener
    core::arith::rvector v(num_cols(m));
    for(unsigned i = 0; i < num_cols(m); i++)
      v[i] = m[row1][i];

    for(unsigned i = 0; i < num_cols(m);i++)
      m[row1][i] = m[row2][i];

    for(unsigned i = 0; i < num_rows(v);i++)
      m[row2][i] = v[i];
  }

  bool inside(const core::arith::rvector &p, const core::arith::ivector &b)
  {
    if(!(core::arith::Ub(p) - core::arith::Lb(p) - core::arith::Ub(b) + core::arith::Lb(b))) {
      for(int i = core::arith::Lb(p); i < core::arith::Ub(p); i++) {
        if(!(p[i] <=  b[core::arith::Lb(b) + i - core::arith::Lb(p)])) {
          return false;
        }
      }
    } else {
      THROW(IllegalArguments, "Dimensions do not match!");
    }
    return true;
  }

  core::arith::mreal totalWidth(const core::arith::ivector &x)
  {
    core::arith::mreal wi(0);
    for(int i = core::arith::Lb(x); i <= core::arith::Ub(x); i++)
      wi += wid(x[i]);
    return wi;
  }

  bool IsEmpty(const core::arith::ivector &v)
  {
    int ub = Ub(v);
    for(int i = Lb(v); i <= ub; i++)
      if(core::arith::isEmpty(v[i]))
        return true;
    return false;
  }

  bool overlap(const core::arith::interval &i1, const core::arith::interval &i2)
  {
    return ((inf(i1) <= i2) || (sup(i1) <= i2)
            || (inf(i2) <= i1) || (sup(i2) <= i1));
  }

  core::arith::imatrix iIdentity(int size)
  {
    core::arith::imatrix m(size,size);
    for(int i=0; i<size; i++)
      for(int j=0; j<size; j++)
        m[i][j] = i==j ? 1 : 0;
    return m;
  }

  bool isSpecialValue(const core::arith::interval &i)
  {
    return core::arith::is_infinity(to_double(inf(i))) || core::arith::is_infinity(to_double(sup(i))) ||
      core::arith::is_quiet_NaN(inf(i)) || core::arith::is_quiet_NaN(sup(i)) ||
      core::arith::is_signaling_NaN(inf(i)) || core::arith::is_signaling_NaN(sup(i));
  }

}


