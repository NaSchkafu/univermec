//-*-c++-*-
template <typename T>
WeightRatz92<T>::WeightRatz92(const functions::IFunction &obj)
  :m_obj(obj), m_dobj(obj.features().derivative())
{
  if(!m_dobj) {
    assert(!"WeightRatz92 requested but no first order derivatives are available!");
    BOOST_THROW_EXCEPTION(core::univermec_exception() << core::error_details("WeightRatz92 requires an objective with first-order derivative support"));
  }
}

template <typename T>
split_lst_t WeightRatz92<T>::weight_(const T& box) const
{
  split_lst_t lst;
  unsigned dim(box.dim());
  for(unsigned i = 0; i < dim; i++)
    lst.push_back(s_info_t(i, wid(m_dobj->grad(i+1)(box.ref_X())*(box.ref_X()[i] - mid(box.ref_X()[i]))), 
			   core::arith::interval(-1,1)));
  return lst;
}

