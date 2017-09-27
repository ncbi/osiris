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
#include <wx/settings.h>
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
#include "nwx/nwxFileUtil.h"
#include "nwxZip/nwxZipInput.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxTimerReceiver.h"

#include "CPlotData.h"
#include "CMenuBar.h"
#include "COsirisIcon.h"

#include "CDialogVolumes.h"
#include "CDialogArtifactLabels.h"
#include "CLabSettings.h"
#include "CLabSettingsFileName.h"
#include "CDialogAbout.h"
#include "CDialogContactUs.h"
#include "CDialogEditGridColours.h"
#include "CDialogReallyQuit.h"
#include "CDialogExportXSL.h"

#include "UnitTest.h"

#if DRAG_DROP_FILES
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
  pFrame->GetClientWindow()->SetDropTarget(this);
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

#endif

//*************************************** mainFrame
wxSize mainFrame::GetChildSize(int nPct)
{
#ifdef __NO_MDI__
  wxSize sz = wxGetClientDisplayRect().GetSize();
#else
  wxSize sz = GetClientWindow()->GetSize();
  wxStatusBar *pBar = GetStatusBar();
  if(pBar)
  {
    wxSize szBar = pBar->GetSize();
    int y = sz.GetHeight() - szBar.GetHeight();
    sz.SetHeight(y);
  }
#endif
  sz.SetHeight(sz.GetHeight() * nPct / 100);
  sz.SetWidth(sz.GetWidth() * nPct / 100);
  return sz;
}


#if mainFrameIsWindow
wxSize mainFrame::g_size80(0,0);
wxSize mainFrame::g_size90(0,0);
wxPoint mainFrame::g_point5(0,0);
wxPoint mainFrame::g_point50(0,0);
//wxPoint mainFrame::g_point100(0,0);

const wxSize &mainFrame::Size80()
{
  SetupSize();
  return g_size80;
}
const wxSize &mainFrame::Size90()
{
  SetupSize();
  return g_size90;
}
const wxPoint &mainFrame::GetStartPosition()
{
  SetupSize();
  return g_point5;
}
const wxSize &mainFrame::mainFrameSize()
{
  const wxSize &x = GET_PERSISTENT_SIZE(mainFrame);
  if(x != wxDefaultSize)
  {
    return x;
  }
  return Size90();
}

const wxPoint &mainFrame::mainFramePos()
{
  const wxPoint &x = GET_PERSISTENT_POSITION(mainFrame);
  if(x != wxDefaultPosition)
  {
    return x;
  }
  return GetStartPosition();
}

void mainFrame::SetupSize()
{
   if(!g_size80.GetHeight())
   {
      wxRect r = wxGetClientDisplayRect();
      int nw = r.GetWidth();
      int nh = r.GetHeight();
      int nx = (nw << 2) / 5;
      int ny = (nh << 2) / 5;
      g_size80.SetHeight(ny);
      g_size80.SetWidth(nx);
      g_point50.x = (nw >> 1) + r.GetX();
      g_point50.y = (nh >> 1) + r.GetY();
      nx = nw * 9 / 10;
      ny = nh * 9 / 10;
      g_size90.SetHeight(ny);
      g_size90.SetWidth(nx);
      g_point5.x = ((nw - nx) >> 1) + r.GetX();
      g_point5.y = ((nh - ny) >> 1) + r.GetY();
//      int nws = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
//      int nhs = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
//      g_point100.x = nws + 1;
//      g_point100.y = nhs + 1;
  }
}

#endif

const size_t mainFrame::MAX_FRAMES = 20;
const int mainFrame::MRU_AT_STARTUP = 1111;
const char * const mainFrame::NOFIND("Cannot find the lab settings file");

CMDIFrame *mainFrame::INIT_LAST_ACTIVE((CMDIFrame *)1);

#ifdef __WXDEBUG__
void mainFrame::_LogActiveFrame()
{
  if(m_pLastActive != NULL)
  {
    wxString s(wxS("active frame: "));
    s.Append(m_pLastActive->GetLabel());
    mainApp::LogMessage(s);
  }
}
#endif

void mainFrame::_SetupCommonMenuBar()
{
  CMenuBar *p = new CMenuBar(true,false);
#if defined(__WXMAC__)
  wxMenuBar::MacSetCommonMenuBar(p);
#elif mainFrameIsWindow
  SetMenuBar(p);
#endif
}

