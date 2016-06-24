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
*  FileName: COARsample.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "COARfile.h"
#include "nwx/stdb.h"
#include <set>
#include <map>
#include <memory>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxString.h"
void COARinterlocusAlert::RegisterAll(bool)
{
  Register(COARfile::g_sLocusName, &COARfile::g_ioLocusName,(void *) &m_vsLocusName);
  Register(COARfile::g_sMessageNumber, &COARfile::g_iovn,(void *) &m_vnMessageNumber);
}
bool COARsampleReviewAccept::Skip(void *pObj)
{
  return ((COARsampleReviewAccept *)pObj)->Skip();
}
bool COARsampleReviewAccept::Skip()
{
  bool bRtn = true;
  if(!m_SampleList.Skip(&m_SampleList)) 
  {
    bRtn = false;
  }
  else if(!m_ChannelList.Skip(&m_ChannelList))
  {
    bRtn = false;
  }
  else if(!m_ILSList.Skip(&m_ILSList))
  {
    bRtn = false;
  }
  return bRtn;
}
void COARsampleReviewAccept::RegisterAll(bool)
{
  const wxString &s(m_SampleList.GetNodeName());
  wxString sSample("Sample");
  wxString sChannel("Channel");
  wxString sILS("ILS");

  sSample.Append(s);
  sChannel.Append(s);
  sILS.Append(s);

  Register(sSample,&m_SampleList);
  Register(sChannel,&m_ChannelList);
  Register(sILS,&m_ILSList);
}
COARsample::~COARsample()
{
  LocalInit();
}

const wxString COARsample::GetFullFileName() const
{
  wxString sName = m_sName;
  wxString sExt(m_pFile->GetFileExt(true));
  size_t nLen = sExt.Len();
  FIX_FILE_NAME(sName);
  if( (sName.Len() < nLen) ||
    (sName.Right(nLen).CmpNoCase(sExt) != 0) )
  {
    sName.Append(sExt);
  }
  return sName;
}

const wxString COARsample::GetName() const
{
  wxString sName = GetFileName();
  wxString sExt(m_pFile->GetFileExt(true));
  size_t nLenExt = sExt.Len();
  size_t nLen = sName.Len();
  if( (nLen > nLenExt) && 
      !sName.Right(nLenExt).CmpNoCase(sExt)
    )
  {
    sName.Truncate(nLen - nLenExt);
  }
  return sName;
}

void COARsample::LocalInit()
{
  m_pFile = NULL;
  m_spLocus.clear();
  vectorptr<COARlocus>::cleanup(&m_vpLocus);
  vectorptr<COARnotes>::cleanup(&m_vpOldNotesSample);
  vectorptr<COARnotes>::cleanup(&m_vpOldNotesChannel);
  vectorptr<COARnotes>::cleanup(&m_vpOldNotesILS);
  m_mapIDchannel.clear();
}


void COARsample::RegisterAll(bool)
{
  RegisterWxString("Name",&m_sName);
  RegisterWxStringNotEmpty("SampleName",&m_sSampleName);
  RegisterWxStringNotEmpty("RunStart",&m_sRunStart);
  Register("EnableHistory",&m_EnableHistory);
  RegisterWxString("Type",&m_sType);
  Register("SampleAlerts",&m_alertSample);
  Register("ILSAlerts",&m_alertILS);
  Register("ChannelAlerts",&m_pvChannelAlerts);
  Register("Locus",&m_ioLocus,(void *)&m_vpLocus);
  Register("InterlocusAlerts",&m_pvAlertInterLocus);
  RegisterWxStringNotEmpty("PositiveControl",&m_sPositiveControl);
  Register("SampleNotes",&m_NotesSample);
  Register("ChannelNotes",&m_NotesChannel);
  Register("ILSNotes",&m_NotesILS);
  Register("OldNotes",&m_OldNotes);
  Register("Reviews",&m_Reviews);
  Register("Acceptance",&m_Acceptance);
  m_OldNotes.SetSample(this);
}
bool COARsample::COARoldNotes::Skip(void *)
{
  return
    SkipArray(&m_p->m_vpOldNotesSample) &&
    SkipArray(&m_p->m_vpOldNotesChannel) &&
    SkipArray(&m_p->m_vpOldNotesILS)
    ;
}
bool COARsample::COARoldNotes::SkipArray(vector<COARnotes *> *pv)
{
  bool bRtn = true;
  COARnotes *pn;
  for(vector<COARnotes *>::iterator itr = pv->begin();
    bRtn && (itr != pv->end());
    ++itr)
  {
    pn = *itr;
    if(!pn->Skip((void *)pn))
    {
      bRtn = false;
    }
  }
  return bRtn;
}
TnwxXmlIOPersistVector<COARnotes> COARsample::COARoldNotes::g_ioNotes(false);

