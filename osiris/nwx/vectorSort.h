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
*  FileName: vectorSort.h
*  Author:   Douglas Hoffman
*
*/
#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"

#ifndef __VECTOR_SORT_H__
#define __VECTOR_SORT_H__



template< class T, class S = less<T> > class vectorSort
{
private:
  template< class TT, class SET > class vectorSortSet
  {
  private:
    vectorSortSet() {}; // prevent instantiation
  public:
    static void Sort(vector<TT> *pvec)
    {
      SET setT;
      typename SET::iterator itrs;
      typename vector<TT>::iterator itrv;
      for(itrv = pvec->begin();
        itrv != pvec->end();
        ++itrv)
      {
        setT.insert(*itrv);
      }
      pvec->clear();
      for(itrs = setT.begin();
        itrs != setT.end();
        ++itrs)
      {
        pvec->push_back(*itrs);
      }
    }
  };
  vectorSort() {}; // prevent instantiation
public:
  static void Sort(vector<T> *pvec)
  {
    vectorSortSet< T,set<T,S> >::Sort(pvec);
  }
  static void SortMulti(vector<T> *pvec)
  {
    vectorSortSet< T,multiset<T,S> >::Sort(pvec);
  }
};

#endif
