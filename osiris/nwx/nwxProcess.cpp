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
*  FileName: nwxProcess.cpp
*  Author:   Douglas Hoffman
*  Date:  3/8/2018 moved from ../OsirisAnalysis/CProcess.cpp
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "nwx/stdb.h"
#include <string>
#include "nwx/stde.h"

#include "nwx/nwxProcess.h"
#include <wx/timer.h>
#include <wx/utils.h>
#ifndef __WXMSW__
#include <sys/wait.h>
#endif

bool nwxProcess::Run(char **argv, int nExeFlags)
{
  bool bRtn = _runInit();
  if(bRtn)
  {
    m_nPID = ::wxExecute(argv,nExeFlags,this);
    bRtn = _runSetup(nExeFlags);
  }
  return bRtn;
}
bool nwxProcess::Run(wchar_t **argv, int nExeFlags)
{
  bool bRtn = _runInit();
  if(bRtn)
  {
    m_nPID = ::wxExecute(argv,nExeFlags,this);
    bRtn = _runSetup(nExeFlags);
  }
  return bRtn;
}
bool nwxProcess::_runInit()
{
  bool bRtn = !m_bRunning;
  if(bRtn)
  {
    Init();
  }
  return bRtn;
}

bool nwxProcess::_runSetup(int nExeFlags)
{
  if(nExeFlags &  wxEXEC_SYNC )
  {
    m_nExitStatus = (int)m_nPID;
    m_bRunning = false;
    m_bFailed = false;
    m_nPID = 0;
  }
  else if(!m_nPID)
  {
    m_nExitStatus = -1;
    m_bRunning = false; 
    m_bFailed = true;
  }
  else
  {
    m_bFailed = false;
    m_bRunning = true;
  }
  return !m_bFailed;
}
nwxProcess::~nwxProcess()
{
  if(m_bRunning)
  {
    Detach();
  }
}
void nwxProcess::Cancel()
{
  if(m_nPID && m_bRunning && !m_bKilled)
  {
    m_bKilled = true;
    wxProcess::Kill((int)m_nPID,wxSIGKILL, wxKILL_CHILDREN);
  }
}

size_t nwxProcess::WriteToProcess(const char *pBuffer, size_t _nLen)
{
  wxOutputStream *pOut = GetOutputStream();
  wxASSERT_MSG(pOut != NULL, "Cannot get output stream for process");
  size_t nRtn = 0;
  if (pOut != NULL)
  {
    const char *pChar = pBuffer;
    size_t nLen = _nLen;
    size_t n;
    while (nLen > 0)
    {
      n = pOut->Write((void *)pChar, nLen).LastWrite();
      if(n)
      {
        nRtn += n;
        pChar += n;
        nLen -= n;
      }
      else if (!pOut->IsOk())
      {
        nLen = 0;
        Cancel();
      }
    }
  }
  return nRtn;
}

size_t nwxProcess::WriteToProcess(const std::vector<wxString> &vs)
{
  std::vector<wxString>::const_iterator itr;
  size_t nRtn = 0;
  for (itr = vs.begin(); itr != vs.end(); ++itr)
  {
    nRtn += WriteToProcess(*itr);
  }
  return nRtn;
}

void nwxProcess::OnTerminate(int nPID,int nStatus)
{
  // this is called as an internal event because it is a virtual
  // function that has been overridden and it also called from
  // the parent window as the result of a wxProcessEvent
  //
  // The reason for the redundancy is that sometimes the
  // macintosh version does not detect when the process exits.
  //
  if(nPID == m_nPID)
  {
    m_nExitStatus = nStatus;
    m_bRunning = false;
    m_nPID = 0;
  }
}



size_t nwxProcess::ProcessIO(size_t nLimit)
{
  size_t nRtn = 0;
  bool bInputClosed = false;
  if(!m_bPaused)
  {
    if(!IsInputOpened())
    {
      if(m_sBuffer.Len())
      {
        m_sBuffer += "\n";
        ProcessLine(m_sBuffer.utf8_str(),m_sBuffer.Len(),false);
        m_sBuffer.Empty();
      }
      bInputClosed = true;
    }
    else 
    {
      while(IsInputAvailable() && (nRtn < nLimit))
      {
        nRtn += ProcessIO(GetInputStream(),m_sBuffer,false);
      }
    }
    if(!IsErrorOpened())
    {
      if(m_sBufferError.Len())
      {
        m_sBufferError += "\n";
        ProcessLine(m_sBufferError.utf8_str(),m_sBufferError.Len(),true);
        m_sBufferError.Empty();
      }
      if(bInputClosed && m_bRunning)
      {
    	  m_bRunning = false;
    	      // we are sometimes not notified when process ends
#ifndef __WXMSW__
    	  // need to clean up zombie because wx sometimes
    	  // fails to do this on the macintosh
    	  int nStatLoc;
    	  pid_t nPID;
    	  nPID = waitpid((pid_t)m_nPID,&nStatLoc,0);
    	  OnTerminate(m_nPID,nStatLoc);
#endif
      }
    }
    else 
    {
      while(IsErrorAvailable() && (nRtn < nLimit))
      {
        nRtn += ProcessIO(GetErrorStream(),m_sBufferError,true);
      }
    }
  }
  return nRtn;
}
bool nwxProcess::IsErrorOpened() const
{
  wxInputStream *pIn = GetErrorStream();
  bool bRtn = pIn && (pIn->GetLastError() != wxSTREAM_EOF);
  return bRtn;
}


size_t nwxProcess::ProcessIO(
  wxInputStream *pIn, 
  wxString &sLine,
  bool bErrStream)
{
  size_t nRtn = 0;
  size_t nLen;
  char *pBuffer;
  char *pEnd;
  const int BUFFER_SIZE = 512;
  char sBuffer[BUFFER_SIZE + 1];
  char EOL = '\n';
  if(pIn->CanRead())
  {
    nRtn = pIn->Read(sBuffer,BUFFER_SIZE).LastRead();
    sBuffer[nRtn] = 0;
    if(nRtn)
    {
      pBuffer = &sBuffer[0];
      for(pEnd = strchr(pBuffer,EOL);
          pEnd != NULL;
          pEnd = strchr(pBuffer,EOL))
      {
        *pEnd = 0;
        sLine.Append(pBuffer);
        sLine.Append(wxChar(EOL));
        *pEnd = EOL; // restore
        nLen = sLine.Len();
        ProcessLine(sLine.utf8_str(), nLen, bErrStream);
        sLine.Empty();
        pBuffer = pEnd;
        pBuffer++;
      }
      sLine += pBuffer;
    }
  }
  return nRtn;
}