void COARsample::COARoldNotes::RegisterAll(bool)
{
  Register("SampleNotes", &g_ioNotes,(void *) &m_p->m_vpOldNotesSample);
  Register("ChannelNotes", &g_ioNotes,(void *) &m_p->m_vpOldNotesChannel);
  Register("ILSNotes", &g_ioNotes,(void *) &m_p->m_vpOldNotesILS);
}
size_t COARsample::CountAlerts(const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  COARlocus *pLocus;
  size_t nRtn = 0;
  nRtn += pMsgs->CountNotHidden(m_alertILS,pTime);
  nRtn += pMsgs->CountNotHidden(m_alertSample,pTime);
  nRtn += CountChannelAlert(pMsgs,pTime);
  for(vector<COARlocus *>::const_iterator itr = m_vpLocus.begin();
    itr != m_vpLocus.end();
    ++itr)
  {
    pLocus = *itr;
    nRtn += pLocus->CountAlerts(pMsgs,pTime);
  }
  return nRtn;
}
bool COARsample::HasAnyAlerts(const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  // NOTE: m_pvAlertInterLocus is currently not used
  bool bRtn = false;
  if(HasILSAlert(pMsgs,pTime))
  {
    bRtn = true;
  }
  else if(HasSampleAlert(pMsgs,pTime))
  {
    bRtn = true;
  }
  else if(HasChannelAlert(pMsgs,pTime))
  {
    bRtn = true;
  }
  else
  {
    COARlocus *pLocus;
    for(vector<COARlocus *>::const_iterator itr = m_vpLocus.begin();
      (itr != m_vpLocus.end()) && !bRtn;
      ++itr)
    {
      pLocus = *itr;
      bRtn = pLocus->HasAlerts(pMsgs,pTime);
    }
  }
  return bRtn;
}
bool COARsample::IsEdited(
  const COARmessages *pMsgs,
  const wxDateTime *pTime,
  bool bCheckLocus) const
{
  bool bRtn = false;
  if(IsCellILSEdited(pMsgs,pTime))
  {
    bRtn = true;
  }
  else if(IsCellChannelEdited(pMsgs,pTime))
  {
    bRtn = true;
  }
  else if(IsSampleLevelEdited(pMsgs,pTime))
  {
    bRtn = true;
  }
  else if(bCheckLocus)
  {
    COARlocus *pLocus;
    for(vector<COARlocus *>::const_iterator itr = m_vpLocus.begin();
      (itr != m_vpLocus.end()) && !bRtn;
      ++itr)
    {
      pLocus = *itr;
      bRtn = pLocus->HasBeenEdited(pMsgs,pTime);
    }
  }
  return bRtn;
}
void COARsample::_AddAllelesToMap(
  MAP_ID_PEAK *pmapPeak,
  const COARlocus *pLocus,
  const wxDateTime *pTime,
  int nPeakID) const
{
  vector<const COARallele *> va;
  const COARallele *pAllele;
  COARpeakAny *pPeak;
  MAP_ID_PEAK::iterator itrIDP;
  pLocus->GetAllelesByTime(&va,pTime);
  size_t n = va.size();
  size_t i;
  bool bDone;
  bool bReplace;
  int nID;
  for(i = 0; i < n; i++)
  {
    pAllele = va.at(i);
/////    if(!pAllele->IsDisabled()) 
// removed if() because disabled alleles are needed 
// in history and editing grids and are harmless in the plot
// because they are ignored in plot -- 5/4/09, discard later

    {
      nID = pAllele->GetID();
      if( (nPeakID < 0) || (nPeakID == nID) )
      {
        if(!nID)
        {
          // older OAR version
          nID = _MakeID(pAllele);
        }
        // 6/10/16, if there is an allele match
        //  take the enabled one
        itrIDP = pmapPeak->find(nID);
        bDone = false;
        bReplace = false;
        if(itrIDP == pmapPeak->end()) {} // OK
        else if( !pAllele->IsDisabled() )
        {
          // prefer the enabled allele
#ifdef __WXDEBUG__
          if(!itrIDP->second->IsDisabled())
          {
            wxString s = wxT("multiple enabled alleles for ID ");
            s += nwxString::FormatNumber(nID);
            wxASSERT_MSG(0,s);
          }
#endif
          bReplace = true;
        }
        else if(!itrIDP->second->IsDisabled())
        {
          bDone = true;  // keep it
        }
        else if(pAllele->GetUpdateTime() > itrIDP->second->GetUpdateTime())
        {
          // we have a newer one
          bReplace = true;
        }
        else
        {
          bDone = true;
        }
        if(!bDone)
        {
          if(bReplace)
          {
            delete (itrIDP->second);
            pmapPeak->erase(itrIDP);
          }
          pPeak = new COARpeakAny(*pAllele);
          pmapPeak->insert(MAP_ID_PEAK::value_type(nID,pPeak));
        }
      }
    }
  }
}
bool COARsample::_GetArtifactsByChannel(
  vector<const COARartifact *> *pPeaks, 
  int nChannel,
  const wxString &sLocus, 
  const wxDateTime *pTime,
  int nPeakID) const
{
  const COARchannelAlert *pcha = GetChannelAlertsByChannel(nChannel);
  bool bRtn = false;
  if(pcha != NULL)
  {
    pcha->GetArtifactsByTime(sLocus,pPeaks,pTime,nPeakID);
    bRtn = !pPeaks->empty();
  }
  return bRtn;
}
void COARsample::_AddArtifactsToMap(
  MAP_ID_PEAK *pmapPeak,
  int nChannel,
  const wxString &sLocus,
  const wxDateTime *pTime,
  bool bIncludeDisabled,
  bool bIncludeNotEditable) const
{
  vector<const COARartifact *> vArtifact;
  if(_GetArtifactsByChannel(&vArtifact,nChannel,sLocus,pTime))
  {
    MAP_ID_PEAK::iterator itrMap;
    const COARartifact *pArtifact;
    COARpeakAny *pPeak;
    size_t nSizeA;
    size_t j;
    int nID;

    nSizeA = vArtifact.size();
    for(j = 0; j < nSizeA; ++j)
    {
      pArtifact = vArtifact.at(j);
      if((!bIncludeDisabled) && pArtifact->IsDisabled())
      {} // don't want it
      else if(bIncludeNotEditable || pArtifact->IsEditable())
      {
        pArtifact->SetLocus(sLocus);
        nID = pArtifact->GetID();
        if(!nID)
        {
          nID = _MakeID(pArtifact);
        }
        itrMap = pmapPeak->find(nID);
        if(itrMap == pmapPeak->end())
        {
          pPeak = new COARpeakAny(*pArtifact);
          pmapPeak->insert(map<int,COARpeakAny *>::value_type(
            nID,pPeak));
        }
        else  //  if(!pArtifact->IsDisabled())  
              // commented out condition, 
              //  6/10/16 disabled should be included
        {
          // set up as artifact
          itrMap->second->SetupArtifactInfo(pArtifact);
        }
      }
    }
  }
}

