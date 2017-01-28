AC_INIT([UniDiWok],[0.2],[stefan.kiel@posteo.de])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
m4_ifdef([AM_PROG_AR],[AM_PROG_AR])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
dnl Libtool
dnl AM_PROG_AR
LT_INIT

old_cxx_flags=$CXXFLAGS
old_cpp_flags=$CPPFLAGS
old_ld_flags=$LDFLAGS
old_libs=$LIBS
old_libdir=$libdir

dnl Compiler
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

AX_CXX_COMPILE_STDCXX(14)
CXXFLAGS="$CXXFLAGS $CXX0X_FLAGS"

dnl System Header
AC_CHECK_HEADERS([fenv.h float.h sys/time.h])
AC_CHECK_FUNCS([fesetround gettimeofday])
#AX_CXX_HEADER_STDCXX_98
#AX_CXX_HEADER_STDCXX_0X

dnl m4 Macro processor
AC_ARG_VAR([M4],[m4 macro processor command])
AC_CHECK_PROG([M4],[m4],[yes],[no])
AS_IF([test x"$M4" = x"no"],[AC_MSG_ERROR([Could not find the m4 macro processor!])],
[test x"$M4" = x"yes"],[M4=m4])

dnl pkg-config
PKG_PROG_PKG_CONFIG

dnl BOOST Header
BOOST_REQUIRE([1.49])
BOOST_FOREACH
BOOST_CONVERSION
BOOST_SMART_PTR
BOOST_STATICASSERT
BOOST_UTILITY
BOOST_ARRAY
BOOST_FIND_HEADER([boost/dynamic_bitset/dynamic_bitset.hpp])
BOOST_FIND_HEADER([boost/ptr_container/ptr_vector.hpp])
BOOST_TUPLE
BOOST_SYSTEM
BOOST_TIMER
#BOOST_GRAPH
BOOST_THREADS
dnl BOOST_STATIC

dnl echo $BOOST_CPPFLAGS
dnl echo $CPPFLAGS
dnl echo $TCPP|

dnl fadbad
SEARCH_HEADER([fadbad],[fadiff.h])

dnl MTL
SEARCH_HEADER([mtl],[boost/numeric/mtl/mtl.hpp], [$BOOST_CPPFLAGS])
AC_DEFINE([MTL_WITH_INITLIST], [], [Use C++11 style initializer lists with MTL])

dnl double als mreal base_type

dnl CXSC
dnl Optional machen

