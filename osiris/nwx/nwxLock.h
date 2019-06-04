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
*  FileName: nwxLock.h
*  Author:   Douglas Hoffman
*
*   create a lock file containing the userid and release it when done
*/

#ifndef __NWX_LOCK_H__
#define __NWX_LOCK_H__

#include <wx/string.h>
#include <wx/filename.h>
#include <wx/datetime.h>

#define nwxLock_TIMEOUT 120
#define nwxLock_FILE "lock"


#if 0
//  OS-679 - removed because access time is unreliable

class nwxLockRead
{
public:
  // class to set the access time of a file
  // and report if the access time was 'recent'
  nwxLockRead()
  {}
  nwxLockRead(const wxString &sFileName)
  {
    SetFileName(sFileName);
  }
  nwxLockRead(const nwxLockRead &x)
  {
    (*this) = x;
  }
  virtual ~nwxLockRead()
  {}
  nwxLockRead &operator =(const nwxLockRead &x)
  {
    m_fnFile = x.m_fnFile;
    return (*this);
  }

  bool IsOK()
  {
    bool bRtn = m_fnFile.IsFileReadable();
    bRtn = bRtn && (m_fnFile.GetSize() > 0);
    return bRtn;
  }
  bool SetFileName(const wxString &sFileName)
  {
    m_fnFile.Assign(sFileName);
    bool bRtn = IsOK();
    return bRtn;
  }

  bool Touch();
  unsigned long SinceLastAccess();

  bool AccessedSince(unsigned long n = 300)
  {
    bool bRtn = IsOK() ? (SinceLastAccess() <= n) : false;
    return bRtn;
  }
#ifdef __WXDEBUG__
  static void UnitTest(const wxString &sFile);
#endif
private:
  wxFileName m_fnFile;
};
#endif

class nwxLock
{
public:
  enum
  {
    LOCK_ALREADY_LOCKED  = 1,
    LOCK_NO_WRITE_PERM,
    LOCK_NO_DIR,
    LOCK_NO_CREATE,
    LOCK_CANNOT_RELEASE,
    LOCK_CANNOT_UPDATE
  };
  nwxLock();
  nwxLock(const wxString &sDir,
      const wxString &sFileName = nwxLock_FILE,
      unsigned int nTimeout = nwxLock_TIMEOUT);
  virtual ~nwxLock();

  int GetLastError()
  {
    return m_nLastError;
  }
  bool IsOK()
  {
    return m_bDirExists;
  }

  wxString GetLastErrorString(const wxString &sPrefix);
  wxString GetLockUser();
  static wxString GetLockUser(
      const wxString &sDir,
      const wxString &sFileName = nwxLock_FILE);
  bool CannotDeleteLock();
  bool IsLocked();
  bool HaveLock()
  {
    return m_bHaveLock;
  }
  bool Touch(); // periodically update mod time to prevent timeout
  bool IsLockFile(const wxString &sDir, 
    const wxString &sFileName = nwxLock_FILE);
  bool Lock(const wxString &sDir, 
    const wxString &sFileName = nwxLock_FILE);
          // return true if current lock file is identical
          // to the parameters
  bool Lock();
  bool ReleaseLock();
  bool CheckTimeout();
  void SetTimeout(unsigned int nTimeout);
  void Clear();

  unsigned int GetTimeout()
  {
    return m_nTimeout;
  }
#ifdef __WXDEBUG__
  static void UnitTest();
#endif
private:
  void _initLastError()
  {
    m_nLastError = 0;
    m_sLastError.Empty();
  }
  void _setupLastError();
  unsigned long _GetAge();
  bool _CheckAge()
  {
    unsigned long n = _GetAge();
    return (n >= m_nTimeout);
  }
  // 1/31/2018 - need failure to lock reason
  wxString m_sLastError;
  wxFileName m_fnFullPath;
  unsigned long m_nTimeout;
  int m_nLastError;
  bool m_bHaveLock;
  bool m_bDirExists;
};

#endif
