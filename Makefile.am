include $(top_srcdir)/build-files/src_files.mk
include $(top_srcdir)/build-files/swg_files.mk
include $(top_srcdir)/build-files/adapter_files.mk
include $(top_srcdir)/build-files/poly_files.mk
include $(top_srcdir)/build-files/interfaces_files.mk
include $(top_srcdir)/build-files/include_files.mk

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = $(BOOST_CPPFLAGS) $(IV_CPPFLAGS)  \
	$(MTL_CPPFLAGS) $(FADBAD_CPPFLAGS) $(OPENMESH_CPPFLAGS) \
	$(TM_CPPFLAGS) $(AA_CPPFLAGS_S)  $(LOKI_CPPFLAGS) \
	-I$(top_srcdir)/src -I$(top_builddir)/.autogen/ -I$(top_srcdir)/inc

CORE_LIBS = $(IV_LIBS) $(TM_LIBS) $(OPENMESH_LIBS) $(BOOST_TIMER_LIBS) $(BOOST_SYSTEM_LIBS) $(AA_LIBS_S) $(BOOST_THREAD_LIBS) -lgomp
CORE_FLAGS = $(AA_LDFLAGS_S) $(TM_LDFLAGS) $(OPENMESH_LDFLAGS) $(BOOST_TIMER_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS) $(BOOST_THREAD_LDFLAGS) $(IV_LDFLAGS)

# Extra Dateien fuer TM
if HAVE_RIOT
SRC_FILES += $(RIOT_FILES)
endif


# Library
lib_LTLIBRARIES = libunivermec.la
libunivermec_la_SOURCES = $(SRC_FILES)
libunivermec_la_CPPFLAGS = $(AM_CPPFLAGS) $(TCPP)
libunivermec_la_CXXFLAGS = $(AM_CXXFLAGS)
# TODO: Hacky, strip leading inc from the path, use standard $(incluedir) and remove install-exec-hook
includedir = $(prefix)
nobase_include_HEADERS = $(UNIVERMEC_INCLUDES)
pkgconfiglibdir = $(libdir)
pkgconfiglib_DATA = univermec.pc

MY_LIBS =

if POLY_SUPPORT
libunivermec_la_SOURCES += $(POLY_SRC_FILES)
libunivermec_la_CPPFLAGS += -DPOLY_SUPPORT
endif
# IpOpt Interface
if IPOPT_SUPPORT
libunivermec_la_SOURCES += $(IPOPT_FILES)
AM_CPPFLAGS += $(IPOPT_CFLAGS)
CORE_LIBS += $(IPOPT_LIBS)
endif
# ValEncIA ThirdParty Code + Interface
if VALENCIA_SUPPORT
libunivermec_la_SOURCES += $(VALENCIA_FILES)
endif

if VNODELP_SUPPORT
lib_LTLIBRARIES += libvnodewrapper.la
libvnodewrapper_la_CPPFLAGS = $(AM_CPPFLAGS) $(VNODELP_CPPFLAGS)
libvnodewrapper_la_SOURCES = $(VNODELP_FILES)
CORE_LIBS += $(VNODELP_LIBS)
CORE_FLAGS += $(VNODELP_LDFLAGS)
MY_LIBS += libvnodewrapper.la
endif

MY_LIBS += libunivermec.la

if ODE_SUPPORT
# Thirdparty F77 Code in Extra Library bauen und diese spaeter dazu linken
lib_LTLIBRARIES += libode.la
libode_la_SOURCES = $(ODE_LIB_FILES)
MY_LIBS += libode.la
#libunivermec_la_SOURCES += $(ODE_FILES) $(ODE_LIB_FILES)
#CORE_LIBS += -lgfortran
endif

BUILT_SOURCES = MAKE_DIRS $(top_builddir)/.autogen/adapter.hpp $(top_builddir)/.autogen/fadbadadapt.hpp $(top_builddir)/.autogen/arithtraits.hpp

