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

*  FileName: mainFrame.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include "mainApp.h"
#include <wx/file.h>
#include <wx/utils.h>
#include "CFrameAnalysis.h"
#include "CFrameRunAnalysis.h"
#include "CFramePlot.h"

#include "OsirisFileTypes.h"
#include "OsirisVersion.h"

#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"

#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/mimetype.h>
#include <wx/dnd.h>
#include <wx/colordlg.h>
#include <wx/gdicmn.h>

#include "nwx/CIncrementer.h"
#include "nwx/nwxXmlMRU.h"
#include "nwx/nwxKeyState.h"
// #include "nwx/CPointerHold.h"
#include "nwx/nwxTimerReceiver.h"

#include "CPlotData.h"
#include "CMenuBar.h"
#include "COsirisIcon.h"

#include "CDialogVolumes.h"
#include "CLabSettings.h"
#include "CLabSettingsFileName.h"
#include "CDialogAbout.h"
#include "CDialogContactUs.h"
#include "CDialogEditGridColours.h"
#include "CDialogReallyQuit.h"
#include "CDialogExportXSL.h"

#include "UnitTest.h"

DEFINE_EVENT_TYPE(CEventKillWindow)
DEFINE_EVENT_TYPE(CEventDragDropDelay)

class CFileDropTarget : public wxFileDropTarget
{
private:
public:
  CFileDropTarget(mainFrame *pFrame);
  static CFileDropTarget *Make(mainFrame *pFrame)
  {
    CFileDropTarget *p = new CFileDropTarget(pFrame);
    return p;
  }
  virtual ~CFileDropTarget();
  virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);
  const wxArrayString &GetFiles()
  {
    return m_sFiles;
  }
private:
  wxArrayString m_sFiles;
  mainFrame *m_pFrame;
};

CFileDropTarget::CFileDropTarget(mainFrame *pFrame) : m_pFrame(pFrame)
{
  wxMDIClientWindow *pWin = pFrame->GetClientWindow();
  pWin->SetDropTarget(this);
}

CFileDropTarget::~CFileDropTarget()
{
  return;
}

bool CFileDropTarget::OnDropFiles(wxCoord, wxCoord, const wxArrayString &filenames)
{
  m_sFiles = filenames;
  m_pFrame->DropFiles();
  return true;
}

//*************************************** mainFrame

wxSize mainFrame::g_size80(0,0);
wxSize mainFrame::g_size90(0,0);
wxPoint mainFrame::g_point5(0,0);
const char * const mainFrame::NOFIND("Cannot find the lab settings file");
const size_t mainFrame::MAX_FRAMES = 20;
const int mainFrame::MRU_NO_WARNING = 1111;

void mainFrame::SetupSize()
{
  wxRect r = wxGetClientDisplayRect();
  int nw = r.GetWidth();
  int nh = r.GetHeight();
//  int nw = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
//  int nh = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
  int nx = (nw << 2) / 5;
  int ny = (nh << 2) / 5;
  g_size80.SetHeight(ny);
  g_size80.SetWidth(nx);
  nx = nw * 9 / 10;
  ny = nh * 9 / 10;
  g_size90.SetHeight(ny);
  g_size90.SetWidth(nx);
  g_point5.x = ((nw - nx) >> 1) + r.GetX();;
  g_point5.y = ((nh - ny) >> 1) + r.GetY();
}
const wxSize &mainFrame::Size80()
{
  if(!g_size80.GetHeight())
  {
    SetupSize();
  }
  return g_size80;
}
const wxSize &mainFrame::Size90()
{
  if(!g_size90.GetHeight())
  {
    SetupSize();
  }
  return g_size90;
}
const wxPoint &mainFrame::Point5()
{
  if(!g_size90.GetHeight())
  {
    SetupSize();
  }
  return g_point5;
}

CMDIFrame *mainFrame::INIT_LAST_ACTIVE((CMDIFrame *)1);

mainFrame::mainFrame() :
  wxMDIParentFrame(NULL, wxID_ANY, _T("OSIRIS"),
        Point5(), Size90(),
        wxDEFAULT_FRAME_STYLE),
    XXX(),
    m_kitColors(),
    m_pDialogOpen(NULL),
    m_pDialogMRU(NULL),
//    m_pVolumes(NULL),
//    m_pDlgAnalysis(NULL),
    m_pDialogErrorLog(NULL),
    m_pTimer(NULL),
    m_pLastMenuShown(NULL),
    m_pColourEditDialog(NULL),
#if HAS_CUSTOM_COLORS
    m_pDialogColour(NULL),
#endif
    m_pDropTarget(NULL),
    m_pAllLoci(NULL),
    m_nTimerCount(0),
    m_pLastActive(INIT_LAST_ACTIVE)
{
  CreateStatusBar(1);
  SetMenuBar(m_pMenuBar = new CMenuBar);
  m_pMenuBar->DisableTableGraph();
  SetupTimer();
  COsirisIcon x;
  SetIcon(x);
  m_pDialogErrorLog = new nwxDialogLog(
    this,wxID_ANY,_T("OSIRIS Message Log"));
  m_pDialogErrorLog->SetIcon(x);
  wxLog *pLog = m_pDialogErrorLog->SetTarget();
  delete pLog;
  m_pDropTarget = CFileDropTarget::Make(this);
  // m_pLastActive was set to 1 so that it would
  // be updated to NULL in CheckActiveFrame()
  CheckActiveFrame();
}

