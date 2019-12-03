AC_INIT([osstarter],1.0)
AC_CONFIG_SRCDIR(../OSstarter.cpp)
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
m4_include(../../osiris.m4)
WXLD=`${WXHOME}/bin/wx-config --libs`
LDFLAGS="-L${CPPTOP}/lib -L${CPPTOP}/lib/system -L../../nwx/lib -lnwx ${WXLD} ${MACLD}"
AC_OUTPUT(Makefile)