# SWIG
if SWIG_SUPPORT
SWIG_INC = $(subst -I,-I$(top_srcdir)/, $(EXTRA_INC))
SWIG_FILES = interfaces/univermecext.i
pkgpython_PYTHON = univermecext.py
pkgpyexec_LTLIBRARIES = _univermecext.la
_univermecext_la_SOURCES = .swig-obj/univermecext_wrap.cpp src/ui/scenegraph.cpp src/ui/scenegraph_util.cpp src/ui/paramtransbuilder.cpp src/ui/treebuilder.cpp
_univermecext_la_CPPFLAGS = $(AM_CPPFLAGS) $(AX_SWIG_PYTHON_CPPFLAGS) $(SWIG_INC)
_univermecext_la_LDFLAGS = -module $(CORE_FLAGS) $(BOOST_GRAPH_LDFLAGS)
_univermecext_la_LIBADD = libunivermec.la $(CORE_LIBS) $(BOOST_GRAPH_LIBS)
endif
#_unidiwok_extension_la_LDADD =  $(CORE_LIBS)
#_unidiwok_extension_la_LDADD = $(CORE_LIBS)
#libunidiwokpython_la_SOURCES = $(SWIG_BUILT) $(SWIG_SOURCES)
#libunidiwokpython_la_CPPFLAGS = $(AM_CPPFLAGS) $(AX_SWIG_PYTHON_CPPFLAGS)
#libunidiwokpython_la_LIBADD = libunidiwok.la $(CORE_LIBS)
#libunidiwokpython_la_LDFLAGS = $(CORE_FLAGS)
#libunidiwokpython_la_LDADD = $(CORE_LIBS)

#BUILT_SOURCES = $(MOC_FILES) $(UIC_FILES)


CLEANFILES = $(BUILT_SOURCES)

.PHONY: MAKE_DIRS

MAKE_DIRS:
	mkdir -p $(top_builddir)/.swig-obj $(top_builddir)/.autogen

# Python interface
$(top_builddir)/.swig-obj/univermecext_wrap.cpp: interfaces/univermecext.i $(SWIG_INTERFACES)
	$(SWIG) $(AM_CPPFLAGS) $(AX_SWIG_PYTHON_OPT) -modern -o $@ $<

# Adapter für Arithmetiken
$(top_builddir)/.autogen/adapter.hpp: $(ADAPTERS)
	@M4@ -I$(top_srcdir)/src/core/adapters -D@REAL_ADAPT@ -D@IV_ADAPT@ -D@AA_ADAPT@ -D@TM_ADAPT@ -DADAPTER_MODE $(top_srcdir)/src/core/adapters/adapters.m4 > $@

$(top_builddir)/.autogen/fadbadadapt.hpp: $(ADAPTERS)
	@M4@ -I$(top_srcdir)/src/core/adapters -D@REAL_ADAPT@ -D@IV_ADAPT@ -D@AA_ADAPT@ -D@TM_ADAPT@ -DFADBAD_MODE $(top_srcdir)/src/core/adapters/adapters.m4 > $@

$(top_builddir)/.autogen/arithtraits.hpp: $(ADAPTERS)
	@M4@ -I$(top_srcdir)/src/core/adapters -D@REAL_ADAPT@ -D@IV_ADAPT@ -D@AA_ADAPT@ -D@TM_ADAPT@ -DTRAITS_MODE $(top_srcdir)/src/core/adapters/adapters.m4 > $@

# CUDA libtool hack
.cu.lo:
	$(LIBTOOL) --tag=CC --mode=compile $(NVCC) --compile $(NVCC_FLAGS) -o $@ $^ -static
#        $(LIBTOOL) --tag=CC --mode=compile $(NVCC) --compile $(NVCC_FLAGS) -o $@ $^ -static
.cu.o:
	$(NVCC) -o $@ -c $< $(NVCCFLAGS) $(libunivermec_la_CPPFLAGS) $(YAGIL_CPPFLAGS) $(CUDA_CFLAGS)

blubb:
	echo $(TM_CPPFLAGS)
	echo $(TM_LIBS)
	echo $(RIOT_CPPFLAGS)
	echo $(prefix)
	echo $(includedir)

install-exec-hook:
	mkdir -p $(includedir)/inc
	cp $(top_builddir)/.autogen/adapter.hpp $(includedir)/inc
	cp $(top_builddir)/.autogen/fadbadadapt.hpp $(includedir)/inc
	cp $(top_builddir)/.autogen/arithtraits.hpp $(includedir)/inc
