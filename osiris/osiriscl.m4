MIN_VERSION=10.9
ARCH=""
PLATFORM=`sh ../../osname`
MACCXX="-mmacosx-version-min=${MIN_VERSION}"
MACLD="${MACCXX}"
# production OPT
OPT="-O3"
# debug/test OPT
#OPT="-O0 -g"
CXXFLAGS="-Wall -Wno-parentheses -Wno-potentially-evaluated-expression -msse2 ${OPT}  -D${PLATFORM} -I../../Version -I../../OsirisMath -I../../BaseClassLib ${ARCH} ${MACCXX}"