mainFrame::~mainFrame()
{

  wxMDIClientWindow *pWin = GetClientWindow();
  pWin->SetDropTarget(NULL);

  if(m_pDialogMRU != NULL)
  {
    delete m_pDialogMRU;
  }
  if(m_pDialogOpen != NULL)
  {
    delete m_pDialogOpen;
  }
  if(m_pTimer != NULL)
  {
    delete m_pTimer;
  }
  if(m_pAllLoci != NULL)
  {
    delete m_pAllLoci;
  }
  if(m_pColourEditDialog != NULL)
  {
    delete m_pColourEditDialog;
  }
#if HAS_CUSTOM_COLORS
  if(m_pDialogColour != NULL)
  {
    delete m_pDialogColour;
  }
#endif
  delete m_pDialogErrorLog;
}
#ifdef __WXMAC__
bool mainFrame::Show(bool bShow)
{
  // NEVER SHOW ON A macintosh
  bool bRtn = IsShown() != bShow;
  wxMDIParentFrame::Show(false);
  return bRtn;
}
#endif
bool mainFrame::ShowWindow(bool bHasArgs)
{
#ifdef __WXMAC__
  Iconize(true);
  bool bRtn = true;
#else
  bool bRtn = Show(true);
#endif
  if(bRtn && !bHasArgs)
  {
    CParmOsirisGlobal parm;
    if(parm->GetStartupMRU())
    {
      // send event to start up with the MRU window
      wxCommandEvent e(wxEVT_COMMAND_MENU_SELECTED,IDlistMRU);
      e.SetInt(MRU_NO_WARNING);
      AddPendingEvent(e);
    }
  }
  return bRtn;
}

wxSize mainFrame::GetChildSize(int nPct)
{
  wxSize sz = GetClientWindow()->GetSize();
  wxStatusBar *pBar = GetStatusBar();
  if(pBar)
  {
    wxSize szBar = pBar->GetSize();
    int y = sz.GetHeight() - szBar.GetHeight();
    sz.SetHeight(y);
  }
  sz.SetHeight(sz.GetHeight() * nPct / 100);
  sz.SetWidth(sz.GetWidth() * nPct / 100);
  return sz;
}
bool mainFrame::ReAnalyzeSamples(
  const CVolume &vol,
  const CParmOsiris &parmNew)
{
  bool bRtn = false;
  if(!CheckMaxFrames(false))
  {
    size_t nCount = 0;
    CFrameRunAnalysis *pFrame = NULL;
    {
      wxBusyCursor BUSY;
      pFrame = 
        new CFrameRunAnalysis(this,GetChildSize(50),vol,parmNew);
      CDirList *pList = pFrame->GetDirList();
      nCount = pList->GetCount();
      if(nCount)
      {
        pFrame->Show(true);
        pFrame->Run();
        bRtn = true;
      }
    } // destroy wxBusyCursor BUSY;
    if(!nCount)
    {
      wxString sError(
        _T("Could not reanalyze data because\n"
           "no valid directories were found."));
      mainApp::ShowError(sError,this);
      pFrame->Destroy();
      bRtn = false;
    }
  }
  return bRtn;
}
bool mainFrame::ReAnalyze(
  CFrameRunAnalysis *pPrev, 
  const CVolume &vol,
  const CParmOsiris &parmNew)
{
  bool bRtn = false;
  if(!CheckMaxFrames(false))
  {
    CFrameRunAnalysis *pFrame = new CFrameRunAnalysis(pPrev,vol,parmNew);
    CDirList *pList = pFrame->GetDirList();
    size_t nCount = pList->GetCount();
    if(nCount)
    {
      pFrame->Show(true);
      pFrame->Run();
      bRtn = true;
    }
    else
    {
      wxString sError(
        _T("Could not reanalyze data because\n"
           "no valid directories were selected."));
      mainApp::ShowError(sError,this);
      pFrame->Destroy();
      bRtn = false;
    }
  }
  return bRtn;
}
bool mainFrame::OpenBatchFile(const wxString &sFileName)
{
  bool bRtn = true;
  CFrameRunAnalysis *pFrame = new CFrameRunAnalysis(this,GetChildSize(50),sFileName);
  CDirList *pList = pFrame->GetDirList();
  if(pList->GetCount())
  {
    pFrame->Show(true);
    AddToMRU(sFileName);
  }
  else
  {
    wxString sError(_T("ERROR: Could not open file\n"));
    sError.Append(sFileName);
    mainApp::ShowError(sError,this);
    pFrame->Destroy();
    bRtn = false;
  }
  return bRtn;
}

CFramePlot *mainFrame::OpenGraphicFile(
  const wxString &sFileName,
  const wxString &sLocus,
  COARfile *pFile)
{
  auto_ptr<CPlotData> pData(new CPlotData());
  pData->LoadFile(sFileName);
  CFramePlot *pPlot(NULL);
  if(pData->GetChannelCount())
  {
    wxSize sz = GetChildSize();
    bool bShiftKeyDown = nwxKeyState::Shift();
    unsigned int nChannel = pData->GetChannelFromLocus(sLocus);
    pPlot = new CFramePlot(this,sz,pData.release(),pFile,&m_kitColors,bShiftKeyDown,nChannel);
    // call ZoomOut or ZoomToLocus AFTER Show as a workaround for a bug in wxPlotCtrl
    // otherwise we would put it in the CFramePlot constructor
    if((nChannel > 0) && !sLocus.IsEmpty())
    {
      pPlot->ZoomToLocus(sLocus,2);
      pPlot->Show(true);
      if(!bShiftKeyDown)
      {
        pPlot->SetFocusPlot(nChannel - 1);
        // pPlot->SendScrollPlotEvent(nChannel,6);
      }
    }
    else
    {
      pPlot->ZoomOut(false,2);
      pPlot->Show(true);
    }
    AddToMRU(sFileName);
  }
  else
  {
    wxString sMsg(sFileName);
    sMsg.Append(_T(" is not a valid graphic file."));
    ErrorMessage(sMsg);
  }
  return pPlot;
}

