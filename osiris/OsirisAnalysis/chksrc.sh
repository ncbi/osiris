#!/bin/sh
MK=""
FN=Makefile.am
for MKDIR in  ./lib ./bin ; do
  MKF=$MKDIR/$FN
  if test -f $MKF ; then
    MK=$MKF
  fi
done

if test "$MK" = "" ; then
echo "Cannot find $FN"
exit 1
fi

for x in *.cpp ; do
    MISS=`grep -L $x $MK`
    if test "${MISS}" != "" ; then
      echo $x
    fi
done
exit 0
