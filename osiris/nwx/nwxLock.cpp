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
*  FileName: nwxLock.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxLock.h"
#include "nwx/nwxString.h"
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/file.h>
#include <wx/ffile.h>
#ifdef __WXDEBUG__
// used in UnitTest()
#include <wx/stdpaths.h>
#include <wx/debug.h>
#endif

unsigned long nwxLockRead::SinceLastAccess()
{
  unsigned long nRtn = 0;
  wxDateTime dtFile;
  if(IsOK() && m_fnFile.GetTimes(&dtFile,NULL,NULL))
  {
    wxDateTime dtNow;
    time_t tFile = dtFile.GetTicks();
    time_t tNow = dtNow.SetToCurrent().GetTicks();
    if(tFile > 0 && tFile < tNow)
    {
      nRtn = (unsigned long)(tNow - tFile);
    }
  }
  return nRtn;
}

bool nwxLockRead::Touch()
{
  bool bRtn = false;
  if(IsOK())
  {
    wxDateTime dtNow;
    wxDateTime dtAccess;
    dtNow.SetToCurrent();
    m_fnFile.SetTimes(&dtNow,NULL,NULL);
    m_fnFile.GetTimes(&dtAccess,NULL,NULL);
    if(dtAccess == dtNow)
    {
      bRtn = true;
    }
    else
    {
      // try reading the file
      char sBuffer[512];
      size_t nBufferSize = sizeof(sBuffer);
      wxULongLong nSize = m_fnFile.GetSize();
      if(nSize > 0)
      {
        wxFFile ff(m_fnFile.GetFullPath(),_T("r"));
        unsigned long lo = nSize.GetLo();
        if( (!nSize.GetHi()) && (nBufferSize > lo) )
        {
          nBufferSize = (size_t) lo;
        }
        if(ff.IsOpened() && (ff.Read((void *)sBuffer,nBufferSize) > 0))
        {
          ff.Close();
          m_fnFile.GetTimes(&dtAccess,NULL,NULL);
          bRtn = true; 
          //  the file is readable and that will have to be good enough
          // (dtAccess >= dtNow);
        }
      }
    }
  }
  return bRtn;
}
        


#define CHECK_TIMEOUT(n) (((n) < 4) ? 4 : n)

nwxLock::nwxLock() :
  m_nTimeout(nwxLock_TIMEOUT),
  m_bHaveLock(false),
  m_bDirExists(false)
{
}

nwxLock::nwxLock(
    const wxString &sDir,
    const wxString &sFileName,
    unsigned int nTimeout) :
  m_nTimeout(CHECK_TIMEOUT(nTimeout)),
  m_bHaveLock(false),
  m_bDirExists(wxDir::Exists(sDir))
{
  m_fnFullPath.Assign(sDir,sFileName);
}

nwxLock::~nwxLock()
{
  ReleaseLock();
}

unsigned long nwxLock::_GetAge()
{
  time_t tFile = 
    m_fnFullPath.FileExists() 
    ? m_fnFullPath.GetModificationTime().GetTicks()
    : 0;
  unsigned long nRtn = 0;
  if(tFile > 0)
  {
    wxDateTime dt;
    dt.SetToCurrent();
    time_t tDiff =  dt.GetTicks() - tFile;
    nRtn = (unsigned long)(tDiff);
  }
  return nRtn;
}

wxString nwxLock::GetLockUser()
{
  wxString sRtn;
  if(m_bHaveLock)
  {
    sRtn = ::wxGetUserId(); 
  }
  else if(IsLocked())
  {
    while(m_fnFullPath.FileExists() && !_GetAge())
    {
      ::wxSleep(1); // wait for file to be 1 second in age
    }
    const wxString sFileName(m_fnFullPath.GetFullPath());
    wxFFile fn(sFileName.c_str(),"r");
    if(fn.IsOpened() && (m_fnFullPath.GetSize() < 1000))
    {
      fn.ReadAll(&sRtn);
      wxString sSpace(_T("\r\n"));
      size_t nLen = sRtn.Len();
      while(nLen && (sSpace.Find(sRtn.Last()) != wxNOT_FOUND))
      {
        nLen--;
        sRtn.Remove(nLen);
      }
    }
    if(sRtn.IsEmpty())
    {
      wxASSERT_MSG(
        0,_T("nwxLock::GetLockUser() - could not find user"));
      sRtn = _T("unknown user");
    }
  }
  return sRtn;
}

wxString nwxLock::GetLockUser(
  const wxString &sDir,
  const wxString &sFileName)
{
  nwxLock l(sDir,sFileName);
  wxString sRtn = l.GetLockUser();
  return sRtn;
}

bool nwxLock::CannotDeleteLock()
{
  bool bRtn = false;
  if(
    !HaveLock() &&
    _GetAge() > m_nTimeout)
  {
    bRtn = !::wxRemoveFile(m_fnFullPath.GetFullPath());
  }
  return bRtn;
}

bool nwxLock::IsLocked()
{
  bool bRtn = m_bHaveLock ||
    (
      m_bDirExists &&
      m_fnFullPath.FileExists() &&
      (_GetAge() <= m_nTimeout)
    );
  return bRtn;
}


