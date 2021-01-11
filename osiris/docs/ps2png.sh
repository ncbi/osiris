#!/bin/bash

R=1
if test "$1" = ""; then
  echo "input file not specified"
elif test ! -r "$1" -o ! -f "$1"; then
  echo "cannot find file $1"
else
  OF=$(echo "$1" | sed -e 's/\.pdf$//')".png"
  echo "Writing ${OF}"
  echo "" | gs -quiet -sDEVICE=png16m -r600 -sOutputFile="${OF}" "$1"
  R=$?
  echo ""
fi
test "$R" = "0" || false

