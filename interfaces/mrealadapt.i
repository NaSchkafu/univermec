%{
#include <iosfwd>
#include "stable.h"
%}

%include "adapter.hpp"
namespace core{ 
namespace arith {
    %extend mreal
    {
      char* __str__()
      {
        static char tmp[1024];
        sprintf(tmp, "%f", to_double(*$self));
        return tmp;
      }

      mreal& set(const mreal &other)
      {
        *$self = other;
        return *$self;
      }
    };

  }
}
