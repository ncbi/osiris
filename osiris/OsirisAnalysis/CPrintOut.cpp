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
*  FileName: CPrintOut.cpp
*  Author:   Douglas Hoffman
*
*/
#include <memory>

#ifdef __WXMAC__
#include <wx/osx/printdlg.h>
#endif
#include <wx/printdlg.h>

#include "CPrintOut.h"
#include "mainApp.h"
#include "CFramePlot.h"
#include "CParmOsiris.h"


// CPrintPreviewFrame

CPrintPreviewFrame::CPrintPreviewFrame(wxPrintPreview *pPrev, wxFrame *parent,
  const wxString &title, bool bPageButtons) :
  wxPreviewFrame(pPrev, parent, title,
    GET_PERSISTENT_POSITION(CPrintPreviewFrame),
    GET_PERSISTENT_SIZE_DEFAULT(CPrintPreviewFrame, wxSize(600, 800))),
  m_bPageButtons(bPageButtons)
{
  InitializeWithModality(wxPreviewFrame_AppModal);
}
void CPrintPreviewFrame::CreateControlBar()
{
  // copied from wxWidgets prntbase.cpp and modified
  // to change buttons
  // and should be checked when updating version of wx
  long buttons = m_bPageButtons ? wxPREVIEW_DEFAULT : wxPREVIEW_ZOOM;
  if (m_printPreview->GetPrintoutForPrinting())
    buttons |= wxPREVIEW_PRINT;

  m_controlBar = new wxPreviewControlBar(m_printPreview, buttons, this);
  m_controlBar->CreateButtons();
}

IMPLEMENT_ABSTRACT_CLASS(CPrintPreviewFrame, wxPreviewFrame)
IMPLEMENT_PERSISTENT_SIZE_POSITION(CPrintPreviewFrame)
BEGIN_EVENT_TABLE(CPrintPreviewFrame, wxPreviewFrame)
EVT_PERSISTENT_SIZE_POSITION(CPrintPreviewFrame)
END_EVENT_TABLE()


bool CPrintPreview::Print(bool interactive)
{
  bool bRtn = wxPrintPreview::Print(interactive);
  const wxChar *psStatus = bRtn ? wxT("OK") : wxT("NOT_OK");
  mainApp::Ping3(PING_EVENT, m_sPrintType, wxT("Status"), psStatus, wxT("FromPreview"), wxT("1"));
  return bRtn;
}
void CPrintPreview::SetZoom(int n)
{
  CParmOsirisGlobal parm;
  parm->SetPrintPreviewZoom(n);
  wxPrintPreview::SetZoom(n);
}

void CPrintPreview::_SetDefaultZoom()
{
  CParmOsirisGlobal parm;
  int n = parm->GetPrintPreviewZoom();
  if (n > 10)
  {
    wxPrintPreview::SetZoom(n);
  }
}



// static data and functions


wxPrintData *CPrintOut::g_printData = NULL;
wxPageSetupDialogData* CPrintOut::g_pageSetupData = NULL;
const int CPrintOut::MAX_PPI = 600;

wxPrintData *CPrintOut::GetPrintData()
{
  if (g_printData == NULL)
  {
    CParmOsiris *pParm = CParmOsiris::GetGlobal();
    g_printData = new wxPrintData();
    g_printData->SetOrientation(
      pParm->GetPrintPlotLandscape() ? wxLANDSCAPE : wxPORTRAIT
    );
    int nPaperID = pParm->GetPrintPlotPaperType();
    bool bSetPaperSize = true;
    if (nPaperID >= 0)
    {
      wxPaperSize nID = wxPaperSize(nPaperID);
      g_printData->SetPaperId(nID);
      bSetPaperSize = (nID == wxPAPER_NONE);
    }
    if(bSetPaperSize)
    {
      int nw = pParm->GetPrintPlotPaperWidth();
      int nh = pParm->GetPrintPlotPaperHeight();
      if (nw > 0 && nh > 0)
      {
        g_printData->SetPaperSize(wxSize(nw, nh));
      }
    }
  }
  return g_printData;
}
  
