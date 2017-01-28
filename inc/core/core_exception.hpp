#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__ 

#include <boost/exception/all.hpp>

namespace core 
{
  struct univermec_exception : virtual std::exception, virtual boost::exception {};
  struct core_exception : virtual univermec_exception {};
  struct conv_exception : virtual core_exception {};
  typedef boost::error_info<struct tag_error_details, std::string> error_details;
}


#endif /*__EXCEPTION_HPP__*/
