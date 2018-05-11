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

*  FileName: mainApp.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "mainFrame.h"
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/splash.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/datetime.h>
#include <wx/cmdline.h>
#include "CDialogAbout.h"
#include "CParmOsiris.h"
#include "CImageAbout.h"
#include <time.h>
#include "nwx/nwxLog.h"
#include "nwx/nwxXmlMRU.h"
#include "nwx/nwxXmlWindowSizes.h"
#include "nwx/nwxXmlCmfList.h"
#include "nwx/nwxString.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxXmlCMF.h"
#include "Platform.h"
#include "ConfigDir.h"
#include "CKitList.h"
#include "CKitColors.h"
#include "CArtifactLabels.h"

#ifdef __WXMSW__
#include <process.h>
#define getpid _getpid
#endif

#ifdef __WXMAC__
#include "CSitePath.h"
#endif

const wxString mainApp::EMPTY_STRING(wxEmptyString);
const int mainApp::DIALOG_STYLE =
  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER; // | wxTHICK_FRAME; //gone in wx 3.0

const int mainApp::RFU_MAX_ENTER = 1000000;
const int mainApp::RFU_MIN_ENTER = 5;

int mainApp::g_count = 0;
int mainApp::g_nMaxLogLevel = 1000;
bool mainApp::g_bSuppressMessages = false;
ConfigDir *mainApp::m_pConfig = NULL;
nwxXmlMRU *mainApp::m_pMRU = NULL;
nwxXmlCmfList *mainApp::m_pCMFlist = NULL;
nwxXmlWindowSizes *mainApp::m_pWindowSize = NULL;
CPersistKitList *mainApp::m_pKitList = NULL;
CKitColors *mainApp::m_pKitColors = NULL;
CArtifactLabels *mainApp::m_pArtifactLabels = NULL;
mainApp *mainApp::g_pThis = NULL;

wxFile *mainApp::m_pFout = NULL;

void mainApp::_Cleanup()
{
  if(g_count <= 1)
  {
    if(m_pConfig != NULL)
    {
      delete m_pConfig;
      m_pConfig = NULL;
    }
    if(m_pMRU != NULL)
    {
      delete m_pMRU;
      m_pMRU = NULL;
    }
    if(m_pCMFlist != NULL)
    {
      delete m_pCMFlist;
      m_pCMFlist = NULL;
    }
    if(m_pWindowSize != NULL)
    {
      delete m_pWindowSize;
      m_pWindowSize = NULL;
    }
    if(m_pArtifactLabels != NULL)
    {
      delete m_pArtifactLabels;
      m_pArtifactLabels = NULL;
    }
    if(m_pKitColors != NULL)
    {
      delete m_pKitColors;
      m_pKitColors = NULL;
    }
    if(m_pKitList != NULL)
    {
      delete m_pKitList;
      m_pKitList = NULL;
    }
  }
}
mainApp::~mainApp()
{
  wxXml2Object::RemoveReceiver(this);
  g_count--;
  _Cleanup(); // should be redundant but harmless
  if(!g_count) { _CloseMessageStream(); }
}
ConfigDir *mainApp::GetConfig()
{
  if(g_count && (m_pConfig == NULL))
  {
    m_pConfig = new ConfigDir(".osiris","Osiris");
  }
  return m_pConfig;
}
nwxXmlMRU *mainApp::GetMRU()
{
  if(g_count && (m_pMRU == NULL))
  {
    wxString sPath = m_pConfig->GetConfigPath();
    nwxFileUtil::EndWithSeparator(&sPath);
    sPath.Append("mru.xml");
    m_pMRU = new nwxXmlMRU(sPath);
  }
  return m_pMRU;
}
nwxXmlCmfList *mainApp::GetCMFlist()
{
  if(g_count && (m_pCMFlist == NULL))
  {
    wxString sPath = m_pConfig->GetCMFListFileName();
    m_pCMFlist = new nwxXmlCmfList(sPath);
  }
  return m_pCMFlist;
}
nwxXmlWindowSizes *mainApp::GetWindowSizes()
{
  if(g_count && (m_pWindowSize == NULL))
  {
    wxString sPath = m_pConfig->GetConfigPath();
    nwxFileUtil::EndWithSeparator(&sPath);
    sPath.Append("wsize.xml");
    m_pWindowSize = nwxXmlWindowSizes::SetupGlobal(sPath);
  }
  return m_pWindowSize;
}
CKitColors *mainApp::GetKitColors()
{
  if(m_pKitColors == NULL)
  {
    m_pKitColors = new CKitColors();
  }
  return m_pKitColors;
}
CArtifactLabels *mainApp::GetArtifactLabels()
{
  if(m_pArtifactLabels == NULL)
  {
    m_pArtifactLabels = new CArtifactLabels();
  }
  return m_pArtifactLabels;
}
CPersistKitList *mainApp::GetKitList()
{
  if(m_pKitList == NULL)
  {
    m_pKitList = new CPersistKitList();
    m_pKitList->Load();
  }
  return m_pKitList;
}
CILSLadderInfo *mainApp::GetILSLadderInfo()
{
  return GetKitList()->GetILSLadderInfo();
}

