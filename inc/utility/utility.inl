// -*-c++-*-
template<typename T>
typename core::arith::ArithTraits<T>::base_t length(const T &v)
{
  typename core::arith::ArithTraits<T>::base_t l(0);
  unsigned dim = size(v);
  for(unsigned i = 0; i < dim; i++)
    l += sqr(v[i]);
  l = sqrt(l);
  return l;
}

template<typename T>
bool normalize(T &v)
{
  typename core::arith::ArithTraits<T>::base_t l(length(v));
  v /= l;
  return true;
}

