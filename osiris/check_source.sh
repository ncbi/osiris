#!/bin/bash
#
#  script to make sure that all source files in the newest vcxproj file is in {lib,bin}/Makefile.am
#
set -o errexit

function GET_PROJ_FILE()
{
# find newest vcxproj file
 (ls -1tr | grep '\.vcxproj$' | tail -1) || true
}
function GET_MAKE_FILE()
{
  local x
  local f
  for x in lib bin; do
    f="${x}/Makefile.am"
    if test -r "${f}"; then
      echo "${f}"
      break
    fi
  done
}

MAKEFILE=${2:-$(GET_MAKE_FILE)}
PROJFILE=${1:-$(GET_PROJ_FILE)}
SRCDIR=${3:-.}
r=0
if test "$MAKEFILE" = ""; then
  echo "Cannot find Makefile.am"
  r=1
elif test ! -r "$MAKEFILE"; then
  echo "Cannot find ${MAKEFILE}"
  r=1
fi
if test "$PROJFILE" = ""; then
  echo "Cannot find vcxproj file"
  r=1
elif test ! -r "$PROJFILE"; then
  echo "Cannot find ${PROJFILE}"
  r=1
fi
test "$r" = "0"

for x in $(grep '\.cpp"' "${PROJFILE}" | sed -e 's/.*Include=.//' -e 's/".*//' -e 's/.*\\//g') ; do
  if test ! -r "${SRCDIR}/$x"; then
    echo "Cannot find $x"
  elif ! grep -l "$x" "${MAKEFILE}" > /dev/null; then
    echo "$x is not in ${MAKEFILE}"
  fi
done
