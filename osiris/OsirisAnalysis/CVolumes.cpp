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
*  FileName: CVolumes.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CVolumes.h"
#include "ConfigDir.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxKillDir.h"
#include "CKitList.h"
#include <ctype.h>
#include <wx/debug.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <memory>

#define ACCESS_FILE       wxS("access.txt")
#define MESSAGE_BOOK_FILE wxS("MessageBookV4.0.xml")
#define STD_SETTINGS_FILE wxS("StdSettings.xml")
#define LAB_SETTINGS_FILE wxS("LabSettings.xml")

#define LOCK_READ_MS  5000
#define LOCK_MS 10000

//******************************************************  CVolume

CVolume::CVolume(const wxString &sPath, bool bSetReadOnly)
{
  m_bIgnoreReadLock = false;
  m_bNewVolume = false;
  m_bOK = true;
  m_nCountDown = 0;
  Load(sPath,bSetReadOnly);
}
CVolume::CVolume(const CVolume &x)
{
  (*this) = x;
}
CVolume::CVolume()
{
  m_bReadOnly = false;
  m_bOK = false;
  m_bIgnoreReadLock = false;
  m_bNewVolume = false;
  m_nCountDown = 0;
}
CVolume &CVolume::operator = (const CVolume &x)
{
  m_bIgnoreReadLock = false;
  m_bNewVolume = false;
  m_nCountDown = 0;
  m_bReadOnly = x.IsReadOnly();
  Load(x.GetPath(),x.IsReadOnly());
  return *this;
}


CVolume::~CVolume() {}

wxString CVolume::GetKitName()
{
  wxString sRtn = m_lab.GetKitName();
  return sRtn;
}


const wxString &CVolume::GetVolumeName()
{
  CLabSettingsInfo *p = m_lab.GetLabSettingsInfo();
  return p->GetVolumeName();
}

wxString CVolume::_GetFilePath(const wxChar *pFileName) const
{
  wxString sRtn;
  if(IsOK() && m_sPath.Len())
  {
    sRtn = GetPathPrefix(m_sPath);
    sRtn.Append(pFileName);
  }
  return sRtn;
}
wxString CVolume::GetAccessFileName() const
{
  return _GetFilePath(ACCESS_FILE);
}
wxString CVolume::GetMessageBookFileName() const
{
  ConfigDir *pDir = mainApp::GetConfig();
  return pDir->GetMessageBookFileName();
////  return _GetFilePath(MESSAGE_BOOK_FILE);
}

wxString CVolume::GetStdSettingsFileName() const
{
  return _GetFilePath(STD_SETTINGS_FILE);
}

wxString CVolume::GetLabSettingsFileName() const
{
  return _GetFilePath(LAB_SETTINGS_FILE);
}

void CVolume::SetVolumeName(const wxString &s)
{
  if(IsOK() && !IsReadOnly() && HasLock())
  {
    m_lab.GetLabSettingsInfo()->SetVolumeName(s);
  }
}
bool CVolume::InitNewVolume(const wxString &s)
{
  bool bRtn = false;
  m_bIgnoreReadLock = true;
  if(IsOK() && !IsReadOnly() && Lock())
  {
    CLabSettingsInfo *pInfo = m_lab.GetLabSettingsInfo();
    pInfo->SetNotes("");
    pInfo->SetVolumeName(s);
    bRtn = Save();
    m_bNewVolume = bRtn;
  }
  return bRtn;
}

bool CVolume::Load(const wxString &sPath, bool bSetReadOnly)
{
  m_nCountDown = 0;
  m_sPath = sPath;
  m_bReadOnly = bSetReadOnly;
  m_bOK = true; // must be true for GetLabSettingsFileName() and GetMessageBookFileName()
  m_bOK = m_lab.LoadFile(GetLabSettingsFileName())
      && m_book.LoadFile(GetMessageBookFileName());
//  m_lockRead.SetFileName(GetAccessFileName());
  return m_bOK;
}

bool CVolume::Save()
{
  bool bRtn = false;
  if(IsOK() && !IsReadOnly() && HasLock())
  {
    bRtn = m_lab.SaveFile(GetLabSettingsFileName());
  }
  return bRtn;
}
bool CVolume::Save(const wxString &sPath)
{
  wxString sPathSave(m_sPath);
  bool bRtn = false;
  m_sPath = sPath;
  if(IsOK() && !IsReadOnly() && Lock() && Save())
  {
    bRtn = true;
  }  
  else
  {
    m_sPath = sPathSave;
    m_nCountDown = 0;
  }
  return bRtn;
}