void mainFrame::OnQuit(wxCommandEvent &)
{
  wxBusyCursor xxx;
  Close();
}

void mainFrame::OnOpen(wxCommandEvent &)
{
  if(!CheckMaxFrames(true))
  {
    OpenFileDialog(TYPE_REPORT);
  }
}

void mainFrame::OpenFileDialog(OSIRIS_FILE_TYPE x)
{
  if(!CheckMaxFrames(true))
  {
    if(m_pDialogOpen == NULL)
    {
      CParmOsirisGlobal parm;
      const char *psReport = FILE_TYPE_REPORT;
      const char *psPlot =   FILE_TYPE_GRAPHIC;
      const char *psBatch =  FILE_TYPE_BATCH;
  #undef FILE_TYPE
      wxString sTypes(_T(psReport));
      sTypes.Append(_T("|"));
      sTypes.Append(_T(psPlot));
      sTypes.Append(_T("|"));
      sTypes.Append(_T(psBatch));
      wxString sFilePath = parm->GetOutputDirectory();
      m_pDialogOpen = new wxFileDialog(
        this,_T("Open File"),sFilePath,wxEmptyString,
        sTypes,
        wxFD_OPEN | wxFILE_MUST_EXIST );
    }
    else
    {
      m_pDialogOpen->SetFilename(wxEmptyString);
    }
    m_pDialogOpen->SetFilterIndex((int) x);
    if(m_pDialogOpen->ShowModal() == wxID_OK)
    {
      wxString sFileName = m_pDialogOpen->GetPath();
      OpenFileCheckNewer(sFileName);
    }
  }
}

void mainFrame::OpenFileCheckNewer(const wxString &_sFileName)
{
  bool bOpen = true;
  wxString sFileName(_sFileName);
  if(IS_ANALYSIS_FILE(sFileName))
  {
    wxFileName fn(sFileName);
    wxString sNewest = COARfile::FindAnalysisFile(fn.GetPath());
    if( (!nwxString::FileNameStringEqual(sNewest,sFileName)) &&
        (FindWindowByName(sNewest) == NULL) )
    {
      int n = _PromptNewerFile(sFileName,sNewest);
      if(n == wxID_YES)
      {
        sFileName = sNewest;
      }
      else if(n == wxID_CANCEL)
      {
        bOpen = false;
      }
    }
  }
  if(bOpen)
  {
    OpenFile(sFileName);
  }
}

void mainFrame::OnOpenPlot(wxCommandEvent &)
{
  if(!CheckMaxFrames(true))
  {
    OpenFileDialog(TYPE_PLOT);
  }
}
void mainFrame::OnOpenBatch(wxCommandEvent &)
{
  if(!CheckMaxFrames(true))
  {
    OpenFileDialog(TYPE_BATCH);
  }
}


void mainFrame::OnAnalyze(wxCommandEvent &)
{
  bool bDone = false;
  while( (!bDone) && (!CheckMaxFrames(true)) )
  {
    CDialogAnalysis dlg(this);
//    CPointerHold<CDialogAnalysis> x(m_pDlgAnalysis,&dlg);
    bDone = true;
    if(!dlg.IsOK())
    {
      mainApp::ShowError(dlg.GetErrorMsg(),this);
    }
    else if(dlg.ShowModal() == wxID_OK)
    {
      CFrameRunAnalysis *pFrame = NULL;
      {
        wxBusyCursor BUSY;
        pFrame = new CFrameRunAnalysis(this,GetChildSize(50),dlg.GetVolume());
        pFrame->Show(true);
      }
      pFrame->Run();
      if(!pFrame->GetDirListCount())
      {
        pFrame->Destroy();
        bDone = false;
      }
    }
  }
}

