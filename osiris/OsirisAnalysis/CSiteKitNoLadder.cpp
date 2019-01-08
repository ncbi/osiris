#define __C_SITE_KIT_NO_LADDER_CPP__
#include <memory>
#include <wx/arrstr.h>
#include "CSiteKitNoLadder.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxFileUtil.h"
#include "CVolumes.h" // for CVolume::GetPathPrefix
#include "CSitePath.h"
nwxIMPLEMENT_GLOBAL_OBJECT(CSiteKitNoLadderCollection)

#define LAB_SETTINGS_FILE wxS("LabSettings.xml")
#define LADDER_INFO_FILE wxS("LadderInfo.xml")
#define SETUP_FILE wxS("Setup_NL.xml")

const wxChar *CSiteKitNoLadderCollection::g_psNames[] =
{
  SETUP_FILE,
  LAB_SETTINGS_FILE,
  LADDER_INFO_FILE,
  NULL
};

const wxString CSiteKitNoLadderCollection::g_sKITSdir(wxS("MarkerSets"));

CSiteKitNoLadder * CSiteKitNoLadderCollection::_findByFileName(const wxString &sFileName)
{
  FILE_TO_KIT::iterator itr = m_mapFileNameKit.find(sFileName);
  CSiteKitNoLadder *pRtn =
    (itr == m_mapFileNameKit.end()) ? NULL : itr->second;
  return pRtn;
}

wxDirTraverseResult CSiteKitNoLadderCollection::OnDir(const wxString &dirname)
{
  wxString sFileName;
  bool bLoad = false;
  if(_hasFiles(dirname))
  {
    sFileName = CVolume::GetPathPrefix(dirname);
    sFileName.Append(SETUP_FILE);
    CSiteKitNoLadder *pTemp = _findByFileName(sFileName);
    bLoad = (pTemp == NULL);
  }
  if (bLoad)
  {
    // not already loaded
    auto_ptr<CSiteKitNoLadder> m_ptr(new CSiteKitNoLadder);
    if (!m_ptr->LoadFile(sFileName))
    {
      wxString s(wxS("Cannot load file ladder free kit file: "));
      s.Append(sFileName);
      nwxLog::LogMessage(s);
      _incrementErrorCount();
    }
    else if (m_mapFileNameKit.find(sFileName) != m_mapFileNameKit.end())
    {
      wxString s(wxS("File name already found in Ladder Free kit list: "));
      s.Append(sFileName);
      nwxLog::LogMessage(s);
      _incrementErrorCount();
    }
    else
    {
      m_mapKitName.clear();
      m_mapFileNameKit.insert(
          FILE_TO_KIT::value_type(sFileName, m_ptr.release()));
    }
  }
  return wxDIR_IGNORE;
}
wxDirTraverseResult CSiteKitNoLadderCollection::OnFile(const wxString &)
{
  return wxDIR_CONTINUE;
}


void CSiteKitNoLadderCollection::_cleanup()
{
  _clearErrorCount();
  m_mapKitName.clear();
  pFILE_TO_KIT::cleanup(&m_mapFileNameKit);
}

bool CSiteKitNoLadderCollection::Load(bool bForce)
{
  _clearErrorCount();
  if (bForce)
  {
    _cleanup();
  }
  else
  {
    _checkReload();
  }
  // GET PATH and Traverse
  CSitePath *pSitePath = CSitePath::GetGlobal();
  wxString sPath = pSitePath->GetRealSitePath();
  nwxFileUtil::EndWithSeparator(&sPath);
  sPath.Append(g_sKITSdir);
  if(wxFileName::DirExists(sPath))
  {
    wxDir dirKits(sPath);
    dirKits.Traverse(*this, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN);
  }
  return !GetErrorCount();
}

bool CSiteKitNoLadderCollection::_hasFiles(const wxString &sDirName)
{
  return CVolume::HasAllFiles(sDirName, g_psNames);
}