int mainFrame::g_mainFrameCount = 0;

mainFrame::mainFrame() :
#if mainFrameIsWindow
  mainFrameSuper(NULL, wxID_ANY, OSIRIS_WINDOW_TITLE,
        mainFramePos(),
        mainFrameSize(),
        wxRESIZE_BORDER | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU | wxDEFAULT_FRAME_STYLE
        ),
//#else
//  macintosh hidden dummy window, hopefully not needed
//  mainFrameSuper(NULL,wxID_ANY, wxEmptyString, wxPoint(0,0),wxSize(0,0),0),
#endif

    XXX(),
    m_pDialogOpen(NULL),
    m_pDialogMRU(NULL),
//    m_pVolumes(NULL),
//    m_pDlgAnalysis(NULL),
    m_pDialogErrorLog(NULL),
    m_pTimer(NULL),
    m_pColourEditDialog(NULL),
#if HAS_CUSTOM_COLORS
    m_pDialogColour(NULL),
#endif
#if DRAG_DROP_FILES
    m_pDropTarget(NULL),
#endif
    m_pAllLoci(NULL),
    m_nTimerCount(0),
    m_pLastActive(INIT_LAST_ACTIVE)
#ifdef MANUALLY_PLACE_FRAMES
    ,m_nFrameSpace(-1)
#endif
    ,m_bDoCloseCalled(false)
{
#ifndef __NO_MDI__
  CreateStatusBar(1);
#endif
  _SetupCommonMenuBar();
  SetupTimer();
  COsirisIcon x;
#if mainFrameIsWindow
  SetIcon(x);
#endif
  m_pDialogErrorLog = new nwxDialogLog(
    DialogParent(),wxID_ANY,"OSIRIS Message Log");
  m_pDialogErrorLog->SetIcon(x);
  m_pDialogErrorLog->SetTarget();
#if DRAG_DROP_FILES
  m_pDropTarget = CFileDropTarget::Make(this);
#endif
  // m_pLastActive was set to 1 so that it would
  // be updated to NULL in CheckActiveFrame()
#ifndef __NO_MDI__
  CheckActiveFrame();
#endif
  ++g_mainFrameCount;
}

  
mainFrame::~mainFrame()
{
  --g_mainFrameCount;
  DoClose();
#if DRAG_DROP_FILES
  GetClientWindow()->SetDropTarget(NULL);
#endif
  if(m_pDialogMRU != NULL)
  {
    m_pDialogMRU->Destroy();
  }
  if(m_pDialogOpen != NULL)
  {
    m_pDialogOpen->Destroy();
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
    m_pColourEditDialog->Destroy();
  }
#if HAS_CUSTOM_COLORS
  if(m_pDialogColour != NULL)
  {
    m_pDialogColour->Destroy();
  }
#endif
  m_pDialogErrorLog->Destroy();
}
bool mainFrame::Startup(bool bHasArgs)
{
#if mainFrameIsWindow
  bool bRtn = Show(true);
#else
  bool bRtn = true;
#endif

  CParmOsirisGlobal parm;
  nwxFileUtil::SetDoNotSelectFile(parm->GetShowFileLocationDir());
  if(bRtn && !bHasArgs)
  {
    if(parm->GetStartupMRU())
    {
      // send event to start up with the MRU window
      wxCommandEvent e(wxEVT_COMMAND_MENU_SELECTED,IDlistMRU);
      e.SetInt(MRU_AT_STARTUP);
#ifdef __WXMSW__
      GetEventHandler()->AddPendingEvent(e);
#else
      AddPendingEvent(e);
#endif
    }
  }
  return bRtn;
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
        "Could not reanalyze data because\n"
           "no valid directories were found.");
      ErrorMessage(sError);
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
        "Could not reanalyze data because\n"
           "no valid directories were selected.");
      ErrorMessage(sError);
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
    wxString sError("ERROR: Could not open file\n");
    sError.Append(sFileName);
    ErrorMessage(sError);
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
    pPlot = new CFramePlot(this,sz,pData.release(),pFile,mainApp::GetKitColors(),bShiftKeyDown,nChannel);
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
    sMsg.Append(" is not a valid graphic file.");
    ErrorMessage(sMsg);
  }
  return pPlot;
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
      const char *psArchive = FILE_TYPE_ARCHIVE;
      wxString sTypes(psReport);
      sTypes.Append("|");
      sTypes.Append(psPlot);
      sTypes.Append("|");
      sTypes.Append(psBatch);
      sTypes.Append("|");
      sTypes.Append(psArchive);
      wxString sFilePath = parm->GetOutputDirectory();
      m_pDialogOpen = new wxFileDialog(
        DialogParent(),"Open File",sFilePath,wxEmptyString,
        sTypes,
        wxFD_OPEN | wxFD_FILE_MUST_EXIST );
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

