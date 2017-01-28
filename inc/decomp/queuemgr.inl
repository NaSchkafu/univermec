// -*-c++-*-
template <typename T>
void QueueMgr<T>::add_(T* b)
{
  m_boxes.push_front(b);
  // std::cout << "Füge hinzu " << b << " ";
  // std::cout << boxes.size() << "this @" << this << std::endl;
}

template <typename T>
void QueueMgr<T>::add_(const typename IBoxListMgr<T>::box_lst_t &lst)
{
  for(auto it(lst.begin()); it != lst.end(); ++it) {
    m_boxes.push_front(*it);
    // std::cout << "Füge hinzu " << *it << " ";
    // std::cout << boxes.size() << "this @" << this << std::endl;
  }
}

template <typename T>
T* QueueMgr<T>::select_()
{
  T* ele = 0;
  assert(m_boxes.size() && "select_ called on empty element!");
  ele = m_boxes.back();
  m_boxes.pop_back();
  return ele;
}

template <typename T>
unsigned QueueMgr<T>::count_() const
{
  return m_boxes.size();
}

template <typename T>
QueueMgr<T>::QueueMgr()
  :m_turn(0)
{}



template <typename T>
typename IBoxListMgr<T>::box_lst_t QueueMgr<T>::list_() const
{
  typename IBoxListMgr<T>::box_lst_t ret(m_boxes.size());
  std::copy(m_boxes.begin(), m_boxes.end(), ret.begin());
  return ret;
}

template <typename T>
void QueueMgr<T>::clear_()
{
  m_boxes.clear();
}

template <typename T>
void QueueMgr<T>::del_clear_()
{
  for_each(m_boxes.begin(), m_boxes.end(), [](T* b) { delete b; });
  clear_();
}


template <typename T>
bool QueueMgr<T>::empty_() const
{
  return m_boxes.empty();
}
