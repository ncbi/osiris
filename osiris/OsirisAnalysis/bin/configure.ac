AC_INIT(../mainApp.cpp)
AM_INIT_AUTOMAKE([osirisanalysis],1.3)
AC_PROG_CXX
AC_PROG_INSTALL
m4_include(../../osiris.m4)
WXLD=`${WXHOME}/bin/wx-config --libs`
MACLD="-isysroot /Developer/SDKs/MacOSX10.5.sdk"
LDFLAGS="-L../../nwx/lib -L../../wxXml2/lib -L../../wxXsl/lib -L../../wxthings/lib -L../../wxplotctrl/lib -lnwx -lwxXsl -lwxXml2 -lwxthings -lwxplotctrl -lxslt -lexslt -lxml2 -liconv ${WXLD} ${MACLD}"
AC_OUTPUT(Makefile)
