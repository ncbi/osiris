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
*  FileName: CPrintPreview.h
*  Author:   Douglas Hoffman
*
*  CPrintPreview implements wxPrintPreview
*  CPrintPreviewFrame implement wxPrintPreviewFrame
*/
#include <wx/arrstr.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/valnum.h>
#include "CPrintPreview.h"
#include "CPrintOut.h"
#include "CFramePlot.h"
#include "CFrameAnalysis.h"
#include "CParmOsiris.h"
#include "CDialogPrintSettings.h"
#include "CDialogPrintColor.h"
#include "nwx/nwxString.h"

// CPrintPreviewFrame

CPrintPreviewFrame::CPrintPreviewFrame(CPrintPreview *pPrev, wxWindow *parent,
  const wxString &title, bool bPageButtons) :
  wxPreviewFrame(pPrev, parent, title,
    GET_PERSISTENT_POSITION(CPrintPreviewFrame),
    GET_PERSISTENT_SIZE_DEFAULT(CPrintPreviewFrame, wxSize(600, 800))),
  m_pPreview(pPrev),
  m_pDialogSettings(NULL)
{
  m_pFrameAnalysis = wxDynamicCast(parent, CFrameAnalysis);
  m_pFramePlot = wxDynamicCast(parent, CFramePlot);
  InitializeWithModality(wxPreviewFrame_AppModal);
  wxStatusBar *pStat = GetStatusBar();
  if (pStat != NULL)
  {
    pStat->Hide();
    Layout();
  }
}
void CPrintPreviewFrame::CreateControlBar()
{
  // copied from wxWidgets prntbase.cpp and modified
  // to change buttons
  // and should be checked when updating version of wx
  m_controlBar = new CPreviewControlBar(this, wxDynamicCast(m_printPreview, CPrintPreview));
  m_controlBar->CreateButtons();
}
void CPrintPreviewFrame::_OnColors(wxCommandEvent &)
{
  CDialogPrintColor dlg(this);
  if (dlg.ShowModal() == wxID_OK)
  {
    CPreviewControlBar *pController = _GetControlBar();
    int nPage = pController->GetPage();
    m_pPreview->SetCurrentPage(0); // show blank page in order to refresh current page
    m_pPreview->SetCurrentPage(nPage);
  }
}
void CPrintPreviewFrame::_OnSettings(wxCommandEvent &)
{
  if (m_pDialogSettings == NULL)
  {
    if (m_pFrameAnalysis != NULL)
    {
      m_pDialogSettings = new CDialogPrintSettings(this, m_pFrameAnalysis);
      mainApp::Ping(PING_EVENT, wxT("PrintSettingsCreate"));
    }
    else
    {
      m_pDialogSettings = new CDialogPrintSettingsSample(this, m_pFramePlot->GetSample());
      mainApp::Ping(PING_EVENT, wxT("PrintSettingsSampleCreate"));
    }
    m_pDialogSettings->Show();
  }
  else if (!m_pDialogSettings->IsShown())
  {
    m_pDialogSettings->TransferDataToWindow();
    m_pDialogSettings->Show();
    const wxChar *p = (m_pFrameAnalysis != NULL)
      ? wxT("PrintSettingsShow") : wxT("PrintSettingsSampleShow");
    mainApp::Ping(PING_EVENT, p);
  }
  if (m_pDialogSettings != NULL)
  {
    m_pDialogSettings->Raise();
  }
}
void CPrintPreviewFrame::_OnZoom(wxCommandEvent &)
{
  m_pPreview->SetZoom(_GetControlBar()->GetZoom());
}
void CPrintPreviewFrame::_OnPageChange(wxCommandEvent &)
{
  m_pPreview->SetCurrentPage(_GetControlBar()->GetPage());
}
void CPrintPreviewFrame::_OnPrint(wxCommandEvent &)
{
  m_pPreview->Print(true);
  // once the PrintOut object is used (for printing), it needs to be destroyed
  // because otherwise print settings can change the number of pages
  // causing problems with reporting pages being printed
  // Closing this window takes care of this issue
  Close();
}
void CPrintPreviewFrame::UpdateSettings()
{
  wxDynamicCast(m_pPreview->GetPrintout(), CPrintOut)->RebuildPages();
  CPreviewControlBar *pController = _GetControlBar();
  int nPageBefore = pController->GetPage();
  _GetControlBar()->SetPageCount(m_pPreview->GetMaxPage());
  int nPageAfter = pController->GetPage();
  if (nPageAfter == nPageBefore)
  {
    // force a page refresh by printing page 0 (blank)
    // followed by returning to current page
    m_pPreview->SetCurrentPage(0);
  }
  m_pPreview->SetCurrentPage(nPageAfter);
}

