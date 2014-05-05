ARCH="-arch i386"
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
MACCXX="-mmacosx-version-min=10.6"
OSIRISCXX="-Wall -D${PLATFORM} -I/usr/include/libxslt -I/usr/include/libxml2 -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${ARCH} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

