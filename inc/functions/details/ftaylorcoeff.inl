// -*-c++-*-
// template<typename T>
// struct is_param_functor_t
// {
//   template<typename U, fadbad::F<core::arith::ivector> (U::*)(const fadbad::F<core::arith::ivector> &, const core::arith::ivector &) const> struct SFINAE {};
//   template<typename U> static char Test(SFINAE<U, &U::operator()>*);
//   template<typename U> static int Test(...);
//   static const bool is_param = sizeof(Test<T>(0)) == sizeof(char);
// };

// template <typename T, typename T1, typename T2>
// typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, T1>::type eval_param_functor_1(const T& func, const T1& args)
// {
//   return T1(1, func(args), params);
// }

// template <typename T, typename T1, typename T2>
// typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, T1>::type eval_param_functor_1(const T& func, const T1& args)
// {
//   return func(args, params);
// }

template<typename T>
struct is_param_v_functor_t
{
  template<typename U, mtl::dense_vector<fadbad::F<core::arith::interval> > (U::*)(const mtl::dense_vector<fadbad::F<core::arith::interval> >&, 
										   const core::arith::ivector &) const> struct SFINAE {};
  template<typename U> static char Test(SFINAE<U, &U::operator()>*);
  template<typename U> static int Test(...);
  static const bool is_param = sizeof(Test<T>(0)) == sizeof(char);
};

template<typename T>
struct is_param_s_functor_t
{
  template<typename U, fadbad::F<core::arith::interval> (U::*)(const mtl::dense_vector<fadbad::F<core::arith::interval> > &, 
							       const core::arith::ivector &) const> struct SFINAE {};
  template<typename U> static char Test(SFINAE<U, &U::operator()>*);
  template<typename U> static int Test(...);
  static const bool is_param = sizeof(Test<T>(0)) == sizeof(char);
};

template <typename T>
struct is_param_functor_t
{
  static const bool is_param = is_param_v_functor_t<T>::is_param || is_param_s_functor_t<T>::is_param;
};

template <typename T, typename T1, typename T2>
typename boost::enable_if<boost::mpl::and_<boost::mpl::bool_<is_scalar_functor_t<T>::is_scalar>,
                                           boost::mpl::bool_<is_param_functor_t<T>::is_param> >, T1>::type
eval_param_functor_1(const T& func, const T1& args, const T2 & params)
{
  return T1(1, func(args, params));
}

template <typename T, typename T1, typename T2>
typename boost::disable_if<boost::mpl::or_<boost::mpl::bool_<is_scalar_functor_t<T>::is_scalar>,
                                           boost::mpl::not_<boost::mpl::bool_<is_param_functor_t<T>::is_param> > >, T1>::type
eval_param_functor_1(const T& func, const T1& args, const T2& params)
{
  return func(args, params);
}

template <typename T, typename T1, typename T2>
typename boost::disable_if_c<is_param_functor_t<T>::is_param, T1>::type 
eval_param_functor_1(const T& func, const T1&, const T2& )
{
  assert(!"eval_param_functor_1 called on non parametric functor. This is most likely a bug!");
}

template <typename T>
FTaylorCoeff<T>::FTaylorCoeff() { }

template <typename T>
FTaylorCoeff<T>::~FTaylorCoeff() { }


template <typename T>
void FTaylorCoeff<T>::init() { }

template <typename T>
std::vector<core::arith::rvector> FTaylorCoeff<T>::taylor_coeff_ode_(const core::arith::rvector &args, unsigned order) const
{
  return calc_taylor_coeff_ode(args, order);
}

template <typename T>
std::vector<core::arith::ivector> FTaylorCoeff<T>::taylor_coeff_ode_(const core::arith::ivector &args, unsigned order) const
{
  return calc_taylor_coeff_ode(args, order);
}

template <typename T>
std::vector<core::arith::avector> FTaylorCoeff<T>::taylor_coeff_ode_(const core::arith::avector &args, unsigned order) const
{
  return calc_taylor_coeff_ode(args, order);
}

template <typename T>
std::vector<core::arith::tvector> FTaylorCoeff<T>::taylor_coeff_ode_(const core::arith::tvector &args, unsigned order) const
{
  return calc_taylor_coeff_ode(args, order);
}

template <typename T>
std::vector<std::vector<core::arith::rvector> > FTaylorCoeff<T>::taylor_coeff_ode_df_(const core::arith::rvector &args, unsigned order) const
{
  return calc_taylor_coeff_ode_df(args, order);
}