bool mainFrame::_VerifyClose()
{
  bool bRtn = CDialogReallyQuit::ReallyQuit(this);
  return bRtn;
}
void mainFrame::OnClose(wxCloseEvent &e)
{
  bool bDone = true;
  if(!_VerifyClose())
  {
    bDone = false;
  }
//  if( (!m_MDImgr.EditedFiles()) && (!_VerifyClose()) )
//  {
//    // no edited files, user does not want to exit
//    bDone = false;
//  }
  else
  {
    bDone = m_MDImgr.CloseAll();
  }
  if(bDone)
  {
    m_pMenuBar->Cleanup();
    if(m_pTimer != NULL)
    {
      m_pTimer->Stop();
    }
    e.Skip();
//    Destroy();
  }
}
#if HAS_CUSTOM_COLORS
bool mainFrame::ShowColourDialog(wxColour *pColour)
{
  // return true if color changed false otherwise
  bool bRtn = false;
  if(m_pDialogColour == NULL)
  {
    m_pDialogColour = new wxColourDialog(this);
  }
  wxColourData &cd(m_pDialogColour->GetColourData());
  cd.SetColour(*pColour);
  cd.SetChooseFull(true);
  m_pDialogColour->CentreOnParent();
  if(m_pDialogColour->ShowModal() == wxID_OK)
  {
    wxColour &cNew = m_pDialogColour->GetColourData().GetColour();
    if(cNew != *pColour)
    {
      bRtn = true;
      *pColour = cNew;
    }
  }
  return bRtn;
}
#else
bool mainFrame::ShowColourDialog(wxColour *pColour)
{
  wxColour c = wxGetColourFromUser(this,*pColour,_T("Colors"));
  bool bRtn = false;
  if(c != *pColour)
  {
    *pColour = c;
    bRtn = true;
  }
  return bRtn;
}
#endif
void mainFrame::OnRecentFiles(wxCommandEvent &e)
{
  nwxXmlMRU *pMRU(mainApp::GetMRU());
  pMRU->ReloadFile();
  if(pMRU->Size() > 0)
  {
    {
      wxBusyCursor bzc;
      if(m_pDialogMRU == NULL)
      {
        int nStyle = MRU_STYLE_CLEANUP | MRU_STYLE_OPEN_STARTUP;
        m_pDialogMRU = new CDialogMRU(mainApp::GetMRU(),this,nStyle);
      }
      else
      {
        m_pDialogMRU->TransferDataToWindow();
        m_pDialogMRU->ClearSelection();
      }
      wxSize sz = GetSize();
      sz.SetWidth(sz.GetWidth() * 8 / 10);
      sz.SetHeight(sz.GetHeight() >> 1);
      m_pDialogMRU->SetSize(sz);
      m_pDialogMRU->CentreOnParent();
    } // bracket will remove busy cursor before showing window
    bool bBefore = m_pDialogMRU->StartupChecked();
    if(m_pDialogMRU->ShowModal() == wxID_OK)
    {
      wxBusyCursor xxx;
      m_pDialogMRU->LoadFiles();
    }
    bool bAfter = m_pDialogMRU->StartupChecked();
    if(bBefore != bAfter)
    {
      m_pDialogMRU->UpdateStartup();
    }
  }
  else if(e.GetInt() != MRU_NO_WARNING)
  {
    mainApp::ShowError(_T("There are no recent files...."),this);
  }
}
void mainFrame::OnEditGridColours(wxCommandEvent &)
{
  if(m_pColourEditDialog == NULL)
  {
    m_pColourEditDialog = new CDialogEditGridColours(this);
  }
  else
  {
//    m_pColourEditDialog->TransferDataToWindow();
  }
  
  if(m_pColourEditDialog->ShowModal() && m_pColourEditDialog->IsModified())
  {
    m_MDImgr.RefreshAllOAR();
  }
}
void mainFrame::OnLabSettings(wxCommandEvent &)
{
  {
    CDialogVolumes dlg(this);
//    CPointerHold<CDialogVolumes> x(m_pVolumes,&dlg);
    dlg.ShowModal();
  }
  return;
}
void mainFrame::OnExportSettings(wxCommandEvent &)
{
  bool bMod = false;
  {
    CDialogExportXSL x(this);
    x.ShowModal();
    bMod = x.IsModified();
  }
  if(bMod)
  {
    m_pMenuBar->UpdateFileMenu();
  }
}

void mainFrame::_CheckDragDropQueue()
{
  list<wxString>::iterator itr = m_lsDragDropQueue.begin();
  if(itr != m_lsDragDropQueue.end())
  {
    wxString s = *itr;
    m_lsDragDropQueue.erase(itr);
    OpenFile(s);
  }
}

void mainFrame::OnTimer(wxTimerEvent &e)
{
  if(!m_nTimerCount)
  {
    CIncrementer incr(m_nTimerCount);
#ifdef _DEBUG
    UnitTest::Run();
#endif
    nwxTimerReceiver::DispatchTimer(e);
/*
   // 1/6/11 - removed code and started using nwxTimerReceiver
    m_MDImgr.OnTimer(e);
    if(m_pVolumes != NULL)
    {
      m_pVolumes->OnTimer(e);
    }
    if(m_pDlgAnalysis != NULL)
    {
      m_pDlgAnalysis->OnTimer(e);
    }
*/
#if CHECK_FRAME_ON_TIMER
    CheckActiveFrame();
#endif
    if(!m_lsDragDropQueue.empty())
    {
      _CheckDragDropQueue();
    }
  }
}
void mainFrame::DisableMenus()
{
  m_pMenuBar->DisableTableGraph();
}
void mainFrame::SetupMenus()
{
  wxMenu *pMenuTable(NULL);
  wxMenu *pMenuGraph(NULL);
  if( (m_pLastActive != NULL) && FindWindow(m_pLastActive))
  {
    pMenuTable = m_pLastActive->GetTableMenu();
    pMenuGraph = m_pLastActive->GetGraphMenu();
  }
  m_pMenuBar->SetTableGraph(pMenuTable,pMenuGraph);
}
void mainFrame::CheckActiveFrame()
{
  CMDIFrame *pActive = (CMDIFrame *)GetActiveChild();
  if( (pActive != m_pLastActive) && FindWindow(pActive) )
  {
    m_pLastActive = pActive;
    SetupMenus();
    CheckSaveStatus();
    m_pLastActive->UpdateStatusBar();
  }
  else if(pActive == NULL)
  {
    ClearStatusText();
  }
}
void mainFrame::CheckSaveStatus()
{
  bool bSaveAs = false;
  bool bSave = false;
  CFrameAnalysis *pFrame = GetAnalysisFrame();
  if(pFrame != NULL)
  {
    bSaveAs = pFrame->CanSaveAs();
    bSave = bSaveAs && pFrame->CanSave();
  }
  m_pMenuBar->EnableSave(bSaveAs,bSave,bSaveAs);
}

