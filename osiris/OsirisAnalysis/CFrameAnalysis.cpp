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

*  FileName: CFrameAnalysis.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include "mainApp.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/file.h>
#include "CFrameAnalysis.h"
#include "OsirisFileTypes.h"
#include "CDialogDeleteDisabled.h"
#include "CDialogEnableMultiple.h"
#include "CDialogExportFile.h"
#include "CDialogWarnHistory.h"
#include "CDialogCMF.h"
#include "CDialogToggleEnabled.h"
//#include "CDialogAcceptAllele.h"
//#include "CDialogAcceptAlerts.h"
#include "CDialogApprove.h"
#include "CDialogEditPeak.h"
#include "CButtonGraph.h"
#include "CButtonSample.h"
#include "CLabSettings.h"
#include "COARfile.h"
#include "CReAnalyze.h"
#include "CGridAnalysisDisplay.h"
#include "CDirList.h"
#include "CArchiveCollection.h"
#include "CDialogArchiveCreate.h"
#include "nwx/PersistentSize.h"
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/colour.h>
#include <wx/defs.h>
#include <wx/utils.h>
#include "nwx/CPointerHold.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxButtonMenu.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxGrid.h"
#include "wxIDS.h"

#include "CPanelAlerts.h"
#include "CXSLExportFileType.h"
#include "CDialogHistoryAll.h"
#include "CDialogParameters.h"
#include "CDialogCellHistory.h"
#include "CPanelSampleAlertNotebook.h"
#include "COsirisIcon.h"
#include "CPanelPlotPreview.h"
#include "CMenuSort.h"
#include "CMenuBar.h"
#include "CGridLocusPeaks.h"
#include "CPanelStatus.h"
#include "CFrameSample.h"
#include "nwx/stdb.h"
#include <memory>
#include <vector>
#include "nwx/stde.h"
#include <wx/filedlg.h>


// #define SHOW_PREVIEW "Show Preview"
// #define HIDE_PREVIEW "Hide Preview"
// #define EDIT_SAMPLE_LABEL "Edit " COAR_NOTICE_DISPLAY_CAP " and Notes"


DEFINE_EVENT_TYPE(CEventRepaint)
DEFINE_EVENT_TYPE(CEventRestoreScroll)

#define PING_WINDOW_TYPE "FrameAnalysis"

#define LINE_SPACER "\n    "

const int CFrameAnalysis::STATUS_COLUMN = 0;
const int CFrameAnalysis::ILS_COLUMN = 1;
const int CFrameAnalysis::CHANNEL_ALERT_COLUMN = 2;
const int CFrameAnalysis::FIRST_LOCUS_COLUMN = 3;


CFrameAnalysis::~CFrameAnalysis()
{
  _SetPreviewMenu(NULL);
  _CleanupCMF();
  mainApp::Ping2(PING_WINDOW_CLOSE, PING_WINDOW_TYPE, PING_WINDOW_NUMBER, GetFrameNumber());
}