dnl Interval Libs
dnl AC_ARG_WITH([interval], [AS_HELP_STRING([--with-interval=<cxsc|filib>], [Interval library to use])])dnl
m4_define([ivlibs],[[filib], [profil_bias], [cxsc]])
m4_foreach([var], [ivlibs],[
AC_MSG_CHECKING([whether to use ] m4_translit(var, [a-z], [A-Z]))
AS_IF([test x"$with_[]var" != x -o x"$m4_translit(var, [a-z], [A-Z])[]_ROOT" != x],
[AS_IF([test x"$use_iv_lib" = x],[use_iv_lib=var
AC_MSG_RESULT([yes])], [
AC_MSG_ERROR([More than one interval library specified!])])],
[AC_MSG_RESULT([no])])])
AS_CASE([$use_iv_lib],[filib],[
dnl filib++
SEARCH_HEADER([filib],[interval/interval.hpp], [], [filib])
SEARCH_LIBRARY([filib],[filib::fp_traits<double>::setup();],[],[],[#include <interval/interval.hpp>
],[prim])
IV_CPPFLAGS=$FILIB_CPPFLAGS
IV_LIBS=$FILIB_LIBS
IV_LDFLAGS=$FILIB_LDFLAGS
IV_ADAPT=IV_FILIB
],[profil_bias],[
dnl PROFILE/BIAS
SEARCH_HEADER([profil_bias],[Interval.h], [], [])
SEARCH_LIBRARY([profil_bias],[Sin(INTERVAL(2.0));], [-lProfil -lBias -lm], [], [#include <Interval.h>
#include <Functions.h>],[lr])
PROFIL_BIAS_LIBS="-lProfil -lBias $PROFIL_BIAS_LIBS"
IV_CPPFLAGS=$PROFIL_BIAS_CPPFLAGS
IV_LIBS=$PROFIL_BIAS_LIBS
IV_LDFLAGS=$PROFIL_BIAS_LDFLAGS
IV_ADAPT=IV_PROFIL
AC_MSG_WARN([The UniVerMeC Version of ValEncIA is not compatible with PROFIL/BIAS. --disable-valencia is enforced!])
enable_valencia=no
],[
dnl cxsc
AS_IF([test x"$use_iv_lib" = x],[AC_MSG_NOTICE([No interval library specified. Assuming --with-cxsc=yes])
	    with_cxsc=yes; use_iv_lib=cxsc
])
CXSC_REQUIRE([2],[2],[4])
CXSC_FIND_LIB
IV_CPPFLAGS=$CXSC_CPPFLAGS
IV_LIBS=$CXSC_LIBS
IV_LDFLAGS=$CXSC_LDFLAGS
IV_ADAPT=IV_CXSC
])
m4_foreach([var],[ivlibs],[
	AM_CONDITIONAL(m4_translit(var, [a-z], [A-Z]),[test x"$use_iv_lib" = x"var"])
])
AC_SUBST([IV_LIBS], [$IV_LIBS])
AC_SUBST([IV_CPPFLAGS], [$IV_CPPFLAGS])
AC_SUBST([IV_LDFLAGS], [$IV_LDFLAGS])

dnl TM
AC_ARG_WITH([riotng], [AS_HELP_STRING([--with-riotng=<yes>], [Use RiOT-NG for Taylor models (uses pkg-config)])])dnl
m4_define([tmlibs], [[riot],[riotng]])
m4_foreach([var], [tmlibs],[AS_IF([test x"$with_[]var" != x -o x"$m4_translit(var, [a-z], [A-Z])[]_ROOT" != x],[use_tm_lib=var])
])
AS_CASE([$use_tm_lib],[riot], [
dnl RiOT
dnl Legacy Code, nur supported mit C-XSC
AS_IF([test x"$with_cxsc" = x], [AC_MSG_ERROR([RiOT library requested, but compiled without C-XSC support. The current RiOT
	    support is restricted to C-XSC and marked as obsolete. Consider switching to RiOT-NG])])
AC_MSG_WARN([RiOT support is obsolete! Consider swichting to RiOT-NG])
SEARCH_HEADER([riot],[taylormodel.h],[$IV_CPPFLAGS -DCXSC_VERSION=YES],[tm-base])
SEARCH_LIBRARY([riot],[riot::TaylorModel::ZERO_TM()],
	[$IV_CPPFLAGS $IV_LIBS -DCXSC_VERSION=YES $RIOT_CPPFLAGS],
	[tm-base],[#include <taylormodel.h>
],[tm])
TM_CPPFLAGS="$RIOT_CPPFLAGS -DCXSC_VERSION=YES"
TM_LIBS="$RIOT_LIBS $RIOT_LDFLAGS"
TM_ADAPT=TM_RIOT
],[
dnl Default = RiOT NG
PKG_CHECK_MODULES([RIOTNG], [libtm >= 0.1], [
TM_LIBS="$RIOTNG_LIBS"
TM_CPPFLAGS="$RIOTNG_CFLAGS"
TM_ADAPT=TM_RIOT
])])
AC_SUBST([TM_LIBS], [$TM_LIBS])
AC_SUBST([TM_CPPFLAGS], [$TM_CPPFLAGS])
AM_CONDITIONAL([HAVE_RIOT], [test x"$TM_ADAPT" = xTM_RIOT])

dnl affa
dnl Anpassen für andere Intervallbibliotheken...
dnl Interval Libs
dnl AC_ARG_WITH([interval], [AS_HELP_STRING([--with-interval=<cxsc|filib>], [Interval library to use])])dnl
m4_define([aalibs],[[affa], [yalaa]])
m4_foreach([var], [aalibs],[AS_IF([test x"$with_[]var" != x -o x"$m4_translit(var, [a-z], [A-Z])[]_ROOT" != x],[use_aa_lib=var])
])
AS_CASE([$use_aa_lib],[affa],[
dnl affaa
dnl TODO: funktioniert nur mit C-XSC
AS_IF([test x"$with_cxsc" = x], [AC_MSG_ERROR([affa library requested, but compiled without C-XSC support. The current affa
	    version only works with C-XSC])])
SEARCH_HEADER([affa],[aa.h],[$CXSC_CPPFLAGS]) 
SEARCH_LIBRARY([affa],[AAF k(cxsc::interval(0,0))],
	[$AFFA_CPPFLAGS $CXSC_LIBS $CXSC_CPPFLAGS],,[#include <aa.h>])
AA_CPPFLAGS=$AFFA_CPPFLAGS
AA_LDFLAGS=$AFFA_LDFLAGS
AA_LIBS=$AFFA_LIBS
AA_ADAPT=AA_AFFA
],[
dnl cxsc
AS_IF([test x"$use_aa_lib" = x],[AC_MSG_NOTICE([No affine library specified. Assuming --with-yalaa=yes])
	    with_yalaa=yes; use_yalaa_lib=yalaa
])
SEARCH_HEADER([yalaa],[yalaa.hpp],[$IV_CPPFLAGS $BOOST_CPPFLAGS], [include/src]) 
SEARCH_LIBRARY([yalaa],[yalaa::aff_e_d(0.0)],
	[$YALAA_CPPFLAGS $IV_LIBS $IV_CPPFLAGS $IV_LDFLAGS],,[#include <yalaa.hpp>])
AA_CPPFLAGS=$YALAA_CPPFLAGS
AA_LDFLAGS=$YALAA_LDFLAGS
AA_LIBS=$YALAA_LIBS
AA_ADAPT=AA_YALAA
])
m4_foreach([var],[aalibs],[
	AM_CONDITIONAL(m4_translit(var, [a-z], [A-Z]),[test x"$use_aa_lib" = x"var"])
])
AC_SUBST([AA_CPPFLAGS_S],[$AA_CPPFLAGS])
AC_SUBST([AA_LDFLAGS_S],[$AA_LDFLAGS])
AC_SUBST([AA_LIBS_S],[$AA_LIBS])

dnl OpenMesh
AS_IF([test x"$with_openmesh" = xno], [AC_MSG_NOTICE([Compile without OpenMesh requested. Polyhedron support has been disabled!])],
[
SEARCH_HEADER([openmesh],[OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh])
SEARCH_LIBRARY([openmesh],[typedef OpenMesh::VectorT<int, 3> T],
	[$OPENMESH_CPPFLAGS],[lib/OpenMesh],
	[#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>],
	[OpenMeshCore])
])
AM_CONDITIONAL([POLY_SUPPORT], [test x"$with_openmesh" != xno])

dnl LOKI
dnl SEARCH_HEADER([loki],[loki/Singleton.h])
dnl SEARCH_LIBRARY([loki],[typedef Loki::SingletonHolder<K> G;],[$LOKI_CPPFLAGS],[], [ #include <loki/Singleton.h>
dnl	       class K {}; ], [loki])


dnl ****************************************************************
dnl IpOpt Support
dnl ****************************************************************
dnl Fortran
AC_PROG_F77
dnl LAPACK
AX_LAPACK([], [AC_MSG_WARN([LAPACK not available, therefore --with-ipopt=no is enforced!])
	    with_ipopt=no
	    AC_MSG_WARN([LAPACK not available, therefore --with-vnodelp=no is enforced!])
	    with_vnodelp=no
])
dnl BLAS
AX_BLAS([], [AC_MSG_WARN([BLAS not available, therefore --with-ipopt=no is enforced!])
	    with_ipopt=no
	    AC_MSG_WARN([BLAS not available, therefore --with-vnodelp=no is enforced!])
	    with_vnodelp=no
])
dnl IpOpt
AC_ARG_ENABLE([ipopt],AS_HELP_STRING([--enable-ipopt],[enables IpOpt support]), [enable_ipopt="$enableval"], [enable_ipopt=no])
AC_MSG_RESULT([$enable_ipopt])
AS_IF([test x"$enable_ipopt" = xno], [AC_MSG_NOTICE([Compile without IpOpt support requested. IpOpt interface has been disabled!])],
[
dnl SEARCH_HEADER([ipopt], [coin/IpIpoptApplication.hpp])
PKG_CHECK_MODULES([IPOPT], [ipopt >= 3])
AC_DEFINE_UNQUOTED([IPOPT], 1, [Define this if you have the IPOPT header])
dnl SEARCH_LIBRARY([ipopt], [Ipopt::IpoptApplication opt;], [$LAPACK_LIBS $BLAS_LIBS $IPOPT_CPPFLAGS $FLIBS], [], [#include <coin/IpIpoptApplication.hpp>])
])
AM_CONDITIONAL([IPOPT_SUPPORT], [test x"$enable_ipopt" != xno])
dnl ****************************************************************

dnl dnl ****************************************************************
dnl dnl ValEncIA-IVP
dnl dnl ****************************************************************
AC_MSG_CHECKING([whether to build with ValEncIA-IVP code])
AC_ARG_ENABLE([valencia],
    AS_HELP_STRING([--enable-valencia], [Compile without ValEncIA-IVP code]), [enable_valencia="$enableval"], [enable_valencia=no])
AC_MSG_RESULT([$enable_valencia])
AM_CONDITIONAL([VALENCIA_SUPPORT], [test x"$enable_valencia" != xno])
AS_IF([test x"$enable_valencia" != xno], [
AC_DEFINE([HAVE_VALENCIA],1,[Define this if you have ValEncIA-IVP])])

dnl dnl ****************************************************************

dnl dnl ****************************************************************
dnl dnl ODE-IVP
dnl dnl ****************************************************************
AS_IF([test x"$F77" = x], [AC_MSG_WARN([No F77 compiler found, therefore --disable-ode is enforced!])
$enable_ode=no])

AC_MSG_CHECKING([whether to build with ODE code])
AC_ARG_ENABLE([ode],
    AS_HELP_STRING([--enable-ode], [Compile without ODE code]), [enable_ode="$enableval"], [enable_ode=no])
AC_MSG_RESULT([$enable_ode])
AM_CONDITIONAL([ODE_SUPPORT], [test x"$enable_ode" != xno])
ODE_LIBS="$BLAS_LIBS"
AC_SUBST([BLAS_LIBS],[$BLAS_LIBS])

dnl dnl ****************************************************************

dnl ****************************************************************
dnl VNODE-LP
dnl ****************************************************************
AS_IF([test x"$with_vnodelp" = xno], [AC_MSG_NOTICE([Compile without VNODE-LP support requested. VNODE-LP interface has been disabled!])],
[
dnl Workaround um das VNODE interne FADBAD++ zu finden was wir eigentlich nicht brauchen aber in vnode.h eingebunden wird :(
AS_IF([test x"$VNODELP_ROOT" != "x"], [AS_IF([test x"$with_vnodelp" = "x"], [
with_vnodelp=$VNODELP_ROOT])])
VNODELPFADBAD_ROOT=$with_vnodelp
SEARCH_HEADER([vnodelpfadbad], [ffadiff.h],[], [FADBAD++])
CPPFLAGS=$old_cpp_flags
AC_ARG_VAR([USER_VNODELP_CXXFLAGS], [CXXFLAGS used to compile VNODELP])
AC_ARG_VAR([USER_VNODELP_LDFLAGS], [LDFLAGS used to link VNODELP])
AC_ARG_VAR([USER_VNODELP_LIBS], [LIBS used to link VNODELP])
dnl VNODE-LP suchen, der Benutzer muss helfen indem er die Konfiguration von VNODE-LP übergibt
SEARCH_HEADER([vnodelp], [vnode.h], [$USER_VNODELP_CXXFLAGS $VNODELPFADBAD_CPPFLAGS])
VNODELP_CPPFLAGS="$VNODELP_CPPFLAGS $USER_VNODELP_CXXFLAGS $VNODELPFADBAD_CPPFLAGS"
SEARCH_LIBRARY([vnodelp], [VNODE *s = new VNODE((AD*)0);], [$VNODELP_CPPFLAGS $USER_VNODELP_LDFLAGS $USER_VNODELP_LIBS $IV_CPP_FLAGS $IV_LIBS $LAPACK_LIBS $BLAS_LIBS ], [], [#include <vnode.h>
using namespace vnodelp;], [vnode])
VNODELP_LDFLAGS="$VNODELP_LDFLAGS $USER_VNODELP_LDFLAGS"
VNODELP_LIBS="$VNODELP_LIBS $USER_VNODELP_LIBS $LAPACK_LIBS $BLAS_LIBS $FLIBS"
])
AM_CONDITIONAL([VNODELP_SUPPORT], [test x"$with_vnodelp" != xno])

dnl Debugging
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],AS_HELP_STRING([--enable-debug],[enables debugging built]), [enable_debug="$enableval"], [enable_debug=no])
AC_MSG_RESULT([$enable_debug])
AS_IF([test x"$enable_debug" = "xyes"], [AC_DEFINE([DEBUG], [], [Debugging built])
	    DEBUG_FLAGS="-g -O0" 
	    QT_FLAGS+=debug],[AC_DEFINE([NDEBUG], [], [No debugging built])
	    DEBUG_FLAGS=-O3])
AC_SUBST([DEBUG_FLAGS])

dnl CUDA
CHECK_CUDA
AS_IF([test x"$with_cuda" = "xyes"],[
dnl SEARCH_HEADER([yagil],[yagil_test.hpp])])
AM_CONDITIONAL([CUDA_SUPPORT], [test x"$with_cuda" = xyes])
#CT_CHECK_CUDA_API
#CT_CHECK_CUDA_SDK


dnl SWIG
dnl FIX: AM_PATH_PYTHON wird ausgefuehrt wenn SWIG deaktiviert ist
AC_ARG_ENABLE([swig],AS_HELP_STRING([--enable-swig],[enables SWIG python interface]), [enable_swig="$enableval"], [enable_swig=no])
AC_DEFUN([CHECK_SWIG],[
AC_MSG_CHECKING([whether to build with the SWIG python interface])
AS_IF([test x"$enable_swig" != xno],[
AC_MSG_RESULT([yes])
AM_PATH_PYTHON
AX_PKG_SWIG(2.0.1, [], [ AC_MSG_ERROR([SWIG is required to build.]) ])
AX_SWIG_ENABLE_CXX
AX_SWIG_MULTI_MODULE_SUPPORT
AX_SWIG_PYTHON
],[
AC_MSG_RESULT([no])])])
CHECK_SWIG
AM_CONDITIONAL([SWIG_SUPPORT], [test x"$enable_swig" != xno])

dnl .so for SWIG interface
dnl LT_INIT([disable-shared])
dnl AC_DISABLE_STATIC

AC_OPENMP
# -Wno-unused-local-typedefs Deaktivieren einer nervigen Warnmeldungen in verschiedenen Bibs mit gcc 4.8
CXXFLAGS="$CXX0X_FLAGS $old_cxx_flags $DEBUG_FLAGS -Wno-unused-local-typedefs -DBOOST_SPIRIT_THREADSAFE -Wno-deprecated-declarations $OPENMP_CXXFLAGS"
CPPFLAGS="$old_cpp_flags"

dnl echo $AA_LIBS
dnl echo $AA_LDFLAGS

dnl Arithmetic adapters
AC_SUBST([REAL_ADAPT],[REAL_DOUBLE])
AC_SUBST([IV_ADAPT],[$IV_ADAPT])
AC_SUBST([TM_ADAPT],[$TM_ADAPT])
AC_SUBST([AA_ADAPT],[$AA_ADAPT])

dnl echo $IPOPT_CFLAGS
dnl echo $IPOPT_LDFLAGS
dnl echo $IPOPT_LIBS

libdir=$old_libdir

AC_CONFIG_FILES([univermec.pc Makefile])
AC_OUTPUT