void mainFrame::OnHelp(wxCommandEvent &)
{
  wxString sPath = mainApp::GetConfig()->GetExePath();
  bool bError = true;
  sPath.Append(_T("OsirisHelp.pdf"));
  wxFileName fn(sPath);
  if(fn.IsFileReadable())
  {
    wxMimeTypesManager mime;
    auto_ptr<wxFileType> apft(
      mime.GetFileTypeFromExtension(_T("pdf")));
    wxFileType *pft = apft.get();
    wxString sCmd;
    if(pft != NULL)
    {
      sCmd = pft->GetOpenCommand(sPath);
    }
    if(sCmd.Len() && (::wxExecute(sCmd,wxEXEC_ASYNC,NULL) != 0))
    {
      bError = false;
    }
  }
  if(bError)
  {
    ErrorMessage(_T("Cannot find help file"));
  }
}
void mainFrame::OnCheckForUpdates(wxCommandEvent &)
{
  wxString sVersion = OSIRIS_VERS;
  wxString sURL(_T(
    "http://www.ncbi.nlm.nih.gov/projects/SNP/osiris/version.cgi?OS="
    OSIRIS_OS "&V="));
  wxString sTime;
  wxDateTime t;
  t.SetToCurrent();
  sTime.Printf(_T("%lu"),(unsigned long)t.GetTicks());
  sVersion.Replace(_T(" "),_T("+"),true);
  sURL += sVersion;
  sURL += _T("&T=");
  sURL += sTime;
  if(!wxLaunchDefaultBrowser(sURL,0))
  {
    ErrorMessage(_T("Cannot run a browser to check this version of OSIRIS"));
  }
}
void mainFrame::OnAbout(wxCommandEvent &)
{
  CDialogAbout x(this);
  x.ShowModal();
}
void mainFrame::OnContactUs(wxCommandEvent &)
{
  CDialogContactUs x(this);
  x.ShowModal();
}

void mainFrame::OnMaxLadderLabels(wxCommandEvent &)
{
  CDialogMaxLadderLabel x(this);
  bool b = x.EditValue();
  if(b)
  {
    m_MDImgr.UpdateLadderLabels();
  }
}
CFrameAnalysis *mainFrame::GetAnalysisFrame()
{
  CFrameAnalysis *p = 
    ( (m_pLastActive != NULL) && (m_pLastActive != INIT_LAST_ACTIVE) &&
        (m_pLastActive->GetType() == CMDIFrame::FRAME_ANALYSIS) )
    ? (CFrameAnalysis *)m_pLastActive
    : NULL;
  return p;
}
bool mainFrame::CheckMaxFrames(bool bShowError)
{
  bool bRtn = (m_MDImgr.Size() >= MAX_FRAMES);
  if(bRtn && bShowError)
  {
    wxString sError(
      _T("The number of windows has reached\n"
         "the allowable maximum of "));
    sError.Append(nwxString::FormatNumber((int)MAX_FRAMES));
    mainApp::ShowError(sError,this);
    m_lsDragDropQueue.clear();
  }
  return bRtn;
}
void mainFrame::OpenFile(
  const wxString &sFileName,
  const wxString &sLocus,
  COARfile *pOARfile)
{
  CMDIFrame *pFrame = FindWindowByName(sFileName,false);
  if(pFrame != NULL)
  {
    if(pFrame->GetType() == CMDIFrame::FRAME_PLOT)
    {
      CFramePlot *pF = (CFramePlot *) pFrame;
      bool bShiftKeyDown = nwxKeyState::Shift();
      pF->SetOARfile(pOARfile);
      pF->ReInitialize(sLocus,bShiftKeyDown);
    }
    pFrame->RaiseWindow();
  }
  else if(!wxFileName::IsFileReadable(sFileName))
  {
    wxString sError(_T("Cannot open file:\n"));
    sError.Append(sFileName);
    mainApp::ShowError(sError,this);
    if(!wxFileName::FileExists(sFileName))
    {
      RemoveFromMRU(sFileName);
    }
  }
  else if(CheckMaxFrames(true)) {;}
  else if(FileInProgress(sFileName,true)) {;}
  else
  {
    wxBusyCursor bzc;
    wxString sFileLower(sFileName);
    sFileLower.MakeLower();
    if(sFileLower.EndsWith(_T(EXT_GRAPHIC)))
    {
      OpenGraphicFile(sFileName, sLocus, pOARfile);
    }
    else if(sFileLower.EndsWith(_T(EXT_BATCH)))
    {
      OpenBatchFile(sFileName);
    }
    else if(pOARfile != NULL)
    {
      OpenAnalysisFile(pOARfile);
    }
    else if(
      (pOARfile = m_MDImgr.FindOARfile(sFileName)) != NULL)
    {
      // if pOARfile is being used, do not open another
      OpenAnalysisFile(pOARfile);
    }
    else
    {
      OpenAnalysisFile(sFileName);
    }
  }
}

void mainFrame::_CheckAnalysisFile(CFrameAnalysis *pWin)
{
  if(!pWin->FileError())
  {
    pWin->Show(true);
    pWin->Refresh();
    AddToMRU(pWin->GetFileName());
  }
  else
  {
    pWin->Destroy();
  }
  CheckSaveStatus();
}
void mainFrame::OpenAnalysisFile(const wxString &sFileName)
{
  CFrameAnalysis *pWin(new CFrameAnalysis(this,GetChildSize(),sFileName));
  _CheckAnalysisFile(pWin);
}
void mainFrame::OpenAnalysisFile(COARfile *pFile)
{
  if(pFile->GetSampleCount() < 1)
  {
    FileEmptyMessage(pFile->GetFileName());
  }
  else
  {
    CFrameAnalysis *pWin(new CFrameAnalysis(this,GetChildSize(),pFile));
    _CheckAnalysisFile(pWin);
  }
}

