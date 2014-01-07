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
*  FileName: nwxKillDir.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_KILL_DIR_H__
#define __NWX_KILL_DIR_H__

#include "nwx/stdb.h"
#include <list>
#include <set>
#include "nwx/nsstd.h"
#include "nwx/stde.h"
#include <wx/string.h>
#include <wx/dir.h>
#include <wx/datetime.h>

class nwxKillDir : public wxDirTraverser
{
public:
  nwxKillDir() 
  {
    m_t = 0;
    m_bRtn = true;
    m_bRecursive = false;
  }
  nwxKillDir(const wxString &sDir)
  {
    m_t = 0;
    m_bRtn = Kill(sDir, true);
  }
  nwxKillDir(
    const wxString &sDir,
    bool bRecursive,
    const wxDateTime &dt)
  {
    m_t = 0;
    m_bRtn = KillAfter(sDir,bRecursive,dt);
  }
  nwxKillDir(
    const wxString &sDir,
    bool bRecursive,
    time_t t)
  {
    m_bRtn = Kill(sDir,bRecursive,t);
  }
  virtual ~nwxKillDir() {;}
  bool Kill(
    const wxString &sDir,
    bool bRecursive = true,
    time_t t = (time_t)0);
  bool KillAfter(
    const wxString &sDir,
    bool bRecursive,
    const wxDateTime &dt
    )
  {
    return Kill(sDir,bRecursive,dt.GetTicks());
  }
  bool GetStatus()
  {
    return m_bRtn;
  }
  virtual wxDirTraverseResult OnDir(const wxString &dirname);
  virtual wxDirTraverseResult OnFile(const wxString &filename);
  static bool KillStatic(const wxString &sDir)
  {
    nwxKillDir x;
    bool bRtn = x.Kill(sDir);
    return bRtn;
  }
  static bool KillAfterStatic(
    const wxString &sDir, 
    bool bRecursive, 
    const wxDateTime &dt)
  {
    nwxKillDir x;
    bool bRtn = x.KillAfter(sDir,bRecursive,dt);
    return bRtn;
  }
#ifdef _DEBUG
  static void UnitTest();
#endif
private:
  void _Kill(const wxString &sDir);
  list<wxString> m_asFiles;
  list<wxString> m_asDirs;
  time_t m_t;
  bool m_bRtn;
  bool m_bRecursive;
};


#endif
