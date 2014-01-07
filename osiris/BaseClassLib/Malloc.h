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
*  FileName: Malloc.h
*
*/
#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stdlib.h>

class Malloc
{
public:
  Malloc(size_t n)
  {
    m_ptr = (n > 0) ? malloc(n) : NULL;
  }
  Malloc(void *p)
  {
    m_ptr = p;
  }
  virtual ~Malloc()
  {
    if(m_ptr != NULL)
    {
      free(m_ptr);
    }
  }
  void *release()
  {
    void *pRtn = m_ptr;
    m_ptr = NULL;
    return pRtn;
  }
  operator void *()
  {
    return m_ptr;
  }
  operator char *()
  {
    return (char *)m_ptr;
  }
  void *ptr()
  {
    return m_ptr;
  }
private:
  void *m_ptr;
};

#endif
