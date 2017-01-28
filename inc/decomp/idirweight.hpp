#ifndef __IDIRWEIGHT_HPP__
#define __IDIRWEIGHT_HPP__

#include "core/arithcore.hpp"

namespace decomp
{

  /// necessary informations for a split parallel to a coordinate axis
  struct s_info_t 
  {
    s_info_t(unsigned short a_dir, const core::arith::mreal& a_weight, 
	     const core::arith::interval &a_point)
      :dir(a_dir), weight(a_weight), point(a_point)
      {}

    s_info_t() {}
    
    unsigned dir;
    core::arith::mreal weight;
    core::arith::interval point;
  };

  inline bool operator<(const s_info_t &s1, const s_info_t &s2)
  {
    return s1.weight > s2.weight;
  }

  /// list of splits
  typedef std::vector<s_info_t> split_lst_t;

  template <typename T>
  class IDirWeight
  {
  public:
    virtual ~IDirWeight() {}

    /** 
     * Generates a weighted list of split
     * No split points are added
     * 
     * @param box box
     * 
     * @return split list
     */
    inline split_lst_t weight(const T& box) const
      {
        return weight_(box);
      }

  private:
    virtual split_lst_t weight_(const T& box) const = 0;
  };
}

#endif /*__IDIRWEIGHT_HPP__*/
