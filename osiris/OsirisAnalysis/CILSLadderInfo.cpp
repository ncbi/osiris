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
