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

*  FileName: nwxLockFile.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_FILE_LOCK_H__
#define __NWX_FILE_LOCK_H__

#include <wx/string.h>
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class nwxLockFile
{
  // lock a file by creating a file with ".lock" appended to the name
  // if such a file exists
public:
  nwxLockFile() {;}
  virtual ~nwxLockFile()
  {
    UnlockAll();
  }

  bool Lock(const wxString &sFileName, time_t nTimeout = nwxLockFile::TIMEOUT)
  {
    return LockWait(sFileName,0,nTimeout);
  }
  bool WaitUntilUnlocked(const wxString &sFileName, time_t nWait = 0, time_t nTimeout = nwxLockFile::TIMEOUT);
  bool LockWait(const wxString &sFileName, time_t nWait = 0, time_t nTimeout = nwxLockFile::TIMEOUT);
  bool IsLocked(const wxString &sFileName);
      // return true if file is locked.  If locked by this instance,
      // the file time is updated to the current time to reset any possible timeout
  bool Unlock(const wxString &sFileName);
  void UnlockAll();
  bool HasLock(const wxString &sFileName);
  void UpdateAll();
  static bool LockFileExists(const wxString &sFileName);
  time_t AccessAge(const wxString &sFileName, time_t tNow = 0);
  time_t GetAge(const wxString &sLockFileName, time_t tNow = 0);
  static const time_t TIMEOUT; // age of a lock file in seconds before it can be considered abandoned
private:
  static wxString GetLockFileName(const wxString &sFileName);
  set<wxString> m_setFileName;
};

#endif

