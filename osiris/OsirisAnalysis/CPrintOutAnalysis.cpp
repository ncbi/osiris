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
//#include <wx/bitmap.h>
#include <memory>

#define _PING_PRINT wxT("PrintAnalysis")
#define _PING_PRINT_PREVIEW wxT("PrintPreviewAnalysis")

CPrintOutAnalysis::CPrintOutAnalysis(CFrameAnalysis *pFrame, bool bPreview) :
  CPrintOut(bPreview),
  m_pFrameAnalysis(pFrame)
{
  m_pFile = m_pFrameAnalysis->GetOARfile();
  m_pFrameAnalysis->GetSamplesByRow(&m_vSamples);
}

wxFrame *CPrintOutAnalysis::GetParent()
{
  return m_pFrameAnalysis;
}
bool CPrintOutAnalysis::OnPrintPage(int page)
{
  const COARsample *pSample = m_vSamples.at(page - 1);
  const wxString sPlotFile = pSample->GetPlotFileName();
  CPlotData plotData;
  wxString sTitle =
    (m_pFrameAnalysis->GetSampleNameLabelType() == IDmenuDisplayNameSample)
    ? pSample->GetSampleName()
    : pSample->GetName();
  wxDC *pdc = GetDC();
  _setupPageBitmap(pdc);
  if( (!sPlotFile.IsEmpty()) && plotData.LoadFile(sPlotFile) )
  {
    CPanelPlot *panel = new CPanelPlot(m_pFrameAnalysis, m_pFrameAnalysis, &plotData, m_pFile, mainApp::GetKitColors(), true);
    CWindowPointer pw(panel);
    panel->Show(false);
    panel->SetRenderingToWindow(false);
    wxBitmap *pBitmap = panel->CreateAllChannelBitmap(
      m_resOutput.m_nWidth,
      m_resOutput.m_nHeight,
      m_resOutput.m_DPI,
      sTitle , true);
    std::unique_ptr<wxBitmap> px(pBitmap);
    wxRect r = GetLogicalPageMarginsRect(*GetPageSetupData());
    pdc->DrawBitmap(*pBitmap, r.GetLeftTop());
  }
  else
  {
    // could not open plt file, show a message

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
bool CPrintOutAnalysis::HasPage(int page)
{
  return (page >= 0) && (page <= (int) m_vSamples.size());
}
void CPrintOutAnalysis::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
{
  // TEST to see if it is responsible for printing selected pages.
  *minPage = 1;
  *selPageFrom = 1;
  *maxPage = m_vSamples.size();
  *selPageTo = *maxPage;
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
