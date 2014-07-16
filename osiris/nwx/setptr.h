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

*  FileName: setptr.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __SET_PTR_HPP__
#define __SET_PTR_HPP__

#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

template<class T, class S = std::less<T *> > class setptr :
    public std::set<T *,S>
{
public:
  setptr<T,S>() {;}
  setptr<T,S>(const std::set<T *,S> &x)
  {
    copy(this,x);
  }
  virtual ~setptr<T,S>()
  {
    cleanup(this);
  }
  static bool IsEqual(
    const std::set<T *,S> &x1,
    const std::set<T *,S> &x2)
  {
    bool bRtn = false;
    if(x1.size() == x2.size())
    {
      typename std::set<T *,S>::const_iterator itr1;
      typename std::set<T *,S>::const_iterator itr2;
      itr1 = x1.begin();
      itr2 = x2.begin();
      T *p1;
      T *p2;
      bRtn = true;
      while(bRtn && (itr1 != x1.end()))
      {
        p1 = *itr1;
        p2 = *itr2;
        if(!( (*p1) == (*p2) ))
        {
          bRtn = false;
        }
        ++itr1;
        ++itr2;
      }
    }
    return bRtn;
  }

public:
  static void cleanup(std::set<T *,S> *p)
  {
    typename std::set<T *,S>::iterator itr;
    T *px;
    for(itr = p->begin();
      itr != p->end();
      itr = p->begin())
    {
      px = *itr;
      delete (px);
      p->erase(itr);
    }
  }
  static void copy(std::set<T *,S> *pTo, const std::set<T *,S> &xFrom)
  {
    cleanup(pTo);
    typename std::set<T *,S>::const_iterator itr;
    for(itr = xFrom.begin(); itr != xFrom.end(); ++itr)
    {
      T *px = *itr;
      T *pnew = new T(*px);
      pTo->insert(pnew);
    }
  }
};

#endif
