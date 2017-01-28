// -*-c++-*-
//  ********************************************************************************
// CData
//  ********************************************************************************
template<typename T>
CData<T>::CData(const T& f)
  :m_f(f), m_size(0), m_dirs(0), m_member(0)
{ }

/// copy ctor
/**
 * Copying increases the gradient's directions store by 1 on every copy operation!
 */
template<typename T>
CData<T>::CData(const CData<T> &other, bool truecopy)
  :m_f(other.m_f), m_size(other.m_size+(truecopy ? 0 : 1)), m_dirs(new unsigned[m_size]), m_member(other.m_member)
{
  memcpy(m_dirs, other.m_dirs, sizeof(unsigned)*other.m_size);
//  memcpy(m_members, other.m_members, sizeof(unsigned)*other.m_size);
}

template<typename T>
CData<T>::~CData()
{
  delete[] m_dirs;
  //delete[] m_members;
}


//  ********************************************************************************

//  ********************************************************************************
//  Deriv
//  ********************************************************************************
template<typename T, int N>
Deriv<T, N>::Deriv(CData<T> *data)
  :m_data(data)
{ }

template<typename T, int N>
Deriv<T,N>::Deriv(const Deriv<T,N> &other)
{
  m_data = std::auto_ptr<CData<T> >(new CData<T>(*(other.m_data), true));
}

template<typename T, int N>
unsigned Deriv<T, N>::dim() const
{
  return m_data->m_f.dim();
}

template<typename T, int N>
std::string Deriv<T, N>::text() const
{
  std::stringstream sstr;
  sstr << "Gradient of " << m_data->m_f.text();
  return sstr.str();
}

template<typename T, int N>
const CData<T>& Deriv<T, N>::get_data() const
{
  return *m_data;
}

template<typename T, int N>
template<typename AT>
typename AT::value_type Deriv<T, N>::operator()(const AT& args) const
{
/*
  std::cout << "Werte Gradient aus ";
  utility::debug::print_array(m_data->m_dirs, m_data->m_size);
  std::cout << " für N=" << N << std::endl;
*/
  return details::DF<AT, T, N>::eval_functor(args, m_data->m_f, m_data->m_dirs, m_data->m_member);
}

//  ********************************************************************************
//  FDeriv
//  ********************************************************************************

template<typename T, typename T1, int N, int M>
FDeriv<T,T1,N,M>::FDeriv() {  }

template<typename T, typename T1, int N, int M>
FDeriv<T,T1,N,M>::~FDeriv() { }


template<typename T, typename T1, int N, int M>
void FDeriv<T,T1,N,M>::init()
{
  m_g_store = std::make_shared<g_mem_store_t>();
  m_g_store->m_grad.resize(f_cdim());
  create<N>();
}

template<typename T, typename T1, int N, int M>
template<typename T1_, typename T2_>
typename boost::enable_if<boost::is_same<T1_, T2_>, const CData<T2_>& >::type 
FDeriv<T,T1,N,M>::get_data() const
{
  return *(new CData<T2_>(get_f()));
}

template<typename T, typename T1, int N, int M>
template<typename T1_, typename T2_>
typename boost::disable_if<boost::is_same<T1_, T2_>, const CData<T2_>& >::type 
FDeriv<T,T1,N,M>::get_data() const
{
  return get_f().get_data();
}

template<typename T, typename T1, int N, int M>
template<typename T1_, typename T2_>
typename boost::disable_if<boost::is_same<T1_, T2_> >::type 
FDeriv<T,T1,N,M>::clean_data(const CData<T2_> &) const
{ }

template<typename T, typename T1, int N, int M>
template<typename T1_, typename T2_>
typename boost::enable_if<boost::is_same<T1_, T2_> >::type 
FDeriv<T,T1,N,M>::clean_data(const CData<T2_> &data) const
{
  delete const_cast<CData<T2_>* >(&data);
}

template<typename T, typename T1, int N, int M>
template<int N_>
typename boost::enable_if_c<N_-1 != 0>::type FDeriv<T,T1,N,M>::create()
{
  unsigned d = get_f().dim();
  unsigned members = f_cdim();
  const CData<T1>& cdata = get_data<T, T1>();
  for(unsigned j = 0; j < members; j++) {
    for(unsigned i = 0; i < d; i++) {
      CData<T1>* data = new CData<T1>(cdata, false);
      data->m_dirs[data->m_size-1] = i;
      data->m_member = boost::is_same<T, T1>::value ? j : data->m_member;
      FJoin<deriv_t, FDeriv<deriv_t, T1, N_ - 1, M> > *f =
	new FJoin<deriv_t, FDeriv<deriv_t, T1, N - 1, M> >(new deriv_t(data));
      m_g_store->m_grad[j].push_back(f);
    }
    // Grad Fun

    auto fg(new grad_fun_deriv_t(get_f(), m_g_store, j));
    auto f(new FJoin<grad_fun_deriv_t, FGradFun<grad_fun_deriv_t> >(fg));
    m_grad_fun.push_back(f);
  }
  clean_data<T, T1>(cdata);
}

