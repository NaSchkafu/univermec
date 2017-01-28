#include "tests/test_vnode.hpp"

#include "core/arithcore.hpp"

#include "ivp/iivp.hpp"

#include "extras/interfaces/vnodelp/vnodelpwrapper.hpp"

#include "tests/lorenzivp.hpp"

void test_vnode()
{
  ivp::IIVP *lorenz = create_lorenz_ivp();
  //extras::interfaces::solve_ivp_vnodelp(*lorenz, 20, std::cout, 1.0);
}
