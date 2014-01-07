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
*  FileName: CSampleSort.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_SAMPLE_SORT_H__
#define __COAR_SAMPLE_SORT_H__

#include "COARfile.h"
#include "CHistoryTime.h"



class COARsampleSort
{
public:
  static const int DISPLAYED_NAME;
  static const int FILE_NAME;
  static const int SAMPLE_NAME;
  static const int SEVERITY;
  static const int RUN_TIME;


  COARsampleSort(COARfile *pFile)
  {
    m_dtLastLoad = pFile->GetLastLoad();
    Sort(pFile,false);
  }
  COARsampleSort() : 
    m_dtLastLoad((time_t)0),
    m_pFile(NULL),
    m_nLastSort(-1),
    m_bLastControlOnTop(false)
  {; }
  virtual ~COARsampleSort() {;}

  void Sort(COARfile *pFile, const wxDateTime *pTime = NULL);
  const vector<COARsample *> *GetSamples()
  {
    _CheckUpdate();
    return &m_vpSamples;
  }
  COARsample *GetSample(size_t i)
  {
    _CheckUpdate();
    return m_vpSamples.at(i);
  }
  size_t GetCount()
  {
    _CheckUpdate();
    return m_vpSamples.size();
  }
  vector<COARsample *> *operator ->()
  {
    _CheckUpdate();
    return &m_vpSamples;
  }
  size_t GetSampleIndex(COARsample *pSample);
  static const size_t NPOS;
private:
  void _CheckUpdate();
  void _Sort();
//  void _SortBySeverity();
//  void _SortBySampleName();
//  void _SortByFileName();

  void _SortByDisplay(int nDisplay);
  vector<COARsample *> m_vpSamples;

  CHistoryTime m_histTime;
  wxDateTime m_dtLastLoad;
  COARfile *m_pFile;
  int m_nLastSort;
  bool m_bLastControlOnTop;
};



#endif

