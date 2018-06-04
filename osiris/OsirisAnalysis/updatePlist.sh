#!/bin/sh
function checkerror()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 1>&2
    else
      echo "aborting script" 1>&2
    fi
    exit $1
  fi
}
UNAME=`uname | grep MINGW`
if test "$UNAME" != ""; then
  XSLTPROC=`which xsltproc.exe`
else
  XSLTPROC=`which xsltproc`
fi
test "$XSLTPROC" != ""
checkerror $? "Cannot find xsltproc"


XML=Info.plist
BACKUP="Info.plist."`date +%Y%m%d-%H%M%S`
XSL=updatePlist.xsl
OUTFILE=tmpInfo.txt
cd `dirname $0`
test -r $XML
checkerror $? "cannot find plist file, ${XML}"
test -r $XSL
checkerror $? "cannot find xsl file, ${XSL}"
V=`grep "define OSIRIS_VERS_BASE " ../Version/OsirisVersion.h | sed 's/^.* "//' | sed 's/".*//'`
test "$V" != ""
checkerror $? "cannot obtain osiris version"
echo "version: $V"
head -2 "${XML}" > $OUTFILE
checkerror $?
"$XSLTPROC" --param "version" "'${V} '" "$XSL" "$XML" >> $OUTFILE
checkerror $?
mv "$XML" "$BACKUP"
checkerror $?
mv "$OUTFILE" "$XML"
E=$?
if test "$E" != "0"; then
  mv "$BACKUP" "$XML"
  checkerror $E
fi

