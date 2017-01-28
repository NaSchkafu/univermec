// -*-c++-*-
// ****************************************************************
// FJoin
// ****************************************************************
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
typename is_scalar_functor_t<T>::functor_t* FJoin<T, T1, T2, T3, T4, T5, T6>::clone_() const
{
  return new my_t(FVBase<T>::get_f());
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
template<typename T_>
typename boost::enable_if<boost::is_base_of<T_, FJoin<T, T1, T2, T3, T4, T5, T6> >, const T_*>::type  
FJoin<T, T1, T2, T3, T4, T5, T6>::detect_feature() const
{
  return this;
}


template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
template<typename T_>
typename boost::disable_if<boost::is_base_of<T_, FJoin<T, T1, T2, T3, T4, T5, T6> >, const T_*>::type 
FJoin<T, T1, T2, T3, T4, T5, T6>::detect_feature() const
{
  return nullptr;
}


// template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
// template<typename T_>
// typename boost::enable_if<boost::is_base_of<T_, FJoin<T, T1, T2, T3, T4, T5, T6> >, T_*>::type  
// FJoin<T, T1, T2, T3, T4, T5, T6>::detect_feature()
// {
//   return this;
// }


// template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
// template<typename T_>
// typename boost::disable_if<boost::is_base_of<T_, FJoin<T, T1, T2, T3, T4, T5, T6> >, T_*>::type 
// FJoin<T, T1, T2, T3, T4, T5, T6>::detect_feature() 
// {
//   return nullptr;
// }


template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
const IDerivative* FJoin<T, T1, T2, T3, T4, T5, T6>::derivative_() const
{
  return detect_feature<IDerivative>();
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
const ICustomConsistency* FJoin<T, T1, T2, T3, T4, T5, T6>::custom_consistency_() const
{
  return detect_feature<ICustomConsistency>();
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
const ICustomEncl* FJoin<T, T1, T2, T3, T4, T5, T6>::custom_encl_() const
{
  return detect_feature<ICustomEncl>();
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
const ITaylorCoeff* FJoin<T, T1, T2, T3, T4, T5, T6>::taylor_coeff_() const
{
  return detect_feature<ITaylorCoeff>();
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
IGPUEval* FJoin<T, T1, T2, T3, T4, T5, T6>::gpu_eval_() const
{
#ifdef HAVE_CUDA
  // Nicht schoen...
  return const_cast<IGPUEval*>(detect_feature<IGPUEval>());
#endif
  return nullptr;
}

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
const IFFeatures& FJoin<T, T1, T2, T3, T4, T5, T6>::features_() const 
{
  return *this;
}
