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
*  FileName: COARmessage.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARfile.h"

void COARmessage::RegisterAll(bool)
{
  RegisterInt(COARfile::g_sMessageNumber,&m_nMessageNumber);
  RegisterIntNonZero("Index",&m_nIndex);// message # in OsirisMessagesBook.xml
  RegisterWxString("Text",&m_sText);
  RegisterBoolSkipFalse("Hidden",&m_bHidden);
  RegisterWxDateTimeNonZero("LastUpdate",&m_tLastUpdate);
  RegisterBoolTrueSkip("Critical",&m_bCritical);
  RegisterBoolTrueSkip("Enabled",&m_bEnabled);
  RegisterBoolTrueSkip("Editable",&m_bEditable);
  RegisterBoolSkipFalse("DisplayExportInfo",&m_bDisplayExportInfo);
  RegisterBoolTrueSkip("DisplayOsirisInfo",&m_bDisplayOsiris);
  RegisterWxStringNotEmpty("MsgName",&m_sMsgName);
}

//  class COARmessages

typedef multimap<int,COARmessage *>::const_iterator MITR;

void COARmessages::KeepOnly(const set<int> *psn)
{
  vector<COARmessage *> vpMsgKill;
  vector<COARmessage *> vpMsg;
  vector<COARmessage *> vpMsgEdit;

  vector<COARmessage *> *pvpMsg;
  vector<COARmessage *> *pvpMsgIn;
  COARmessage *pMsg;
  int n;

  vpMsgKill.reserve(m_vpMessage.size() + m_vpMessageEdited.size());
  vpMsg.reserve(psn->size());
  vpMsgEdit.reserve(m_vpMessageEdited.size());
  pvpMsg = &vpMsg;   // receive messages where number is in psn
  pvpMsgIn = &m_vpMessage; // original message list
  for(size_t i = 0; i < 2; i++)
  {
    for(vector<COARmessage *>::iterator itr = pvpMsgIn->begin();
      itr != pvpMsgIn->end();
      ++itr)
    {
      pMsg = *itr;
      n = pMsg->GetMessageNumber();
      if(psn->find(n) == psn->end())
      {
        vpMsgKill.push_back(pMsg);
      }
      else
      {
        pvpMsg->push_back(pMsg);
      }
    }
    pvpMsg = &vpMsgEdit; // received edited messages in 2nd loop
    pvpMsgIn = &m_vpMessageEdited; // originial edited messages
  }
  if(vpMsgKill.size() > 0)
  {
    // changes have been made, cleanup
    vectorptr<COARmessage>::cleanup(&vpMsgKill);
    m_vpMessage = vpMsg;
    m_vpMessageEdited = vpMsgEdit;
    _InitMaps();
  }
}

void COARmessages::_CopyOnly(
  vector<COARmessage *> *pvpTo,
  const vector<COARmessage *> *pvpFrom, 
  const set<int> *psnKeep)
{
  int n;
  pvpTo->reserve(psnKeep->size() + pvpTo->size());
  for( vector<COARmessage *>::const_iterator itr = pvpFrom->begin();
    itr != pvpFrom->end();
    ++itr)
  {
    n = (*itr)->GetMessageNumber();
    if(psnKeep->find(n) != psnKeep->end())
    {
      COARmessage *pFrom = *itr;
      COARmessage *pmsg = new COARmessage(*pFrom);
      pvpTo->push_back(pmsg);
    }
  }
}

void COARmessages::CopyOnly(
  const COARmessages &x, const set<int> *psn, bool bAppend)
{
  if(!bAppend)
  {
    Cleanup();
  }
  _CopyOnly(&m_vpMessage,       &x.m_vpMessage,       psn);
  _CopyOnly(&m_vpMessageEdited, &x.m_vpMessageEdited, psn);
  _InitMaps();
  // SEE NOTES IN operator= in .h file regarding m_pMsgExport
  SetMsgExport(x.GetMsgExport(), false);
}

void COARmessages::_VectorToSet(const vector<int> *pvnFrom, set<int> *psnTo)
{
  for(vector<int>::const_iterator itr = pvnFrom->begin();
    itr != pvnFrom->end();
    ++itr)
  {
    psnTo->insert(*itr);
  }
}

void COARmessages::KeepOnly(const vector<int> *pvn)
{
  set<int> sn;
  _VectorToSet(pvn,&sn);
  KeepOnly(&sn);
}
void COARmessages::CopyOnly(
  const COARmessages &x, 
  const vector<int> *pvn,
  bool bAppend)
{
  set<int> sn;
  _VectorToSet(pvn,&sn);
  CopyOnly(x,&sn,bAppend);
}
void COARmessages::RegisterAll(bool)
{
  Register("Message",&m_io,&m_vpMessage);
  Register("OldMessage",&m_io,&m_vpMessageEdited);
}
bool COARmessages::IsModified(const COARmessages *pm1, const COARmessages *pm2)
{
  size_t nCount = pm1->GetMessageCount();
  size_t nCount2 = pm2->GetMessageCount();
  size_t i;
  const COARmessage *pMsg1;
  const COARmessage *pMsg2;
  bool bRtn = false;
  if(nCount > nCount2)
  {
    const COARmessages *tmp = pm1;
    pm1 = pm2;
    pm2 = tmp;
    nCount = nCount2;
  }
  for(i = 0; i < nCount; i++)
  {
    pMsg1 = pm1->GetMessage(i);
    pMsg2 = pm2->GetMessageByNumber(pMsg1->GetMessageNumber());
    if( (pMsg2 != NULL) && pMsg2->IsDifferent(*pMsg1) )
    {
      bRtn = true;
      i = nCount;
    }
  }
  return bRtn;
}
#if 0
bool COARmessages::IsModified(const COARmessages &msgs) const
{
  //  return true if any messages in 'this' are not found
  //  in msgs unmodified.  'this' will usually be a subset of msgs
  //  being edited for a particular sample, locus, etc.

  size_t nCount = GetMessageCount();
  size_t i;
  const COARmessage *pMsgOrig;
  const COARmessage *pMsg;
  bool bRtn = false;
  for(i = 0; i < nCount; i++)
  {
    pMsg = GetMessage(i);
    pMsgOrig = msgs.GetMessageByNumber(pMsg->GetMessageNumber());
    if( (pMsgOrig == NULL) || pMsg->IsDifferent(*pMsgOrig) )
    {
      bRtn = true;
      i = nCount;
    }
  }
  return bRtn;
}
#endif
size_t COARmessages::CountNotHidden(const vector<int> &vn, const wxDateTime *pTime) const
{
  size_t nRtn = 0;
  int n;
  for(vector<int>::const_iterator itr = vn.begin();
    itr != vn.end();
    ++itr)
  {
    n = *itr;
    if(!IsHidden(n,pTime))
    {
      nRtn++;
    }
  }
  return nRtn;
}

