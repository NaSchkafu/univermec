#####
#
# SYNOPSIS
#
# CT_CHECK_CUDA_API
#
# DESCRIPTION
#
# This macro tries to find the headers and libraries for the
# CUDA API to build client applications.
#
# If includes are found, the variable CDINCPATH will be set. If
# libraries are found, the variable CDLIBPATH will be set. if no check
# was successful, the script exits with a error message.
#
# LAST MODIFICATION
#
# 2011-01-04
#
# COPYLEFT
#
# Copyright © 2011 <www.platos-cave.org>
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty provided
# the copyright notice and this notice are preserved.

AC_DEFUN([CT_CHECK_CUDA_API], [

AC_ARG_WITH(cuda,
[ --with-cuda=PREFIX Prefix of your CUDA installation],
[cd_prefix=$withval], [cd_prefix=""])


AC_SUBST(CDINCPATH)
AC_SUBST(CDLIBPATH)
AC_SUBST(NVCC)

AC_CANONICAL_HOST

#find out what version we are running
ARCH=`uname -m`
if [[ $ARCH == "x86_64" ]];
then
SUFFIX="64"
else
SUFFIX=""
fi


# AC_MSG_NOTICE([$cd_prefix, $withval])

# cd_prefix will be set to "yes" if --with-cuda is passed in with no value
if test "$cd_prefix" == "yes"; then
if test "$withval" == "yes"; then
cd_prefix="/usr/local/cuda"
fi
fi

if test "$cd_prefix" != ""; then

AC_MSG_CHECKING([for CUDA compiler in $cd_prefix/bin])
if test -f "$cd_prefix/bin/nvcc" ; then
NVCC="$cd_prefix/bin/nvcc"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(nvcc not found)
fi

AC_MSG_CHECKING([for CUDA includes in $cd_prefix/include])
if test -f "$cd_prefix/include/cuda.h" ; then
CDINCPATH="-I$cd_prefix/include"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(cuda.h not found)
fi

AC_MSG_CHECKING([for CUDA libraries in $cd_prefix/lib$SUFFIX])
case $host_os in
darwin*)
if test -f "$cd_prefix/lib$SUFFIX/libcudart.dylib" ; then
CDLIBPATH="-L$cd_prefix/lib$SUFFIX"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(libcublas.dylib not found)
fi
;;
linux*)
if test -f "$cd_prefix/lib$SUFFIX/libcudart.so" ; then
CDLIBPATH="-L$cd_prefix/lib$SUFFIX"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(libcudart.so not found)
fi
;;
*)
#Default Case
AC_MSG_ERROR([Your platform is not currently supported]) ;;
esac
fi

if test "$CDINCPATH" = "" ; then
AC_CHECK_HEADER([cuda.h], [], AC_MSG_ERROR(cuda.h not found))
fi
if test "$CDLIBPATH" = "" ; then
# AC_CHECK_LIB(cublas, cudaSetDevice, [], AC_MSG_ERROR(libcublas.so not found))
AC_CHECK_LIB([cudart], [cudaMalloc])
fi

])


#####
#
# SYNOPSIS
#
# CT_CHECK_CUDA_SDK
#
# DESCRIPTION
#
# This macro tries to find the headers and libraries for the
# CUDA SDK to build client applications.
#
# If includes are found, the variable CDSDKINCPATH will be set. If
# libraries are found, the variable CDSDKLIBPATH will be set. if no check
# was successful, the script exits with a error message.
#
# LAST MODIFICATION
#
# 2011-01-04
#
# COPYLEFT
#
# Copyright © 2011 <www.platos-cave.org>
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty provided
# the copyright notice and this notice are preserved.


AC_DEFUN([CT_CHECK_CUDA_SDK], [

AC_ARG_WITH([cudasdk],
[AS_HELP_STRING([--with-cudasdk],
[Prefix of your CUDA SDK installation])],
[cdsdk_prefix=$withval],
[cdsdk_prefix=yes])

AC_CANONICAL_HOST

AC_SUBST(CDSDK_COMMON_INCPATH)
AC_SUBST(CDSDK_COMMON_LIBPATH)
AC_SUBST(CDSDK_SHARED_INCPATH)
AC_SUBST(CDSDK_SHARED_LIBPATH)

# AC_MSG_NOTICE([$cdsdk_prefix, $withval])

# cd_prefix will be set to "yes" if --with-cudasdk is passed in with no value
if test "$cdsdk_prefix" == "yes"; then
if test "$withval" == "yes"; then

case $host_os in
darwin*)
cdsdk_prefix="/Developer/GPU Computing"
;;
linux*)
cdsdk_prefix="/usr/local/cuda/NVIDIA_GPU_COMPUTING_SDK"
;;
*)
#Default Case
AC_MSG_ERROR([Your platform is not currently supported]) ;;
esac

fi
fi

if test "$cdsdk_prefix" != ""; then
#########################
# common
AC_MSG_CHECKING([for CUDA SDK common includes in $cdsdk_prefix/C/common/inc])
if test -f "$cdsdk_prefix/C/common/inc/cutil.h" ; then
CDSDK_COMMON_INCPATH="-I$cdsdk_prefix/C/common/inc"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(cutil.h not found)
fi

AC_MSG_CHECKING([for CUDA SDK common libraries in $cdsdk_prefix/C/common/lib])
case $host_os in
darwin*)
if test -f "$cdsdk_prefix/C/common/lib/darwin/libcudpp_i386.a" ; then
CDSDK_COMMON_LIBPATH="-L$cdsdk_prefix/C/common/lib/ darwin/"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(libcudpp_i386.a not found)
fi
;;
linux*)
if test -f "$cdsdk_prefix/C/common/lib/linux/libnvcuvid.dylib" ; then
CDSDK_COMMON_LIBPATH="-L$cdsdk_prefix/C/common/lib/ linux/"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(libcudpp_i386.a not found)
fi
;;
*)
#Default Case
AC_MSG_ERROR([Your platform is not currently supported]) ;;
esac

