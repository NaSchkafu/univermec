#ifndef __MIDCHOOSER_HPP__
#define __MIDCHOOSER_HPP__

#include "ipointchooser.hpp"

namespace decomp 
{
  template <typename T>
  class MidChooser : public IPointChooser<T>
  {
  private:
    virtual void pchoose_(const T& box, 
			  split_lst_t &dlst,
			  unsigned sects) const;
  };

  #include "midchooser.inl"
}

#endif /*__MIDCHOOSER_HPP__*/
