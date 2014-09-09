ARCH=""
CPPTOP=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr
PLATFORM=`sh ../../osname`
WXHOME=`sh ../../findwx`
WXCXX=`${WXHOME}/bin/wx-config --cxxflags`
MACMIN="-mmacosx-version-min=10.8"
MACCXX=${MACMIN}
MACLD=${MACMIN}
OSIRISCXX="-Wall -D${PLATFORM} -I${CPPTOP}/include/libxml2  -I../../wxthings/include -I../../wxplotctrl/include -I.. -I../..  -I../../Version ${WXCXX} ${ARCH} ${MACCXX}"
CXXFLAGS="${OSIRISCXX}"
CFLAGS="${CXXFLAGS}"

