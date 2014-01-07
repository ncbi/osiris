AC_INIT(../rgstring.cpp)
AM_INIT_AUTOMAKE([librgtools.a],1.0)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
PLATFORM=`sh ../../osname`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} ${MACCXX}"
CFLAGS="-Wall -msse2 -O3  -D${PLATFORM}"
AC_OUTPUT(Makefile)


