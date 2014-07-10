ARCH=""
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
MACCXX="-mmacosx-version-min=10.8"
OSIRISCXX="-Wall -D${PLATFORM} -I/usr/include/libxslt -I/usr/include/libxml2 -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${ARCH} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

