#!/bin/sh
echo '@echo off' | u2d > temp_uninstall.bat
grep ProductCode Setup.vdproj |  sed 's/.*:/start %SystemRoot%\\system32\\MsiExec.exe \/x/' | sed 's/"$//' | u2d >> temp_uninstall.bat
diff -q uninstall.bat temp_uninstall.bat
if test "$?" != "0"; then
  mv -v temp_uninstall.bat uninstall.bat
fi
