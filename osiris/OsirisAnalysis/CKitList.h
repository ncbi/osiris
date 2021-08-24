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
*  FileName: CKitList.h
*  Author:   Douglas Hoffman
*
*   1/28/16
*   this needs to be written because:
*    1. CPersistKitList has a separate structure for everything about each kit:
      std::map<wxString, wxArrayString *> m_mLS;               // lane standard for each kit
      std::map<wxString, wxArrayString *> m_mILS;              // ils family for each kit, where applicable
      std::map<wxString, int> m_msChannelCount;                // ILS channel for each kit
      std::map< wxString, CLocusNameList * > m_mapKitLocus;    // loci for each kit
      std::map< wxString, CKitChannelMap *> m_mapKitChannels;  // channels for each kit

      This should be changed to a collection of kit objects where each object has all of the above for one kit

*    2. All kits are loaded at once and there are now over 30.  It would be better to load on demand
*
*/
#ifndef __C_KIT_LIST_H__
#define __C_KIT_LIST_H__
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include <set>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include "nwx/stde.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/CIncrementer.h"
#include "nwx/nsstd.h"
class CILSLadderInfo;
//  new for 2.7 CKitChannelMap, CKitChannel
//  CKitChannelMap stores <FsaChannelMap>
//  CKitChannel stores <FsaChannelMap><Channel>

class CKitLadderFiles : public wxDirTraverser
{
public:
  CKitLadderFiles()
  {
    m_dtDirSite.Set(time_t(0));
    Reload();
  }
  virtual ~CKitLadderFiles()
  {}
  bool Reload();
  virtual wxDirTraverseResult OnDir(const wxString& dirname);
  virtual wxDirTraverseResult OnFile(const wxString& filename);
  virtual wxDirTraverseResult OnOpenError(const wxString& openerrorname);
  const wxArrayString *operator ->() const
  {
    return &m_asList;
  }
  bool IsOK()
  {
    return m_asErrorList.IsEmpty();
  }
  const wxArrayString &Files() const
  {
    return m_asList;
  }
  const wxArrayString &Errors() const
  {
    return m_asErrorList;
  }
  const wxDateTime GetSiteModTime() const
  {
    return m_dtDirSite;
  }

private:
  void _Load(const wxString &sFilePath, bool bSiteFolder);
  wxDateTime m_dtDirSite;
  wxArrayString m_asList;
  wxArrayString m_asErrorList;
  static const wxString g_sEndName;
};

class CKitChannel : public nwxXmlPersist
{
public:
  CKitChannel() : nwxXmlPersist(true)
  {
    RegisterAll(true);
  }
  CKitChannel(const CKitChannel &x) : nwxXmlPersist(true)
  {
    RegisterAll(true);
    (*this) = x;
  }
  const CKitChannel &operator = (const CKitChannel &x)
  {
    m_nKitNr = x.m_nKitNr;
    m_nFsaNr = x.m_nFsaNr;
    m_sColor = x.m_sColor;
    m_sDyeName = x.m_sDyeName;
    return *this;
  }
  const wxString &GetColor() const
  {
    return m_sColor;
  }
  const wxString &GetDyeName() const
  {
    return m_sDyeName;
  }
  unsigned int GetKitNr() const
  {
    return m_nKitNr;
  }
  unsigned int GetFsaNr() const
  {
    return m_nFsaNr;
  }
  unsigned int GetKey() const  
  {
    return m_nKitNr;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterUint(wxT("KitChannelNumber"),&m_nKitNr);
    RegisterUint(wxT("fsaChannelNumber"),&m_nFsaNr);
    RegisterWxString(wxT("Color"),&m_sColor);
    RegisterWxString(wxT("DyeName"),&m_sDyeName);
  }
private:
  wxString m_sColor;
  wxString m_sDyeName;
  unsigned int m_nKitNr;
  unsigned int m_nFsaNr;
};

typedef TnwxXmlPersistMap<unsigned int,CKitChannel> CKitChannelMap;

class CLocusNameChannel : public nwxXmlPersist
{
public:
  CLocusNameChannel() 
    : nwxXmlPersist(true)
  {
    RegisterAll(true);
  }
  CLocusNameChannel(const CLocusNameChannel &x)
    : nwxXmlPersist(true)
  {
    RegisterAll(true);
    (*this) = x;
  }
  CLocusNameChannel &operator = (const CLocusNameChannel &x)
  {
    m_sName = x.m_sName;
    m_nChannel = x.m_nChannel;
    return (*this);
  }
  unsigned int GetChannel() const
  {
    return m_nChannel;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  bool operator <(const CLocusNameChannel &x) const;
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Name",&m_sName);
    RegisterUint("Channel",&m_nChannel);
    RegisterInt("MinBP",&m_nMinBP);
    RegisterInt("MaxBP",&m_nMaxBP);
  }
private:
  wxString m_sName;
  unsigned int m_nChannel;
  int m_nMinBP;
  int m_nMaxBP;
};

