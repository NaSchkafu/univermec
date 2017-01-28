%{
#include "iencl.hpp"
#include "ilieencl.hpp"
#include "meanvalue.hpp"
#include "nencl.hpp"
%}

%include "functions/iencl.hpp"
%include "functions/enclosures/ilieencl.hpp"
%include "functions/enclosures/meanvalue.hpp"
%include "functions/enclosures/nencl.hpp"

namespace functions
{
  namespace encl {
    %template(IVEncl) NEncl<0>;
    %template(AAFEncl) NEncl<1>;
    %template(TMEncl) NEncl<2>;
  }
}