void mainFrame::OnOpenArchive(wxCommandEvent &)
{
  if(!CheckMaxFrames(true))
  {
    OpenFileDialog(TYPE_ARCHIVE);
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
    CDialogAnalysis dlg(DialogParent());
//    CPointerHold<CDialogAnalysis> x(m_pDlgAnalysis,&dlg);
    bDone = true;
    if(!dlg.IsOK())
    {
      ErrorMessage(dlg.GetErrorMsg());
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
  bool bRtn = CDialogReallyQuit::ReallyQuit(DialogParent());
  return bRtn;
}
bool mainFrame::DoClose()
{
  bool bDone = true;
  if(!m_bDoCloseCalled)
  {
    if(!_VerifyClose())
    {
      bDone = false;
    }
    else
    {
      bDone = m_MDImgr.CloseAll();
    }
    if(bDone)
    {
      m_bDoCloseCalled = true;
      if(m_pTimer != NULL)
      {
        m_pTimer->Stop();
      }
    }
  }
  return bDone;
}

#ifdef MANUALLY_PLACE_FRAMES
void mainFrame::PlaceFrame(CMDIFrame *pWin)
{
  wxRect r = wxGetClientDisplayRect();
  wxPoint pt(r.x,r.y);
  if(m_pLastActive != NULL && m_pLastActive != INIT_LAST_ACTIVE)
  {
    pt = m_pLastActive->GetPosition();

    if(pt.x >= 0 || pt.y >= 0)
    {
      if(m_nFrameSpace < 0)
      {
        m_nFrameSpace = wxSystemSettings::GetMetric(wxSYS_CAPTION_Y,m_pLastActive);
        if(m_nFrameSpace > 0)
        {
          m_nFrameSpace += 2;
        }
        else
        {
          m_nFrameSpace = 24;  // guess
        }
      }
      pt.x += m_nFrameSpace;
      pt.y += m_nFrameSpace;
      if(pt.x < r.x) { pt.x = r.x;}
      if(pt.y < r.y) { pt.y = r.y;}
    }
  }
  pWin->SetPosition(pt);
}
#endif

#if mainFrameIsWindow
void mainFrame::OnClose(wxCloseEvent &e)
{
  if(DoClose()) e.Skip();
}
#endif

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
  wxColour c = wxGetColourFromUser(DialogParent(),*pColour,"Colors");
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
  if( (e.GetInt() != MRU_AT_STARTUP) || !m_MDImgr.Size() )
     // check for startup from opening data file from 
     // finder on ia macintosh
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
          wxSize sz = GetChildSize(80);
          m_pDialogMRU = new CDialogMRU(mainApp::GetMRU(),this,sz,nStyle);
        }
        else
        {
          m_pDialogMRU->TransferDataToWindow();
          m_pDialogMRU->ClearSelection();
        }
//        m_pDialogMRU->SetSize(sz);
//        m_pDialogMRU->CentreOnParent();
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
    else if(e.GetInt() != MRU_AT_STARTUP)
    {
      ErrorMessage("There are no recent files....");
    }
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
    wxBeginBusyCursor();
    CDialogVolumes dlg(DialogParent());
    wxEndBusyCursor();
    dlg.ShowModal();
  }
  return;
}
void mainFrame::OnArtifactLabels(wxCommandEvent &)
{
  {
    wxBeginBusyCursor();
    CDialogArtifactLabels dlg(DialogParent());
    wxEndBusyCursor();
    dlg.ShowModal();
    if(dlg.IsModified())
    {
      m_MDImgr.RefreshAllPlot();
      m_MDImgr.RefreshAllOAR();
    }
  }
  return;
}
void mainFrame::OnExportSettings(wxCommandEvent &)
{
  bool bMod = false;
  {
    CDialogExportXSL x(DialogParent());
    x.ShowModal();
    bMod = x.IsModified();
  }
  if(bMod)
  {
    m_MDImgr.UpdateFileMenu();
  }
}
#if DRAG_DROP_FILES
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
#endif

