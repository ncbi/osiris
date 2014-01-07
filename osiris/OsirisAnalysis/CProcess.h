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

*  FileName: CProcess.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PROCESS_H__
#define __C_PROCESS_H__

#include <wx/stream.h>
#include <wx/string.h>
#include <wx/process.h>


class CProcess : public wxProcess
{
public:
  CProcess(wxEvtHandler *parent, int id, char **argv) : 
      wxProcess(parent,id)
  {
    Redirect();
    Init();
    Run(argv);
  }
  CProcess(wxEvtHandler *parent, int id = wxID_ANY) : 
    wxProcess(parent,id)
  {
    Redirect();
    Init();
  }
  virtual ~CProcess();
  virtual void ProcessLine(const char *p, size_t nLen, bool bErrStream) = 0;
  virtual void OnTerminate(int nPID,int nExitCode);
  bool Run(char **argv);
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
  void Wait();
  size_t ProcessIO(size_t nLimit = 0x7fffffff);
  bool IsErrorOpened() const;
private:
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


class CProcessPause
{
public:
  CProcessPause(CProcess *p) : m_pProcess(p)
  {
    m_pProcess->Pause(true);
  }
  virtual ~CProcessPause()
  {
    m_pProcess->Pause(false);
  }
private:
  CProcess *m_pProcess;
};

#endif

