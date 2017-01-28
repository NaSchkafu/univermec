#ifndef __IPOINTCHOOSER_HPP__
#define __IPOINTCHOOSER_HPP__

#include "idirweight.hpp"

namespace decomp
{
  template <typename T>
  class IPointChooser
  {
  public:
    virtual ~IPointChooser() {}
    /** 
     * Chooses split points depending on the dir ordering
     * 
     * @param box box to split
     * @param slst split list with weights
     * @param sects number of sections
     * 
     */
    inline void pchoose(const T& box, split_lst_t &slst,
                              unsigned sects) const
      {
        return pchoose_(box, slst, sects);
      }

  private:
    virtual void pchoose_(const T& box, split_lst_t &slst,
			  unsigned sects) const = 0;
  };
}

#endif /*__IPOINTCHOOSER_HPP__*/
