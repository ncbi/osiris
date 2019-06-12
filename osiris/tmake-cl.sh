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
  echo "MAKE ********* $@"
  echo ""
  shift
  make "$@"
  checkRC $? "$1"
}


function BUILD
{
f=0
for x in BaseClassLib/lib  OsirisLib2.01/lib OsirisMath/lib  ; do
  MAKE $x "$@" > make${f}.txt 2>&1 &
  f=`expr $f + 1`
done
wait

if test ! -r "$ERRFILE"; then
  rm -f TestAnalysisDirectoryLCv2.11/bin/TestAnalysisDirectoryLC
  MAKE TestAnalysisDirectoryLCv2.11/bin "$@" > make${f}.txt 2>&1 &
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
BUILD "$@" > make.txt 2>&1
test ! -r $ERRFILE
exit $?
