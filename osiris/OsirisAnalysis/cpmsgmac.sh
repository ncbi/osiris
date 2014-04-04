#!/bin/sh
#
# shell to update OSIRIS message book in user defined
# 
#
function testRC
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 1>&2
    fi
    exit $1
  fi
}

UNAME=`uname`
if test "$UNAME" != "Darwin"; then
  echo "This script is designed for the Macintosh environment"
  exit 1
fi

cd `dirname $0`
OKDIR=`pwd | grep '\.app/Contents/MacOS$'`
if test "$OKDIR" != ""; then
  true
elif test -d ./Osiris.app/Contents/MacOS; then
  cd ./Osiris.app/Contents/MacOS
  OKDIR=`pwd | grep '\.app/Contents/MacOS$'`
fi
test "$OKDIR" != ""
testRC $? "This script needs to be in the MacOS directory the directory containing Osiris.app"
SOURCEFILE="./Config/Volumes/ID/ID_MessageBookV4.0.xml"
test -f "$SOURCEFILE"
testRC $? "Cannot find message book in OSIRIS installation; ${SOURCEFILE}"

ISAPP=`pwd | grep "^/Applications/"`
DESTSUBDIR="Osiris-Files/Volumes"
if test "$ISAPP" = ""; then
  DESTDIR="../../../${DESTSUBDIR}"
else
  DESTDIR="/Library/${DESTSUBDIR}"
fi
test -d "${DESTDIR}"
testRC $? "Cannot find operating procedure directory, ${DESTDIR}"
for x in ${DESTDIR}/*; do
  BASE=`basename $x`
  DESTFILE="${DESTDIR}/${BASE}/${BASE}_MessageBookV4.0.xml"
	if test -r "${DESTFILE}"; then
    diff -q "${SOURCEFILE}" "${DESTFILE}" >/dev/null
    if test "$?" != "0"; then
      mv "${DESTFILE}" "${DESTFILE}.${$}.backup"
      testRC $? "Cannot backup ${DESTFILE}"
      cp -vp "${SOURCEFILE}" "${DESTFILE}"
      testRC $?
    fi
  fi
done


