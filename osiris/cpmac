#!/bin/sh
cd `dirname $0`
if test ! -d "${OSIRISTGZ}" ; then
OSIRISTGZ=`pwd`
export OSIRISTGZ
fi
echo OSIRISTGZ=${OSIRISTGZ}
cd ZipPerl
exec perl MakeZip.pl $@