bool CVolume::CheckReload(bool bForceReload)
{
  bool bRtn = false;
  if(!IsOK())
  {} // not previously loaded
//  else if(IsLocked() && !HasLock())  // 9/7/16, continue to check even if locked
//  {} // someone else has it locked, cannot load
  else if(bForceReload)
  {
    bRtn = m_lab.ReloadFile()
        && m_book.LoadFile(GetMessageBookFileName());
  }
  else
  {
    bRtn = m_lab.CheckFileModification(true) &&
      m_book.CheckReload();
  }
  return bRtn;
}
#if 0

//  OS-679 - removed because access time is unreliable
bool CVolume::SetInUseOnTimer(int nms)
{
  m_nCountDown -= nms;
  bool bRtn = true;
  if(m_nCountDown <= 0)
  {
    bRtn = SetInUse();
    m_nCountDown = LOCK_READ_MS;
  }
  return bRtn;
}
#endif
wxString CVolume::GetLockUser()
{
  wxString sRtn;
  if(IsLocked())
  {
    sRtn = m_lock.GetLockUser();
  }
  else
  {
    sRtn = nwxLock::GetLockUser(m_sPath);
  }
  return sRtn;
}

#if 0
//  OS-679 - removed because access time is unreliable

bool CVolume::AccessedSince(int nSeconds)
{
  bool bRtn = m_lockRead.AccessedSince(nSeconds);
  return bRtn;
}
#endif
void CVolume::_InitError()
{
  m_sLastError = "This " Volume_string ", ";
  m_sLastError.Append(GetVolumeName());
  m_sLastError.Append(",\n");  
}
bool CVolume::Lock()
{
  bool bRtn = false;
  if(IsReadOnly() || 
    !wxFileName::IsFileWritable(this->GetLabSettingsFileName()) )
  {
    _InitError();
    m_sLastError.Append(
      "cannot be locked because it is read only,\n"
         "and cannot be modified");
  }
#if 0
  //  OS-679 - removed because access time is unreliable

  else if((!m_bIgnoreReadLock) && AccessedSince(30))
  {
    _InitError();
    m_sLastError.Append(
      
        "cannot be locked because it in use.\n"
        "Please try again later.");
  }
#endif
  else if(m_lock.Lock(m_sPath))
  {
    CheckReload();
    bRtn = true;
  }
  else
  {
    wxString sUser = m_lock.GetLockUser();
    _InitError();
    if(sUser.Len())
    {
      m_sLastError.Append("is currently locked by ");
      m_sLastError.Append(sUser);
    }
    else
    {
      m_sLastError.Append(
        "cannot be locked, possibly\n"
           "due to file permissions");
    }
  }
  return bRtn;
}
bool CVolume::IsLocked()
{
  wxString sUser = nwxLock::GetLockUser(m_sPath);
  bool bRtn = !sUser.IsEmpty();
  return bRtn;
}
bool CVolume::HasLock()
{
  bool bRtn = IsOK() && !IsReadOnly() && m_lock.HaveLock();
  return bRtn;
}
bool CVolume::ReleaseLock()
{
  bool bRtn = m_lock.ReleaseLock();
  m_bIgnoreReadLock = false;
  return bRtn;
}
wxString CVolume::GetPathPrefix(const wxString &_sDir)
{
  wxString sDir(_sDir);
  wxString sBase = nwxFileUtil::BaseName(sDir);
  nwxFileUtil::EndWithSeparator(&sDir);
  sDir.Append(sBase);
  sDir.Append(wxChar('_'));
  return sDir;
}


//******************************************************  CVolumes

CVolumes::~CVolumes() 
{
  _Cleanup();
}

CVolumes::CVolumes() :
  m_dtLastMod((time_t)0), 
  m_nCountDown(0),
  m_bBase(false)
{
  _SetupPath();
  CheckReload(true);
}