IMPLEMENT_ABSTRACT_CLASS(CPrintPreviewFrame, wxPreviewFrame)
IMPLEMENT_PERSISTENT_SIZE_POSITION(CPrintPreviewFrame)
BEGIN_EVENT_TABLE(CPrintPreviewFrame, wxPreviewFrame)
EVT_BUTTON(IDprintSettings, CPrintPreviewFrame::_OnSettings)
EVT_BUTTON(IDprintColors, CPrintPreviewFrame::_OnColors)
EVT_BUTTON(IDprintPrint, CPrintPreviewFrame::_OnPrint)
EVT_CHOICE(IDprintZoom, CPrintPreviewFrame::_OnZoom)
EVT_TEXT(IDprintPageText, CPrintPreviewFrame::_OnPageChange)
EVT_PERSISTENT_SIZE_POSITION(CPrintPreviewFrame)
END_EVENT_TABLE()


int CPrintPreview::GetMaxPage() const
{
  CPrintOut *p = wxDynamicCast(GetPrintout(), CPrintOut);
  int nRtn = (p == NULL) ? wxPrintPreview::GetMaxPage() : p->GetMaxPage();
  return nRtn;
}


bool CPrintPreview::Print(bool interactive)
{
  bool bRtn = wxPrintPreview::Print(interactive);
  const wxChar *psStatus = bRtn ? wxT("OK") : wxT("NOT_OK");
  mainApp::Ping2(PING_EVENT, m_sPrintType, wxT("Status"), psStatus);
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
  if (n >= 10)
  {
    wxPrintPreview::SetZoom(n);
  }
}

// CPreviewControlBar

#define S_SETTINGS wxT("Settings...")
#define S_COLORS wxT("Colors...")
#define S_PRINT wxT("Print...")
#define S_PAGE wxT("Page:")
#define S_PAGE_COUNT \
  nwxString::FormatNumber(m_nPageCount).Append( \
      (m_nPageCount == 1) ? wxT(" page") : wxT(" pages"))
#define S_ZOOM wxT("Zoom:")
#define S_TT_PRINT wxT("Print...")
#define S_TT_FIRST wxT("Show first page")
#define S_TT_PREV wxT("Show previous page")
#define S_TT_NEXT wxT("Show next page")
#define S_TT_LAST wxT("Show last page")
#define S_TT_ZOOM_0 wxT("Decrease preview size")
#define S_TT_ZOOM_1 wxT("Increase preview size")



wxArrayString CPreviewControlBar::g_asZoomChoices;
const int CPreviewControlBar::g_anZoomChoices[5] = { 50, 75, 100, 150, 200};

void CPreviewControlBar::_SetupZoomChoices()
{
  if (g_asZoomChoices.IsEmpty())
  {
    size_t n = sizeof(g_anZoomChoices) / sizeof(g_anZoomChoices[0]);
    const int *pn = g_anZoomChoices;
    for (size_t i = 0; i < n; ++i)
    {
      g_asZoomChoices.Add(nwxString::FormatNumber(*pn++));
    }
  }
}
int CPreviewControlBar::_GetZoomIndex(int nPct)
{
  size_t n = sizeof(g_anZoomChoices) / sizeof(g_anZoomChoices[0]);
  const int *pn = g_anZoomChoices;
  int nRtn = 2; // 100%
  for (size_t i = 0; i < n; ++i)
  {
    if ((*pn) >= nPct)
    {
      nRtn = i;
      break;
    }
    ++pn;
  }
  return nRtn;
}

CPreviewControlBar::CPreviewControlBar(
  CPrintPreviewFrame *parent, CPrintPreview *preview) :
    wxPreviewControlBar(preview, 0, parent),
    m_pTextPage(NULL),
    m_pTextPageCount(NULL),
    m_pListZoom(NULL),
    m_pButtonFirst(NULL),
    m_pButtonPrev(NULL),
    m_pButtonNext(NULL),
    m_pButtonLast(NULL),
    m_nPageCount(preview->GetMaxPage())
{
  _SetupZoomChoices();
}

