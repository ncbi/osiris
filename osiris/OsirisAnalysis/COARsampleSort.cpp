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
*  FileName: CSampleSort.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARsampleSort.h"
#include "CGridAnalysisDisplay.h"
#include "wxIDS.h"
#include <set>
#include <memory>

typedef pair<size_t,size_t> SeverityPair;

class SeverityLess
{
public:
  // sort by 'second' descending then 'first' ascending
  bool operator()(const SeverityPair &a, const SeverityPair &b)
  {
    return (a.second > b.second) ||
      ( (a.second == b.second) && (a.first < b.first) );
  }
};

class FileNameLess
{
public:
    bool operator()(const COARsample *p1, const COARsample *p2)
    {
      int nCmp = nwxString::CompareSort(p1->GetFileName(),p2->GetFileName());
      return (nCmp < 0);
    }
};

class SampleNameLess
{
public:
    bool operator()(const COARsample *p1, const COARsample *p2)
    {
      int nCmp = nwxString::CompareSort(p1->GetSampleName(),p2->GetSampleName());
      if(!nCmp)
      {
        nCmp = nwxString::CompareSort(p1->GetFileName(),p2->GetFileName());
      }
      return (nCmp < 0);
    }
};


class SampleRunStartLess
{
public:
    bool operator()(const COARsample *p1, const COARsample *p2)
    {
      int nCmp;
      if( (nCmp = nwxString::Compare(p1->GetRunStart(),p2->GetRunStart())) != 0 );
      else if( (nCmp = nwxString::CompareSort(p1->GetSampleName(),p2->GetSampleName())) != 0 );
      else
      {
        nCmp = nwxString::CompareSort(p1->GetFileName(),p2->GetFileName());
      }
      return (nCmp < 0);
    }
};

typedef set<SeverityPair, SeverityLess> SeveritySet;
typedef set<COARsample *,FileNameLess> SampleFileNameSet;
typedef set<COARsample *, SampleNameLess> SampleNameSet;
typedef set<COARsample *,SampleRunStartLess> SampleRunStartSet;

const int COARsampleSort::DISPLAYED_NAME(IDmenuSortDisplayedName - IDmenuSortBASE);
const int COARsampleSort::FILE_NAME(IDmenuSortFileName  - IDmenuSortBASE);
const int COARsampleSort::SAMPLE_NAME(IDmenuSortSampleName - IDmenuSortBASE);
const int COARsampleSort::SEVERITY(IDmenuSortSeverity  - IDmenuSortBASE);
const int COARsampleSort::RUN_TIME(IDmenuSortRunTime  - IDmenuSortBASE);

class ISampleSorter
{
public:
  virtual ~ISampleSorter() {};
  virtual void Clear() = 0;
  virtual void Insert(COARsample *p) = 0;
  virtual COARsample *First() = 0;
  virtual COARsample *Next() = 0;
};

class SampleSortSeverity : public ISampleSorter
{
public:
  SampleSortSeverity(COARfile *pFile, CHistoryTime *pTime) :
    m_pFile(pFile), m_pTime(pTime)
  {
    m_pv.reserve(120);
  }
  virtual ~SampleSortSeverity() {}
  virtual void Clear()
  {
    m_set.clear();
    m_pv.clear();
    m_pv.reserve(120);
  }
  virtual void Insert(COARsample *pSample)
  {
    size_t n = m_pv.size();
    size_t nSeverity = pSample->CountAlerts(m_pFile->GetMessages(),*m_pTime);
    SeverityPair xpair(n,nSeverity);
    m_set.insert(xpair);
    m_pv.push_back(pSample);
  }
  virtual COARsample *First()
  {
    m_itr = m_set.begin();
    return Next();
  }
  virtual COARsample *Next()
  {
    COARsample *pRtn = NULL;
    if(m_itr != m_set.end())
    {
      pRtn = m_pv.at(m_itr->first);
      m_itr++;
    }
    return pRtn;
  }

private:
  COARfile *m_pFile;
  CHistoryTime *m_pTime;
  SeveritySet m_set;
  vector<COARsample *> m_pv;
  SeveritySet::iterator m_itr;
};

template<class SETTYPE> class SampleSort : public ISampleSorter
{
public:
  virtual ~SampleSort<SETTYPE>() {}
  virtual void Clear()
  {
    m_set.clear();
  }
  virtual void Insert(COARsample *p)
  {
    m_set.insert(p);
  }
  virtual COARsample *First()
  {
    m_itr = m_set.begin();
    return Next();
  }
  virtual COARsample *Next()
  {
    COARsample *pRtn = NULL;
    if(m_itr != m_set.end())
    {
      pRtn = *m_itr;
      m_itr++;
    }
    return pRtn;
  }
private:
  typename SETTYPE::iterator m_itr;
  SETTYPE m_set;
};


