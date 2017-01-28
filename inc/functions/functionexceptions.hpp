#ifndef __FUNCTIONEXCEPTIONS_HPP__
#define __FUNCTIONEXCEPTIONS_HPP__

#include "core/core_exception.hpp"

namespace functions
{
  /// base class for all exceptions thrown in the functions package
  struct functions_exception : virtual core::univermec_exception {};
  typedef boost::error_info<struct tag_functions_invalid_idx, int> invalid_idx;

  /// Thrown by the [function parser](@ref functions::parser)
  struct parser_exception : virtual functions_exception {};
  typedef boost::error_info<struct tag_parser_err_pos, int> parser_err_pos;
  typedef boost::error_info<struct tag_unknown_variable, int> parser_unknown_variable;
}


#endif /*__FUNCTIONEXCEPTIONS_HPP__*/
