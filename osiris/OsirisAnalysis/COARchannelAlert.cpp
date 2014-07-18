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
*  FileName: COARchannelAlert.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARfile.h"
#include "nwx/vectorptr.h"
#include "nwx/nsstd.h"

void COARartifactAllele::RegisterAll(bool)
{
  RegisterWxString("Name",&m_sName);
  Register("OffLadder",&COARpeak::IO_OL,&m_sOffLadder);
  RegisterDouble("BPS",&m_dBPS);
  RegisterWxString("Locus",&m_sLocus);
  RegisterIntNonZero("Location",&m_nLocation);
  RegisterIntNonZero("Count",&m_nCount);
}


COARchannelAlert::~COARchannelAlert() 
{
  m_ioArtifact.Cleanup();
  return;
}

COARartifact::~COARartifact() 
{
  m_ioAllele.Cleanup();
}

void COARartifact::RegisterAll(bool)
{
  RegisterInt("Id",&m_nID);
  RegisterInt("Level", &m_nLevel);
  RegisterDouble("RFU",&m_dRFU);
  RegisterDouble("meanbps",&m_dMeanBPS);
  RegisterDouble("PeakArea",&m_dPeakArea);
  RegisterDouble("Time",&m_dTime);
  RegisterDoubleM1("Fit",&m_dFit);
  RegisterBoolTrue("AllowPeakEdit",&m_bIsEditable);
  RegisterWxString("Label",&m_sLabel);
  RegisterBoolSkipFalse("Disabled",&m_bDisabled);
  RegisterWxDateTimeNonZero("Update",&m_dtUpdate);
  Register(COARfile::g_sMessageNumber, &COARfile::g_iovn,(void *) &m_vnMessageNumber);
  Register("Allele",&m_ioAllele,&m_vpAllele);
  m_ioAllele.Manage(&m_vpAllele);
}
COARartifact &COARartifact::operator =(const COARartifact &x)
{
  COARcopy(m_sLabel);
  COARcopy(m_dtUpdate);
  COARcopy(m_vnMessageNumber);
  COARcopyVP(COARartifactAllele,m_vpAllele);
  COARcopy(m_dRFU);
  COARcopy(m_dMeanBPS);
  COARcopy(m_dPeakArea);
  COARcopy(m_dTime);
  COARcopy(m_dFit);
  COARcopy(m_pAlleleCurrent);
  COARcopy(m_nID);
  COARcopy(m_nLevel);
  COARcopy(m_bDisabled);
  COARcopy(m_bIsEditable);
  return *this;
}

bool COARartifact::SetLocus(const wxString &s) const
{
  vector<COARartifactAllele *>::const_iterator itr;
  m_pAlleleCurrent = NULL;
  if(!s.IsEmpty())
  {
    for(itr = m_vpAllele.begin();
      itr != m_vpAllele.end();
      ++itr)
    {
      if( ! ((*itr)->GetLocus().CmpNoCase(s)) )
      {
        m_pAlleleCurrent = *itr;
        break;
      }
    }
  }
  return (m_pAlleleCurrent != NULL);
}

void COARchannelAlert::RegisterAll(bool)
{
  RegisterInt("ChannelNr",&m_nChannelNr);
  Register(COARfile::g_sMessageNumber, &COARfile::g_iovn,(void *) &m_vnMessageNumber);  
  Register("Artifact",&m_ioArtifact,&m_vpArtifact);
  Register("OldArtifact",&m_ioArtifact,&m_vpOldArtifact);
  m_ioArtifact.Manage(&m_vpArtifact);
  m_ioArtifact.Manage(&m_vpOldArtifact);
}

COARartifact *COARchannelAlert::GetArtifactByID(int nID)
{
  vector<COARartifact *>::iterator itr;
  COARartifact *pRtn(NULL);
  for(itr = m_vpArtifact.begin();
    (pRtn == NULL) && (itr != m_vpArtifact.end());
    ++itr)
  {
    if((*itr)->GetID() == nID)
    {
      pRtn = *itr;
    }
  }
  return pRtn;
}

bool COARchannelAlert::UpdateArtifact(
  const wxString &sLocus, IOARpeak *pArt)
{
  bool bRtn = false;
  COARartifact *pCurrent = GetArtifactByID(pArt->GetID());
  if(pCurrent == NULL)
  {
    // this ID has never been used as an artifact
    // if it is actually an artifact, add to the list
    if(pArt->IsArtifact() && !pArt->IsDisabled())
    {
      COARartifact *pNew = new COARartifact;
      pNew->Set(*pArt);
      pNew->SetUpdateTimeCurrent();
      pNew->AddNewAllele(sLocus);
      pNew->SetMeanBPS(pArt->GetBPS());
      m_vpArtifact.push_back(pNew);
      bRtn = true;
    }
  }
  else if( !COARpeak::Equal(*pCurrent,*pArt,false,true) )
  {
    COARartifact *pNew = new COARartifact(*pCurrent);
    m_vpOldArtifact.push_back(pNew);
    pCurrent->Set(*pArt);
    pCurrent->SetUpdateTimeCurrent();
    bRtn = true;
  }
  return bRtn;
}


void COARchannelAlert::GetArtifactsByTime(
  const wxString &sLocus, // empty if for any locus
  vector<const COARartifact *> *pva, 
  const wxDateTime *pTime) const
{
  size_t ndx1;
  size_t i;
  size_t nCount;
  map<int,const COARartifact *> mapA;
  map<int,const COARartifact *>::iterator itrA;
  const vector<COARartifact *> *ppva;
  const COARartifact *pA;
  bool bDone;
  const vector<COARartifact *> *apva[] =
  {
    &m_vpArtifact,
    &m_vpOldArtifact
  };
  size_t nArrayCount = (pTime == NULL) ? 1 : 2;
  // if pTime is NULL, then ignore old (edited) artifacts
  for(ndx1 = 0; ndx1 < nArrayCount; ++ndx1)
  {
    ppva = apva[ndx1];
    nCount = ppva->size();
    for(i = 0; i < nCount; i++)
    {
      pA = ppva->at(i);
      const wxDateTime &dt(pA->GetUpdate());
      if( ((pTime == NULL) || (*pTime >= dt))
        && 
        (sLocus.IsEmpty() || pA->SetLocus(sLocus))
        )
      {
        // time for this artifact is OK
        //  AND locus was found

        bDone = false;
        itrA = mapA.find(pA->GetID());
        if(itrA != mapA.end())
        {
          if(itrA->second->GetUpdate() < dt)
          {
            // time for this artifact is newer than
            // the one stored in map
            mapA.erase(itrA);
          }
          else
          {
            // time for artifact in map is newer than this,
            // so ignore this one by setting bDone = true
            bDone = true;
          }
        }
        if(!bDone)
        {
          mapA.insert(map<int,const COARartifact *>::value_type(
            pA->GetID(),pA ));
        }
      }
    }
  }
  vectorptr<const COARartifact>::cleanup(pva);
  pva->reserve(mapA.size());
  for(itrA = mapA.begin();
    itrA != mapA.end();
    ++itrA)
  {
    pva->push_back(itrA->second);
  }
}

bool COARchannelAlert::LoadFromNode(wxXmlNode *p, void *pObj)
{
  bool b = nwxXmlPersist::LoadFromNode(p,pObj);
  _CleanupOldArtifact(); // bug fix
  return b;
}
bool COARchannelAlert::LoadFromNode(wxXmlNode *p)
{
  bool b = nwxXmlPersist::LoadFromNode(p);
  _CleanupOldArtifact(); // bug fix
  return b;
}
