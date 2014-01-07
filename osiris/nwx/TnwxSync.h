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
*  FileName: TnwxSync.h
*  Author:   Douglas Hoffman
*
*  Template class for syncronizing the state of multiple instances
*  of a class.  Current usage: nwxSyncSplitter, nwxSyncNotebook,
*    and nwxSyncCheckbox
*/
#ifndef __T_NWX_SYNC_H__
#define __T_NWX_SYNC_H__

#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

template<class T, class N> class TnwxSync
{
public:
  virtual ~TnwxSync<T,N>() {;}
  virtual void SyncThis(T *p, N n) = 0;
  virtual void Sync(T *p) = 0;

  virtual void SyncTo(N n)
  {
    for( typename set<T *>::iterator itr = m_set.begin();
      itr != m_set.end();
      ++itr)
    {
      SyncThis(*itr,n);
    }
  }
  virtual void Add(T *p)
  {
    if(p != NULL)
    {
      m_set.insert(p);
    }
  }
  virtual bool Find(T *p)
  {
    return (p == NULL) ? false : (m_set.find(p) != m_set.end());
  }
protected:
  set<T *> m_set;
};


#endif
