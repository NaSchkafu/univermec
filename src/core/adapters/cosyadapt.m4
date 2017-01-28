define(`UNARY_STANDARD_FUNCTIONS_taylormodel', ``abs', `acos', `acosh', `asin', `asinh', `atan', `atanh', `cos', `cosh', `exp', `log', `sin', `sinh', `sqr', `Sqrt', `tan', `tanh'')dnl

dnl Laenge eines Cosy Objekts
define(`COSY_LENGTH',10000)

dnl Konstruktoren
 define(`B_PERFORM_MREAL_CTOR_taylormodel', `:m_adapt(0,COSY_LENGTH)
 	{
	  Cosy encl(0,2);
	  interv(to_double(r), to_double(r), encl);
	  m_adapt = cons(encl) + (0*TM(11)+(encl-cons(encl)));
 	}'
 )

 define(`B_PERFORM_MREAL_MREAL_CTOR_taylormodel',`:m_adapt(0,COSY_LENGTH)
 	{
	  Cosy encl(0,2);
	  interv(to_double(lb), to_double(ub), encl);
	  m_adapt = cons(encl) + (0*TM(11)+(encl-cons(encl)));
 	}'
 )

 define(`B_PERFORM_INTERVAL_CTOR_taylormodel',`:m_adapt(0,COSY_LENGTH)
 	{
	  Cosy encl(0,2);
	  interv(to_double(inf(i)), to_double(sup(i)), encl);
	  m_adapt = cons(encl) + (0*TM(11)+(encl-cons(encl)));
 	}
 ')


 define(`B_PERFORM_DEFAULT_CTOR_taylormodel',`
 	:m_adapt(0,COSY_LENGTH) {
	  Cosy encl(0,2);
	  interv(0.0, 0.0, encl);
	  m_adapt = cons(encl) + (0*TM(11)+(encl-cons(encl)));
	}
 ')

dnl Intervallenclosure
 define(`B_PERFORM_ENCL_taylormodel', `
 Cosy enc(ldb(m_adapt));
 return interval(toDouble(inl(enc)), toDouble(inu(enc)));
 ')

# dnl Assigment operator real
# define(`B_PERFORM_OP_=_taylormodel_mreal',
# 	`
# 	m_adapt = riot::TaylorModel::ZERO_TM();
# 	m_adapt += to_double(other);
# 	')

# define(`B_PERFORM_OP_=_taylormodel_interval',
# 	`
# 	m_adapt = riot::TaylorModel::ZERO_TM();
# 	m_adapt += Interval(to_double(inf(other)), to_double(sup(other)));
# 	')

# define(`B_PERFORM_OP_==_taylormodel_taylormodel',`
# assert(!"Comparison of two Taylormodel is not supported in RiOt");
# return false;
# ')

dnl TM Variable erstellen
 define(`B_PERFORM_CREATE_TM_VAR_taylormodel', `
 static unsigned short num = 1;
 // assert(!"FIXME");
 // using namespace riot;
 // static unsigned number = 0;
 // std::stringstream sstr;
 // sstr << "autotm" << number++;

  Cosy ic(0,2);
  interv(to_double(inf(i)), to_double(sup(i)), ic);

  taylormodel tm;
  tm.m_adapt = cons(ic)+0.5*width(ic)*TM((num%10)+1);
  return tm;
 ')

dnl Extra Funktionen zur Initialisierung
define(`EXTRA_PUBLIC_FUNCTIONS_taylormodel',`
 enum TM_BOUNDER { TM_INTERVAL, TM_MEAN_VALUE, TM_LDB };
 static bool init();
dnl FIXME
 static void set_order(unsigned order);
 static void set_bounder(taylormodel::TM_BOUNDER bounder);
 static unsigned order();
 private:
 static bool force_init;
 friend taylormodel create_tm_var(const core::arith::interval &i);
 public:
')

dnl Power Ops
define(`B_PERFORM_BINARY_FUN_pown_taylormodel',`
 arg.m_adapt = pow(arg.m_adapt,e);
 return arg;')
define(`B_PERFORM_BINARY_FUN_pow_taylormodel_mreal',`
 arg.m_adapt = pow(arg.m_adapt,to_double(e));
 return arg;')
 define(`B_PERFORM_BINARY_FUN_pow_taylormodel_taylormodel',`
 arg.m_adapt = pow(arg.m_adapt,e.m_adapt);
 return arg;')

dnl Cast mreal -> Cosy
define(`B_PERFORM_OP_CAST_taylormodel_mreal',``details::mr_to_cosy(other)'')
dnl Cast interval -> Cosy
define(`B_PERFORM_OP_CAST_taylormodel_interval',``details::iv_to_cosy(other)'')
dnl Zusaetzliche Cast Ops
define(`OTHER_EXTRA_INCLUDE_taylormodel',`namespace details { inline Cosy iv_to_cosy(const interval &other) { Cosy ret(0,COSY_LENGTH); 
interv(to_double(inf(other)), to_double(sup(other)), ret); return ret; }
inline Cosy mr_to_cosy(const mreal &other) {
       Cosy ret(0, COSY_LENGTH);
       ret += to_double(other);
       return ret;
} }')

dnl Nicht von Cosy unterstuetzte Ops
define(`B_PERFORM_UNARY_FUN_asinh_taylormodel',`details::arith_op_not_supported(); return args;')
define(`B_PERFORM_UNARY_FUN_acosh_taylormodel',`details::arith_op_not_supported(); return args;')
define(`B_PERFORM_UNARY_FUN_atanh_taylormodel',`details::arith_op_not_supported(); return args;')
define(`B_PERFORM_UNARY_FUN_is_special_taylormodel', `return false;')
define(`B_PERFORM_UNARY_FUN_is_unbounded_taylormodel', `details::arith_op_not_supported(); return false;')
define(`B_PERFORM_UNARY_FUN_is_empty_taylormodel', `details::arith_op_not_supported(); return false;')



REGISTER_OTHER(`taylormodel',`Cosy',`taylormodel',`EXTRA_PUBLIC_FUNCTIONS_taylormodel',`',`// COSY Headers
#include "thirdparty/cosy/cosy.h"
#undef max
#undef min
#undef VOID
',`0',`0')
