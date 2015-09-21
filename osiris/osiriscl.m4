ARCH=""
PLATFORM=`sh ../../osname`
MACCXX="-mmacosx-version-min=10.9"
MACLD="${MACCXX}"
CXXFLAGS="-Wall -msse2 -O3  -D${PLATFORM} -I../../Version -I../../BaseClassLib ${ARCH} ${MACCXX}"