void mainFrame::OnTimer(wxTimerEvent &e)
{
  if(!m_nTimerCount)
  {
    CIncrementer incr(m_nTimerCount);
#ifdef _DEBUG
    UnitTest::Run();
#endif
    nwxTimerReceiver::DispatchTimer(e);
#if CHECK_FRAME_ON_TIMER && !defined(__NO_MDI__)
    CheckActiveFrame();
#endif
#if DRAG_DROP_FILES
    if(!m_lsDragDropQueue.empty())
    {
      _CheckDragDropQueue();
    }
#endif
  }
}

#ifndef __NO_MDI__
void mainFrame::CheckActiveFrame()
{
  CMDIFrame *pActive = (CMDIFrame *)GetActiveChild();
  if( (pActive != m_pLastActive) && FindWindow(pActive) )
  {
    SetActiveFrame(pActive);
    m_pLastActive->UpdateStatusBar();
  }
  else if(pActive == NULL)
  {
    ClearStatusText();
  }
}
#endif



void mainFrame::OnHelp(wxCommandEvent &)
{
  wxString sPath = mainApp::GetConfig()->GetExePath();
  bool bError = true;
  sPath.Append("OsirisHelp.pdf");
  wxFileName fn(sPath);
  if(fn.IsFileReadable())
  {
    wxMimeTypesManager mime;
    auto_ptr<wxFileType> apft(
      mime.GetFileTypeFromExtension("pdf"));
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
    ErrorMessage("Cannot find help file");
  }
}
void mainFrame::OnCheckForUpdates(wxCommandEvent &)
{
  wxString sVersion = OSIRIS_VERS;
  wxString sURL(
    "https://www.ncbi.nlm.nih.gov/projects/SNP/osiris/version.cgi?OS="
    OSIRIS_OS "&V=");
  wxString sTime;
  wxDateTime t;
  t.SetToCurrent();
  sTime.Printf("%lu",(unsigned long)t.GetTicks());
  sVersion.Replace(" ","+",true);
  sURL += sVersion;
  sURL += "&T=";
  sURL += sTime;
  if(!wxLaunchDefaultBrowser(sURL,0))
  {
    ErrorMessage("Cannot run a browser to check this version of OSIRIS");
  }
}

#ifdef __WINDOW_LIST__
void mainFrame::OnWindowMenu(wxCommandEvent &e)
{
  int nID = e.GetId();
  if(nID >= IDmenuWindow_Frame)
  {
    m_MDImgr.ActivateFromWindowMenu(nID);
  }
#ifdef __WXMAC__
  else
  {
    switch(e.GetId())
    {
    case IDmenuWindow_Minimize:
      if(m_pLastActive != NULL)
      {
        m_pLastActive->Iconize(true);
      }
      break;
    case IDmenuWindow_Zoom:
      if(m_pLastActive != NULL)
      {
        m_pLastActive->Maximize(!m_pLastActive->IsMaximized());
      }
      break;
    case IDmenuWindow_AllToFront:
      m_MDImgr.BringAllToFront();
      break;
    default:
      wxASSERT_MSG(0,wxS("mainFrame::OnWindowMenu invalid ID"));
    }
  }
#endif
}
#endif
void mainFrame::OnAbout(wxCommandEvent &)
{
  CDialogAbout x(DialogParent());
  x.ShowModal();
}
void mainFrame::OnContactUs(wxCommandEvent &)
{
  CDialogContactUs x(DialogParent());
  x.ShowModal();
}

void mainFrame::OnMaxLadderLabels(wxCommandEvent &)
{
  CDialogMaxLadderLabel x(DialogParent());
  bool b = x.EditValue();
  if(b)
  {
    m_MDImgr.UpdateLadderLabels();
  }
}
CFrameAnalysis *mainFrame::GetAnalysisFrame()
{
#ifndef __NO_MDI__
  CheckActiveFrame();
#endif
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
      "The number of windows has reached\n"
         "the allowable maximum of ");
    sError.Append(nwxString::FormatNumber((int)MAX_FRAMES));
    ErrorMessage(sError);
