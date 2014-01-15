AC_INIT(../rgstring.cpp)
AM_INIT_AUTOMAKE([librgtools.a],1.0)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
m4_include(../../osiriscl.m4)
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} ${ARCH} ${MACCXX}"
CFLAGS="${CXXFLAGS}"
AC_OUTPUT(Makefile)


