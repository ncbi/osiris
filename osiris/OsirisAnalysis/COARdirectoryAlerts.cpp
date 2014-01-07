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
*  FileName: COARdirectoryAlerts.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARfile.h"
#include "COARdirectoryAlerts.h"

COARbaseLocus::~COARbaseLocus() {;}

void COARbaseLocus::RegisterAll(bool)
{
  RegisterWxString(_T("BaseLocusName"),&m_sLocusName);
  Register(_T("BaseLocusAlerts"),&m_vnMessages);
}
void COARbaseLocus::AppendMessagesToList(vector<int> *pvn) const
{
  const vector<int> *pvnMsg(GetMessageNumbers());
  pvn->reserve(pvn->size() + pvnMsg->size());
  for(vector<int>::const_iterator itr = pvnMsg->begin();
      itr != pvnMsg->end();
      ++itr)
  {
    pvn->push_back(*itr);
  }
}

COARdirectoryAlerts::~COARdirectoryAlerts()
{
  _ClearLocusMap();
  m_ioBaseLocus.FinalCleanup();
}

const COARbaseLocus *COARdirectoryAlerts::_GetBaseLocusAlertsByLocus(
  const wxString &sLocusName) const
{
  // called from GetBaseLocusAlertsByLocus() ONLY
  wxString s;
  const COARbaseLocus *pRtn = NULL;
  if(m_mapLocus.empty())
  {
    // build map
    vector<COARbaseLocus *>::const_iterator itr;
    for(itr = m_vpBaseLocus.begin();
      itr != m_vpBaseLocus.end();
      ++itr)
    {
      s = (*itr)->GetLocusName();
      s = s.MakeUpper();
      m_mapLocus.insert(
        map<wxString, const COARbaseLocus *>::value_type(s,*itr));
    }
  }

  // search map

  map<wxString, const COARbaseLocus *>::const_iterator itrm;
  s = sLocusName;
  s = s.MakeUpper();
  itrm = m_mapLocus.find(s);
  if(itrm != m_mapLocus.end())
  {
    pRtn = itrm->second;
  }
  return pRtn;
}

void COARdirectoryAlerts::RegisterAll(bool)
{
  Register(_T("MessageNumber"),&COARfile::g_iovn,(void *)&m_vnMessages);
  Register(_T("BaseLocus"),&m_ioBaseLocus,(void *)&m_vpBaseLocus);
}
void COARdirectoryAlerts::AppendMessagesToList(vector<int> *pvn) const
{
  const vector<int> *pvnMsg(GetMessagesNumbers());
  pvn->reserve(pvn->size() + pvnMsg->size());
  for(vector<int>::const_iterator itr = pvnMsg->begin();
      itr != pvnMsg->end();
      ++itr)
  {
    pvn->push_back(*itr);
  }
}

bool COARdirectoryAlerts::Skip(void *pObj)
{
  bool bRtn = false;
  if(pObj == NULL || pObj == (void *)this)
  {
    bRtn = !HasAlerts();
  }
  else
  {
    COARdirectoryAlerts *p = (COARdirectoryAlerts *)pObj;
    bRtn = !p->HasAlerts();
  }
  return bRtn;
}
bool COARdirectoryAlerts::HasAlerts() const
{
  bool bRtn = 
    !(m_vpBaseLocus.empty() && m_vnMessages.empty());
  return bRtn;
}

size_t COARdirectoryAlerts::BuildMessageList(
  COARmessages *pMsgs, 
  vector<wxString> *pvsLocus,
  const COARmessages &AllMessages
  ) const
{
  wxString s;
  vector<COARbaseLocus *>::const_iterator itr;
  size_t nBefore;
  size_t nDiff;
  size_t i;
  pvsLocus->clear();
  pMsgs->CopyOnly(AllMessages,&m_vnMessages,false);

  nBefore = pMsgs->GetMessageCount();
  for(i = 0; i < nBefore; i++)
  {
    pvsLocus->push_back(s);
  }
  for(itr = m_vpBaseLocus.begin();
    itr != m_vpBaseLocus.end();
    ++itr)
  {
    nBefore = pMsgs->GetMessageCount();
    pMsgs->CopyOnly(
      AllMessages,(*itr)->GetMessageNumbers(),true);
    nDiff = pMsgs->GetMessageCount() - nBefore;
    for(i = 0; i < nDiff; i++)
    {
      pvsLocus->push_back((*itr)->GetLocusName());
    }
  }
  return pMsgs->GetMessageCount();
}

bool COARdirectoryAlerts::IsModified(
  const COARmessages &AllMessages, const wxDateTime *pTime) const
{

  bool bRtn = AllMessages.FindEdited(m_vnMessages,pTime);
  if(!bRtn)
  {
    vector<COARbaseLocus *>::const_iterator itr;
    const vector<int> *pvn;
    for(itr = m_vpBaseLocus.begin();
      itr != m_vpBaseLocus.end();
      ++itr)
    {
      pvn = (*itr)->GetMessageNumbers();
      if(AllMessages.FindEdited(*pvn,pTime))
      {
        bRtn = true;
        break;
      }
    }
  }
  return bRtn;
}

bool COARdirectoryAlerts::FindDisplayed(
  const COARmessages &AllMessages) const
{
  bool bRtn = AllMessages.FindDisplayed(m_vnMessages);
  if(!bRtn)
  {
    vector<COARbaseLocus *>::const_iterator itr;
    const vector<int> *pvn;
    for(itr = m_vpBaseLocus.begin();
      itr != m_vpBaseLocus.end();
      ++itr)
    {
      pvn = (*itr)->GetMessageNumbers();
      if(AllMessages.FindDisplayed(*pvn))
      {
        bRtn = true;
        break;
      }
    }
  }
  return bRtn;
}

bool COARdirectoryAlerts::NeedAttention(
  const COARmessages &AllMessages,
  const CLabSettings &lab, 
  int nReviewCount, int nAcceptCount) const
{
  int nReviewReqd = lab.GetReviewerCountDir();
  int nAcceptReqd = lab.GetAcceptanceCountDir();
  bool bRtn = false;
  if((nReviewReqd > nReviewCount) &&
      IsModified(AllMessages))
  {
    bRtn = true;
  }
  else if((nAcceptReqd > nAcceptCount) &&
      FindDisplayed(AllMessages))
  {
    bRtn = true;
  }
  return bRtn;
}