bool COARmessages::FindNotHidden(const vector<int> &vn, const wxDateTime *pTime) const
{
  bool bRtn = false;
  int n;
  for(vector<int>::const_iterator itr = vn.begin();
    (itr != vn.end()) && !bRtn;
    ++itr)
  {
    n = *itr;
    bRtn = !IsHidden(n,pTime);
  }
  return bRtn;
}
bool COARmessages::FindDisplayed(const vector<int> &vn, const wxDateTime *pTime) const
{
  bool bRtn = false;
  int n;
  for(vector<int>::const_iterator itr = vn.begin();
    (itr != vn.end()) && !bRtn;
    ++itr)
  {
    n = *itr;
    bRtn = IsDisplayed(n,pTime);
  }
  return bRtn;
}
bool COARmessages::FindEdited(const vector<int> &vn,const wxDateTime *pTime) const
{
  bool bRtn = false;
  int n;
  for(vector<int>::const_iterator itr = vn.begin();
    (itr != vn.end()) && !bRtn;
    ++itr)
  {
    n = *itr;
    bRtn = IsMessageEdited(n,pTime);
  }
  return bRtn;
}

void COARmessages::__BuildMap() const
{
  const vector<COARmessage *> *pvec;
  map<int,COARmessage *> *pmap;
  pvec = &m_vpMessage;
  pmap = &m_mapMessage;
  pmap->clear();
  for(vector<COARmessage *>::const_iterator itr = pvec->begin();
    itr != pvec->end();
    ++itr)
  {
    COARmessage *pm(*itr);
    pmap->insert(
      map<int,COARmessage *>::value_type(pm->GetMessageNumber(),pm));
  }
};

void COARmessages::__BuildMapEdited() const
{
  m_mapMessageEdited.clear();
  for(vector<COARmessage *>::const_iterator itr = m_vpMessageEdited.begin();
    itr != m_vpMessageEdited.end();
    ++itr)
  {
    COARmessage *pm(*itr);
    m_mapMessageEdited.insert(
      map<int,COARmessage *>::value_type(pm->GetMessageNumber(),pm));
  }
}
const COARmessage *COARmessages::GetMessageByNumber(int n) const
{
  const COARmessage *pRtn(NULL);
  _BuildMap();
  map<int,COARmessage *>::const_iterator itr =
    m_mapMessage.find(n);
  if(itr != m_mapMessage.end())
  {
    pRtn = itr->second;
  }
  return pRtn;
}
const COARmessage *COARmessages::GetMessageByNumber(int n, const wxDateTime *pTime) const
{
  // get the 'newest' message by number (n)
  // where update time <= pTime

  // the time original message for each number is unspecified
  // therefore it will be used even if *pTime is less than
  // the time the original file was created...

  const COARmessage *pRtn(GetMessageByNumber(n));
  if( (pTime == NULL) || (pRtn == NULL) ) {;} // no time specified or no message found, we are done
  else if( pRtn->GetTime() > *pTime )
  {
    // no message yet or message is too new
    const COARmessage *pMsg;
    pRtn = NULL;
    __BuildMapEdited();
    pair<MITR,MITR> itrs = m_mapMessageEdited.equal_range(n);
    MITR itr;
    for(itr = itrs.first; itr != itrs.second; ++itr)
    {
      pMsg = itr->second;
      const wxDateTime &dt(pMsg->GetTime());
      if(dt > *pTime) {;} // skip this one
      else if( (pRtn == NULL) || (dt > pRtn->GetTime()) )
      {
        // this one is newer and still <= *pTime
        pRtn = pMsg;
      }
    }
  }
  return pRtn;
}
bool COARmessages::IsMessageEdited(int n, const wxDateTime *pTime) const
{
  bool bRtn = false;
  _BuildMapEdited();
  pair<MITR,MITR> itrs = m_mapMessageEdited.equal_range(n);
  if(pTime == NULL)
  {
    bRtn = (itrs.first != itrs.second);
  }
  else
  {
    const COARmessage *pMsg;
    for(MITR itr = itrs.first; (itr != itrs.second) && (!bRtn); ++itr)
    {
      pMsg = itr->second;
      const wxDateTime &dt(pMsg->GetTime());
      if((dt.GetTicks() > 0) && (dt < *pTime))
      {
        bRtn = true;
      }
    }
  }
  return bRtn;
}
