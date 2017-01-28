// -*-c++-*-

// ********************************************************************************
template <typename T>
f_grad_fun_deriv_t<T>::f_grad_fun_deriv_t(const T& func, const std::shared_ptr<g_mem_store_t>& store,
					  unsigned member)
  :m_func(func), m_store(store), m_member(member)
{ }

// template <typename T>
// f_grad_fun_deriv_t<T>::f_grad_fun_deriv_t(const f_grad_fun_deriv_t<T> &other)
//   :m_master(dynamic_cast<FMaster<T>*>(dynamic_cast<const functions::IVFunction&>(*other.m_master).clone())), 
//    m_member(other.m_member)
// { }


template <typename T>
unsigned f_grad_fun_deriv_t<T>::dim() const
{
  return m_func.dim();
}

template <typename T>
std::string f_grad_fun_deriv_t<T>::text() const
{
  std::stringstream sstr;
  sstr << "Gradient of " << m_func.text();
  return sstr.str();
}

template <typename T>
unsigned f_grad_fun_deriv_t<T>::cdim() const
{
  return m_func.dim()+1;
}

template <typename T>
template <typename T_>
T_ f_grad_fun_deriv_t<T>::operator()(const T_& args) const
{
  T_ ret(cdim());
  mtl::dense_vector<fadbad::B<typename T_::value_type> > dargs(args);
  fadbad::B<typename T_::value_type> res(eval_functor_member(m_func, dargs, m_member));
  res.diff(0, 1);
  ret[0] = res.x();
  for(unsigned i = 0; i < size(dargs); i++)
    ret[i+1] = dargs[i].d(0);
  return ret;
}

template <typename T>
unsigned f_grad_fun_deriv_t<T>::member() const
{
  return m_member;
}

template <typename T>
const g_mem_store_t& f_grad_fun_deriv_t<T>::store() const
{
  return *m_store;
}


// ********************************************************************************

template <typename T>
void FGradFun<T>::init() 
{ }

template <typename T>
const IFunction& FGradFun<T>::grad_(unsigned dir, unsigned member) const
{
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad(dir);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad(dir);
}

template <typename T>
core::arith::ivector FGradFun<T>::grad_(const core::arith::ivector &args, unsigned member) const
{
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad(args);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad(args);
}

template <typename T>
core::arith::avector FGradFun<T>::grad_(const core::arith::avector &args, unsigned member) const
{
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad(args);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad(args);
}

template <typename T>
core::arith::tvector FGradFun<T>::grad_(const core::arith::tvector &args, unsigned member) const
{
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad(args);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad(args);
}

template <typename T>
core::arith::rvector FGradFun<T>::grad_(const core::arith::rvector &args, unsigned member) const
{
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad(args);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad(args);
}

template <typename T>
core::arith::rmatrix FGradFun<T>::jacobian_(const core::arith::rvector &args) const
{
  return calc_jacobian(args);
}

template <typename T>
core::arith::imatrix FGradFun<T>::jacobian_(const core::arith::ivector &args) const
{
  return calc_jacobian(args);
}

template <typename T>
core::arith::amatrix FGradFun<T>::jacobian_(const core::arith::avector &args) const
{
  return calc_jacobian(args);
}

template <typename T>
core::arith::tmatrix FGradFun<T>::jacobian_(const core::arith::tvector &args) const
{
  return calc_jacobian(args);
}

template <typename T>
const IVFunction& FGradFun<T>::grad_fun_(unsigned member) const
{
  assert(member < f_cdim() && "Gradient function of non-existent member requested!");
  if(!member)
    return dynamic_cast<const IVFunction&>(*this);
  T f(get_f());
  return get_f().store().m_grad[get_f().member()][member-1].features().derivative()->grad_fun(0);
  //return m_parent->grad(member, get_f().member()).features().derivative()->grad_fun(0);
}

template<typename T>
template<typename T_>
typename core::arith::ArithTraits<typename T_::value_type>::mat_t FGradFun<T>::calc_jacobian(const T_ &args) const
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