void mainApp::SetupCMFlist()
{
  nwxXmlCMF::SetCmfList(mainApp::GetCMFlist());
}

bool mainApp::OnInit()
{
  if(g_pThis != NULL) return false;
	if(!wxApp::OnInit()) return false;

  g_pThis = this;
  // Set up splash screen
#ifndef _DEBUG
  wxBitmap bitmap;
  if(CImageAbout::LoadBitmap(&bitmap))
  {
      new wxSplashScreen(bitmap,
      wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
      2000, NULL, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP|wxFRAME_NO_TASKBAR);
    wxYield();
  }
#endif
  GetConfig();
  GetWindowSizes();
  // set up log file if Debug directory exists
  _OpenMessageStream();

  // set up window

  m_pFrame = new mainFrame();
  bool bHasArgs = (argc > 1) && (argv[1] != (wxChar *)NULL);
#ifdef __WXMAC__
  if(m_asFiles.Count() > 0)
  {
    bHasArgs = true;
  }
  CSitePath::GetGlobal()->LogTestString();
#endif
  
  m_pFrame->Startup(bHasArgs);
  const wxChar *psFormat(wxS("argv[%d] = %ls"));
  LogMessageV(psFormat,0,argv[0].wc_str());
  for(int i = 1; i < argc; ++i)
  {
    LogMessageV(psFormat,i,argv[i].wc_str());
    m_pFrame->OpenFile(argv[i]);
  }
  wxString sPID = wxString::Format(wxS("PID: %d"),(int)getpid());
  LogMessage(sPID);
#if defined(__WXDEBUG__)
  const wxPlatformInfo &plat(wxPlatformInfo::Get());
  wxString sOS = wxString::Format(
    wxT("Platform:\n  OS: %s; %s\n  Version: %d.%d\n  Arch: %s\n  Toolkit: %d.%d\n  Port: %s"),
    (const char *) plat.GetOperatingSystemFamilyName().ToAscii(),
    (const char *) plat.GetOperatingSystemDescription().ToAscii(),
    plat.GetOSMajorVersion(),
    plat.GetOSMinorVersion(),
    (const char *)plat.GetArchName().ToAscii(),
    plat.GetToolkitMajorVersion(),
    plat.GetToolkitMinorVersion(),
    (const char *) plat.GetPortIdName().ToAscii());

  sOS += wxString::Format(wxS("\n  sizeof(char):        %d"),(int)(sizeof(char)));
  sOS += wxString::Format(wxS("\n  sizeof(wxChar):      %d"),(int)(sizeof(wxChar)));
  sOS += wxString::Format(wxS("\n  sizeof(wchar_t):     %d"),(int)(sizeof(wchar_t)));
  sOS += wxString::Format(wxS("\n  sizeof(short):       %d"),(int)(sizeof(short)));
  sOS += wxString::Format(wxS("\n  sizeof(int):         %d"),(int)(sizeof(int)));
  sOS += wxString::Format(wxS("\n  sizeof(long):        %d"),(int)(sizeof(long)));
  sOS += wxString::Format(wxS("\n  sizeof(longlong):    %d"),(int)(sizeof(long long)));
  sOS += wxString::Format(wxS("\n  sizeof(size_t):      %d"),(int)(sizeof(size_t)));
  sOS += wxString::Format(wxS("\n  sizeof(time_t):      %d"),(int)(sizeof(time_t)));
  sOS += wxString::Format(wxS("\n  sizeof(float):       %d"),(int)(sizeof(float)));
  sOS += wxString::Format(wxS("\n  sizeof(double):      %d"),(int)(sizeof(double)));
  sOS += wxString::Format(wxS("\n  sizeof(long double): %d"),(int)(sizeof(long double )));

  LogMessage(sOS);
#endif

#ifdef __WXDEBUG__

    
  m_pConfig->Log();

#endif
#ifdef __WXMAC__
  if(bHasArgs)
  {
    _MacOpenFiles();
  }
#endif
  return true;
}
#ifdef __WXMAC__
void mainApp::MacOpenFile(const wxString &sFileName)
{
  m_asFiles.Add(sFileName);
  _MacOpenFiles();
}
void mainApp::_MacOpenFiles()
{
  if( (m_asFiles.Count() > 0) && (m_pFrame != NULL) )
  {
    m_pFrame->OpenFiles(m_asFiles);
    m_asFiles.Empty();
  }
}
#endif

