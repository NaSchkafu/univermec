#ifndef __IMULTISECTION_HPP__
#define __IMULTISECTION_HPP__

#include <vector>

namespace decomp
{
  template<typename T>
  class IMultiSection
  {
  public:
    virtual ~IMultiSection() {}

    typedef std::vector<T*> box_lst_t;

    inline box_lst_t section(const T& box)
      {
        return section_(box);
      }

  private:
    virtual box_lst_t section_(const T &box) const = 0;
  };
}

#endif /*__IMULTISECTION_HPP__*/