vectorptr<COARpeakAny> *COARsample::_SortMap(MAP_ID_PEAK *pmapPeak)
{
  setCOARpeakAny setPeak;
  setCOARpeakAny::iterator itrSet;
  MAP_ID_PEAK::iterator itrMap;

  // build set of peaks for sorting

  for(itrMap = pmapPeak->begin();
    itrMap != pmapPeak->end();
    ++itrMap)
  {
    setPeak.insert(itrMap->second);
  }

  // now copy set to vector

  auto_ptr< vectorptr<COARpeakAny> > pRtn(new vectorptr<COARpeakAny>);
  pRtn->reserve(setPeak.size());

  for(itrSet = setPeak.begin();
    itrSet != setPeak.end();
    ++itrSet)
  {
    pRtn->push_back(*itrSet);
  }
  return pRtn.release();
}

void COARsample::_MapIDPeakByChannel(
  MAP_ID_PEAK *pMapPeak,
  const COARchannel *pChannel,
  const wxDateTime *pTime,
  bool bIncludeDisabled) const
{
  wxString sLocus;
  size_t nLocus = pChannel->GetLocusCount();
  size_t i;
  int nChannel = pChannel->GetChannelNr();
  const COARlocus *pLocus;

  for(i = 0; i < nLocus; i++)
  {
    sLocus = pChannel->GetLocusName(i);
    pLocus = _FindLocus(sLocus);
    if(pLocus != NULL)
    {
      _AddAllelesToMap(pMapPeak,pLocus,pTime);
    }
  }
  sLocus.Clear();
  _AddArtifactsToMap(pMapPeak,nChannel,sLocus,pTime,bIncludeDisabled);
}