void mainApp::_OpenMessageStream()
{
  if(m_pFout == NULL) // should always be NULL because OnInit should be called only once
  {
    wxString sOutFile = m_pConfig->GetConfigPath();
    nwxFileUtil::EndWithSeparator(&sOutFile);
    sOutFile += "Debug";
    if(wxFileName::DirExists(sOutFile))
    {
      int nPID = getpid();
      nwxFileUtil::EndWithSeparator(&sOutFile);
      sOutFile += wxString::Format("%d.txt",nPID);
      m_pFout = new wxFile();
      if(!m_pFout->Create(sOutFile.wc_str(),true))
      {
        delete m_pFout;
        m_pFout = NULL;
        nwxLog::LogMessage(wxT("Cannot open message log"));
      }
      else
      {
        nwxLog::SetCallback(_LogMessageFile);
      }
    }
  }
}

void mainApp::ShowError(const wxString &sMsg, wxWindow *parent)
{
  if(!MessagesSuppressed())
  {
    wxMessageDialog dlgmsg(
      parent,sMsg,"ERROR",wxOK | wxICON_ERROR);
    dlgmsg.ShowModal();
  }
}
void mainApp::ShowAlert(const wxString &sMsg, wxWindow *parent)
{
  if(!MessagesSuppressed())
  {
    wxMessageDialog dlgmsg(
      parent,sMsg,"Alert",wxOK | wxICON_EXCLAMATION);
    dlgmsg.ShowModal();
  } 
}
void mainApp::SetBoldFont(wxWindow *pWindow)
{
  wxFont fn = pWindow->GetFont();
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  pWindow->SetFont(fn);
}

void mainApp::LogMessageV(const wxChar *psFormat,...)
{
  va_list ap;
  va_start(ap,psFormat);
  wxString s = wxString::FormatV(psFormat,ap);
  va_end(ap);
  _LogMessage(s);
}
void mainApp::_LogMessage(const wxString &sMsg)
{
#if mainFrameIsWindow
  if(mainFrame::GetCount() > 0)
#endif
  {
    nwxLog::LogMessage(sMsg);
  }
}
void mainApp::ReceiveXml2Error(const wxString &s)
{
  LogMessage(s);
}
bool mainApp::Confirm(wxWindow *parent, const wxString &sPrompt,const wxString &sConfirm, int nDEFAULT)
{
  wxMessageDialog dlg(parent,sPrompt,sConfirm,
    wxYES_NO | wxICON_QUESTION | nDEFAULT );
  int n = dlg.ShowModal();
  return (n == wxID_YES) || (n == wxID_OK);
}
bool mainApp::ConfirmModificationsLost(wxWindow *parent)
{
  const wxString sPrompt(
    "Warning: Modifications will be lost!\n"
        "Do you wish to continue?");
  const wxString sTitle("Warning");
  return Confirm(parent,sPrompt,sTitle);
}

void mainApp::_LogMessageFile(const wxString &sMsg, time_t t)
{
  if(!LogIsOpen())  { return; }
  if(!t)
  {
    time(&t);
  }
  wxString sTime = nwxString::FormatDateTime(t);
  wxString sFullMessage;
  sFullMessage.Alloc(sMsg.Len() + sTime.Len() + 8);
  sFullMessage = sTime;
  sFullMessage.Append(" ");
  sFullMessage.Append(sMsg);
  sFullMessage.Append(nwxString::EOL);

  size_t nToWrite = sFullMessage.Len();
  size_t n;
  const char *sBuffer = sFullMessage.utf8_str();
  while(nToWrite > 0)
  {
    n = m_pFout->Write(sBuffer,nToWrite);
    if(!n)
    {
      nToWrite = 0; // loop exit
      _CloseMessageStream();
      nwxLog::LogMessage(wxT("Cannot write to message log, file has been closed"));
    }
    else
    {
      nToWrite -= n;
      sBuffer += n;
    }
  }
  m_pFout->Flush();
}
bool mainApp::LogIsOpen()
{
  return (m_pFout != NULL);
}