bool CSiteKitNoLadderCollection::_checkReload()
{
  wxString sName;
  bool bChange = false;
  int nBeginErrorCount = GetErrorCount();
  FILE_TO_KIT::iterator itr;
  CSiteKitNoLadder *pKit;
  std::vector<wxString> vKillList;
  for (itr = m_mapFileNameKit.begin();
    itr != m_mapFileNameKit.end();
    ++itr
    )
  {
    pKit = itr->second;
    sName = pKit->GetKitName();
    if (!pKit->FileExists())
    {
      // file was removed, probably by another process
      vKillList.push_back(itr->first);
    }
    else if (!pKit->CheckFileModification())
    {
    }
    else if(!pKit->ReloadFile())
    {
      // found a problem
      _incrementErrorCount();
      wxString sError(wxS("Could not reload file: "));
      sError.Append(itr->first);
      nwxLog::LogMessage(sError);
      vKillList.push_back(itr->first);
    }
    else if(pKit->GetKitName() != sName)
    {
      // a kit changed its name
      // wipe out m_mapKitNames
      // if GetNames() is called, rebuild
      bChange = true;
    }
  }
  if(vKillList.size() > 0 )
  {
    std::vector<wxString>::iterator itrf;
    for (itrf = vKillList.begin();
      itrf != vKillList.end();
      ++itrf)
    {
      itr = m_mapFileNameKit.find(*itrf);
      if (itr != m_mapFileNameKit.end())
      {
        pKit = itr->second;
        m_mapFileNameKit.erase(itr);
        delete pKit;
      }
    }
    // wipe out m_mapKitNames
    // if GetNames() is called, rebuild
    bChange = true;
  }
  if (bChange)
  {
    // something changed somewhere, if GetNames() is called, rebuild
    m_mapKitName.clear();
  }
  return (nBeginErrorCount == GetErrorCount());
}


void CSiteKitNoLadderCollection::_setupNames()
{
  if (m_mapFileNameKit.size() && !m_mapKitName.size())
  {
    FILE_TO_KIT::iterator itr;
    for (itr = m_mapFileNameKit.begin();
      itr != m_mapFileNameKit.end();
      ++itr)
    {
      const wxString &sName = itr->second->GetKitName();
      if (m_mapKitName.find(sName) == m_mapKitName.end())
      {
        m_mapKitName.insert(NAME_TO_KIT::value_type(sName, itr->second));
      }
      else
      {
        wxString s(wxS("Name found in more than one kit, "));
        s.Append(sName);
        nwxLog::LogMessage(s);
      }
    }
  }
}

bool CSiteKitNoLadderCollection::RemoveKit(const CSiteKitNoLadder &kit)
{
  bool bRtn = false;
  wxString sFileName = kit.GetFileName();
  wxString sKitName = kit.GetKitName();
  m_sLastMessage.Empty();
  if (sFileName.IsEmpty())
  {
    m_sLastMessage = "Cannot remove kit, ";
    m_sLastMessage.Append(sKitName);
    m_sLastMessage.Append(", file name is unknown.");
  }
  else
  {
    wxFileName fn(sFileName);
    wxFileName fnPath(fn.GetPath());
    if (fnPath.Rmdir(wxPATH_RMDIR_RECURSIVE))
    {
      bRtn = true;
    }
    else
    {
      m_sLastMessage = "Attempt to remove files for kit, ";
      m_sLastMessage.Append(sKitName);
      m_sLastMessage.Append(", was not successful.");
    }
    Load(false);
  }
  return bRtn;
}
wxString CSiteKitNoLadderCollection::GetTopDirectory(bool bAppendSeparator)
{
  wxString sPath;
  CSitePath *pSite = CSitePath::GetGlobal();
  if (pSite->SitePathExists())
  {
    sPath = pSite->GetSitePath();
    nwxFileUtil::EndWithSeparator(&sPath);
    sPath.Append("MarkerSetsNoLadder");
    if(bAppendSeparator) { nwxFileUtil::EndWithSeparator(&sPath); }
  }
  return sPath;
}
bool CSiteKitNoLadderCollection::CreateKit(const CSiteKitNoLadder &kit)
{
  return false;
}

bool CSiteKitNoLadderCollection::UpdateKit(const CSiteKitNoLadder &kit)
{
  return false;
}

size_t CSiteKitNoLadderCollection::GetNames(
  wxArrayString *pasNames, bool bWithSite)
{
  pasNames->Empty();
  _setupNames();
  NAME_TO_KIT::iterator itr;
  wxString s;
  for (itr = m_mapKitName.begin(); itr != m_mapKitName.end(); ++itr)
  {
    s = itr->first;
    if (bWithSite)
    {
      CSiteKitNoLadder::SetSiteIndicatorOnName(&s);
    }
    else
    {
      CSiteKitNoLadder::ClearSiteIndicatorOnName(&s);
    }
    pasNames->Add(itr->first);
  }
  return pasNames->GetCount();
}




