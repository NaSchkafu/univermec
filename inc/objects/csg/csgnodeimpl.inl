// -*-c++-*-
// ********************************************************************************
// CSGTransformImpl
// ********************************************************************************
template<typename T>
CSGTransformImpl<T>::CSGTransformImpl(unsigned min_ch, unsigned max_ch,
				      CSG_NODE_TYPE type, ICSGNode *parent)
  :CSGNodeImpl(min_ch, max_ch, type, parent)
{}


template<typename T>
core::arith::imatrix CSGTransformImpl<T>::rot_() const
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return core::arith::ArithTraits<core::arith::imatrix>::convert(m_r);
  assert(!"Transformation information is not available!");
  // TODO: Exception
  return core::arith::imatrix(0);
}

template<typename T>
core::arith::imatrix CSGTransformImpl<T>::i_rot_() const
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return core::arith::ArithTraits<core::arith::imatrix>::convert(m_cf->get_m());
  assert(!"Transformation information is not available!");
  // TODO: Exception
  return core::arith::imatrix(0);
}

template<typename T>
core::arith::ivector CSGTransformImpl<T>::trans_() const
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return core::arith::ArithTraits<core::arith::ivector>::convert(m_t);
  assert(!"Transformation information is not available!");
  // TODO: Exception
  return core::arith::ivector(0);
}

template<typename T>
core::arith::ivector CSGTransformImpl<T>::i_trans_() const
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return core::arith::ArithTraits<core::arith::ivector>::convert(m_cf->get_c());
  // TODO: Exception
  return core::arith::ivector(0);
}

template<typename T>
void CSGTransformImpl<T>::set_t_s(const typename core::arith::ArithTraits<T>::mat_t &r, 
				  const typename core::arith::ArithTraits<T>::mat_t &ir,
				  const typename core::arith::ArithTraits<T>::vec_t &t,
				  const typename core::arith::ArithTraits<T>::vec_t &it)
{
  m_r = r;
  m_t = t;
  m_cf.reset(new fun_t(this->operator[](0).cf(), ir, it));
  m_nf.reset(details::normal_fun(*m_cf.get()));
}

template<typename T>
const functions::IFunction& CSGTransformImpl<T>::cf_() const
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return *m_cf;
}

template<typename T>
unsigned CSGTransformImpl<T>::dim_() const 
{
  assert(m_cf.get() && "set_t_s was not called on this node! Node is invalid.");
  return m_cf->dim();
}

template <typename T>
const functions::IVFunction* CSGTransformImpl<T>::normals_() const
{
  return m_nf.get();
}

/*
template<typename POLY>
CSGPolyLeafImpl<POLY>::CSGPolyLeafImpl(unsigned min_ch, unsigned max_ch,
                                 CSG_NODE_TYPE type, ICSGNode *parent)
  :CSGNodeImpl(min_ch, max_ch, type, parent)
{ }

// die dynamic casts sind überflüssig und können bei Bedarf per enable_if zur Compilezeit
// entsprechend statisch angepasst werden, falls nötig
template<typename POLY>
const INonConvexPolyhedron<core::arith::mreal>* CSGPolyLeafImpl<POLY>::real_poly_() const
{
  return dynamic_cast<INonConvexPolyhedron<core::arith::mreal>*>(m_poly.get());
}

template<typename POLY>
const INonConvexPolyhedron<core::arith::interval>* CSGPolyLeafImpl<POLY>::iv_poly_() const
{
  return dynamic_cast<INonConvexPolyhedron<core::arith::interval>*>(m_poly.get());
}

template<typename POLY>
void CSGPolyLeafImpl<POLY>::set_poly(INonConvexPolyhedron<POLY> *poly)
{
  m_poly = std::auto_ptr<INonConvexPolyhedron<POLY> >(poly);
}

template<typename POLY>
const functions::IFunction& CSGPolyLeafImpl<POLY>::in_cf_() const
{
  return m_poly->cf();
}

// Alternative Implementierung ohne set_t_s
// Könnte problematisch werden, da theoretisch der Transform auch mittels FCADBase anstatt FBase
// implementiert werden könnte für automatische Differenzierung.

  template<typename TRANS>
  void CSGTransformImpl<TRANS>::set_in_cf(functions::IFunction *in_cf)
  {
  CSGIndependentImpl::set_in_cf(in_cf);
  functions::FBase<TRANS> *ft = dynamic_cast<functions::FBase<TRANS>*>(in_cf);
  if(ft)
  m_t_s = ft->structure_();
  else
  m_t_s = 0;
  }

*/