void mainApp::_CloseMessageStream()
{
  if(LogIsOpen())
  {
    _LogMessageFile("Closing log file on exit.",0);
    wxFile *p = m_pFout;
    m_pFout = NULL;
    delete p;
  }
}

const wxString mainApp::FormatWindowTitle(
  const wxChar *psType,
  const wxString &fileName,
  bool bModified,
  const CParmOsiris *pParm,
  const wxDateTime *pTime)
{
  wxString s;
  wxString sRtn;
  if(fileName.IsEmpty())
  {}
  else if(fileName.Contains(wxT("; ")))
  {
    s = fileName;
  }
  else
  {
    wxFileName fn(fileName);
    s = fn.GetFullName();
  }
  sRtn.Alloc(512);
  sRtn = psType;
  if(s.Len())
  {
    sRtn.Append(" - ");
    sRtn.Append(s);
  }
  if(bModified)
  {
    sRtn.Append(" *");
  }
  if(pParm != NULL)
  {
    sRtn.Append("; ");
    sRtn.Append(pParm->GetKitName());
    sRtn.Append(", ");
    sRtn.Append(pParm->GetLsName());
  }
  if(pTime != NULL)
  {
    sRtn.Append(" on ");
    sRtn.Append(nwxString::FormatDateTime(*pTime));
  }
  sRtn.Append(" - OSIRIS");
  return sRtn;
}
wxWindow *mainApp::GetTopLevelParent(wxWindow *p)
{
  wxClassInfo *pClass = CLASSINFO(wxTopLevelWindow);
  wxWindow *pRtn = p;
  wxWindow *pLast = p;
  while( (pRtn != NULL) && (!pRtn->IsKindOf(pClass))  )
  {
    pLast = pRtn;
    pRtn = pRtn->GetParent();
  }
  return pRtn;
}
void mainApp::LAYOUT_HACK(wxWindow *p)
{
  // sometimes a window will 'layout' or cleanup
  // after a resize when Layout() doesn't work
  // created as a result of OS-803
  wxTopLevelWindow *pTop = wxDynamicCast(p,wxTopLevelWindow);
  if( (pTop != NULL) && pTop->IsMaximized() )
  {
    ((wxTopLevelWindow *)p)->Maximize(false);
    ((wxTopLevelWindow *)p)->Maximize(true);
  }
  else
  {
    wxSize sz(p->GetSize());
    if(sz.x > 1)
    {
      sz.x--;
      p->SetSize(sz);
      sz.x++;
      p->SetSize(sz);
    }
  }
}

#ifndef __WXMAC__
void mainApp::OnInitCmdLine (wxCmdLineParser &parser) 
{
  static const wxCmdLineEntryDesc cmdLineDesc[] =
  {    
    { wxCMD_LINE_PARAM,  NULL, NULL, "input-file", 
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE | wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION,  "verbose" ,"verbose",NULL,
      wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_NONE }
  };
  parser.SetDesc(cmdLineDesc);
  return;
}
#endif

#ifdef __WXDEBUG__
const wxString mainApp::g_sACTIVE(wxT("mainApp::OnActivate: active"));
const wxString mainApp::g_sINACTIVE(wxT("mainApp::OnActivate: inactive"));
#endif

void mainApp::OnActivate(wxActivateEvent &e)
{
#ifdef __WXDEBUG__
    bool b = IsActive();
    const wxString *ps =
      b ? &g_sACTIVE : &g_sINACTIVE;
    mainApp::LogMessage(*ps);
#endif
    e.Skip();
}

void mainApp::OnQuit(wxCommandEvent &e)
{
  bool bSkip;
  wxBusyCursor xxx;
  bSkip = m_pFrame->DoClose();
#if mainFrameIsWindow
  if(bSkip)
  {
    bSkip = m_pFrame->Close();
  }
#else
  if(bSkip)
  {
    m_pFrame->DeletePendingEvents();
    delete m_pFrame;
    m_pFrame = NULL;
    _Cleanup();
  }
#endif
  e.Skip(bSkip);
  _LogMessageFile(wxT("mainApp::OnQuit"),0);
}