void COARsampleSort::Sort(COARfile *pFile, const wxDateTime *pTime)
{
  m_pFile = pFile;
  m_histTime.SetDateTime(pTime);
  _Sort();
}

void COARsampleSort::_CheckUpdate()
{
  if( (m_pFile != NULL) &&
      (m_dtLastLoad != m_pFile->GetLastLoad()) )
  {
    m_dtLastLoad = m_pFile->GetLastLoad();
    if((m_nLastSort < 0) || (m_nLastSort == SEVERITY))
    {
      _Sort();
    }
  }
}
void COARsampleSort::_Sort()
{

  size_t nRowCount = m_pFile->GetSampleCount();
  auto_ptr<ISampleSorter> pSort(NULL);

  m_bLastControlOnTop = CGridAnalysisDisplay::GetControlsOnTop();
  m_nLastSort = CGridAnalysisDisplay::GetSortType();
  m_vpSamples.clear();
  m_vpSamples.reserve(nRowCount);

  if(m_nLastSort == SAMPLE_NAME)
  {
    pSort = auto_ptr<ISampleSorter>(new SampleSort<SampleNameSet>);
  }
  else if(m_nLastSort == SEVERITY)
  {
    pSort = auto_ptr<ISampleSorter>(new SampleSortSeverity(m_pFile,&m_histTime));
  }
  else if(m_nLastSort == RUN_TIME)
  {
    pSort = auto_ptr<ISampleSorter>(new SampleSort<SampleRunStartSet>);
  }
  else
  {
    pSort = auto_ptr<ISampleSorter>(new SampleSort<SampleFileNameSet>);
  }
  size_t i;
  COARsample *pSample;
  for(i = 0; i < nRowCount; i++)
  {
    pSample = m_pFile->GetSample(i);
    if((!m_bLastControlOnTop) || pSample->IsSampleType())
    {
      pSort->Insert(m_pFile->GetSample(i));
    }
    else
    {
      m_vpSamples.push_back(pSample);
    }
  }
  for(pSample = pSort->First(); pSample != NULL; pSample = pSort->Next())
  {
    m_vpSamples.push_back(pSample);
  }

}
const size_t COARsampleSort::NPOS = ~((size_t)0);

size_t COARsampleSort::GetSampleIndex(COARsample *pSample)
{
  size_t nRtn = NPOS;
  size_t nSize = GetCount();
  for(size_t i = 0; i < nSize; ++i)
  {
    if(pSample == GetSample(i))
    {
      nRtn = i;
      i = nSize;
    }
  }
  return nRtn;
}


/*
void COARsampleSort::_SortByFileName()
{
  // very simple
  size_t nRowCount = m_pFile->GetSampleCount();
  size_t i;
  for(i = 0; i < nRowCount; ++i)
  {
    m_vpSamples.push_back(m_pFile->GetSample(i));
  }
}


void COARsampleSort::_SortBySampleName()
{
  SampleNameSet setSamples;
  SampleNameSet::iterator itr;
  size_t nRowCount = m_pFile->GetSampleCount();
  size_t i;
  COARsample *pSample;
  for(i = 0; i < nRowCount; i++)
  {
    pSample = m_pFile->GetSample(i);
    if(pSample->IsSampleType())
    {
      setSamples.insert(pSample);
    }
    else
    {
      m_vpSamples.push_back(pSample);
    }
  }
  for(itr = setSamples.begin();
    itr != setSamples.end();
    ++itr)
  {
    m_vpSamples.push_back(*itr);
  }
}
void COARsampleSort::_SortBySeverity()
{
  //  build set of PairSeverity to sort

  SeveritySet setSamples;
  COARsample *pSample;
  size_t nRowCount = m_pFile->GetSampleCount();
  size_t i;
  for(i = 0; i < nRowCount; ++i)
  {
    pSample = m_pFile->GetSample(i);
    SeverityPair xpair(i,pSample->CountAlerts(m_pFile->GetMessages(),m_histTime));
    setSamples.insert(xpair);
  }
    //
    //  now build array in reverse order of severity
    //
  for(SeveritySet::iterator itr = setSamples.begin();
    itr != setSamples.end();
    ++itr)
  {
    pSample = m_pFile->GetSample((*itr).first);
    m_vpSamples.push_back(pSample);
  }
}


*/
