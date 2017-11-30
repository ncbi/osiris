#!/usr/bin/bash

DEST="/f/Profiles/OsirisDist"
function ECHO()
{
  if test "$VERBOSE" = "1"; then
    echo $@
  fi

}
function copyfile()
{
  FILE="$2"
  DIR="$1"
  if test "$FILE" = ""; then
    ECHO "No file in ${DIR}"
  elif test -r "${DIR}/${FILE}" ; then
    ECHO "${DIR}/${FILE}"
    cp -vup "${DIR}/${FILE}" "${DEST}"
    chmod g+r "${DEST}/${FILE}"
  else
    ECHO "Cannot find file: ${DIR}/${FILE}"
  fi
}
function findfile()
{
  DIR="$1"
  EXT="$2"
  FILE=`ls -1tr "${DIR}" | grep '\.'"${2}"'$' | tail -1`
  copyfile "${DIR}" "${FILE}"
}
cd `dirname $0`
SRCDIR="../Setup/Release"
if test "$1" = "-v"; then
  VERBOSE=1
else
  VERBOSE=0
fi

findfile "../Setup/Release" 'msi'
findfile "." 'zip'


