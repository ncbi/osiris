#!/bin/bash
#
#  check vcxproj files for TMP_DEBUG when not a beta
#

cd $(dirname $(readlink -f "$0"))
R=0
if ! (grep -i beta ./Version/OsirisVersion.h > /dev/null); then
  for file in $(find . -name "*.vcxproj" -print); do
    S=$(grep "TMP_DEBUG" ${file} | grep -v WXDEBUG)
    if test "$S" != ""; then
      echo "${file} has TMP_DEBUG"
      R=1
    fi
  done
fi
exit $R
