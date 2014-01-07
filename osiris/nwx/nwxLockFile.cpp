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

*  FileName: nwxLockFile.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwxLockFile.h"
#include <wx/filefn.h>
#include <wx/log.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/utils.h>

const time_t nwxLockFile::TIMEOUT = 30;
wxString nwxLockFile::GetLockFileName(const wxString &sFileName)
{
  wxFileName fn(sFileName);
  wxString sRtn;
  if(fn.IsOk())
  {
    sRtn = sFileName;
    if(!wxFileName::IsCaseSensitive())
    {
      sRtn.MakeLower();
    }
    sRtn.Append(_T(".lock"));
  }
  return sRtn;
}

bool nwxLockFile::LockFileExists(const wxString &sFileName)
{
  return wxFileExists(GetLockFileName(sFileName));
}
bool nwxLockFile::WaitUntilUnlocked(
  const wxString &sFileName,
  time_t nWait,
  time_t nTimeout)
{
  wxString sLockFile = GetLockFileName(sFileName);
  time_t tUntil = 0;
  bool bDone = false;
  bool bRtn = false;
  while(!(bDone || bRtn))
  {
    if(!wxFileName::FileExists(sLockFile))
    {
      bRtn = true;
    }
    else
    {
      time_t tNow;
      time(&tNow);
      if(GetAge(sLockFile,tNow) >= nTimeout)
      {
        bRtn = true;
      }
      else if(!nWait)
      {
        bDone = true;
      }
      else if(!tUntil)
      {
        tUntil = tNow + nWait;
      }
      else if(tNow <= tUntil)
      {
        wxSleep(1);
      }
      else
      {
        bDone = true;
      }
    }
  }
  return bRtn;
}
bool nwxLockFile::IsLocked(const wxString &sFileName)
{
  wxString sLockFile = GetLockFileName(sFileName);
  wxFileName fn(sLockFile);
  bool bRtn = fn.FileExists();
  set<wxString>::iterator itr = m_setFileName.find(sFileName);
  if(itr != m_setFileName.end())
  {
    if(bRtn)
    {
      fn.Touch();
    }
    else
    {
      m_setFileName.erase(itr);
    }
  }
  return bRtn;
}
bool nwxLockFile::LockWait(const wxString &sFileName, time_t nWait, time_t nTimeout)
{
  wxString sFileLockName = GetLockFileName(sFileName);
  bool bRtn = false;
  if(sFileLockName.Len())
  {
    wxFile fl;
    time_t tUntil = 0;
    bool bDone = false;
    while(!bDone)
    {
      if(!wxFileName::FileExists(sFileLockName))
      {
        //if(AccessAge(sFileName) > 1) //-- too many problems
        {
          bRtn = fl.Create(sFileLockName.c_str(),false);
          bDone = true;
        }
      }
      else if(m_setFileName.find(sFileName) != m_setFileName.end())
      {
        //
        // file exists and was locked by this instance
        //
        bRtn = true;
        bDone = true;
        wxFileName fn(sFileLockName);
        fn.Touch(); // update file time to prevent timeout
        sFileLockName.Clear();
      }
      else if(GetAge(sFileLockName) >= nTimeout)
      {
        bRtn = fl.Create(sFileLockName.c_str(),true);
        bDone = true;
      }
      if(!bDone)
      {
        time_t tNow;
        time(&tNow);
        if(!nWait)
        {
          bDone = true;
        }
        else if(!tUntil)
        {
          tUntil = tNow + nWait;
        }
        else if(tNow <= tUntil)
        {
          wxSleep(1);
        }
        else
        {
          bDone = true;
        }
      }
    }
    if(bRtn && sFileLockName.Len())
    {
      m_setFileName.insert(sFileName);
    }
  }
  if(!bRtn)
  {
    wxString s(_T("Cannot lock file: "));
    time_t t;
    s.Append(sFileName);
    time(&t);
    wxLog::OnLog(wxLOG_Message,s,t);
    wxASSERT_MSG(false,s);
  }
  return bRtn;
}
time_t nwxLockFile::GetAge(const wxString &sLockFileName, time_t tNow)
{
  if(!tNow)
  {
    time(&tNow);
  }
  time_t tRtn(tNow);
  wxFileName fn(sLockFileName);
  if(fn.FileExists())
  {
    time_t tf = fn.GetModificationTime().GetTicks();
    if(tf > tRtn)
    {
      tRtn = 0;
    }
    else
    {
      tRtn -= tf;
    }
  }
  return tRtn;
}
time_t nwxLockFile::AccessAge(const wxString &sFileName, time_t tNow)
{
  if(!tNow)
  {
    time(&tNow);
  }
  time_t tRtn(tNow);
  wxDateTime dtAccess;
  wxFileName fn(sFileName);
  if(fn.FileExists())
  {
    fn.GetTimes(&dtAccess,NULL,NULL);
    time_t ta = dtAccess.GetTicks();
    if(ta > tRtn)
    {
      tRtn = 0;
    }
    else
    {
      tRtn -= ta;
    }
  }
  return tRtn;
}
bool nwxLockFile::HasLock(const wxString &sFileName)
{
  wxString sFileLockName = GetLockFileName(sFileName);
  set<wxString>::iterator itr = m_setFileName.find(sFileName);
  bool bRtn = false;
  if(itr != m_setFileName.end())
  {
    bRtn = LockWait(sFileName,0);
  }
  return bRtn;
}
bool nwxLockFile::Unlock(const wxString &sFileName)
{
  set<wxString>::iterator itr = m_setFileName.find(sFileName);
  bool bRtn = false;
  if(itr != m_setFileName.end())
  {
    wxString sFileLockName = GetLockFileName(sFileName);
    bRtn = true; // file was locked by this instance
    if(!wxFileExists(sFileLockName)) {;} // OK
    else if(!wxRemoveFile(sFileLockName))
    {
      // have a problem removing this file, log the error and still return true
      time_t t;
      time(&t);
      wxString sMsg(_T("Cannot remove lock file: "));
      sMsg.Append(sFileLockName);
      wxLog::OnLog(wxLOG_Message,(const wxChar *)sMsg,t);
      wxASSERT_MSG(false,sMsg);
    }
    m_setFileName.erase(itr);
  }
  return bRtn;
}
void nwxLockFile::UnlockAll()
{
  while(m_setFileName.size() > 0)
  {
    set<wxString>::iterator itr = m_setFileName.begin();
    Unlock(*itr);
  }
}
void nwxLockFile::UpdateAll()
{
  for(set<wxString>::iterator itr = m_setFileName.begin();
    itr != m_setFileName.end();
    ++itr)
  {
    Lock(*itr); // touch file
  }
}
