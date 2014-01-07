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

*  FileName: nwxBatch.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_BATCH_H__
#define __NWX_BATCH_H__

#include <set>
#include <vector>

template <class T> class TnwxBatch
{
public:
  TnwxBatch<T>(T *p)
  {
    p->BeginBatch();
    m_p = p;
  }
  virtual ~TnwxBatch<T>()
  {
    m_p->EndBatch();
  }
private:
  T *m_p;
};

template < class T, class TYP  = std::set<T *> > class TnwxBatchColl
{
public:
  TnwxBatchColl<T,TYP>(TYP &coll)
  {
    m_V.reserve(coll.size());
    for(typename TYP::iterator itr = coll.begin();
      itr != coll.end();
      ++itr)
    {
      (*itr)->BeginBatch();
      m_V.push_back(*itr);
    }
  }
  virtual ~TnwxBatchColl<T,TYP>()
  {
    for(typename std::vector<T *>::iterator itr = m_V.begin();
      itr != m_V.end();
      ++itr)
    {
      (*itr)->EndBatch();
    }
  }
private:
  std::vector<T *> m_V;
};

class wxGrid;
typedef TnwxBatch<wxGrid> nwxGridBatch;

#endif
