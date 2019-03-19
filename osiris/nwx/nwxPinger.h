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
*  FileName: nwxPinger.h
*  Author:   Douglas Hoffman
*  Date:  2/14/2019
*  send event info back to ncbi via https://www.ncbi.nlm.nih.gov/stat
*/
#ifndef __NWX_PINGER_H__
#define __NWX_PINGER_H__

#include <wx/window.h>
#include <wx/string.h>
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include "nwx/stde.h"
#include "nwx/nwxString.h"

class _nwxPingerProcess;
class wxFile;
class nwxPinger;
class wxEvtHandler;

class nwxPingerSet
{
public:
  nwxPingerSet() : m_pPinger(NULL)
  {}
  nwxPingerSet(nwxPinger *p) : m_pPinger(p)
  {}
  nwxPingerSet(const nwxPingerSet &s, nwxPinger *p = NULL) :
    m_pPinger(p)
  {
    (*this) = s;
  }
  virtual ~nwxPingerSet()
  {
    if ( m_mapPairs.size() && (m_pPinger != NULL) )
    {
      Send(m_pPinger);
    }
  }
  bool Send()
  {
    return (m_pPinger == NULL) ? false : Send(m_pPinger);
  }
  bool Send(nwxPinger *pPinger);
  size_t GetSize() const
  {
    return m_mapPairs.size();
  }
  nwxPingerSet &operator =(const nwxPingerSet &s)
  {
    Init();
    m_mapPairs = s.m_mapPairs;
    return(*this);
  }

  void Set(const wxString &sName, const wxString &sValue)
  {
    if (sName.Len())
    {
      m_mapPairs.insert(
        std::multimap<wxString, wxString>::value_type(
          sName, sValue));
    }
  }
  void Set(const wxString &sName, int nValue)
  {
    Set(sName, nwxString::FormatNumber(nValue));
  }
  void Init()
  {
    m_mapPairs.clear();
  }
  size_t GetList(std::vector<wxString> *pvs, bool bIncludeEolGo = true) const
  {
    pvs->clear();
    return AppendList(pvs, bIncludeEolGo);
  }
  size_t AppendList(std::vector<wxString> *pvs, bool bIncludeEolGo = true) const;
private:
  std::multimap<wxString, wxString> m_mapPairs;
  nwxPinger *m_pPinger;
};

class nwxPinger
{
public:
  friend class _nwxPingerProcess;
  nwxPinger(
    wxEvtHandler *parent,
    int id,
    const wxString &sAppName,
    const wxString &sAppVersion,
    const nwxPingerSet *pSetDefaults = NULL,
    wxFile *pLogFile = NULL,
    bool bLog = 
#if defined(TMP_DEBUG) || defined(__WXDEBUG__)
      true
#else
      false
#endif
  );
  nwxPinger();
  virtual ~nwxPinger();
  void Exit();
  bool Ping(const nwxPingerSet &set);
  bool IsRunning()
  {
    return _checkProcess();
  }
  bool Setup(
    wxEvtHandler *parent,
    int id,
    const nwxPingerSet *pSetDefaults = NULL,
    const wxString &sAppName = wxEmptyString,
    const wxString &sAppVersion = wxEmptyString,
    wxFile *pLogFile = NULL,
    bool bLog = false)
  {
    bool rtn = false;
    if (!m_bSetup)
    {
      m_pLogFile = pLogFile;
      m_bLog = bLog;
      rtn = _setup(parent, id, pSetDefaults, sAppName, sAppVersion);
    }
    return rtn;
  }
private:
  bool _setup(
    wxEvtHandler *parent,
    int id,
    const nwxPingerSet *pSetDefaults = NULL,
    const wxString &sAppName = wxEmptyString,
    const wxString &sAppVersion = wxEmptyString);
  bool _checkProcess();
  void _processExit(int n)
  {
    _logExit(n);
    m_process = NULL;
  }
  void _logExit(int n);
  _nwxPingerProcess *m_process;
  wxFile *m_pLogFile;
  bool m_bLog;
  bool m_bSetup;
  bool m_bInDestructor;
  bool m_bLoggedExit;
};



#endif