class CLocusNameChannelLess
{
public:
  CLocusNameChannelLess(){}
  virtual ~CLocusNameChannelLess() {}
  bool operator()(const CLocusNameChannel &x1, const CLocusNameChannel &x2) const
  {
    return x1 < x2;
  }
  bool operator()(const CLocusNameChannel *px1, const CLocusNameChannel *px2) const
  {
    return (*px1) < (*px2);
  }
};


class CLocusNameList : 
  public set<const CLocusNameChannel *,CLocusNameChannelLess>
{
public:
  const wxArrayString &GetArrayString(bool bRebuild = false) const
  {
    if(bRebuild || (m_as.GetCount() != size()))
    {
      m_as.Clear();
      m_as.Alloc(size());
      for(const_iterator itr = begin(); itr != end(); itr++)
      {
        m_as.Add((*itr)->GetName());
      }
    }
    return m_as;
  }
private:
  mutable wxArrayString m_as;
};

class CPersistKitList : public nwxXmlPersist
{
private:
  typedef std::map<wxString, wxArrayString *>::iterator LSitr;
  typedef std::map<wxString, wxArrayString *>::const_iterator LScitr;
  typedef std::map< wxString, CLocusNameList * >::iterator KLNCitr;
  typedef std::map< wxString, CLocusNameList * >::const_iterator KLNCcitr;
  typedef std::map< wxString, CKitChannelMap *>::iterator KCMitr;
  typedef std::map< wxString, CKitChannelMap *>::const_iterator KCMcitr;
public:
  CPersistKitList() : nwxXmlPersist(true)
  {
    m_pILS = NULL;
    m_pLastKitLocus = NULL;
    m_pLastKitLS = NULL;
    m_pLastKit_ILS = NULL;
    m_pLastKitChannelMap = NULL;
    m_nInLoad = 0;
    m_bV1 = false;
    m_dtSiteILSLadderDir.Set(time_t(0));

    Register("Kits",this);
    Register("Set",this);
    Register("Name",this);
    Register("LS",this);
    Register("LSName",this);
    Register("ChannelNo",this);
    Register("Locus", this);
    // new for v 2.7
    Register("ILS", this);
    Register("LSBases", this);
    Register("ILSName", this);
    Register("FsaChannelMap", this);
    // new for 2.12
    Register("LadderFree", this);
    Register("AllLaneStandards", this);
  }
  virtual ~CPersistKitList();

  bool Load_V1(); // load kit data for Osiris Version 1.x - unsupported
  bool Load();    // load kit data for Osiris Version 2.x
  CILSLadderInfo *GetILSLadderInfo();

  void SortILS();
  virtual bool LoadFromNode(wxXmlNode *pNode);
  const wxArrayString *_GetAll_ILSarray() const;
  const wxArrayString *GetLsArray(const wxString &sKit) const
  {
    const wxArrayString *pRtn = NULL;
    if (!IsAll_ILS(sKit))
    {
      LScitr itr = m_mLS.find(sKit);
      pRtn = (itr == m_mLS.end()) ? NULL : itr->second;
    }
    else
    {
      pRtn = _GetAll_ILSarray();
    }
    return pRtn;
  }
  const wxArrayString &GetKitList() const
  {
    return m_as;
  }
  const wxArrayString *GetIlsArray(const wxString &sKit) const
  {
    LScitr itr = m_mILS.find(sKit);
    return (itr == m_mILS.end()) ? NULL : itr->second;
  }
  bool ILSDyeNamesByFamily(const wxString &sKit) const
  {
    bool b = GetIlsArray(sKit) != NULL;
    return b;
  }
  const CLocusNameList *GetLocusNameList(const wxString &sKit) const
  {
    KLNCcitr itr = m_mapKitLocus.find(sKit);
    const CLocusNameList *pRtn =
      (itr == m_mapKitLocus.end()) ? NULL : itr->second;
    return pRtn;
  }
  int GetChannelCount(const wxString &sKit) const 
  // 1/28/2016 djh
  // this actually retrieves the ILS channel number
  // and at this time is always equal to the number of
  // channels but that could change
  {
    std::map<wxString, int>::const_iterator
      itr = m_msChannelCount.find(sKit);
    int nRtn =
      (itr == m_msChannelCount.end()) ? 0 : itr->second;
    return nRtn;
  }
  unsigned int GetILSchannelNumber(const wxString &sKit) const
  {
    // 1/28/16
    // ILS is always the last channel, so far
    int n = GetChannelCount(sKit);
    return (unsigned int)n;
  }
  const wxArrayString *GetLocusArrayString(
    const wxString &sKit, bool bRebuild = false) const
  {
    const CLocusNameList *pList = GetLocusNameList(sKit);
    const wxArrayString *pRtn(NULL);
    if(pList != NULL)
    {
      const wxArrayString &as = pList->GetArrayString(bRebuild);
      pRtn = &as;
    }
    return pRtn;
  }
  const std::map<unsigned int, CKitChannel *> *GetChannelColorMap(const wxString &sKit) const
  {
    KCMcitr itr = m_mapKitChannels.find(sKit);
    CKitChannelMap *pMap =
      (itr != m_mapKitChannels.end()) ? itr->second : NULL;
    const std::map<unsigned int, CKitChannel *> *pRtn = (pMap != NULL) ? pMap->Get() : NULL;
    return pRtn;
  }
  bool IsLadderFree(const wxString &sKit) const
  {
    bool b = m_setLadderFree.find(sKit) != m_setLadderFree.end();
    return b;
  }
  bool IsAll_ILS(const wxString &sKit) const
  {
    bool b = m_setAll_ILS.find(sKit) != m_setAll_ILS.end();
    return b;
  }
  void Clear();

