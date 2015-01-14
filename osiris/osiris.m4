ARCH=""
CPPTOP=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
WXCXXR="-DwxDEBUG_LEVEL=0 -fno-common -mmacosx-version-min=10.8"
MACCXX=${MACMIN}
MACLD=${MACMIN}
OSIRISCXX="-Wall -Wno-unknown-pragmas -D${PLATFORM} -I${CPPTOP}/include/libxml2  -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${WXCXXR} ${ARCH} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

