#!/bin/sh

if test "$WXHOME" = ""; then
  WXHOME=`sh findwx`
  if test "$WXHOME" = ""; then
    echo "Cannot find WXHOME in environment" 1>&2
    exit 1
  fi
  export WXHOME
fi
for x in wxthings/lib wxplotctrl/lib BaseClassLib/lib OsirisMath/lib OsirisLib2.01/lib TestAnalysisDirectoryLCv2.11/bin fsa2xml/bin wxXml2/lib wxXsl/lib nwxZip/lib nwx/lib OsirisAnalysis/bin OSstarter/bin OpenFolder/bin; do
(cd $x ; pwd ; aclocal; autoconf; automake -a)
done
(cd OsirisAnalysis; sh updatePlist.sh)
(cd OsirisLib2.01 ; sh blex.sh clean all )

aclocal
autoconf
automake -a
./configure
