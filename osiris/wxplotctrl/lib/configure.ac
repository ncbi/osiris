AC_INIT(../src/plotctrl.cpp)
AM_INIT_AUTOMAKE([libwxplotctrl.a],1.0)
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
CXXFLAGS="-Wall -O3 -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../.. ${WXCXX} ${MACCXX}"
CFLAGS="${CXXFLAGS}"
AC_OUTPUT(Makefile)
