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

*  FileName: mainApp.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "mainFrame.h"
#include <wx/filefn.h>
#include <wx/dialog.h>
#include <wx/bitmap.h>
#include <wx/splash.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/file.h>
#include <wx/datetime.h>
#include <wx/cmdline.h>
#ifdef __WXMAC__
#include <wx/stattext.h>
#include <wx/colour.h>
#endif
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
#include "nwx/nwxUtil.h"
#include "nwx/nwxPinger.h"
#include "nwx/nwxTimerReceiver.h"
#include "nwx/nwxStaticBitmap.h"
#include "Platform.h"
#include "ConfigDir.h"
#include "CKitList.h"
#include "CKitColors.h"
#include "CArtifactLabels.h"
#include "Version/OsirisVersion.h"
#include "CPrintOutPlot.h"

#ifdef __WXMSW__
#include <process.h>
#define getpid _getpid
#endif

#ifdef __WXMAC__
#include "CSitePath.h"
#endif
#ifdef _DEBUG
#include "UnitTest.h"
#endif

const wxString mainApp::EMPTY_STRING(wxEmptyString);
const int mainApp::DIALOG_STYLE =
  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER; // | wxTHICK_FRAME; //gone in wx 3.0

const int mainApp::RFU_MAX_ENTER = 1000000;
const int mainApp::RFU_MIN_ENTER = 5;

int mainApp::g_count = 0;
int mainApp::g_nMaxLogLevel = 1000;
int mainApp::g_nWindowCounter = 0;
bool mainApp::g_bSuppressMessages = false;
ConfigDir *mainApp::m_pConfig = NULL;
nwxXmlMRU *mainApp::m_pMRU = NULL;
nwxXmlCmfList *mainApp::m_pCMFlist = NULL;
nwxXmlWindowSizes *mainApp::m_pWindowSize = NULL;
CPersistKitList *mainApp::m_pKitList = NULL;
CKitColors *mainApp::m_pKitColors = NULL;
CArtifactLabels *mainApp::m_pArtifactLabels = NULL;
nwxPinger *mainApp::g_pPinger = NULL;
mainApp *mainApp::g_pThis = NULL;
wxFile *mainApp::m_pFout = NULL;

void mainApp::_cleanupPinger(bool bByUser)
{
  if (g_pPinger != NULL)
  {
    if (bByUser)
    {
      g_pPinger->SetClosedByUser(true);
      // alerts the process that this was closed by the user
      // if OSIRIS crashes, this will not be sent and a "crash"
      // ping will be logged
    }
    delete g_pPinger;
    g_pPinger = NULL;
  }
}

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
    if(m_pTimer != NULL)
    {
      delete m_pTimer;
      m_pTimer = NULL;
    }
    CPrintOutPlot::StaticCleanup();
    _cleanupPinger();
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

nwxPinger *mainApp::GetPinger()
{
  if ((g_pPinger != NULL) && !g_pPinger->IsRunning())
  {
    _cleanupPinger();
  }
  return g_pPinger;
}

