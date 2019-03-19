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
*
*  FileName: nwxPinger.cpp
*  Author:   Douglas Hoffman
*  Date:  2/15/2019
*  send event info back to ncbi via https://www.ncbi.nlm.nih.gov/stat
*/
#include "nwx/nwxPinger.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "nwx/nwxProcess.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxTimerReceiver.h"

#ifdef __WXMSW__
#include <process.h>
#define getpid _getpid
#endif


#ifdef __WXMSW__
#define __EOL "\r\n"
#else
#define __EOL "\n"
#endif

class _nwxPingerProcess : public nwxProcess, nwxTimerReceiver
{
public:
  _nwxPingerProcess(nwxPinger *pOwner, wxEvtHandler *parent, int id, char **argv, wxFile *pFileLog, bool bLog) :
    nwxProcess(parent, id, argv),
    m_pOwner(pOwner),
    m_pFileLog(pFileLog)
    ,m_nTimer(0)
    ,m_bLog(bLog)
  {
    ProcessIO();
  }
  virtual ~_nwxPingerProcess()
  {
    if (m_pOwner != NULL)
    {
      m_pOwner->_processExit(GetExitStatus());
    }
  }
  void DetachOwner()
  {
    m_pOwner = NULL;
  }
  virtual void OnTerminate(int nPID, int nExitCode);

  virtual void ProcessLine(const char *p, size_t nLen, bool bErrStream);
  virtual void OnTimer(wxTimerEvent &);
  void CheckIO()
  {
    ProcessIO();
    m_nTimer = 0;
  }
private:
  nwxPinger *m_pOwner;
  wxFile * m_pFileLog;
  int m_nTimer;
  bool m_bLog;
};
void _nwxPingerProcess::OnTimer(wxTimerEvent &e)
{
  m_nTimer += e.GetInterval();
  if (m_nTimer >= 5000)
  {
    CheckIO();
  }
}
void _nwxPingerProcess::OnTerminate(int nPID, int nStatus)
{
  // the problem, if nwxPinger 'delete's the process, it crashes
  //  if it doesn't delete it there is a memory leak
  // calling wxProcess::OnTerminate() seems to fix this (Windows)
  nwxProcess::OnTerminate(nPID, nStatus);
  wxProcess::OnTerminate(nPID, nStatus);
}

void _nwxPingerProcess::ProcessLine(const char *p, size_t nLen, bool bErrStream)
{
  wxString s = bErrStream ? "usage stats ERROR: " : "usage stats: ";
  wxString sLine(p, nLen);
  s.Append(sLine);
  nwxString::Trim(&s);
  if (m_bLog)
  {
    nwxLog::LogMessage(s);
  }
  if((m_pFileLog != NULL) && m_pFileLog->IsOpened())
  {
    s.Append(__EOL);
    if (!m_pFileLog->Write(s))
    {
      nwxLog::LogMessage("ERROR: could not write to usage stats log file");
      m_pFileLog = NULL;
    }
  }
}
size_t nwxPingerSet::AppendList(std::vector<wxString> *pvs, bool bIncludeEolGo) const
{
  wxString s;
  pvs->reserve(pvs->size() + m_mapPairs.size() + (bIncludeEolGo ? 1 : 0));
  std::multimap<wxString, wxString>::const_iterator itr;
  for (itr = m_mapPairs.begin();
    itr != m_mapPairs.end();
    ++itr)
  {
    s = itr->first;
    s.Append("=");
    s.Append(itr->second);
    if (bIncludeEolGo) { s.Append(__EOL); }
    pvs->push_back(s);
  }
  if (bIncludeEolGo)
  {
    s = "go" __EOL;
    pvs->push_back(s);
  }
  return pvs->size();
}

bool nwxPingerSet::Send(nwxPinger *pPinger)
{
  bool bRtn = (pPinger == NULL) ? false : pPinger->Ping(*this);
  Init();
  return bRtn;
}

nwxPinger::nwxPinger(
  wxEvtHandler *parent,
  int id,
  const wxString &sAppName,
  const wxString &sAppVersion,
  const nwxPingerSet *pSetDefaults,
  wxFile *pLogFile,
  bool bLog) :
  m_process(NULL),
  m_pLogFile(pLogFile),
  m_bLog(bLog),
  m_bSetup(false),
  m_bInDestructor(false),
  m_bLoggedExit(false)
{
  _setup(parent, id, pSetDefaults, sAppName, sAppVersion);
}
nwxPinger::nwxPinger() : m_process(NULL),
m_pLogFile(NULL),
m_bLog(false),
m_bSetup(false),
m_bInDestructor(false),
m_bLoggedExit(false)
{}


