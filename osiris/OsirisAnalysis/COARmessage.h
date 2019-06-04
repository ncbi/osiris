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
*  FileName: COARmessage.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_OAR_MESSAGE_H__
#define __C_OAR_MESSAGE_H__

#include "COARmisc.h"

class COARmsgExportMap;

//********************************************************* COARmessage


class COARmessage : public nwxXmlPersist
{
public:
  COARmessage() : m_tLastUpdate((time_t)0)
  {
    RegisterAll(true);
  }
  COARmessage(const COARmessage &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARmessage() {;}
  virtual void RegisterAll(bool = false);
  int GetMessageNumber() const
  {
    return m_nMessageNumber;
  }
  const wxString &GetText() const
  {
    return m_sText;
  }
  int GetIndex() const // index number in MessageBook xml file
  {
    return m_nIndex;
  }
  bool GetHidden() const
  {
    return m_bHidden;
  }
  bool GetEditable() const
  {
    return m_bEditable;
  }
  bool GetHiddenOrNotEditable() const
  {
    return m_bHidden || !m_bEditable;
  }
  void SetHidden(bool b)
  {
    m_bHidden = b;
    m_bEnabled = !b;
  }
  void SetText(const wxString &s)
  {
    m_sText = s;
  }
  bool operator ==(const COARmessage &x) const
  {
    bool bRtn = true;
#define CMP(field) if(field != x.field) { bRtn = false; }
    CMP(m_sText)
    else CMP(m_bHidden)
    else CMP(m_bEnabled)
    else CMP(m_sMsgName)
    else CMP(m_nMessageNumber)
    else CMP(m_nIndex)
    else CMP(m_bCritical)
    else CMP(m_bEditable)
    else CMP(m_bDisplayExportInfo)
    else CMP(m_bDisplayOsiris)
    // DO NOT COMPARE last update // else CMP(m_tLastUpdate)
#undef CMP
    return bRtn;
  }
  bool operator != (const COARmessage &x) const
  {
    return !((*this) == x);
  }
  bool IsDifferent(const COARmessage &x) const
  {
    return !((*this) == x);
  }
  void SetTime(const wxDateTime &t)
  {
    m_tLastUpdate = t;
  }
  const wxDateTime &GetTime() const
  {
    return m_tLastUpdate;
  }
  bool IsModified() const
  {
    return m_tLastUpdate.GetTicks() > 0;
  }
  const wxString &GetMessageName() const
  {
    return m_sMsgName;
  }
  bool DisplayOsiris() const
  {
    return m_bDisplayOsiris;
  }
  bool IsDisplayed() const
  {
    return m_bDisplayOsiris && !m_bHidden;
  }
private:
  wxString m_sText;
  wxString m_sMsgName;
  wxDateTime m_tLastUpdate;
  int m_nMessageNumber;
  int m_nIndex;
  bool m_bHidden;
  bool m_bEnabled;  // should always be opposite of hidden
  bool m_bCritical; // default is true;
  bool m_bEditable; // default is true
  bool m_bDisplayExportInfo; // default is false
  bool m_bDisplayOsiris; // default is true

};


//********************************************************* COARmessages

class COARmessages : public nwxXmlPersist
{
public:
  COARmessages() : m_pMsgExport(NULL)
  {
    RegisterAll(true);
  }
  COARmessages(const COARmessages &x) : m_pMsgExport(NULL)
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARmessages &operator=(const COARmessages &x)
  {
    // NOTE: when messages are copied, either here 
    // or in COARmessages::CopyOnly()
    // it is expected that the destination instance
    // will be destroyed before the source, 
    // otherwise, m_pMsgExport should be set to NULL

    Init();
    COARcopyVP(COARmessage,m_vpMessage);
    COARcopyVP(COARmessage,m_vpMessageEdited);
    SetMsgExport(x.GetMsgExport(),false);
    return *this;
  }
  virtual ~COARmessages()
  {
    Cleanup();
  }
  void Cleanup()
  {
    _InitMaps();
    vectorptr<COARmessage>::cleanup(&m_vpMessage);
    vectorptr<COARmessage>::cleanup(&m_vpMessageEdited);
  }

