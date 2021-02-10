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
*  FileName: CILSLadderInfo.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CILSLadderInfo.h"
#include <wx/arrstr.h>
#include <wx/filename.h>
#include "nwx/nwxString.h"

CILSLadderInfo::CILSLadderInfo() : 
  nwxXmlPersist(true),
  m_mapCILSfamilyTmp(wxT("ILS"),false),
  m_bIsOK(false)
  
{
  m_dtFileModTime.Set(time_t(0));
  RegisterAll(true);
  Load();
}

bool CILSLadderInfo::_NeedReload()
{
  bool bReload = !IsOK();
  if (!bReload)
  {
    // check site data
    wxString sFile = mainApp::GetConfig()->GetSiteILSFileName();
    wxFileName fn(sFile);
    bReload = fn.IsFileReadable()
      ? (fn.GetModificationTime() != m_dtFileModTime)
      : (m_dtFileModTime.GetTicks() > 0);
  }
  return bReload;
}
bool CILSLadderInfo::CheckReload()
{
  bool bRtn = _NeedReload() ? Load() : IsOK();
  return bRtn;
}
bool CILSLadderInfo::Load()
{
  wxString sFile = mainApp::GetConfig()->GetILSLadderFileName();
  _Cleanup();
  m_bIsOK = LoadFile(sFile);
  _MoveData(false);

  if (m_bIsOK)
  {
    sFile = mainApp::GetConfig()->GetSiteILSFileName();
    wxFileName fn(sFile);
    if (fn.IsFileReadable())
    {
      m_dtSiteFile = fn.GetModificationTime();
      m_bIsOK = LoadFile(sFile);
      _MoveData(true);
    }
  }
  return m_bIsOK;
}

void CILSLadderInfo::_MoveData(bool bSite)
{
  TnwxXmlPersistMap<wxString, CILSfamily>::iterator itr;
  for (itr = m_mapCILSfamilyTmp->begin();
    itr != m_mapCILSfamilyTmp->end();
    ++itr)
  {
    if (m_mapCILSfamily.find(itr->first) != m_mapCILSfamily.end())
    {
      delete itr->second;
    }
    else
    {
      itr->second->SetSite(bSite);
      m_mapCILSfamily.insert(std::map<wxString, CILSfamily *>::value_type(itr->first, itr->second));
    }
  }
  m_mapCILSfamilyTmp->clear();
}

void CILSLadderInfo::_Cleanup()
{
  m_mapNameToDisplay.clear();
  mapptr<wxString, CILSfamily>::cleanup(&m_mapCILSfamily);
  m_mapCILSfamilyTmp.Clear();
  m_sVersion.Clear();
  m_dtSiteFile.Set(time_t(0));
  m_bIsOK = false;
}

void CILSLadderInfo::_BuildNameToDisplay() const
{
  std::map<wxString,CILSfamily *>::const_iterator itrf;
  for(itrf = m_mapCILSfamily.begin(); itrf != m_mapCILSfamily.end(); ++itrf)
  {
    const CILSfamily *pFam = itrf->second;
    const std::vector<CILSname *> &vName = pFam->GetNames();
    std::vector<CILSname *>::const_iterator itr;
    for(itr = vName.begin(); itr != vName.end(); ++itr)
    {
      m_mapNameToDisplay.insert(
        std::map<const wxString ,const wxString &>::value_type
          ( (*itr)->GetName(),(*itr)->GetDisplayName() ) );
    }
  }
}


const wxString &CILSLadderInfo::FindDisplayName
  (const wxString &sLSname) const
{
  if(IsOK() && !m_mapNameToDisplay.size())
  {
    _BuildNameToDisplay();
  }
  std::map<const wxString ,const wxString &>::const_iterator itr =
    m_mapNameToDisplay.find(sLSname);
  const wxString &sRtn = (itr == m_mapNameToDisplay.end())
    ? mainApp::EMPTY_STRING : itr->second;
  return sRtn;
}

void CILSLadderInfo::BuildAll_ILSarray(wxArrayString *ps) const
{
  std::map<const wxString, const wxString &>::const_iterator itr;
  ps->Empty();
  if (!m_mapNameToDisplay.size())
  {
    _BuildNameToDisplay();
  }
  ps->Alloc(m_mapNameToDisplay.size());
  for (itr = m_mapNameToDisplay.begin();
    itr != m_mapNameToDisplay.end();
    ++itr)
  {
    ps->Add(itr->first);
  }
}