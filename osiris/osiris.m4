MIN_VERSION=10.12
SDK_VERSION=
VMIN="-mmacosx-version-min=${MIN_VERSION}"
ARCH=""
CPPTOP=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${SDK_VERSION}.sdk/usr
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
# production OPT
OPT="-O3"
# debug/test OPT
#OPT="-g"

WXCXXR="-DwxDEBUG_LEVEL=0 -fno-common ${OPT} -fno-strict-aliasing -fno-common -D_WCHAR_H_CPLUSPLUS_98_CONFORMANCE_ ${VMIN}"
MACLD="${VMIN}"
OSIRISCXX="-Wall -Wno-potentially-evaluated-expression -Wno-unknown-pragmas -D${PLATFORM} -I${CPPTOP}/include/libxml2  -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${WXCXXR} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

