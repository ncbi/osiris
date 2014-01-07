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
*  FileName: RGmemCheck.h
*  Author:   Robert Goor
*
*/
#ifndef __RG_MEM_CHECK_H__
#define __RG_MEM_CHECK_H__
//
//  RGmemCheck.h
//     Classes for finding and debugging memory links.
//
//     library needed for linking:
//        msvcrtd.lib
//     usage:
//        at or near the top of the .cpp file containing main()
//        add the following:
//
//      MEMCHECK(g_xxx,0)
//     where g_xxx is the name of the variable used for an instance of RGmemCheck
//
//     It is important that the destructor is called AFTER memory is released from any 
//     global class instances and so far, placing it above main() works
//     The destructor will dump information about all memory leaks.
//
//     if you suspect a memory leak withing a block of code, wrap the following around it
//
//       BEGIN_MEMDIFF(x)
//
//         /// enter code here
//
//       END_MEMDIFF
//
//     BEGIN_MEMDIFF(x) will create an instance of RGmemDiff x;
//       on the stack which contains the current state
//       of allocated memory
//     END_MEMDIFF 
//       pops the RGmemDiff occurrence off the stack and its destructor
//       dumps the difference in memory allocation since it was created
//       however this info is somewhat vague.
//
//
//      Q. Why use macros instead of more explicit class instances?
//      A. The class instances will not work unless _DEBUG and WIN32 are
//             defined in the C Preprocessor.  The macros will do NOTHING and therefore
//             will do no harm if these conditions are not met.
//
#if defined(_DEBUG) && defined(WIN32)

#include <stdlib.h>
#include <crtdbg.h>

class RGmemCheck
{
public:
  RGmemCheck(long nBreakAlloc = 0) 
  {
    if(nBreakAlloc > 0)
    {
      _CrtSetBreakAlloc(nBreakAlloc);
    }
    return;
  }
  virtual ~RGmemCheck()
  {
    _CrtDumpMemoryLeaks();
  }
};

class RGmemDiff
{
public:
  RGmemDiff()
  {
    _CrtMemCheckpoint(&m_mem);
  }
  virtual ~RGmemDiff()
  {
    _CrtMemState m2;
    _CrtMemState mdiff;
    _CrtMemCheckpoint(&m2);
    if(_CrtMemDifference(&mdiff,&m_mem,&m2))
    {
      _CrtMemDumpStatistics(&mdiff);
    }
  }
private:
  _CrtMemState m_mem;

};

#define MEMCHECK(x,y) RGmemCheck x(y)
#define MEMDIFF(x) RGmemDiff x
#define BEGIN_MEMDIFF(x) { MEMDIFF(x);
#define END_MEMDIFF }


#else

#define MEMCHECK(x,y)
#define MEMDIFF(x)
#define BEGIN_MEMDIFF(x)
#define END_MEMDIFF

#endif
#endif
