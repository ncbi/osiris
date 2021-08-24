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
*  For use with Windows Visual Studio
*
* Purpose - help to find memory links
* before running in debugger:
* in C:\Program Files (x86)\Windows Kits\10\Source\10.0.17763.0\ucrt\heap\debug_heap.cpp
* set a break wherever __acrt_current_request_number is created - currently line 314
* using Visual Studio 15.9.33
* At the break, keep track of the pointer &__acrt_current_request_number and disable the breakpoint
*
* In nwxMemoryDebug.cpp create a breakpoint at the line containing
*  "if (p_current_request_number != NULL)" currently line 48.
*  set nwxMemoryDebug::p_current_request_number to &__acrt_current_request_number
*  retrieved earlier.  Then disable the breakpoint to increase performance.
*
* In the code, near the leak, put statments of
* SHOW_MEMORY or SHOW_MEMORY_NOTE(x) immediately before and after allocating memory
* this will dump the file, line, and __acrt_current_request_number to the console
* use __acrt_current_request_number - If the allocation number of the memory leak is between
* two numbers from "SHOW_MEMORY" the leak was allocated between these two occurrences
*
*  there is perl script in this directory, mem_report.pl, which sorts the output of SHOW_MEMORY(_NOTE) and the
*  actual memory leaks.  usage: perl mem_report.pl <input-file> <output-file>
*  For the input file, copy/paste the entire contents of the output window after the
*  debugger dumps memory leaks.
*
*/
#ifndef __NWX_MEMORY_DEBUG_H__
#define __NWX_MEMORY_DEBUG_H__
#ifdef _DEBUG
#ifdef _WINDOWS
#include <string.h>

class nwxMemoryDebug
{
private:
  ~nwxMemoryDebug() {} // prevent instantiation
public:
  static long *p_current_request_number;
  static void DumpAlloc(const char *pFile, int nLine, const wchar_t *psNote = NULL);
  static void DumpAlloc(const char *pFile, int nLine, const char *psNote)
  {
    // function to allow (const char *) for note and
    // converts to (const wchar_t *)
    const int SIZE = 1024;
    wchar_t sl[SIZE];
    const char *ps = psNote;
    wchar_t *psl = sl;
    wchar_t *psEnd = &sl[SIZE-1];

    for (psl = sl, ps = psNote; 
        (psl < psEnd) && (*ps);
        ++ps, ++psl)
    {
      *psl = wchar_t(*ps);
    }
    *psl = 0;
    DumpAlloc(pFile, nLine, sl);
  }
};

#define SHOW_MEMORY nwxMemoryDebug::DumpAlloc( __FILE__, __LINE__)
#define SHOW_MEMORY_NOTE(x) nwxMemoryDebug::DumpAlloc( __FILE__, __LINE__, x)


#endif // __WXDEBUG__
#endif // __WXMSW__

#ifndef SHOW_MEMORY
#define SHOW_MEMORY
#define SHOW_MEMORY_NOTE(x)
#endif


#endif