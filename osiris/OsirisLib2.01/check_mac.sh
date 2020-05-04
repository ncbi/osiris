#!/bin/bash
cd $(dirname "$0")

function GETLIST()
{
  grep cpp OsirisLib.vcxproj | sed -e 's/.*="//' -e 's/" .*//'
}
function CHECK_ITEM()
{
  grep -iL "$1" ./lib/Makefile.am > /dev/null 2>&1 || echo "$1"
}

for x in $(GETLIST); do
  test "$x" != "ExpressionParser.cpp" &&  CHECK_ITEM "$x"
done
