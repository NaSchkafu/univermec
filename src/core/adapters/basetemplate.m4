dnl List with arithmetic operations
define(`STANDARD_OPERATIONS',``+',`-',`*',`/',`='')dnl
dnl List with unary functions
define(`UNARY_STANDARD_FUNCTIONS', ``abs', `acos', `acosh', `asin', `asinh', `atan', `atanh', `cos', `cosh', `exp', `log', `sin', `sinh', `sqr', `sqrt', `tan', `tanh'')dnl
dnl List with comparisons supported by all arithmetics
define(`STANDARD_COMPARISONS',``=='')
define(`STANDARD_COMPARISONS_NAMES', ``eq'')

define(`EXTRA_COMPARISONS_mreal',``>',`<',`>=',`<='')
define(`EXTRA_COMPARISONS_NAME_mreal',``greater',`less',`greatereq',`lesseq'')

include(baseadapter.m4)

define(`ADAPTER_TYPES',`FINISHED')

dnl A recursive pushdef
define(`rpushdef',`pushdef(`$1', `$2
popdef(`$1')dnl 
ifdef(`$1', `$1', `')')')dnl

dnl Defines extra operation/ctor signatures for taylormodels
dnl Note: An adapter with the trait type taylormodel has to support these
dnl extra operations by defining the B_PERFORM_xxx macros
define(`OTHER_EXTRA_taylormodel',`
dnl define(EXTRA_CTOR_`'CLASS_NAME,
dnl CLASS_NAME`'(const std::string &vname, double dpoint, const interval &domain)
dnl indir(B_PERFORM_VAR_DPOINT_DOMAIN_CTOR_`'CLASS_NAME)
dnl explicit CLASS_NAME`'(const std::string vname)
dnl indir(B_PERFORM_VAR_CTOR_`'CLASS_NAME))

define(OTHER_EXTRA_PUBLIC_FUNCTIONS_`'CLASS_NAME, 
inline interval encl() const 
{
indir(B_PERFORM_ENCL_`'CLASS_NAME)
})

define(OTHER_EXTRA_FREE_FUNCTIONS_`'CLASS_NAME,
inline CLASS_NAME`' create_tm_var(const interval &i)
{
indir(B_PERFORM_CREATE_TM_VAR_`'CLASS_NAME)
}
dnl inline CLASS_NAME`' create_tm_var(const interval &i, const std::string &vname)
dnl {
dnl indir(B_PERFORM_CREATE_TM_VAR_interval_string_`'CLASS_NAME)
dnl }
)
')dnl

define(`OTHER_EXTRA_aaf',`
define(OTHER_EXTRA_PUBLIC_FUNCTIONS_`'CLASS_NAME,
typedef core::arith::AAFErrorTerm<indir(B_AAF_ERROR_TERM_TYPE_`'CLASS_NAME)> aaf_error_t;
typedef std::vector<aaf::aaf_error_t> error_comb_t;
inline mreal central() const
{
B_PERFORM_FUN_central_`'CLASS_NAME
}
inline error_comb_t comb() const
{
B_PERFORM_FUN_comb_`'CLASS_NAME
}
inline interval encl() const
{
indir(B_PERFORM_encl_`'CLASS_NAME)
}
dnl inline bool valid() const
dnl {
dnl B_PERFORM_FUN_valid_`'CLASS_NAME
dnl }
dnl inline bool infinity() const
dnl {
dnl B_PERFORM_FUN_infinity_`'CLASS_NAME
dnl }
)
define(`OTHER_EXTRA_INCLUDE_aaf',`#include "core/aaferrorterm.inl"
undefine(`OTHER_EXTRA_INCLUDE_aaf')')')

dnl Helper macro for recursively creating all adapters
define(`CREATE_ADAPTERS',`ifelse(ADAPTER_TYPES,`FINISHED',`',`
pushdef(`ADAPTER_TYPES_TMP',ADAPTER_TYPES)dnl
popdef(`ADAPTER_TYPES')dnl
CREATE_ADAPTERS($@)`'dnl
pushdef(`CLASS_NAME',ADAPTER_TYPES_TMP)dnl
$1`'dnl
popdef(`CLASS_NAME')dnl
popdef(`ADAPTER_TYPES_TMP')')')dnl

dnl Helper macro for recursivley creating all header declarations
define(`CREATE_ADAPTERS_HEADERS',`dnl
ifdef(`ADAPTERS_HEADERS',`ADAPTERS_HEADERS
popdef(`ADAPTERS_HEADERS')dnl
CREATE_ADAPTERS_HEADERS',`')')dnl


dnl Macro for performing some basis work for data type registration
dnl $1: Type name
dnl $2: Base Type
dnl $3: Arithmetic type (used for the traits type)
dnl $4: Extra Public
dnl $5: Extra free functions
dnl $6: Required headers
define(`REGISTER_BASE',`
pushdef(`CLASS_NAME', `$1')
pushdef(`ADAPTER_TYPES', CLASS_NAME)
dnl Creates a list of all registered classes
ifdef(`CLASS_LIST',`
define(`CLASS_LIST_TMP',`defn(`CLASS_LIST')')
define(`CLASS_LIST',quote(CLASS_LIST_TMP`,`$1''))
undefine(`CLASS_LIST_TMP')
',`define(`CLASS_LIST',``$1'')')
define(`$1_BASE_TYPE',`$2')
define(`$1_TRAITS_TYPE',`$3_type_t')
dnl Creation of standard operations/functions
B_CREATE_UNARY_OPERATORS
B_CREATE_COPY_CTOR
B_CREATE_STANDARD_OPS(CLASS_NAME,1)
B_CREATE_COMPARISON_OPERATORS(`$1',1)
B_CREATE_STANDARD_UNARY_FUN
B_CREATE_PRINT
B_CREATE_MREAL_CTOR
B_CREATE_DEFAULT_CTOR
pushdef(`EXTRA_PUBLIC_$1',`$4')
pushdef(`FRIEND_DECL_$1',`B_FRIEND_DECL')
pushdef(`FREE_FUNCTIONS_$1',`B_UNARY_FUN_DEF')
pushdef(`ADAPTERS_HEADERS',`$6')
pushdef(`SELF_ASSIGNMENTS_$1',B_SELF_ASSIGNMENTS(`$1',1)
B_COMPARISONS_CLASS(`$1')
ifdef(`EXTRA_COMPARISONS_$1',`B_COMPARISONS_CLASS(`$1',`EXTRA_COMPARISONS_$1',`EXTRA_COMPARISONS_NAME_$1')', `'))
pushdef(`FREE_OPERATORS_$1',B_FREE_OPERATORS($1)
B_FREE_COMPARISON_OPERATORS(`$1')
ifdef(`EXTRA_COMPARISONS_$1', `B_FREE_COMPARISON_OPERATORS(`$1', `EXTRA_COMPARISONS_$1', `EXTRA_COMPARISONS_NAME_$1')',`'))')

dnl Registers an adapter for the underyling scalar real type
dnl of UniVerMeC
dnl $1: Base Type of real
dnl $2: Extra Public
dnl $3: Required headers
define(`REGISTER_REAL', `
ifelse(ADAPTER_TYPES,`FINISHED',`',`ERROR: Please use `REGISTER_REAL' before registering any other adapter! m4exit(1)')
REGISTER_BASE(`mreal', `$1', `mreal', `$2', `', `$6')
dnl Double Ctor
B_CREATE_DOUBLE_CTOR_MREAL
define(`EXTRA_CTOR_mreal', `mreal(double d)
B_PERFORM_DOUBLE_CTOR_MREAL')

dnl Extra friends
rpushdef(FRIEND_DECL_`'CLASS_NAME,
friend double to_double(const mreal& r);
)

rpushdef(FREE_FUNCTIONS_`'CLASS_NAME,
dnl Conversion to double
inline double to_double(const mreal &r)
{
ifdef(`B_PERFORM_UNARY_FUN_to_double_mreal', `B_PERFORM_UNARY_FUN_to_double_mreal', `return r.m_adapt;')
}
dnl Value for infinities
inline mreal infinity() 
{
ifdef(`B_PERFORM_FUN_INFINITY_mreal', `B_PERFORM_FUN_INFINITY_MREAL', `return std::numeric_limits<mreal::base_t>::infinity();')
})
popdef(`CLASS_NAME')
')

dnl Registers an adapter for the underyling interval type
dnl of UniVerMeC
dnl $1: Base Type of interval
dnl $2: Extra Public
dnl $3: Extra free functions
dnl $4: Required headers
define(`REGISTER_INTERVAL', `
ifelse(ADAPTER_TYPES,`mreal',`',`ERROR: Please use `REGISTER_INTERVAL' directly after registering the mreal adapter! m4exit(1)')
define(`STANDARD_OPERATIONS_TMP',defn(`STANDARD_OPERATIONS'))
pushdef(`STANDARD_OPERATIONS', `STANDARD_OPERATIONS_TMP,`&',`|'')
REGISTER_BASE(`interval', `$1', `interval', `$2', `$3', `$4')
popdef(`STANDARD_OPERATIONS')
undefine(`STANDARD_OPERATIONS_TMP')
B_CREATE_STANDARD_OPS(`mreal',1)
B_CREATE_STANDARD_UNARY_FUN
B_CREATE_COMPARISON_OPERATORS(`mreal',0)
dnl Default Impl fuer is_special, is_empty, is_unbound
ifdef(`B_PERFORM_UNARY_FUN_is_special_interval', `', `
define(B_PERFORM_UNARY_FUN_is_special_interval, `
return inf(args) > sup(args) || !std::isfinite(to_double(inf(args))) || !std::isfinite(to_double(sup(args)));
')')
ifdef(`B_PERFORM_UNARY_FUN_is_empty_interval', `', `
define(B_PERFORM_UNARY_FUN_is_empty_interval, `
return inf(args) > sup(args);
')')
ifdef(`B_PERFORM_UNARY_FUN_is_unbounded_interval', `', `
define(B_PERFORM_UNARY_FUN_is_unbounded_interval, `
return std::isinf(to_double(inf(args))) || std::isinf(to_double(sup(args)));
')')
dnl Extra Ops aus Liste auslesen (Reihenfolge: inf, sup, mid, wid, rad)
ifdef(`B_PERFORM_UNARY_FUN_inf_interval',`',`
define(B_PERFORM_UNARY_FUN_inf_interval, B_PERFORM_UNARY_FUN_IV_MREAL_STD(at(listlen(UNARY_STANDARD_FUNCTIONS), UNARY_STANDARD_FUNCTIONS_interval)))')
ifdef(`B_PERFORM_UNARY_FUN_sup_interval',`',`
define(B_PERFORM_UNARY_FUN_sup_interval, B_PERFORM_UNARY_FUN_IV_MREAL_STD(at(incr(listlen(UNARY_STANDARD_FUNCTIONS)), UNARY_STANDARD_FUNCTIONS_interval)))')
ifdef(`B_PERFORM_UNARY_FUN_mid_interval',`',`
define(B_PERFORM_UNARY_FUN_mid_interval, B_PERFORM_UNARY_FUN_IV_MREAL_STD(at(incr(incr(listlen(UNARY_STANDARD_FUNCTIONS))), UNARY_STANDARD_FUNCTIONS_interval)))')
ifdef(`B_PERFORM_UNARY_FUN_wid_interval',`',`
define(B_PERFORM_UNARY_FUN_wid_interval, B_PERFORM_UNARY_FUN_IV_MREAL_STD(at(incr(incr(incr(listlen(UNARY_STANDARD_FUNCTIONS)))), UNARY_STANDARD_FUNCTIONS_interval)))')
ifdef(`B_PERFORM_UNARY_FUN_rad_interval',`',`
define(B_PERFORM_UNARY_FUN_rad_interval, B_PERFORM_UNARY_FUN_IV_MREAL_STD(at(incr(incr(incr(incr(listlen(UNARY_STANDARD_FUNCTIONS))))), UNARY_STANDARD_FUNCTIONS_interval)))')
ifdef(`B_PERFORM_BINARY_FUN_operator_input', `',`
define(B_PERFORM_BINARY_FUN_operator_input,`is >> iv.m_adapt;')' )
define(FREE_FUNCTIONS_`'CLASS_NAME, B_UNARY_FUN_DEF
inline mreal inf(const interval& args)
{
B_PERFORM_UNARY_FUN_inf_interval
}
inline mreal sup(const interval &args)
{
B_PERFORM_UNARY_FUN_sup_interval
}
inline mreal mid(const interval &args)
{
B_PERFORM_UNARY_FUN_mid_interval
}
inline interval interval_pi()
{
B_PERFORM_FUN_interval_pi_interval
}
inline mreal wid(const interval &args)
{
B_PERFORM_UNARY_FUN_wid_interval
}
inline mreal rad(const interval &args)
{
B_PERFORM_UNARY_FUN_rad_interval
}

inline std::istream& operator>>(std::istream &is, interval &iv)
{
B_PERFORM_BINARY_FUN_operator_input
return is;
}
)dnl
B_CREATE_MREAL_MREAL_CTOR
rpushdef(FRIEND_DECL_`'CLASS_NAME,
friend mreal inf(const interval&);
friend mreal sup(const interval&);
friend mreal mid(const interval&);
friend mreal wid(const interval&);
friend mreal rad(const interval&);
friend interval interval_pi();
friend std::istream& operator>>(std::istream &is, interval &iv);
)
rpushdef(SELF_ASSIGNMENTS_`'CLASS_NAME,`B_SELF_ASSIGNMENTS(`mreal',1)
B_COMPARISONS_CLASS(`mreal')
')
rpushdef(FREE_OPERATORS_`'CLASS_NAME,`B_FREE_OPERATORS(mreal)
B_FREE_COMPARISON_OPERATORS(`mreal')
')
dnl rpushdef(`EXTRA_PUBLIC_interval',`inline void set_inf(const mreal &r)
dnl {
dnl B_PERFORM_UNARY_FUN_set_inf_interval
dnl }
dnl inline void set_sup(const mreal& r)
dnl {
dnl B_PERFORM_UNARY_FUN_set_sup_interval
dnl }
dnl ')
rpushdef(`FRIEND_DECL_mreal',`friend class interval;')
popdef(`CLASS_NAME')
')

dnl Registers an adapter for an arithmetic type at the third level
dnl of UniVerMeC
dnl $1: Type name
dnl $2: Base Type of other
dnl $3: Arithmetic type (used for the traits type)
dnl $4: Extra Public
dnl $5: Extra free functions
dnl $6: Required headers
dnl $7: Native cast mreal?
dnl $8: Native cast interval?
define(`REGISTER_OTHER', `
ifelse(ADAPTER_TYPES,`mreal',`ERROR: Please use `REGISTER_INTERVAL' directly after registering the mreal adapter! m4exit(1)')
ifelse(ADAPTER_TYPES,`FINISHED',`ERROR: Please use `REGISTER_REAL' first. m4exit(1)')
REGISTER_BASE(`$1', `$2', `$3', `$4', `$5', `$6')
ifdef(`OTHER_EXTRA_$3', `OTHER_EXTRA_$3', `')
B_CREATE_STANDARD_OPS(`interval',$8)
B_CREATE_STANDARD_OPS(`mreal',$7)
rpushdef(FREE_FUNCTIONS_`'CLASS_NAME, 
ifdef(OTHER_EXTRA_FREE_FUNCTIONS_`'CLASS_NAME,`indir(OTHER_EXTRA_FREE_FUNCTIONS_`'CLASS_NAME)')
inline CLASS_NAME CLASS_NAME`'_pi()
{
ifdef(B_PERFORM_pi_`'CLASS_NAME,`indir(B_PERFORM_pi_`'CLASS_NAME)',`return CLASS_NAME`'(interval_pi());')
})
B_CREATE_MREAL_MREAL_CTOR
rpushdef(`EXTRA_PUBLIC_$1', `
indir(OTHER_EXTRA_PUBLIC_FUNCTIONS_`'CLASS_NAME)')
rpushdef(SELF_ASSIGNMENTS_`'CLASS_NAME,`B_SELF_ASSIGNMENTS(`mreal',1)
B_SELF_ASSIGNMENTS(`interval',1)
')
rpushdef(`FREE_OPERATORS_$1',`B_FREE_OPERATORS(interval)
B_FREE_OPERATORS(mreal)
')
dnl FREE_OPERATORS_taylormodel
rpushdef(`FRIEND_DECL_mreal',`friend class $1;')
rpushdef(`FRIEND_DECL_interval',`friend class $1;')
popdef(`CLASS_NAME')
')

define(`CREATE_CLASS',`dnl
    ifdef(OTHER_EXTRA_INCLUDE_`'CLASS_NAME,`indir(OTHER_EXTRA_INCLUDE_`'CLASS_NAME)')
    class CLASS_NAME 
    {
    public:
      CLASS_NAME`'()
      indir(B_PERFORM_DEFAULT_CTOR_`'CLASS_NAME)

      CLASS_NAME`'(const CLASS_NAME`'& other)
      indir(B_PERFORM_COPY_CTOR_`'CLASS_NAME)

      ifelse(CLASS_NAME,`mreal',`',`explicit CLASS_NAME`'(const mreal`'& r)
      indir(B_PERFORM_MREAL_CTOR_`'CLASS_NAME)

      CLASS_NAME`'(const mreal& lb, const mreal &ub)
      indir(B_PERFORM_MREAL_MREAL_CTOR_`'CLASS_NAME)

      ifelse(CLASS_NAME,`interval',`',`explicit CLASS_NAME`'(const interval& i)
      indir(B_PERFORM_INTERVAL_CTOR_`'CLASS_NAME)')')

      ifdef(EXTRA_CTOR_`'CLASS_NAME,EXTRA_CTOR_`'CLASS_NAME,`')
      indir(EXTRA_PUBLIC_`'CLASS_NAME)

      typedef indir(CLASS_NAME`'_BASE_TYPE) base_t;
      typedef void indir(CLASS_NAME`'_TRAITS_TYPE);

      indir(SELF_ASSIGNMENTS_`'CLASS_NAME)

      friend CLASS_NAME operator-(const CLASS_NAME`'&);
      indir(FRIEND_DECL_`'CLASS_NAME)
      friend CLASS_NAME pow(CLASS_NAME, const mreal&);
      friend CLASS_NAME pow(CLASS_NAME, int);
      friend bool is_special(const CLASS_NAME&);
      friend bool is_empty(const CLASS_NAME&);
      friend bool is_unbounded(const CLASS_NAME&);
      ifelse(CLASS_NAME,`mreal',`',`friend CLASS_NAME pow(CLASS_NAME, const CLASS_NAME`'&);')
      inline void print(std::ostream &os) const
      {
	indir(B_PERFORM_PRINT_`'CLASS_NAME)
      }

    private:
      base_t m_adapt;
    };

    inline CLASS_NAME`'& operator+(CLASS_NAME`'& arg)
    {
	return arg;
    }

    inline const CLASS_NAME operator+(const CLASS_NAME`'&arg)
    {
	return arg;
    }

    inline CLASS_NAME operator-(const CLASS_NAME`'& arg)
    {
	indir(B_PERFORM_UNARY_OP_MINUS_`'CLASS_NAME)
    }

    indir(FREE_OPERATORS_`'CLASS_NAME)
    indir(FREE_FUNCTIONS_`'CLASS_NAME)
    inline CLASS_NAME pow(CLASS_NAME arg, int e)
    {
	indir(B_PERFORM_BINARY_FUN_pown_`'CLASS_NAME)
    }
    inline CLASS_NAME pow(CLASS_NAME arg, const mreal &e)
    {
	indir(B_PERFORM_BINARY_FUN_pow_`'CLASS_NAME`'_mreal)
    }
    inline bool is_special(const CLASS_NAME &args) 
    {
    	indir(B_PERFORM_UNARY_FUN_is_special_`'CLASS_NAME)
    }
    inline bool is_empty(const CLASS_NAME &args) 
    {
    	indir(B_PERFORM_UNARY_FUN_is_empty_`'CLASS_NAME)
    }
    inline bool is_unbounded(const CLASS_NAME &args) 
    {
    	indir(B_PERFORM_UNARY_FUN_is_unbounded_`'CLASS_NAME)
    }
    ifelse(CLASS_NAME,`mreal',`',`inline CLASS_NAME pow(CLASS_NAME arg, const CLASS_NAME`'& e) 
    {
	indir(B_PERFORM_BINARY_FUN_pow_`'CLASS_NAME`'_`'CLASS_NAME)
    }')

    inline std::ostream& operator<<(std::ostream &os, const CLASS_NAME`'& x)
    {
	x.print(os);
	return os;
    }

')dnl