  // SEE NOTES IN operator= above
  void CopyOnly(
    const COARmessages &x, 
    const vector<int> *pvn,
    bool bAppend = false);
  void CopyOnly(
    const COARmessages &x, 
    const set<int> *psn,
    bool bAppend = false);
  void KeepOnly(const vector<int> *pvn);
  void KeepOnly(const set<int> *psn);
  virtual void Init()
  {
    _InitMaps();
    nwxXmlPersist::Init();
  }
  virtual void Init(void *p)
  {
    _InitMaps();
    if(p != NULL && p != (void *)this)
    {
      ((COARmessages *)p)->_InitMaps();
    }
    nwxXmlPersist::Init(p);
  }
  static bool IsModified(const COARmessages *pm1, const COARmessages *pm2);
  bool IsModified(const COARmessages &msgs) const
  {
    return IsModified(&msgs,this);
  }
  virtual void RegisterAll(bool = false);

  const COARmessage *GetMessageByNumber(int n) const;
  COARmessage *GetMessageByNumber(int n)
  {
    const COARmessages *pm(this);
    const COARmessage *pRtn = pm->GetMessageByNumber(n);
    return (COARmessage *) pRtn;
/*
    _BuildMap();
    map<int,COARmessage *>::const_iterator itr =
      m_mapMessage.find(n);
    if(itr != m_mapMessage.end())
    {
      pRtn = itr->second;
    }
    return pRtn;
*/
  }
  const COARmessage *GetMessageByNumber(int n, const wxDateTime *pTime) const;
  COARmessage *GetMessageByNumber(int n, const wxDateTime *pTime)
  {
    const COARmessages *pm(this);
    const COARmessage *pRtn = pm->GetMessageByNumber(n,pTime);
    return (COARmessage *)pRtn;
  }

  bool IsMessageEdited(int n, const wxDateTime *pTime = NULL) const;
  bool IsEditedSince(
    int n, const wxDateTime &dtSince, const wxDateTime *pTime = NULL) const
  {
    const COARmessage *pMsg = GetMessageByNumber(n,pTime);
    bool bRtn = (pMsg->GetTime() > dtSince);
    return bRtn;
  }
  void SetHidden(int n, bool bHidden)
  {
    const COARmessage *pMsg = GetMessageByNumber(n);
    if(pMsg != NULL)
    {
      COARmessage *pM = (COARmessage *) pMsg;
      pM->SetHidden(bHidden);
    }
  }
  void SetHidden(const map<int,bool> &mapHide)
  {
    for(map<int,bool>::const_iterator itr = mapHide.begin();
      itr != mapHide.end();
      ++itr)
    {
      SetHidden(itr->first,itr->second);
    }
  }
  bool IsHidden(int n, const wxDateTime *pTime = NULL) const
  {
    const COARmessage *pMsg = GetMessageByNumber(n,pTime);
    return (pMsg == NULL) || pMsg->GetHidden();
  }
  bool IsDisplayed(int n, const wxDateTime *pTime = NULL) const
  {
    const COARmessage *pMsg = GetMessageByNumber(n,pTime);
    return (pMsg != NULL) && pMsg->IsDisplayed();
  }
  void GetHidden(
    const vector<int> &vn, map<int,bool> *pMap, 
    const wxDateTime *pTime = NULL) const
  {
    int n;
    const COARmessage *pMsg;
    pMap->clear();
    for(vector<int>::const_iterator itr = vn.begin();
      itr != vn.end();
      ++itr)
    {
      n = *itr;
      pMsg = GetMessageByNumber(n,pTime);
      if(pMsg != NULL)
      {
        pMap->insert(map<int,bool>::value_type(n,pMsg->GetHidden()));
      }
    }
  }
  void GetHidden(
    COARalerts &alerts,map<int,bool> *pMap, 
    const wxDateTime *pTime = NULL) const
  {
    const vector<int> *pvn = alerts.Get();
    GetHidden(*pvn,pMap,pTime);
  }
  bool FindNotHidden(const vector<int> &vn, const wxDateTime *pTime = NULL) const;
  bool FindNotHidden(const COARalerts &a, const wxDateTime *pTime = NULL) const
  {
    return FindNotHidden(*a.Get(),pTime);
  }
  size_t CountDisplayed(const vector<int> &vn, const wxDateTime *pTime = NULL, bool bStopAtOne = false) const;
  size_t CountDisplayed(const COARalerts &a, const wxDateTime *pTime = NULL, bool bStopAtOne = false) const
  {
    return CountDisplayed(*a.Get(),pTime,bStopAtOne);
  }
  bool FindDisplayed(const vector<int> &vn, const wxDateTime *pTime = NULL) const
  {
    return (CountDisplayed(vn,pTime,true) > 0);
  }

