#!/bin/bash
cd $(dirname "$0")
~/Documents/digisign/sign-file.sh Osiris*.exe  ../Setup/Release/*.msi
bash copyf.sh
