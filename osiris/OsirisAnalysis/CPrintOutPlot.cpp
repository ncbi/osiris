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
*
*  FileName: CPrintOutPlot.cpp
*  Author:   Douglas Hoffman
*
*/
#include <memory>

#ifdef __WXMAC__
#include <wx/osx/printdlg.h>
#endif
#include <wx/printdlg.h>

#include "CPrintOutPlot.h"
#include "CFramePlot.h"
#include "CDialogPrintSettings.h"
#include "mainApp.h"

#define _PING_PRINT wxT("PrintPlot")
#define _PING_PRINT_PREVIEW wxT("PrintPreviewPlot")


void CPrintOutPlot::DoPrintPreview(CFramePlot *pFrame)
{
  // using the wxWidgets printing sample program
  _DoPrintPreview(
    new CPrintOutPlot(pFrame, true),
    new CPrintOutPlot(pFrame),
    _PING_PRINT_PREVIEW,
    _PING_PRINT);
}

// end static functions - begin virtual wxPrintout functions

CPrintOutPlot::~CPrintOutPlot() { ; }

wxFrame *CPrintOutPlot::GetParent()
{
  return m_pFramePlot;
}

int CPrintOutPlot::GetPlotsPerPage()
{
  return CDialogPrintSettingsSample::GetPlotsPerPage(m_pFramePlot->GetSample());
}
int CPrintOutPlot::GetMaxPage()
{
  int nPlots = (int)m_pFramePlot->GetPlotCount();
  int nPlotsPerPage = GetPlotsPerPage();
  int nRtn = (nPlots + nPlotsPerPage - 1) / nPlotsPerPage;
  return nRtn;
}
bool CPrintOutPlot::OnPrintPage(int page)
{
  bool bRtn = true;
  if (HasPage(page))
  {
    wxDC *pdc = GetDC();
    _setupPageBitmap(pdc);
    std::unique_ptr<wxBitmap> px(m_pFramePlot->CreateBitmap(
      m_resOutput.m_nWidth,
      m_resOutput.m_nHeight,
      m_resOutput.m_DPI,
      wxEmptyString,
      GetPlotsPerPage(),
      page,
      true));
    wxRect r = GetLogicalPageMarginsRect(*GetPageSetupData());
    pdc->DrawBitmap(*px, r.GetLeftTop());
  }
  else
  {
    bRtn = false;
    mainApp::LogMessageV(wxT("Invalid page number %d"), page);
  }
  return bRtn;
}

IMPLEMENT_ABSTRACT_CLASS(CPrintOutPlot, CPrintOut)