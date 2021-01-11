PLATFORM=`uname`
# production OPT
OPT="-O3"
# debug/test OPT
#OPT="-O0 -g"
CXXFLAGS="-Wall -Wno-parentheses -msse2 ${OPT}  -D${PLATFORM} -I../../Version -I../../OsirisMath -I../../BaseClassLib ${ARCH} ${MACCXX}"


