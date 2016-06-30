#!/bin/sh


function checkrc
{
  if test "$1" != "0" -a "$1" != ""; then
    if test "$2" != ""; then
      echo "" 1>&2
      echo "$2" 1>&2
    fi
    exit $1
  fi
}

function buildIt
{
DIR=`pwd`
DEBUG=`basename "${DIR}" | grep -i debug`
if test "$DEBUG" = ""; then
  PREFIX=~/local/wxRelease
  CF=""
  CPPFLAGS="-DwxDEBUG_LEVEL=0"
else
  PREFIX=~/local/wxDebug
  CF="--enable-debug --enable-debug_gdb"  
  CPPFLAGS=""
fi
if test "$XCODE" != ""; then
  XPATH=$XCODE/Contents/Developer/usr/bin
  if test -d "${XPATH}" -a -x "${XPATH}/gcc" ; then
    PATH="${XPATH}:${PATH}"
    export PATH
  fi
fi
function setupConfigure
{
  test -r ../configure
  checkrc $? "Cannot find ../configure"
  if test ! -x ../configure ; then
    chmod +x ../configure
  fi
  test -x ../configure
  checkrc $? "../configure is not executable"
  chmod +x `find .. -name \*.py`
}

mkdir -p ${PREFIX}
checkrc $?
setupConfigure

../configure ${CF} \
  --with-macosx-version-min=10.9 \
  --with-cocoa \
  --disable-shared \
  --disable-compat28 \
  --without-libjbig \
  --without-liblzma \
  --without-libtiff \
  --with-libpng \
  --disable-webviewwebkit \
  --prefix=${PREFIX}
checkrc $?
make clean
checkrc $? "Failed to clean wx"
make
checkrc $? "Failed to build wx"
cd samples
checkrc $? 
make clean
checkrc $? "Failed to clean wx samples"
make
checkrc $? "Failed to build wx samples"
cd ../demos
checkrc $?
make clean
checkrc $? "Failed to clean wx demos"
make
checkrc $? "Failed to build wx demos"
}

UNAME=`uname`
if test "$UNAME" != "Darwin"; then
  echo "This scripts is for building wxWidgets on a macintosh"
else
  cd `dirname $0`
  buildIt > build_log.txt 2>&1
fi

