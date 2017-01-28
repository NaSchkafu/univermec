dnl This file contains functions which are used for automatically generating
dnl arithmetic adapters.
dnl Tools
define(`shiftn', `ifelse(eval($1 > 0), 1, `shiftn(decr($1), shift(shift($@)))', `shift($@)')')dnl
dnl define(`listlen2', `ifelse($#,2,`$1',`listlen2(incr($1),shiftn(2,$@))')')dnl
dnl define(`listlen', `ifelse($#,0,`$0',`listlen2(1, $@)')')dnl
define(`listlen', `$#')dnl
define(`at2',`ifelse($1,`0',`$2',`at2(decr($1),shiftn(2,$@))')')
define(`at',`at2($*)')dnl
define(`dequote',$1)
define(`quote',$@)
define(`quote2',``$@'')

dnl Template for a standard unary function call
dnl (cf. B_PERFORM_OP_STD for details)
dnl $1 Function
define(`B_PERFORM_UNARY_FUN_STD',`args.m_adapt = $1(args.m_adapt);')dnl

define(`B_PERFORM_UNARY_FUN_IV_MREAL_STD', `return $1(args.m_adapt);')dnl

dnl Creates macros for unary standard function class
dnl This macro creates all required B_PERFORM_UNARY_FUN_<funname>_xx macros using B_PERFORM_UNARY_FUN_STD
dnl and the function names specified in UNARY_STANDARD_FUNCTIONS_xx
define(`B_CREATE_STANDARD_UNARY_FUN', `ifelse($#, 0, `B_CREATE_STANDARD_UNARY_FUN(0, listlen(UNARY_STANDARD_FUNCTIONS))',dnl
`ifdef(B_PERFORM_UNARY_FUN_`'at($1,UNARY_STANDARD_FUNCTIONS)`'_`'CLASS_NAME,`',`pushdef(B_PERFORM_UNARY_FUN_`'at($1,UNARY_STANDARD_FUNCTIONS)`'_`'CLASS_NAME, B_PERFORM_UNARY_FUN_STD(at($1,UNARY_STANDARD_FUNCTIONS_`'CLASS_NAME)))')dnl
ifelse(eval($1 < $2),1,`B_CREATE_STANDARD_UNARY_FUN(incr($1),$2)')')')dnl

dnl Template for a standard operation
dnl This macro is used for creating predefined PEFORM_OP_?_xx_yy macros which carry out the actual
dnl operation. If the library used needs another scheme, you have to create those yourself.
dnl $1: Type of operation
dnl $2: Type of other operand
dnl $3: Native cast possible
dnl $3 indicates whether BASE_TYPE can directly combined with BASE_TYPE of $2
dnl NOTE: $3 is only used if $2 != CLASS_NAME
define(`B_PERFORM_OP_STD', `ifelse(eval($3 + ifelse(`CLASS_NAME', $2, 1, 0) >  0),1,dnl
`m_adapt $1ifelse(`$1',`=',`',`=') other.m_adapt',dnl
`m_adapt $1ifelse(`$1',`=',`',`=') B_PERFORM_OP_CAST_`'CLASS_NAME`'_$2(other)');
')dnl

define(`B_PERFORM_OP_CMP', `ifelse(eval($3 + ifelse(`CLASS_NAME', $2, 1, 0) > 0), 1,
`return m_adapt $1 other.m_adapt',dnl
`return m_adapt $1 CLASS_NAME`'(other).m_adapt');')

dnl Creates macros for standard operations
dnl This macro creates all requires PERFORM_OP_?_xx_yy macros using B_PERFORM_OP_STD
dnl $1: Type of the other operand
dnl $2: Nativer cast possible
dnl $3: B_PERFORM macro (optional)
dnl $4: Set of operations (optional)
define(`B_CREATE_STANDARD_OPS',`dnl
ifelse(eval($# == 2), 1, `B_CREATE_STANDARD_OPS($1, $2, `B_PERFORM_OP_STD', STANDARD_OPERATIONS)',dnl
`ifdef(B_PERFORM_OP_$4_`'CLASS_NAME`'_$1,`',`pushdef(B_PERFORM_OP_$4_`'CLASS_NAME`'_$1, $3($4, $1, $2))dnl
ifelse(eval($# > 4),1, `B_CREATE_STANDARD_OPS($1, $2, `$3', shiftn(4,$@))')')')dnl
')dnl

dnl Create macros for comparion operations
dnl This macro creates all required PERFORM_OP_?_xx_yy macros using B_PERFORM_OP_CMP
dnl $1 Type of other operand
dnl $2: Native cast possible
define(`B_CREATE_COMPARISON_OPERATORS', `B_CREATE_STANDARD_OPS($1, $2, `B_PERFORM_OP_CMP', STANDARD_COMPARISONS)
ifdef(EXTRA_COMPARISONS_`'CLASS_NAME, `B_CREATE_STANDARD_OPS($1, $2, `B_PERFORM_OP_CMP', dequote(EXTRA_COMPARISONS_`'CLASS_NAME))'.`')')

dnl Expands to one SELF_ASSIGMENT operation for the current CLASS_NAME
dnl $1: Type of operation
dnl $2: Type of other operand
define(`B_SELF_ASSIGMENT', `inline CLASS_NAME& operator$1dnl
ifelse(`$1',`=',`',`=')(const $2 &other)
{
indir(B_PERFORM_OP_$1_`'CLASS_NAME`'_$2)dnl
return *this;
}
')dnl

dnl Expands to one comparison function for the current CLASS_NAME
dnl $1: Type of comparison
dnl $2: Type of other operand
dnl $3: Name of function
define(`B_COMPARISON_CLASS',`inline bool $3(const $2 &other) const
{
indir(B_PERFORM_OP_$1_`'CLASS_NAME`'_$2)
}')dnl

dnl Creates all self assignment operators
dnl $1: Type of other operands
dnl $2: Native cast possible
dnl $3: Macro for signature (optional)
dnl $4: List of operations (optional)
define(`B_SELF_ASSIGNMENTS', `ifelse(eval($# == 2),1,`B_SELF_ASSIGNMENTS(`$1', `$2', STANDARD_OPERATIONS)',dnl
`B_SELF_ASSIGMENT(`$3', `$1')'dnl
ifelse(eval($# > 3), 1, `B_SELF_ASSIGNMENTS(`$1', `$2', shiftn(3,$@))')dnl
)')dnl

dnl $1: Type of other operands
dnl $2: Quoted list with operators (optional)
dnl $3: Quoted list with names for member functions(optiona)
define(`B_COMPARISONS_CLASS', `ifelse($#,1,`B_COMPARISONS_CLASS(`$1', `STANDARD_COMPARISONS', `STANDARD_COMPARISONS_NAMES')',`
ifelse($#, 3, `B_COMPARISONS_CLASS(`$1', `$2', `$3', decr(listlen($2)))',`
B_COMPARISON_CLASS(at($4, $2), $1, at($4, $3))
ifelse($4, 0, `', `B_COMPARISONS_CLASS(`$1', `$2', `$3', decr($4))')')')')

dnl $1: Type of other operands
define(`B_FREE_COMPARISON_OPERATORS', `ifelse($#,1,`B_FREE_COMPARISON_OPERATORS(`$1', `STANDARD_COMPARISONS', `STANDARD_COMPARISONS_NAMES')',`
ifelse($#, 3, `B_FREE_COMPARISON_OPERATORS(`$1', `$2', `$3', decr(listlen($2)))',`
inline bool operator`'at($4,$2)`'(const CLASS_NAME`'& left, const $1& right)
{
return left.`'at($4,$3)`'(right);
}
ifelse(at($4,$2),`==',`
inline bool operator!=(const CLASS_NAME`'& left, const $1& right)
{
return !left.`'at($4,$3)`'(right);
}
')
ifelse($1, CLASS_NAME, `',`
inline bool operator`'at($4,$2)`'(const $1& left, const CLASS_NAME`'& right)
{
return right.`'at($4,$3)`'(left);
}'
ifelse(at($4,$2),`!=',`
inline bool operator!=(const CLASS_NAME`'& left, const $1& right)
{
return !left.`'at($4,$3)`'(right);
}
'))
ifelse($4, 0, `', `B_FREE_COMPARISON_OPERATORS(`$1', `$2', `$3', decr($4))')')')')

dnl Creates a free operator with CLASS_NAME as left operand
dnl $1: Type of operation
dnl $2: Other Type
define(`B_FREE_OPERATOR_L',`inline CLASS_NAME operator$1(CLASS_NAME left, const $2 &right)
{
return left $1= right;
}')dnl

dnl Creates a free operator with CLASS_NAME as right operand and a commutative operation
dnl $1: Type of operation
dnl $2: Other Type
define(`B_FREE_OPERATOR_R_COM',`inline CLASS_NAME operator$1(const $2 &left, CLASS_NAME right)
{
return right $1= left;
}')

dnl Creates a free operator with CLASS_NAME as right operand and a non commutative operation
dnl $1: Type of operation
dnl $2: Other Type
define(`B_FREE_OPERATOR_R_NOCOM',`inline CLASS_NAME operator$1(const $2 &left, const CLASS_NAME`'& right)
{
CLASS_NAME ret(left);
return ret $1= right;
}')dnl

dnl Creates free operators on basis of self assign operators
dnl $1: Other Type
define(`B_FREE_OPERATORS',`ifelse(eval($# == 1), 1, `B_FREE_OPERATORS($1, STANDARD_OPERATIONS)',
`ifelse($2,`=',`',`
B_FREE_OPERATOR_L($2, $1)
ifelse(CLASS_NAME, `$1', `', `
ifelse(eval(ifelse($2, +, 1, 0) + ifelse($2, *, 1, 0)), 1, `B_FREE_OPERATOR_R_COM($2, $1)', `B_FREE_OPERATOR_R_NOCOM($2, $1)')')')
ifelse($#, 2, dnl
, `B_FREE_OPERATORS($1, shiftn(2,$@))')')')dnl

dnl Creates friend declarations for all unary standard functions
define(`B_FRIEND_DECL', `ifelse($#,0,`B_FRIEND_DECL(UNARY_STANDARD_FUNCTIONS)',dnl
`friend CLASS_NAME $1(CLASS_NAME`');'
ifelse(eval($# > 1),1,`B_FRIEND_DECL(shift($@))'))')dnl

define(`B_UNARY_FUN_DEF',`ifelse($#, 0, `B_UNARY_FUN_DEF(UNARY_STANDARD_FUNCTIONS)',dnl
`inline CLASS_NAME $1(CLASS_NAME args)
{
indir(B_PERFORM_UNARY_FUN_$1_`'CLASS_NAME)
return args;
}
ifelse(eval($# > 1),1,`B_UNARY_FUN_DEF(shift($@))')')')dnl

dnl Defines perform macro for a copy ctor
define(`B_CREATE_COPY_CTOR',`
ifdef(B_PERFORM_COPY_CTOR_`'CLASS_NAME, `', `define(B_PERFORM_COPY_CTOR_`'CLASS_NAME, `:m_adapt(other.m_adapt) {}')')')
')dnl

dnl Defines perform macro for a constructor with a mreal argument
define(`B_CREATE_MREAL_CTOR',`ifdef(B_PERFORM_MREAL_CTOR_`'CLASS_NAME, `', `define(B_PERFORM_MREAL_CTOR_`'CLASS_NAME, `:m_adapt(r.m_adapt) {}')')')

dnl Defines perform macro for a constructor with two mreal arguments
define(`B_CREATE_MREAL_MREAL_CTOR',`ifdef(B_PERFORM_MREAL_MREAL_CTOR_`'CLASS_NAME, `', `define(B_PERFORM_MREAL_MREAL_CTOR_`'CLASS_NAME, `:m_adapt(lb.m_adapt, ub.m_adapt) {}')')')

define(`B_CREATE_DOUBLE_CTOR_MREAL',`ifdef(`B_PERFORM_DOUBLE_CTOR_MREAL', `', `define(`B_PERFORM_DOUBLE_CTOR_MREAL', `:m_adapt(d) {}')')')

define(`B_CREATE_UNARY_OPERATORS',`ifdef(B_PERFORM_UNARY_OP_MINUS_`'CLASS_NAME,`',`define(B_PERFORM_UNARY_OP_MINUS_`'CLASS_NAME, 
`CLASS_NAME tmp(arg);
tmp.m_adapt = -tmp.m_adapt;
return tmp;
')')')

define(`B_CREATE_PRINT',`ifdef(B_PERFORM_PRINT_`'CLASS_NAME,`',define(B_PERFORM_PRINT_`'CLASS_NAME,`os << m_adapt;'))')
define(`B_CREATE_DEFAULT_CTOR', `ifdef(B_PERFORM_DEFAULT_CTOR_`'CLASS_NAME,`',`define(B_PERFORM_DEFAULT_CTOR_`'CLASS_NAME,
`:m_adapt(0.0) {}')')')