template<typename T, typename T1, int N, int M>
template<int N_>
typename boost::disable_if_c<N_-1 != 0>::type FDeriv<T,T1,N,M>::create()
{
  unsigned d = get_f().dim();
  unsigned members = f_cdim();
  const CData<T1>& cdata = get_data<T, T1>();
  for(unsigned j = 0; j < members; j++) {
    for(unsigned i = 0; i < d; i++) {
      CData<T1>* data = new CData<T1>(cdata, false);
      data->m_dirs[data->m_size-1] = i;
      FVBase<deriv_t> *f = new FVBase<deriv_t>(new deriv_t(data));
      m_g_store->m_grad[j].push_back(f);
    }
    // Gradientenfunktionen generieren
    m_grad_fun.push_back(new FVBase<grad_fun_deriv_t>(new grad_fun_deriv_t(get_f(), m_g_store, j)));
  }
}

template<typename T, typename T1, int N, int M>
const IFunction& FDeriv<T,T1,N,M>::grad_(unsigned dir, unsigned member) const
{
  assert(dir > 0 && dir <= get_f().dim() &&  "Gradient direction does not exist!");
  return m_g_store->m_grad[member][dir-1];
}

template<typename T, typename T1, int N, int M>
core::arith::ivector FDeriv<T,T1,N,M>::grad_(const core::arith::ivector &args, unsigned member) const
{
  return calc_grad(args, member);
}

template<typename T, typename T1, int N, int M>
core::arith::rvector FDeriv<T,T1,N,M>::grad_(const core::arith::rvector &args, unsigned member) const
{
  return calc_grad(args, member);
}

template<typename T, typename T1, int N, int M>
core::arith::avector FDeriv<T,T1,N,M>::grad_(const core::arith::avector &args, unsigned member) const
{
  return calc_grad(args, member);
}

template<typename T, typename T1, int N, int M>
core::arith::tvector FDeriv<T,T1,N,M>::grad_(const core::arith::tvector &args, unsigned member) const
{
  return calc_grad(args, member);
}

// template<typename T, typename T1, int N, int M>
// core::arith::rmatrix FDeriv<T,T1,N,M>::taylor_(const core::arith::rvector &args, unsigned order) const
// {
//   return calc_taylor(args, order);
// }

// template<typename T, typename T1, int N, int M>
// core::arith::imatrix FDeriv<T,T1,N,M>::taylor_(const core::arith::ivector &args, unsigned order) const
// {
//   return calc_taylor(args, order);
// }

// template<typename T, typename T1, int N, int M>
// core::arith::amatrix FDeriv<T,T1,N,M>::taylor_(const core::arith::avector &args, unsigned order) const
// {
//   return calc_taylor(args, order);
// }

// template<typename T, typename T1, int N, int M>
// core::arith::tmatrix FDeriv<T,T1,N,M>::taylor_(const core::arith::tvector &args, unsigned order) const
// {
//   return calc_taylor(args, order);
// }

template<typename T, typename T1, int N, int M>
core::arith::rmatrix FDeriv<T,T1,N,M>::jacobian_(const core::arith::rvector &args) const
{
  return calc_jacobian(args);
}

template<typename T, typename T1, int N, int M>
core::arith::imatrix FDeriv<T,T1,N,M>::jacobian_(const core::arith::ivector &args) const
{
  return calc_jacobian(args);
}

template<typename T, typename T1, int N, int M>
core::arith::amatrix FDeriv<T,T1,N,M>::jacobian_(const core::arith::avector &args) const
{
  return calc_jacobian(args);
}

template<typename T, typename T1, int N, int M>
core::arith::tmatrix FDeriv<T,T1,N,M>::jacobian_(const core::arith::tvector &args) const
{
  return calc_jacobian(args);
}


template<typename T, typename T1, int N, int M>
template<typename T_>
typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, T_>::type FDeriv<T,T1,N,M>::calc_grad(const T_ &args, unsigned member) const
{
  assert(!member && "Invalid member selected");
  T_ ret(get_f().dim()+1);
  mtl::dense_vector<fadbad::B<typename T_::value_type> > dargs(args);
  fadbad::B<typename T_::value_type> res(this->get_f()(dargs));
  res.diff(0, 1);
  ret[0] = res.x();
  for(unsigned i = 0; i < size(dargs); i++)
    ret[i+1] = dargs[i].d(0);
  return ret;
}

