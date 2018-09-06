AC_INIT([osirisanalysis],2.5)
AC_CONFIG_SRCDIR(../mainApp.cpp)
AM_INIT_AUTOMAKE
AC_PROG_CXX
AC_PROG_INSTALL
m4_include(../../osiris.m4)
WXLD=`${WXHOME}/bin/wx-config --libs`
LDFLAGS="-L${CPPTOP}/lib -L${CPPTOP}/lib/system -L../../OsirisMath/lib -L../../nwxZip/lib -L../../nwx/lib -L../../wxXml2/lib -L../../wxXsl/lib -L../../wxthings/lib -L../../wxplotctrl/lib -lOsirisMath -lnwxZip -lnwx -lwxXsl -lwxXml2 -lwxthings -lwxplotctrl -lxslt -lexslt -lxml2 -liconv ${WXLD} ${MACLD}"
AC_OUTPUT(Makefile)
