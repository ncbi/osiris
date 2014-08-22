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
#include "nwx/nwxXmlMRU.h"
#include "nwx/nwxString.h"
#include "nwx/nwxFileUtil.h"
#include "Platform.h"
#include "ConfigDir.h"
#include "CKitList.h"

#ifdef __WXMSW__
#include <process.h>
#define getpid _getpid
#endif

const int mainApp::DIALOG_STYLE =
  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER; // | wxTHICK_FRAME; //gone in wx 3.0

const int mainApp::RFU_MAX_ENTER = 1000000;
const int mainApp::RFU_MIN_ENTER = 5;

int mainApp::g_count = 0;
int mainApp::g_nMaxLogLevel = 1000;
bool mainApp::g_bSuppressMessages = false;
ConfigDir *mainApp::m_pConfig = NULL;
nwxXmlMRU *mainApp::m_pMRU = NULL;
CPersistKitList *mainApp::m_pKitList = NULL;
mainApp *mainApp::g_pThis = NULL;

wxFile *mainApp::m_pFout = NULL;

mainApp::~mainApp()
{
  g_count--;
  wxXml2Object::RemoveReceiver(this);
  if(!g_count)
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
    if(m_pKitList != NULL)
    {
      delete m_pKitList;
      m_pKitList = NULL;
    }
    _CloseMessageStream();
  }
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
CPersistKitList *mainApp::GetKitList()
{
  if(m_pKitList == NULL)
  {
    m_pKitList = new CPersistKitList();
    m_pKitList->Load();
  }
  return m_pKitList;
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
#endif
  
  m_pFrame->Startup(bHasArgs);
  const wxChar *psFormat(wxS("argv[%d] = %ls"));
  LogMessageV(psFormat,0,argv[0].wc_str());
  for(int i = 1; i < argc; ++i)
  {
    LogMessageV(psFormat,i,argv[i].wc_str());
    m_pFrame->OpenFile(argv[i]);
  }
//  time_t t;
//  time(&t);
  wxString sPID = wxString::Format(wxS("PID: %d"),(int)getpid());
//  wxLog::OnLog(wxLOG_Message,(const wxChar *)sPID,t);
  LogMessage(sPID);
#if 1 || defined(__WXDEBUG__)
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
        time_t t;
        time(&t);
        wxLog::OnLog(wxLOG_Message,"Cannot open message log",t);
      }
//      else
//      {
//        LogMessage("File Opened ****************************");
//      }
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
  time_t t;
  time(&t);
  wxLog::OnLog(wxLOG_Message,sMsg,t);
  if(LogIsOpen())
  {
    _LogMessageFile(sMsg,t);
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
      wxLog::OnLog(wxLOG_Message,"Cannot write to message log, file has been closed",t);
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
    delete m_pFout;
    m_pFout = NULL;
    _LogMessage("File closed on exit");
  }
}

const wxString mainApp::FormatWindowTitle(
  const wxString &fileName,
  bool bModified,
  const CParmOsiris *pParm,
  const wxDateTime *pTime)
{
  wxString s;
  wxString sRtn;
  if(!fileName.IsEmpty())
  {
    wxFileName fn(fileName);
    s = fn.GetFullName();
  }
  size_t nLen(s.Len());
  sRtn.Alloc(nLen + 16);
  sRtn = "OSIRIS";
  if(s.Len())
  {
    sRtn.Append(" - ");
    sRtn.Append(s);
    if(bModified)
    {
      sRtn.Append(" *");
    }
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

void mainApp::OnInitCmdLine (wxCmdLineParser &parser) 
{
  static const wxCmdLineEntryDesc cmdLineDesc[] =
  {    
    { wxCMD_LINE_PARAM,  NULL, NULL, "input-file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE },
    { wxCMD_LINE_NONE }
  };
  parser.SetDesc(cmdLineDesc);
  return;
}
bool mainApp::OnCmdLineParsed (wxCmdLineParser &parser) 
{
  bool b = wxApp::OnCmdLineParsed(parser);
  return b;
}


#define DEFINE_CMD_HANDLER(x) \
  void mainApp::x (wxCommandEvent &e) { m_pFrame->x(e); }


DEFINE_CMD_HANDLER(OnOpen)
DEFINE_CMD_HANDLER(OnQuit)
DEFINE_CMD_HANDLER(OnRecentFiles)
DEFINE_CMD_HANDLER(OnLabSettings)
DEFINE_CMD_HANDLER(OnExportSettings)
DEFINE_CMD_HANDLER(OnEditGridColours)
DEFINE_CMD_HANDLER(OnShowLog)

DEFINE_CMD_HANDLER(OnAnalyze)
DEFINE_CMD_HANDLER(OnOpenPlot)
DEFINE_CMD_HANDLER(OnOpenBatch)
DEFINE_CMD_HANDLER(OnHelp)
DEFINE_CMD_HANDLER(OnAbout)
DEFINE_CMD_HANDLER(OnCheckForUpdates)
DEFINE_CMD_HANDLER(OnContactUs)
DEFINE_CMD_HANDLER(OnMenu)
DEFINE_CMD_HANDLER(OnMaxLadderLabels)

DEFINE_CMD_HANDLER(OnSave)

#undef DEFINE_CMD_HANDLER

IMPLEMENT_APP(mainApp)
BEGIN_EVENT_TABLE(mainApp,wxApp)

EVT_MENU(wxID_OPEN,   mainApp::OnOpen)
EVT_MENU(wxID_EXIT,   mainApp::OnQuit)
EVT_MENU(IDlistMRU,   mainApp::OnRecentFiles)
EVT_MENU(IDlab,       mainApp::OnLabSettings)
EVT_MENU(IDexport,    mainApp::OnExportSettings)
EVT_MENU(IDeditColours, mainApp::OnEditGridColours)
EVT_MENU(IDlog,       mainApp::OnShowLog)

EVT_MENU(IDanalyze,   mainApp::OnAnalyze)
EVT_MENU(IDopenPlot,  mainApp::OnOpenPlot)
EVT_MENU(IDopenBatch, mainApp::OnOpenBatch)
EVT_MENU(IDhelp,      mainApp::OnHelp)
EVT_MENU(wxID_ABOUT, mainApp::OnAbout)
EVT_MENU(IDcheckForUpdates, mainApp::OnCheckForUpdates)
EVT_MENU(IDhelpContactUs, mainApp::OnContactUs)
EVT_MENU(IDExportGraphic, mainApp::OnMenu)
EVT_MENU(IDMaxLadderLabels, mainApp::OnMaxLadderLabels)

EVT_MENU(wxID_SAVEAS, mainApp::OnSave)
EVT_MENU(wxID_SAVE, mainApp::OnSave)

END_EVENT_TABLE()