wxPageSetupDialogData *CPrintOut::GetPageSetupData()
{
  if(g_pageSetupData == NULL)
  {
    CParmOsiris *pParm = CParmOsiris::GetGlobal();
    g_pageSetupData = new wxPageSetupDialogData(*GetPrintData());
    // set saved margins, to do save/retrieve paper ID or sice
    g_pageSetupData->SetMarginTopLeft(
      wxPoint(
        pParm->GetPrintPlotMarginLeft(),
        pParm->GetPrintPlotMarginTop()
      ));
    g_pageSetupData->SetMarginBottomRight(
        wxPoint(
          pParm->GetPrintPlotMarginRight(),
          pParm->GetPrintPlotMarginBottom()
        ));
    *g_pageSetupData = *GetPrintData();
  }
  return g_pageSetupData;
}
void CPrintOut::UpdatePageSetup()
{
  // update page setup in user osiris parameters
  CParmOsirisGlobal pParm;
  if (g_pageSetupData != NULL)
  {
    wxPoint pt;
    pt = g_pageSetupData->GetMarginTopLeft();
    pParm->SetPrintPlotMarginLeft((unsigned int)pt.x);
    pParm->SetPrintPlotMarginTop((unsigned int)pt.y);
    pt = g_pageSetupData->GetMarginBottomRight();
    pParm->SetPrintPlotMarginRight((unsigned int)pt.x);
    pParm->SetPrintPlotMarginBottom((unsigned int)pt.y);
  }
  if (g_printData != NULL)
  {
    wxSize szPaperSize(-1, -1);
    wxPaperSize nPaperID = g_printData->GetPaperId();
    pParm->SetPrintPlotPaperType(int(nPaperID));
    if (nPaperID == wxPAPER_NONE)
    {
      szPaperSize = g_printData->GetPaperSize();
    }
    pParm->SetPrintPlotPaperWidth(szPaperSize.GetWidth());
    pParm->SetPrintPlotPaperHeight(szPaperSize.GetHeight());
    pParm->SetPrintPlotLandscape(
      g_printData->GetOrientation() == wxLANDSCAPE);
  }
}
void CPrintOut::UpdatePageSetupData(wxPrintData *pPrintData, wxPageSetupDialogData *pSetupData)
{
  bool bUpdate = false;
  if (pPrintData != NULL)
  {
    bUpdate = true;
    *(GetPrintData()) = *pPrintData;
  }
  if (pSetupData != NULL)
  {
    bUpdate = true;
    *(GetPageSetupData()) = *pSetupData;
  }
  if (bUpdate)
  {
    UpdatePageSetup(); // save user settings
  }
}

void CPrintOut::DoPageSetup(wxFrame *pParent)
{
  wxPageSetupDialogData *pSetupData = GetPageSetupData();
  wxPrintData *pPrintData = GetPrintData();
  *pSetupData = *pPrintData;

  wxPageSetupDialog pageSetupDialog(pParent, pSetupData);
  pageSetupDialog.ShowModal();
  UpdatePageSetupData(
    &pageSetupDialog.GetPageSetupDialogData().GetPrintData(),
    &pageSetupDialog.GetPageSetupDialogData()
  );

  mainApp::Ping(PING_EVENT, wxT("PrintPageSetup"));
}

#ifdef __WXMAC__
void CPrintOut::DoPageMargins(wxFrame *parent)
{
  wxPageSetupDialogData *pSetupData = GetPageSetupData();
  wxPrintData *pPrintData = GetPrintData();
  *pSetupData = *pPrintData;

  wxMacPageMarginsDialog pageMarginsDialog(parent, g_pageSetupData);
  pageMarginsDialog.ShowModal();

  UpdatePageSetupData(
    &pageMarginsDialog.GetPageSetupDialogData().GetPrintData(),
    &pageMarginsDialog.GetPageSetupDialogData()
  );
}
#endif

void CPrintOut::_DoPrint(CPrintOut *pPrintout, const wxString &sPingType)
{
  wxPrintDialogData printDialogData(*GetPrintData());

  wxPrinter printer(&printDialogData);
  wxString sStatus;
  if (printer.Print(pPrintout->GetParent(), pPrintout, true /*prompt*/))
  {
    // OK
    UpdatePageSetupData(
      &(printer.GetPrintDialogData().GetPrintData()),
      NULL
    );
    sStatus = wxT("OK");
  }
  else if (wxPrinter::GetLastError() == wxPRINTER_ERROR)
  {
    // error
    mainApp::LogMessage(wxT("Printer error"));
    sStatus = wxT("ERROR");
  }
  else
  {
    // printout canceled
    sStatus = wxT("CANCEL");
  }
  mainApp::Ping2(PING_EVENT, sPingType, wxT("Status"), sStatus);
}

void CPrintOut::_DoPrintPreview(
  CPrintOut *pPreview,
  CPrintOut *pPrint,
  const wxString &sPingPreview,
  const wxString &sPingPrint,
  bool bPageButtons)
{
  wxPrintDialogData printDialogData(*GetPrintData());
  wxPrintPreview *preview =
    new CPrintPreview(
      sPingPrint,
      pPreview,
      pPrint,
      &printDialogData);
  wxString sStatus;
  CPrintPreviewFrame *pFrame = NULL;
  if (!preview->IsOk())
  {
    delete preview;
    mainApp::ShowError(
      wxT("There was a problem with print preview.\nPlease check your printer setup."),
      pPreview->GetParent());
    sStatus = wxT("ERROR");
  }
  else
  {
    sStatus = wxT("OK");
    pFrame =
      new CPrintPreviewFrame(preview, pPreview->GetParent(), sPingPreview, bPageButtons);
  }
  mainApp::Ping2(PING_EVENT, sPingPreview, wxT("Status"), sStatus);
  if (pFrame != NULL)
  {
    // "Ping" before show
    pFrame->Show();
  }
}




