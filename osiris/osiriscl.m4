MIN_VERSION=10.9
ARCH=""
PLATFORM=`sh ../../osname`
MACCXX="-mmacosx-version-min=${MIN_VERSION}"
MACLD="${MACCXX}"
CXXFLAGS="-Wall -Wnoparentheses -Wnopotentially-evaluated-expression -msse2 -O3  -D${PLATFORM} -I../../Version -I../../BaseClassLib ${ARCH} ${MACCXX}"

