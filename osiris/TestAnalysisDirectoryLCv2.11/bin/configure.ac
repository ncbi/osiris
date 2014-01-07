AC_INIT(../TestAnalysisDirectoryLC.cpp)
AM_INIT_AUTOMAKE([TestAnalysisDirectoryLC],1.0)
AC_PROG_CXX
AC_PROG_INSTALL
PLATFORM=`sh ../../osname`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
MACLD="-isysroot /Developer/SDKs/MacOSX10.5.sdk"
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} ${MACCXX}"
LDFLAGS="${MACLD}"
AC_OUTPUT(Makefile)