nwxPinger::~nwxPinger()
{
  m_bInDestructor = true;
  if (m_process != NULL)
  {
    m_process->DetachOwner();
    if (_checkProcess())
    {
      wxString sQuit = "q" __EOL;
      m_process->WriteToProcess(sQuit);
      for (int i = 0; (i < 10) && _checkProcess(); ++i)
      {
        // max 10 loops, give it one second to stop
        wxMilliSleep(100);
      }
      if (_checkProcess())
      {
        m_process->Cancel();
        //      delete m_process; // do not delete
        m_process = NULL;
      }
    }
  }
}

void nwxPinger::_logExit(int n)
{
  if (!(m_bInDestructor || m_bLoggedExit))
  {
    // process exited unexpectedly, log message
    m_bLoggedExit = true;
    wxString s("Usage statistics process exited unexpectedly, return = ");
    s.Append(nwxString::FormatNumber(n));
    nwxLog::LogMessage(s);
  }
}

bool nwxPinger::_checkProcess()
{
  bool bRtn = false;
  if (m_process == NULL)
  {}
  else if (m_process->IsRunning())
  {
    m_process->CheckIO();
    bRtn = true;
  }
  else
  {
    _logExit(m_process->GetExitStatus());
//    delete m_process;
    m_process = NULL; // do not delete
  }
  return bRtn;
}

bool nwxPinger::_setup(
  wxEvtHandler *parent,
  int id,
  const nwxPingerSet *pSetDefaults,
  const wxString &sAppName,
  const wxString &sAppVersion)
{
  std::vector<wxString> vsArgs;
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxFileName fn(sp.GetExecutablePath());
  wxString sScript;
  wxString sInterpreter;
  bool bRtn = true;
  if (pSetDefaults != NULL)
  {
    pSetDefaults->GetList(&vsArgs, false);
  }
  sScript = fn.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
  nwxFileUtil::EndWithSeparator(&sScript);

#ifdef __WXMSW__
  // ms windows, check for cscript.exe pinger.vbs
  sInterpreter = nwxFileUtil::PathFind(wxS("cscript.exe"), true, false);
  if (sInterpreter.IsEmpty())
  {
    nwxLog::LogMessage("Cannot find cscript.exe, OSIRIS will not send usage");
    bRtn = false;
  }
  else
  {
    sScript.Append("pinger.vbs");
    if (!wxFileName::FileExists(sScript))
    {
      nwxLog::LogMessage("Cannot find pinger.vbs, OSIRIS will not send usage");
      bRtn = false;
    }
  }
#else
  {
    // mac, check for /bin/bash pinger.sh
    sInterpreter = wxS("/bin/bash");
    if (!wxFileName::IsFileExecutable(sInterpreter))
    {
      bRtn = false;
      nwxLog::LogMessage("Cannot find /bin/bash, OSIRIS will not send usage");
    }
    else
    {
      sScript.Append("pinger.sh");
      if (!wxFileName::FileExists(sScript))
      {
        nwxLog::LogMessage("Cannot find pinger.sh, OSIRIS will not send usage");
        bRtn = false;
      }
    }

  }
#endif
  if(bRtn)
  {
    wxString sPID = nwxString::FormatNumber(getpid());
    const size_t NARG(126);
    const char *ARGV[NARG + 2];
    size_t n = 0;
    ARGV[n++] = sInterpreter.utf8_str();
#ifdef __WXMSW__
    ARGV[n++] = "//NoLogo";
#endif
    ARGV[n++] = sScript.utf8_str();

    if (!sAppName.IsEmpty())
    {
      ARGV[n++] = "-a";
      ARGV[n++] = sAppName.utf8_str();
    }
    if (!sAppVersion.IsEmpty())
    {
      ARGV[n++] = "-v";
      ARGV[n++] = sAppVersion.utf8_str();
    }
    ARGV[n++] = "-p";
    ARGV[n++] = sPID.utf8_str();
    if (vsArgs.size())
    {
      std::vector<wxString>::const_iterator itr;
      for (itr = vsArgs.begin();
        (itr != vsArgs.end()) && (n < NARG);
        ++itr)
      {
        if ((*itr).Len())
        {
          ARGV[n++] = "-s";
          ARGV[n++] = (*itr).utf8_str();
        }
      }
    }
    ARGV[n] = NULL;
    m_process = new _nwxPingerProcess(this, parent, id, (char **)ARGV, m_pLogFile, m_bLog);
  }
  m_bSetup = true;
  return bRtn;
}

bool nwxPinger::Ping(const nwxPingerSet &set)
{
  if (_checkProcess())
  {
    std::vector<wxString> vs;
    set.GetList(&vs, true);
    if (vs.size())
    {
      m_process->WriteToProcess(vs);
    }
  }
  return _checkProcess();
}