void mainFrame::RemoveFromMRU(const wxString &sFileName)
{
  nwxXmlMRU *pMRU = mainApp::GetMRU();
  pMRU->RemoveFile(sFileName);
}
void mainFrame::AddToMRU(const wxString &sFileName)
{
  nwxXmlMRU *pMRU = mainApp::GetMRU();
  pMRU->UpdateFileAccess(sFileName);
}
void mainFrame::FileErrorMessage(const wxString &sFileName)
{
  const char *psFile = sFileName.IsEmpty() ? "This" : sFileName.c_str();
  wxString sMessage = wxString::Format(_T("%s is not a valid file"),psFile);
  mainApp::ShowError(sMessage,this);
}
void mainFrame::FileEmptyMessage(const wxString &sFileName)
{
  const char *psFile = sFileName.IsEmpty() ? "This" : sFileName.c_str();
  wxString sMessage = wxString::Format(_T("%s does not contain any sample data"),psFile);
  mainApp::ShowError(sMessage,this);
}

void mainFrame::ErrorMessage(const wxString &sMessage)
{
  mainApp::ShowError(sMessage,this);
}
void mainFrame::OnDoClose(wxCommandEvent &)
{
  wxBusyCursor xxx;
  wxWindow *pw = GetActiveChild();
  if( (pw != NULL) && (pw->Close()) )
  {
    pw->Destroy();
  }
}
void mainFrame::OnShowLog(wxCommandEvent &)
{
  m_pDialogErrorLog->Show(true);
}
void mainFrame::OnSave(wxCommandEvent &e)
{
  CFrameAnalysis *pf = GetAnalysisFrame();
  if(pf != NULL)
  {
    int n = e.GetId();
    if(n == wxID_SAVE)
    {
      pf->SaveFile();
    }
    else if(n == wxID_SAVEAS)
    {
      pf->SaveFileAs();
    }
  }
}


void mainFrame::OnMenu(wxCommandEvent &e)
{
//  m_MDImgr.OnMenu(e,m_pLastActive);
  if((m_pLastActive == NULL) || 
     (m_pLastActive == INIT_LAST_ACTIVE) ||
      !m_pLastActive->MenuEvent(e))
  {
    e.Skip();
  }
}

void mainFrame::OnMenuOpen(wxMenuEvent &e)
{
  m_pLastMenuShown = e.GetMenu();
}
void mainFrame::OnMenuClose(wxMenuEvent &)
{
  CMDIFrame *pFrame = (CMDIFrame *)GetActiveChild();
  if(pFrame != NULL)
  {
    pFrame->UpdateStatusBar();
  }
}

void mainFrame::OnKillWindow(wxCommandEvent &e)
{
  wxWindow *pFrame = (wxWindow *)e.GetEventObject();
  if( (pFrame != NULL) && (pFrame->Close()) )
  {
    pFrame->Destroy();
  }
}

void mainFrame::DropFiles()
{
  wxCommandEvent ee(CEventDragDropDelay,GetId());
  ee.SetEventObject(this);
  AddPendingEvent(ee);
}
void mainFrame::KillWindow(wxWindow *pFrame)
{
  wxCommandEvent ee(CEventKillWindow,pFrame->GetId());
  ee.SetEventObject(pFrame);
  AddPendingEvent(ee);
}

bool mainFrame::FileExtensionOK(const wxString &s)
{
  wxString sl(s);
  bool bRtn = false;
  static const char *EXT[] = {
    EXT_GRAPHIC,
    EXT_REPORT,
    EXT_REPORT_EDITED,
    EXT_BATCH
  };
  static const size_t EXT_COUNT = sizeof(EXT) / sizeof(EXT[0]);
  sl.MakeLower();
  for(size_t i = 0; i < EXT_COUNT; i++)
  {
    if(sl.EndsWith(_T(EXT[i])))
    {
      bRtn = true;
      i = EXT_COUNT;
    }
  }
  return bRtn;
}


