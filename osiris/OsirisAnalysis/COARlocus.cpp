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
*  FileName: COARlocus.cpp
*  Author:   Douglas Hoffman
*
*/

#include "COARfile.h"
#include "LABEL_TYPES.h"

//  COARlocus



void COARlocus::RegisterAll(bool)
{
  RegisterWxString("LocusName",&m_sLocusName);
  Register("LocusAlerts",&m_vnLocusAlerts);
  Register("Allele",&m_ioallele,(void *) &m_pvAllele);
  Register("OldAllele",&m_ioallele,(void *) &m_pvAlleleEdited);
  Register(&m_sNotes);
  Register("OldNotes",&m_pvOldNotes);
  Register("ReviewList",&m_ReviewList);
  Register("AcceptanceList",&m_AcceptanceList);
}

wxString COARlocus::_GetCellAlleles(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  bool bAmel = IsAmel();
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatName(bAmel,true);
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
wxString COARlocus::_GetCellBPS(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatBPS(true);
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
wxString COARlocus::_GetCellRFU(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatRFU();
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
wxString COARlocus::_GetCellTime(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatTime();
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
wxString COARlocus::_GetCellPeakArea(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatPeakArea();
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
wxString COARlocus::_GetCellILS_BPS(const wxDateTime *pTime) const
{
  wxString sRtn;
  wxString s;
  vector<const COARallele *> va;
  GetAllelesByTime(&va,pTime);
  for(vector<const COARallele *>::iterator itr = va.begin();
    itr != va.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      s = (*itr)->FormatILS_BPS(true);
      COARfile::AppendCellString(&sRtn,s,(*itr)->GetCount());
    }
  }
  return sRtn;
}
size_t COARlocus::EnabledAlleleCount() const
{
  size_t nRtn = 0;
  for(vector<COARallele *>::const_iterator itr = m_pvAllele.begin();
    itr != m_pvAllele.end();
    ++itr)
  {
    if(!(*itr)->IsDisabled())
    {
      nRtn++;
    }
  }
  return nRtn;
}

void COARlocus::GetAllelesByTime(vector<const COARallele *> *pva, const wxDateTime *pTime) const
{
  set<COARallele *,COARalleleLessByTime> setA;
  set<COARallele *,COARalleleLessByTime>::iterator itrSet;
  if(pTime == NULL)
  {
    //  get the most recently updated alleles ASAP

    for(vector<COARallele *>::const_iterator itr = m_pvAllele.begin();
      itr != m_pvAllele.end();
      ++itr)
    {
      setA.insert(*itr);
    }
  }
  else
  {
    const vector<COARallele *> *pvec[2] = { &m_pvAllele, &m_pvAlleleEdited };
    for(size_t ndx = 0; ndx < 2; ++ndx)
    {
      const vector<COARallele *> *pva = pvec[ndx];
    
      for(vector<COARallele *>::const_iterator itr = pva->begin();
        itr != pva->end();
        ++itr)
      {
        const wxDateTime &dt((*itr)->GetUpdateTime());
        if(dt <= *pTime) // does the update time qualify
        {
          itrSet = setA.find(*itr);
          if(itrSet == setA.end())
          {
            // setA does not contain an allele for this 'time'
            setA.insert(*itr);
          }
          else if( (*itrSet)->GetUpdateTime() < dt ) 
          {
            // we have a more recently updated allele, replace in set
            setA.erase(itrSet);
            setA.insert(*itr);
          }
        }
      }
    }
  }
  pva->clear();
  pva->reserve(setA.size());
  for(itrSet = setA.begin();
      itrSet != setA.end();
      ++itrSet)
  {
    pva->push_back(*itrSet);
  }
}



wxString COARlocus::GetCell(int nLabelType, const wxDateTime *pTime) const
{
  wxString sRtn;
  switch(nLabelType)
  {
  case (int)LABEL_CELL_ALLELE:
    sRtn = _GetCellAlleles(pTime);
    break;
  case (int)LABEL_CELL_BPS:
    sRtn = _GetCellBPS(pTime);
    break;
  case (int)LABEL_CELL_RFU:
    sRtn = _GetCellRFU(pTime);
    break;
  case (int)LABEL_CELL_TIME:
    sRtn = _GetCellTime(pTime);
    break;
  case (int)LABEL_CELL_PEAK_AREA:
    sRtn = _GetCellPeakArea(pTime);
    break;
  case (int)LABEL_CELL_ILS_BPS:
    sRtn = _GetCellILS_BPS(pTime);
    break;
  default:
    break;
  }
  return sRtn;
}

int COARlocus::_FindAlleleByID(int nID) const
{
  size_t nAlleleCount = AlleleCount();
  size_t i;
  int nRtn = -1;
  const COARallele *p;
  // nAlleleCount should be small, <= 4, 
  // so this is hopefully fast enough
  for(i = 0; i < nAlleleCount; i++)
  {
    p = GetAllele(i);
    if(p->GetID() == nID)
    {
      nRtn = (int)i;
      i = nAlleleCount; // loop exit
    }
  }
  return nRtn;
}
const COARallele *COARlocus::GetAlleleByID(int nID) const
{
  int nRtn = _FindAlleleByID(nID);
  const COARallele *pRtn((nRtn < 0) ? NULL : GetAllele(nRtn));
  return pRtn;
}
COARallele *COARlocus::GetAlleleByID(int nID)
{
  int nRtn = _FindAlleleByID(nID);
  COARallele *pRtn((nRtn < 0) ? NULL : GetAllele(nRtn));
  return pRtn;
}

bool COARlocus::UpdateAllele(IOARpeak *pAllele)
{
  COARallele *pCurrent = GetAlleleByID(pAllele->GetID());
  bool bRtn = false; // return true is a change was made
  if(pCurrent == NULL)
  {
    // this ID has never been used as an allele
    // if it is actually an allele, then add it to the list
    if(pAllele->IsAllele() && !pAllele->IsDisabled())
    {
      COARallele *pNew = new COARallele;
      pNew->Set(*pAllele);
      pNew->SetUpdateTimeCurrent();
      pNew->SetLocus(this);
      m_pvAllele.push_back(pNew);
      bRtn = true; // a change was made
    }
  }
  else if( !COARpeak::Equal(*pCurrent,*pAllele,true,false) )
  {
    COARallele *pNew = new COARallele(*pCurrent);
    pNew->SetLocus(this);
    m_pvAlleleEdited.push_back(pNew);
    pCurrent->Set(*pAllele);
    pCurrent->SetUpdateTimeCurrent();
    bRtn = true;
  }
  return bRtn;
}
wxString COARlocus::FormatReviewAcceptance(
  const wxDateTime *pTime) const
{
  wxString sRtn = COARreviewList::FormatReviewAcceptance(
    m_AcceptanceList,
    m_ReviewList,
    pTime);
  return sRtn;
}
bool COARlocus::UpdateNotes(const COARnotes &x, const wxDateTime &t)
{
  bool bRtn = m_sNotes.Update(x,m_pvOldNotes.Get(),t);
  return bRtn;
}
bool COARlocus::Update(const COARlocus &Locus, const wxDateTime &t)
{
  bool bRtn = false;
  size_t nAlleleCount = AlleleCount();
  if(nAlleleCount == Locus.AlleleCount())
  {
    const COARallele *pAlleleMod;
    COARallele *pAllele;
    if(m_sNotes.Update(Locus.m_sNotes,m_pvOldNotes.Get(),t))
    {
      bRtn = true;
    }
    for(size_t i = 0; i < nAlleleCount; i++)
    {
      pAllele = GetAllele(i);
      pAlleleMod = Locus.GetAllele(i);
      if((*pAllele) != (*pAlleleMod))
      {
        COARallele *pNew = new COARallele(*pAllele);
        pNew->SetLocus(this);
        m_pvAlleleEdited.push_back(pNew);
        (*pAllele) = (*pAlleleMod);
        pAllele->SetLocus(this);
        pAllele->SetUpdateTime(t);
        bRtn = true;
      }
    }
  }
  return bRtn;
}

void COARlocus::AppendAlerts(vector<int> *pvn, const wxDateTime *pTime) const
{
  size_t nTotalSize = m_vnLocusAlerts.Get()->size();
  size_t nVsize = 0;
  vector<const COARallele *> va;
  vector<const COARallele *>::const_iterator itr;
  const COARallele *pa;
  GetAllelesByTime(&va,pTime);

  for( itr = va.begin(); itr != va.end(); ++itr)
  {
    pa = *itr;
    if(!pa->IsDisabled())
    {
      nVsize += pa->GetAlerts().size();
    }
  }
  nTotalSize += nVsize;
  pvn->reserve(pvn->size() + nTotalSize);
  m_vnLocusAlerts.AppendToList(pvn);
  if(nVsize)
  {
    // probably zero

    for( itr = va.begin(); itr != va.end(); ++itr)
    {
      pa = *itr;
      if(!pa->IsDisabled())
      {
        const vector<int> &vn = pa->GetAlerts();
        for(vector<int>::const_iterator itrn = vn.begin();
          itrn != vn.end();
          ++itrn)
        {
          pvn->push_back(*itrn);
        }
      }
    }
  }
}

size_t COARlocus::AlertCount(const COARmessages *pmsgs, const wxDateTime *pTime, bool bStopAtOne) const
{
  size_t nRtn = pmsgs->CountDisplayed(m_vnLocusAlerts,pTime,bStopAtOne);
  if(!(bStopAtOne && nRtn))
  {
    // try alleles
    vector<const COARallele *> vpa;
    const COARallele *pa;
    GetAllelesByTime(&vpa, pTime);
    size_t n = vpa.size();
    for(size_t i = 0; i < n; ++i)
    {
      pa = vpa.at(i);
      if(!pa->IsDisabled())
      {
        const vector<int> &msgs(pa->GetMessages());
        nRtn += pmsgs->CountDisplayed(msgs,pTime,bStopAtOne);
        if(bStopAtOne && nRtn)
        {
          i = n;
        }
      }
    }
  }
  return nRtn;
}
const COARmessages *COARlocus::GetMessages() const
{
  const COARmessages *pmsgs =
    (m_pSample == NULL) ? NULL : m_pSample->GetMessages();
  return pmsgs;
}

size_t COARlocus::CountAlerts(const COARmessages *pmsgs, const wxDateTime *pTime) const
{
  vector<int> vn;
  AppendAlerts(&vn,pTime);
  size_t nRtn = pmsgs->CountNotHidden(vn,pTime);
  return nRtn;
}

bool COARlocus::HasBeenEdited(
  const COARmessages *pmsgs,
  const COARsample *pSample,
  int nChannel,
  const wxDateTime *pTime) const
{
  // check if notes have been edited

  const COARnotes *pNotes = GetNotesPtr(pTime);
  //const COARmessage *pMsg;
  bool bRtn = false;
  if(pNotes != NULL)
  {
    const wxDateTime dt(pNotes->GetTime());
    bRtn = (dt.GetTicks() != 0);
  }
  if(!bRtn)
  {
    const vector<int> *pvnMsgs = m_vnLocusAlerts.Get();
    bRtn = pmsgs->FindEdited(*pvnMsgs,pTime);
  }
  if(!bRtn)
  {
    // check alleles

    vector<int> vn;
    vector<const COARallele *> vpa;
    GetAllelesByTime(&vpa,pTime);
    for(vector<const COARallele *>::iterator itra = vpa.begin();
      (itra != vpa.end()) && !bRtn;
      ++itra)
    {
      if((*itra)->HasBeenEdited(pmsgs,pTime))
      {
        bRtn = true;
      }
    }
  }
  if(!bRtn)
  {
    // now check artifacts

    vector<const COARartifact *> v;
    pSample->GetArtifactsByLocus(
      &v,m_sLocusName,nChannel,pTime);
    vector<const COARartifact *>::iterator itr;
    for(itr = v.begin(); itr != v.end(); ++itr)
    {
      if((*itr)->HasBeenModified())
      {
        bRtn = true;
        break;
      }
    }
  }
  return bRtn;
}

bool COARlocus::operator ==(const COARlocus &x) const
{
  bool bRtn = true;
  if(m_pvAllele.size() != x.m_pvAllele.size())
  {
    bRtn = false;
  }
  else if(m_pvAlleleEdited.size() != x.m_pvAlleleEdited.size())
  {
    bRtn = false;
  }
  else if(m_sLocusName != x.m_sLocusName)
  {
    bRtn = false;
  }
  else if(m_sNotes != x.m_sNotes)
  {
    bRtn = false;
  }
  else if(!EqualAlleles(&m_pvAllele,&x.m_pvAllele))
  {
    bRtn = false;
  }
  else if(!EqualAlleles(&m_pvAlleleEdited,&x.m_pvAlleleEdited))
  {
    bRtn = false;
  }
  else if(m_ReviewList != x.m_ReviewList)
  {
    bRtn = false;
  }
  else if(m_AcceptanceList != x.m_AcceptanceList)
  {
    bRtn = false;
  }
  return bRtn;
}

bool COARlocus::EqualAlleles(
    const vector<COARallele *> *pv1,
    const vector<COARallele *> *pv2)
{
  size_t nSize = pv1->size();
  bool bRtn = (nSize == pv2->size());
  if(bRtn)
  {
    const COARallele *pa1;
    const COARallele *pa2;
    for(size_t i = 0; i < nSize; ++i)
    {
      pa1 = pv1->at(i);
      pa2 = pv2->at(i);
      if((*pa1) != (*pa2))
      {
        bRtn = false;
        i = nSize; // loop exit
      }
    }
  }
  return bRtn;
}

void COARlocus::PostProcessFile(COARsample *pSample)
{
  vector<COARallele *>::iterator itr;
  if(pSample != NULL)
  {
    m_pSample = pSample;
  }
  for(itr = m_pvAllele.begin();
    itr != m_pvAllele.end();
    ++itr)
  {
    (*itr)->SetLocus(this);
  }
  for(itr = m_pvAlleleEdited.begin();
    itr != m_pvAlleleEdited.end();
    ++itr)
  {
    (*itr)->SetLocus(this);
  }
}


int COARlocus::GetChannelNr() const
{
  int nRtn = 0;
  if(m_pSample != NULL)
  {
    nRtn = m_pSample->GetChannelNrFromLocus(GetName());
  }
  return nRtn;
}
void COARlocus::SetIsModified(bool b)
{
  if(m_pSample != NULL)
  {
    m_pSample->SetIsModified(b);
  }
}
void COARlocus::UpdateVersion()
{
  if(m_pSample != NULL)
  {
    m_pSample->UpdateVersion();
  }
}