void COARsample::_BuildMapIDchannel() const
{
  if(m_mapIDchannel.empty())
  {
    // build map of ID to locus
    // first find all peak IDs
    vectorptr<COARpeakAny>::iterator itr;
    const COARchannel *pChannel;
    size_t nCount = m_pFile->GetChannelCount();
    size_t i;
    int nID;
    for(i =  0; i < nCount; ++i)
    {
      pChannel = m_pFile->GetChannel(i);
      auto_ptr< vectorptr<COARpeakAny> > pPeaks(
        GetPeaksByChannel(pChannel,NULL,true));
      for(itr = pPeaks->begin(); itr != pPeaks->end(); ++itr)
      {
        nID = (*itr)->GetID();
          m_mapIDchannel.insert(
            map_ID_CHANNEL::value_type(nID,pChannel->GetChannelNr()));
      }
    }
  }
}
COARartifact *COARsample::GetArtifactByID(int nID) const
{
  COARartifact *pRtn = NULL;
  bool bOK = (nID > 0) && m_pFile->CanEditArtifacts(); // && !IsLadderType(); not sure about ladders
  if(bOK)
  {
    vector<const COARartifact *> vArt;
    _BuildMapIDchannel();
    map_ID_CHANNEL::iterator itr =  m_mapIDchannel.find(nID);
    if(itr == m_mapIDchannel.end()) {}
    else if( _GetArtifactsByChannel(&vArt,itr->second,wxEmptyString,NULL,nID))
    {
      vector<const COARartifact *>::iterator itr = vArt.begin();
      pRtn = new COARartifact(*(*itr));
      if(vArt.size() > 1)
      {
        wxString s = nwxString::FormatNumber((int)vArt.size());
        s += wxT(" artifacts found for peak ID: ");
        s += nwxString::FormatNumber(nID);
#ifdef __WXDEBUG__
        wxASSERT_MSG(0,s);
#else
        mainApp::LogMessage(s);
#endif
      }
    }
  }
  return pRtn;
}
size_t COARsample::GetAlleleCountByID(int nID) const
{
  size_t nRtn = 0;
  auto_ptr<COARartifact> p(GetArtifactByID(nID));
  if(p.get() == NULL)
  {
    std::vector<COARpeakAny *> vp;
    nRtn = GetAllelesByID(nID,false,&vp);
  }
  else
  {
    nRtn = p.get()->GetNumberOfAlleles();
  }
  return nRtn;
}

size_t COARsample::GetAllelesByID(int nID, bool bInjectArtifact, vector<COARpeakAny *> *pv) const
{
  bool bOK = (nID > 0) && m_pFile->CanEditArtifacts();
  size_t nRtn = 0;
  if(bOK)
  {
    _BuildMapIDchannel();
    map_ID_CHANNEL::iterator itr =  m_mapIDchannel.find(nID);
    if(itr != m_mapIDchannel.end())
    {
      const COARchannel *pcha = m_pFile->GetChannelByNr(itr->second);
      if(pcha != NULL)
      {
        auto_ptr<COARartifact> apArt(
          bInjectArtifact ? GetArtifactByID(nID) : NULL);
        COARartifact *pArt = apArt.get();

        const COARlocus *pLocus;
        const COARallele *pAllele;
        COARpeakAny *pPeak;
        size_t nLocus = pcha->GetLocusCount();
        size_t i;
        for(i = 0; i < nLocus; i++)
        {
          const wxString &sLocusName(pcha->GetLocusName(i));
          pPeak = NULL;
          if( (pLocus = _FindLocus(sLocusName)) == NULL ) 
          {}
          else if ( (pAllele = pLocus->GetAlleleByID(nID)) == NULL )
          {
            if( (pArt != NULL) && (pArt->SetLocus(sLocusName)) )
            {
              pPeak = new COARpeakAny(*pArt);
              pPeak->SetIsAllele(false);
            }
          }
          else
          {
            pPeak = new COARpeakAny(*pAllele);
            if(pArt != NULL)
            {
              pPeak->SetupArtifactInfo(pArt);
            }
          }
          if(pPeak != NULL)
          {
            pv->push_back(pPeak);
            nRtn++;
          }
        }
      }
    }
  }
  return nRtn;
}


