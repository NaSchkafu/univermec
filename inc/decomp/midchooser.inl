//-*-c++-*-

template <typename T>
void MidChooser<T>::pchoose_(const T& box, 
			     split_lst_t &slst,
			     unsigned sects) const
{  
  for(unsigned i = 0; i < sects;i++)
    slst[i].point = core::arith::interval(mid(box.ref_X()[slst[i].dir]));
}