bool CVolumes::CheckReload(bool bForce)
{
  bool bRtn = false;
  bool bReloadAll = bForce;
  wxDateTime dt((time_t)0);
  if(!bForce)
  {
    if(wxFileName::DirExists(m_sDirVolume))
    {
      wxFileName fn(m_sDirVolume);
      dt = fn.GetModificationTime();
      bReloadAll = (dt != m_dtLastMod);
    }
  }
  if(!bReloadAll)
  {
    MapVolume::iterator itr;
    CVolume *pVol;
    bool bReloadAll = false;
    for(itr = m_mapVol.begin();
      (!bReloadAll) && (itr != m_mapVol.end());
      ++itr)
    {
      pVol = itr->second;
      if(pVol->CheckReload(false))
      {
        bRtn = true;
        if(pVol->GetVolumeName() != itr->first)
        {
          // we had a name change, reload all
          bReloadAll = true;
        }
      }
    }
  }
  if(bReloadAll)
  {
    _Load();
    if(wxDir::Exists(m_sDirVolume))
    {
      wxFileName fn(m_sDirVolume);
      m_dtLastMod =  fn.GetModificationTime();
    }
  }
  if(bRtn)
  {
    m_nCountDown = 0;
  }
  return bRtn;
}
void CVolumes::RefreshLocksOnTimer(int nms)
{
  m_nCountDown -= nms;
  if(m_nCountDown < 0)
  {
    m_nCountDown = LOCK_MS;
    RefreshLocks();
  }
}

CVolume *CVolumes::Find(const wxString &sName)
{
  pair<MapVolume::iterator, MapVolume::iterator> pitr =
    m_mapVol.equal_range(sName);
  CVolume *pRtn =
    (pitr.first == pitr.second)
    ? NULL
    : pitr.first->second;
  return pRtn;
}

size_t CVolumes::FindAll(
  vector<CVolume *> *pvVol, const wxString &sName)
{
  pair<MapVolume::iterator, MapVolume::iterator> pitr =
    m_mapVol.equal_range(sName);
  MapVolume::iterator itr;
  size_t nRtn;
  pvVol->clear();
  for(itr = pitr.first; itr != pitr.second; ++itr)
  {
    pvVol->push_back(itr->second);
  }
  nRtn = pvVol->size();
  return nRtn;
}

void CVolumes::_SetErrorNameExists(const wxString &s)
{
  m_sLastError = AVolume_string " with the name, ";
  m_sLastError.Append(s);
  m_sLastError.Append(", exists.");
}
CVolume *CVolumes::Create(
  const wxString &sCopyFrom, const wxString &_sName)
{
  wxString sPath;
  wxString sName(_sName);
  MapVolume::iterator itrm;
  CVolume *pRtn(NULL);
  CVolume *pCopyFrom;

  nwxString::Trim(&sName);
  if(sName.IsEmpty())
  {
    m_sLastError = VOLUME_STRING " name has not been specified.";
  }
  else if(Find(sName) != NULL)
  {
    _SetErrorNameExists(sName);
  }
  else if( (pCopyFrom = Find(sCopyFrom)) == NULL )
  {
    m_sLastError = "Cannot create new " Volume_string " from ";
    m_sLastError.Append(sCopyFrom);
    m_sLastError.Append(".\nThe " Volume_string " was not found.");
    wxASSERT_MSG(0,m_sLastError);
  }
  else if( pCopyFrom->IsLocked() && !pCopyFrom->HasLock() )
  {
    m_sLastError = "Cannot create new " Volume_string " from ";
    m_sLastError.Append(sCopyFrom);
    m_sLastError.Append("\nbecause it is locked by ");
    m_sLastError.Append(pCopyFrom->GetLockUser());
    wxASSERT_MSG(0,m_sLastError);
  }
  else if(_BuildNewPath(pCopyFrom,&sPath))
  {
    auto_ptr<CVolume> apRtn(new CVolume(sPath));

    if(!(apRtn->IsOK() && apRtn->InitNewVolume(sName)))
    {
        m_sLastError = 
          "Could not create new " Volume_string ".\n"
          "Please check space on disk drive";
    }
    else if((itrm = m_mapVol.insert(MapVolume::value_type(sName,apRtn.get()))) == m_mapVol.end())
    {
      m_sLastError =
        "Could not add new " Volume_string " to the set.\n"
        "System may not have sufficient memory.";
    }
    else
    {
      _SetModified();
      pRtn = apRtn.release();
    }
  }
  return pRtn;
}

