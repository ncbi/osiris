#!/bin/bash
clear
cd `dirname $0`
cd ../..
DIR=`pwd`
cd ..
echo "Preparing to run OSIRIS as an administrator in order to modify settings"
echo "You will be prompted for your Macintosh password"
echo "If OSIRIS is currently running, switch to it and exit"
echo ""
echo "Press the 'return' key to continue"
head -1
sudo open -a "${DIR}"
echo "Press the 'return' key to exit"
head -1
