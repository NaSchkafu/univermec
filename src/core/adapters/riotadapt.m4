dnl define(`UNARY_STANDARD_FUNCTIONS_taylormodel', ``sin', `cos', `tan', `asin', `acos', `atan', `sinh', `tanh', `asinh', `atanh', `sqrt', `exp', `log', `cosh', `acosh', `sqr', `abs'')dnl
define(`UNARY_STANDARD_FUNCTIONS_taylormodel', ``abs', `acos', `acosh', `asin', `asinh', `atan', `atanh', `cos', `cosh', `exp', `log', `sin', `sinh', `sqr', `sqrt', `tan', `tanh'')dnl
# define(B_PERFORM_UNARY_FUN_inf_interval, `return mreal(_double(Inf(args.m_adapt)));')
# define(B_PERFORM_UNARY_FUN_sup_interval, `return mreal(_double(Sup(args.m_adapt)));')
# define(B_PERFORM_UNARY_FUN_mid_interval, `return mreal(_double(mid(args.m_adapt)));')

define(`B_PERFORM_MREAL_CTOR_taylormodel', `:m_adapt(riot::TaylorModel::ZERO_TM())
	{
	  m_adapt += to_double(r);
	}'
)

define(`B_PERFORM_MREAL_MREAL_CTOR_taylormodel',`:m_adapt(riot::TaylorModel::ZERO_TM())
	{
	  m_adapt += Interval(to_double(lb), to_double(ub));
	}'
)

define(`B_PERFORM_INTERVAL_CTOR_taylormodel',`:m_adapt(riot::TaylorModel::ZERO_TM())
	{
	  m_adapt += Interval(to_double(inf(i)), to_double(sup(i)));
	}
')

dnl define(`B_PERFORM_VAR_DPOINT_DOMAIN_CTOR_taylormodel', `:m_adapt(vname, dpoint, Adapt::Interval(to_double(inf(domain)), 
dnl 						      to_double(sup(domain))))
dnl 	{}')
dnl define(`EXTRA_CTOR_taylormodel',`taylormodel(const std::string &vname, double dpoint, const interval &domain)
dnl	indir(B_PERFORM_VAR_DPOINT_DOMAIN_CTOR_taylormodel)
dnl	explicit taylormodel(const std::string vname)
dnl	indir(B_PERFORM_VAR_CTOR_taylormodel)')

define(`B_PERFORM_VAR_DPOINT_DOMAIN_CTOR_taylormodel', `:m_adapt(vname, 0.0, Adapt::Interval(-1,1))
						        {}')

define(`EXTRA_CTOR_taylormodel', `explicit taylormodel(const std::string &vname)
B_PERFORM_VAR_DPOINT_DOMAIN_CTOR_taylormodel')

define(`B_PERFORM_VAR_CTOR_taylormodel', `
	:m_adapt(vname, 0.0, Adapt::Interval(-1, 1))
	{}')

define(`B_PERFORM_DEFAULT_CTOR_taylormodel',`
	:m_adapt(riot::TaylorModel::ZERO_TM()) {}
')

define(`B_PERFORM_ENCL_taylormodel', `
	  Adapt::Interval i(m_adapt.``eval()'');
	  return interval(mreal(i.inf()), mreal(i.sup()));
')

dnl Assigment operator real
define(`B_PERFORM_OP_=_taylormodel_mreal',
	`
	m_adapt = riot::TaylorModel::ZERO_TM();
	m_adapt += to_double(other);
	')

define(`B_PERFORM_OP_=_taylormodel_interval',
	`
	m_adapt = riot::TaylorModel::ZERO_TM();
	m_adapt += Interval(to_double(inf(other)), to_double(sup(other)));
	')

define(`B_PERFORM_OP_==_taylormodel_taylormodel',`
assert(!"Comparison of two Taylormodel is not supported in RiOt");
return false;
')

define(`B_PERFORM_CREATE_TM_VAR_taylormodel', `
      using namespace riot;
      static unsigned number = 0;
      std::stringstream sstr;
      sstr << "autotm" << (number++)%11;
      return mid(i)+0.5*wid(i)*taylormodel(sstr.str());
')

define(`B_PERFORM_CREATE_TM_VAR_interval_string_taylormodel',`
      return mid(i)+0.5*diam(i)*taylormodel(vname);')

define(`B_PERFORM_UNARY_FUN_abs_taylormodel', `
details::arith_op_not_supported();
return args;
')

define(`EXTRA_PUBLIC_FUNCTIONS_taylormodel',`
enum TM_BOUNDER { TM_INTERVAL, TM_MEAN_VALUE, TM_LDB };
static bool init();
dnl FIXME
static void set_order(unsigned order);
static void set_bounder(taylormodel::TM_BOUNDER bounder);
static unsigned order();
private:
static bool force_init;
public:
')

define(`B_PERFORM_BINARY_FUN_pown_taylormodel',`
arg.m_adapt = power(arg.m_adapt,e);
return arg;')
define(`B_PERFORM_BINARY_FUN_pow_taylormodel_mreal',`
riot::TaylorModel et(riot::TaylorModel::ZERO_TM());
et += to_double(e);
arg.m_adapt = pow(arg.m_adapt,et);
return arg;')
define(`B_PERFORM_BINARY_FUN_pow_taylormodel_taylormodel',`
arg.m_adapt = pow(arg.m_adapt,e.m_adapt);
return arg;')

dnl Automatisch generieren, dafuer Native Cast reparieren...
define(`B_PERFORM_OP_+_taylormodel_interval',`
m_adapt += Adapt::Interval(to_double(inf(other)), to_double(sup(other)));
return *this;
')

define(`B_PERFORM_OP_-_taylormodel_interval',`
m_adapt -= Adapt::Interval(to_double(inf(other)), to_double(sup(other)));
return *this;
')

define(`B_PERFORM_OP_/_taylormodel_interval',`
m_adapt /= Adapt::Interval(to_double(inf(other)), to_double(sup(other)));
return *this;
')

define(`B_PERFORM_OP_*_taylormodel_interval',`
m_adapt *= Adapt::Interval(to_double(inf(other)), to_double(sup(other)));
return *this;
')

define(`B_PERFORM_UNARY_FUN_is_special_taylormodel', `return false;')
define(`B_PERFORM_UNARY_FUN_is_unbounded_taylormodel', `details::arith_op_not_supported(); return false;')
define(`B_PERFORM_UNARY_FUN_is_empty_taylormodel', `details::arith_op_not_supported(); return false;')

REGISTER_OTHER(`taylormodel',`riot::TaylorModel',`taylormodel',`EXTRA_PUBLIC_FUNCTIONS_taylormodel',`',`// RiOt Headers
#define ZERO_XXXXX_ZERO ZERO
#undef ZERO
#include <taylormodel.h>
#define ZERO ZERO_XXXXX_ZERO
',`1',`0')
