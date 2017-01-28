#include "valencia.hpp"

#include "ivp/iivp.hpp"
#include "thirdparty/valencia/valencia-eq.hpp"

int valencia_main_internal_(const std::string &file);

namespace extras
{
  namespace interfaces
  {
    void solve_ivp_valencia(const ivp::IIVP &ivp, const core::arith::mreal &stop,
                const core::arith::mreal &step, const std::string &file)
    {
      valencia::valencia_problem_t::set_problem(&ivp);
      valencia::valencia_problem_t::set_stop_time(stop);
      valencia::valencia_problem_t::set_step_size(step);
      valencia_main_internal_(file);
    }
  }
}