bool nwxLock::IsLockFile(const wxString &sDir,const wxString &sFileName)
{
  wxFileName fn(sDir,sFileName);
  bool bRtn = (fn == m_fnFullPath);
  return bRtn;
}

bool nwxLock::Lock(const wxString &sDir,const wxString &sFileName)
{
  if(wxDir::Exists(sDir))
  {
    m_bDirExists = true;
    if(!IsLockFile(sDir,sFileName))
    {
      ReleaseLock();
      if(!m_bHaveLock)
      {
        m_fnFullPath.Assign(sDir,sFileName);
      }
    }
    if(!m_bHaveLock)
    {
      Lock();
    }
  }
  return m_bHaveLock;
}

bool nwxLock::Lock()
{
  if(!(m_bHaveLock || IsLocked()))
  {
    wxFile fh;
    wxString sPath = m_fnFullPath.GetFullPath();
    if(fh.Create(sPath,true,0777))
    {
      wxString sUser = ::wxGetUserId();
      sUser.Append(nwxString::EOL);
      m_bHaveLock = fh.Write(sUser);
      fh.Close();
      if(!m_bHaveLock)
      {
        ::wxRemoveFile(sPath);
      }
    }
  }
  return m_bHaveLock;
}
bool nwxLock::ReleaseLock()
{
  if(m_bHaveLock)
  {
    wxString sPath = m_fnFullPath.GetFullPath();
    if( (!wxFile::Exists(sPath)) ||
         ::wxRemoveFile(sPath) )
    {
      m_bHaveLock = false;
    }
  }
  return !m_bHaveLock;
}

bool nwxLock::CheckTimeout()
{
  //  the instance that has the lock should call this
  //  periodically in order to touch() the file to
  //  prevent a timeout
  //
  //  returns true if the file exists and has timed out
  //
  bool bRtn = false;
  if(m_bHaveLock)
  {
    if(_GetAge() < (m_nTimeout >> 2))
    {} // do nothing
    else if(!Touch())
    {
      bRtn = _CheckAge();
    }
  }
  else
  {
    bRtn = _CheckAge();
  }
  return bRtn;
}

void nwxLock::SetTimeout(unsigned int nTimeout)
{
  m_nTimeout = CHECK_TIMEOUT(nTimeout);
  CheckTimeout();
}

void nwxLock::Clear()
{
  ReleaseLock();
  m_bHaveLock = false;
  m_bDirExists = false;
  m_fnFullPath.Assign(wxEmptyString);
}

#ifdef __WXDEBUG__

void nwxLockRead::UnitTest(const wxString &sFileName)
{
#define FN "nwxLockRead::UnitTest() "
  wxString sErr;
  nwxLockRead x(sFileName);
  if(!x.IsOK())
  {
    sErr = _T(FN "Cannot find test file: ");
    sErr.Append(sFileName);
  }
  else if(!x.Touch())
  {
    sErr = _T(FN "Could not set access time for file: ");
    sErr.Append(sFileName);
  }
  if(sErr.Len())
  {
    wxASSERT_MSG(0,sErr);
  }
#undef FN
}

void nwxLock::UnitTest()
{
#define FN "nwxLock::UnitTest "
  wxStandardPaths sp;
  wxString sDocDir(sp.GetDocumentsDir());
  wxString sError;
  nwxLock lock1(sDocDir,nwxLock_FILE,32);
  bool b;
  wxString sUser2;
  wxString sUser1;
  lock1.Lock();
  sUser1 = lock1.GetLockUser();
  if(lock1.HaveLock())
  {
    nwxLock lock2(sDocDir);
    sUser2 = lock2.GetLockUser();
    if(lock2.HaveLock())
    {
      sError.Append(_T(FN "Two locks have the same file\n"));
    }
    sUser2 = lock2.GetLockUser();
    if(sUser2 != sUser1)
    {
      sError.Append(_T(FN "Inconsistent user names:\n"));
      sError.Append(sUser1);
      sError.Append(_T("\n"));
      sError.Append(sUser2);
      sError.Append(_T("\n"));
    }
    b = lock1.CheckTimeout();
    b = lock1.ReleaseLock();
    b = lock2.IsLockFile(sDocDir);
    b = lock2.IsLocked();
    b = lock2.Lock(sDocDir,nwxLock_FILE);
  }
  else
  {
    sError.Append(_T(FN "Cannot lock file.\n"));
  }
  if(lock1.IsLocked())
  {
    sError.Append(_T(FN "file is locked and shouldn't be: user = "));
    sError.Append(lock1.GetLockUser());
    sError.Append(_T("\n"));
  }
  if(lock1.CannotDeleteLock())
  {
    sError.Append(_T(FN "Cannot delete lock\n"));
  }

// now test a readonly lock
#ifdef __WXMSW__
// this will break with Windows Vista or newer

#define READ_FILE "OsirisHelp.pdf"
#else
#define READ_FILE "/bin/pwd"
#endif

  nwxLockRead lockRead;
  if(!lockRead.SetFileName(_T(READ_FILE)))
  {
    sError.Append(_T("Cannot find file: " READ_FILE "\n"));
  }
  else if(!lockRead.Touch())
  {
    sError.Append(_T("Cannot update access time of file: " READ_FILE "\n"));
  }
  wxASSERT_MSG(sError.IsEmpty(),sError);
#undef FN
}
#endif // __WXDEBUG__