bool mainApp::SetupSiteSettings()
{
  return g_pThis->m_pFrame->SetupSiteSettings();
}

#define DEFINE_CMD_HANDLER(x) \
  void mainApp::x (wxCommandEvent &e) { m_pFrame->x(e); }


DEFINE_CMD_HANDLER(OnOpen)
DEFINE_CMD_HANDLER(OnRecentFiles)
DEFINE_CMD_HANDLER(OnLabSettings)
DEFINE_CMD_HANDLER(OnArtifactLabels)
DEFINE_CMD_HANDLER(OnExportSettings)
DEFINE_CMD_HANDLER(OnEditGridColours)
DEFINE_CMD_HANDLER(OnShowLog)
DEFINE_CMD_HANDLER(OnAccessSiteSettings)
DEFINE_CMD_HANDLER(OnShowSiteSettings)
DEFINE_CMD_HANDLER(OnAnalyze)
DEFINE_CMD_HANDLER(OnOpenPlot)
DEFINE_CMD_HANDLER(OnOpenBatch)
DEFINE_CMD_HANDLER(OnOpenArchive)
DEFINE_CMD_HANDLER(OnHelp)
DEFINE_CMD_HANDLER(OnAbout)
DEFINE_CMD_HANDLER(OnCheckForUpdates)
DEFINE_CMD_HANDLER(OnContactUs)
DEFINE_CMD_HANDLER(OnMenu)
DEFINE_CMD_HANDLER(OnMaxLadderLabels)
//DEFINE_CMD_HANDLER(OnSave) // commented out 9/16/16

#ifdef __WINDOW_LIST__
DEFINE_CMD_HANDLER(OnWindowMenu)
#endif

#undef DEFINE_CMD_HANDLER

IMPLEMENT_APP(mainApp)
BEGIN_EVENT_TABLE(mainApp,wxApp)

EVT_MENU(wxID_OPEN,   mainApp::OnOpen)
EVT_MENU(wxID_EXIT,   mainApp::OnQuit)
EVT_MENU(IDlistMRU,   mainApp::OnRecentFiles)
EVT_MENU(IDlab,       mainApp::OnLabSettings)
EVT_MENU(IDartifactLabels, mainApp::OnArtifactLabels)
EVT_MENU(IDexport,    mainApp::OnExportSettings)
EVT_MENU(IDeditColours, mainApp::OnEditGridColours)
EVT_MENU(IDlog,       mainApp::OnShowLog)

EVT_MENU(IDsiteSettings, mainApp::OnAccessSiteSettings)
EVT_MENU(IDsiteShow, mainApp::OnShowSiteSettings)

EVT_MENU(IDanalyze,   mainApp::OnAnalyze)
EVT_MENU(IDArchiveExtract, mainApp::OnOpenArchive)
EVT_MENU(IDopenPlot,  mainApp::OnOpenPlot)
EVT_MENU(IDopenBatch, mainApp::OnOpenBatch)
EVT_MENU(IDhelp,      mainApp::OnHelp)
EVT_MENU(wxID_ABOUT, mainApp::OnAbout)
EVT_MENU(IDcheckForUpdates, mainApp::OnCheckForUpdates)
EVT_MENU(IDhelpContactUs, mainApp::OnContactUs)
EVT_MENU(IDExportGraphic, mainApp::OnMenu)
EVT_MENU(IDMaxLadderLabels, mainApp::OnMaxLadderLabels)

//EVT_MENU(wxID_SAVEAS, mainApp::OnSave) // commented out 9/16/16b
//EVT_MENU(wxID_SAVE, mainApp::OnSave)


#ifdef __WINDOW_LIST__
#ifdef __WXMAC__
EVT_MENU_RANGE(IDmenuWindow_Minimize,IDmenuWindow_Frame_END,mainApp::OnWindowMenu)
#else
EVT_MENU_RANGE(IDmenuWindowFrame,IDmenuWindow_Frame_END,mainApp::OnWindowMenu)
#endif
#endif
EVT_ACTIVATE_APP(mainApp::OnActivate)
END_EVENT_TABLE()
