#!/bin/bash


function checkRC()
{
  if test "$1" != "0"; then
    echo $2 >> "$ERRFILE"
    exit $1
  fi
}


function MAKE()
{
  cd "$1"
  checkRC $? "$1"
  echo ""
  echo "MAKE ********* $1"
  echo ""
  make
  checkRC $? "$1"
}


function BUILD
{
f=0
for x in wxthings/lib wxplotctrl/lib BaseClassLib/lib \
        OsirisLib2.01/lib OsirisMath/lib  wxXml2/lib wxXsl/lib nwx/lib \
        nwxZip/lib ; do
  MAKE $x > make${f}.txt 2>&1 &
  f=`expr $f + 1`
done
wait
if test ! -r "$ERRFILE"; then
  for x in OpenFolder/bin fsa2xml/bin OSstarter/bin TestAnalysisDirectoryLCv2.11/bin OsirisAnalysis/bin; do
    MAKE $x > make${f}.txt 2>&1 &
    f=`expr $f + 1`
  done
  wait
fi

g=0
while test "$g" != "$f"; do
  if test -r make${g}.txt; then
    cat make${g}.txt
  fi
  g=`expr $g + 1`
done

}

cd `dirname $0`
ERRFILE=`pwd`/fail.txt
export ERRFILE
#make clean
if test -r $ERRFILE; then
  rm -f $ERRFILE
  checkRC $? "cannot remove $ERRFILE"
fi
BUILD > make.txt 2>&1
test ! -r $ERRFILE
exit $?
