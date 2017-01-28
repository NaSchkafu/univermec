#ifndef __INTERFACE_EXCEPTION_HPP__
#define __INTERFACE_EXCEPTION_HPP__

#include "core/core_exception.hpp"

namespace extras
{
  namespace interfaces 
  {
    /// Generated if an error during an interface call occurs
    struct interface_exception : virtual core::univermec_exception {};
    /// Generated if the interface is called with invalid inputs
    struct interface_input_exception : virtual interface_exception {};
  }
}

#endif /*__INTERFACE_EXCEPTION_HPP__*/
