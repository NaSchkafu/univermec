#ifndef __WEIGHTMW_HPP__
#define __WEIGHTMW_HPP__

#include "idirweight.hpp"

namespace decomp 
{

  template <typename T>
  class WeightMW : public IDirWeight<T> 
  {
  public:
    virtual ~WeightMW() {}

  private:
    virtual split_lst_t weight_(const T& box) const;
  };

  #include "weightmw.inl"

}

#endif /*__WEIGHTMW_HPP__*/
