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

*  FileName: nwxProcess.h
*  Author:   Douglas Hoffman
*  Date:  3/8/2018 moved from ../OsirisAnalysis/CProcess.h
*
*/
#ifndef __C_PROCESS_H__
#define __C_PROCESS_H__

#include <wx/stream.h>
#include <wx/string.h>
#include <wx/process.h>
#include <vector>

class nwxProcess : public wxProcess
{
public:
  nwxProcess(wxEvtHandler *parent, int id, char **argv) : 
      wxProcess(parent,id)
  {
    Redirect();
    Init();
    Run(argv);
  }
  nwxProcess(wxEvtHandler *parent, int id, wchar_t **argv) : 
      wxProcess(parent,id)
  {
    Redirect();
    Init();
    Run(argv);
  }
  nwxProcess(wxEvtHandler *parent, int id = wxID_ANY) : 
    wxProcess(parent,id)
  {
    Redirect();
    Init();
  }
  virtual ~nwxProcess();
  virtual void ProcessLine(const char *p, size_t nLen, bool bErrStream) = 0;
  virtual void OnTerminate(int nPID,int nExitCode);
  size_t WriteToProcess(const char *buffer, size_t size);
  size_t WriteToProcess(const wxString &s)
  {
    const char *p = s.utf8_str();
    return WriteToProcess(p, strlen(p));
  }
  size_t WriteToProcess(const std::vector<wxString> &vs);
  bool Run(char **argv, int nExeFlags = wxEXEC_ASYNC);
  bool Run(wchar_t **argv, int nExeFlags = wxEXEC_ASYNC);
  void Pause(bool bPause = true)
  {
    m_bPaused = bPause;
  }
  void Cancel();
  void Continue()
  {
    Pause(false);
  }
  bool Failed()
  {
    return m_bFailed;
  }
  bool IsRunning()
  {
    return m_bRunning;
  }
  int GetExitStatus()
  {
    return m_nExitStatus;
  }
  bool Killed()
  {
    return m_bKilled;
  }
  //void Wait();
  size_t ProcessIO(size_t nLimit = 0x7fffffff);
  bool IsErrorOpened() const;
private:
  void _ProcessLine(const wxString &s, bool bErr)
  {
    const char *p = s.utf8_str();
    ProcessLine(p, strlen(p), bErr);
  }
  bool _runInit();
  bool _runSetup(int nExeFlags);
  size_t ProcessIO(wxInputStream *pIn, wxString &sLine, bool bErrStream);
  void Init()
  {
    m_nPID = 0;
    m_nExitStatus = 0;
    m_bFailed = false;
    m_bRunning = false;
    m_bKilled = false;
    m_bPaused = false;
  }
  wxString m_sBuffer;
  wxString m_sBufferError;
  long m_nPID;
  int m_nExitStatus;
  bool m_bFailed;
  bool m_bRunning;
  bool m_bKilled;
  bool m_bPaused;
};


class nwxProcessPause
{
public:
  nwxProcessPause(nwxProcess *p) : m_pProcess(p)
  {
    m_pProcess->Pause(true);
  }
  virtual ~nwxProcessPause()
  {
    m_pProcess->Pause(false);
  }
private:
  nwxProcess *m_pProcess;
};

class nwxProcessSimple : public nwxProcess
{
public:
  nwxProcessSimple(wxEvtHandler *parent, int id, char **argv) :
      nwxProcess(parent,id,argv)
  {}
  nwxProcessSimple(wxEvtHandler *parent, int id, wchar_t **argv) :
      nwxProcess(parent,id,argv)
  {}
  nwxProcessSimple(wxEvtHandler *parent, int id = wxID_ANY) : 
    nwxProcess(parent,id)
  {}
  nwxProcessSimple() :
    nwxProcess(NULL)
  {}
  virtual void ProcessLine(const char *p, size_t nLen, bool bErrStream)
  {
    wxString s = wxString::FromUTF8(p,nLen);
    wxArrayString *pas = bErrStream ? &m_asStdErr : &m_asStdOut;
    pas->Add(s);
  }
  const wxArrayString &GetStdErr()
  {
    return m_asStdErr;
  }
  const wxArrayString &GetStdOut()
  {
    return m_asStdOut;
  }
  void Clear()
  {
    m_asStdOut.Empty();
    m_asStdErr.Empty();
  }
  static wxString ARGV2str(const wchar_t **argv)
  {
    wxString s;
    wxString sTmp;
    const wchar_t **ps;
    for(ps = argv; (*ps) != NULL; ++ps)
    {
      sTmp = *ps;
      _appendArg(&s,sTmp);
    }
    s.Trim();
    return s;
  }
  static wxString ARGV2str(const char **argv)
  {
    wxString s;
    wxString sTmp;
    const char **ps;
    for(ps = argv; (*ps) != NULL; ++ps)
    {
      sTmp = wxString::FromUTF8(*ps);
      _appendArg(&s,sTmp);
    }
    s.Trim();
    return s;
  }
  const wxString &BuildLog(const char **argv)
  {
    m_sLog = ARGV2str(argv);
    _appendLog();
    return m_sLog;
  }
  const wxString &BuildLog(const wchar_t **argv)
  {
    m_sLog = ARGV2str(argv);
    _appendLog();
    return m_sLog;
  }
private:
  static void _appendArg(wxString *ps, const wxString sArg)
  {
    if(sArg.IsEmpty())
    {
      ps->Append("\"\" ");
    }
    else if(sArg.Find(wxS(" ")) != wxNOT_FOUND)
    {
      ps->Append("\"");
      ps->Append(sArg);
      ps->Append("\" ");
    }
    else
    {
      ps->Append(sArg);
      ps->Append(wxS(" "));
    }
  }
  void _appendLog()
  {
    m_sLog.Append(
      wxString::Format(
        wxS("\nreturn code: %d"), GetExitStatus()
        ));
    wxArrayString *ppas[2] = {&m_asStdOut, &m_asStdErr};
    size_t anLen[2] = {m_asStdOut.GetCount(), m_asStdErr.GetCount()};
    size_t nLen,n;
    const char *psHeader[2] = {"\n\nstdout:","\n\nstderr"};
    for (int i = 0; i < 2; ++i)
    {
      nLen = anLen[i];
      if(nLen)
      {
        wxArrayString *pas = ppas[i];
        m_sLog.Append(psHeader[i]);
        for(n = 0; n < nLen; ++n)
        {
          m_sLog.Append("\n");
          m_sLog.Append(pas->Item(n));
        }
      }
    }
    m_sLog.Append("\n");
  }
  wxArrayString m_asStdOut;
  wxArrayString m_asStdErr;
  wxString m_sLog;
};


#endif