bool CVolumes::Remove(CVolume *pVolume)
{
  MapVolume::iterator itr = _FindVolume(pVolume);
  bool bRtn = false;
  if(!(pVolume->IsReadOnly() || pVolume->Lock()))
  {
    ; // do not remove
  }
  else if(itr != m_mapVol.end())
  {
    _RemoveFiles(pVolume->GetPath());
    if(!_HasFiles(pVolume->GetPath()))
    {
      bRtn = true;
      delete pVolume;
      m_mapVol.erase(itr);
      _SetModified();
    }
  }
  return bRtn;
}

bool CVolumes::Rename(CVolume *p,const wxString &_sNewName)
{
  bool bRtn = false;
  wxString sOldName = p->GetVolumeName();
  wxString sNewName(_sNewName);
  nwxString::Trim(&sNewName);
  if(!p->Lock())
  {
    m_sLastError = "Cannot rename " Volume_string ",\n";
    m_sLastError.Append(p->GetVolumeName());
    m_sLastError.Append(".\nThis " Volume_string " cannot be locked.");
  }
  else if(sNewName.IsEmpty())
  {
    m_sLastError = "Cannot rename " Volume_string ",\n";
    m_sLastError.Append(p->GetVolumeName());
    m_sLastError.Append(".\nA new name has not been specified.");
  }
  else if(sOldName == sNewName)
  {
    bRtn = true; // no change
  }
  else if(Find(sNewName) != NULL)
  {
    _SetErrorNameExists(sNewName);
  }
  else
  {
    MapVolume::iterator itr = _FindVolume(p);
    if(itr != m_mapVol.end())
    {
      m_mapVol.erase(itr);
      _SetModified();
      p->SetVolumeName(sNewName);
      itr = m_mapVol.insert(MapVolume::value_type(sNewName,p));
      if( (itr != m_mapVol.end()) && p->Save() )
      {
        bRtn = true;
      }
      else
      {
        m_sLastError = "An error occurred when attempting to rename " Volume_string ".";
        itr = m_mapVol.insert(MapVolume::value_type(sOldName,p));
        if(itr == m_mapVol.end())
        {
          delete p;
          m_sLastError.Append("\nThis " Volume_string " has been removed from the list");
        }
        else
        {
          p->SetVolumeName(sOldName);
        }
      }
    }
  }
  return bRtn;
}

size_t CVolumes::GetNames(SetVolumeNames *pSet) const
{
  pSet->clear();
  for(MapVolume::const_iterator itr = m_mapVol.begin();
    itr != m_mapVol.end();
    ++itr)
  {
    pSet->insert(itr->first);
  }
  return pSet->size();
}

const wxChar *CVolumes::g_psNames[] =
{
  LAB_SETTINGS_FILE,
  ACCESS_FILE,
  MESSAGE_BOOK_FILE,
  STD_SETTINGS_FILE,
  NULL
};

