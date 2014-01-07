AC_INIT(../wxXml2Document.cpp)
AM_INIT_AUTOMAKE([libwxXml2.a],1.0)
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
CXXFLAGS="-Wall -D${PLATFORM} -I/usr/include/libxml2 -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  ${WXCXX} ${MACCXX}"
AC_OUTPUT(Makefile)
