AC_INIT(../SampleData.cpp)
AM_INIT_AUTOMAKE([libosiris.a],1.0)
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
PLATFORM=`sh ../../osname`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} -I../../Version -I../../BaseClassLib ${MACCXX}"
AC_OUTPUT(Makefile)

