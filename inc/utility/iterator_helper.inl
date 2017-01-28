// -*-c++-*-
// Impl
template<class T>
tree_iterator<T>::tree_iterator()
  :m_base(0), m_pos(0)
{ }

template<class T>
tree_iterator<T>::tree_iterator(T* base)
  :m_base(base), m_pos(1)
{ }

template<class T>
void tree_iterator<T>::increment()
{
    if(m_pos && m_pos <= m_base->children())
        m_pos++;
}

template<class T>
T& tree_iterator<T>::dereference() const
{
  assert(m_base && "Iterator is not initialized!");
  assert(m_pos && "Tried to deref. an iterator that reached end()");
  return (*m_base)[m_pos-1];
}

template<class T>
bool tree_iterator<T>::equal(tree_iterator const& other) const
{
  return (other.m_pos == m_pos) && (other.m_base == m_base);
}
