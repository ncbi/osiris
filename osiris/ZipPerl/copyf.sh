#!/usr/bin/bash

DEST="/f/Profiles/OsirisDist"
BATFILE=~/Documents/digisign/sign-file.bat

function CHECKRC()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 1>&2
    fi
    exit $1
  fi
}

function DOSPATH()
{
  echo "$1" | sed 's|^/||' | sed 's/\(.\)/\1:/' | sed 's|/|\\|g' | sed 's| |\\ |g'
}

function SIGNFILE()
{
  FILE=`DOSPATH "$1"`
  if test "${BATFILEDOS}" != ""; then
    cmd.exe /c "${BATFILEDOS} ${FILE}"
  else
    echo "Cannot sign file, ${FILE}, batch script not found" 1>&2
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
SRCDIR="../Setup/Release"
if test "$1" = "-v"; then
  VERBOSE=1
else
  VERBOSE=0
fi

if test -r "$BATFILE" ; then
  BATFILEDOS=`DOSPATH "${BATFILE}"`
else
  BATFILEDOS=""
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

SIGNFILE "$EXE"
SIGNFILE "$MSI"
COPYFILE "$EXE"
COPYFILE "$MSI"





