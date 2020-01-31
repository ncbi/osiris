#!/bin/bash
cd $(dirname "$0")
~/Documents/digisign/sign-file.sh Osiris*.exe 
bash copyf.sh
