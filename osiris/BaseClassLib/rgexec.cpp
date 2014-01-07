/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
*  FileName: rgexec.cpp
*  Author:   Robert Goor
*
*/
//
//  class RGExec which incorporates process-related functions, for starting processes and monitoring them
//

#include <stdio.h>

#ifdef _WINDOWS

#include <process.h>
#include <io.h>
#include <time.h>

#elif defined WIN32

#include <process.h>
#include <io.h>
#include <time.h>

#else

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#endif

#include "rgexec.h"

int RGExec :: System (const char* cmd) {

#ifdef _WINDOWS
	_flushall ();
#elif WIN32
	_flushall ();
#endif

	return system (cmd);
}

#ifdef _WINDOWS

long int RGExec :: FileModificationTime (const char* file) {

	struct _finddata_t Attributes;
	long ReturnValue;

	ReturnValue = _findfirst (file, &Attributes);

	if (ReturnValue == -1L)
		return -1;

	long int time;
	time = Attributes.time_write;
	_findclose (ReturnValue);
	return time;
}


long int RGExec :: FileSize (const char* file) {

	struct _finddata_t Attributes;
	long ReturnValue;

	ReturnValue = _findfirst (file, &Attributes);

	if (ReturnValue == -1L)
		return -1;

	long int Size;
	Size = Attributes.size;
	_findclose (ReturnValue);
	return Size;
}


#else

#include <sys/stat.h>

long int RGExec::FileModificationTime(const char *file)
{
  long nRtn = 0;
	struct stat statbuf;
  if(!stat(file,&statbuf))
  {
#ifdef Darwin
    nRtn = (long) statbuf.st_mtimespec.tv_sec;
#else
    nRtn = (long) statbuf.st_mtime;
#endif
  }
  return nRtn;
}
long int RGExec::FileSize(const char *file)
{
  long nRtn = 0;
	struct stat statbuf;
  if(!stat(file, &statbuf))
  {
    nRtn = (long) statbuf.st_size;
  }
  return nRtn;
}

#endif