vectorptr<COARpeakAny> *COARsample::GetPeaksByChannel(
  const COARchannel *pChannel, 
  const wxDateTime *pTime, 
  bool bIncludeDisabled) const
{
  MAP_ID_PEAK mapPeak;
  _MapIDPeakByChannel(&mapPeak,pChannel,pTime,bIncludeDisabled);
  vectorptr<COARpeakAny> *pRtn = _SortMap(&mapPeak);
  return pRtn;
}
bool COARsample::HasPeaks(int nChannel,const wxString &sLocus, bool bIncludeNotEditable) const
{
  const COARlocus *pLocus = _FindLocus(sLocus);
  bool bRtn = false;
  if(pLocus == NULL) {} // done
  else if(pLocus->AlleleCount() > 0)
  {
    bRtn = true;  // done
  }
  else
  {
    const COARchannelAlert *pcha = GetChannelAlertsByChannel(nChannel);
    if(pcha != NULL)
    {
      //MAP_ID_PEAK::iterator itrMap;
      vector<const COARartifact *> vArtifact;
      pcha->GetArtifactsByTime(sLocus,&vArtifact,NULL);
      if(vArtifact.empty()) {}
      else if(bIncludeNotEditable)
      {
        bRtn = true;
      }
      else
      {
        vector<const COARartifact *>::iterator itr;
        for(itr = vArtifact.begin(); itr != vArtifact.end(); ++itr)
        {
          if((*itr)->IsEditable())
          {
            bRtn = true;
            break;
          }
        }
      }
    }
  }
  return bRtn;
}
void COARsample::GetArtifactsByLocus(
  vector<const COARartifact *> *pv,
    const wxString &sLocusName,
    int nChannel,
    const wxDateTime *pt) const
{
  const COARchannelAlert *pcha = GetChannelAlertsByChannel(nChannel);
  if(pcha != NULL)
  {
    pcha->GetArtifactsByTime(sLocusName,pv,pt);
  }
}

vectorptr<COARpeakAny> *COARsample::GetPeaksByLocusName(
    const wxString &sLocus,
    int nChannel,
    const wxDateTime *pTime,
    bool bIncludeNotEditable) const
{
  MAP_ID_PEAK mapPeak;
  const COARlocus *pLocus = _FindLocus(sLocus);
  if(pLocus != NULL)
  {
    _AddAllelesToMap(&mapPeak,pLocus,pTime);
  }
  _AddArtifactsToMap(
    &mapPeak,nChannel,sLocus,pTime,true,bIncludeNotEditable);

  // now sort by time by placing all peaks in a set<>
  vectorptr<COARpeakAny> *pRtn = _SortMap(&mapPeak);
  return pRtn;
}
bool COARsample::SetPeaksByLocusName(
    const wxString &sLocus,
    int nChannel,
    vector<COARpeakAny *> *pv)
{
  vectorptr<COARpeakAny>::iterator itr;
  COARlocus *pLocus = FindLocus(sLocus);
  COARchannelAlert *pcha = _GetChannelAlertsByChannel(nChannel);
  bool bLocusUpdate;
  bool bUpdate(false);
  if(pLocus != NULL)
  {
    for(itr = pv->begin();
      itr != pv->end();
      ++itr)
    {
      bLocusUpdate = pLocus->UpdateAllele( (*itr) );
      if(bLocusUpdate)
      {
        bUpdate = true;
        if((*itr)->IsAllele() && (pcha != NULL))
        {
          // this peak was modified and it is an allele
          // if it is interlocus, then make sure that
          // the peak isn't used for an allele call
          // in another locus
          _CheckForSingleAllele((*itr)->GetID(), pcha, sLocus);
          //  6/9/16 need to make this more efficient 
          //  using GetPeaksByID -- STOP HERE 
        }
      }
    }
  }
//  vector<COARchannelAlert *> *pvAlert = m_pvChannelAlerts.Get();
//  vector<const COARartifact *> vArtifact;
  for(itr = pv->begin();
    itr != pv->end();
    ++itr)
  {
    COARpeakAny *pPeak = *itr;
    if( (pcha == NULL) && pPeak->IsArtifact() && !pPeak->IsDisabled() )
    {
      // there were no alerts for this channel until now
      // create new COARchannelAlert for this channel
      // second parameter == true means to create if it doesn't exist
      pcha = _GetChannelAlertsByChannel(nChannel,true);
    }
    if((pcha != NULL) && pcha->UpdateArtifact(sLocus, pPeak ))
    {
      bUpdate = true;
    }
  }
  return bUpdate;
}



