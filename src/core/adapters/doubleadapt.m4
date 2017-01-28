define(`UNARY_STANDARD_FUNCTIONS_mreal', ``std::fabs', `std::acos', `std::acosh', `std::asin', `std::asinh', `std::atan', `std::atanh', `std::cos', `std::cosh', `std::exp', `std::log', `std::sin', `std::sinh', `std::sqr', `std::sqrt', `std::tan', `std::tanh'')dnl

define(`B_PERFORM_UNARY_FUN_sqr_mreal', `args.m_adapt *= args.m_adapt;')
define(`B_PERFORM_BINARY_FUN_pown_mreal',`
arg.m_adapt = std::pow(arg.m_adapt, e);
return arg;')
define(`B_PERFORM_BINARY_FUN_pow_mreal_mreal',`arg.m_adapt = std::pow(arg.m_adapt, e.m_adapt);
return arg;')
define(`B_PERFORM_UNARY_FUN_is_special_mreal',`return !std::isfinite(args.m_adapt);')
define(`B_PERFORM_UNARY_FUN_is_empty_mreal',`return std::isnan(args.m_adapt);')
define(`B_PERFORM_UNARY_FUN_is_unbounded_mreal',`return std::isinf(args.m_adapt);')
REGISTER_REAL(`double',`',`')
