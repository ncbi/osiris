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
#include <wx/datetime.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "ConfigDir.h"
class wxArrayString;
// v2.7 CILSfamily and CILSname.  Load ILS 'family' names with LS names
class CILSname : public nwxXmlPersist
{
public:
  CILSname(const CILSname &x) : nwxXmlPersist(true)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CILSname() : nwxXmlPersist(true)
  {
    RegisterAll(true);
  }
  const CILSname &operator = (const CILSname &x)
  {
    m_sName = x.m_sName;
    m_sDisplayName = x.m_sDisplayName;
    return *this;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const wxString &GetDisplayName() const
  {
    return m_sDisplayName;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("Name"),&m_sName);
    RegisterWxString(wxT("DisplayName"),&m_sDisplayName);
  }
private:
  wxString m_sName;
  wxString m_sDisplayName;
};
class CILSfamily : public nwxXmlPersist
{
public:
  CILSfamily(const CILSfamily &x) : 
      nwxXmlPersist(true), 
      m_io(false)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CILSfamily() : 
      nwxXmlPersist(true),
      m_io(false)
  {
    RegisterAll(true);
  }
  virtual ~CILSfamily() 
  {
    vectorptr<CILSname>::cleanup(&m_vNames);
  }
  const CILSfamily &operator = (const CILSfamily &x)
  {
    m_sILSname = x.m_sILSname;
    m_sDyeName = x.m_sDyeName;
    vectorptr<CILSname>::copy(&m_vNames,x.m_vNames);
    return *this;
  }
  const wxString &GetILSname() const
  {
    return m_sILSname;
  }
  const wxString &GetKey() const
  {
    return m_sILSname;
  }
  const wxString  &GetDyeName() const
  {
    return m_sDyeName;
  }
  const std::vector<CILSname *> &GetNames() const
  {
    return m_vNames;
  }
  bool HasLS(const wxString &sLSname)
  {
    std::vector<CILSname *>::const_iterator itr;
    bool bRtn = false;
    for(itr  = m_vNames.begin(); !bRtn && (itr != m_vNames.end()); ++itr)
    {
      if((*itr)->GetName() == sLSname)
      {
        bRtn = true;
      }
    }
    return bRtn;
  }
#ifdef __WXDEBUG__
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    return nwxXmlPersist::LoadFromNode(p,pObj);
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    return nwxXmlPersist::LoadFromNode(p);
  }
#endif
  void SetFileName(const wxString &sFileName)
  {
    m_sFileName = sFileName;
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("ILSName"),&m_sILSname);
    RegisterWxString(wxT("DyeName"),&m_sDyeName);
    Register(wxT("SubFamily"),this);
    Register(wxT("LaneStandard"),&m_io,&m_vNames);
  }
private:
  wxString m_sILSname;
  wxString m_sDyeName;
  wxString m_sFileName;
  TnwxXmlIOPersistVector<CILSname> m_io;
  std::vector<CILSname *> m_vNames; // must be after m_io
  //SUBFamily m_SUBFamily;
};

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
  wxString GetFilePath() const
  {
    ConfigDir *pDir = mainApp::GetConfig();
    wxString sFile = pDir->GetILSLadderFileName(m_sFileName);
    return sFile;
  }
#ifdef __WXDEBUG__
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    return nwxXmlPersist::LoadFromNode(p,pObj);
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    return nwxXmlPersist::LoadFromNode(p);
  }
#endif
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("KitName"),&m_sKitName);
    RegisterWxString(wxT("FileName"),&m_sFileName);
  }
  wxString m_sKitName;
  wxString m_sFileName;
};

class CILSLadderInfo : public nwxXmlPersist
{
public:
  CILSLadderInfo();
  virtual ~CILSLadderInfo()
  {
    _Cleanup();
  }
  bool CheckReload();
  bool IsOK() const
  {
    return m_bIsOK;
  }
  const CILSfamily *GetFamily(const wxString &sILSName) const
  {
    ConstIterator itr = m_mapCILSfamily.find(sILSName);
    const CILSfamily *pRtn = (itr == m_mapCILSfamily.end()) ? NULL : itr->second;
    return pRtn;
  }
  const wxString &GetDyeName(const wxString &sILSName) const
  {
    const CILSfamily *pFam = GetFamily(sILSName);
    const wxString &s = (pFam != NULL) ? pFam->GetDyeName() : mainApp::EMPTY_STRING;
    return s;
  }
  const CILSfamily *GetFamilyFromLS(const wxString &sLSname) const
  {
    std::map<wxString,CILSfamily *>::const_iterator itr;
    const CILSfamily *pRtn = NULL;
    for(itr = m_mapCILSfamily.begin(); (pRtn == NULL) && (itr != m_mapCILSfamily.end()); ++itr)
    {
      if(itr->second->HasLS(sLSname))
      {
        pRtn = itr->second;
      }
    }
    return pRtn;
  }
  const wxString GetDyeNameFromLS(const wxString &sLSname) const
  {
    wxString sRtn;
    const CILSfamily *pFam = GetFamilyFromLS(sLSname);
    if(pFam != NULL) { sRtn = pFam->GetDyeName(); }
    return sRtn;
  }
  const wxString &FindDisplayName(const wxString &sLSname) const;
  void BuildAll_ILSarray(wxArrayString *ps) const;

#ifdef __WXDEBUG__
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    return nwxXmlPersist::LoadFromNode(p,pObj);
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    return nwxXmlPersist::LoadFromNode(p);
  }
#endif

protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString(wxT("Version"),&m_sVersion);
    Register(wxT("LaneStandards"),&m_mapCILSfamilyTmp);
  }

private:
  bool _Load(); // private because anything outside of the constructor should call CheckReload()
  bool _LoadFile(const wxString &sFileName)
  {
    bool bRtn = LoadFile(sFileName);
    if (bRtn)
    {
      _MoveData(sFileName);
    }
    else
    {
      m_mapCILSfamilyTmp.Clear();
    }
    return bRtn;
  }
  bool _SetupSiteFileNames();
  bool _NeedReload();
  void _Cleanup();
  void _BuildNameToDisplay() const;
  void _MoveData(const wxString &sFileName);
  wxString m_sVersion;
  wxDateTime m_dtSiteFile;
  std::map<wxString, wxDateTime> m_mapSiteFiles;
  std::map<wxString,CILSfamily *> m_mapCILSfamily;
  typedef std::map<wxString, CILSfamily *>::iterator Iterator;
  typedef std::map<wxString, CILSfamily *>::const_iterator ConstIterator;
  TnwxXmlPersistMap<wxString, CILSfamily> m_mapCILSfamilyTmp;
  mutable std::map<const wxString ,const wxString &> m_mapNameToDisplay;
  bool m_bIsOK;
};
