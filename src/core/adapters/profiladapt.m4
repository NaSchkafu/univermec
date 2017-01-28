define(`UNARY_STANDARD_FUNCTIONS_interval', ``Abs', `ArcCos', `ArCosh', `ArcSin', `ArSinh', `ArcTan', `ArTanh', `Cos', `Cosh', `Exp', `Log10', `Sin', `Sinh', `Sqr', `Sqrt', `Tan', `Tanh', `Inf', `Sup', `Mid', `Diam', `rad'')dnl

define(B_PERFORM_UNARY_FUN_rad_interval, `return mreal(Diam(args.m_adapt)/2.0);')
define(`B_PERFORM_FUN_interval_pi_interval',`interval ret(0.0);
ret.m_adapt = ArcCos(INTERVAL(-1.0, -1.0));
return ret;')
dnl define(`B_PERFORM_UNARY_FUN_set_inf_interval', `Inf(m_adapt) = to_double(r);')
dnl define(`B_PERFORM_UNARY_FUN_set_sup_interval', `Sup(m_adapt) = to_double(r);')

dnl Power Function
define(`B_PERFORM_BINARY_FUN_pown_interval',`arg.m_adapt = Power(arg.m_adapt, e); return arg;')
define(`B_PERFORM_BINARY_FUN_pow_interval_mreal',`arg.m_adapt = Power(arg.m_adapt, to_double(e)); return arg;')
define(`B_PERFORM_BINARY_FUN_pow_interval_interval',`arg.m_adapt = Power(arg.m_adapt, e.m_adapt); return arg;')

dnl Hull Operator
define(`B_PERFORM_OP_|_interval_interval', `m_adapt = Hull(m_adapt, other.m_adapt);')
define(`B_PERFORM_OP_&_interval_interval', `if(!Intersection(m_adapt, m_adapt, other.m_adapt)) m_adapt = INTERVAL(1, -1);')


REGISTER_INTERVAL(`INTERVAL',`',`// PROFIL/BIAS Headers
#include <Interval.h>
#include <Functions.h>
')