#########################
# shared
AC_MSG_CHECKING([for CUDA SDK shared includes in $cdsdk_prefix/shared/inc])
if test -f "$cdsdk_prefix/shared/inc/shrUtils.h" ; then
CDSDK_SHARED_INCPATH="-I$cdsdk_prefix/shared/inc"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(shrUtils.h not found)
fi

AC_MSG_CHECKING([for CUDA SDK shared libraries in $cdsdk_prefix/shared/lib])
if test -f "$cdsdk_prefix/shared/lib/libshrutil_i386.a" ; then
CDSDK_SHARED_LIBPATH="-I$cdsdk_prefix/shared/lib"
AC_MSG_RESULT([yes])
else
AC_MSG_ERROR(libshrutil_i386.a not found)
fi
else
#Default Case
AC_MSG_ERROR([cdsdk_prefix is NULL])
fi


if test "$CDSDK_COMMON_INCPATH" = "" ; then
AC_CHECK_HEADER([cutil.h], [], AC_MSG_ERROR(cuda.h not found))
fi
if test "$CDSDK_COMMON_LIBPATH" = "" ; then
AC_CHECK_LIB(cudpp_i386, cutilCheckError, [], AC_MSG_ERROR(libcudpp_i386.a not found))
fi
if test "$CDSDK_SHARED_INCPATH" = "" ; then
AC_CHECK_HEADER([shrUtils.h], [], AC_MSG_ERROR(cuda.h not found))
fi
if test "$CDSDK_SHARED_LIBPATH" = "" ; then
AC_CHECK_LIB(shrutil_i386, __shrExitEX, [], AC_MSG_ERROR(libshrutil_i386.a not found))
fi

]) 

############################################
# NVIDIA Cuda Compiler detection and setup #
############################################
# If cuda is requested to be enabled
AC_DEFUN([CHECK_CUDA],[
AC_ARG_ENABLE(cuda, 
        AS_HELP_STRING([--enable-cuda=ARCH], [Enable cuda based modules for architecture ARCH (see nvcc option -arch).]),[

        # Search nvcc compiler
        AC_PATH_PROG(NVCC, nvcc, "no")
        AS_IF([test "x$NVCC" = "xno"],[
                        AC_MSG_ERROR([NVCC compiler not found!])
        ])

        # Check nvcc version, should be 3.0
        AS_IF([nvcc --version | grep -q "\(release 4.2\)\|\(release 5.0\)"],
                        [],
                        [AC_MSG_ERROR([NVCC compiler version is NOT 4.1!])
        ])

        # If $with_cuda is not empty, set to CUDA_ARCH to
        # supplied value, else set to value sm_11
        AS_IF([test "x$enableval" = "xyes"],[
                CUDA_ARCH=" -arch=compute_20"
		with_cuda=yes
        ],[
                CUDA_ARCH=" -arch=$enableval"
		with_cuda=yes
        ])

                # Set CUDA_CFLAGS to $NVCC, where substring "bin/nvcc"
                # is substituted by "include".
                CUDA_CFLAGS=" -I${NVCC/'bin/nvcc'/include}"

                #Set CUDA_CFLAGS to $NVCC, where substring "bin/nvcc"
                #is substituted by "lib".
                CUDA_LDFLAGS=" -L${NVCC/'bin/nvcc'/lib}"

                # If $build_cpu contains "_64", append "64" to CUDA_LIBS
        AS_IF([echo $build_cpu | grep -q "_64"],
                [CUDA_LDFLAGS+="64"])

                # Append " -lcudart" to CUDA_LIBS
                CUDA_LDADD+=" -lcudart"

        # Symbolize that cuda is wanted
	# with_cuda=$enableval

        # Make variables available in Makefile.am
        AC_SUBST(CUDA_CFLAGS)
        AC_SUBST(CUDA_LDADD)
        AC_SUBST(CUDA_LDFLAGS)
        AC_SUBST(NVCC)
])
# Set this conditional if cuda is wanted
AM_CONDITIONAL([WANT_CUDA], [test -n "$with_cuda"])

# Check whether to use device emulation mode for cuda (if no cuda capable gpu is available)
dnl AC_ARG_ENABLE([emu],
dnl         AS_HELP_STRING([--enable-emu], [Enable device emulation for cuda modules (nvcc version <= 3.0 only).]),
dnl         [EMULATION=true],
dnl         [EMULATION=false])

# Set up compilation flags for cuda compiler nvcc, if with_cuda is set
AS_IF([test -n "$with_cuda"],[

        # If debug flag is set apply debugging compilation flags, otherwise build compilation flags
        AS_IF([test "x$enable_debug" = "xyes"],
                [NVCCFLAGS="-g --compiler-options -fno-strict-aliasing --compiler-options -fno-inline"],
                [NVCCFLAGS="-O3 --compiler-options -fno-strict-aliasing --compiler-options -fno-inline"])

        # Add architecture to flags
        NVCCFLAGS+=" $CUDA_ARCH"

        # If device emulation was set, add deviceemu flag
        AS_IF([test "x$EMULATION" = "xtrue"],
                [NVCCFLAGS+=" -deviceemu"])
	AC_MSG_NOTICE([Using $NVCCFLAGS for $NVCC compiler])

      AC_DEFINE_UNQUOTED([HAVE_CUDA], 1, [Define this if you have CUDA])
])
# Make NVCCFLAGS available in Makefile.am
AC_SUBST(NVCCFLAGS)
])
