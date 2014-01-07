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
*  FileName: CIlsLadderInfo.h
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "ConfigDir.h"


class CILSkit : public nwxXmlPersist
{
public:
  CILSkit(const CILSkit &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CILSkit()
  {
    RegisterAll(true);
  }
  virtual ~CILSkit() {}
  const CILSkit &operator = (const CILSkit &x)
  {
    m_sKitName = x.m_sKitName;
    m_sFileName = x.m_sFileName;
    return *this;
  }
  const wxString &GetKitName() const
  {
    return m_sKitName;
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
  wxString GetFilePath()
  {
    ConfigDir *pDir = mainApp::GetConfig();
    wxString sFile = pDir->GetILSLadderFileName(m_sFileName);
    return sFile;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(_T("KitName"),&m_sKitName);
    RegisterWxString(_T("FileName"),&m_sFileName);
  }
  wxString m_sKitName;
  wxString m_sFileName;
};

class CILSkitLess
{
public:
  CILSkitLess() {}
  virtual ~CILSkitLess() {}
  bool operator ()(const CILSkit &a, const CILSkit &b) const
  {
    int n = a.GetKitName().CmpNoCase(b.GetKitName());
    return (n < 0);
  }
  bool operator ()(const CILSkit *pa, const CILSkit *pb) const
  {
    return (*this)(*pa,*pb);
  }
};

class CILSLadderInfo : public nwxXmlPersist
{
public:
  CILSLadderInfo(bool bLoad = false) : m_vKits(_T("Set")), m_bIsOK(false)
  {
    RegisterAll(true);
    if(bLoad)
    {
      Load();
    }
  }
  virtual ~CILSLadderInfo() 
  {
    m_vKits.Clear();
  }
  vector<CILSkit *> *GetKits()
  {
    return m_vKits.Get();
  }
  bool IsKitName(const wxString &sKitName) const
  {
    const std::vector<CILSkit *> *pvKit(m_vKits.Get());
    std::vector<CILSkit *>::const_iterator itr;
    bool bRtn = false;
    for(itr = pvKit->begin(); (itr != pvKit->end()) && (!bRtn); ++itr)
    {
      if(!(*itr)->GetKitName().Cmp(sKitName))
      {
        bRtn = true;
      }
    }
    return bRtn;
  }
  bool Load()
  {
    ConfigDir *pDir = mainApp::GetConfig();
    wxString sFile = pDir->GetILSLadderFileName();
    m_vKits.Clear();
    m_bIsOK = LoadFile(sFile);
    if(!m_bIsOK)
    {
      m_vKits.Clear();
    }
    return m_bIsOK;
  }
  bool IsOK() const
  {
    return m_bIsOK;
  }

protected:
  virtual void RegisterAll(bool = false)
  {
    Register(_T("Kits"),&m_vKits);
  }

private:
  TnwxXmlPersistVector<CILSkit> m_vKits;
  bool m_bIsOK;

};