void mainFrame::OpenFiles(const wxArrayString &filenames)
{
  // on drag/drop files

  size_t n = filenames.Count();
  size_t na = n << 3;
  size_t nRead = 0;
  size_t nType = 0;
  size_t nInUse = 0;
  size_t nOK = 0;
  size_t i;
  long nStyle = wxOK;
  wxArrayString asFiles;
  wxString sMsgRead;
  wxString sMsgType;
  wxString sMsgInUse;
  wxString sMsgOK;
  wxString sFileName;
  wxString LF(_T("\n"));
  bool bOK = true;

  sMsgRead.Alloc(na);
  sMsgType.Alloc(na);
  sMsgOK.Alloc(na);
  sMsgInUse.Alloc(na);
  asFiles.Alloc(n);

  for(i =0; i < n; i++)
  {
    sFileName = filenames.Item(i);
    if(!wxFileName::IsFileReadable(sFileName))
    {
      sMsgRead.Append(sFileName);
      sMsgRead.Append(LF);
      nRead++;
    }
    else if(!FileExtensionOK(sFileName))
    {
      sMsgType.Append(sFileName);
      sMsgType.Append(LF);
      nType++;
    }
    else if(FileInProgress(sFileName,false))
    {
      sMsgInUse.Append(sFileName);
      sMsgInUse.Append(LF);
      nInUse++;
    }
    else
    {
      asFiles.Add(sFileName);
      nOK++;
    }
  }
  if( (nType | nRead | nInUse) != 0 )
  {
    if(nType)
    {
      if(nType > 1)
      {
        sMsgOK = _T("The following files are not recognized by OSIRIS:\n");
      }
      else
      {
        sMsgOK = _T("The following file is not recognized by OSIRIS:\n");
      }
      sMsgOK.Append(sMsgType);
      sMsgOK.Append(LF);
    }
    if(nRead)
    {
      if(nRead > 1)
      {
        sMsgOK.Append(_T("The following files are not readable:\n"));
      }
      else
      {
        sMsgOK.Append(_T("The following file is not readable:\n"));
      }
      sMsgOK.Append(sMsgRead);
      sMsgOK.Append(LF);
    }
    if(nInUse)
    {
      if(nInUse > 1)
      {
        sMsgOK.Append(_T("The following files are being created\n"
            "and cannot be opened yet:\n"));
      }
      else
      {
        sMsgOK.Append(_T("The following file is being created\n"
            "and cannot be opened yet:\n"));
      }
      sMsgOK.Append(sMsgInUse);
      sMsgOK.Append(LF);
    }
    if(nOK)
    {
      if(nOK > 1)
      {
        sMsgOK.Append(_T("The following files can be opened:\n"));
      }
      else
      {
        sMsgOK.Append(_T("The following file can be opened:\n"));
      }
      for(i = 0; i < nOK; i++)
      {
        sMsgOK.Append(asFiles[i]);
        sMsgOK.Append(LF);
      }
      sMsgOK.Append(_T("\nDo you wish to continue?"));
      nStyle = wxYES_NO;
    }
    wxMessageDialog dlg(this,sMsgOK,_T("Error"),nStyle | wxICON_ERROR);
    long nRtn = dlg.ShowModal();
    bOK = (nRtn == wxID_YES) && (nOK > 0);
  }
  if(bOK)
  {
    wxArrayString asFilesNew;
    wxArrayString *pasFiles = &asFiles;
    if(_CheckForNewerFiles(asFiles,&asFilesNew))
    {
      pasFiles = &asFilesNew;
      nOK = pasFiles->GetCount();
    }
    for(i = 0; i < nOK; i++)
    {
      m_lsDragDropQueue.push_back(pasFiles->Item(i));
    }
  }
}

bool mainFrame::ContainsEnd(const wxString &sFileName)
{
  wxFile fl(sFileName);
  char sBuffer[16];
  bool bRtn = false;
  memset((void *)sBuffer,0,sizeof(sBuffer));
  if(fl.IsOpened())
  {
    wxFileOffset offs = fl.Length();
    if(offs < 12) { offs = 0;}
    else { offs -= 12; }
    if(fl.Seek(offs) != offs) {;} // problem with seek
    else if(fl.Read((void *) sBuffer, 12) < 6) {;} // could not read at least 6 bytes
    else if(strstr(sBuffer,":end:") != NULL)
    {
      bRtn = true;
    }
  }
  return bRtn;
}
bool mainFrame::FileInProgress(const wxString &sFileName, bool bMessage)
{
  wxFileName fn(sFileName);
  bool bRtn = false;
  if( IS_FILE_TYPE(sFileName,EXT_GRAPHIC) ||
      IS_FILE_TYPE(sFileName,EXT_REPORT) ||
      IS_FILE_TYPE(sFileName,EXT_REPORT_EDITED) ||
      IS_FILE_TYPE(sFileName,EXT_BATCH) )
  {
    wxString sDisplayName = fn.GetFullName();
    fn.SetFullName(_T("Working.txt"));
    if(!fn.FileExists()) {;} // OK
    else
    {
      wxDateTime tNow;
      wxTimeSpan tsMax(0,5); // 5 minutes
      tNow.SetToCurrent();
      tNow = tNow.Subtract(tsMax); // 5 minutes ago
      if(fn.GetModificationTime() >= tNow)
      {
        // file is less than five minutes old
        // look for :end: near the end of the file
        wxString s = fn.GetFullPath();
        bRtn = !ContainsEnd(s);
      }
      if( bRtn && bMessage && !ContainsEnd(fn.GetFullPath()) )
      {
        wxString sMessage(_T("The file, "));
        sMessage.Append(sDisplayName);
        sMessage.Append(_T(", is currently being\n"
          "created and cannot be opened yet."));
        mainApp::ShowError(sMessage,this);
      }
    }
  }
  return bRtn;
}
#ifdef __WXMAC__
wxPoint mainFrame::SelectPosition()
{
  wxPoint ptRtn = wxDefaultPosition;
  wxMDIChildFrame *pFrame = GetActiveChild();
  if(pFrame != NULL)
  {
    ptRtn = pFrame->GetPosition();
    ptRtn.x += 24;
    ptRtn.y += 24;
  }
  return ptRtn;
}
#endif

typedef struct
{
  wxString sLatestFile;
  SET_FILE_NAMES setList;
} 
FILEDIR_ENTRY;


