AC_INIT(../src/block.cpp)
AM_INIT_AUTOMAKE([libwxthings.a],1.0)
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
WXHOME=`sh ../../findwx`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
CXXFLAGS="-Wall -O3 -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../.. ${WXCXX} ${MACCXX}"
AC_OUTPUT(Makefile)
