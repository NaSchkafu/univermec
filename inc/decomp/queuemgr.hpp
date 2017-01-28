#ifndef __QUEUEMGR_HPP__
#define __QUEUEMGR_HPP__

#include <deque>
#include "iboxlistmgr.hpp"

namespace decomp 
{
 
  template <typename T>
  class QueueMgr : public IBoxListMgr<T> 
  {
  public:
    QueueMgr();
    virtual ~QueueMgr() { }

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
    unsigned short m_turn;
    std::deque<T*> m_boxes;
  };

  #include "queuemgr.inl"

}

#endif /*__QUEUEMGR_HPP__*/