#if DRAG_DROP_FILES
    m_lsDragDropQueue.clear();
#endif
  }
  return bRtn;
}
void mainFrame::OpenFile(
  const wxString &sFileName,
  const wxString &sLocus,
  COARfile *pOARfile,
  bool bNoChange)
{
  CMDIFrame *pFrame = FindWindowByName(sFileName,false);
  if(pFrame != NULL)
  {
    if(pFrame->GetType() == CMDIFrame::FRAME_PLOT)
    {
      CFramePlot *pF = (CFramePlot *) pFrame;
      bool bShiftKeyDown = nwxKeyState::Shift();
      pF->SetOARfile(pOARfile);
      if(!bNoChange)
      {
        pF->ReInitialize(sLocus,bShiftKeyDown);
      }
    }
    pFrame->RaiseWindow();
    pFrame->Show();
    pFrame->Layout();
  }
  else if(!wxFileName::IsFileReadable(sFileName))
  {
    wxString sError("Cannot open file:\n");
    sError.Append(sFileName);
    ErrorMessage(sError);
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
    if(sFileLower.EndsWith(EXT_GRAPHIC))
    {
      OpenGraphicFile(sFileName, sLocus, pOARfile);
    }
    else if(sFileLower.EndsWith(EXT_BATCH))
    {
      OpenBatchFile(sFileName);
    }
    else if(sFileLower.EndsWith(EXT_ARCHIVE))
    {
      OpenArchiveFile(sFileName);
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
}
void mainFrame::OpenArchiveFile(const wxString &sFileName)
{
  nwxZipInput zipFile(sFileName);
  if(!zipFile.IsZipOpen())
  {
    mainApp::ShowError(wxT("Invalid OSIRIS Archive"),DialogParent());
  }
  else
  {
    wxString sFile;
    wxArrayString as;
    wxString sPattern;
    wxString sPatternBase = wxT("*");
    nwxFileUtil::EndWithSeparator(&sPatternBase);
    sPatternBase.Append(wxT("output"));
    nwxFileUtil::EndWithSeparator(&sPatternBase);
    sPatternBase.Append(wxT("*"));
    wxDateTime t0;
    wxDateTime t((time_t)0);

    sPattern = sPatternBase;
    sPattern.Append(EXT_REPORT_EDITED);
    size_t nCount = zipFile.GetFiles(&as,false,false, sPattern);
    if(!nCount)
    {
      sPattern = sPatternBase;
      sPattern.Append(EXT_REPORT);
      nCount = zipFile.GetFiles(&as,false,false, sPattern);
    }
    // if more than one file, find the newest.
    for(size_t i = 0; i < nCount; i++)
    {
      const wxString &s = as.Item(i);
      t0 = zipFile.GetDateTime(s,true);
      if(t0 > t)
      {
        t = t0;
        sFile = s; // sFile contains the path of the file we want to open
      }
    }
    if(sFile.IsEmpty())
    {
      mainApp::ShowError(wxT("Invalid OSIRIS Archive,\ncannot find analysis file."),DialogParent());
    }
    else
    {
      wxFileName fn(sFileName);
      wxString sPath;
      if(fn.IsDirWritable())
      {
        sPath = fn.GetPath();
      }
      else
      {
        sPath = mainApp::GetConfig()->GetFilePath();
      }
      wxDirDialog dDlg(DialogParent(),
        wxT("Extract OSIRIS Archive"),sPath);
      bool bDone = false;
      bool bWritten = false;
      
      while(!bDone)
      {
        bDone = true;
        if(dDlg.ShowModal() == wxID_OK)
        {
          sPath = dDlg.GetPath();
          size_t nExist = zipFile.FilesExist(sPath);
          if(nExist)
          {
            wxString sMsg;
            if(nExist == 1)
            {
              sMsg = wxT("One file");
            }
            else
            {
              sMsg = wxString::Format(wxT("%d files"),(int) nExist);
            }
            sMsg.Append(wxT(" will be overwritten.\nDo you wish to continue?"));
            wxMessageDialog dlgAsk(DialogParent(),sMsg,wxT("Overwrite?"),wxYES_NO | wxNO_DEFAULT);
            bDone = (dlgAsk.ShowModal() == wxID_YES);
          }
          if(bDone) 
          {
            bWritten = false;
            {
              wxBusyCursor x;
              bWritten = zipFile.WriteAll(sPath);
            }
            if(!bWritten)
            {
              mainApp::ShowError(wxT("Could not extract OSIRIS archive at specified location."),DialogParent());
              bDone = false;  // try again
            }
          }
        }
      }
      if(bWritten)
      {
        nwxFileUtil::EndWithSeparator(&sPath);
        nwxFileUtil::NoStartWithSeparator(&sFile);
        sPath.Append(sFile);
        OpenFile(sPath);
      }
    }
  }
  return;
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
  const wxChar *psFile = sFileName.IsEmpty() ? wxS("This") : sFileName.wc_str();
  wxString sMessage = wxString::Format("%ls is not a valid file",psFile);
  ErrorMessage(sMessage);
}
void mainFrame::FileEmptyMessage(const wxString &sFileName)
{
  const wxChar *psFile = sFileName.IsEmpty() ? wxS("This") : sFileName.wc_str();
  wxString sMessage = wxString::Format(wxS("%ls does not contain any sample data"),psFile);
  ErrorMessage(sMessage);
}
void mainFrame::RemoveWindow(CMDIFrame *p)
{
  // called from CMDIFrame::Destroy()
  p->StopReceiver();
  m_MDImgr.RemoveWindow(p);
  if(p == m_pLastActive)
  {
    SetActiveFrame(NULL);
  }
  if(!m_MDImgr.Size())
  {
    ClearStatusText();
  }
}
void mainFrame::TileTwoWindows(CMDIFrame *pLeft, CMDIFrame *pRight)
{
#if mainFrameIsWindow
  wxSize sz(GetClientSize());
  wxPoint ptLeft(0,0);
#else
  wxRect rect = wxGetClientDisplayRect();
  wxSize sz(rect.GetWidth(),rect.GetHeight());
  wxPoint ptLeft(rect.GetLeft(),rect.GetTop());
#endif
  int nHalf = (sz.x >> 1);
  pLeft->CheckRestore();
  pRight->CheckRestore();
  wxSize szLeft = pLeft->GetSize();
  wxSize szRight;
  if(szLeft.GetWidth() > nHalf)
  {
    szLeft.SetWidth(nHalf);
  }
  szLeft.SetHeight(sz.GetHeight());
  szRight.SetHeight(sz.GetHeight());
  szRight.SetWidth(sz.GetWidth() - szLeft.GetWidth());
  wxPoint ptRight(ptLeft.x + szLeft.GetWidth(),ptLeft.y);
  pRight->Show(true);
  pRight->Raise();
  pLeft->Show(true);
  pLeft->Raise();
  pLeft->SetSize(wxRect(ptLeft,szLeft),wxSIZE_FORCE);
  pRight->SetSize(wxRect(ptRight,szRight),wxSIZE_FORCE);
  pLeft->SetFocus();
}

void mainFrame::ErrorMessage(const wxString &sMessage)
{
  mainApp::ShowError(sMessage,DialogParent());
}
void mainFrame::OnShowLog(wxCommandEvent &)
{
  m_pDialogErrorLog->Show(true);
  m_pDialogErrorLog->Raise();
}
#if 0
// 9/16/16 commented out
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
  else
  {
    e.Skip();
  }
}

#endif

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


#if DRAG_DROP_FILES
void mainFrame::DropFiles()
{
  wxCommandEvent ee(CEventDragDropDelay,GetId());
  ee.SetEventObject(this);
  GetEventHandler()->AddPendingEvent(ee);
}
#endif

bool mainFrame::FileExtensionOK(const wxString &s)
{
  wxString sl(s);
  bool bRtn = false;
  static const char *EXT[] = {
    EXT_GRAPHIC,
    EXT_REPORT,
    EXT_REPORT_EDITED,
    EXT_BATCH,
    EXT_ARCHIVE
  };
  static const size_t EXT_COUNT = sizeof(EXT) / sizeof(EXT[0]);
  sl.MakeLower();
  for(size_t i = 0; i < EXT_COUNT; i++)
  {
    if(sl.EndsWith(EXT[i]))
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
  wxString LF("\n");
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
        sMsgOK = "The following files are not recognized by OSIRIS:\n";
      }
      else
      {
        sMsgOK = "The following file is not recognized by OSIRIS:\n";
      }
      sMsgOK.Append(sMsgType);
      sMsgOK.Append(LF);
    }
    if(nRead)
    {
      if(nRead > 1)
      {
        sMsgOK.Append("The following files are not readable:\n");
      }
      else
      {
        sMsgOK.Append("The following file is not readable:\n");
      }
      sMsgOK.Append(sMsgRead);
      sMsgOK.Append(LF);
    }
    if(nInUse)
    {
      if(nInUse > 1)
      {
        sMsgOK.Append("The following files are being created\n"
            "and cannot be opened yet:\n");
      }
      else
      {
        sMsgOK.Append("The following file is being created\n"
            "and cannot be opened yet:\n");
      }
      sMsgOK.Append(sMsgInUse);
      sMsgOK.Append(LF);
    }
    if(nOK)
    {
      if(nOK > 1)
      {
        sMsgOK.Append("The following files can be opened:\n");
      }
      else
      {
        sMsgOK.Append("The following file can be opened:\n");
      }
      for(i = 0; i < nOK; i++)
      {
        sMsgOK.Append(asFiles[i]);
        sMsgOK.Append(LF);
      }
      sMsgOK.Append("\nDo you wish to continue?");
      nStyle = wxYES_NO;
    }
    wxMessageDialog dlg(DialogParent(),sMsgOK,"Error",nStyle | wxICON_ERROR);
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
#if DRAG_DROP_FILES
      m_lsDragDropQueue.push_back(pasFiles->Item(i));
#else
      OpenFile(pasFiles->Item(i));
#endif
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
    fn.SetFullName("Working.txt");
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
        wxString sMessage("The file, ");
        sMessage.Append(sDisplayName);
        sMessage.Append(", is currently being\n"
          "created and cannot be opened yet.");
        ErrorMessage(sMessage);
      }
    }
  }
  return bRtn;
}

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
          wxString ss = "File not found in list ";
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
            wxString ss = "File not found in list ";
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
void mainFrame::OnMenuOpen(wxMenuEvent &e)
{
  CMDIFrame *pFrame = GetActiveFrame();
  if(pFrame != NULL && pFrame != INIT_LAST_ACTIVE)
  {
    pFrame->OnMenuOpen(e);
  }
}
void mainFrame::OnMenuClose(wxMenuEvent &e)
{
  CMDIFrame *pFrame = GetActiveFrame();
  if(pFrame != NULL && pFrame != INIT_LAST_ACTIVE)
  {
    pFrame->OnMenuClose(e);
  }
}


