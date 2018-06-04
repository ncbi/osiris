#!/bin/bash
cd $(dirname $0)
if test ! -r makeinit.sh; then
  echo "Cannot find makeinit.sh"
  exit 1
fi
. makeinit.sh
MAKE_TAR_GZ "$@"
