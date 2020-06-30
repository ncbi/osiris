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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*  FileName: CPrintOutAnalysis.h
*  Author:   Douglas Hoffman
*
*  Print all samples from Analysis Window
*
*/
#ifndef __C_PRINT_OUT_ANALYSIS_H__
#define __C_PRINT_OUT_ANALYSIS_H__

#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"
#include "CPrintOut.h"

class COARSample;
class COARfile;
class CFrameAnalysis;
class wxDC;

class CPrintOutAnalysis : public CPrintOut
{
public:
  CPrintOutAnalysis(CFrameAnalysis *pFrame, bool bPreview = false);
  virtual ~CPrintOutAnalysis();
  virtual wxFrame *GetParent();
  virtual int GetMaxPage();
  virtual bool OnPrintPage(int page);
  virtual bool HasPage(int page);
  virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

  static void DoPrintPreview(CFrameAnalysis *pFrame);

  static const std::set<const COARsample *> &GetExcludedSamples()
  {
    // everything else will be in OsrisParms
    return g_setSamplesOmitApplied;
  }
  static bool SampleIncluded(const COARsample *p)
  {
    return(g_setSamplesOmitApplied.find(p) == g_setSamplesOmitApplied.end());
  }
  static void InvalidateAll()
  {
    std::set<CPrintOutAnalysis *>::iterator itr;
    for (itr = g_setAll.begin(); itr != g_setAll.end(); ++itr)
    {
      (*itr)->InvalidateSamples();
    }
  }
  virtual void RebuildPages()
  {
    InvalidateAll();
  }
  static void SetOmittedSamples(const std::set<const COARsample*> &setOmit)
  {
    g_setSamplesOmitApplied = setOmit;
    InvalidateAll();
  }

private:
  static std::set<CPrintOutAnalysis *> g_setAll; // all instances of this class
  static std::set<const COARsample*> g_setSamplesOmitApplied;
  class CPrintPage
  {
  public:
    CPrintPage() : 
      pSample(NULL),
      nFirstChannel(0),
      nChannels(0),
      nChannelPerPage(0)
    {}
    CPrintPage(const COARsample *p, int n1, int nC, int nPg) :
      pSample(p),
      nFirstChannel(n1),
      nChannels(nC),
      nChannelPerPage(nPg)
    {
      pSample = p;
      n1 = nFirstChannel;
      nChannels = nC;
      nChannelPerPage = nPg;
         
    }
    CPrintPage(const CPrintPage &x)
    {
      (*this) = x;
    }
    CPrintPage &operator = (const CPrintPage &x)
    {
      pSample = x.pSample;
      nFirstChannel = x.nFirstChannel;
      nChannels = x.nChannels;
      nChannelPerPage = x.nChannelPerPage;
      return *this;
    }
    const COARsample *pSample;
    int nFirstChannel;
    int nChannels;
    int nChannelPerPage; // used for plot size
  };
  bool _OnPrintPage(int page);
  CFrameAnalysis *m_pFrameAnalysis;
  COARfile *m_pFile;

  // m_vPages contains information for each page.
  // it should be modified only in _DoSetupSampleList() or InvalidateSamples()
  // and should be directly accessed only from GetPages() or _SetupSampleList()
  std::vector<CPrintPage> m_vPages;
  void _DoSetupSampleList();
  void _SetupSampleList()
  {
    if (m_vPages.empty())
    {
      _DoSetupSampleList();
    }
  }
  const std::vector<CPrintPage> &GetPages()
  {
    _SetupSampleList();
    return m_vPages;
  }
public:
  void InvalidateSamples()
  {
    m_vPages.clear();
  }
  DECLARE_ABSTRACT_CLASS(CPrintOutAnalysis)
};


#endif
