#ifndef __CXSCCOMP_HPP__
#define __CXSCCOMP_HPP__

#include <core/arithcore.hpp>

namespace core
{
  namespace arith
  {
    template<typename T>
    int Ub(const T & v);

    template<typename T>
    int Lb(const T & v);

    template<typename T>
    int Ub(const T & v, int i);

    template<typename T>
    int Lb(const T & v, int i);

    template<typename T>
    T Id(const T &v);

    inline bool isEmpty(const interval &i)
    {
      return sup(i) < inf(i);
    }

    inline bool isEmpty(const ArithTraits<interval>::vec_t &iv)
    {
      for(unsigned i = 0; i < size(iv);i++)
	if(isEmpty(iv[i]))
	  return true;
      return false;
    }

   #include "cxsccomp.inl"
  }
}




#endif /*__CXSCCOMP_HPP__*/
