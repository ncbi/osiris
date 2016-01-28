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

*  FileName: CKitList.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_KIT_LIST_H__
#define __C_KIT_LIST_H__
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include <set>
#include <wx/arrstr.h>
#include "nwx/stde.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/CIncrementer.h"
#include "nwx/nsstd.h"

class CILSLadderInfo;

//  new for 2.7 CKitChannelMap, CKitChannel
//  CKitChannelMap stores <FsaChannelMap>
//  CKitChannel stores <FsaChannelMap><Channel>

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
  int GetKitNr() const
  {
    return m_nKitNr;
  }
  int GetFsaNr() const
  {
    return m_nFsaNr;
  }
  int GetKey() const  
  {
    return m_nKitNr;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterInt(wxT("KitChannelNumber"),&m_nKitNr);
    RegisterInt(wxT("fsaChannelNumber"),&m_nFsaNr);
    RegisterWxString(wxT("Color"),&m_sColor);
    RegisterWxString(wxT("DyeName"),&m_sDyeName);
  }
private:
  wxString m_sColor;
  wxString m_sDyeName;
  int m_nKitNr;
  int m_nFsaNr;
};

typedef TnwxXmlPersistMap<int,CKitChannel> CKitChannelMap;

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
  int GetChannel() const
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
    RegisterInt("Channel",&m_nChannel);
    RegisterInt("MinBP",&m_nMinBP);
    RegisterInt("MaxBP",&m_nMaxBP);
  }
private:
  wxString m_sName;
  int m_nChannel;
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
  typedef map<wxString, wxArrayString *>::iterator LSitr;
  typedef map<wxString, wxArrayString *>::const_iterator LScitr;
  typedef map< wxString, CLocusNameList * >::iterator KLNCitr;
  typedef map< wxString, CLocusNameList * >::const_iterator KLNCcitr;
  typedef map< wxString, CKitChannelMap *>::iterator KCMitr;
  typedef map< wxString, CKitChannelMap *>::iterator KCMcitr;
public:
  CPersistKitList() : nwxXmlPersist(true)
  {
    m_nInLoad = 0;
    m_bV1 = false;
    m_pLastKitLocus = NULL;
    m_pLastKitLS = NULL;
    m_pLastKit_ILS = NULL;

    Register("Kits",this);
    Register("Set",this);
    Register("Name",this);
    Register("LS",this);
    Register("LSName",this);
    Register("ChannelNo",this);
    Register("Locus",this);
    // new for v 2.7
    Register("ILS",this);
    Register("LSBases",this);
    Register("ILSName",this);
    Register("FsaChannelMap",this);
  }
  ~CPersistKitList()
  {
    Clear();
  }

  bool Load_V1(); // load kit data for Osiris Version 1.x
  bool Load();    // load kit data for Osiris Version 2.x
  void SortILS();
  virtual bool LoadFromNode(wxXmlNode *pNode);
  const wxArrayString &GetArray() const
  {
    return m_as;
  }
  const wxArrayString *GetLsArray(const wxString &sKit) const
  {
    LScitr itr = m_mLS.find(sKit);
    return (itr == m_mLS.end()) ? NULL : itr->second;
  }
  const CLocusNameList *GetLocusNameList(const wxString &sKit) const
  {
    KLNCcitr itr = m_mapKitLocus.find(sKit);
    const CLocusNameList *pRtn =
      (itr == m_mapKitLocus.end()) ? NULL : itr->second;
    return pRtn;
  }
  int GetChannelCount(const wxString &sKit) const
  {
    map<wxString, int>::const_iterator
      itr = m_msChannelCount.find(sKit);
    int nRtn =
      (itr == m_msChannelCount.end()) ? 0 : itr->second;
    return nRtn;
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
  void Add(const wxString &s)
  {
    m_as.Add(s);
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
  void _HACK_27(CILSLadderInfo *pILS);
  bool m_bV1;
  int m_nInLoad;
  wxArrayString m_as;
  wxString m_sLastKit;
  map<wxString, wxArrayString *> m_mLS;
  map<wxString, wxArrayString *> m_mILS; // 2.7 ils family
  map<wxString, int> m_msChannelCount;
  nwxXmlIOwxString m_XmlString;
  wxString m_sErrorMsg;
  map< wxString, CLocusNameList * > m_mapKitLocus;
  map< wxString, CKitChannelMap *> m_mapKitChannels;

  CLocusNameList *m_pLastKitLocus;
  wxArrayString *m_pLastKitLS;
  wxArrayString *m_pLastKit_ILS; // v2.7 ils family
  CKitChannelMap *m_pLastKitChannelMap;
};

#endif
