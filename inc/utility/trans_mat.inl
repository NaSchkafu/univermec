// -*-c++-*-
template<typename T>
typename core::arith::ArithTraits<T>::mat_t trans_mat(
  const typename core::arith::ArithTraits<T>::vec_t &axis, const T& angle,
  const typename core::arith::ArithTraits<T>::vec_t &translation)
{

  if(!normalize(axis)) {
    assert(!"Could not normalize axis for rotation!");
    // TODO: Exception
  }

  T co(cos(angle * core::arith::ArithTraits<T>::convert(core::arith::interval_pi() / 180)));
  T si(sin(angle * core::arith::ArithTraits<T>::convert(core::arith::interval_pi() / 180)));
  typename core::arith::ArithTraits<T>::mat_t m(4,4);
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


template<typename T>
typename core::arith::ArithTraits<T>::mat_t trans_mat(
  const typename core::arith::ArithTraits<T>::vec_t &axis, const T& angle)
{
  if(!normalize(axis)) {
    assert(!"Could not normalize axis for rotation!");
    // TODO: Exception
  }

  T co(cos(angle * core::arith::ArithTraits<T>::convert(core::arith::interval_pi() / 180)));
  T si(sin(angle * core::arith::ArithTraits<T>::convert(core::arith::interval_pi() / 180)));
  typename core::arith::ArithTraits<T>::mat_t m(3,3);
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
  return m;
}

template<typename T>
T trans_mat(const T& rot, const typename 
	    core::arith::ArithTraits<typename core::arith::ArithTraits<T>::base_t>::vec_t &trans)
{
  assert(num_cols(rot) == num_rows(rot) && num_cols(rot) == size(trans) && "Dimensions do not match!");
  unsigned dim = size(trans);
  T m(dim+1, dim+1);
  m = 1.0;
  for(unsigned i = 0; i < dim; i++) {
    for(unsigned j = 0; j < dim; j++) 
      m[i][j] = rot[i][j];
    m[i][dim] = trans[i];
  }
  return m;
}
