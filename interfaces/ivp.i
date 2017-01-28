%{
#include "ivp/iivp.hpp"
#include "ivp/details/dummyu.hpp"
#include "ivp/details/ivpimpl.hpp"
#include "extras/interfaces/vnodelp/vnodelpwrapper.hpp"
#include "extras/interfaces/valencia/valencia.hpp"
%}

// IVP Problem specification
%include "ivp/iivp.hpp"
// IVP problem specification implementation
%include "ivp/details/dummyu.hpp"
%include "ivp/details/ivpimpl.hpp"
// Interfaces to IVP solvers
%include "extras/interfaces/vnodelp/vnodelpwrapper.hpp"
%include "extras/interfaces/valencia/valencia.hpp"
