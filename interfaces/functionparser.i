%{
#include "stable.h"
#include "functionparser.hpp"  
%}

namespace functions
{
  class IFunction;
  namespace parser
  {
    IFunction* parse_string(const std::string &fstr);
  }
}
