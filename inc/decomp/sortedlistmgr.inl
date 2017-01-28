// -*-c++-*-
template <typename T, typename CMP, typename C>
void SortedBoxListMgr<T, CMP, C>::add_(T* b)
{
  C& boxes = select_queue();
  boxes.push_back(b);
  push_heap(boxes.begin(), boxes.end(), m_cmp);
  // std::cout << "Füge hinzu " << b << " ";
  // std::cout << boxes.size() << "this @" << this << std::endl;
}

template <typename T, typename CMP, typename C>
void SortedBoxListMgr<T, CMP, C>::add_(const typename IBoxListMgr<T>::box_lst_t &lst)
{
  C& boxes = select_queue();
  for(auto it(lst.begin()); it != lst.end(); ++it) {
    boxes.push_back(*it);
    push_heap(boxes.begin(), boxes.end(), m_cmp);
    // std::cout << "Füge hinzu " << *it << " ";
    // std::cout << boxes.size() << "this @" << this << std::endl;
  }
}

template <typename T, typename CMP, typename C>
C& SortedBoxListMgr<T, CMP, C>::select_queue()
{
  unsigned short s = (m_turn+1) % 4;
  while(s != m_turn) {
    switch(s) {
    case 0:
      if(!m_boxes1.empty())
	return m_boxes1;
      break;
    case 1:
      if(!m_boxes2.empty())
	return m_boxes2;
      break;
    case 2:
      if(!m_boxes3.empty())
	return m_boxes3;
      break;
    case 3:
      if(!m_boxes4.empty())
	return m_boxes4;
      break;
    }
    s = (s+1) % 4;
  }
  return m_boxes1;
}

template <typename T, typename CMP, typename C>
T* SortedBoxListMgr<T, CMP, C>::select_()
{
  C & boxes = select_queue();
  T* ele = 0;
  assert(boxes.size() && "select_ called on empty element!");
  pop_heap(boxes.begin(), boxes.end(), m_cmp);
  ele = boxes.back();
  boxes.pop_back();
  return ele;
}

template <typename T, typename CMP, typename C>
unsigned SortedBoxListMgr<T, CMP, C>::count_() const
{
  return m_boxes1.size()+m_boxes2.size()+m_boxes3.size()+m_boxes4.size();
}

template <typename T, typename CMP, typename C>
SortedBoxListMgr<T, CMP, C>::SortedBoxListMgr()
  :m_turn(0)
{}

template <typename T, typename CMP, typename C>
SortedBoxListMgr<T, CMP, C>::SortedBoxListMgr(const CMP &cmp)
  :m_turn(0), m_cmp(cmp)
{}


template <typename T, typename CMP, typename C>
typename IBoxListMgr<T>::box_lst_t SortedBoxListMgr<T, CMP, C>::list_() const
{
  typename IBoxListMgr<T>::box_lst_t ret(m_boxes1.size()+m_boxes2.size()+m_boxes3.size()+m_boxes4.size());
  std::copy(m_boxes1.begin(), m_boxes1.end(), ret.begin());
  unsigned offset = m_boxes1.size();
  std::copy(m_boxes2.begin(), m_boxes2.end(), ret.begin()+offset);
  offset += m_boxes2.size();
  std::copy(m_boxes3.begin(), m_boxes3.end(), ret.begin()+offset);
  offset += m_boxes3.size();
  std::copy(m_boxes4.begin(), m_boxes4.end(), ret.begin()+offset);
  return ret;
}

template <typename T, typename CMP, typename C>
void SortedBoxListMgr<T, CMP, C>::clear_()
{
  m_boxes1.clear();
  m_boxes2.clear();
  m_boxes3.clear();
  m_boxes4.clear();
}

template <typename T, typename CMP, typename C>
void SortedBoxListMgr<T, CMP, C>::del_clear_()
{
  for_each(m_boxes1.begin(), m_boxes1.end(), [](T* b) { delete b; });
  for_each(m_boxes2.begin(), m_boxes2.end(), [](T* b) { delete b; });
  for_each(m_boxes3.begin(), m_boxes3.end(), [](T* b) { delete b; });
  for_each(m_boxes4.begin(), m_boxes4.end(), [](T* b) { delete b; });
  clear_();
}


template <typename T, typename CMP, typename C>
bool SortedBoxListMgr<T, CMP, C>::empty_() const
{
  return m_boxes1.empty() && m_boxes2.empty() && m_boxes3.empty() && m_boxes4.empty();
}
