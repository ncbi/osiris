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
*  FileName: CPrintOutAnalysis.cpp
*  Author:   Douglas Hoffman
*
*  Print all samples from Analysis Window
*
*/
#include "CFrameAnalysis.h"
#include "CPrintOutAnalysis.h"
#include "COARfile.h"
#include "CPlotData.h"
#include "CPanelPlot.h"
#include "CParmOsiris.h"
#include "CDialogPrintSettings.h"
#include <memory>

#define _PING_PRINT wxT("PrintAnalysis")
#define _PING_PRINT_PREVIEW wxT("PrintPreviewAnalysis")

std::set<const COARsample*> CPrintOutAnalysis::g_setSamplesOmitApplied;
std::set<CPrintOutAnalysis *> CPrintOutAnalysis::g_setAll; // all instances of this class


CPrintOutAnalysis::CPrintOutAnalysis(CFrameAnalysis *pFrame, bool bPreview) :
  CPrintOut(bPreview),
  m_pFrameAnalysis(pFrame)
{
  m_pFile = m_pFrameAnalysis->GetOARfile();
  g_setAll.insert(this);
}
CPrintOutAnalysis::~CPrintOutAnalysis()
{
  g_setAll.erase(this);
  if (g_setAll.empty())
  {
    g_setSamplesOmitApplied.clear();
  }
}

int CPrintOutAnalysis::GetMaxPage()
{
  _SetupSampleList();
  return (int)GetPages().size();
}

void CPrintOutAnalysis::_DoSetupSampleList()
{
  CParmOsirisGlobal pParm;

  // set up flag before gettings list from analysis window
  struct INCLUDE_TYPE
  {
    bool bType;
    int nFLag;
  } TYPES[] =
  {
    {pParm->GetPrintSamplesLadders(), CFrameAnalysis::INCLUDE_LADDER},
    {pParm->GetPrintSamplesDisabled(), CFrameAnalysis::INCLUDE_DISABLED},
    {pParm->GetPrintSamplesNegCtrl(), CFrameAnalysis::INCLUDE_NEG_CTRL},
    {pParm->GetPrintSamplesPosCtrl(), CFrameAnalysis::INCLUDE_POS_CTRL}
  };
  size_t SIZE_TYPE = sizeof(TYPES) / sizeof(TYPES[0]);
  int nFlag = 0;
  for (size_t i = 0; i < SIZE_TYPE; ++i)
  {
    if (TYPES[i].bType)
    {
      nFlag |= TYPES[i].nFLag;
    }
  }
  std::vector<const COARsample *> vTemp;
  std::vector<const COARsample *>::iterator itr;
  m_pFrameAnalysis->GetSamplesByRow(&vTemp, nFlag);

  // copy list without omitted samples
  std::vector<const COARsample *> vSamples;
  size_t nSizeOmit = g_setSamplesOmitApplied.size();
  size_t nSizeList = vTemp.size();
  if (!nSizeOmit)
  {
    // nothing is omitted
    vSamples = vTemp;
  }
  else
  {
    // copy without omitted samples
    vSamples.reserve((nSizeList > nSizeOmit) ? (nSizeList - nSizeOmit) : nSizeList);
    for (itr = vTemp.begin(); itr != vTemp.end(); ++itr)
    {
      if(SampleIncluded(*itr))
      {
        vSamples.push_back(*itr);
      }
    }
  }
  // compute page count and info

  int nChannels = (int)m_pFile->GetChannelCount() + int(!pParm->GetPrintChannelsOmitILS());
  int nChannelSample = pParm->GetPrintChannelsSamples();
  int nTotalChannels;
  int nFirstChannel;
  if ( (!nChannelSample) || (nChannelSample > nChannels) )
  {
    nChannelSample = nChannels;
  }
  int nChannelLadder =
    (nFlag & CFrameAnalysis::INCLUDE_LADDER)
    ? pParm->GetPrintChannelsLadders() 
    : nChannelSample;
  int nChannelNegCtrl =
    (nFlag & CFrameAnalysis::INCLUDE_NEG_CTRL)
    ? pParm->GetPrintChannelsNegCtrl()
    : nChannelSample;
  int nPageChannels = nChannelSample;
  const COARsample *pSample;
  if (nChannelLadder > nChannels) nChannelLadder = nChannels;
  if (nChannelNegCtrl > nChannels) nChannelNegCtrl = nChannels;
  bool bAllSame = (nChannelSample == nChannelLadder) &&
    (nChannelSample == nChannelNegCtrl);
  m_vPages.empty();
  m_vPages.reserve(((nChannelSample + nChannels - 1) / nChannelSample) * vSamples.size());
  for (itr = vSamples.begin(); itr != vSamples.end(); ++itr)
  {
    pSample = *itr;
    // figure out channels per page
    if (bAllSame) {} // no check needed
    else if (pSample->IsLadderType())
    {
      nPageChannels = nChannelLadder;
    }
    else if (pSample->IsNegControl())
    {
      nPageChannels = nChannelNegCtrl;
    }
    else
    {
      nPageChannels = nChannelSample;
    }
    nFirstChannel = 1;
    nTotalChannels = nChannels;
#define myMIN(a,b) (a < b) ? a : b
    for (nTotalChannels = nChannels; nTotalChannels > 0; nTotalChannels -= nPageChannels)
    {
      CPrintPage x(pSample, nFirstChannel,
        myMIN(nTotalChannels, nPageChannels),
        nPageChannels);
      m_vPages.push_back(x);
      nFirstChannel += nPageChannels;
    }
#undef myMIN       
  }
}

