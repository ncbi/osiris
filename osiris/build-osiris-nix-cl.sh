#!/bin/bash
cd $(dirname "$0")
for x in BaseClassLib/lib OsirisMath/lib OsirisLib2.01/lib TestAnalysisDirectoryLCv2.11/bin; do
(cd $x ; pwd ; aclocal; autoconf; automake -a)
done
(cd OsirisLib2.01 ; sh blex.sh clean all )

aclocal
autoconf configure-cl.ac > configure
mv Makefile.am Makefile.am.backup
cp Makefile-cl.am Makefile.am
automake -a
mv Makefile.am.backup Makefile.am
./configure