template <typename T>
std::vector<std::vector<core::arith::ivector> > FTaylorCoeff<T>::taylor_coeff_ode_df_(const core::arith::ivector &args, unsigned order) const
{
  return calc_taylor_coeff_ode_df(args, order);
}

template <typename T>
std::vector<std::vector<core::arith::avector> > FTaylorCoeff<T>::taylor_coeff_ode_df_(const core::arith::avector &args, unsigned order) const
{
  return calc_taylor_coeff_ode_df(args, order);
}

template <typename T>
std::vector<std::vector<core::arith::tvector> > FTaylorCoeff<T>::taylor_coeff_ode_df_(const core::arith::tvector &args, unsigned order) const
{
  return calc_taylor_coeff_ode_df(args, order);
}

template <typename T>
template <typename T_>
std::vector<T_> FTaylorCoeff<T>::calc_taylor_coeff_ode(const T_& args, unsigned order) const
{
  unsigned cdim = f_cdim();
  unsigned dim = get_f().dim();
  std::vector<T_> ret(cdim, T_(order+1));
  mtl::dense_vector<fadbad::T<typename T_::value_type> > targs(cdim);
  mtl::dense_vector<fadbad::T<typename T_::value_type> > res(cdim);

  if(cdim < dim && is_param_functor_t<T>::is_param) {
    // Annahme der Situation R^{n+p+u} \to \R^n
    // Parametrischer Funktor mit operator()(args, params) liegt vor
    for(unsigned i = 0; i < cdim; i++)
      targs[i][0]= args[i];
    T_ params(args[mtl::irange(cdim, dim)]);
    res = eval_param_functor_1(get_f(), targs, params);
  }
  else if(cdim == dim) {
    targs = args;
    res = eval_functor_1(get_f(), targs);
  }
  else
    assert(!"Mapping has to n \to n or n+p+u \to n with a parametric functor");

  // Algorithmus aus VNODE-LP um TaylorKoeffizienten der rechten Seite
  // zu bestimmen
  for(unsigned eqn = 0; eqn < cdim;eqn++) {
    res[eqn].reset();
  }

  for(unsigned coeff= 1;coeff<=order+1;coeff++) {
    for(unsigned eqn= 0;eqn < cdim;eqn++) {
      res[eqn].eval(coeff-1);
      targs[eqn][coeff] = res[eqn][coeff-1]/double(coeff);
      ret[eqn][coeff-1] = targs[eqn][coeff-1];
    }
  }

  return ret;
}


template <typename T>
template <typename T_>
std::vector<std::vector<T_> > FTaylorCoeff<T>::calc_taylor_coeff_ode_df(const T_& args, unsigned order) const
{
  unsigned cdim = f_cdim();
  unsigned dim = get_f().dim();
  std::vector<std::vector<T_> > ret(cdim);
  mtl::dense_vector<fadbad::T<fadbad::F<typename T_::value_type> > > targs(cdim);
  mtl::dense_vector<fadbad::T<fadbad::F<typename T_::value_type> > > res(cdim);

  if(cdim < dim && is_param_functor_t<T>::is_param) {
    // Annahme der Situation R^{n+p+u} \to \R^n
    // Parametrischer Funktor mit operator()(args, params) liegt vor
    for(unsigned i = 0; i < cdim; i++)
      targs[i][0]= args[i];
    T_ params(args[mtl::irange(cdim, dim)]);
    res = eval_param_functor_1(get_f(), targs, params);
  }
  else if(cdim == dim) {
    targs = args;
    res = eval_functor_1(get_f(), targs);
  }
  else
    assert(!"Mapping has to n \to n or n+p+u \to n with a parametric functor");

  for(unsigned eqn = 0; eqn < cdim;eqn++) {
    ret[eqn] = std::vector<T_>(cdim, T_(order+1));
    res[eqn].reset();
    targs[eqn][0].diff(eqn, cdim);
  }

  for(unsigned coeff= 1;coeff<=order+1;coeff++) {
    for(unsigned eqn= 0;eqn < cdim;eqn++) {
      res[eqn].eval(coeff-1);
      targs[eqn][coeff] = res[eqn][coeff-1]/double(coeff);
      for(unsigned d = 0; d < cdim; d++) {
        //std::cout << "eqn:" << eqn << " coeff-1:" << coeff-1 << " d:" << d << ": " << targs[eqn][coeff-1].d(d) << std::endl;
        ret[eqn][d][coeff-1] = targs[eqn][coeff-1].d(d);
      }
    }
  }

  return ret;
}
