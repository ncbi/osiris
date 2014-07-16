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

*  FileName: mapptr.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __MAP_PTR_HPP__
#define __MAP_PTR_HPP__

#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

template<class K, class T, class S = std::less<K> > class mapptr :
    public std::map<K, T *,S>
{
private:
  mapptr<K,T,S>() {;} // prevent instantiation
public:
  static bool IsEqual(
    const std::map<K, T *,S> &x1,
    const std::map<K, T *,S> &x2)
  {
    bool bRtn = false;
    if(x1.size() == x2.size())
    {
      typename std::map<K, T *,S>::const_iterator itr1;
      typename std::map<K, T *,S>::const_iterator itr2;
      itr1 = x1.begin();
      itr2 = x2.begin();
      T *p1;
      T *p2;
      bRtn = true;
      while(bRtn && (itr1 != x1.end()))
      {
        if(itr1->first != itr2->first)
        {
          bRtn = false;
        }
        else
        {
          p1 = itr1->second;
          p2 = itr2->second;
          if(!( (*p1) == (*p2) ))
          {
            bRtn = false;
          }
        }
        ++itr1;
        ++itr2;
      }
    }
    return bRtn;
  }

public:
  static void cleanup(std::map<K, T *,S> *p)
  {
    typename std::map<K, T *,S>::iterator itr;
    T *px;
    for(itr = p->begin();
      itr != p->end();
      itr = p->begin())
    {
      px = itr->second;
      delete (px);
      p->erase(itr);
    }
  }
  static void copy(std::map<K, T *,S> *pTo, const std::map<K, T *,S> &xFrom)
  {
    typedef typename std::map<K, T *,S>::value_type VALUE_TYPE;
    cleanup(pTo);
    typename std::map<K, T *,S>::const_iterator itr;
    for(itr = xFrom.begin(); itr != xFrom.end(); ++itr)
    {
      T *px = itr->second;
      T *pnew = new T(*px);
      pTo->insert(VALUE_TYPE(itr->first,pnew));
    }
  }
};

#endif
