define(`UNARY_STANDARD_FUNCTIONS_interval', ``abs', `acos', `acosh', `asin', `asinh', `atan', `atanh', `cos', `cosh', `exp', `log10', `sin', `sinh', `sqr', `sqrt', `tan', `tanh', `Inf', `Sup', `mid'')dnl

define(B_PERFORM_UNARY_FUN_inf_interval, `return mreal(_double(Inf(args.m_adapt)));')
define(B_PERFORM_UNARY_FUN_sup_interval, `return mreal(_double(Sup(args.m_adapt)));')
define(B_PERFORM_UNARY_FUN_mid_interval, `return mreal(_double(mid(args.m_adapt)));')
define(B_PERFORM_UNARY_FUN_rad_interval, `return mreal(_double(diam(args.m_adapt)/2.0));')
define(B_PERFORM_UNARY_FUN_wid_interval, `return mreal(_double(diam(args.m_adapt)));')
define(`B_PERFORM_FUN_interval_pi_interval',`interval ret(0.0);
ret.m_adapt = cxsc::Pi();
return ret;')
define(`B_PERFORM_UNARY_FUN_set_inf_interval', `Inf(m_adapt) = to_double(r);')
define(`B_PERFORM_UNARY_FUN_set_sup_interval', `Sup(m_adapt) = to_double(r);')

define(`B_PERFORM_BINARY_FUN_pown_interval',`
arg.m_adapt = power(arg.m_adapt, e);
return arg;')

define(`B_PERFORM_BINARY_FUN_pow_interval_mreal',`
// TODO: Workaround fuer C-XSC
// General pow function selbst implementieren (pow2 wird fuer SQ normalen benoetigt...)
if(e < 0 && inf(arg) <= 0)
     arg.m_adapt = cxsc::interval(cxsc::QuietNaN);
else
     arg.m_adapt = pow(arg.m_adapt, cxsc::interval(to_double(e)));
//     throw "General interval function currently not supported!";
return arg;')

define(`B_PERFORM_BINARY_FUN_pow_interval_interval',`
arg.m_adapt = pow(arg.m_adapt, e.m_adapt);
return arg;')

REGISTER_INTERVAL(`cxsc::interval',`',`// C-XSC Headers
#include <interval.hpp>
#include <imath.hpp>
')

