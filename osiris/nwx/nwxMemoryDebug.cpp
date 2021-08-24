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
*  FileName: nwxMemoryDebug.h
*  Author:   Douglas Hoffman
*
*/

#ifdef _WINDOWS
#ifdef _DEBUG
#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE
#endif
#endif
#include <windows.h>
#include <debugapi.h>
#include <string.h>
#include "nwx/nwxMemoryDebug.h"

long *nwxMemoryDebug::p_current_request_number = NULL;
void nwxMemoryDebug::DumpAlloc(const char *pFile, int nLine, const wchar_t *psNote)
{
  if (p_current_request_number != NULL)
  {
    const char DELIMW = L'\\';
    const char DELIM = L'/';
    const char *ps;
    if (psNote == NULL)
    {
      psNote = L"";
      
    }
    if (*pFile)
    {
      for (ps = pFile + strlen(pFile) - 1; (ps > pFile) && ((*ps) != DELIM); --ps);
      if ((*ps) == DELIMW || (*ps) == DELIM) ++ps;
    }
    else
    {
      ps = "file";
    }
    wchar_t s[2048];
    wsprintf(s,L"** MEMORY %hs:%d {%ld} %s\n", ps, nLine, *p_current_request_number, psNote);
    OutputDebugStringW(s);
  }
}

#endif
#endif
