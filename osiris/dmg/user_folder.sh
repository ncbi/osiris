#!/bin/bash
function CHECKRC()
{
  if test "$1" != "0"; then
    exit $1
  fi
}
FOLDER=~/Applications
if test ! -d "${FOLDER}" ; then
  mkdir "${FOLDER}"
  CHECKRC $?
fi
if test -d "${FOLDER}"; then
  /usr/bin/open "${FOLDER}"
fi