CFrameAnalysis::CFrameAnalysis(
    mainFrame *parent, wxSize sz, const wxString &sFileName) :
  CMDIFrame(
    parent, wxID_ANY,"",
    wxDefaultPosition,
    GET_PERSISTENT_SIZE_DEFAULT(CFrameAnalysis,sz)),
  m_pOARfile(NULL),
  m_pMenu(NULL),
  m_pMenuBar(NULL),
  m_pGridLocus(NULL),
  m_pPanelStatus(NULL),
  m_pAlerts(NULL),
  m_pSplitterLocusNotices(NULL),
  m_pPanelInfo(NULL),
  m_pPanelPlotPreview(NULL),
  m_pSizerInfo(NULL),
  m_pDlgAnalysis(NULL),
  m_pCMF(NULL),
  m_nNoTimer(0),
  m_bFileError(false)
{
  if(LoadFile(sFileName))
  {
    _Build();
  }
  else
  {
    m_bFileError = true;
    parent->RemoveWindow(this);
  }
}
CFrameAnalysis::CFrameAnalysis(
    mainFrame *parent, wxSize sz, COARfile *pFile) :
  CMDIFrame(
    parent, wxID_ANY,"",
    wxDefaultPosition,
    GET_PERSISTENT_SIZE_DEFAULT(CFrameAnalysis,sz)
    ),
  m_pOARfile(pFile),
  m_pMenu(NULL),
  m_pMenuBar(NULL),
  m_pGridLocus(NULL),
  m_pPanelStatus(NULL),
  m_pAlerts(NULL),
  m_pSplitterLocusNotices(NULL),
  m_pPanelInfo(NULL),
  m_pPanelPlotPreview(NULL),
  m_pSizerInfo(NULL),
  m_pDlgAnalysis(NULL),
  m_pCMF(NULL),
  m_nNoTimer(0),
  m_bFileError(false)
{
  if(pFile->GetSampleCount() < 1)
  {
    m_bFileError = true;
  }
  else
  {
    SetAutoReload(true);
    _Build();
    SetAutoReload(false);
  }
}
void CFrameAnalysis::_Build()
{
  FUNC_ENTER("CFrameAnalysis::_Build()")
  wxColour clrTextNoEdit;
  nwxButtonMenu *pButtonSort(NULL);
  int nDisplayPeak = 0;
  int nDisplayName;
  bool bShowPreview = false;

  mainApp::Ping2(PING_WINDOW_OPEN, PING_WINDOW_TYPE, PING_WINDOW_NUMBER, GetFrameNumber());
  m_pParent->InsertWindow(this,m_pOARfile);
  m_pButtonEdit = NULL;
  m_pComboCellType = NULL;
  m_pComboName = NULL;
  m_pButtonHistory = NULL;
  m_pCMF = NULL;
  m_nLastRowSelect = -1;
  m_nLastColSelect = -1;
  m_nEntireRowSelected = -1;



  m_pSplitter = new wxSplitterWindow(
    this,IDsplitterWindow,wxDefaultPosition, wxDefaultSize,
    ID_SPLITTER_STYLE);
  m_pSplitterTop = new wxSplitterWindow(
    m_pSplitter,wxID_ANY,wxDefaultPosition, wxDefaultSize,
    ID_SPLITTER_STYLE);



  m_pPanelGrid = new wxPanel(m_pSplitterTop);
  m_pSplitterLocusNotices = new wxSplitterWindow(
    m_pSplitter,wxID_ANY, wxDefaultPosition, wxDefaultSize,
    ID_SPLITTER_STYLE);
  m_pPanelInfo = new wxPanel(m_pSplitterLocusNotices);
  m_pPanelText = new wxPanel(m_pSplitterLocusNotices);
  m_pGrid = new CGridAnalysis(m_pPanelGrid);

  wxBoxSizer *pGridSizer = new wxBoxSizer(wxVERTICAL);
  pGridSizer->Add(m_pGrid,1,wxEXPAND);

  m_pPanelGrid->SetSizer(pGridSizer);

  //  m_pPanelToolbar

  m_pPanelToolbar = new wxPanel(this);
  m_pLabelFile = new wxTextCtrl(
    m_pPanelToolbar,wxID_ANY,wxEmptyString,
    wxDefaultPosition,
    wxDefaultSize,
    wxTE_READONLY);
  m_pButtonSample = new CButtonSample(m_pPanelToolbar);
  m_pButtonGraphic = new CButtonGraph(m_pPanelToolbar);
  m_SampleSort.Sort(m_pOARfile,NULL);
  m_pTogglePreview = new nwxToggleButton(
    m_pPanelToolbar,
    IDmenuTogglePreview,
    "Preview");
  m_pTogglePreview->SetValue(false);
  m_pTogglePreview->SetToolTip(
    CMenuAnalysis::PREVIEW_HELP_TEXT);

  GetMenu();  // force menu creation
  m_pButtonEdit = new nwxButtonMenu(
    *m_pMenu->GetMenuEdit(),
    m_pPanelToolbar,
    wxID_ANY,
    "Edit",
    wxDefaultPosition,
    wxDefaultSize,
    wxBU_EXACTFIT);

  m_pButtonEdit->SetToolTip("Edit the data selected below.");
  m_pButtonHistory = new CPanelHistoryMenu(
    m_pOARfile,
    m_pMenu->GetMenuHistoryPopup(),
    this,
    m_pPanelToolbar,
    IDhistoryButton);

  pButtonSort = new nwxButtonMenu(
    *m_pMenu->GetMenuSortPopup(),
    m_pPanelToolbar,
    wxID_ANY,
    "Sort",
    wxDefaultPosition,
    wxDefaultSize,
    wxBU_EXACTFIT
    );
  pButtonSort->SetToolTip(
    "Sort rows by sample name or severity\n(number of alerts)");
  m_pButtonParms = new wxButton(
    m_pPanelToolbar,
    IDmenuParameters,
    "Parameters...",
    wxDefaultPosition,
    wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonParms->SetToolTip(CMenuAnalysis::PARAMETERS_TOOL_TIP);
  m_pButtonGraphic->Enable(false);
  m_pButtonSample->Enable(false);
  m_pButtonParms->Enable(false);

  wxFont fn(m_pLabelFile->GetFont());
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  m_pLabelFile->SetFont(fn);
  clrTextNoEdit = nwxColorUtil::BrightenBackground(m_pPanelToolbar);
  m_pLabelFile->SetBackgroundColour(clrTextNoEdit);
  m_pPanelInfo->SetBackgroundColour(clrTextNoEdit);

  wxBoxSizer *pSizerLabel = new wxBoxSizer(wxHORIZONTAL);
  pSizerLabel->Add(m_pButtonSample,0,
    wxALL | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  pSizerLabel->Add(m_pButtonGraphic,0,
    (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  pSizerLabel->Add(m_pTogglePreview,0,
    (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  m_pButtonEdit->Enable(false);
  pSizerLabel->Add(m_pButtonEdit,0,
    (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  pSizerLabel->Add(m_pButtonHistory,0,
    wxALIGN_CENTER_VERTICAL, 0);
  pSizerLabel->Add(pButtonSort,0,
    wxALL | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  pSizerLabel->Add(m_pButtonParms,0,
    (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  wxButton *pButtonCMF;
  pSizerLabel->Add(
    new wxStaticText(m_pPanelToolbar,wxID_ANY,"Display:"),
    0, wxALL | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  m_pComboCellType = new CComboLabels(m_pPanelToolbar,IDgraphLabelsCombo);
  m_pComboName = new CComboLabelsName(m_pPanelToolbar,IDgraphNameCombo);
  BOX_COMBO(m_pComboCellType, pSizerLabel,
      ((wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL)); // #defined in CComboLabels.h
  BOX_COMBO(m_pComboName, pSizerLabel,
      ((wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL)); // #defined in CComboLabels.h
  pButtonCMF = new wxButton(m_pPanelToolbar,IDExportCMF,"CMF",
    wxDefaultPosition,
    wxDefaultSize,
    wxBU_EXACTFIT);
  pButtonCMF->SetToolTip("Export CODIS CMF 3.2 file");
  pSizerLabel->Add(pButtonCMF,0,
    (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  wxBoxSizer *pSizerFileName = new wxBoxSizer(wxVERTICAL);
  pSizerFileName->AddStretchSpacer(1);
  pSizerFileName->Add(m_pLabelFile,0, wxEXPAND);
  pSizerFileName->AddStretchSpacer(1);
  pSizerLabel->Add(
    pSizerFileName,1,
    (wxALL ^ wxLEFT) | wxEXPAND | wxALIGN_CENTER_VERTICAL, ID_BORDER);
  m_pPanelToolbar->SetSizer(pSizerLabel);

  // end m_pPanelToolbar

  bool bEditArtifacts = m_pOARfile->CanEditArtifacts();
  if(bEditArtifacts)
  {
    m_pAlerts = new CPanelAlerts(m_pPanelText);
  }
  else
  {
    m_pAlerts = NULL;
  }
  m_pText = new wxTextCtrl(
    m_pPanelText,
    wxID_ANY,
    "",
    wxDefaultPosition,
    wxDefaultSize,
    wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP );
  m_pText->SetBackgroundColour(clrTextNoEdit);
  wxBoxSizer *pSizerBottom = new wxBoxSizer(wxVERTICAL);
  if(bEditArtifacts)
  {
    pSizerBottom->Add(m_pAlerts,0,wxEXPAND);
  }
  pSizerBottom->Add(m_pText,1,wxEXPAND);
  m_pPanelText->SetSizer(pSizerBottom);

  m_pSplitterTop->Initialize(m_pPanelGrid);
#ifdef __WXMAC__
#define LOCUS_WIDTH 300
#define LOCUS_HEIGHT -270
#else
#define LOCUS_WIDTH 290
#define LOCUS_HEIGHT -235
#endif
  m_pSplitterLocusNotices->SplitVertically(m_pPanelInfo, m_pPanelText,LOCUS_WIDTH);
  m_pSplitterLocusNotices->SetSashGravity(0.25);
  m_pSplitterLocusNotices->SetMinimumPaneSize(1);
  m_pSplitter->SplitHorizontally(m_pSplitterTop,m_pSplitterLocusNotices,LOCUS_HEIGHT);
  // send event to fix sash position if it doesn't appear where specified
  // this is a workaround for a bug where the splitter does not
  // appear where specified when the MDI clients are maximized

  m_pSplitter->SetSashGravity(0.75);
  m_pSplitter->SetMinimumPaneSize(1);

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

  pSizer->Add(m_pPanelToolbar,0,wxEXPAND);
  pSizer->Add(m_pSplitter,1,wxEXPAND);
  SetSizer(pSizer);

  CParmOsirisGlobal parm;
  bool b = parm->GetHideTextToolbar();
  bool bControlAtTop = parm->GetTableControlsAtTop();
  ShowToolbar(!b);

  bShowPreview = parm->GetShowPreview();
  nDisplayPeak = parm->GetTableDisplayPeak();
  nDisplayName = CGridAnalysisDisplay::GetDisplayType();
  m_SampleSort.Sort(m_pOARfile);
  if(!ID_IS_WITHIN(nDisplayPeak,0,(int)m_pComboCellType->GetCount() - 1))
  {
    nDisplayPeak = 0;
  }
  m_pComboCellType->Select(nDisplayPeak);
  m_pMenu->SetCellTypeOffset(nDisplayPeak);
  if(!ID_IS_WITHIN(nDisplayName,0,(int)m_pComboName->GetCount() - 1))
  {
    nDisplayName = 0;
  }
  m_pComboName->Select(nDisplayName);
  m_pMenu->SetNameTypeOffset(nDisplayName);
  //m_pMenu->SetControlsOnTop(bControlAtTop);


  Layout();
  DisplayFile();
  if(bShowPreview)
  {
    // information is obtained from the grid in order to draw the preview
    // send an event to draw the preview so that the grid will be ready 
    // before the event is handled
    wxCommandEvent e(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,IDmenuTogglePreview);
    GetEventHandler()->AddPendingEvent(e);
  }
  if(bControlAtTop)
  {
    // workaround for problem setting it before the window is displayed
    wxCommandEvent e(wxEVT_COMMAND_MENU_SELECTED,IDmenuSortControlsAtTop);
    e.SetInt(1);
    GetEventHandler()->AddPendingEvent(e);
  }
  {
    wxCommandEvent e(wxEVT_COMMAND_ENTER,IDsplitterWindow); 
    e.SetEventObject(m_pSplitter);
    e.SetInt(LOCUS_HEIGHT);
    GetEventHandler()->AddPendingEvent(e);
  }
  COsirisIcon x;
  SetIcon(x);
  FUNC_EXIT("CFrameAnalysis::_Build()")
}
#undef LOCUS_WIDTH
#undef LOCUS_HEIGHT

bool CFrameAnalysis::FileError()
{
#ifdef TMP_DEBUG
  wxString s("CFrameAnalysis::FileError - ");
  s.Append(m_bFileError ? "true" : "false");
  nwxLog::LogMessage(s);
#endif
  return m_bFileError;
}

void CFrameAnalysis::UpdateFileMenu()
{
  m_pMenuBar->UpdateFileMenu();
}


wxString CFrameAnalysis::GetFileName()
{
  wxString sRtn;
  if(_XmlFile())
  {
    sRtn = m_pOARfile->GetFileName();
  }
  return sRtn;
}
int CFrameAnalysis::GetType()
{
  return FRAME_ANALYSIS;
}
bool CFrameAnalysis::SetToolbarMenuLabel(bool bShow, bool bPlural)
{
  bool bRtn = CMDIFrame::SetToolbarMenuLabel(bShow,bPlural);
  m_pMenu->UpdateChildren();
  return bRtn;
}

bool CFrameAnalysis::CheckIfHistoryOK()
{
  bool bRtn = 
    m_pButtonHistory->IsCurrent()
    ? true
    : CDialogWarnHistory::Continue(this);
  return bRtn;
}

void CFrameAnalysis::CheckSaveStatus()
{
  bool bSaveAs = CanSaveAs();
  bool bSave = m_pOARfile->CanSave();
  m_pMenuBar->EnableSave(bSaveAs,bSave,bSaveAs);
}
void CFrameAnalysis::EditPeak(
  COARpeakAny *pPeak, COARsample *pSample, CMDIFrame *parent)
{
  int n = -1;
  if(parent == NULL) { parent = this; }
  if( (pPeak == NULL) || (pSample == NULL) || !pSample->IsPeakEditable(pPeak) )
  {
    mainApp::ShowAlert("This peak cannot be edited.",parent);
  }
  else if(pSample->IsDisabled())
  {
    mainApp::ShowAlert("This sample has been disabled.",parent);
  }
  else
  {
    CDialogEditPeak xx(parent,pSample,pPeak,this->m_pOARfile->GetReviewerAllowUserOverride());
    if(xx.IsOK())
    {
      n = xx.ShowModal();
      if(n != wxID_CANCEL)
      {
        bool b = xx.UpdateFile(m_pOARfile,this);
        if(!b)
        {
          wxString ss = wxT("Problem with CDialogEditPeak::UpdateFile");
          wxASSERT_MSG(0,ss);
          mainApp::LogMessage(ss);
        }
      }
    }
    else
    {
      mainApp::ShowAlert("There was a problem creating the peak edit window",parent);
    }
  }
  return;
}
bool CFrameAnalysis::MenuEvent(wxCommandEvent &e)
{
  int nID = e.GetId();
  bool bRtn = true;
  bool bCancel = false;

  // first do the history warning
  if(!m_pButtonHistory->IsCurrent())
  {
    switch(nID)
    {
    case IDmenuAcceptLocus:
    case IDmenuReviewLocus:
    case IDmenuAcceptSample:
    case IDmenuAcceptDirectory:
    case IDmenuAcceptILS:
    case IDmenuAcceptChannels:
    case IDmenuReviewDirectory:
    case IDmenuReviewSample:
    case IDmenuReviewILS:
    case IDmenuReviewChannels:
    case IDmenuDisableSample:
    case IDmenuDisableMultiple:
    case IDmenuEditCell:
    case IDmenuEditDirectory:
      bCancel = !CheckIfHistoryOK();
      break;
    default:
      break;
    }
  }    
  if(!bCancel)
  {
    switch(nID)
    {
    case IDmenuDisplayGraph:
      OnShowGraphic(e);
      break;
    case IDmenuDisplaySample:
      OnShowSample(e);
      break;
    case IDmenuSampleTile:
      OnShowSampleAndGraphic(e);
      break;
    case IDmenuAcceptLocus:
      _OnAcceptLocus(e);
      //OnShowSample(e);
      break;
    case IDmenuReviewLocus:
      _OnReviewLocus(e);
      break;

    case IDmenuAcceptSample:
    case IDmenuAcceptILS:
    case IDmenuAcceptChannels:
    case IDmenuAcceptDirectory:
      _OnAcceptSample(nID);
      break;

    case IDmenuReviewDirectory:
    case IDmenuReviewSample:
    case IDmenuReviewILS:
    case IDmenuReviewChannels:
      _OnReviewSample(nID);
      break;
    case IDmenuDisableSample:
      _OnEnableSample();
      break;
    case IDmenuDisableMultiple:
      _OnEnableMultiple();
      break;
    case IDmenuDeleteDisabled:
      _OnDeleteDisabled();
      break;
    case IDmenuReAnalyze:
      _OnReAnalyze();
      break;
    case IDmenuEditDirectory:
      OnShowSample(e);
      // OnEditDirectory(e);
      break;
    case IDmenuEditCell:
      OnShowSample(e);
      // OnEdit(e);
      break;
    case IDmenuHistoryView:
      OnHistoryView(e);
      break;
    case IDmenuSortDisplayedName:
    case IDmenuSortFileName:
    case IDmenuSortSampleName:
    case IDmenuSortSeverity:
    case IDmenuSortRunTime:
    case IDmenuSortControlsAtTop:
      OnSortGrid(e);
      break;
    case IDmenuShowHideToolbar:
      ToggleToolbar();
      break;
    case IDmenuParameters:
      OnShowDetails(e);
      break;
    case IDmenuTogglePreview:
      OnTogglePreview(e);
      break;
    case IDExportCMF:
      OnExportCMF(e);
      break;
    case IDArchiveCreate:
      OnArchiveCreate(e);
      break;
    case IDmenuDisplayNameBASE + IDmenuDisplayNameFile:
    case IDmenuDisplayNameBASE + IDmenuDisplayNameSample:
      OnLabelNameChangedMenu(e);
      break;
    default:
      if(ID_IS_MENU_PLOT(nID))
      {
        bRtn = m_pPanelPlotPreview->MenuEvent(e);
      }
      else if(ID_IS_USER_EXPORT(nID))
      {
        OnUserExport(e);
      }
      else if(ID_IS_HISTORY(nID))
      {
        // this will send a CEventHistory command event
        m_pButtonHistory->HistoryEvent(e);
      }
      else if(ID_IS_DISPLAY(nID, IDmenuDisplayBEGIN))
      {
        OnLabelTypeChangedMenu(e);
      }
      else
      {
        wxString s;
        s.Printf("Unknown menu ID %d",e.GetId());
        wxASSERT_MSG(0,s);
        mainApp::LogMessage(s);
        bRtn = false;
      }
      break;
    }
  }
  return bRtn;
}


void CFrameAnalysis::ToggleToolbar()
{
  bool bWasShown = IsToolbarShown();
  ShowToolbar(!bWasShown);
  CParmOsirisGlobal parm;
  parm->SetHideTextToolbar(bWasShown);
}
bool CFrameAnalysis::_CheckIfTampered(COARfile *pOAR)
{
  const wxChar *TAMPER_PROMPT =
    wxS("It is suspected that this file has been modified\n")
	  wxS("outside of OSIRIS.  Do you wish to continue?");
  bool bRtn = false;
  if(pOAR == NULL)
  {
    pOAR = m_pOARfile;
  }
  if(pOAR->IsTampered() &&
      !mainApp::Confirm(GetParent(),TAMPER_PROMPT) )
  {
    bRtn = true;
  }
  return bRtn;
}
bool CFrameAnalysis::LoadFile(const wxString &sFileName)
{
  FUNC_ENTER("CFrameAnalysis::LoadFile")
  const size_t BUFSIZE = 12;
  char sBuf[BUFSIZE + 1];
  wxFile file(sFileName);
  m_bFileError = false;
  bool bOK = false;
  size_t nRead = 0;
  if(file.IsOpened())
  {
    nRead = file.Read((void *)sBuf,BUFSIZE);
    file.Close();
  }

  if( (nRead != BUFSIZE) || (sBuf[0] != '<') )
  {
    m_pParent->FileErrorMessage(sFileName);
    m_pOARfile = NULL;
  }
  else
  {
    auto_ptr<COARfile> pOAR(new COARfile());
    size_t nSampleCount;
    bOK = pOAR->LoadFile(sFileName);
    if(!bOK)
    {
      m_pParent->FileErrorMessage(sFileName);
      m_bFileError = true;
    }
    else if( (nSampleCount = pOAR->GetSampleCount()) < 1 )
    {
      bOK = false;
      m_bFileError = true;
#ifdef TMP_DEBUG
      nwxLog::LogMessage("CFrameAnalysis::LoadFile() no samples");
      FileError();
#endif
      m_pParent->FileEmptyMessage(sFileName);
    }
    else if(_CheckIfTampered(pOAR.get()))
    {
      bOK = false;
    }
    else
    {
      m_pOARfile = pOAR.release();
    }
  }
#ifdef TMP_DEBUG
  if(!bOK)
  {
    if(m_pOARfile != NULL)
    {
      nwxLog::LogMessage("CFrameAnalysis::LoadFile failed, m_pOARfile is not null");
    }
    FileError();
  }
#endif
  FUNC_EXIT("CFrameAnalysis::LoadFile")
  return bOK;
}
bool CFrameAnalysis::DisplayFile()
{
  bool bOK = !m_bFileError;
  if(bOK)
  {
    wxString sFileName = GetFileName();
    wxFileName fn(sFileName);
    m_sPath = fn.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
    bOK = TransferDataToWindow();
    SetupTitle();
  }
  return bOK;
}

void CFrameAnalysis::OnTimer(wxTimerEvent &e)
{
  if(!m_nNoTimer)
  {
    CheckSelection();
#if 0
    if(m_pDlgAnalysis != NULL)
    {
      m_pDlgAnalysis->OnTimer(e);
    }
#endif
    if(m_pPanelPlotPreview != NULL)
    {
      m_pPanelPlotPreview->OnTimer(e);
    }
    if(m_pMenuBar != NULL)
    {
      m_pMenuBar->OnTimer(e);
    }
  }
}
void CFrameAnalysis::CheckSelectionXML(bool bForceUpdate)
{
  int nRow = m_pGrid->GetGridCursorRow();
  int nCol = m_pGrid->GetGridCursorCol();
  bool bNewRow = (nRow != m_nLastRowSelect);
  bool bSelection = m_pGrid->IsSelection();
  bool bClickEdit = bSelection && !m_pGrid->IsInSelection(nRow,nCol);
  CheckFileModification();
  if(bForceUpdate || bClickEdit || bNewRow || (nCol != m_nLastColSelect))
  {
    nwxGridBatch xx(m_pGrid);
    bool bEdit = false;
    m_nEntireRowSelected = -1;
    COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
    m_nLastRowSelect = nRow;
    m_nLastColSelect = nCol;
    if(bNewRow || bForceUpdate)
    {
      SetFileNameLabel(pSample);
    }
    ShowStatusText();
    bEdit = (pSample != NULL) && pSample->IsEnabled(); // && !_IsControlColumn(nCol);
    _UpdateMenu();
    _UpdatePreview();
    UpdateStatusBar();
    m_pButtonEdit->Enable(bEdit);
    bSelection = false; // force SelectBlock() below
  }
  if(!bSelection)
  {
    m_pGrid->SelectBlock(nRow,nCol,nRow,nCol);
  }
}
bool CFrameAnalysis::_CheckStatusPanel(COARsample *pSample,bool bDestroy)
{
  bool bRtn = false;
  if(m_pPanelStatus != NULL)
  {
    bRtn = m_pPanelStatus->CheckSample(pSample);
    if((!bRtn) && bDestroy)
    {
      _DestroyStatusPanel();
    }
  }
  return bRtn;
}
void CFrameAnalysis::_HideStatusPanel()
{
  if(m_pPanelStatus != NULL)
  {
    m_pPanelStatus->Show(false);
    if( (m_pSizerInfo != NULL) &&
        m_pSizerInfo->Detach(m_pPanelStatus) )
    {
      m_pSizerInfo->Layout();
    }
  }
}
void CFrameAnalysis::_DestroyStatusPanel()
{
  if(m_pPanelStatus != NULL)
  {
    _HideStatusPanel();
    m_pPanelStatus->Destroy();
    m_pPanelStatus = NULL;
  }
}
void CFrameAnalysis::_DestroyLocusPanel()
{
  if(m_pGridLocus != NULL)
  {
    if(m_pSizerInfo->Detach(m_pGridLocus))
    {
      m_pGridLocus->Show(false);
      m_pPanelInfo->Layout();
    }
    m_pGridLocus->Destroy();
    m_pGridLocus = NULL;
  }
}

bool CFrameAnalysis::_RowNeedsAttention(int /*nRow */)
{
  return true;
//  bool bRtn = (m_pGrid->GetCellValue(nRow,STATUS_COLUMN) == CGridAnalysis::g_sStatusNeedsAttention);
//  return bRtn;
}
void CFrameAnalysis::_SetupStatusPanel(COARsample *pSample)
{
  _DestroyLocusPanel();
  if( (pSample != NULL) &&
      _XmlFile() && 
      m_pOARfile->CanEditArtifacts()
    )
  {
    bool bLayout = false;
    if(!_CheckStatusPanel(pSample,true))
    {
      m_pPanelInfo->Show(false);
      m_pPanelStatus = new CPanelStatus(m_pOARfile,pSample,this);
      m_pPanelStatus->SetBackgroundColour(m_pText->GetBackgroundColour());
      bLayout = true;
    }
    if(bLayout || !m_pPanelStatus->IsShown())
    {
      _LayoutInfo(m_pPanelStatus);
    }
  }
}
void CFrameAnalysis::_LayoutInfo(wxWindow *pChild)
{
  if(m_pSizerInfo == NULL)
  {
    m_pSizerInfo = new wxBoxSizer(wxVERTICAL);
    m_pPanelInfo->SetSizer(m_pSizerInfo);
  }
  m_pSizerInfo->Add(pChild,1,wxEXPAND,0);
  m_pPanelInfo->Layout();
  pChild->Show(true);
  m_pPanelInfo->Show(true);
}
void CFrameAnalysis::_SetupLocusPanel(COARsample *pSample, int nChannel, const wxString &sLocusName)
{
  _DestroyLocusPanel();
  _HideStatusPanel();
  if( (pSample != NULL) && (!sLocusName.IsEmpty()) && (nChannel > 0) )
  {
    if(pSample->HasPeaks(nChannel, sLocusName, false))
    {
      m_pPanelInfo->Show(false);
      wxClientDC dc(this);
      m_pGridLocus = new CGridLocusPeaks(
        pSample,nChannel,sLocusName,m_pPanelInfo,
        IDgridLocus,true,GetSelectedTime());
      _LayoutInfo(m_pGridLocus);
    }
  }
  else if(m_pPanelInfo != NULL)
  {
    m_pPanelInfo->Layout();
  }
}

void CFrameAnalysis::_LayoutAll()
{
  m_pPanelGrid->Layout();  // set up scroll bars on grid
  m_pPanelText->Layout();
  m_pPanelToolbar->Layout();
  m_pSplitter->Layout();
  if(m_pSplitterLocusNotices != NULL)
  {
    m_pPanelInfo->Layout();
  }
}

void CFrameAnalysis::SetFileNameLabel(COARsample *pSample)
{
  wxString s;
  if(pSample != NULL)
  {
    s = m_pOARfile->GetInputDirectory();
    nwxFileUtil::EndWithSeparator(&s);
    s.Append(pSample->GetFileName());
  }
  SetFileNameLabel(s);
}


void CFrameAnalysis::ShowStatusText(bool bIncludeStatusPanel)
{
  wxString sText;
  wxString sLocusName;
  const wxDateTime *pHistory(m_pButtonHistory->GetSelectedTime());
  COARsample *pSample = m_SampleSort.GetSample((size_t)m_nLastRowSelect);
  CAlertViewStatus viewStatus;
  if(m_pAlerts != NULL)
  {
    viewStatus = m_pAlerts->GetStatus();
  }
  int nChannel = 0;
  if(pSample == NULL)
  {}
  else if(m_nEntireRowSelected >= 0)
  {
    sText = m_pOARfile->FormatAlertsBySample(pSample,viewStatus,pHistory);
  }
  else if(m_nLastColSelect == ILS_COLUMN)
  {
    sText = m_pOARfile->FormatIlsAlerts(pSample,viewStatus,pHistory);
  }
  else if(m_nLastColSelect == CHANNEL_ALERT_COLUMN)
  {
    sText = m_pOARfile->FormatChannelAlerts(pSample,viewStatus,pHistory);
  }
  else if(m_nLastColSelect == STATUS_COLUMN)
  {
    sText = m_pOARfile->FormatAlertsBySample(pSample,viewStatus,pHistory);
  }
  else if(_IsLocusColumn(m_nLastColSelect))
  {
    sLocusName = m_pOARfile->GetLocusName(m_nLastColSelect - FIRST_LOCUS_COLUMN);
    const COARchannel *pChannel = m_pOARfile->GetChannelFromLocus(sLocusName);
    COARlocus *pLocus = pSample->FindLocus(sLocusName);
    nChannel = pChannel->GetChannelNr();
    sText = m_pOARfile->FormatLocusAlerts(pSample,pLocus,viewStatus,pHistory);
  }
  else
  {
    sText = m_pOARfile->FormatAlertsBySample(pSample,viewStatus,pHistory);
  }

  m_pText->SetValue(sText);
  if(bIncludeStatusPanel)
  {
    m_pButtonGraphic->Enable(true);
    m_pButtonSample->Enable(true);
    m_pButtonParms->Enable(true);
    m_pPanelToolbar->Layout();

    if(nChannel)
    {
      _SetupLocusPanel(pSample,nChannel,sLocusName);
    }
    else if(_RowNeedsAttention(m_nLastRowSelect))
    {
      _SetupStatusPanel(pSample);
    }
    else
    {
      _DestroyStatusPanel();
      _DestroyLocusPanel();
    }
  }
}



bool CFrameAnalysis::TransferDataToWindow()
{
  CIncrementer x(m_nNoTimer);
  wxClientDC dc(this);
  m_nLastRowSelect = -1;
  m_nLastColSelect = -1;
  bool bError = true;
  if(_XmlFile())
  {
    int nLabelType = m_pComboCellType->GetSelection();
    int nLabelName = m_pComboName->GetSelection();
    bError = !m_pGrid->TransferDataToGrid(
      m_pOARfile,&m_SampleSort,nLabelType,nLabelName,&dc);
    m_nLastRowSelect = m_pGrid->GetGridCursorRow();
    m_nLastColSelect = m_pGrid->GetGridCursorCol();
  }

  m_pText->Clear();
  SetFileNameLabel(wxEmptyString);
  m_pButtonGraphic->Enable(false);
  m_pButtonSample->Enable(false);
  m_pButtonParms->Enable(false);
  _DestroyStatusPanel();
  _DestroyLocusPanel();
  _LayoutAll();
  if(!bError)
  {
    CheckSelection(true);
  }
  return !bError;
}

void CFrameAnalysis::OnShowDetails(wxCommandEvent &)
{
  const CParmOsiris &parm(GetFileParameters());
  if((&parm) != NULL)
  {
    wxString sFileName(GetFileName());
    CDialogParameters dlg(
      this,
      &parm,
      &sFileName,
      NULL,
      &m_pOARfile->GetFullVersion(),
      &m_pOARfile->GetLabSettings(),
      m_pOARfile->GetMessageBook());
    dlg.ShowModal();
  }
  else
  {
    mainApp::LogMessage("Cannot get runtime parameters");
  }
}

void CFrameAnalysis::OnShowGraphic(wxCommandEvent &)
{
  ShowGraphicByRow(-1);
}
#ifndef __WXMAC__
#ifdef __WXDEBUG__
_CRTIMP extern long _crtBreakAlloc;
extern long _lRequestCurr;
#endif
#endif

void CFrameAnalysis::ShowSampleFrame(
  COARsample *pSample, const wxString &sLocus, int nAlertType, int nEventID )
{
  if(pSample == NULL)
  {
    wxASSERT_MSG(0,"CFrameAnalysis::_ShowSampleFrame no sample specified");
  }
  else
  {
    CFrameSample *pFrame = _FindSampleFrame(pSample);
    bool bNoChange = (nEventID > 0);
    bool bCancel = false;
    if(pFrame == NULL)
    {
#ifdef __WXMAC__
      if(
        (nEventID == IDmenuSampleTile) &&
        CheckCannotTile(this,true)
        )
      {
        bCancel = true;
      }
      else
#endif
      {
        const wxSize &sz = GET_PERSISTENT_SIZE(CFrameSample);
        pFrame = new CFrameSample(this,m_pParent,sz,m_pOARfile,pSample);
        _AddSample(pSample,pFrame);
        bNoChange = false; // new window, select page by sLocus or alert type
      }
    }
#ifdef __WXMAC__
    else if(CheckCannotTile(pFrame,false))
    {
      RaiseWindow();
      bCancel = true;
    }
#endif

    if(!bCancel)
    {
      //if(nEventID != IDmenuSampleTile)
      {
        pFrame->Show(true);
        pFrame->Raise();
      }
      if(bNoChange)
      {}
      else if(sLocus.IsEmpty())
      {
        pFrame->SelectAlerts(nAlertType);
      }
      else
      {
        pFrame->SelectLocus(sLocus);
      }
      if(nEventID == IDmenuSampleTile)
      {
        wxCommandEvent e;
        e.SetId(nEventID);
        pFrame->MenuEvent(e);
      }
    }
  }
}
void CFrameAnalysis::OnShowSample(wxCommandEvent &e)
{
  if(_XmlFile() && m_pOARfile->CanEditArtifacts())
  {
    int nRow = m_pGrid->GetGridCursorRow();
    COARsample *pSample = _FindSampleByRow(nRow);
    if(pSample != NULL)
    {
      wxString sLocus;
      int nType = -1;
      if(e.GetId() == IDmenuEditDirectory)
      {
        nType = SA_NDX_DIR;
      }
      else
      {
        int nCol =
          (m_nEntireRowSelected >= 0)
          ? -1
          : m_pGrid->GetGridCursorCol();
        if((nCol < FIRST_LOCUS_COLUMN) || _IsControlColumn(nCol)) 
        {
          nType = _ColToType(nCol);
        }
        else
        {
          sLocus = m_pOARfile->GetLocusName(nCol - FIRST_LOCUS_COLUMN);
        }
      }
      ShowSampleFrame(pSample,sLocus,nType,e.GetId());
    }
    else
    {
      wxString sError(wxT("Cannot find sample for row "));
      sError.Append(nwxString::FormatNumber(nRow + 1));
      mainApp::ShowError(sError,this);
    }
  }
}
void CFrameAnalysis::OnShowSampleAndGraphic(wxCommandEvent &e)
{
  OnShowGraphic(e);
  OnShowSample(e);
  CFrameSample *pFrame = _FindSampleFrameByRow();
  if(pFrame != NULL)
  {
    wxCommandEvent ee(wxEVT_MENU);
    ee.SetId(e.GetId());
    pFrame->GetEventHandler()->AddPendingEvent(ee);
  }
}

bool CFrameAnalysis::_CheckSamples()
{
  std::vector<CFrameSample *> vpSamples;
  std::vector<CFrameSample *>::iterator itrv;
  std::vector<wxString> vsUnsaved;
  std::vector<int> vnPage;
  std::vector<int>::iterator itrPage;
  MapSampleFrame::iterator itr;
  CFrameSample *pFrame;
  int nPage;
  for(itr = m_mapSamples.begin();
      itr != m_mapSamples.end(); 
      ++itr)
  {
    pFrame = itr->second;
    if(pFrame->IsModified(&nPage))
    {
      vsUnsaved.push_back(pFrame->GetDisplayedSampleName());
      vnPage.push_back(nPage);
      vpSamples.push_back(pFrame);
    }
  }
  size_t nSize = vnPage.size();
  if(nSize)
  {
    wxString sMessage;
    wxString sName;
    if(nSize > 1)
    {
      sMessage = 
        "The following sample has been modified\n"
        "and the changes have not been applied:";
    }
    else
    {
      sMessage = 
        "The following samples have been modified\n"
        "and the changes have not been applied:";
    }
    sMessage.Append(LINE_SPACER);
    nwxString::Join(vsUnsaved,&sName,LINE_SPACER);
    sMessage.Append(sName);
    sMessage.Append(
      "\n\nThis window cannot be closed until all changes\n"
      "are applied or sample windows have been closed.");
    mainApp::ShowAlert(sMessage,this);
    for(itrv = vpSamples.begin(), itrPage = vnPage.begin();
      itrv != vpSamples.end();
      ++itrv, ++itrPage)
    {
      (*itrv)->Show();
      (*itrv)->Raise();
      (*itrv)->Select(*itrPage);
    }
  }
  return !nSize;
}
bool CFrameAnalysis::_DestroySamples()
{
  size_t n = m_mapSamples.size();
  bool bRtn = true;
  if(n)
  {
    std::vector<CFrameSample *> vSamples;
    std::vector<CFrameSample *>::iterator itrv;
    MapSampleFrame::iterator itr;
    vSamples.reserve(n);
    // first copy CFrameSample pointers to a vector because
    // when each is destroyed, m_mapSamples may be modified
    for(itr = m_mapSamples.begin();
        itr != m_mapSamples.end(); 
        ++itr)
    {
      vSamples.push_back(itr->second);
    }
    for(itrv = vSamples.begin();
      itrv != vSamples.end();
      ++itrv)
    {
      if(!(*itrv)->Close(false))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}

void CFrameAnalysis::OnHistoryUpdate(wxCommandEvent &e)
{
  if( _XmlFile() )
  {
    if(m_pMenu != NULL)
    {
      CPanelHistoryMenu *pPanel = (CPanelHistoryMenu *)e.GetEventObject();
      if(pPanel != NULL)
      {
        m_pMenu->SelectTime(pPanel->GetSelectedTime());
      }
    }
    RepaintData();
  }
}

void CFrameAnalysis::OnChangeAlertView(wxCommandEvent &)
{
  ShowStatusText(false);
}
void CFrameAnalysis::OnCheckSplitter(wxCommandEvent &e)
{
    // this is a workaround for a bug where the splitter does not
    // appear where specified when the window is created while the
    // MDI clients are maximized
  wxSplitterWindow *pWin = (wxSplitterWindow *)e.GetEventObject();
  if(pWin->IsSplit())
  {
    wxSize sz = pWin->GetSize();
    const int THRESHOLD = 10;
    int nSelect = e.GetInt();
    int nCurrent = pWin->GetSashPosition();
    int nMax = (pWin->GetSplitMode() == wxSPLIT_VERTICAL) ? sz.GetWidth() : sz.GetHeight();
    if( (nCurrent < 0) != (nSelect < 0) )
    {
      if(nCurrent < 0)
      {
        nCurrent += nMax;
      }
      else
      {
        nCurrent -= nMax;
      }
    }
    if(nSelect < nMax && nSelect > -nMax)
    {
      // selection is within range
      int nDiff = nCurrent - nSelect;
    
      if(nDiff > THRESHOLD || nDiff < -THRESHOLD)
      {
          pWin->SetSashPosition(nSelect,true);
      }
    }
  }
}
void CFrameAnalysis::DoReviewLocus(COARsample *pSample, COARlocus *pLocus)
{
  if(_XmlFile() && m_pOARfile->CanEditArtifacts() &&
    (pSample != NULL) && (pLocus != NULL))
  {
    const COARmessages *pMsg = m_pOARfile->GetMessages();
    wxSize sz(SIZE_LOCUS_HISTORY);
    const COARchannel *pChannel =
          m_pOARfile->GetChannelFromLocus(pLocus->GetName());
    int nChannel =
          (pChannel == NULL) ? -1 : pChannel->GetChannelNr();
    CDialogApprove dlg(
          *pSample,
          nChannel,
          pLocus,
          *pMsg,
          *(m_pOARfile->GetHistory()),
          m_pOARfile->GetReviewerAllowUserOverride(),
          true,
          this,
          wxID_ANY,
          sz);
    int n = dlg.ShowModal();
    bool bGridFocus = true;
    if(n == wxID_OK)
    {
      RepaintAllData(pSample);
    }
    else if(n == IDmenuEditCell)
    {
      ShowSampleFrame(pSample,pLocus->GetName(),-1);
      bGridFocus = false;
    }
    if(bGridFocus)
    {
      m_pGrid->SetFocus();
    }
  }
}
void CFrameAnalysis::_SetupLocusColumn(wxCommandEvent &e,
  COARsample **ppSample, COARlocus **ppLocus)
{
  int nRow = m_pGrid->GetGridCursorRow();
  COARsample *pSample = m_SampleSort.GetSample((size_t) nRow);
  COARlocus *pLocus = NULL;
  if(pSample != NULL)
  {
    int nCol =
      (m_nEntireRowSelected >= 0)
      ? -1
      : m_pGrid->GetGridCursorCol();
    if(_IsLocusColumn(nCol))
    {
      const wxString &sLocus
        (m_pOARfile->GetLocusName(nCol - FIRST_LOCUS_COLUMN));
      pLocus = pSample->FindLocus(sLocus);
    }
    else
    {
      pLocus = (COARlocus *)e.GetClientData();
    }
  }
  *ppSample = pSample;
  *ppLocus = pLocus;
}

void CFrameAnalysis::_OnReviewLocus(wxCommandEvent &e)
{
  COARsample *pSample = NULL;
  COARlocus *pLocus = NULL;
  _SetupLocusColumn(e,&pSample,&pLocus);
  if(pLocus != NULL)
  {
    DoReviewLocus(pSample,pLocus);
  }
}
void CFrameAnalysis::_OnAcceptSample(int nReviewType)
{
  int nRow = m_pGrid->GetGridCursorRow();
  COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
  DoAcceptSample(nReviewType,pSample);
}

void CFrameAnalysis::DoAcceptSample(int nReviewType,COARsample *pSample)
{
  if(_XmlFile() && m_pOARfile->CanEditArtifacts() && (pSample != NULL))
  {
    int nSelect = -1;

    switch(nReviewType)
    {
    case IDmenuAcceptSample:
      nSelect = SA_NDX_SAMPLE;
      break;
    case IDmenuAcceptILS:
      nSelect = SA_NDX_ILS;
      break;
    case IDmenuAcceptChannels:
      nSelect = SA_NDX_CHANNEL;
      break;
    case IDmenuAcceptDirectory:
      nSelect = SA_NDX_DIR;
      break;
    }
    if(nSelect >= 0)
    {
      ShowSampleFrame(pSample,wxEmptyString,nSelect);
    }
  }
}
void CFrameAnalysis::_OnReviewSample(int nReviewType)
{
  int nRow = m_pGrid->GetGridCursorRow();
  COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
  DoReviewSample(nReviewType,pSample);
}
void CFrameAnalysis::DoReviewSample(int nReviewType, COARsample *pSample)
{
  if(_XmlFile() && 
    m_pOARfile->CanEditArtifacts() && 
    (pSample != NULL))
  {
    int nSelect = -1;
    switch(nReviewType)
    {
    case IDmenuReviewDirectory:
      nSelect = SA_NDX_DIR;
      break;
    case IDmenuReviewSample:
      nSelect = SA_NDX_SAMPLE;
      break;
    case IDmenuReviewILS:
      nSelect = SA_NDX_ILS;
      break;
    case IDmenuReviewChannels:
      nSelect = SA_NDX_CHANNEL;
      break;
    }
    if(nSelect >= 0)
    {
      wxSize sz(SIZE_EDIT_LOCUS);
      CDialogApprove dlg(
        nSelect,
        m_pOARfile,
        pSample,
        true,
        this,
        wxID_ANY,
        sz);
      int n = dlg.ShowModal();
      if(n == wxID_OK)
      {
        RepaintAllData(pSample);
      }
      else if(n == IDmenuEditCell)
      {
        ShowSampleFrame(pSample,wxEmptyString,nSelect);
      }
      m_pGrid->SetFocus();
    }
  }
}

void CFrameAnalysis::_OnReAnalyze()
{
  mainFrame *parent = (mainFrame *)GetParent();
  if(!parent->CheckMaxFrames(true))
  {
    CReAnalyze rz(m_pOARfile);
    if(rz.IsOK())
    {
      CParmOsiris parm(rz.GetParms());
      CDialogAnalysis dlg(this,&parm,false,false,false);
      CPointerHold<CDialogAnalysis> x(m_pDlgAnalysis,&dlg);
      const wxString &sVolume(m_pOARfile->GetLabSettings().GetVolumeName());
      if(sVolume.IsEmpty())
      {
        wxString sError = 
          "This file is from an older version of OSIRIS and\n"
          "does not contain the name of an " Volume_string
          ",\ntherefore this data cannot be reanalyzed."
          ;
        mainApp::ShowError(sError,this);
      }
      else if(!dlg.SetVolumeName(sVolume))
      {
        wxString sError;
        sError = "Cannot set " Volume_string " to ";
        sError.Append(sVolume);
        mainApp::ShowError(sError,this);
      }
      else if(dlg.ShowModal() == wxID_OK)
      {
        parent->ReAnalyzeSamples(dlg.GetVolume(),parm);
      }
      else
      {
        rz.CleanupNewFiles();
      }
    }
    else
    {
      mainApp::ShowError(rz.GetErrorMessage(),this);
    }
  }
}
void CFrameAnalysis::_OnDeleteDisabled()
{
  std::vector<const COARsample *> vps;
  if(m_pOARfile->GetDisabledSamples(&vps,false))
  {
    int nLabelTypeName = m_pComboName->GetSelection();
    std::vector<wxString> vsNames;
    std::vector<const COARsample *>::const_iterator itr;
    vsNames.reserve(vps.size());
    if(nLabelTypeName == IDmenuDisplayNameSample)
    {
      for(itr = vps.begin(); itr != vps.end(); ++itr)
      {
        // show sample names
        vsNames.push_back((*itr)->GetSampleName());
      }
    }
    else
    {
      for(itr = vps.begin(); itr != vps.end(); ++itr)
      {
        // show file names
        vsNames.push_back((*itr)->GetName());
      }
    }
    CDialogDeleteDisabled dlg(
      this, vsNames,m_pOARfile->GetReviewerAllowUserOverride());
    if(dlg.ShowModal() == wxID_OK)
    {
      wxString sPath;
      CMDIFrame *pFrame;
      for(itr = vps.begin();
        itr != vps.end();
        ++itr)
      {
        pFrame = _FindSampleFrame(*itr);
        if(pFrame != NULL)
        {
          pFrame->Close(false);
        }
        sPath = m_pOARfile->FindPlotFile(*itr);
        pFrame = sPath.IsEmpty() ? NULL : m_pParent->FindWindowByName(sPath);
        if(pFrame != NULL)
        {
          pFrame->Close(false);
        }
      }
      m_pOARfile->DeleteDisabledSamples(dlg.GetUserID());
      m_SampleSort.UpdateSort();
      RepaintData();
      m_pGrid->SetFocus();
    }
  }
}
void CFrameAnalysis::_OnEnableMultiple()
{
  if(_XmlFile() && m_pOARfile->CanEditArtifacts())
  {
    const std::vector<COARsample *> *pvs = m_SampleSort.GetSamples();
    
#ifdef __WXMAC__
    wxWindow *parent = this;
#else
    wxWindow *parent = GetParent();
#endif

    CDialogEnableMultiple dlg(
      pvs,m_pOARfile->GetReviewerAllowUserOverride(),parent);
    if(!dlg.HasSamples())
    {
      mainApp::ShowAlert(
        "There are no samples in this file.",this);
    }
    else if(dlg.ShowModal() == wxID_OK)
    {
      RepaintData();
    }
  }
  m_pGrid->SetFocus();
}
void CFrameAnalysis::_OnEnableSample()
{
  int nRow = m_pGrid->GetGridCursorRow();
  COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
  if(_XmlFile() && 
     m_pOARfile->CanEditArtifacts() && 
     pSample->CanDisableSample())
  {
    CDialogToggleEnabled dlg(
      pSample,
      m_pOARfile->GetReviewerAllowUserOverride(),
      this);
    if(dlg.ShowModal() == wxID_OK)
    {
      RepaintAllData(pSample);
    }
  }
  m_pGrid->SetFocus();
}

void CFrameAnalysis::OnHistoryView(wxCommandEvent &)
{
  if( _XmlFile() )
  {
    int nCol =
      (m_nEntireRowSelected >= 0)
      ? -1
      : m_pGrid->GetGridCursorCol();
    int nRow = m_pGrid->GetGridCursorRow();

    const COARmessages *pMsg = m_pOARfile->GetMessages();
    COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
    bool bLocusColumn = _IsLocusColumn(nCol);
    if(pSample == NULL)
    {
      wxString s;
      s.Format(
        "No sample, row %d, col %d\n",
        nRow,nCol);
      mainApp::LogMessage(s);
      wxASSERT_MSG(0,s);
    }
    else if(!bLocusColumn)
    {
      if(!pSample->IsEdited(pMsg,NULL,false))
      {
        const wxString sMsg = 
        "There is no history for sample, channel, or ILS alerts.";
        mainApp::ShowAlert(sMsg,this);
      }
      else
      {
        wxSize sz(SIZE_EDIT_LOCUS);
        map<int,wxString> mapChannelNames; // empty for now
        int nType = _ColToType(nCol);
        CDialogCellHistory dlg(
          nType,
          *m_pOARfile,
          *pSample,
          mapChannelNames,
          this,
          wxID_ANY,
          sz);
        dlg.ShowModal();
      }
    }
    else
    {
      COARlocus *pLocus =
          (pSample != NULL)
          ? pSample->FindLocus(m_pOARfile->GetLocusName(nCol - FIRST_LOCUS_COLUMN))
          : NULL;
      if((pLocus == NULL) || (!pLocus->HasBeenEdited(pMsg,NULL)))
      {
        const wxString sMsg = 
          "There is no history for this locus.";
        mainApp::ShowAlert(sMsg,this);
      }
      else if(m_pOARfile->CanEditArtifacts())
      {
        wxSize sz(SIZE_LOCUS_HISTORY);
        const COARchannel *pChannel =
          m_pOARfile->GetChannelFromLocus(pLocus->GetName());
        int nChannel =
          (pChannel == NULL) ? -1 : pChannel->GetChannelNr();
        CDialogCellHistory dlg(
          *pSample,
          nChannel,
          *pLocus,
          *pMsg,
          *(m_pOARfile->GetHistory()),
          this,
          wxID_ANY,
          sz);
        dlg.ShowModal();
      }
      else
      {
        wxSize sz(SIZE_EDIT_LOCUS);
        CDialogCellHistory dlg(
          *pLocus,
          *pMsg,
          *(m_pOARfile->GetHistory()),
          pSample->GetName(),
          this,
          wxID_ANY,
          sz);
        dlg.ShowModal();
      }
    }
  }
}
CXSLExportFileType *CFrameAnalysis::GetFileTypeByID(int nID)
{
  return m_pMenuBar->GetFileTypeByID(nID);
}
void CFrameAnalysis::_EnablePreview()
{
  m_pTogglePreview->Enable(true);
  m_pMenu->EnablePreview(true);
}
void CFrameAnalysis::_DisablePreview()
{
  _HidePreview();
  m_pTogglePreview->SetValue(false);
  m_pTogglePreview->Enable(false);
  m_pMenu->EnablePreview(false);
}
void CFrameAnalysis::_HidePreview()
{
  if(m_pSplitterTop->IsSplit())
  {
    m_pSplitterTop->Unsplit(m_pPanelPlotPreview);
    m_pTogglePreview->SetValue(false);
    m_pMenu->SetPreviewTextShow(true);
    _SetPreviewMenu(NULL);
    _LayoutAll();
  }
}
void CFrameAnalysis::_ShowPreview()
{
  if(!m_pSplitterTop->IsSplit())
  {
    m_pTogglePreview->SetValue(true);
    m_pMenu->SetPreviewTextShow(false);
    if(m_pPanelPlotPreview == NULL)
    {
      m_pPanelPlotPreview = new CPanelPlotPreview(
        this,
        m_pSplitterTop,
        m_pOARfile,
        mainApp::GetKitColors(),
        true,
        6);
    }

    m_pSplitterTop->SplitHorizontally(
        m_pPanelGrid,m_pPanelPlotPreview);
    m_pSplitterTop->SetMinimumPaneSize(1);
    // bUpdate is false when this is called from _Build()
    _UpdatePreview();
    _LayoutAll();
  }
}
void CFrameAnalysis::OnTogglePreview(wxCommandEvent &)
{
  CParmOsirisGlobal parm;
  bool bShowPreview = !m_pSplitterTop->IsSplit();
  if(bShowPreview)
  {
    _ShowPreview();
  }
  else
  {
    _HidePreview();
  }
  parm->SetShowPreview(bShowPreview);
}
int CFrameAnalysis::_GetPreviewColumn()
{
  int nCol =
    (m_nEntireRowSelected >= 0)
    ? -1
    : m_pGrid->GetGridCursorCol();
  if(_IsControlColumn(nCol))
  {
    nCol = -1;
  }
  return nCol;
}
LABEL_PLOT_TYPE CFrameAnalysis::GetPlotLabelType()
{
  int nLabel = m_pComboCellType->GetCurrentSelection();
  LABEL_PLOT_TYPE nRtn = (LABEL_PLOT_TYPE)CELL_TO_PLOT(nLabel);
  return nRtn;
}
wxWindow *CFrameAnalysis::GetInfoPanel()
{
  return m_pPanelInfo;
}

void CFrameAnalysis::_UpdatePreviewLabelType(int n)
{
  if(m_pSplitterTop->IsSplit())
  {
    int nPlotLabel = CELL_TO_PLOT(n);
    {
      int nCol = _GetPreviewColumn();
      if(_IsLocusColumn(nCol) || (nCol == ILS_COLUMN))
      {
        m_pPanelPlotPreview->SetPeakLabelType(nPlotLabel);
      }
    }
  }
}
void CFrameAnalysis::_UpdatePreview()
{
  if(m_pSplitterTop->IsSplit())
  {
    int nRow = m_pGrid->GetGridCursorRow();
    wxString sFileName = _GetGraphicFileName(nRow,false);
    if(sFileName.IsEmpty())
    {
      m_pPanelPlotPreview->SetFileMissing();
    }
    else
    {
      wxString s;
      int nCol = _GetPreviewColumn();
      if(nCol == ILS_COLUMN)
      {
        m_pPanelPlotPreview->SetFileILS(
          sFileName,false);
      }
      else if(nCol == CHANNEL_ALERT_COLUMN)
      {
        std::vector<unsigned int> vn;
        std::vector<wxString> vs;
        int nChannel;
        size_t nSize;
        nRow = m_pGrid->GetGridCursorRow();
        s = m_pGrid->GetCellValue(nRow,nCol);
        nwxString::Split(s.utf8_str(),&vs,",");
        nwxString::Trim(&vs);
        nSize = vs.size();
        if(nSize)
        {
          vn.reserve(nSize);
        }
        for(size_t i = 0; i < vs.size(); i++)
        {
          s = vs.at(i);
          nChannel = atoi(s.utf8_str());
          if(nChannel)
          {
            vn.push_back((unsigned int) nChannel);
          }
        }
        if(vn.empty())
        {
          m_pPanelPlotPreview->SetFileAllChannels(
            sFileName,false);
        }
        else
        {
          m_pPanelPlotPreview->SetFileChannels(
            sFileName,vn,false);
        }
      }
      else if(_IsLocusColumn(nCol))
      {
        s = _GetLocusFromColumn(nCol);
        m_pPanelPlotPreview->SetFileLocus(sFileName,s,false);

      }
      else // if( (nCol < 0) || _IsControlColumn(nCol) )
      {
        m_pPanelPlotPreview->SetFileAllChannels(
          sFileName,false);
      }
    }
    _SetPreviewMenu(m_pPanelPlotPreview->GetMenu());
  }
}
void CFrameAnalysis::DoAcceptLocus(COARsample *pSample, COARlocus *pLocus)
{
  if(_XmlFile() && 
      m_pOARfile->CanEditArtifacts() && 
      (pSample != NULL) &&
      (pLocus != NULL) )
  {
    ShowSampleFrame(pSample,pLocus->GetName(),-1);
  }
}

void CFrameAnalysis::_OnAcceptLocus(wxCommandEvent &e)
{
  COARsample *pSample = NULL;
  COARlocus *pLocus = NULL;
  _SetupLocusColumn(e,&pSample,&pLocus);
  if(pLocus != NULL && pSample != NULL && !pSample->IsDisabled())
  {
    DoAcceptLocus(pSample,pLocus);
  }
}



void CFrameAnalysis::OnEditFromGrid(wxGridEvent &e)
{
  if(CheckIfHistoryOK())
  {
    CheckFileModification();
    wxCommandEvent ee(wxEVT_NULL, IDmenuDisplaySample);
    OnShowSample(ee);
  }
  e.Skip();
}
void CFrameAnalysis::OnEditMenu(wxGridEvent &e)
{
  PopupMenu_(m_pMenu->GetMenuEdit(),wxDefaultPosition);
  e.Skip();
}



void CFrameAnalysis::ShowGraphicByRow(int nRow)
{
  if(nRow < 0)
  {
    nRow = m_pGrid->GetGridCursorRow();
  }
  if(_XmlFile())
  {
    wxString sFile = _GetGraphicFileName(nRow,true);
    if(!sFile.IsEmpty())
    {
      wxString sLocus = _GetLocusFromColumn();
      m_pParent->OpenFile(sFile,sLocus,m_pOARfile);
    }
  }

}
wxString CFrameAnalysis::_GetLocusFromColumn(int nCol)
{
  wxString sLocus;
  if(_XmlFile())
  {
    if(nCol < 0)
    {
      nCol = m_pGrid->GetGridCursorCol();
    }
    if(_IsLocusColumn(nCol))
    {
      sLocus = m_pOARfile->GetLocusName(nCol - FIRST_LOCUS_COLUMN);
    }
  }
  return sLocus;
}
int CFrameAnalysis::_GetChannelFromColumn(int nCol)
{
  int nRtn = 0;
  if (_XmlFile())
  {
    if (nCol < 0)
    {
      nCol = m_pGrid->GetGridCursorCol();
    }
    if (_IsLocusColumn(nCol))
    {
      nRtn = m_pOARfile->GetChannelNr(nCol - FIRST_LOCUS_COLUMN);
    }
  }
  return nRtn;
}
wxString CFrameAnalysis::_GetGraphicFileName(int nRow,bool bMessage)
{
  wxString sFile;
  if(_XmlFile())
  {
    if(nRow < 0)
    {
      nRow = m_pGrid->GetGridCursorRow();
    }
    if(nRow >= 0)
    {
      sFile = m_pOARfile->FindPlotFile(m_SampleSort.GetSample((size_t)nRow));
      if(sFile.IsEmpty())
      {
        if(bMessage)
        {
          wxString smsg("Cannot find graphic file");
          mainApp::ShowError(smsg,m_pParent->DialogParent());
          sFile.Clear();
        }
      }
      else if(m_pParent->FileInProgress(sFile,bMessage))
      {
        sFile.Clear();
      }
    }
  }
  return sFile;
}
void CFrameAnalysis::CheckFileModification()
{
  bool bReload = false;
  bool bError = false;
  wxString sFile;
  if(_XmlFile() && !DialogIsShowingOrNoFocus())
  {
    sFile = m_pOARfile->GetLastFileName();
  }
  if(sFile.IsEmpty())
  {;}
  else if(!m_pOARfile->CheckFileModification(false))
  {;} // file was not modified
  else if(m_pParent->FileInProgress(m_pOARfile->GetLastFileName()))
  {
    if(m_pSplitterTop->IsSplit())
    {
      mainApp::ShowAlert(
        "The graph preview cannot be displayed when\n"
        "the analysis files are being updated",
        this);
    }
    _DisablePreview();
  }
  else if(!PromptReload(m_pOARfile->GetLastFileName()))
  {
    if(m_pSplitterTop->IsSplit())
    {
      mainApp::ShowAlert(
          "The graph preview cannot be displayed after\n"
          "the analysis files have been updated",
          this);
    }
    _DisablePreview();
  } // user declined reload
  else if(!m_pOARfile->IsModified())
  {
    bReload = m_pOARfile->ReloadFile();

  }
  else if(CheckSaveOnCloseFile())
  {
    // user was prompted to save changes
    // and either saved changes or declined
    // i.e. user did not cancel
    bReload = m_pOARfile->LoadFile(sFile);
    bError = !bReload;
  }
  if(bReload)
  {
    if(!_CheckIfTampered())
    {
      m_SampleSort.Sort(
        m_pOARfile,
        m_pButtonHistory->GetSelectedTime());
      TransferDataToWindow();
      m_pParent->UpdatePlots(m_pOARfile);
      SetupTitle();
      _EnablePreview();
    }
    else
    {
      _Kill();
    }
  }
  else if(bError)
  {
    wxFileName fn(sFile);
    wxString s("Cannot open file, ");
    s.Append(fn.GetFullName());
    mainApp::ShowError(s,this);
    _Kill();
  }
}
bool CFrameAnalysis::IsEdited()
{
  bool bRtn = _XmlFile() ? m_pOARfile->IsModified() : false;
  return bRtn;
}
void CFrameAnalysis::SelectRowCol(int nRow, int nCol)
{
  int nR = m_pGrid->GetNumberRows();
  int nC = m_pGrid->GetNumberCols();
  if(nRow < 0 || nCol < 0 || nR < 1 || nC < 1)
  {}
  else if(nRow == m_nLastRowSelect && nCol == m_nLastColSelect)
  {}
  else
  {
    m_pGrid->SetGridCursor(
      nRow < nR ? nRow : (nR-1),
      nCol < nC ? nCol : (nC-1));
    CheckSelection(true);
  }
}
void CFrameAnalysis::SelectRow(int nRow)
{
  int nR = m_pGrid->GetNumberRows();
  if(nRow >= 0 && nR > 0)
  {
    nwxGridBatch xx(m_pGrid);
    if(nRow >= nR) { nRow = nR - 1; }
    bool bNewRow = (nRow != m_nLastRowSelect);
    m_nLastColSelect = 0;
    m_nLastRowSelect = nRow;
    m_nEntireRowSelected = nRow;
    m_pGrid->SelectRow(nRow,false);
    m_pGrid->SetGridCursor(nRow,0);

    if(_XmlFile())
    {
      COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
      bool bEnableEdit =
          (pSample != NULL) && pSample->IsEnabled();
      ShowStatusText();
      if(bNewRow)
      {
        SetFileNameLabel(pSample);
      }
      m_pButtonEdit->Enable(bEnableEdit);
      _UpdateMenu();
      _UpdatePreview();
    }

    UpdateStatusBar();
    m_pGrid->MakeCellVisible(nRow,0);
  }
}
#if HAS_STATUS_BAR
void CFrameAnalysis::UpdateStatusBar()
{
  wxString sText;
  if(m_pGrid != NULL)
  {
    int n = m_pGrid->GetGridCursorRow();
    if(n >= 0)
    {
      sText = m_pGrid->GetRowLabelValue(n);
    }
    n = m_pGrid->GetGridCursorCol();
    if(n >= 0)
    {
      wxString s = m_pGrid->GetColLabelValue(n);
      nwxString::Trim(&s);
      if(!s.IsEmpty())
      {
        sText.Append("; ");
        sText.Append(s);
      }
    }
  }
  m_pParent->SetStatusText(sText);
}
#endif
size_t CFrameAnalysis::GetSampleRow(const wxString &sSample)
{
  size_t nRtn = (size_t)wxNOT_FOUND;
  if(_XmlFile())
  {
    size_t nSize = m_SampleSort.GetCount();
    size_t i;
    COARsample *pSample;
    for(i = 0; i < nSize; i++)
    {
      pSample = m_SampleSort.GetSample(i);
      if(pSample != NULL && nwxString::FileNameStringEqual(pSample->GetName(),sSample))
      {
        nRtn = i;
        i = nSize; // loop exit
      }
    }
  }
  return nRtn;
}
bool CFrameAnalysis::SelectSample(const wxString &sSample)
{
  bool bRtn = false;
  size_t nRow = GetSampleRow(sSample);
  if(nRow != wxNOT_FOUND)
  {
    bRtn = true;
    SelectRow((int)nRow);
  }
  return bRtn;
}
bool CFrameAnalysis::SelectSampleLocus(const wxString &sSample, const wxString &sLocus)
{
  size_t nRow = GetSampleRow(sSample);
  bool bRtn = false;
  if(nRow != wxNOT_FOUND)
  {
    size_t nCount = m_pOARfile->GetLocusCount();
    for(size_t i = 0; i < nCount; ++i)
    {
      const wxString &s(m_pOARfile->GetLocusName(i));
      if(!sLocus.CmpNoCase(s))
      {
        SelectRowCol(int(nRow),int(i) + FIRST_LOCUS_COLUMN);
        i = nCount; // loop exit
        bRtn = true;
      }
    }
  }
  return bRtn;
}
bool CFrameAnalysis::SelectSampleType(const wxString &sSample, int nType)
{
  size_t nRow = GetSampleRow(sSample);
  bool bRtn = false;
  if(nRow != wxNOT_FOUND)
  {
    int nCol = _TypeToCol(nType);
    if(nCol < 0) { nCol = 0; };
    SelectRowCol(nRow,nCol);
    bRtn = true;
  }
  return bRtn;
}
void CFrameAnalysis::OnGridCellGraph(wxGridEvent &e)
{
  int nRow = e.GetRow();
  if(nRow >= 0)
  {
    ShowGraphicByRow(nRow);
  }
}
void CFrameAnalysis::OnGridGraph(wxGridEvent &e)
{
  int nRow = e.GetRow();
  if(nRow >= 0)
  {
    SelectRow(nRow);
    ShowGraphicByRow(nRow);
  }
  else
  {
    ResetSelection();
  }
}
void CFrameAnalysis::OnLabelClick(wxGridEvent &e)
{
  int nRow = e.GetRow();
  if(nRow >= 0)
  {
    SelectRow(nRow);
  }
  else
  {
    ResetSelection();
  }
}
void CFrameAnalysis::ShowFileSaveError(const wxString &s)
{
  wxString sMessage("ERROR: could not save file ");
  wxFileName fn(s);
  sMessage.Append(fn.GetFullName());
  mainApp::ShowError(sMessage,this);
}

void CFrameAnalysis::_CleanupCMF()
{
  if(m_pCMF != NULL)
  {
    delete m_pCMF;
    m_pCMF = NULL;
  }
}

bool CFrameAnalysis::FileEmpty()
{
  bool bRtn = true;
  if(_XmlFile())
  {
    bRtn = (m_pOARfile->GetSampleCount() < 1);
  }
  return bRtn;
}
bool CFrameAnalysis::_SaveOERFile(const wxString &sFileName)
{
  wxBusyCursor xxx;
  bool bRtn = m_pOARfile->SaveFile(sFileName);
  return bRtn;
}
void CFrameAnalysis::RepaintAllData(const COARsample *p)
{
  CheckSaveStatus();
  m_pParent->UpdateSamplePlot(m_pOARfile,p->GetFileName(false));
  CFrameSample *pF = _FindSampleFrame(p);
  if(pF != NULL)
  {
    pF->RepaintData();
  }
  RepaintData();
}

bool CFrameAnalysis::_CheckPromptNewer()
{
  // check if file was created by newer version of OSIRIS
  // if not, return TRUE
  // otherwise
  bool bRtn = true;
  if(m_pOARfile->CheckVersion() && m_pOARfile->CreatedByNewerVersion())
  {
    wxString sMsg =
        "This file was created with a newer version of OSIRIS,\n";
    sMsg.Append(m_pOARfile->GetFullVersion());
    sMsg.Append(
        "\nSaving changes may result in a loss of data.\n"
        "Do you wish to continue?");
        ;
    bRtn = mainApp::Confirm(this,sMsg,"Warning",wxNO_DEFAULT);
    if(bRtn)
    {
      m_pOARfile->ClearCheckVersion();
    }
  }
  return bRtn;
}

bool CFrameAnalysis::SaveFile()
{
  bool bRtn = false;
  if(m_pOARfile != NULL)
  {
    bool bCanSaveAs = true;
    if(!m_pOARfile->CanSave()) {}
    else if(!_CheckPromptNewer())
    {
      bCanSaveAs = false;
    }
    else
    {
      {
        wxBusyCursor xxxxx;
        if(m_pOARfile->SaveFile())
        {
          bRtn = true;
          m_pParent->AddToMRU(m_pOARfile->GetFileName());
          CheckSaveStatus();
          m_pParent->UpdateHistory(m_pOARfile);
          SetupTitle();
          mainApp::Ping2(PING_WINDOW_NUMBER, GetFrameNumber(),
            PING_EVENT, "SaveFile");
        }
        // destroy wxBusyCursor here by enclosing in {}
      }
      if(!bRtn)
      {
        ShowFileSaveError(m_pOARfile->GetFileName());
      }
    }
    if(bCanSaveAs && !bRtn)
    {
      bRtn = SaveFileAs();
    }
    if (!bRtn)
    {
      mainApp::Ping3(PING_WINDOW_NUMBER, GetFrameNumber(),
        PING_EVENT, "SaveFile", PING_ERROR, "1");
    }
  }
  return bRtn;
}
void CFrameAnalysis::UpdateHistory()
{
  m_pMenu->UpdateHistory();
}
void CFrameAnalysis::UpdateLadderLabels()
{
  if(_IsPreviewShowing())
  {
    m_pPanelPlotPreview->UpdateLadderLabels();
  }
}

bool CFrameAnalysis::SaveFileAs()
{
  bool bRtn = false;
  if(m_pOARfile != NULL)
  {
    wxString sOldName = m_pOARfile->GetFileName();
    wxString sFileName = sOldName;
    wxString sFilePath;
    bool bOAR = m_pOARfile->IsOAR();
    if(bOAR)
    {
      // change .oar file to a .oer file
      sFileName = nwxFileUtil::SetupFileName(sFileName,EXT_REPORT_EDITED);
    }
    if(sFileName.Len())
    {
      wxFileName fn(sFileName);
      sFilePath = fn.GetPath();
      sFileName = fn.GetFullName();
    }
    if(_CheckPromptNewer())
    {
      wxFileDialog dlg(
        this,"Save File",sFilePath,sFileName,
        FILE_TYPE_REPORT_SAVE_AS,
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT  );
      int n = dlg.ShowModal();
      SetFocus(); // OS-522, another window get the focus
                  // when this dialog is shown
      if(n == wxID_OK)
      {
        sFileName = dlg.GetPath();
        CMDIFrame *pFrame = m_pParent->FindWindowByName(sFileName);
        if( (pFrame != NULL) && (pFrame != (CMDIFrame *)this) )
        {
          wxString sError(
            "A file by this name is already open.\n"
                "Please close the file before overwriting it.");
          mainApp::ShowError(sError,this);
        }
        else if(_SaveOERFile(sFileName))
        {
          bRtn = true;
          m_pParent->AddToMRU(sFileName);
          CheckSaveStatus();
          m_pParent->UpdateHistory(m_pOARfile);
          m_pMenu->UpdateHistory();
          SetupTitle();
          mainApp::Ping3(PING_WINDOW_NUMBER, GetFrameNumber(),
            PING_EVENT, "SaveFileAs", "from", bOAR ? "FromOAR" : "FromOER");
        }
        else
        {
          ShowFileSaveError(sFileName);
          mainApp::Ping3(PING_WINDOW_NUMBER, GetFrameNumber(),
            PING_EVENT, "SaveFileAs", PING_ERROR, "1");
        }
      }
    }
  }
  return bRtn;
}
bool CFrameAnalysis::CheckSaveOnCloseFile()
{
  bool bDone = true;
  if(_XmlFile() && m_pOARfile->IsModified())
  {
    bDone = false;
    bool bCancel = false;
    while(!bDone && !bCancel)
    {
      wxFileName fn(m_pOARfile->GetFileName());
      wxString sMessage("Analysis File: ");
      sMessage.Append(fn.GetFullName());
      sMessage.Append(" has been modified.\n"
        "Would you like to save your changes?");
      wxMessageDialog dlg(this,sMessage,"Save File",
        wxYES_NO | wxCANCEL | wxYES_DEFAULT | wxICON_QUESTION);
      int n = dlg.ShowModal();
      if(n == wxID_YES || n == wxID_OK)
      {
        bDone = false;
        if(m_pOARfile->CanSave())
        {
          bDone = SaveFile();
        }
        if(!bDone)
        {
          bDone = SaveFileAs();
        }
        // if save fails, then this while loop will repeat
        // with the 'Would you like to save...' prompt
      }
      else if(n == wxID_CANCEL)
      {
        bDone = false;
        bCancel = true;
      }
      else if(n == wxID_NO)
      {
        bDone = true;
        _DestroySamples();
        m_pParent->DiscardChanges(m_pOARfile);
      }
    }
  }
  return bDone;
}
void CFrameAnalysis::OnClose(wxCloseEvent &e)
{
  CIncrementer xxx(m_nFocusRecursive);
  wxBusyCursor xxxx;
  bool bDone = true;
  if(!_CheckSamples())
  {
    bDone = false;
  }
  else if(!CheckSaveOnCloseFile())
  {
    bDone = false;
  }
  else if(!_DestroySamples())
  {
    bDone = false;
  }
  if(bDone)
  {
    Destroy();
  }
  else if(e.CanVeto())
  {
    e.Veto(true);
  }
}
void CFrameAnalysis::RepaintGridXML()
{
  COARfile *pFile = m_pOARfile;
  if(pFile != NULL)
  {
    wxBusyCursor xxx;
    nwxGridBatch xBatch(m_pGrid);
    int n = m_pComboCellType->GetSelection();
    int n2 = m_pComboName->GetSelection();
    m_pGrid->SaveScrollPosition();
    if( n == wxNOT_FOUND )
    {
      n = 0;
    }
    wxClientDC dc(this);
    m_pGrid->UpdateGrid(pFile,&m_SampleSort,n,n2,
      m_pButtonHistory->GetSelectedTime(),&dc);
    _LayoutAll();

    wxCommandEvent ee(CEventRestoreScroll,GetId());
    ee.SetEventObject(this);
    GetEventHandler()->AddPendingEvent(ee);
  }
}
void CFrameAnalysis::_OnRestoreScroll(wxCommandEvent &)
{
    m_pGrid->RestoreScrollPosition();
}
void CFrameAnalysis::_OnRepaint(wxCommandEvent &)
{
  // update all data in table and alert window
  if(_XmlFile())
  {
    int nRow = m_nEntireRowSelected;
    RepaintGridXML();
    _DestroyLocusPanel();
    _DestroyStatusPanel();
    if(nRow >= 0)
    {
      SelectRow(nRow);
    }
    else
    {
      CheckSelectionXML(true);
    }
    SetupTitle();
    m_pButtonHistory->EnablePrevNextButtons();
  }
}

void CFrameAnalysis::OnSave(wxCommandEvent &e)
{
  int n = e.GetId();
  if(n == wxID_SAVE)
  {
    SaveFile();
  }
  else if(n == wxID_SAVEAS)
  {
    SaveFileAs();
  }
  else
  {
    mainApp::LogMessage(wxT("CFrameAnalysis::OnSave() called with ID != ID_SAVE nor ID_SAVE_AS"));
    e.Skip();
  }
}
void CFrameAnalysis::OnLabelNameChangedMenu(wxCommandEvent &e)
{
  if(m_pOARfile)
  {
    int nLabel = m_pMenu->GetNameTypeOffsetById(e.GetId());
    CGridAnalysisDisplay::SetDisplayType(nLabel);
    if(m_pComboName != NULL)
    {
      m_pComboName->CheckSetSelection(nLabel);
      //m_pMenu->SetCellTypeToComboBox(m_pComboCellType);
    }
    m_pMenu->SetNameTypeFromId(e.GetId());
    wxClientDC dc(this);
    m_pGrid->UpdateGridRowLabels(&m_SampleSort,nLabel,&dc);
  }
}
void CFrameAnalysis::OnLabelNameChanged(wxCommandEvent &)
{
  if(m_pOARfile != NULL)
  {
    int nLabel = m_pComboName->GetCurrentSelection();
    CGridAnalysisDisplay::SetDisplayType(nLabel);
    if(m_pMenu != NULL)
    {
      m_pMenu->SetNameTypeFromComboBox(m_pComboName);
    }
    wxClientDC dc(this);
    m_pGrid->UpdateGridRowLabels(&m_SampleSort,nLabel,&dc);
  }
}


void CFrameAnalysis::SetLabelType(int n)
{
  if(_XmlFile())
  {
    bool bRepaint = false;
    if(m_pMenu != NULL && m_pMenu->SetCellTypeOffset(n))
    {
      // change was made
      bRepaint = true;
    }
    if( (m_pComboCellType != NULL) &&
        (m_pComboCellType->GetCurrentSelection() != n) &&
          ((int)m_pComboCellType->GetCount() > n)
      )
    {
      m_pComboCellType->Select(n);
      bRepaint = true;
    }
    if(bRepaint)
    {
      RepaintGridXML();
    }
  }
}

void CFrameAnalysis::OnLabelTypeChangedMenu(wxCommandEvent &e)
{
  CParmOsirisGlobal parm;
  int nLabel = m_pMenu->GetCellTypeOffsetById(e.GetId());
  m_pMenu->SetCellTypeOffset(nLabel); // echo to other menu
  parm->SetTableDisplayPeak(nLabel);
  if(m_pComboCellType != NULL)
  {
    m_pComboCellType->CheckSetSelection(nLabel);
    //m_pMenu->SetCellTypeToComboBox(m_pComboCellType);
  }
  RepaintGridXML();
  _UpdatePreviewLabelType(nLabel);
}
void CFrameAnalysis::OnLabelTypeChanged(wxCommandEvent &)
{
  CParmOsirisGlobal parm;
  int nLabel = m_pComboCellType->GetCurrentSelection();
  if(m_pMenu != NULL)
  {
    m_pMenu->SetCellTypeFromComboBox(m_pComboCellType);
  }
  parm->SetTableDisplayPeak(nLabel);
  RepaintGridXML();
  _UpdatePreviewLabelType(nLabel);
}
void CFrameAnalysis::OnClickCell(wxGridEvent &e)
{
  int nRow = e.GetRow();
  int nCol = e.GetCol();
  bool bForce = (m_nEntireRowSelected >= 0);
  m_nEntireRowSelected = -1;
  m_pGrid->SetGridCursor(nRow,nCol);
  m_pGrid->SelectBlock(nRow,nCol,nRow,nCol);
  CheckSelection(bForce);
  e.Skip(true);
}
void CFrameAnalysis::OnContextMenu(wxContextMenuEvent &e)
{
  if( (m_pOARfile != NULL) && (m_nLastRowSelect >= 0) )
  {

    int nCol = m_nLastColSelect;
    if(nCol < 0) { nCol = 0; };
    wxPoint pt(e.GetPosition());
    if(pt.x < 0 || pt.y < 0)
    {
      // compute position based on cell position
      // because this event was triggered by the keyboard
      wxRect r = m_pGrid->CellToRect(m_nLastRowSelect,nCol);
      pt = r.GetLeftBottom();
      wxPoint ptLabel(
        m_pGrid->GetRowLabelSize(),
        m_pGrid->GetColLabelSize());
      wxPoint ptScroll;
      wxPoint ptUnit;
      m_pGrid->GetViewStart(&ptScroll.x,&ptScroll.y);
      m_pGrid->GetScrollPixelsPerUnit(&ptUnit.x,&ptUnit.y);
      ptScroll.x *= ptUnit.x;
      ptScroll.y *= ptUnit.y;
      pt += ptLabel;
      pt -= ptScroll;
      if(pt.x < 0) { pt.x = 0;}
      if(pt.y < 0) { pt.y = 0;}
      for(wxWindow *pWindow = m_pGrid;
          (pWindow != NULL) && (pWindow != this);
          pWindow = pWindow->GetParent())
      {
        pt += pWindow->GetPosition();
      }
    }
    else
    {
      pt = wxDefaultPosition;
    }
    PopupMenu_(m_pMenu->GetPopup(),pt);
  }
}
void CFrameAnalysis::OnRightClickCell(wxGridEvent &e)
{
  int nRow = e.GetRow();
  if(nRow >= 0)
  {
    // this is a row label or cell, not a column label
    int nCol = e.GetCol();
    if(nCol >= 0)
    {
      OnClickCell(e);
    }
    else
    {
      OnLabelClick(e);
    }
    wxContextMenuEvent ec(wxEVT_NULL,wxID_ANY,wxPoint(1,1));
    OnContextMenu(ec);
  }
}
bool CFrameAnalysis::_IsCellEdited(int nRow, int nCol)
{
  bool bRtn = false;
  if(_XmlFile() && (nRow >= 0))
  {
    const COARmessages *pMsgs = m_pOARfile->GetMessages();
    COARsample *pSample = m_SampleSort.GetSample((size_t)nRow);
    if(pSample == NULL)
    {;}
    else if(m_nEntireRowSelected >= 0)
    {
      bRtn = pSample->IsSampleLevelEdited(pMsgs);
    }
    else if(nCol == ILS_COLUMN)
    {
      bRtn = pSample->IsCellILSEdited(pMsgs);
    }
    else if(nCol == CHANNEL_ALERT_COLUMN)
    {
      bRtn = pSample->IsCellChannelEdited(pMsgs);
    }
    else if(_IsLocusColumn(nCol))
    {
      COARlocus *pLocus =
        pSample->FindLocus(m_pOARfile->GetLocusName(nCol - FIRST_LOCUS_COLUMN));
      if(pLocus != NULL)
      {
        const COARchannel *pChannel = m_pOARfile->GetChannelFromLocus(pLocus->GetName());
        int nChannel = pChannel->GetChannelNr();
        bRtn = pLocus->HasBeenEdited(pMsgs,pSample,nChannel);
      }
    }
  }
  return bRtn;
}

void CFrameAnalysis::_UpdateHistoryMenu(int nRow,int nCol,bool bEnabled)
{
  wxString sType;
  wxString s;
  bool bHist = false;
  if(_IsLocusColumn(nCol))
  {
    s = "Allele Edit History";
  }
  else 
  {
    if(nCol <= 0)
    {
      sType = CGridAlerts::LABEL_SAMPLE;
    }
    else if(nCol == ILS_COLUMN)
    {
      sType = CGridAlerts::LABEL_ILS;
    }
    else if(nCol == CHANNEL_ALERT_COLUMN)
    {
      sType = CGridAlerts::LABEL_CHANNEL;
    }
    if(!sType.IsEmpty())
    {
      s = "View ";
      s.Append(sType);
      s.Append(" ");
      s.Append(COAR_NOTICE_DISPLAY_CAP);
    }
  }
  if(!s.IsEmpty())
  {
    m_pMenu->SetHistoryCellLabel(s);
    bHist = _IsCellEdited(nRow,nCol);
  }
  m_pMenu->EnableHistoryView(bHist && bEnabled);
}

void CFrameAnalysis::_UpdateMenu()
{
  if(m_nLastRowSelect >= 0 
      && 
     (m_nLastColSelect >= 0 || m_nEntireRowSelected >= 0)
    )
  {
    wxString sLocus;
    COARsample *pSample = m_SampleSort.GetSample((size_t)m_nLastRowSelect);
    bool bAllowDisabled = pSample->CanDisableSample();
    bool bLocus = _IsLocusColumn();
    bool bControl = (!bLocus) && _IsControlColumn();

    GetMenu();
    int nReviewerCount = 0;
    int nAcceptanceCount = 0;
    bool bReviewLocus = false;
    bool bReviewSample = false;
    bool bReviewILS = false;
    bool bReviewChannels = false;
    bool bAcceptLocus = false;
    bool bAcceptSample = false;
    bool bAcceptILS = false;
    bool bAcceptChannels = false;
    bool bEnabled = !pSample->IsDisabled(); // pHistory);
    bool bEnableEditLocus = false;
    bool bEnableEditDir = m_pOARfile->HasDirectoryMessages();
    bool bAcceptDirectory = bEnableEditDir && m_pOARfile->NeedDirAcceptance();
    bool bReviewDirectory = 
      bEnableEditDir && (!bAcceptDirectory) && m_pOARfile->NeedDirReview();


    if(bEnabled)
    {
      m_pOARfile->GetReviewerCounts(&nReviewerCount, &nAcceptanceCount,CLabReview::REVIEW_SAMPLE);
      bReviewSample = 
        pSample->NeedSampleReview(nReviewerCount,NULL);
      if(!bReviewSample)
      {
        bAcceptSample =
          pSample->NeedSampleAcceptance(nAcceptanceCount,NULL);
      }

      m_pOARfile->GetReviewerCounts(
        &nReviewerCount, &nAcceptanceCount,
        CLabReview::REVIEW_CHANNEL);
      bReviewChannels = 
        pSample->NeedChannelReview(nReviewerCount,NULL);
      if(!bReviewChannels)
      {
        bAcceptChannels =
          pSample->NeedChannelAcceptance(nAcceptanceCount,NULL);
      }

      m_pOARfile->GetReviewerCounts(
        &nReviewerCount, &nAcceptanceCount,CLabReview::REVIEW_ILS);
      bReviewILS = 
        pSample->NeedILSReview(nReviewerCount,NULL);
      if(!bReviewILS)
      {
        bAcceptILS =
          pSample->NeedILSAcceptance(nAcceptanceCount,NULL);
      }
    }


    if(bLocus)
    {
      sLocus = _GetLocusFromColumn();
      COARlocus *pLocus = pSample->FindLocus(sLocus);
      bEnableEditLocus = bEnabled && (pLocus != NULL);
      if(bEnableEditLocus)
      {
        m_pOARfile->GetReviewerCounts(
          &nReviewerCount, &nAcceptanceCount,
          CLabReview::REVIEW_LOCUS);
        bReviewLocus =
          pLocus->NeedReview(nReviewerCount,NULL);
        if(!bReviewLocus)
        {
          bAcceptLocus = pLocus->NeedAcceptance(
            nAcceptanceCount,NULL);
        }
      }
      m_pMenu->EnableEditLocus(bEnableEditLocus);
    }
    m_pMenu->EnableEditDirectory(bEnableEditDir);
    m_pMenu->SetLocus(sLocus,bEnabled && !bControl);
    m_pMenu->SetSampleEnabled(
      bEnabled,bAllowDisabled);
    if(bEnabled)
    {
      m_pMenu->EnableAccept(
        bAcceptLocus,bAcceptSample,bAcceptILS,bAcceptChannels,bAcceptDirectory);
      m_pMenu->EnableReview(
        bReviewLocus,bReviewSample,bReviewILS,bReviewChannels,bReviewDirectory);
    }
    _UpdateHistoryMenu(bEnabled);
  }
}
wxMenu *CFrameAnalysis::GetGraphMenu()
{
  wxMenu *pRtn(NULL);
  if(m_pPanelPlotPreview != NULL)
  {
    pRtn = m_pPanelPlotPreview->GetMenu();
  }
  return pRtn;
}
wxMenu *CFrameAnalysis::GetTableMenu()
{
  return GetMenu();
}

wxMenu *CFrameAnalysis::GetMenu()
{
  if((!m_nFocusRecursive) && _XmlFile() && (m_pMenu == NULL))
  {
    m_pMenu = new CMenuAnalysis(m_pOARfile);
    if(!m_pOARfile->CanEditArtifacts())
    {
      m_pMenu->Enable(IDmenuDisableMultiple,false);
    }
    if(m_pComboCellType != NULL)
    {
      m_pMenu->SetCellTypeFromComboBox(m_pComboCellType);
    }
    if(m_pComboName != NULL)
    {
      m_pMenu->SetNameTypeFromComboBox(m_pComboName);
    }
    _UpdateMenu();
    m_pMenuBar = new CMenuBarAnalysis(this,m_pMenu);
    SetMenuBar(m_pMenuBar);
  }
  return m_pMenu;
}

void CFrameAnalysis::SetupTitle()
{
  // Set window title to show file name and
  // an asterisk if the file has been modified since
  // last saved.
  //
  wxString s;
  const CParmOsiris &parm(GetFileParameters());
  bool bMod = false;
  const wxDateTime *pTime(NULL);
  if(_XmlFile())
  {
    s = m_pOARfile->GetFileName();
    bMod = m_pOARfile->IsModified();
    pTime = m_pButtonHistory->GetSelectedTime();
  }
  SetTitle(mainApp::FormatWindowTitle(wxT("Table"),s,bMod,&parm,pTime));
}

void CFrameAnalysis::OnSortGrid(wxCommandEvent &e)
{
  wxBusyCursor xxxxxx;
  int nEntireRow = m_nEntireRowSelected;
  int nCol = m_nLastColSelect;
  int nRow = m_nLastRowSelect;
  int nGet = (nEntireRow >= 0) ? nEntireRow : nRow;
  size_t nNew(0);
  COARsample *pSample = (nGet >= 0) ? m_SampleSort.GetSample((size_t) nGet) : NULL;
  int nID = e.GetId();
  if(nID != IDmenuSortControlsAtTop)
  {
    CGridAnalysisDisplay::SetSortTypeFromEvent(nID);
    int nLabel = m_pGrid->GetCurrentLabelType();
    CGridAnalysisDisplay::SetDisplayType(nLabel);
  }
  else
  {
    CGridAnalysisDisplay::SetControlsOnTop(e.IsChecked());
    m_pMenu->SetControlsOnTop(e.IsChecked());
  }

  m_SampleSort.Sort(
    m_pOARfile,
    m_pButtonHistory->GetSelectedTime());
  if(pSample != NULL)
  {
    nNew = m_SampleSort.GetSampleIndex(pSample);
    if(nNew == COARsampleSort::NPOS)
    {
      // could not find sample
      m_pGrid->SetGridCursor(0,0);
    }
    else if(nEntireRow >= 0)
    {
      SelectRow((int)nNew);
    }
    else if(nCol >= 0)
    {
      m_pGrid->SetGridCursor((int)nNew,nCol);
    }
  }
  RepaintData();
}

bool CFrameAnalysis::FileNeedsAttention(bool bCMF, bool bShowMessage)
{
  wxString sName;
  size_t nCount = m_pOARfile->GetSampleCount();
  size_t i;
  COARsample *pSample;
  int nAttention = 0;
  bool bFound = false;
  bool bDirNeedsAttention = m_pOARfile->DirectoryNeedAttention();

  for(i = 0; i < nCount; i++)
  {
    pSample = m_pOARfile->GetSample(i);
    //if(!pSample->IsSampleType()) {}
    //else 
    if(pSample->NeedAttention(m_pOARfile->GetLabSettings()))
    {
      nAttention++;
      if(nAttention == 1)
      {
        sName = pSample->GetName();
      }
    }
    else if(pSample->IsSampleType() && !pSample->IsDisabled())
    {
      bFound = true;
    }
  }
  wxString sMsg;
  bool bOK = !(bDirNeedsAttention || nAttention);
  bool bNotNeedAttn = bOK && (bFound || !bCMF);
    // for xslt export, we don't care if a sample was found
  if(bNotNeedAttn)
  {} // all is  OK
  else if(!bShowMessage)
  {} // all is done
  else if(!bOK)
  {
#define ATTENTION_CMF " attention before creating a CMF file."
#define ATTENTION_EXPORT " attention.\nDo you still want to export the data?"
    wxString sMsgSample;
    if(nAttention > 1)
    {
      sMsgSample.Printf(
        "%d samples",
        nAttention);
    }
    else
    {
      sMsgSample = sName;
    }
    if(bDirNeedsAttention)
    {
      sMsg = "Directory messages";
      if(nAttention > 0)
      {
        sMsg.Append(" and ");
      }
    }
    sMsg.Append(sMsgSample);
    sMsg.Append(
        (bDirNeedsAttention || (nAttention > 1))
        ? " need "
        : " needs "
        );
    sMsg.Append(bCMF ? ATTENTION_CMF : ATTENTION_EXPORT);
#undef ATTENTION
#undef ATTENTION_EXPORT
  }
  else if(bCMF && !bFound)
  {
    sMsg = "There are no samples to be exported to a CMF file.";
  }
  if(bNotNeedAttn)
  {} // all is good
  else if(sMsg.IsEmpty())
  {}
  else if(bCMF)
  {
    mainApp::ShowError(sMsg,this);
  }
  else
  {
    wxMessageDialog dlgmsg(
      this,sMsg,"Alert",wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
    int n = dlgmsg.ShowModal();
    bNotNeedAttn = (n == wxID_YES) || (n == wxID_OK);

  }
  return !bNotNeedAttn;
}

bool CFrameAnalysis::PromptSaveFileNow(const wxString &sMessage)
{
  bool bRtn = true;
  if(m_pOARfile->IsModified())
  {
    static const wxString sTITLE("Save Analysis File?");
    bRtn = false;
    if(mainApp::Confirm(this,sMessage,sTITLE,wxYES_DEFAULT))
    {
      bRtn = SaveFile() && !m_pOARfile->IsModified();
    }
  }
  return bRtn;
}

bool CFrameAnalysis::ExportCMF()
{
  bool bOK = true;

  // check if any samples exist and none need attention

  if(m_pOARfile->CanEditArtifacts())
  {
    bOK = !FileNeedsAttention(true,true);
    /*
    wxString sName;
    size_t nCount = m_pOARfile->GetSampleCount();
    size_t i;
    COARsample *pSample;
    int nAttention = 0;
    bool bFound = false;
    bool bDirNeedsAttention = m_pOARfile->DirectoryNeedAttention();

    for(i = 0; i < nCount; i++)
    {
      pSample = m_pOARfile->GetSample(i);
      //if(!pSample->IsSampleType()) {}
      //else 
      if(pSample->NeedAttention(m_pOARfile->GetLabSettings()))
      {
        nAttention++;
        if(nAttention == 1)
        {
          sName = pSample->GetName();
        }
      }
      else if(pSample->IsSampleType() && !pSample->IsDisabled())
      {
        bFound = true;
      }
    }
    wxString sMsg;
    wxString sMsgSample;
    if(bDirNeedsAttention || nAttention)
    {
#define ATTENTION " attention before creating a CMF file."
      if(nAttention > 1)
      {
        sMsgSample.Printf(
          "%d samples",
          nAttention);
      }
      else
      {
        sMsgSample = sName;
      }
      if(bDirNeedsAttention)
      {
        sMsg = "Directory messages";
        if(nAttention > 0)
        {
          sMsg.Append(" and ");
        }
      }
      sMsg.Append(sMsgSample);
      sMsg.Append(
          (bDirNeedsAttention || (nAttention > 1))
          ? " need " ATTENTION
          : " needs " ATTENTION
          );
      bOK = false;
#undef ATTENTION
    }
    else if(!bFound)
    {
      sMsg = "There are no samples to be exported to a CMF file.";
      bOK = false;
    }
    if(!bOK)
    {
      mainApp::ShowError(sMsg,this);
    }
    */
  }

  // END check if any samples exist and none need attention

  if(bOK)
  {
    static const wxString sCONFIRM(
      "The analysis file has been modified and must be saved\n"
      "before the data are exported to a CMF file.\n"
      "Would you like to save the analysis file now?");
    bOK = PromptSaveFileNow(sCONFIRM);
  }
  if(bOK)
  {
    if(m_pCMF == NULL)
    {
      m_pCMF = new CDialogCMF(m_pParent->DialogParent(),wxID_ANY,m_pOARfile,&m_SampleSort);
    }
    int n = m_pCMF->ShowModal();
    if(n != IDbuttonFinishLater)
    {
      _CleanupCMF();
    }
  }
  return bOK;
}


void CFrameAnalysis::OnUserExport(wxCommandEvent &e)
{
  bool bOK = true;
  int nID = e.GetId();
  CXSLExportFileType *pExport = GetFileTypeByID(nID);
  if(pExport == NULL)
  {
    mainApp::ShowError("Cannot find export file type",this);
    bOK = false;
  }
  else if(
    m_pOARfile->CanEditArtifacts() &&
    !m_pOARfile->GetLabSettings().GetAllowExportWithAttnReqd())
  {
    bOK = !FileNeedsAttention(false,true);
  }
  if(bOK)
  {
    static const wxString sCONFIRM(
      "The analysis file has been modified and must\n"
      "be saved before the data are exported.\n"
      "Would you like to save the analysis file now?");
    bOK = PromptSaveFileNow(sCONFIRM);
  }
#define ALL_FILES "All Files (*.*)|*.*"
  if(bOK)
  {
    const wxString &sExportType = pExport->GetName();
    wxString sType;
    wxString sDefaultExt;
    const set<wxString> &ssType =
      pExport->GetFileExt();
    bool bOverride = pExport->AllowExtOverride();
    if(ssType.size())
    {
      wxString sExt;
      size_t n = ssType.size() << 3;
      bool bFirst = true;
      sType.Alloc(n + 32 + sExportType.Len());
      sExt.Alloc(n);

      for (set<wxString>::const_iterator itr = ssType.begin();
        itr != ssType.end();
        ++itr)
      {
        if(!bFirst)
        {
          sExt.Append(";");
        }
        else
        {
          bFirst = false;
          sDefaultExt = *itr;
        }
        sExt.Append( "*.");
        sExt.Append(*itr);
      }
      sType = sExportType;
      sType.Append(" (");
      sType.Append(sExt);
      sType.Append(")");
      sType.Append("|");
      sType.Append(sExt);
      if(bOverride)
      {
        sType.Append("|" ALL_FILES);
      }
    }
    else
    {
      sType.Append(ALL_FILES);
    }

    // we now have file types
    wxString sTitle;
    wxString sDefaultDir;
    CParmOsirisGlobal parm;
    wxFileName fn(m_pOARfile->GetLastFileName());
    if(pExport->IsDefaultLocationLast())
    {
      sDefaultDir = parm->GetLastExportDirectory();
    }
    else if(pExport->IsDefaultLocationAnalysisFile())
    {
      sDefaultDir = fn.GetPath();
    }
    else
    {
      sDefaultDir = pExport->GetDefaultLocation();
    }
    if(!wxDir::Exists(sDefaultDir))
    {
      sDefaultDir = mainApp::GetConfig()->GetFilePath();
    }
    wxFileName fn2(sDefaultDir,fn.GetFullName());
    wxString sDefaultFileName = nwxFileUtil::SetupFileName(
      fn2.GetFullPath(),sDefaultExt);
    wxFileName fnDefaultFile(sDefaultFileName);
    sTitle.Alloc(sExportType.Len() + 12);
    sTitle = "Export ";
    sTitle.Append(sExportType);
    bOK = false;
    bool bCancel = false;
    wxFileDialog dlg(
      this,sTitle,fnDefaultFile.GetPath(),fnDefaultFile.GetFullName(),
      sType,
      wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if(dlg.ShowModal() == wxID_OK)
    {
      wxXml2SimpleErrorReceiver errorReceiver;
      wxString sFile = dlg.GetPath();
      bOK = CDialogExportFile::Run(
        this,pExport,fn.GetFullPath(),sFile, &bCancel);
      if(bOK)
      {
        wxFileName fn1(sFile);
        parm->SetLastExportDirectory(fn1.GetPath());
      }
      else if(!bCancel)
      {
        wxString sError;
        wxString sAppend;
        const std::vector<wxString> &asErrors = errorReceiver.GetErrors();
        size_t nCount = asErrors.size();
        const size_t MAX_ERRORS = 5;
        if(nCount > MAX_ERRORS)
        {
          nCount = MAX_ERRORS;
          sAppend = "\n........\n\n"
            "There are more messages, select \"Message Log\"\n"
            "from the \"Tools\" pulldown menu to view them.";
        }
        for(size_t ndx = 0; ndx < nCount; ndx++)
        {
          sError.Append(asErrors.at(ndx));
          sError.Append("\n");
        }
        sError.Append(sAppend);
        nwxString::Trim(&sError);
        if(!sError.IsEmpty())
        {
          mainApp::ShowAlert(sError,this);
        }
      }
    }
    else
    {
      bCancel = true;
    }
  }
}

void CFrameAnalysis::OnArchiveCreate(wxCommandEvent &)
{
  static const wxString sCONFIRM(
      "The analysis file has been modified and must\n"
      "be saved before the data are archived.\n"
      "Would you like to save the analysis file now?");
  bool bOK = PromptSaveFileNow(sCONFIRM);
  if(bOK)
  {
    CArchiveCollection orz(m_pOARfile);
    if(!orz.HasPlotFiles())
    {
      mainApp::ShowError(wxT("No plot files found,\nCannot create an archive."),this);
    }
    else
    {
      bool bIncludeInput = false;
      bool bShowFile = false;
      bOK = false;
      {
        CDialogArchiveCreate dlgc(&orz,this);
        if(dlgc.ShowModal() == wxID_OK)
        {
          bIncludeInput = dlgc.IncludeInputFiles();
          bShowFile = dlgc.ShowFileLocation();
          bOK = true;
        }
        // in braces to destroy dialog window here
      }
      if(bOK)
      {
        wxFileName fn(m_pOARfile->GetLastFileName());
        wxString sDefaultFileName = nwxFileUtil::SetupFileName
          (fn.GetFullPath(),wxT(".orz"));
        wxFileName fnDef(sDefaultFileName);
        wxFileDialog dlg(
          this,
          wxT("Create OSIRIS Archive"),
          fnDef.GetPath(),
          fnDef.GetFullName(),
          FILE_TYPE_ARCHIVE, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if(dlg.ShowModal() == wxID_OK)
        {
          //  save archive here
          wxString sFile = dlg.GetPath();
          bOK = false;
          {
            wxBusyCursor x;
            bOK = orz.WriteArchive(sFile,bIncludeInput);
          }
          if(!bOK)
          {
            mainApp::ShowError(
              wxT("Cannot create archive file.\nCheck file and folder permissions."),
              this);
          }
          else if(bShowFile)
          {
            nwxFileUtil::ShowFileFolder(sFile);
          }
        }
      }
    }
  }
}
void CFrameAnalysis::OnExportCMF(wxCommandEvent &)
{
  ExportCMF();
}

IMPLEMENT_PERSISTENT_SIZE(CFrameAnalysis)
IMPLEMENT_ABSTRACT_CLASS(CFrameAnalysis,CMDIFrame)

BEGIN_EVENT_TABLE(CFrameAnalysis,CMDIFrame)
EVT_PERSISTENT_SIZE(CFrameAnalysis)

EVT_CONTEXT_MENU(CFrameAnalysis::OnContextMenu)
EVT_COMMAND(IDhistoryButton,CEventHistory,CFrameAnalysis::OnHistoryUpdate)
EVT_COMMAND(wxID_ANY,wxEVT_ALERT_VIEW_CHANGED,CFrameAnalysis::OnChangeAlertView)
EVT_COMMAND_ENTER(IDsplitterWindow,CFrameAnalysis::OnCheckSplitter)

EVT_BUTTON(IDExportCMF,CFrameAnalysis::OnExportCMF)
//EVT_BUTTON(IDmenuEditCell,  CFrameAnalysis::OnEdit)
EVT_TOGGLEBUTTON(IDmenuTogglePreview,  CFrameAnalysis::OnTogglePreview)
EVT_BUTTON(IDmenuSampleTile,CFrameAnalysis::OnShowSampleAndGraphic)
EVT_BUTTON(IDmenuDisplaySample,CFrameAnalysis::OnShowSample)
EVT_BUTTON(IDmenuDisplayGraph, CFrameAnalysis::OnShowGraphic)
EVT_BUTTON(IDmenuParameters, CFrameAnalysis::OnShowDetails)
EVT_COMBOBOX(IDgraphLabelsCombo, CFrameAnalysis::OnLabelTypeChanged)
EVT_COMBOBOX(IDgraphNameCombo, CFrameAnalysis::OnLabelNameChanged)

EVT_GRID_CMD_LABEL_LEFT_DCLICK(IDgrid,CFrameAnalysis::OnGridGraph)
EVT_GRID_CMD_CELL_LEFT_DCLICK(IDgrid,CFrameAnalysis::OnGridCellGraph)
EVT_GRID_CMD_LABEL_LEFT_CLICK(IDgrid,CFrameAnalysis::OnLabelClick)
EVT_GRID_CMD_LABEL_RIGHT_CLICK(IDgrid,CFrameAnalysis::OnRightClickCell)
EVT_GRID_CMD_CELL_RIGHT_CLICK(IDgrid,CFrameAnalysis::OnRightClickCell)
EVT_GRID_CMD_CELL_LEFT_CLICK(IDgrid,CFrameAnalysis::OnClickCell)

EVT_GRID_CMD_LABEL_LEFT_DCLICK(IDgridLocus,CFrameAnalysis::OnEditFromGrid)
EVT_GRID_CMD_CELL_LEFT_DCLICK(IDgridLocus,CFrameAnalysis::OnEditFromGrid)
EVT_GRID_CMD_LABEL_RIGHT_CLICK(IDgridLocus,CFrameAnalysis::OnEditMenu)
EVT_GRID_CMD_CELL_RIGHT_CLICK(IDgridLocus,CFrameAnalysis::OnEditMenu)

EVT_SET_FOCUS(CFrameAnalysis::OnFocusSet)
EVT_KILL_FOCUS(CFrameAnalysis::OnFocusKill)

EVT_COMMAND(wxID_ANY, CEventRepaint,CFrameAnalysis::_OnRepaint)
EVT_COMMAND(wxID_ANY, CEventRestoreScroll,CFrameAnalysis::_OnRestoreScroll)

EVT_MENU(wxID_SAVEAS, CFrameAnalysis::OnSave)
EVT_MENU(wxID_SAVE, CFrameAnalysis::OnSave)


EVT_CLOSE(CFrameAnalysis::OnClose)
END_EVENT_TABLE()

