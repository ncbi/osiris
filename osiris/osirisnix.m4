MIN_VERSION=10.12
ARCH=""
PLATFORM=
MACCXX=
MACLD=
# production OPT
OPT="-O3"
# debug/test OPT
#OPT="-O0 -g"
CXXFLAGS="-Wall -Wno-parentheses -Wno-potentially-evaluated-expression -msse2 ${OPT}   -I../../Version -I../../OsirisMath -I../../BaseClassLib ${ARCH} ${MACCXX}"


