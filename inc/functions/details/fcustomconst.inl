// -*-c++-*-
template<typename T>
bool FIConstBase<T>::prune_(core::Box &box, const core::arith::interval &rhs) const
{
  return m_c_base.get_f().prune(box, rhs);
}

template<typename T>
bool FIConstBase<T>::prune_(core::GappedBox &box, const core::arith::interval &rhs) const
{
  return m_c_base.get_f().prune(box, rhs);
}

template<typename T>
FIConstBase<T>::FIConstBase(const FCustomConst<T> &base)
  :m_c_base(base)
{ }

template<typename T>
FIConstBase<T>::~FIConstBase()
{}

template<typename T>
void FCustomConst<T>::init()
{
  add(new FIConstBase<T>(*this));
}

template<typename T>
FCustomConst<T>::~FCustomConst() {}

template <typename T>
std::string FIConstBase<T>::text_() const
{
  std::stringstream sstr;
  sstr << "Custom consistency for " << m_c_base.get_f().text();
  return sstr.str();
}

// \todo check_box_ info durchreichen
template <typename T>
float FIConstBase<T>::check_box_(const core::Box&) const
{
  return 1.0;
}