bool CVolumes::_RemoveFiles(const wxString &sDirName)
{
  bool bRtn = true;
  {
    // first just try to kill the whole directory
    nwxKillDir killDir(sDirName);
  }
  if(wxDir::Exists(sDirName))
  {
    // didn't remove whole directory...
    // remove essential files
    wxString sPrefix = CVolume::GetPathPrefix(sDirName);
    wxString sFile;
    for(const wxChar **pp = g_psNames; (*pp) != NULL; ++pp)
    {
      sFile = sPrefix;
      sFile.Append(*pp);
      if(wxFileName::IsFileReadable(sFile) && !::wxRemoveFile(sFile))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}


bool CVolumes::_HasFiles(const wxString &sDirName)
{
  wxString sPrefix = CVolume::GetPathPrefix(sDirName);
  wxString sFile;
  bool bRtn = true;
  for(const wxChar **pp = g_psNames;((*pp) != NULL) && bRtn; ++pp)
  {
    sFile = sPrefix;
    sFile.Append(*pp);
    if(!wxFileName::IsFileReadable(sFile))
    {
      bRtn = false;
    }
  }
  return bRtn;
}


void CVolumes::_Cleanup()
{
  MapVolume::iterator itr;
  for(itr = m_mapVol.begin();
    itr != m_mapVol.end();
    itr = m_mapVol.begin())
  {
    delete itr->second;
    m_mapVol.erase(itr);
  }
  _SetModified();
}
void CVolumes::_SetModified()
{
  m_asVolumeNames.Empty();
}
void CVolumes::_LoadDir(const wxString &sDirName)
{
  if(wxDir::Exists(sDirName))
  {
    wxDir dir(sDirName);
    if(dir.IsOpened())
    {
      dir.Traverse(*this);
    }
  }
}
void CVolumes::_SetupKitVolumes()
{
  m_asKits.Clear();
  m_mapKitVolume.clear();
  for(MapVolume::iterator itr = m_mapVol.begin();
    itr != m_mapVol.end();
    ++itr)
  {
    CVolume *pVol = itr->second;
    const wxString &sKit = pVol->GetKitName();
    m_mapKitVolume.insert(
      map<wxString,CVolume *>::value_type(sKit,pVol));
  }
}
const wxArrayString &CVolumes::GetVolumeNames() const
{
  if(m_asVolumeNames.IsEmpty())
  {
    SetVolumeNames setNames;
    size_t n = GetNames(&setNames);
    m_asVolumeNames.Alloc(n);
    for(SetVolumeNames::iterator itr = setNames.begin();
      itr != setNames.end();
      ++itr)
    {
      m_asVolumeNames.Add(*itr);
    }
  }
  return m_asVolumeNames;
}
const wxArrayString &CVolumes::GetKits() const
{
  if(m_asKits.IsEmpty())
  {
    CPersistKitList *pKits = mainApp::GetKitList();
    map<wxString,CVolume *>::const_iterator itr;
    for(itr = m_mapKitVolume.begin();
      itr != m_mapKitVolume.end();
      ++itr)
    {
      const wxString &sKit(itr->first);
      if(pKits->GetLsArray(sKit) != NULL)
      {
        m_asKits.Add(sKit);
      }

      else
      {
        wxString sError("Cannot find kit: ");
        sError.Append(sKit);
        sError.Append(" in ILSandLadderInfo.xml");
        wxASSERT_MSG(0,sError);
        mainApp::LogMessage(sError);
      }
    }
  }
  return m_asKits;
}

wxDirTraverseResult CVolumes::OnDir(const wxString &dirname)
{
  if(_HasFiles(dirname))
  {
    auto_ptr<CVolume> apVol(new CVolume(dirname,m_bBase));
    if(apVol->IsOK())
    {
      wxString s = apVol->GetVolumeName();
      MapVolume::iterator itr;
      if(s.IsEmpty())
      {
        m_sLastError.Append("no " Volume_string " name for ");
        m_sLastError.Append(dirname);
        m_sLastError.Append("\n");
      }
      else
      {
        itr = m_mapVol.insert(MapVolume::value_type(s,apVol.get()));
        if(itr != m_mapVol.end())
        {
          apVol.release();
        }
      }
    }
  }
  return wxDIR_IGNORE;
}
wxDirTraverseResult CVolumes::OnFile(const wxString &)
{
  return wxDIR_CONTINUE;
}
void CVolumes::RefreshLocks()
{
  for(MapVolume::iterator itr = m_mapVol.begin();
    itr != m_mapVol.end();
    ++itr)
  {
    itr->second->RefreshLock();
  }
}


MapVolume::iterator CVolumes::_FindVolume(CVolume *pVolume)
{
  pair<MapVolume::iterator,MapVolume::iterator> pitr = 
    m_mapVol.equal_range(pVolume->GetVolumeName());
  MapVolume::iterator itr;
  if(pitr.first == m_mapVol.end())
  {
    // name has changed but not in map,
    // this is a problem, so we will check entire list
    pitr.first = m_mapVol.begin();
    wxASSERT_MSG(0,"_FindVolume() could not find CVolume from name.");
  }
  for(itr = pitr.first;
    (itr != pitr.second) && (itr->second != pVolume);
    ++itr) {}
  if(itr == pitr.second)
  {
    itr = m_mapVol.end();
    wxASSERT_MSG(0,"_FindVolume() failed");
    m_sLastError = "Could not find " Volume_string " in list.";
  }
  return itr;
};

bool CVolumes::_BuildNewPath(const CVolume *pCopyFrom, wxString *psPath)
{
  bool bRtn = false;
  if(m_sDirVolume.IsEmpty())
  {
    m_sLastError = "Cannot find parent directory for new " Volume_string "s";
    wxASSERT_MSG(0,m_sLastError);
  }
  else
  {
    ConfigDir *pDir = mainApp::GetConfig();
    wxString sPath = m_sDirVolume;
    wxString sNewPath;
    wxChar suff;
    wxChar A('A');
    wxChar Z('Z');
    wxDateTime dt;
    bool bDone = false;

    dt.SetToCurrent();
    nwxFileUtil::EndWithSeparator(&sPath);
    sPath.Append(dt.Format("V-%Y%m%d-%H%M%S"));
    sNewPath = sPath;
    for(suff = A;
      ((bDone = wxDir::Exists(sNewPath)) == true) && (suff <= Z);
      suff++)
    {
      sNewPath = sPath;
      sNewPath.Append(wxChar('-'));
      sNewPath.Append(suff);
    }
    if(suff > Z)
    {
      m_sLastError = "Cannot find name for new " Volume_string " directory in\n";
      m_sLastError.Append(pDir->GetExeVolumePath());
      wxASSERT_MSG(0,m_sLastError);
    }
    else if(!nwxFileUtil::MkDir(sNewPath))
    {
      m_sLastError = "Cannot create directory for new volume:\n";
      m_sLastError += sNewPath;
      m_sLastError += "\nYou may not have access privileges";
    }
    else
    {
      // now copy files
      wxString sToPrefix = CVolume::GetPathPrefix(sNewPath);
      wxString sFromPrefix = CVolume::GetPathPrefix(pCopyFrom->GetPath());
      wxString sTo;
      wxString sFrom;
      bRtn = true;
      for(const wxChar **pp = g_psNames; bRtn && ((*pp) != NULL); ++pp)
      {
        sTo = sToPrefix;
        sTo.Append(*pp);
        sFrom = sFromPrefix;
        sFrom.Append(*pp);
        if(!::wxCopyFile(sFrom,sTo))
        {
          bRtn = false;
          m_sLastError = "Cannot copy\n";
          m_sLastError.Append(sFrom);
          m_sLastError.Append("\nto\n");
          m_sLastError.Append(sTo);
          wxASSERT_MSG(0,m_sLastError);
        }
      }
      if(bRtn)
      {
        *psPath = sNewPath;
      }
    }
  }
  return bRtn;
}


void CVolumes::_SetupPath()
{
  ConfigDir *pDir = mainApp::GetConfig();
  m_sDirBase = pDir->GetExeVolumePath();
  m_sDirVolume = pDir->GetSitePath();
  nwxFileUtil::EndWithSeparator(&m_sDirVolume);
  m_sDirVolume.Append("Volumes");
}

#ifdef __WXDEBUG__
void CVolumes::UnitTest()
{
  CVolumes vols;
  SetVolumeNames setNames;
  wxString sText;
  size_t nSize = vols.GetNames(&setNames);
  wxString sRO;
  wxString sLock;
  CVolume *pVolume;
  sText.Alloc(4096);
  sText.Append(wxString::Format(
    "CVolumes::UnitTest()\nVolume count: %d\n\n",(int)nSize));
  for(SetVolumeNames::iterator itr = setNames.begin();
    itr != setNames.end();
    ++itr)
  {
    pVolume = vols.Find(*itr);
    sRO = (pVolume == NULL)
      ? "X"
      : (pVolume->IsReadOnly() ? "R" : "RW");
    sLock = (sRO == "RW")
      ? wxString((pVolume->Lock() ? "Lock" : "NO-LOCK!"))
      : pVolume->GetLockUser();
    sText.Append(wxString::Format(
      "%ls %ls %ls\n",
      (*itr).wc_str(),
      sRO.wc_str(),
      sLock.wc_str() ));    
  }
  // mainApp::ShowAlert(sText,NULL);
  pVolume = vols.Create("[Profiler Plus]","UnitTest");
  sText.Clear();
  if(pVolume == NULL)
  {
    sText.Append("Cannot create " Volume_string ", \"UnitTest\"\n");
  }
  else
  {
    if(!pVolume->Lock())
    {
      sText.Append("Cannot lock " Volume_string ", \"UnitTest\"\n");
    }
    else if(!vols.Remove(pVolume))
    {
      sText.Append("Cannot Remove " Volume_string ", \"UnitTest\"\n");
    }
  }
  wxASSERT_MSG(sText.IsEmpty(),sText);
}
#endif
