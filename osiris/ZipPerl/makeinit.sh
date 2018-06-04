#!/bin/bash
function CHECKRC()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 2>&1
    fi
    exit $1
  fi
}

function usage()
{

SCRIPT=$(basename $0)
if test "$SCRIPT" = "makedmg.sh"; then
FTYPE=dmg
VOLUME="The volume name is Osiris-<version>"
else
FTYPE=tar.gz
VOLUME=""
fi
cat <<EOF
Usage: ${SCRIPT} <folder-path> <version>
  folder-path - the folder containing the contents of the ${FTYPE} file
  version - the OSIRIS version

The ${FTYPE} file will be Osiris-Mac-<version>.${FTYPE}
The location is the parent folder of <folder-path>
${VOLUME}

EOF
}

function SETUP()
{

if test "$2" = ""; then
  usage $0
  exit
fi

FOLDER=$(echo "$1" |sed 's|/*$||')
VERSION="$2"
FILENAME="Osiris-Mac-${2}"
test -d "$FOLDER"
CHECKRC $? "folder, ${FOLDER}, does not exist"
PARENT=$(dirname "$FOLDER")
test -d "$PARENT"
CHECKRC $? "folder, ${PARENT} does not exist"

}

function CP()
{
  BASE=$(basename "$1")
  if test -r "$1" -a -d "$ODIST"; then
    cp -pv "$1" "${ODIST}/${BASE}"
    CHECKRC $?
    chmod g+w "${ODIST}/${BASE}"
    CHECKRC $?
  fi
}

function MAKE_DMG()
{
SETUP "$1" "$2"
DEST1="${PARENT}/tmp.dmg"
DEST2="${PARENT}/${FILENAME}.dmg"
VOLUME="Osiris-${VERSION}"
/usr/bin/hdiutil  create "${DEST1}" -ov -volname "${VOLUME}" -fs HFS+ -srcfolder "${FOLDER}/"
CHECKRC $?
if test -r "${DEST2}"; then
  rm -f "${DEST2}"
  CHECKRC $?
fi
/usr/bin/hdiutil convert "${DEST1}" -format UDZO -o "${DEST2}"
CHECKRC $?
CP "${DEST2}"
#rm "${DEST1}"
}

function MAKE_TAR_GZ()
{
SETUP "$1" "$2"
cd "${FOLDER}"
CHECKRC $?
FULLNAME="${PARENT}/${FILENAME}.tar.gz"
tar zcf "${FULLNAME}" *
CHECKRC $?
CP "${FULLNAME}"
}
