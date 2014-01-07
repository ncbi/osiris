AC_INIT(../mainApp.cpp)
AM_INIT_AUTOMAKE([osirisanalysis],1.3)
AC_PROG_CXX
AC_PROG_INSTALL
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
WXLD=`${WXHOME}/bin/wx-config --libs`
MACCXX="-isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.5"
MACLD="-isysroot /Developer/SDKs/MacOSX10.5.sdk"
CXXFLAGS="-Wall -O3 -D${PLATFORM}  -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../.. -I/usr/include/libxml2 -I/usr/include/libxslt -I../../Version ${WXCXX} ${MACCXX}"
LDFLAGS="-L../../nwx/lib -L../../wxXml2/lib -L../../wxXsl/lib -L../../wxthings/lib -L../../wxplotctrl/lib -lnwx -lwxXsl -lwxXml2 -lwxthings -lwxplotctrl -lxslt -lexslt -lxml2 -liconv ${WXLD} ${MACLD}"
AC_OUTPUT(Makefile)