//  3/13/09, if an allele is updated, make sure it isn't
//  an allele call in an adjacent locus

void COARsample::_CheckForSingleAllele(
  int nID, COARchannelAlert *pCha, const wxString &sLocus)
{
  COARartifact *pArt = NULL;
  if(pCha != NULL && nID > 0)
  {
    pArt = pCha->GetArtifactByID(nID);
  }
  if(pArt != NULL)
  {
    vector<const COARallele *> vpAllele;
    vector<COARlocus *>  vpLocus;
    vector<const COARallele *>::iterator itr;
    vector<COARlocus *>::iterator itrL;
    const COARartifactAllele *pArtAllele;
    const COARallele *pAllele;
    COARlocus *pLocus;
    size_t nCount = pArt->GetNumberOfAlleles();
    size_t i;

    vpAllele.reserve(nCount);
    vpLocus.reserve(nCount);
    for(i = 0; i < nCount; i++)
    {
      pArtAllele = pArt->GetAllele(i);
      const wxString &sAlleleLocus(pArtAllele->GetLocus());
      pLocus = FindLocus(sAlleleLocus);
      if(pLocus == NULL)
      {
        // should NEVER happen
        wxString sErr(
          "COARsample::_CheckForSingleAllele\n"
          "  Cannot find locus: ");
        sErr.Append(sAlleleLocus);
        wxASSERT_MSG(0,sErr);
        mainApp::LogMessage(sErr);
      }
      else if(sAlleleLocus == sLocus)
      {
        pAllele = pLocus->GetAlleleByID(nID);
        if((pAllele != NULL) && pAllele->IsDisabled())
        {
          // allele is not enabled in this locus,
          // therefore this check was not necessary
          i = nCount; // loop exit
          // clear array because we don't need to disable
          // adjacent allele
          vpAllele.clear();
          vpLocus.clear();
        }
      }
      else
      {
        pAllele = pLocus->GetAlleleByID(nID);
        if( (pAllele != NULL) && !pAllele->IsDisabled() )
        {
          // this allele may need to be disabled
          vpAllele.push_back(pAllele);
          vpLocus.push_back(pLocus);
        }
      }
    }

    // if the arrays are not empty, then the allele was
    // enabled in more than one locus
    // the array size should be 0 or 1
    wxASSERT_MSG(vpAllele.size() < 2,
      "COARsample::_CheckForSingleAllele array size > 1");

    for(itr = vpAllele.begin(), itrL = vpLocus.begin();
        itr != vpAllele.end();
        ++itr, ++itrL)
    {
      COARallele Allele(*(*itr));  // copy allele
      Allele.SetIsAllele(false);   // disable copy
      (*itrL)->UpdateAllele(&Allele); // update allele
    }
  }
}




size_t COARsample::CountChannelAlert(
  const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  const vector<COARchannelAlert *> *pvChannelAlerts = m_pvChannelAlerts.Get();
  const COARchannelAlert *pcha;
  const vector<int> *pvn;
  size_t nRtn = 0;
  for(vector<COARchannelAlert *>::const_iterator itr = pvChannelAlerts->begin();
    itr != pvChannelAlerts->end();
    ++itr)
  {
    pcha = *itr;
    pvn = pcha->GetMessageNumbers();
    nRtn += pMsgs->CountNotHidden(*pvn,pTime);
  }
  return nRtn;
}


bool COARsample::HasChannelAlert(
  const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  bool bRtn = false;
  if(pMsgs != NULL)
  {
    const vector<COARchannelAlert *> *pvChannelAlerts = m_pvChannelAlerts.Get();
    const COARchannelAlert *pcha;
    const vector<int> *pvn;
    for(vector<COARchannelAlert *>::const_iterator itr = pvChannelAlerts->begin();
      (itr != pvChannelAlerts->end()) && !bRtn;
      ++itr)
    {
      pcha = *itr;
      pvn = pcha->GetMessageNumbers();
      bRtn = pMsgs->FindDisplayed(*pvn,pTime);
    }
  }
  return bRtn;
}
bool COARsample::HasSampleAlert(
  const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  bool bRtn = 
    (pMsgs == NULL)
    ? false
    : pMsgs->FindDisplayed(m_alertSample,pTime);
  return bRtn;
}
bool COARsample::HasILSAlert(
  const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  bool bRtn = 
    (pMsgs == NULL)
    ? false
    : pMsgs->FindDisplayed(m_alertILS,pTime);
  return bRtn;
}

