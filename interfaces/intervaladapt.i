%{ 
#include "stable.h"
%}

namespace core {
  namespace arith {
    %extend interval 
    {
      char* __str__() 
      {
	static char tmp[1024];
	std::stringstream sstr;
	sstr << *$self;
	strncpy(tmp, sstr.str().c_str(), 1024);
	return tmp;
      }

      interval& set(const interval &other) 
      {
	*$self = other;
	return *$self;
      }

      interval& set(const mreal &other) 
      {
	*$self = other;
	return *$self;
      }
    }
  }
}




