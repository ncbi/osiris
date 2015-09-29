ARCH=""
#CPPTOP=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr
CPPTOP=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
# on OSX 10.9 for 10.8
#WXCXXR="-DwxDEBUG_LEVEL=0 -fno-common -mmacosx-version-min=10.8"
# on OSX 10.10 for 10.9
WXCXXR="-O2 -fno-strict-aliasing -fno-common -mmacosx-version-min=10.8 -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_"
MACCXX=${MACMIN}
MACLD=${MACMIN}
OSIRISCXX="-Wall -Wno-unknown-pragmas -D${PLATFORM} -I${CPPTOP}/include/libxml2  -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${WXCXXR} ${ARCH} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