template<typename T, typename T1, int N, int M>
template<typename T_>
typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, T_>::type FDeriv<T,T1,N,M>::calc_grad(const T_ &args, unsigned member) const
{
  assert(member < f_cdim() && "Invalid member selected");
  T_ ret(get_f().dim()+1);
  mtl::dense_vector<fadbad::B<typename T_::value_type> > dargs(args);
  fadbad::B<typename T_::value_type> res(this->get_f()(dargs)[member]);
  res.diff(0, 1);
  ret[0] = res.x();
  for(unsigned i = 0; i < size(dargs); i++)
    ret[i+1] = dargs[i].d(0);
  return ret;
}

template<typename T, typename T1, int N, int M>
template<typename T_>
typename boost::enable_if_c<is_scalar_functor_t<T>::is_scalar, typename core::arith::ArithTraits<typename T_::value_type>::mat_t>::type 
FDeriv<T, T1, N, M>::calc_jacobian(const T_ &args) const
{
  T_ rv(calc_grad(args, 0));
  typename core::arith::ArithTraits<typename T_::value_type>::mat_t rm(1, size(rv));
  for(unsigned i = 0; i < size(rv); i++)
    rm[0][i] = rv[i];
  return rm;
}

template<typename T, typename T1, int N, int M>
template<typename T_>
typename boost::disable_if_c<is_scalar_functor_t<T>::is_scalar, typename core::arith::ArithTraits<typename T_::value_type>::mat_t>::type 
FDeriv<T, T1, N, M>::calc_jacobian(const T_ &args) const
{
  unsigned const m = f_cdim();
  unsigned const n = get_f().dim();
  typename core::arith::ArithTraits<typename T_::value_type>::mat_t rm(m, n+1);
  mtl::dense_vector<fadbad::B<typename T_::value_type> > dargs(args);
  mtl::dense_vector<fadbad::B<typename T_::value_type> > res(get_f()(dargs));
  for(unsigned i = 0; i < m; i++)
    res[i].diff(i, m);
  for(unsigned i = 0; i < m; i++) {
    rm[i][0] = res[i].x();
    for(unsigned j = 0; j < n; j++)
      rm[i][j+1] = dargs[j].d(i);
  }
  return rm;
}

  // void FadbadODE::set(const interval&t0,const iVector&y0,
  //                     const interval&h,int k)
  // {
  //   t[0]= t0;
  //   t[1]= h;
  //   stepsize= h;order= k;
  //   for(int eqn= 0;eqn<size;eqn++)
  //     y_coeff[eqn][0]= y0[eqn];
  // }
// template<typename T, typename T1, int N, int M>
// template <typename T_>
// typename core::arith::ArithTraits<typename T_::value_type>::mat_t
// FDeriv<T, T1, N, M>::calc_taylor(const T_ &args, unsigned order) const 
// {
//   unsigned dim = get_f().dim();
//   typename core::arith::ArithTraits<typename T_::value_type>::mat_t ret(dim, order+1);

//   // mtl::dense_vector<fadbad::T<typename T_::value_type> > targs(args);
//   // for(unsigned i = 0; i < dim; i++)
//   //   targs[i][0] = args[i];
//   // fadbad::T<typename T_::value_type> res(this->get_f()(targs));
//   // for(unsigned j = 0; j <= order; j++) {
//   //   res.eval(j);
//   //   ret[0][j] = res[j];
//   //   //targs[][i+1] = ode.xp[i]/double(i+1);// Use dx/dt=ode(x).

//   // for(unsigned i = 0; i < dim; i++) {
//   //   // Effizienter machen...
//   //mtl::dense_vector<fadbad::T<typename T_::value_type> > targs(args);
//   //   targs[i][1] = 1;
//   //   // std::cout << targs[0][0] << std::endl;
//   //   // std::cout << targs[1][0] << std::endl;
//   //   // std::cout << "**************" << std::endl;
//   //   fadbad::T<typename T_::value_type> res(this->get_f()(targs));
//   //   res.eval(order);
//   //   for(unsigned j = 0; j <= order; j++)
//   //     ret[i][j] = res[j];

//   // }
//   return ret;
// }

// ****************************************************************

template<typename T, typename T1, int N, int M>
const IVFunction& FDeriv<T, T1, N, M>::grad_fun_(unsigned member) const
{
  return m_grad_fun[member];
}