  bool FindDisplayed(const COARalerts &a, const wxDateTime *pTime = NULL) const
  {
    return (CountDisplayed(*a.Get(),pTime,true) > 0);
  }

  size_t CountNotHidden(const vector<int> &vn, const wxDateTime *pTime = NULL) const;
  size_t CountNotHidden(const COARalerts &a, const wxDateTime *pTime = NULL) const
  {
    return CountNotHidden(*a.Get(),pTime);
  }

  bool FindEdited(const vector<int> &vn,const wxDateTime *pTime = NULL) const;
  bool FindEdited(const COARalerts &a,const wxDateTime *pTime = NULL) const
  {
    return FindEdited(*a.Get(),pTime);
  }
  size_t GetMessageCount() const
  {
    return m_vpMessage.size();
  }
  COARmessage *GetMessage(size_t n)
  {
    return m_vpMessage.at(n);
  }
  const COARmessage *GetMessage(size_t n) const
  {
    return m_vpMessage.at(n);
  }
  const COARmessage *GetMessage(size_t n, const wxDateTime *pTime) const
  {
    const COARmessage *pRtn(GetMessage(n));
    if(pTime != NULL)
    {
      int nn = pRtn->GetMessageNumber();
      pRtn = GetMessageByNumber(nn,pTime);
    }
    return pRtn;
  }
  void AddMessage(const COARmessage &msg, const wxDateTime &t)
  {
    _InitMap();
    COARmessage *pNew = new COARmessage(msg);
    pNew->SetTime(t);
    m_vpMessage.push_back(pNew);
  }
  void AddMessage(const COARmessage &msg)
  {
    _InitMap();
    COARmessage *pNew = new COARmessage(msg);
    m_vpMessage.push_back(pNew);
  }
  bool UpdateMessage(const COARmessage &msg, const wxDateTime &t)
  {
    bool bRtn = false;
    COARmessage *pmsg = GetMessageByNumber(msg.GetMessageNumber());
    if(pmsg == NULL)
    {
      AddMessage(msg,t);
      bRtn = true;
    }
    else if((*pmsg) != msg)
    {
      COARmessage *pNew = new COARmessage(*pmsg);
      m_vpMessageEdited.push_back(pNew);
      _InitMapEdited();
      (*pmsg) = msg;
      pmsg->SetTime(t);
      bRtn = true;
    }
    return bRtn;
  }
  void Reserve(size_t nToAdd)
  {
    m_vpMessage.reserve(m_vpMessage.size() + nToAdd);
  }
  const COARmsgExportMap *GetMsgExport() const
  {
    return m_pMsgExport;
  }
  void SetMsgExport(const COARmsgExportMap *p, bool bOverwrite = false) const
  {
    if(bOverwrite || (m_pMsgExport == NULL))
    {
      m_pMsgExport = p;
      // above is mutable
    }
  }
private:
  static void _VectorToSet(const vector<int> *pvnFrom, set<int> *psnTo);
  void __BuildMap() const;
  void _BuildMap() const
  {
    if(m_mapMessage.empty())
    {
      __BuildMap();
    }
  }
  void __BuildMapEdited() const;
  void _BuildMapEdited() const
  {
    if(m_mapMessageEdited.empty() && !m_vpMessageEdited.empty())
    {
      __BuildMapEdited();
    }
  }
  void _InitMapEdited() const
  {
    m_mapMessageEdited.clear();
  }
  void _InitMap() const
  {
    m_mapMessage.clear();
  }
  void _InitMaps() const
  {
    _InitMap();
    _InitMapEdited();
  }

  mutable map<int,COARmessage *> m_mapMessage;
  mutable multimap<int,COARmessage *> m_mapMessageEdited;
  mutable const COARmsgExportMap *m_pMsgExport;

  vector<COARmessage *> m_vpMessage;
  vector<COARmessage *> m_vpMessageEdited;
  static void _CopyOnly(
    vector<COARmessage *> *pvpTo, 
    const vector<COARmessage *> *pvpFrom, 
    const set<int> *psnKeep);

private:
  TnwxXmlIOPersistVector<COARmessage> m_io;
};



#endif
