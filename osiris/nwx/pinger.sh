#!/bin/bash
#
#  ncbi pinger for macintosh
#
#

function ECHO2()
{
  echo "$@" 1>&2
}

function CHECKRC()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      ECHO2 "$2"
    fi
    exit $1
  fi
}
function FIND()
{
  for x in "/usr/bin" "/bin" "/usr/sbin" "/sbin" ; do
    s="${x}/${1}"
    if test -x "${s}"; then
      echo "${s}"
      return 0
    fi
  done
  s=$(which "$1")
  if test -x "${s}"; then
    echo "${s}"
    return 0
  fi
  return 1
}

function SETUP()
{
  URL="https://www.ncbi.nlm.nih.gov/stat"
  APP_VER="v0"
  APP_NAME="ncbi pinger"
  BASE_QUERY=""
  STARTTIME=$(date '+%s')
  SESSION=$(date '+%y%m%d.%H%M%S.')$$
  NSCURL=$(FIND nscurl)
  CCURL=$(FIND curl)
  SWVERS=$(FIND sw_vers)
  
  if test -x "${SWVERS}" ; then
    eval $(sw_vers | sed -e 's/:\s*/="/' -e 's/$/"/')
  fi
  ProductName=${ProductName:-Mac OS X}
  ProductVersion=${ProductVersion:-unknown}
  export OSVERSION="${ProductName} ${ProductVersion}"

  if test -x "${CCURL}" ; then
    export CURL="${CCURL}"
    export ARGS="-s -f"
    SCUTIL=$(FIND scutil)
    if test -x "${SCUTIL}" ; then
      HTTPSEnable=""
      HTTPSPort=""
      HTTPSProxy=""
      eval $("${SCUTIL}" --proxy | grep "HTTPS.*:" | sed -e 's/^ *//' -e 's/\s*:\s*/="/' -e 's/$/"/')
      if test "${HTTPSEnable}" = "1" -a "${HTTPSPort}" != "" -a "${HTTPSProxy}" != ""; then
        export https_proxy="http://${HTTPSProxy}:${HTTPSPort}/"
        export http_proxy="${https_proxy}"
      fi
    fi
  elif test -x "${NSCURL}" ; then
    export CURL="${NSCURL}"
    export ARGS=""
  else
    CHECKRC 1 "Cannot find curl, exit $BASH_SOURCE"
  fi
}



function rawurlencode()
{
# https://stackoverflow.com/questions/296536/how-to-urlencode-data-for-curl-command

  local string="${1}"
  local strlen=${#string}
  local encoded=""
  local pos c o

  for (( pos=0 ; pos<strlen ; pos++ )); do
     c=${string:$pos:1}
     case "$c" in
        [-_.~a-zA-Z0-9] ) o="${c}" ;;
        * )               printf -v o '%%%02x' "'$c"
     esac
     encoded+="${o}"
  done
  echo "${encoded}"    # You can either set a return variable (FASTER) 
  REPLY="${encoded}"   #+or echo the result (EASIER)... or both... :p
}

function ADD_PAIR()
{
  local BASE="$1"
  local PAIR=$(rawurlencode "$2" | sed 's/%3d/=/i')
  # decode first equal sign
  if test "$PAIR" = ""; then
    true
  elif test "$(echo \"$PAIR\" | grep '=')" = ""; then
    PAIR="${PAIR}="
  fi
  if test "${BASE}" != "" -a "${PAIR}" != ""; then
    echo "${BASE}&${PAIR}"
  else
    echo "${BASE}${PAIR}"
  fi
}

function PING()
{
  T=$(date '+%s')
  E=$(expr $T - $STARTTIME)
  Q=$(ADD_PAIR "$1" "elapse=${E}")
  echo "$CURL" $ARGS -A "${USER_AGENT}" -D - -o /dev/null "${URL}?${Q}"
  CHECKRC $? "${CURL} failed"
}


function MAIN()
{
SETUP

while getopts ":v:a:s:" opt ; do
  case $opt in
    a)
        APP_NAME="${OPTARG}"
        BASE_QUERY=$(ADD_PAIR "${BASE_QUERY}" "ncbi_app=${APP_NAME}")
        ;;
    v)
        APP_VER="${OPTARG}"
        BASE_QUERY=$(ADD_PAIR "${BASE_QUERY}" "app_ver=${APP_VER}")
        ;;
    u)
        URL="${OPTARG}"
        ;;
    s)
        BASE_QUERY=$(ADD_PAIR "${BASE_QUERY}" "${OPTARG}")
        ;;
    *)
       ECHO2 "${opt}: invalid parameter"
       ;;
  esac
done

USER_AGENT="${APP_NAME} ${APP_VER} for ${OSVERSION}"
BASE_QUERY=$(ADD_PAIR "${BASE_QUERY}" "session=${SESSION}")
BASE_QUERY=$(ADD_PAIR "${BASE_QUERY}" "os=${OSVERSION}")

PING "${BASE_QUERY}"
Q="${BASE_QUERY}"
while read x; do
  if test "$x" = "go"; then
    PING "$Q"
    Q="${BASE_QUERY}"
  else
    Q=$(ADD_PAIR "${Q}" "$x")
  fi
done

Q=$(ADD_PAIR "${BASE_QUERY}" "done=1")
PING "$Q"

}
MAIN "$@"
