#!/usr/bin/bash
# bash script for MinGW (using package with git-bash)
# copies file to $DEST which will need to updated for local site
DEST="//panfs/pan1/osiris/Profiles/OsirisDist"
SIGNSCRIPT=~/Documents/digisign/sign-file.sh

function CHECKRC()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 1>&2
    fi
    exit $1
  fi
}


function ECHO()
{
  if test "$VERBOSE" = "1"; then
    echo $@
  fi

}
function COPYFILE()
{
  FILE="$1"
  if test -r "${FILE}" ; then
    ECHO "${FILE}"
    cp -vup "${FILE}" "${DEST}"
    CHECKRC $?
  else
    ECHO "Cannot find file: ${DIR}/${FILE}"
  fi
}

cd `dirname $0`
if test "$1" = "-v"; then
  VERBOSE=1
else
  VERBOSE=0
fi


TMP=`ls -1tr ../Setup/Release/*.msi | tail -1`
test "$TMP" != ""
CHECKRC $? "Cannot find msi file"
MSI=`readlink -f "$TMP"`
test -r "$MSI"
CHECKRC $? "Cannot find msi file: ${MSI}"

TMP=`ls -1tr Osiris*.exe | tail -1`
test "$TMP" != ""
CHECKRC $? "Cannot find exe file"
EXE=`readlink -f "${TMP}"`
test -r "$EXE"
CHECKRC $? "Cannot find exe file: ${EXE}"

COPYFILE "$EXE"
COPYFILE "$MSI"
