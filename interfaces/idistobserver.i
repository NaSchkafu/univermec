%{
#include "stable.h"
#include "algorithm/idistobserver.hpp"
%}

%feature("director") algorithm::IDistObserver;
//%feature("notabstract") algorithm::IDistObserver;

%include "algorithm/idistobserver.hpp"
