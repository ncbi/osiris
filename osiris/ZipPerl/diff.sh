#!/usr/bin/bash
echo $@
set -x
/usr/bin/diff $1 $2
