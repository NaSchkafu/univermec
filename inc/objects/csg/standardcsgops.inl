// -*-c++-*-
// inl
// template<typename TB>
// CSGStandardTransform<TB>::CSGStandardTransform(const ICSGNode &n, const  MAT_T &m, const MAT_T &im, 
// 					       const VEC_T &t, const VEC_T &it)
//   :m_node(n), m_m(m), m_im(im), m_t(t), m_it(it)
// {
// }

// template<typename TB>
// unsigned CSGStandardTransform<TB>::dim() const
// {
//   return m_node.dim();
// }

// template<typename TB>
// std::string CSGStandardTransform<TB>::text() const
// {
//   std::stringstream sstream;
//   sstream << "StandardCSG Transformation Node with Transformation Matrix ";
//   sstream << m_m;
//   return sstream.str();
// }

// template<typename TB>
// template<typename T>
// typename T::value_type CSGStandardTransform<TB>::operator()(const T& args) const
// {
//   return m_node.cf()(static_cast<T>(core::arith::ArithTraits<typename core::arith::ArithTraits
// 				    <typename T::value_type>::mat_t>::force_convert(m_im)*args+
// 				    core::arith::ArithTraits<typename core::arith::ArithTraits
// 				    <typename T::value_type>::vec_t>::force_convert(m_it)));
// }

// template<typename TB>
// typename CSGStandardTransform<TB>::MAT_T CSGStandardTransform<TB>::rot() const
// {
//   return m_m;
// }

// template<typename TB>
// typename CSGStandardTransform<TB>::MAT_T CSGStandardTransform<TB>::i_rot() const
// {
//   return m_im;
// }

// template<typename TB>
// typename CSGStandardTransform<TB>::VEC_T CSGStandardTransform<TB>::trans() const
// {
//   return m_t;
// }

// template<typename TB>
// typename CSGStandardTransform<TB>::VEC_T CSGStandardTransform<TB>::i_trans() const
// {
//   return m_it;
// }

// template<typename T>
// typename T::value_type CSGStandardTransform<core::arith::mreal>::operator()(const T& args) const
// {
//   return m_node.cf()(static_cast<T>(m_im*static_cast<T>((args+m_it))));
// }

template<typename T>
typename T::value_type CSGStandardInversion::operator()(const T& args) const
{
  return -m_node.cf()(args);
}

template<typename T>
typename T::value_type CSGStandardUnion::operator()(const T& args) const
{
  return m_node1.cf()(args) | m_node2.cf()(args);
}

template<typename T>
typename T::value_type CSGStandardIntersection::operator()(const T& args) const
{
  return -(-m_node1.cf()(args) | -m_node2.cf()(args));
}
