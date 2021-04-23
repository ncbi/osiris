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
  _SetupSiteFileNames();
  _Load();
}

bool CILSLadderInfo::_SetupSiteFileNames()
{
  // get all site ILS and Ladder name files
  // if the list has changed or a modification time
  // has changed, then return true to indicate that it
  // needs to be reloaded
  std::set<wxString> setFiles;
  std::set<wxString>::iterator itrs;
  std::map<wxString, wxDateTime>::iterator itrm;
  wxDateTime t;
  size_t nCount = mainApp::GetConfig()->GetSiteILSFileNames(&setFiles);
  bool bReload = (nCount != m_mapSiteFiles.size());
  if (!bReload)
  {
    for (itrs = setFiles.begin(); (!bReload) && itrs != setFiles.end(); ++itrs)
    {
      itrm = m_mapSiteFiles.find(*itrs);
      bReload = (itrm == m_mapSiteFiles.end()) ||
        (itrm->second != nwxFileUtil::GetModTime(*itrs));
    }
  }
  if (bReload)
  {
    m_mapSiteFiles.clear();
    for (itrs = setFiles.begin(); itrs != setFiles.end(); ++itrs)
    {
      m_mapSiteFiles.insert(
        std::map<wxString, wxDateTime>::value_type(*itrs, nwxFileUtil::GetModTime(*itrs)));
    }
  }
  return bReload;
}

bool CILSLadderInfo::_NeedReload()
{
  bool bReload = IsOK() ? _SetupSiteFileNames() : false;
  return bReload;
}
bool CILSLadderInfo::CheckReload()
{
  bool bRtn = _NeedReload() ? _Load() : IsOK();
  return bRtn;
}
bool CILSLadderInfo::_Load()
{
  wxString sFile = mainApp::GetConfig()->GetILSLadderFileName();
  _Cleanup();
  m_bIsOK = _LoadFile(sFile);

  if (m_bIsOK)
  {
    std::map<wxString, wxDateTime>::iterator itr;
    for (itr = m_mapSiteFiles.begin(); itr != m_mapSiteFiles.end(); ++itr)
    {
      _LoadFile(itr->first);
    }
  }
  return m_bIsOK;
}

void CILSLadderInfo::_MoveData(const wxString &sFileName)
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
      itr->second->SetFileName(sFileName);
      m_mapCILSfamily.insert(*itr);
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