const COARmessages *COARsample::GetMessages() const
{
  return m_pFile->GetMessages();
}

void COARsample::_BuildSetLocus() const
{
  m_spLocus.clear();
  for(vector<COARlocus *>::const_iterator itr = m_vpLocus.begin();
    itr != m_vpLocus.end();
    ++itr)
  {
    m_spLocus.insert(*itr);
  }
}
const COARlocus *COARsample::_FindLocus(const wxString &sName) const
{
  COARlocus x(sName);
  set<COARlocus *,COARlocusLessByName>::const_iterator itr;
  const COARlocus *pRtn = NULL;
  if(m_spLocus.empty())
  {
    _BuildSetLocus();
  }
  itr = m_spLocus.find(&x);
  if(itr != m_spLocus.end()) { pRtn = *itr; }
  return pRtn;
}

void COARsample::PostProcessFile(COARfile *pFile)
{
  vector<COARlocus *>::iterator itr;
//  vector<COARchannelAlert *> *vpC = m_pvChannelAlerts.Get();
  if(pFile != NULL)
  {
    m_pFile = pFile;
  }
  for(itr = m_vpLocus.begin();
    itr != m_vpLocus.end();
    ++itr)
  {
    (*itr)->PostProcessFile(this);
  }
}


const COARchannelAlert *COARsample::GetChannelAlertsByChannel(int n) const
{
  const COARchannelAlert *pRtn(NULL);
  const COARchannelAlert *p;
  const vector<COARchannelAlert *> *pch(GetChannelAlerts());
  size_t nSize = pch->size();
  for(size_t i = 0; i < nSize; ++i)
  {
    p = pch->at(i);
    if(p->GetNumber() == n)
    {
      pRtn = p;
      i = nSize; // loop exit
    }
  }
  return pRtn;
}


void COARfile::AppendVectorInt(vector<int> *pvn, const vector<int> &vn)
{
  pvn->reserve(pvn->size() + vn.size());
  for(vector<int>::const_iterator itr = vn.begin();
    itr != vn.end();
    ++itr)
  {
    pvn->push_back(*itr);
  }
}

void COARsample::AppendChannelAlerts(vector<int> *_pvn) const
{
  const vector<COARchannelAlert *> *pv = m_pvChannelAlerts.Get();
  vector<COARchannelAlert *>::const_iterator itr;
  const vector<int> *pvn;
  size_t nSize = _pvn->size();
  for(itr = pv->begin(); itr != pv->end(); ++itr)
  {
    nSize += (*itr)->GetMessageNumbers()->size();
  }
  _pvn->reserve(nSize);
  for(itr = pv->begin(); itr != pv->end(); ++itr)
  {
    pvn = (*itr)->GetMessageNumbers();
    COARfile::AppendVectorInt(_pvn,*pvn);
  }
}
const wxString COARsample::g_sCellChannelEdited("~");
const wxString COARsample::g_sCellILSNotice("!");

wxString COARsample::GetCellChannel(const COARmessages *pMsgs, const wxDateTime *pTime) const
{
  wxString s;
  wxString sRtn;
  set<int> setChannelNr;
  const vector<COARchannelAlert *> *pv = m_pvChannelAlerts.Get();
  const COARchannelAlert *pca;
  const vector<int> *pvn;
  bool bEdited = IsCellChannelEdited(pMsgs,pTime);
//    (!m_NotesChannel.GetText().IsEmpty()) ||
//    (!m_vpOldNotesChannel.empty());

  for(vector<COARchannelAlert *>::const_iterator itr = pv->begin();
    itr != pv->end();
    ++itr)
  {
    pca = *itr;
    pvn = pca->GetMessageNumbers();
    if(pMsgs->FindDisplayed(*pvn,pTime))
    {
      setChannelNr.insert( pca->GetNumber() );
    }
//    if( (!bEdited) && pMsgs->FindEdited(*pvn,pTime) )
//    {
//      bEdited = true;
//    }
  }
  for(set<int>::iterator nitr = setChannelNr.begin();
    nitr != setChannelNr.end();
    ++nitr)
  {
    s.Printf("%d",*nitr);
    COARfile::AppendCellString(&sRtn,s);
  }
  if(bEdited && sRtn.IsEmpty())
  {
    sRtn = g_sCellChannelEdited;
  }
  return sRtn;
}
bool COARsample::IsEnabled(const wxDateTime *pTime) const
{
  bool bRtn = m_EnableHistory.IsEnabled(pTime);
  return bRtn;
}
bool COARsample::SetEnabled(const wxString &sName, bool bEnable)
{
  bool bRtn = (bEnable != IsEnabled());
  if((!bEnable) && (m_pFile != NULL))
  {
    m_pFile->SetLastSampleDisabled(this);
  }
  if(bRtn)
  {
    m_EnableHistory.SetEnabled(sName,bEnable);
    UpdateVersion();
  }
  return bRtn;
}
bool COARsample::IsCellILSEdited(const COARmessages *pMsgs,const wxDateTime *pTime) const
{
  bool bEdited = IsNotesILSEdited(pTime);
  if(!bEdited && (pMsgs != NULL))
  {
    bEdited = pMsgs->FindEdited(m_alertILS,pTime);
  }
  return bEdited;
}