wxFrame *CPrintOutAnalysis::GetParent()
{
  return m_pFrameAnalysis;
}
bool CPrintOutAnalysis::OnPrintPage(int page)
{
  bool bRtn = true;
  if (page > 0)
  {

    bRtn = GetPages().size() ? _OnPrintPage(page) : false;
  }
  else
  {
    // in preview, 0 is a blank page
    bRtn = IsPreview();
  }
  return bRtn;
}
bool CPrintOutAnalysis::_OnPrintPage(int page)
{
  const CPrintPage &printPage(GetPages().at(page - 1));
  const COARsample *pSample(printPage.pSample);
  const wxString sPlotFile = pSample->GetPlotFileName();
  CPlotData plotData;
  wxDC *pdc = GetDC();
  _setupPageBitmap(pdc);
  if( (!sPlotFile.IsEmpty()) && plotData.LoadFile(sPlotFile) )
  {
    CPanelPlot *panel = new CPanelPlot(
      m_pFrameAnalysis,
      &plotData, m_pFile);
    CWindowPointer pw(panel);
    panel->Show(false);
    panel->SetRenderingToWindow(false);
    panel->SetPrintSettings();
    wxBitmap *pBitmap = panel->CreateMultiChannelBitmap(
      m_resOutput.m_nWidth,
      m_resOutput.m_nHeight,
      m_resOutput.m_DPI,
      printPage.nFirstChannel,
      page,
      true);
    std::unique_ptr<wxBitmap> px(pBitmap);
    wxRect r = GetLogicalPageMarginsRect(*GetPageSetupData());
    pdc->DrawBitmap(*pBitmap, r.GetLeftTop());
  }
  else
  {
    // could not open plt file, show a message

    wxString sTitle =
      (m_pFrameAnalysis->GetSampleNameLabelType() == IDmenuDisplayNameSample)
      ? pSample->GetSampleName()
      : pSample->GetName();
    int nPointSize = 48;
    wxFont font(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
      wxFONTWEIGHT_BOLD, false);
    pdc->SetBackground(*wxWHITE_BRUSH);
    pdc->Clear();
    wxString sMsg(wxT("Cannot find plot file for "));
    sMsg.Append(sTitle);
    pdc->SetFont(font);
    wxSize sz = pdc->GetTextExtent(sTitle);
    wxSize szDC = pdc->GetSize();
    if (sz.GetWidth() > szDC.GetWidth())
    {
      font.SetPointSize(int(double(nPointSize * szDC.GetWidth()) / sz.GetWidth()));
      sz = pdc->GetTextExtent(sTitle);
      pdc->SetFont(font);
    }
    wxPoint pt(
      (szDC.GetWidth() - sz.GetWidth()) >> 1,
      (szDC.GetHeight() - sz.GetHeight()) >> 1);
    if (pt.x < 0) { pt.x = 0; }
    if (pt.y < 0) { pt.y = 0; }
    pdc->DrawText(sMsg, pt);
    mainApp::LogMessage(sMsg);
  }
  return true;
}

void CPrintOutAnalysis::DoPrintPreview(CFrameAnalysis *pFrame)
{
  // using the wxWidgets printing sample program
  _DoPrintPreview(
    new CPrintOutAnalysis(pFrame, true),
    new CPrintOutAnalysis(pFrame),
    _PING_PRINT_PREVIEW,
    _PING_PRINT,
    true
  );
}
IMPLEMENT_ABSTRACT_CLASS(CPrintOutAnalysis, CPrintOut)
