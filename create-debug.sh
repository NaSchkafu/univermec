#!/bin/sh

#export AFFA_ROOT=$HOME/libs/affa
export YALAA_ROOT=$HOME/libs/yalaa/
export CXSC_ROOT=$HOME/libs/cxsc
export MTL_ROOT=$HOME/libs/mtl
export FADBAD_ROOT=$HOME/libs/fadbad
export RIOT_ROOT=$HOME/libs/riot/include/tm-base
export OPENMESH_ROOT=$HOME/libs/openmesh/
export VISLIB_ROOT=$HOME/libs/vl
export BOOST_ROOT=$HOME/libs/boost
export LOKI_ROOT=$HOME/libs/loki
export YAGIL_ROOT=$HOME/libs/yagil
#export QWT_ROOT=$HOME/libs/qwt
#export IPOPT_ROOT=$HOME/libs/ipopt
export PKG_CONFIG_PATH=$HOME/libs/ipopt/lib/pkgconfig

#export VNODELPFADBAD_ROOT=$HOME/libs/vnodelp
# VNODE-LP
# Derzeit muss die VNODE-LP Konfiguration manuell uebergeben werden
# Includepfad zur IA Lib von VNODE etc
export USER_VNODELP_CXXFLAGS="-I$HOME/libs/profil/include -I$HOME/libs/profil/include/BIAS -I$HOME/libs/profil/src/Base -DPROFIL_VNODE -DMAXORDER=50" 
# Libpfad zur IA Lib von VNODE
export USER_VNODELP_LDFLAGS="-L$HOME/libs/profil/lib" 
# Linker Flags fuer VNODE IA Lib
export USER_VNODELP_LIBS="-lProfil -lBias -llr" 
export VNODELP_ROOT=$HOME/libs/vnodelp




../configure --enable-debug --enable-ipopt --disable-shared --with-openmesh=no --with-gui=vericell --with-riot-ldpath=/home/fluxy/libs/riot/lib --prefix=$HOME/builds/univermec
