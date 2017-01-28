define(`YALAA_CLASS_NAME',`aaf')
define(`YALAA_BASE_TYPE',`yalaa::aff_e_d')

define(`UNARY_STANDARD_FUNCTIONS_aaf', ``abs', `acos', `acosh', `asin', `asinh', `atan', `atanh', `cos', `cosh', `exp', `log', `sin', `sinh', `sqr', `sqrt', `tan', `tanh'')dnl

define(B_PERFORM_BINARY_FUN_pown_`'YALAA_CLASS_NAME,`
arg.m_adapt = pown(arg.m_adapt,e);
return arg;')
define(B_PERFORM_BINARY_FUN_pow_`'YALAA_CLASS_NAME`'_mreal,
arg.m_adapt = pow(arg.m_adapt,YALAA_BASE_TYPE`'(to_double(e)));
return arg;)
define(B_PERFORM_BINARY_FUN_pow_`'YALAA_CLASS_NAME`'_`'YALAA_CLASS_NAME,`
arg.m_adapt = pow(arg.m_adapt,e.m_adapt);
return arg;')
define(B_PERFORM_FUN_comb_`'YALAA_CLASS_NAME,
const YALAA_BASE_TYPE`'::ac_t &c = m_adapt.ac();
error_comb_t comb;
for(YALAA_BASE_TYPE`'::ac_t::aff_comb_const_iter it(c.begin()); it != c.end(); ++it)
  comb.push_back(aaf_error_t(*it));
return comb;
)
define(B_PERFORM_FUN_central_`'YALAA_CLASS_NAME,`return mreal(m_adapt.ac().central());')

define(B_AAF_ERROR_TERM_TYPE_`'YALAA_CLASS_NAME,YALAA_BASE_TYPE`'::error_t)

define(B_PERFORM_MREAL_MREAL_CTOR_`'YALAA_CLASS_NAME, `
:m_adapt(interval(lb,ub).m_adapt) {}
')

dnl Special
define(B_PERFORM_UNARY_FUN_is_special_`'YALAA_CLASS_NAME,`return aaf::base_t::ep_t::valid(args.m_adapt);')
dnl Unbound
define(B_PERFORM_UNARY_FUN_is_unbounded_`'YALAA_CLASS_NAME,`return args.m_adapt.special() == aaf::base_t::ep_t::R;')
dnl empty
define(B_PERFORM_UNARY_FUN_is_empty_`'YALAA_CLASS_NAME,`return args.m_adapt.special() == aaf::base_t::ep_t::EMPTY;')

define(B_PERFORM_INTERVAL_CTOR_`'YALAA_CLASS_NAME,`:m_adapt(i.m_adapt){}')

define(B_PERFORM_UNARY_FUN_abs_`'YALAA_CLASS_NAME,`details::arith_op_not_supported();
return args;')

define(B_PERFORM_OP_=_`'YALAA_CLASS_NAME`'_interval,`m_adapt = base_t(other.m_adapt);')

define(B_PERFORM_OP_=_`'YALAA_CLASS_NAME`'_mreal,`m_adapt = base_t(other.m_adapt);')

define(B_PERFORM_encl_`'YALAA_CLASS_NAME,`const yalaa::details::double_iv_t &iv = to_iv(m_adapt);
return core::arith::interval(yalaa::traits::interval_traits<yalaa::details::double_iv_t>::my_inf(iv),
			     yalaa::traits::interval_traits<yalaa::details::double_iv_t>::my_sup(iv));')

REGISTER_OTHER(YALAA_CLASS_NAME,YALAA_BASE_TYPE,`aaf',,`',`// YalAA Headers
#include <yalaa.hpp>
',`1',`1')
