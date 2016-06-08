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
*  FileName: CArtifactLabels.cpp
*  Author:   Douglas Hoffman
*
*  Class for loading and retrieving the labels shown
*  for artifacts on a plot.  These are stored in the file,
*  ArtifactLabels.xml in the Config/LadderSpecifications directory
*  of the OSIRIS distribution
* 
*/

#include "mainApp.h"
#include "ConfigDir.h"
#include "CArtifactLabels.h"
#include "CArtifactLabelsUser.h"
#include "nwx/vectorSort.h"
#include "TArtifactGreater.h"

CArtifactGroup::CArtifactGroup() : nwxXmlPersist(true)
{
  RegisterAll(true);
  SetDefaults();
}
void CArtifactGroup::RegisterAll(bool)
{
  RegisterWxString(wxT("ID"),  &m_sID);
  RegisterWxString(wxT("Label"),  &m_sLabel);
  RegisterWxString(wxT("Display"),&m_sDisplay);
  RegisterInt(wxT("Priority"),    &m_nPriority);
  Register(wxT("MsgName"),     &m_ios, (void *) &m_vsMsgName);
  Register(wxT("SearchString"),&m_ios, (void *) &m_vsSearchString);
}
void CArtifactGroup::SetDisplay(const IArtifactGroup &g)
{
  SetDisplay(g.GetDisplayUser());
  SetPriority(g.GetPriorityUser());
}
void CArtifactGroup::_cleanupRegEx() const
{
  vectorptr<wxRegEx>::cleanup(&m_pvSearchRE);
}
void CArtifactGroup::_buildRegEx() const
{
  std::vector<wxString>::const_iterator itr;
  _cleanupRegEx();
  for(itr = m_vsSearchString.begin();
    itr != m_vsSearchString.end();
    ++itr)
  {
    wxRegEx *pRE = new wxRegEx(*itr,wxRE_ICASE | wxRE_ADVANCED );
    if(!pRE->IsValid())
    {
      delete pRE;
      pRE = NULL;
      wxString sMsg(wxT("Invalid regular expression in artifact labels: "));
      sMsg += *itr;
      mainApp::LogMessage(sMsg);
    }
    m_pvSearchRE.push_back(pRE); // push back even if null to make count match
  }
}
bool CArtifactGroup::IsStringMatch(const wxString &s) const
{
  std::vector<wxRegEx *>::iterator itr;
  bool bRtn = false;

  if(m_pvSearchRE.size() != m_vsSearchString.size())
  {
    _buildRegEx();
  }

  for(itr = m_pvSearchRE.begin();
    (itr != m_pvSearchRE.end()) && (!bRtn);
    ++itr)
  {
    if( ((*itr) != NULL) && (*itr)->Matches(s) )
    {
      bRtn = true;
    }
  }
  return bRtn;
}
bool CArtifactGroup::IsMsgTypeMatch(const wxString &s) const
{
  bool bRtn = false;
  std::vector<wxString>::const_iterator itr;
  for(itr = m_vsMsgName.begin();
    (itr != m_vsMsgName.end()) && (!bRtn);
    ++itr)
  {
    if(! (*itr).CompareTo(s) )
    {
      bRtn = true;
    }
  }
  return bRtn;
}

const wxString CArtifactLabels::g_sDEFAULT_LABEL(wxT("A"));


CArtifactLabels::CArtifactLabels() : nwxXmlPersist()
{
  RegisterAll(true);
  wxString sFile = mainApp::GetConfig()->GetArtifactLabelsFileName();
  m_bOK = !sFile.IsEmpty() && LoadFile(sFile,false);
  if(!m_bOK)
  {
    mainApp::LogMessage(wxT("Cannot load artifact labels file."));
  }
  else
  {
    CArtifactLabelsUser lb(true);
    if(lb.GetCount()) { SetUserLabels(lb); }
  }
}
CArtifactLabels::~CArtifactLabels()
{
  m_io.Cleanup();
}

bool CArtifactLabels::LoadFile(const wxString &sFileName, bool bLock)
{
  bool bRtn = nwxXmlPersist::LoadFile(sFileName,bLock);
  if(bRtn) { Sort(); }
  else { vectorptr<CArtifactGroup>::cleanup(&m_vector); }
  return bRtn;
}

const wxString &CArtifactLabels::GetDisplayFromString(const wxString &sArtifactString) const
{
  if(this->IsOK())
  {
    std::vector<CArtifactGroup *>::const_iterator itr;
    for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
    {
      if((*itr)->IsStringMatch(sArtifactString))
      {
        return (*itr)->GetDisplay();
      }
    }
  }
  return g_sDEFAULT_LABEL;
}

const wxString &CArtifactLabels::GetDisplayFromMsgType(const wxString &sMsgType) const
{
  if(IsOK())
  {
    std::vector<CArtifactGroup *>::const_iterator itr;
    for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
    {
      if((*itr)->IsMsgTypeMatch(sMsgType))
      {
        return (*itr)->GetDisplay();
      }
    }
  }
  return mainApp::EMPTY_STRING;
}



void CArtifactLabels::Sort()
{
  typedef class TArtifactGreater<CArtifactGroup> CArtifactGreater;
  vectorSort<CArtifactGroup *,CArtifactGreater>::SortMulti(&m_vector);

  //
  //  renumber priority
  //
  vector<CArtifactGroup *>::iterator itr;
  int nPriority;
  for(itr = m_vector.begin(), nPriority = m_vector.size();
    itr != m_vector.end();
    ++itr, --nPriority)
  {
    (*itr)->SetPriority(nPriority);
  }
}

bool CArtifactLabels::SetUserLabels(const CArtifactLabelsUser &labels)
{
  // returns true if at least one label changes
  bool bRtn = false;
  size_t n = labels.GetCount();
  if(n && m_vector.size())  // update only when needed
  {
    //  build map of label to CArtifactGroup
    std::map<wxString,CArtifactGroup *> mapIDGroup;
    std::map<wxString,CArtifactGroup *>::iterator itrL, itrLend;
    std::vector<CArtifactGroup *>::iterator itr;
    for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
    {
      mapIDGroup.insert(
        std::map<wxString,CArtifactGroup *>::value_type
          ((*itr)->GetID(),*itr)
          );
    }
    itrLend = mapIDGroup.end();
    size_t i;
    const CArtifactGroupUser *pl;
    for(i = 0; i < n; i++)
    {
      pl = labels.Get(i);
      itrL = mapIDGroup.find(pl->GetID());
      if( (itrL != itrLend) &&
        !IArtifactGroup::UserDataEqual(itrL->second,pl) )
      {
        itrL->second->SetDisplay(*pl);
        bRtn = true;
      }
    }
    if(bRtn) { Sort(); }
  }
  return bRtn;
}
CArtifactLabelsUser *CArtifactLabels::BuildUserLabels() const
{
  CArtifactLabelsUser *pRtn = new CArtifactLabelsUser(false);
  std::vector<CArtifactGroup *>::const_iterator itr;
  for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
  {
    pRtn->AddLabel(*(*itr));
  }
  return pRtn;
}
void CArtifactLabels::ResetDefaults()
{
  for(vector<CArtifactGroup *>::iterator itr = m_vector.begin();
    itr != m_vector.end();
    ++itr)
  {
    (*itr)->SetDefaults();
  }
  Sort();
}
