#!/bin/sh
ISDEBUG=`echo $0 | grep -i debug | wc -l | sed 's/ //g'`
if test "$ISDEBUG" = "0"; then
SUBDIR="osx-release"
DEBUG="--disable-debug"
else
SUBDIR="osx-debug"
DEBUG="--enable-debug"
fi

cd `dirname "$0"`
if ! test -d $SUBDIR ; then
mkdir $SUBDIR
fi
cd $SUBDIR
ARCH="-arch i386"
MAC="--with-macosx-version-min=10.5"
MACLD=""
MACCXX=""

CFLAGS="${ARCH} ${MACCXX}"
CXXFLAGS="${ARCH} ${MACCXX}"
CPPFLAGS="${ARCH} ${MACCXX}"
LDFLAGS="${ARCH} ${MACLD}"
OBJCFLAGS="${ARCH} ${MACCXX}"
OBJCXXFLAGS="${ARCH} ${MACCXX}"

export CFLAGS
export CXXFLAGS
export CPPFLAGS
export LDFLAGS
export OBJCFLAGS
export OBJCXXFLAGS

../configure  ${MAC} \
${DEBUG} \
--with-carbon \
--enable-gui \
--enable-streams \
--disable-unicode \
--disable-shared \
--enable-mdi \
--enable-xpm \
--with-libpng \
--with-libxpm \
--prefix=${HOME}/local 2>&1 | tee config-out.txt

if test "$?" = "0"; then
  make 2>&1 | tee make.txt
fi
if test "?" = "0"; then
  make samples 2>&1 | tee samples.txt
fi