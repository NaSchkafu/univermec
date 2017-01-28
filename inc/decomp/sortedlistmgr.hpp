#ifndef __SORTEDLISTMGR_HPP__
#define __SORTEDLISTMGR_HPP__

#include <functional>
#include <deque>

#include "iboxlistmgr.hpp"

namespace decomp 
{
 
  template <typename T, typename CMP = std::less<T>, typename C = std::deque<T*> >
  class SortedBoxListMgr : public IBoxListMgr<T> 
  {
  public:
    SortedBoxListMgr();
    explicit SortedBoxListMgr(const CMP &t);
    virtual ~SortedBoxListMgr() { }

  private:
    virtual void add_(T* b);
    virtual void add_(const typename IBoxListMgr<T>::box_lst_t &lst);
    virtual T* select_();
    virtual unsigned count_() const;
    virtual typename IBoxListMgr<T>::box_lst_t list_() const;
    virtual void clear_();
    virtual void del_clear_();   
    virtual bool empty_() const;

  private:
    C& select_queue();

    unsigned short m_turn;
    C m_boxes1;
    C m_boxes2;
    C m_boxes3;
    C m_boxes4;
    CMP m_cmp;
  };

  #include "sortedlistmgr.inl"

}

#endif /*__SORTEDLISTMGR_HPP__*/
