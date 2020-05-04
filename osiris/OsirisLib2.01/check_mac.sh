#!/bin/bash
cd $(dirname "$0")
grep cpp OsirisLib.vcxproj | sed -e 's/.*="//' -e 's/" .*/ \\/' -e 's/^/..\//' > f.txt
diff -b f.txt lib/Makefile.am