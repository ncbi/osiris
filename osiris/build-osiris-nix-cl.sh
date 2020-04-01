#!/bin/bash
set -o errexit

DATE=$(date +%Y%m%d_%H%H%S)
function SETUP_LINUX()
{
  cp -pv configure-cl.ac configure.ac
  cp -pv Makefile-cl.am Makefile.am
  cp -pv osirisnix.m4 osiriscl.m4
}
function RESTORE()
{
  cp -pv configure.ac.mac configure.ac
  cp -pv Makefile.am.mac Makefile.am
  cp -pv osiriscl.m4.mac osiriscl.m4
}

cd $(dirname "$0")
for x in BaseClassLib/lib OsirisMath/lib OsirisLib2.01/lib TestAnalysisDirectoryLCv2.11/bin fsa2xml/bin; do
(cd $x ; pwd ; aclocal; autoconf; automake -a)
done
(cd OsirisLib2.01 ; sh blex.sh clean all )

SETUP_LINUX
aclocal
autoconf
automake -a
RESTORE
echo -n "Press <Enter> to continue... "
read x
./configure

