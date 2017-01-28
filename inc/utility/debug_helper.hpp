#include "stable.h"
#ifndef __DEBUG_HELPER_HPP__
#define __DEBUG_HELPER_HPP__
namespace utility 
{
  namesapce debug 
  {
    template<typename A>
      void print_array(const A* arr, unsigned size) 
    {
      for(unsigned i = 0; i < size; i++)
	std::cout << arr[i] << " ";
    }
  }
}

#endif /*__DEBUG_HELPER_HPP__*/
