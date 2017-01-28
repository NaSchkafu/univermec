#include "ode-eq.hpp"

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"
#include "functions/functionhelper.hpp"

namespace ode
{
 

  const ivp::IIVP* ode_problem_t::S_PROBLEM = nullptr;
  unsigned ode_problem_t::S_DIM = 0;
  unsigned ode_problem_t::P_DIM = 0;
  unsigned ode_problem_t::U_DIM = 0;
  unsigned ode_problem_t::NEQ = 0;
 // bool ode_problem_t::S_FWD = true;
  core::arith::mreal ode_problem_t::S_STOP_TIME = 1.0;
}
