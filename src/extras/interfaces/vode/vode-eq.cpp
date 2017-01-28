#include "vode-eq.hpp"

#include "functions/ifunction.hpp"
#include "functions/ivfunction.hpp"
#include "functions/functionhelper.hpp"

namespace vode
{

  const ivp::IIVP* vode_problem_t::S_PROBLEM = nullptr;
  unsigned vode_problem_t::S_DIM = 0;
  unsigned vode_problem_t::P_DIM = 0;
  unsigned vode_problem_t::U_DIM = 0;
  unsigned vode_problem_t::NEQ = 0;
  core::arith::mreal vode_problem_t::S_STOP_TIME = 1.0;
}