bool COARsample::IsSampleLevelEdited(const COARmessages *pMsgs,const wxDateTime *pTime) const
{
  bool bEdited = IsNotesSampleEdited(pTime);
  if(!bEdited && (pMsgs != NULL))
  {
    bEdited = pMsgs->FindEdited(m_alertSample,pTime);
  }
  return bEdited;
}

bool COARsample::IsCellChannelEdited(const COARmessages *pMsgs,const wxDateTime *pTime) const
{
  bool bEdited = IsNotesChannelEdited(pTime);

  if(pMsgs != NULL)
  {
    const vector<int> *pvn;
    const vector<COARchannelAlert *> *pv = m_pvChannelAlerts.Get();
    const COARchannelAlert *pca;
    for(vector<COARchannelAlert *>::const_iterator itr = pv->begin();
      itr != pv->end() && !bEdited;
      ++itr)
    {
      pca = *itr;
      pvn = pca->GetMessageNumbers();
      if( pMsgs->FindEdited(*pvn,pTime) )
      {
        bEdited = true;
      }
    }
  }
  return bEdited;
}

bool COARsample::NeedAttention(const CLabSettings &lab) const
{
  bool bRtn = false;
  if(!IsDisabled())
  {
    bRtn = 
      NeedILSReview(lab.GetReviewerCountILS(),NULL) ||
      NeedChannelReview(lab.GetReviewerCountChannel(),NULL) ||
      NeedSampleReview(lab.GetReviewerCountSample(),NULL) ||
      NeedILSAcceptance(lab.GetAcceptanceCountILS(),NULL) ||
      NeedChannelAcceptance(lab.GetAcceptanceCountChannel(),NULL) ||
      NeedSampleAcceptance(lab.GetAcceptanceCountSample(),NULL);
    if(!bRtn)
    {
      const COARlocus *pLocus;
      int nMinReviewer = lab.GetReviewerCountLocus();
      int nMinAcceptance = lab.GetAcceptanceCountLocus();
      for(vector<COARlocus *>::const_iterator itr = m_vpLocus.begin();
        (itr != m_vpLocus.end()) && !bRtn;
        ++itr)
      {
        pLocus = *itr;
        bRtn = pLocus->NeedAttention(nMinReviewer, nMinAcceptance);
      }
    }
  }
  return bRtn;
}

int COARsample::GetChannelNrFromLocus(const wxString &sLocusName) const
{
  int nRtn = 0;
  if(m_pFile != NULL)
  {
    const COARchannel *pChannel =
      m_pFile->GetChannelFromLocus(sLocusName);
    if(pChannel != NULL)
    {
      nRtn = pChannel->GetChannelNr();
    }
  }
  return nRtn;
}
int COARsample::GetChannelNrFromLocus(const COARlocus *pLocus) const
{
  int nRtn = GetChannelNrFromLocus(pLocus->GetName());
  return nRtn;
}

void COARsample::SetIsModified(bool b)
{
  if(m_pFile != NULL)
  {
    m_pFile->SetIsModified(b);
  }
}
void COARsample::UpdateVersion()
{
  if(m_pFile != NULL)
  {
    m_pFile->UpdateVersion();
  }
}
wxString COARsample::FormatRunStart() const
{
  wxString s = GetRunStart();
  nwxString::Trim(&s);
  size_t nLen = s.Len();
  s.Replace("T"," ",true);
  if((nLen == 18) && (s.Mid(13,1) == ":"))
  {
    //                          11111111
    //                012345678901234567
    // in the form of YYYY-MM-DD HH:MMSS
    s.insert(16,":");
  }
  return s;
}
