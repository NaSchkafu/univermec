// -*-c++-*-
/**
 * @file   helper.inl
 * @author Stefan Kiel
 * @date   Mon May 11 14:23:49 2009
 *
 * @brief  Inline functions for helpers
 *
 *
 */


inline core::arith::rvector doubleToVec(const double *n, unsigned int size)
{
  core::arith::rvector vec(size);
  for(unsigned int i = 0; i < size; i++)
    vec[i] = n[i];
  return vec;
}

template<class T>
core::arith::interval rLength(const T &v)
{
  core::arith::mreal result(0);
  for(int i = Lb(v); i <= Ub(v); i++)
    result += core::arith::sqr(v[i]);
  return core::arith::enclosure_cast<core::arith::interval> (sqrt(result));
}


template<class T>
core::arith::interval iLength(const T &v)
{
  core::arith::interval result(0,0);
  for(int i = Lb(v); i <= Ub(v); i++)
    result += core::arith::sqr(v[i]);
  return sqrt(result);
}


template<class T>
T copyVec(const T& source, unsigned int to, unsigned int )
{
  assert(false && "Bugged?!");
  T res(to);
  for(unsigned int i = 0; i <= to; i++)
    res[i] = source[i];
  return res;
}
