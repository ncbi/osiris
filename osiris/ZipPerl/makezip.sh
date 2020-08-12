#!/bin/bash
cd $(dirname $(readlink -f "$0"))
export PERL5LIB=.
export PATH="/c/Program Files/Perl64/bin:/c/Program Files/7-Zip:${PATH}"

perl MakeZip.pl
