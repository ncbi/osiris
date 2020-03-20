#!/bin/bash
if test -d "/c"; then
  DRIVE="/c"
elif test -d "/cygdrive/c"; then
  DRIVE="/cygdrive/c"
else
  echo "Cannot find c drive"
  exit 1
fi
export PATH="/bin:/usr/bin:${PATH}"
find "${DRIVE}/Apps/User_Installs/Osiris-2.14-Beta-5" | grep -v "/site.." | grep -v "^${DRIVE}/Apps/User_Installs/Osiris-2.14-Beta-5"'$' | sed "s|.*${DRIVE}/Apps/User_Installs/Osiris-2.14-Beta-5.||" > install.txt
find "Osiris-2.14-Beta-5" | grep -v "^Osiris-2.14-Beta-5"'$' | sed 's/.*Osiris-2.14-Beta-5.//' > zip.txt
diff install.txt zip.txt
