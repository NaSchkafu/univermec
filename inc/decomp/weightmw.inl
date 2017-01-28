//-*-c++-*-
template <typename T>
split_lst_t WeightMW<T>::weight_(const T& box) const
{
  split_lst_t lst;
  unsigned dim(box.dim());
  for(unsigned i = 0; i < dim; i++)
    lst.push_back(s_info_t(i, wid(box.ref_X()[i]), core::arith::interval(-1,1)));
  //sort(lst.begin(), lst.end());
  return lst;
}