bool mainFrame::_CheckForNewerFiles(
  const wxArrayString &filenames, 
  wxArrayString *psNewFilenames)
{
  typedef map<wxString,FILEDIR_ENTRY,nwxStringLessFileName> MAP_ENTRY;

  // build file name list

  MAP_ENTRY mapDirEntry;
  MAP_ENTRY::iterator itrMap;
  SET_FILE_NAMES setFileNames;
  bool bRtn = false;
  for(size_t i = 0; i < filenames.Count(); i++)
  {
    const wxString &sFileName(filenames.Item(i));
    wxFileName fn(sFileName);
    if(setFileNames.find(sFileName) == setFileNames.end())
    {
      setFileNames.insert(sFileName);
      if(fn.FileExists(sFileName) && IS_ANALYSIS_FILE(sFileName))
      {
        wxString sDir = fn.GetPath();
        itrMap = mapDirEntry.find(sDir);
        if(itrMap == mapDirEntry.end())
        {
          FILEDIR_ENTRY x;
          mapDirEntry.insert(MAP_ENTRY::value_type(sDir,x));
          itrMap = mapDirEntry.find(sDir);
        }
        itrMap->second.setList.insert(sFileName);
      }
    }
  }

  // find newest files

  for(itrMap = mapDirEntry.begin(); itrMap != mapDirEntry.end(); ++itrMap)
  {
    const wxString &sDir = itrMap->first;
    FILEDIR_ENTRY &DirEntry = itrMap->second;
    DirEntry.sLatestFile = COARfile::FindAnalysisFile(sDir);
    if(FindWindowByName(DirEntry.sLatestFile) != NULL)
    {} // already opened
    else if(DirEntry.setList.find(DirEntry.sLatestFile) != DirEntry.setList.end())
    {} // already selected
    else if(DirEntry.setList.size() == 1)
    {
      int n = _PromptNewerFile(*DirEntry.setList.begin(),DirEntry.sLatestFile);
      if(n == wxID_YES || n == wxID_CANCEL)
      {
        SET_FILE_NAMES::iterator itr = setFileNames.find(*DirEntry.setList.begin());
        if(itr == setFileNames.end())
        {
          wxString ss = _T("File not found in list ");
          ss.Append(*DirEntry.setList.begin());
          wxASSERT_MSG(0,ss);
        }
        else
        {
          setFileNames.erase(itr);
        }
        if(n == wxID_YES)
        {
          setFileNames.insert(DirEntry.sLatestFile);
        }
        bRtn = true;
      }
    }
    else
    {
      int n = _PromptNewerFiles(DirEntry.setList, DirEntry.sLatestFile);
      if(n == wxID_YES)
      {
        setFileNames.insert(DirEntry.sLatestFile);
        bRtn = true;
      }
      else if(n == wxID_CANCEL)
      {
        SET_FILE_NAMES::iterator itr;
        SET_FILE_NAMES::iterator itrFind;
        for(itr = DirEntry.setList.begin(); itr != DirEntry.setList.end(); ++itr)
        {
          itrFind = setFileNames.find(*itr);
          if(itrFind == setFileNames.end())
          {
            wxString ss = _T("File not found in list ");
            ss.Append(*itr);
            wxASSERT_MSG(0,ss);
          }
          else
          {
            setFileNames.erase(itrFind);
          }
        }
        bRtn = true;
      }
    }
  }
  if(bRtn)
  {
    SET_FILE_NAMES::iterator itr;
    psNewFilenames->Empty();
    psNewFilenames->Alloc(setFileNames.size());
    for(itr = setFileNames.begin(); itr != setFileNames.end(); ++itr)
    {
      psNewFilenames->Add(*itr);
    }
  }
  return bRtn;
}
void mainFrame::OnDropFiles(wxCommandEvent &)
{
  OpenFiles(m_pDropTarget->GetFiles());
}


BEGIN_EVENT_TABLE(mainFrame,wxMDIParentFrame)
EVT_MENU_OPEN(mainFrame::OnMenuOpen)
EVT_MENU_CLOSE(mainFrame::OnMenuClose)
EVT_MENU(wxID_CLOSE,  mainFrame::OnDoClose)
EVT_MENU(wxID_OPEN,   mainFrame::OnOpen)
EVT_MENU(wxID_EXIT,   mainFrame::OnQuit)
EVT_MENU(IDlistMRU,   mainFrame::OnRecentFiles)
EVT_MENU(IDlab,       mainFrame::OnLabSettings)
EVT_MENU(IDexport,    mainFrame::OnExportSettings)
EVT_MENU(IDeditColours, mainFrame::OnEditGridColours)
EVT_MENU(IDanalyze,   mainFrame::OnAnalyze)
EVT_MENU(IDopenPlot,  mainFrame::OnOpenPlot)
EVT_MENU(IDopenBatch, mainFrame::OnOpenBatch)
EVT_MENU(IDlog,       mainFrame::OnShowLog)
EVT_MENU(IDhelp,      mainFrame::OnHelp)
EVT_MENU(wxID_ABOUT, mainFrame::OnAbout)
EVT_MENU(IDcheckForUpdates, mainFrame::OnCheckForUpdates)
EVT_MENU(IDhelpContactUs, mainFrame::OnContactUs)
EVT_MENU(IDExportGraphic, mainFrame::OnMenu)
EVT_MENU(IDMaxLadderLabels, mainFrame::OnMaxLadderLabels)

EVT_MENU(wxID_SAVEAS, mainFrame::OnSave)
EVT_MENU(wxID_SAVE, mainFrame::OnSave)

EVT_MENU_RANGE(IDmenu_START, IDmenu_END, mainFrame::OnMenu)
EVT_MENU_RANGE(IDExportUser, IDExportUserSubMenu - 1, mainFrame::OnMenu)

EVT_CLOSE(mainFrame::OnClose)
EVT_TIMER(IDtimer, mainFrame::OnTimer)

EVT_COMMAND(wxID_ANY,CEventKillWindow,mainFrame::OnKillWindow)
EVT_COMMAND(wxID_ANY,CEventDragDropDelay, mainFrame::OnDropFiles)
//EVT_SIZE(mainFrame::OnResize)
END_EVENT_TABLE()
