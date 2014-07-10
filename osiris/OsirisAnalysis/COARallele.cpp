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
*  FileName: COARallele.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARfile.h"

COARallele::XmlIOintArtifact COARallele::g_ioArtifact;

void COARallele::RegisterAll(bool)
{
  RegisterInt("Id",&m_nID);
  RegisterWxString("Name",&m_sName);
  Register("ArtifactCriticalLevel",
    &g_ioArtifact,(void *) &m_nArtifactLevel);
  RegisterDouble("BPS",&m_dBPS);
  RegisterDouble("RFU",&m_dRFU);
  RegisterDouble("meanbps",&m_dMeanBPS);
  RegisterDouble("PeakArea",&m_dPeakArea);
  RegisterDouble("Time",&m_dTime);
  RegisterDouble("Fit",&m_dFit);
  Register("OffLadder",&COARpeak::IO_OL,&m_sOffLadder);
  RegisterIntNonZero("Count",&m_nCount);
  RegisterBoolSkipFalse("Disabled",&m_bDisabled);
  Register(COARfile::g_sMessageNumber,
    &COARfile::g_iovn,(void *) &m_vnMessageNumber);
  RegisterWxDateTimeNonZero("Update",&m_tUpdate);
  RegisterWxStringNotEmpty("InterLocus",&m_sInterLocus);
}

bool COARallele::HasAlerts(const COARmessages *pmsgs, const wxDateTime *pTime) const
{
  bool bRtn = (!IsDisabled()) && pmsgs->FindDisplayed(m_vnMessageNumber,pTime);
  return bRtn;
}
bool COARallele::HasBeenEdited(const COARmessages *pmsgs, const wxDateTime *pTime) const
{
  bool bRtn = false;
  if(pmsgs->FindEdited(m_vnMessageNumber,pTime))
  { 
    bRtn = true;
  }
  else
  {
    bRtn = (GetUpdateTime().GetTicks() > 0);
  }
  return bRtn;
}

COARallele &COARallele::operator =(const COARallele &x)
{
  COARcopy(m_vnMessageNumber);
  COARcopy(m_sName);
  COARcopy(m_sInterLocus);
  COARcopy(m_sLocusName);
  COARcopy(m_tUpdate);
  COARcopy(m_dBPS);
  COARcopy(m_dRFU);
  COARcopy(m_dPeakArea);
  COARcopy(m_dMeanBPS);
  COARcopy(m_dTime); // time of peak in seconds
  COARcopy(m_dFit);
  COARcopy(m_nCount);   // 2 if tall peak and called homozygous, 0 or 1 otherwise
  COARcopy(m_nID);
  COARcopy(m_nArtifactLevel);
  COARcopy(m_sOffLadder);
  COARcopy(m_bDisabled);
  m_pLocus = NULL;
  return *this;
}

bool COARallele::operator ==(const COARallele &x) const
{
  bool bRtn = true;
  if(m_bDisabled != x.m_bDisabled) { bRtn = false; }
  else if(GetCountBool() != x.GetCountBool()) { bRtn = false;}
  else if(m_sOffLadder != x.m_sOffLadder) { bRtn = false; }
  else if(m_sName != x.m_sName) { bRtn = false; }
  else if(!COARpeak::DoubleEqual(m_dBPS,x.m_dBPS)) { bRtn = false; }
  else if(!COARpeak::DoubleEqual(m_dRFU,x.m_dRFU)) { bRtn = false; }
  else if(!COARpeak::DoubleEqual(m_dPeakArea,x.m_dPeakArea)) 
    { bRtn = false; }
  else if(!COARpeak::DoubleEqual(
    m_dFit,x.m_dFit,COARpeak::FIT_DIGIT_MATCH)) { bRtn = false; }
  else if(!COARpeak::DoubleEqual(m_dTime,x.m_dTime)) { bRtn = false; }
  return bRtn;
}

void COARallele::SetLocus(COARlocus *pLocus)
{
  m_pLocus = pLocus;
  if(pLocus == NULL)
  {
    m_sLocusName.Empty();
  }
  else
  {
    m_sLocusName = pLocus->GetName();
  }
}
