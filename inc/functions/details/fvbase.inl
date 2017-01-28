// -*-c++-*-



// **********************************************************************************************
// FVBase
// **********************************************************************************************
template<typename T>
T& FVBase<T>::get_f_()
{
  return *m_struc;
}

template<typename T>
const T& FVBase<T>::get_f_() const
{
  return *m_struc;
}

template<typename T>
FVBase<T>::FVBase()
  :m_struc(new T)
{
  init();
}

template<typename T>
FVBase<T>::FVBase(const T& struc)
  :m_struc(new T(struc))
{
  init();
}

template<typename T>
FVBase<T>::FVBase(T* struc)
  :m_struc(struc)
{
  init();
}

template <typename T>
void FVBase<T>::init()
{
  // if(!S_IS_SCALAR)
  //   for(unsigned i = 0; i < cdim_(); i++)
  //     m_members.push_back(cd_wrap(*this, i));
}

template <typename T>
typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, const IFunction*>::type return_self(const FVBase<T> *f)
{
  return f;
}

template <typename T>
typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, const IFunction*>::type return_self(const FVBase<T> *f)
{
  return nullptr;
}


// TODO: FIX
// Die so erzeugten Memberfunktionen haben keine Eigenschaften mehr
// von der ursprünglichen Version (Differenzierbarkeit etc)
// Durch Wrapper ersetzen
// template <typename T>
// const IFunction& FVBase<T>::fun_(unsigned i) const
// {
//   if(S_IS_SCALAR) {
//     if(!i)
//       return *return_self(this);
//   } 
//   else if(i < m_members.size())
//     return m_members[i];
//   assert(!"Invalid member requested!");
//   return *((IFunction*)0);
// }

template <typename T, typename T1>
typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type eval_functor_2(const T& func, const T1& args)
{
  return func(args);
}

template <typename T, typename T1>
typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, typename T1::value_type>::type eval_functor_2(const T& func, const T1& args)
{
  assert(!"Calling eval_functor_2 is only allowed for scalar functors!");
  throw "eval_functor_2 call illegal!";
}


template<typename T>
core::arith::ivector FVBase<T>::calc_v_(const core::arith::ivector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_1(*m_struc, args);
}

template<typename T>
core::arith::avector FVBase<T>::calc_v_(const core::arith::avector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_1(*m_struc, args);
}

template<typename T>
core::arith::rvector FVBase<T>::calc_v_(const core::arith::rvector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_1(*m_struc, args);
}

template<typename T>
core::arith::tvector FVBase<T>::calc_v_(const core::arith::tvector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");

  // // Unabhaengige TM Variablen zuweisen...
  // unsigned dim = size(args);
  // assert(dim <= 10 && "RiOt and COSY only support 10 variables");
  // core::arith::tvector tx(dim);
  // for(unsigned i = 0; i < dim; i++)
  //   tx[i] = core::arith::create_tm_var(enclosure_cast<core::arith::interval>(args[i]));

  return eval_functor_1(*m_struc, args);
}

template <typename T>
core::arith::interval FVBase<T>::calc_(const core::arith::ivector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_2(*m_struc, args);
}

template <typename T>
core::arith::aaf FVBase<T>::calc_(const core::arith::avector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_2(*m_struc, args);
}

template <typename T>
core::arith::mreal FVBase<T>::calc_(const core::arith::rvector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");
  return eval_functor_2(*m_struc, args);
}

template <typename T>
core::arith::taylormodel FVBase<T>::calc_(const core::arith::tvector &args) const
{
  assert(size(args) == dim_() && "Invalid number of input arguments!");

  // // Unabhaengige TM Variablen zuweisen...
  // unsigned dim = size(args);
  // assert(dim <= 10 && "RiOt and COSY only support 10 variables");
  // core::arith::tvector tx(dim);
  // for(unsigned i = 0; i < dim; i++)
  //   tx[i] = core::arith::create_tm_var(enclosure_cast<core::arith::interval>(args[i]));

  return eval_functor_2(*m_struc, args);
}

template <typename T>
typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, unsigned>::type eval_functor_cdim(const T& func)
{
  return 1;
}

template <typename T>
typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, unsigned>::type eval_functor_cdim(const T& func)
{
  return func.cdim();
}

template<typename T>
unsigned FVBase<T>::cdim_() const
{
  return eval_functor_cdim(*m_struc);
}

template <typename T>
unsigned FVBase<T>::f_cdim_() const
{
  return cdim_();
}

template<typename T>
unsigned FVBase<T>::dim_() const
{
  return m_struc->dim();
}

template<typename T>
std::string FVBase<T>::text_() const
{
  return m_struc->text();
}

template<typename T>
typename is_scalar_functor_t<T>::functor_t* FVBase<T>::clone_() const
{
  return new FVBase<T>(*m_struc);
}

template<typename T>
const IFFeatures& FVBase<T>::features_() const
{
  return *this;
}

template<typename T>
const IDerivative* FVBase<T>::derivative_() const
{
  return nullptr;
}

template<typename T>
const ICustomConsistency* FVBase<T>::custom_consistency_() const
{
  return nullptr;
}

template<typename T>
const ICustomEncl* FVBase<T>::custom_encl_() const
{
  return nullptr;
}

template <typename T>
const ITaylorCoeff* FVBase<T>::taylor_coeff_() const
{
  return nullptr;
}

template <typename T>
IGPUEval* FVBase<T>::gpu_eval_() const
{
  return nullptr;
}


//  ********************************************************************************