#if DRAG_DROP_FILES
void mainFrame::OnDropFiles(wxCommandEvent &)
{
  OpenFiles(m_pDropTarget->GetFiles());
}
#endif

#if mainFrameIsWindow
IMPLEMENT_ABSTRACT_CLASS(mainFrame,mainFrameSuper)
#ifdef __WXDEBUG__
void mainFrame::OnPersistMove(wxMoveEvent &e)
{
  if(e.GetEventObject() == this)
  {
    wxPoint pointE = e.GetPosition();
    wxPoint pointW = this->GetPosition() ;
    nwxXmlWindowSizes *ps = mainApp::GetWindowSizes();
    ps->SaveWindowPos( wxString( "mainFrame" ), pointW );
  }
  e.Skip();
}
IMPLEMENT_PERSISTENT_SIZE(mainFrame)
#else
IMPLEMENT_PERSISTENT_SIZE_POSITION(mainFrame)
#endif
#endif


BEGIN_EVENT_TABLE(mainFrame,mainFrameSuper)

#if mainFrameIsWindow
EVT_CLOSE(mainFrame::OnClose)
#endif

EVT_TIMER(IDtimer, mainFrame::OnTimer)

#if DRAG_DROP_FILES
EVT_COMMAND(wxID_ANY,CEventDragDropDelay, mainFrame::OnDropFiles)
#endif

EVT_MENU_OPEN(mainFrame::OnMenuOpen)
EVT_MENU_CLOSE(mainFrame::OnMenuClose)
#if mainFrameIsWindow
EVT_PERSISTENT_SIZE_POSITION(mainFrame)
#endif
END_EVENT_TABLE()