void mainApp::Ping(const wxString &sName, const wxString &sValue)
{
  nwxPinger *p(GetPinger());
  if (p != NULL)
  {
    nwxPingerSet pset(p);
    pset.Set(sName, sValue);
  }
  // destructor will send the event
}
void mainApp::Ping2(const wxString &sName1, const wxString &sValue1,
  const wxString &sName2, const wxString &sValue2)
{
  nwxPinger *p(GetPinger());
  if (p != NULL)
  {
    nwxPingerSet pset(p);
    pset.Set(sName1, sValue1);
    pset.Set(sName2, sValue2);
  }
  // destructor will send the event
}
void mainApp::Ping3(const wxString &sName1, const wxString &sValue1,
  const wxString &sName2, const wxString &sValue2,
  const wxString &sName3, const wxString &sValue3)
{
  nwxPinger *p(GetPinger());
  if (p != NULL)
  {
    nwxPingerSet pset(p);
    pset.Set(sName1, sValue1);
    pset.Set(sName2, sValue2);
    pset.Set(sName3, sValue3);
  }
  // destructor will send the event
}
void mainApp::PingList(const wxChar ** apList)
{
  const wxChar **ps;
  bool bName = true;
  nwxPingerSet pset(GetPinger());
  wxString sName, sValue;
  for (ps = apList; *ps != NULL; ps++)
  {
    if (bName)
    {
      sName = *ps;
    }
    else
    {
      sValue = *ps;
      pset.Set(sName, sValue);
    }
    bName = !bName;
  }
}
void mainApp::PingExit()
{
  mainApp::Ping(PING_EVENT, "user-exit");
  _cleanupPinger(true);
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
  else
  {
    m_pKitList->CheckReload();
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

//  class for frame created on mac in order to
//  show the app correctly on OS X 10.14 and later
//
//  on the mac mainApp::OnInit() MUST create a window
//  and not do much else or the windowing system 
//  will not work properly in that windows will not
//  appear and control+TAB will not activate this program
//
//  the code in _InitializeApp() was removed
//  from mainApp::OnInit because of the window
//  problems.  On the mac it is delayed
//
#ifdef __WXMAC__

class CFakeFrame : public wxFrame, public nwxTimerReceiver
{

public:
  CFakeFrame() :
    wxFrame(NULL, wxID_ANY, "",
            wxDefaultPosition,
            wxDefaultSize,
            0),
    m_nTick(0)
  {
    wxStaticText *pText = new wxStaticText(
      this,wxID_ANY,"Initializing OSIRIS...",
      wxDefaultPosition, wxDefaultSize,
      wxALIGN_CENTRE_HORIZONTAL);
    wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
    pText->SetBackgroundColour(*wxWHITE);
    SetBackgroundColour(*wxWHITE);
    pSizer->Add(pText, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER | wxALL, 12);
    SetSizer(pSizer);
    pSizer->Layout();
    pSizer->Fit(this);
    Center();
    
  }
  virtual ~CFakeFrame()
  {}
  virtual void OnTimer(wxTimerEvent &);
private:
  int m_nTick;
};


void CFakeFrame::OnTimer(wxTimerEvent &e)
{
  m_nTick++;
  if(m_nTick == 2)
  {
    mainApp::Get()->InitializeApp();
    Destroy();
  }
}


#endif

bool mainApp::OnInit()
{
  if(g_pThis != NULL) return false;
	if(!wxApp::OnInit()) return false;
  SetupTimer();

  g_pThis = this;
#ifdef __WXMAC__
  m_pInvisibleFrame = new CFakeFrame;
  m_pInvisibleFrame->Show(true);
#else
  _InitializeApp();
#endif
  return true;
}

void mainApp::_InitializeApp()
{
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
  wxXml2Object::AddReceiver(this);
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
#ifdef __WXDEBUG__
  CSitePath::GetGlobal()->LogTestString();
#endif
#endif

  m_pFrame->Startup(bHasArgs);
  const wxChar *psFormat(wxS("argv[%d] = %ls"));
  LogMessageV(psFormat,0,argv[0].wc_str());
  LogMessageV(wxT("cwd: %ls"), wxGetCwd().wc_str());
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
  _setupPinger();

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

void mainApp::_setupPinger()
{
  if (!PingerEnabled()) {} // not enabled, return
  else if ((g_pPinger != NULL) && g_pPinger->IsRunning()) {} // already running, return
  else
  {
    if (g_pPinger != NULL)
    {
      _cleanupPinger();
    }
    wxString sAppName("osirisapp");
    wxString sAppVer(OSIRIS_VERS_BASE);
    g_pPinger = new nwxPinger(g_pThis->m_pFrame, IDpingerProcess, sAppName, sAppVer, NULL, m_pFout);
  }
}

void mainApp::Raise()
{
#if mainFrameIsWindow
  Get()->m_pFrame->Raise();
#endif
}

wxString mainApp::_pingerFile()
{
  wxString sPath = mainApp::GetConfig()->GetSitePath();
  nwxFileUtil::EndWithSeparator(&sPath);
  sPath.Append("nousage.txt");
  return sPath;
}
bool mainApp::PingerEnabled()
{
  return !wxFileName::FileExists(_pingerFile());
}

bool mainApp::SetPingerEnabled(bool bEnable)
{
  // set the usage stats 'pinger' as enabled or
  // disabled.  If successful, return true else false
  // It is disabled if the file "nousage.txt" exists
  // in the site folder.
  // The user must have access privileges
  // in order to change this setting
  bool bIsEnabled = PingerEnabled();
  bool bRtn = true;
  if (bEnable != bIsEnabled)
  {
    wxString sPath(_pingerFile());
    if (!bEnable)
    {
      wxFile sFile(sPath, wxFile::write);
      bRtn = sFile.IsOpened();
      sFile.Close();
      if (bRtn)
      {
#ifndef __WXMSW__
        nwxFileUtil::SetFilePermissionFromDir(sPath);
#endif
        mainApp::Ping(PING_EVENT, "PingerDisabled");
        mainApp::_cleanupPinger(true);
      }
    }
    else
    {
      bRtn = wxRemoveFile(sPath);
      if (bRtn)
      {
        mainApp::_setupPinger();
        mainApp::Ping(PING_EVENT, "PingerEnabled");
      }
    }
  }
  return bRtn;
}

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

void mainApp::DumpBitmap(wxBitmap *pBitmap, const wxString &sFileName)
{
  wxString sPath = mainApp::GetConfig()->GetDebugConfigPath(true);
  if (!sPath.IsEmpty())
  {
    sPath.Append(sFileName);
    sPath = nwxFileUtil::FindNewFileName(sPath);
    nwxStaticBitmap::AddPngHandler();
    if (!pBitmap->ConvertToImage().SaveFile(sPath, wxBITMAP_TYPE_PNG))
    {
      wxString s(wxT("Cannot save file: "));
      s.Append(sPath);
      mainApp::LogMessage(s);
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
  return nwxUtil::GetTopLevelParent(p);
}
void mainApp::ReRender(wxWindow *p)
{
  wxClassInfo *pClass(CLASSINFO(wxTopLevelWindow));
  bool bTop = p->IsKindOf(pClass);
  wxSizer *pSizer = bTop ? p->GetSizer() : NULL;
  if (pSizer != NULL)
  {
    pSizer->Layout();
  }
  else if (bTop)
  {
    // loop through each child
    wxWindowList &lw = p->GetChildren();
    if (lw.GetCount() == 1)
    {
      p->Layout();
    }
    for (wxWindowListNode *pw = lw.GetFirst(); pw != NULL; pw = pw->GetNext())
    {
      ReRender(pw->GetData());
    }
  }
  else
  {
    p->Layout();
  }
  p->Refresh();
}

#if 0
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
#endif

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

void mainApp::OnTimer(wxTimerEvent &e)
{
  if(!m_nTimerCount)
  {
    CIncrementer incr(m_nTimerCount);
#ifdef _DEBUG
//    UnitTest::Run();
#endif
    nwxTimerReceiver::DispatchTimer(e);
    if(m_pFrame != NULL)
    {
#if CHECK_FRAME_ON_TIMER && !defined(__NO_MDI__)
    m_pFrame->CheckActiveFrame();
#endif
#if DRAG_DROP_FILES
      m_pFrame->CheckDragDropQueue();
#endif
    }
  }
}

void mainApp::OnQuit(wxCommandEvent &e)
{
  bool bSkip;
  wxBusyCursor xxx;
  bSkip = m_pFrame->DoClose();
  if(bSkip)
  {
#if mainFrameIsWindow
    m_pFrame->Hide();
    bSkip = m_pFrame->Close();
    m_pFrame->Destroy();
    mainApp::PingExit();
#else
    mainApp::PingExit();
    m_pFrame->DeletePendingEvents();
    delete m_pFrame;
    m_pFrame = NULL;
    _Cleanup();
#endif
  }
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
DEFINE_CMD_HANDLER(OnPinger)
DEFINE_CMD_HANDLER(OnExportSettings)
DEFINE_CMD_HANDLER(OnEditGridColours)
DEFINE_CMD_HANDLER(OnShowLog)
DEFINE_CMD_HANDLER(OnAccessSiteSettings)
DEFINE_CMD_HANDLER(OnShowSiteSettings)
DEFINE_CMD_HANDLER(OnShowCommandLine)
DEFINE_CMD_HANDLER(OnAnalyze)
DEFINE_CMD_HANDLER(OnOpenPlot)
DEFINE_CMD_HANDLER(OnOpenBatch)
DEFINE_CMD_HANDLER(OnOpenArchive)
DEFINE_CMD_HANDLER(OnHelp)
DEFINE_CMD_HANDLER(OnAbout)
DEFINE_CMD_HANDLER(OnPrivacy)
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
EVT_MENU(IDpinger,    mainApp::OnPinger)
EVT_MENU(IDexport,    mainApp::OnExportSettings)
EVT_MENU(IDeditColours, mainApp::OnEditGridColours)
EVT_MENU(IDlog,       mainApp::OnShowLog)

EVT_MENU(IDsiteSettings, mainApp::OnAccessSiteSettings)
EVT_MENU(IDsiteShow, mainApp::OnShowSiteSettings)
EVT_MENU(IDsiteCommandLine, mainApp::OnShowCommandLine)

EVT_MENU(IDanalyze,   mainApp::OnAnalyze)
EVT_MENU(IDArchiveExtract, mainApp::OnOpenArchive)
EVT_MENU(IDopenPlot,  mainApp::OnOpenPlot)
EVT_MENU(IDopenBatch, mainApp::OnOpenBatch)
EVT_MENU(IDhelp,      mainApp::OnHelp)
EVT_MENU(wxID_ABOUT, mainApp::OnAbout)
EVT_MENU(IDprivacy, mainApp::OnPrivacy)
EVT_MENU(IDcheckForUpdates, mainApp::OnCheckForUpdates)
EVT_MENU(IDhelpContactUs, mainApp::OnContactUs)
EVT_MENU(IDExportGraphic, mainApp::OnMenu)
EVT_MENU(IDMaxLadderLabels, mainApp::OnMaxLadderLabels)
#ifdef __WINDOW_LIST__
#ifdef __WXMAC__
EVT_MENU_RANGE(IDmenuWindow_Minimize,IDmenuWindow_Frame_END,mainApp::OnWindowMenu)
#else
EVT_MENU_RANGE(IDmenuWindowFrame,IDmenuWindow_Frame_END,mainApp::OnWindowMenu)
#endif
#endif
EVT_TIMER(IDtimer, mainApp::OnTimer)
END_EVENT_TABLE()