  virtual void Init(void *p)
  {
    if(!m_nInLoad)
    {
      Clear();
    }
    nwxXmlPersist::Init(p);
  }
  virtual void Init()
  {
    if(!m_nInLoad)
    {
      Clear();
    }
    nwxXmlPersist::Init();
  }
  void Add(const wxString &sKitName, const wxString &sLadderInfoFileName)
  {
    m_as.Add(sKitName); 
    m_mapKitToLadderInfo.insert(
      std::pair<wxString, wxString>(sKitName, sLadderInfoFileName));
  }
  const wxString GetLadderInfoFileName(const wxString &sKitName) const
  {
    std::map<wxString, wxString>::const_iterator itr = m_mapKitToLadderInfo.find(sKitName);
    wxString s;
    if (itr != m_mapKitToLadderInfo.end())
    {
      s = itr->second;
    }
    return s;
  }
  bool KitExists(const wxString &sKitName)
  {
    bool b = m_mapKitToLadderInfo.find(sKitName) != m_mapKitToLadderInfo.end();
    return b;
  }
  size_t GetCount()
  {
    return m_as.GetCount();
  }
  const wxString &GetErrorMsg() const
  {
    return m_sErrorMsg;
  }
  bool CheckLastKit(const wxString &sKitName)
  {
    bool bRtn = (sKitName == m_sLastKit);
    if(!bRtn)
    {
      m_sLastKit = sKitName;
      m_pLastKitLocus = NULL;
      m_pLastKitLS = NULL;
      m_pLastKit_ILS = NULL;
    }
    return bRtn;
  }
  bool NeedReload();
  bool CheckReload()
  {
    bool bRtn = NeedReload() ? Load() : true;
    return bRtn;
  }
#ifdef __WXDEBUG__
  static void UnitTest();
#endif
private:

  void _AddError()
  {
    m_sErrorMsg.Append("Cannot load kit: ");
    m_sErrorMsg.Append(m_sLastKit);
    m_sErrorMsg.Append("\n");
  }
  void _SetLoadError()
  {
    m_sErrorMsg = "Cannot load ladder information.";
  }

  void _HACK_27();
  void _CHECK_BOOL(wxXmlNode *pNode, std::set<wxString> *pSet);
  wxArrayString m_as;
  wxString m_sLastKit;
  std::map<wxString, wxString> m_mapKitToLadderInfo;
  std::map<wxString, wxArrayString *> m_mLS;
  std::map<wxString, wxArrayString *> m_mILS; // 2.7 ils family
  std::map<wxString, int> m_msChannelCount;
  nwxXmlIOwxString m_XmlString;
  nwxXmlIObool m_XmlBool;
  wxString m_sErrorMsg;
  std::map< wxString, CLocusNameList * > m_mapKitLocus;
  std::map< wxString, CKitChannelMap *> m_mapKitChannels;
  std::set<wxString> m_setLadderFree;
  std::set<wxString> m_setAll_ILS;
  wxDateTime m_dtSiteILSLadderDir;
  mutable wxArrayString m_asAll_ILS;

  CILSLadderInfo *m_pILS;
  CLocusNameList *m_pLastKitLocus;
  wxArrayString *m_pLastKitLS;
  wxArrayString *m_pLastKit_ILS; // v2.7 ils family
  CKitChannelMap *m_pLastKitChannelMap;
  bool m_bV1;
  int m_nInLoad;
};

#endif
