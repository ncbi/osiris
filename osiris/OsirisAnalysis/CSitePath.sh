#!/bin/bash
#
# ===========================================================================
#
#                            PUBLIC DOMAIN NOTICE                          
#               National Center for Biotechnology Information
#                                                                          
#  This software/database is a "United States Government Work" under the   
#  terms of the United States Copyright Act.  It was written as part of    
#  the author's official duties as a United States Government employee and 
#  thus cannot be copyrighted.  This software/database is freely available 
#  to the public for use. The National Library of Medicine and the U.S.    
#  Government have not placed any restriction on its use or reproduction.  
#                                                                          
#  Although all reasonable efforts have been taken to ensure the accuracy  
#  and reliability of the software and data, the NLM and the U.S.          
#  Government do not and cannot warrant the performance or results that    
#  may be obtained by using this software or data. The NLM and the U.S.    
#  Government disclaim all warranties, express or implied, including       
#  warranties of performance, merchantability or fitness for any particular
#  purpose.                                                                
#                                                                          
#  Please cite the author in any work or product based on this material.   
#
# ===========================================================================
#
#
#  FileName: CSitePath.sh
#  Author:   Douglas Hoffman
#  Date:     2018/02/18
#
#
#  bash script for modifying settings for the
#  directory containing OSIRIS site settings.
#  site settings are used for exports and
#  operating procedures for an installation
#
#  These are called site settings because they
#  affect all users of the software instance
#
#

function CHECKRC()
{
  if test "$1" != "0"; then
    if test "$2" != ""; then
      echo "$2" 1>&2
    fi
    exit $1
  fi
}

function FINDEXE()
{
  EXE="${1}"
  for p in /usr/bin /bin /sw/bin /sw/sbin /usr/sbin /sbin; do
    if test -x "${p}/${EXE}"; then
      echo "${p}/${EXE}"
      return 0
    fi
  done
  if test "$2" = "x"; then
    echo "Cannot find ${EXE}, aborting" 1>&2
    exit 1
  fi
  return 1
}
function CHECKEXE()
{
  EXE="${1}"
  NAME="${2}"
  test "${EXE}" != ""
  CHECKRC $? "Cannot find ${NAME}, aborting..."
}

export PATH="/usr/bin:/bin:/sw/bin:/sw/sbin:/usr/sbin:/sbin:${PATH}"
READLINK=`FINDEXE greadlink`
CHMOD=`FINDEXE chmod`
CHGRP=`FINDEXE chgrp`
FIND=`FINDEXE find`
HEAD=`FINDEXE head`
GREP=`FINDEXE grep`

CHECKEXE "$CHMOD" chmod
CHECKEXE "$CHGRP" chgrp
CHECKEXE "$FIND" find
CHECKEXE "$HEAD" head
CHECKEXE "$GREP" grep


function CHECKGROUP
{
  ## needs work on find commands
  DIR="$1"
  GROUP="$2"
  XX=$("${FIND}" "${DIR}" -not -group "${GROUP}" | "${HEAD}" -1)
  if test "${XX}" != ""; then
    return 1
  fi
  XX=$("${FIND}" "${DIR}" -not -perm -0664 | "${HEAD}" -1)
  if test "${XX}" != ""; then
    return 1
  fi
  XX=$("${FIND}" "${DIR}" -type d -not -perm -02664 | "${HEAD}" -1)
  if test "${XX}" != ""; then
    return 1
  fi
  return 0  
}

function CHECKOWN
{
  ## needs work on find commands
  DIR="$1"
  GROUP="$2"
  USER="$3"
  if test "$USER" != ""; then
    XX=$("${FIND}" "${DIR}" -not -user "${USER}" | "${HEAD}" -1)
    if test "$XX" != ""; then
      return 1
    fi
  fi
  if test "$GROUP" = ""; then
    XX=$("${FIND}" "${DIR}" -perm +020 | "${HEAD}" -1)
    if test "$XX" != ""; then
      return 1
    fi
  fi
}

function CHGRP()
{
  DIR="$1"
  GROUP="$2"
  if test "${READLINK}" != ""; then
    RDIR=$("${READLINK}" -f "${DIR}" )
    if test "${RDIR}" != "${DIR}" -a "${RDIR}" != ""; then
      DIR="${RDIR}"
    fi
  fi
  test -d "${DIR}"
  CHECKRC $?
  "${CHGRP}" -R "${GROUP}" "${DIR}"
  CHECKRC $?
  "${CHMOD}" -R g+w "${DIR}"
  CHECKRC $?
  "${FIND}" "${DIR}" -type d -exec "${CHMOD}" g+ws '{}' ';'
  CHECKRC $?
  CHECKGROUP "${DIR}" "${GROUP}"
  CHECKRC $?
}

function MKDIR()
{
  DIR="$1"
  GROUP="$2"
  USER="$3"
  if test ! -d "${DIR}"; then
    mkdir -p "${DIR}"
    CHECKRC $?
  fi
  if test "$USER" != ""; then
    chown -R "${USER}" "${DIR}"
    CHECKRC $?
    if test "$GROUP" = ""; then
      "${CHMOD}" -R g-ws "${DIR}"
      CHECKRC $?
    fi
    CHECKOWN "${DIR}" "${GROUP}" "${USER}"
    CHECKRC $?
  fi
  if test "$GROUP" != ""; then
    CHGRP "${DIR}" "${GROUP}"
  fi  
}

function MKSUBDIR()
{
  PARENT="$1"
  DIR="$2"
  GROUP="$3"
  USER="$4"
  SLASH=`echo "${DIR}" | "${GREP}" ^/`
  test "$SLASH" = ""
  CHECKRC $? "Subdir, ${DIR}, should not begin with a slash /"
  test -d "${PARENT}"
  CHECKRC $? "cannot find directory ${PARENT}"
  cd "${PARENT}"
  CHECKRC $?
  shift
  MKDIR "$@"
}


function SHOWDIR()
{
  DIR="$1"
  test -d "${DIR}" -a -x "${DIR}"
  CHECKRC $? "Cannot find directory, ${DIR}"
  cd "${DIR}"
  CHECKRC $? "Cannot open directory, ${DIR}"
  open -R "${DIR}"
  CHECKRC $?
}

function SHOWINDIR()
{
  DIR="$1"
  test -d "${DIR}" -a -x "${DIR}"
  CHECKRC $? "Cannot find directory, ${DIR}"
  cd "${DIR}"
  CHECKRC $? "Cannot open directory, ${DIR}"
  open "${DIR}"
  CHECKRC $?
}

if test "$1" != ""; then
  CMD="$1"
  EXIST=`typeset -F | "${GREP}" "^declare -f ${CMD}"'$'`
  test "$EXIST" != ""
  CHECKRC $? "function ${CMD} does not exist"
  shift
  $CMD "$@"
fi
