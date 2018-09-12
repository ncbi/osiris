MIN_VERSION=10.9
ARCH=""
PLATFORM=`sh ../../osname`
MACCXX="-mmacosx-version-min=${MIN_VERSION}"
MACLD="${MACCXX}"
CXXFLAGS="-Wall -Wno-parentheses -Wno-potentially-evaluated-expression -msse2 -O3  -D${PLATFORM} -I../../Version -I../../OsirisMath -I../../BaseClassLib ${ARCH} ${MACCXX}"