// end static functions

CPrintOut::~CPrintOut() 
{
#ifdef TMP_DEBUG
  if (m_nSetupPageCount > 1)
  {
    mainApp::LogMessageV(wxT("**** CPrintOut setup count = %d"), m_nSetupPageCount);
  }
#endif
}

void CPrintOut::_setupPageBitmap(wxDC *pdc)
{
  _resInput res(pdc->GetPPI(), GetLogicalPageMarginsRect(*GetPageSetupData()));
  if (res != m_resInput)
  {
    // compute everything
#ifdef TMP_DEBUG
    m_nSetupPageCount++;
#endif
    wxSize szPPI = res.m_ppi;
    wxRect rectFit = res.m_logicalPage;
    double dPPIscale = 1.0, dScalePixel = 1.0;
    int nMaxPPI = MAX_PPI;
    enum
    {
      SCALE_NONE,
      SCALE_X,
      SCALE_Y
    } nScale = SCALE_NONE;
    int nPPIx, nPPIy, nMinPPI, nUsePPI, nX, nY;
    bool bFit = false;

    m_resInput = res;
    nPPIx = szPPI.GetWidth();
    nPPIy = szPPI.GetHeight();
    if (nPPIx == nPPIy)
    {
      nMinPPI = nPPIx;
    }
    else if (nPPIx < nPPIy)
    {
      nMinPPI = nPPIx;
      nScale = SCALE_Y;
      dScalePixel = double(nPPIx) / double(nPPIy);
    }
    else
    {
      nMinPPI = nPPIy;
      nScale = SCALE_X;
      dScalePixel = double(nPPIy) / double(nPPIx);
    }
    if (IsPrintPreview())
    {
      int nx, ny;
      GetPPIScreen(&nx, &ny);
      nMaxPPI = (nx < ny) ? nx : ny;
      if (nMaxPPI < 48)
      {
        nMaxPPI = 48;
      }
#ifdef TMP_DEBUG
      mainApp::LogMessageV(
        wxT("nMaxPPI = %d; printer PPI (x, y)=(%d, %d); screen PPI (x, y)=(%d, %d)"),
        nMaxPPI, nPPIx, nPPIy, nx, ny);
      mainApp::LogMessageV(
        wxT("rectFix(x, y, w, h) = (%d, %d, %d, %d)"),
        rectFit.GetX(), rectFit.GetY(),
        rectFit.GetWidth(), rectFit.GetHeight());
      double dx, dy;
      pdc->GetLogicalScale(&dx, &dy);
      mainApp::LogMessageV(wxT("wxDC logical scale (x, y)=(%g, %g)"),
        dx, dy);
      pdc->GetUserScale(&dx, &dy);
      mainApp::LogMessageV(wxT("wxDC user scale (x, y)=(%g, %g)"),
        dx, dy);
      pdc->GetSize(&nx, &ny);
      mainApp::LogMessageV(wxT("wxDC size (x, y)=(%d, %d)"),
        nx, ny);
      mainApp::LogMessageV(wxT("wxDC PPI (x, y)=(%d, %d)"),
        nPPIx, nPPIy);
#endif
    }

    if (nMinPPI > nMaxPPI)
    {
      bFit = true;
      nUsePPI = nMaxPPI;
      dPPIscale = nMaxPPI / float(nMinPPI);
      nX = int(rectFit.GetWidth() * dPPIscale);
      nY = int(rectFit.GetHeight() * dPPIscale);
    }
    else
    {
      nUsePPI = nMinPPI;
      nX = rectFit.GetWidth();
      nY = rectFit.GetHeight();
    }
    switch (nScale)
    {
    case SCALE_NONE:
      break;
    case SCALE_X:
      nX = int(dScalePixel * nX);
      bFit = true;
      break;
    case SCALE_Y:
      nY = int(dScalePixel * nY);
      bFit = true;
      break;
    }
    m_resOutput.m_bFit = bFit;
    m_resOutput.m_DPI = nUsePPI;
    m_resOutput.m_nWidth = nX;
    m_resOutput.m_nHeight = nY;
  }
  if (m_resOutput.m_bFit)
  {
    FitThisSizeToPageMargins(wxSize(m_resOutput.m_nWidth, m_resOutput.m_nHeight), *GetPageSetupData());
  }
}