CPreviewControlBar::~CPreviewControlBar() {}

wxButton *CPreviewControlBar::_CreateButton(
  int nID, const wxString &sLabel, const wxString &sToolTip, const wxFont &f)
{
  wxButton *pButton = new wxButton(
    this, nID, sLabel, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
  pButton->SetFont(f);
  pButton->SetToolTip(sToolTip);
  return pButton;
}


void CPreviewControlBar::CreateButtons()
{
// https://en.wikipedia.org/wiki/Geometric_Shapes
#define TRIANGLE_LEFT 9664
#define TRIANGLE_RIGHT 9654

  // UTF-8 printer character.  Since wchar_t can be UTF-16 or 32 bits,
  // use UTF-8 for compatibility
  // source: https://www.fileformat.info/info/unicode/char/1f5b6/index.htm
  //const unsigned char PRINTER_UTF_8[] = { 0xF0, 0x9F, 0x96, 0xB6, 0 };
  // commented out because it is ugly
  const wxChar asButtonFirst[] = { TRIANGLE_LEFT, TRIANGLE_LEFT, 0 };
  const wxChar asButtonPrev[] = { 160, TRIANGLE_LEFT, 160, 0 };
  const wxChar asButtonLast[] = { TRIANGLE_RIGHT, TRIANGLE_RIGHT, 0 };
  const wxChar asButtonNext[] = { 160, TRIANGLE_RIGHT, 160, 0 };
  wxPreviewControlBar::CreateButtons();
  wxButton *pButton;
  wxStaticText *pText;
  wxSizer *pSizer = this->GetSizer();
  wxSizerFlags flags(0);
  flags.Border(wxALL).Center();

  pButton = new wxButton(this, IDprintColors, S_COLORS);
  pSizer->Insert(
    pSizer->GetItemCount() - 1,
    pButton,
    flags);

  pButton = new wxButton(this, IDprintSettings, S_SETTINGS);
  pSizer->Insert(
    pSizer->GetItemCount() - 1,
    pButton,
    flags);

  int nSpacer = wxSizerFlags::GetDefaultBorder();
  int nPOS = 0; // sizer insert position

  wxFont fnt = pButton->GetFont();
  fnt.MakeBold();

  flags.Border(wxLEFT | wxTOP | wxBOTTOM);
  pButton = new wxButton(this, IDprintPrint, S_PRINT,
    wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
  pSizer->Insert(nPOS++, pButton, flags);
  pSizer->InsertSpacer(nPOS++, nSpacer << 1);

  pText = new wxStaticText(this, wxID_ANY, S_PAGE);
  wxFont fntText = pText->GetFont();
  fntText.MakeBold();
  pText->SetFont(fntText);
  pSizer->Insert(nPOS++, pText, flags);
  m_pButtonFirst = _CreateButton(IDprintPageFirst, asButtonFirst, S_TT_FIRST, fnt);
  pSizer->Insert(nPOS++, m_pButtonFirst, flags);
  m_pButtonPrev = _CreateButton(IDprintPagePrev, asButtonPrev, S_TT_PREV, fnt);
  pSizer->Insert(nPOS++, m_pButtonPrev, flags);

  wxIntegerValidator<int> val(NULL, wxNUM_VAL_ZERO_AS_BLANK);
  val.SetRange(1, m_nPageCount);
  wxSize sz = pText->GetTextExtent(wxT("99999"));
  sz.SetHeight(-1);
  m_pTextPage = new wxTextCtrl(this, IDprintPageText, wxT("1"),
    wxDefaultPosition, sz, wxTE_RIGHT, val);
  pSizer->Insert(nPOS++, m_pTextPage, flags);

  m_pButtonNext = _CreateButton(IDprintPageNext, asButtonNext, S_TT_NEXT, fnt);
  pSizer->Insert(nPOS++, m_pButtonNext, flags);
  m_pButtonLast = _CreateButton(IDprintPageLast, asButtonLast, S_TT_LAST, fnt);
  pSizer->Insert(nPOS++, m_pButtonLast, flags);

  m_pTextPageCount = new wxStaticText(this, wxID_ANY, S_PAGE_COUNT);
  m_pTextPageCount->SetFont(fntText);
  pSizer->Insert(nPOS++, m_pTextPageCount, flags);

  pSizer->InsertSpacer(nPOS++, nSpacer << 1);
  pText = new wxStaticText(this, wxID_ANY, S_ZOOM);
  pText->SetFont(fntText);
  pSizer->Insert(nPOS++, pText, flags);

  CParmOsirisGlobal pParm;
  m_pListZoom = new wxChoice(
    this, IDprintZoom, wxDefaultPosition, wxDefaultSize, g_asZoomChoices);
  m_pListZoom->Select(_GetZoomIndex(pParm->GetPrintPreviewZoom()));
  pSizer->Insert(nPOS++, m_pListZoom, flags);
  pText = new wxStaticText(this, wxID_ANY, wxT("%"));
  pText->SetFont(fntText);
  pSizer->Insert(nPOS++, pText, flags);
  _UpdatePageButtons(1);
}

void CPreviewControlBar::SetPageCount(int n)
{
  m_nPageCount = n < 1 ? 1 : n;
  int nPage = GetPage();
  if (nPage > m_nPageCount)
  {
    SetPage(m_nPageCount);
    nPage = m_nPageCount;
  }
  _UpdatePageButtons(nPage);
  wxIntegerValidator<int> *pVal
      ((wxIntegerValidator<int> *) m_pTextPage->GetValidator());
  pVal->SetMax(m_nPageCount);
  m_pTextPageCount->SetLabel(S_PAGE_COUNT);
}
void CPreviewControlBar::_UpdatePageButtons(int n)
{
  int nPage = (n < 1) ? GetPage() : n;
  bool bEnableNext = nPage < GetPageCount();
  bool bEnablePrev = nPage > 1;
  m_pButtonFirst->Enable(bEnablePrev);
  m_pButtonPrev->Enable(bEnablePrev);
  m_pButtonNext->Enable(bEnableNext);
  m_pButtonLast->Enable(bEnableNext);
}
void CPreviewControlBar::SetPage(int n)
{
  int nCount = GetPageCount();
  bool bEnableNext = true;
  bool bEnablePrev = true;
  if (n >= nCount)
  {
    n = nCount;
    bEnableNext = false;
  }
  if (n <= 1)
  {
    n = 1;
    bEnablePrev = false;
  }
  if (n != GetPage())
  {
    m_pTextPage->SetValue(nwxString::FormatNumber(n)); // sends change event
  }
  _UpdatePageButtons(n);
}

int CPreviewControlBar::GetPage()
{
  return atoi(m_pTextPage->GetValue().c_str());
}

void CPreviewControlBar::SetZoom(int n)
{
  m_pListZoom->Select(_GetZoomIndex(n));
}
int CPreviewControlBar::GetZoom()
{
  return g_anZoomChoices[m_pListZoom->GetSelection()];
}
void CPreviewControlBar::_OnButtonFirst(wxCommandEvent &)
{
  SetPage(1);
}
void CPreviewControlBar::_OnButtonPrev(wxCommandEvent &)
{
  DecrementPage();
}
void CPreviewControlBar::_OnButtonNext(wxCommandEvent &)
{
  IncrementPage();
}
void CPreviewControlBar::_OnButtonLast(wxCommandEvent &)
{
  SetPage(GetPageCount());
}
void CPreviewControlBar::_OnTextCtrlPageChange(wxCommandEvent &e)
{
  SetPage(GetPage()); // update next/prev button state
  e.Skip();  // send to calling window
}

IMPLEMENT_ABSTRACT_CLASS(CPreviewControlBar, wxPreviewControlBar)
BEGIN_EVENT_TABLE(CPreviewControlBar, wxPreviewControlBar)
EVT_BUTTON(IDprintPageFirst, CPreviewControlBar::_OnButtonFirst)
EVT_BUTTON(IDprintPageLast, CPreviewControlBar::_OnButtonLast)
EVT_BUTTON(IDprintPageNext, CPreviewControlBar::_OnButtonNext)
EVT_BUTTON(IDprintPagePrev, CPreviewControlBar::_OnButtonPrev)
EVT_TEXT(IDprintPageText, CPreviewControlBar::_OnTextCtrlPageChange)
END_EVENT_TABLE()
