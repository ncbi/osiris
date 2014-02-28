ARCH="-arch i386"
PLATFORM=`sh ../../osname`
MACCXX="-mmacosx-version-min=10.5"
MACLD="${MACCXX}"
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} -I../../Version -I../../BaseClassLib ${ARCH} ${MACCXX}"

