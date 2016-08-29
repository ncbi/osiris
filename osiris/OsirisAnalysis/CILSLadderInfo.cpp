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


CILSLadderInfo::CILSLadderInfo(bool bLoad) : 
  nwxXmlPersist(true),
  m_vKits(wxT("Set")), 
  m_mapCILSfamily(wxT("ILS"),false),
  m_bIsOK(false)
{
  RegisterAll(true);
  if(bLoad)
  {
    Load();
  }
}


bool CILSLadderInfo::Load()
{
  ConfigDir *pDir = mainApp::GetConfig();
  wxString sFile = pDir->GetILSLadderFileName();
  _Cleanup();
  m_bIsOK = LoadFile(sFile);
  if(!m_bIsOK)
  {
    m_vKits.Clear();
  }
  return m_bIsOK;
}

void CILSLadderInfo::_Cleanup()
{
  m_mapNameToDisplay.clear();
  m_mapDisplayToName.clear();
  m_vKits.Clear();
  m_mapCILSfamily.Clear();
  m_bIsOK = false;
}

void CILSLadderInfo::_BuildNameToDisplay() const
{
  //if(IsOK() && !m_mapNameToDisplay.size())
  {
    const std::map<wxString,CILSfamily *> *pmap = m_mapCILSfamily.Get();
    std::map<wxString,CILSfamily *>::const_iterator itrf;
    for(itrf = pmap->begin(); itrf != pmap->end(); ++itrf)
    {
      const CILSfamily *pFam = itrf->second;
      const std::vector<CILSname *> &vName = pFam->GetNames();
      std::vector<CILSname *>::const_iterator itr;
      for(itr = vName.begin(); itr != vName.end(); ++itr)
      {
        m_mapNameToDisplay.insert(
          std::map<const wxString ,const wxString &>::value_type
            ( (*itr)->GetName(),(*itr)->GetDisplayName() ) );
        m_mapDisplayToName.insert(
          std::map<const wxString ,const wxString &>::value_type
            ( (*itr)->GetDisplayName(),(*itr)->GetName() ) );
      }
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

const wxString &CILSLadderInfo::FindLSname
  (const wxString &sDisplayName) const
{
  if(IsOK() && !m_mapDisplayToName.size())
  {
    _BuildNameToDisplay();
  }
  std::map<const wxString ,const wxString &>::const_iterator itr =
    m_mapDisplayToName.find(sDisplayName);
  const wxString &sRtn = (itr == m_mapDisplayToName.end())
    ? mainApp::EMPTY_STRING : itr->second;
  return sRtn;
}
