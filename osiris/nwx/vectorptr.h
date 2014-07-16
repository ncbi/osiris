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

*  FileName: vectorptr.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __VECTOR_PTR_HPP
#define __VECTOR_PTR_HPP

#include "nwx/stdb.h"
#include <vector>
// include <set> to define std::less
#include <set>
#include "nwx/stde.h"

template<class T> class vectorptr : 
  public std::vector<T *>
{
public:
  vectorptr<T>() {;}
  vectorptr<T>(const std::vector<T *> &x)
  {
    copy(this,x);
  }
  virtual ~vectorptr<T>()
  {
    vectorptr<T>::cleanup(this);
  }
public:
  static void cleanup(std::vector<T *> *p)
  {
    typename std::vector<T *>::reverse_iterator itr;
    T *px;
    while(p->size() > 0)
    {
      itr = p->rbegin();
      px = *itr;
      delete (px);
      p->pop_back();
    }
  }
  static std::vector<T *> copy(std::vector<T *> *pTo, const std::vector<T *> &xFrom)
  {
    size_t nSize = xFrom.size();
    cleanup(pTo);
    if(nSize)
    {
      pTo->reserve(nSize);
      for(size_t i = 0; i < nSize; i++)
      {
        T *px(xFrom.at(i));
        pTo->push_back(new T(*px));
      }
    }
    return *pTo;
  }
  static bool IsEqual(
    const std::vector<T *> &x1,
    const std::vector<T *> &x2)
  {
    bool bRtn = false;
    if(x1.size() == x2.size())
    {
      typename std::vector<T *>::const_iterator itr1;
      typename std::vector<T *>::const_iterator itr2;
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
  static bool remove(std::vector<T*> *pv,T *p)
  {
    bool bRtn = false;
    typename std::vector<T*>::iterator itr;
    for(itr = pv->begin(); itr != pv->end(); ++itr)
    {
      if((*itr) == p)
      {
        pv->erase(itr);
        delete p;
        bRtn = true;
        break;
      }
    }
    return bRtn;
  }
  static bool removeAt(std::vector<T*> *pv, size_t n)
  {
    bool bRtn = false;
    if(n < pv->size())
    {
      typename std::vector<T *>::iterator itr = pv->begin();
      for(size_t i = 0; i < n; i++)
      {
        ++itr;
      }
      bRtn = true;
      delete *itr;
      pv->erase(itr);
    }
    return bRtn;
  }
};

template<class T, class S = std::less<T *> > class vectorptrSort
{
private:
  vectorptrSort<T,S>() {} // prevent instantiation
public:
  static void Bsort(std::vector<T *> *pv, const S &cmp)
  {
    // do a bubble sort because it should already be sorted
    size_t nBot = pv->size();
    size_t i;
    size_t j;
    bool bDone = !nBot;
    T *t1;
    T *t2;
    while(!bDone)
    {
      bDone = true;
      j = 0;
      nBot--;
      for(i = 0; i < nBot; i++)
      {
        j++;
        t1 = pv->at(i);
        t2 = pv->at(j);
        if(cmp(t2,t1))
        {
          (*pv)[i] = t2;
          (*pv)[j] = t1;
          bDone = false;
        }
      }
    }
  }
};


#endif
