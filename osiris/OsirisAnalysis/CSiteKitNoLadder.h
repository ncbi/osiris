//  NEEDED : in CSiteKitNoLadderCollection - Ability to lock by name and to lock one and only one at a time
//

#ifndef __C_SITE_KIT_H__
#define __C_SITE_KIT_H__

#include <wx/dir.h>
#include "nwx/mapptr.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxString.h"
#include "nwx/nwxGlobalObject.h"
#include "nwx/nwxLock.h"
#include "nwx/nwxLog.h"
#include "wxIDS.h"
#include "OsirisVersion.h"
#define SK_COPY(z)  z = x.z
#define SK_EQ(z) z == x.z

class wxArrayString;

//************************************************** class CSiteKitChannelDetails

class CSiteKitChannelDetails : public nwxXmlPersist
{
public:
  CSiteKitChannelDetails() : 
    nwxXmlPersist(),
    m_nChannelDisplay(0),
    m_nChannelFromFile(0)
  {
    RegisterAll(true);
  }
  CSiteKitChannelDetails(
    int nChannelDisplay, 
    int nChannelFromFile, 
    const wxString &sDye, 
    const wxString &sColor) : nwxXmlPersist(),
      m_sDyeName(sDye),
      m_sColor(sColor),
      m_nChannelDisplay(nChannelDisplay),
      m_nChannelFromFile(nChannelFromFile)
  {
    RegisterAll(true);
  }
  CSiteKitChannelDetails(const CSiteKitChannelDetails &x) :
    nwxXmlPersist()
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~CSiteKitChannelDetails()
  {}
  CSiteKitChannelDetails &operator = (const CSiteKitChannelDetails &x)
  {
    SK_COPY(m_sDyeName);
    SK_COPY(m_sColor);
    SK_COPY(m_nChannelDisplay);
    SK_COPY(m_nChannelFromFile);
    return *this;
  }
  bool operator == (const CSiteKitChannelDetails &x) const
  {
    bool bRtn = SK_EQ(m_sDyeName) &&
      SK_EQ(m_sColor) &&
      SK_EQ(m_nChannelDisplay) &&
      SK_EQ(m_nChannelFromFile);
    return bRtn;
  }
  bool operator != (const CSiteKitChannelDetails &x) const
  {
    return !((*this) == x);
  }
  //  Set
  void SetChannelDisplay(int n)
  {
    m_nChannelDisplay = n;
  }
  void SetChannelFromFile(int n)
  {
    m_nChannelFromFile = n;
  }
  void SetDyeName(const wxString &s)
  {
    m_sDyeName = s;
  }
  void SetColor(const wxString &s)
  {
    m_sColor = s;
  }

  //  Get
  int GetKey() const
  {
    return m_nChannelDisplay;
  }
  int GetChannelDisplay() const
  {
    return m_nChannelDisplay;
  }
  int GetChannelFromFile() const
  {
    return m_nChannelFromFile;
  }
  const wxString &GetDyeName() const
  {
    return m_sDyeName;
  }
  const wxString &GetColor() const
  {
    return m_sColor;
  }
  bool IsOK() const
  {
    bool bRtn = !(
      m_sDyeName.IsEmpty() ||
      m_sColor.IsEmpty() ||
      (m_nChannelDisplay < 1) ||
      (m_nChannelDisplay > CHANNEL_MAX) ||
      (m_nChannelFromFile < 0)
      );
    return bRtn;
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterInt(wxS("ChannelDisplay"), &m_nChannelDisplay);
    RegisterInt(wxS("ChannelFromFile"), &m_nChannelFromFile);
    RegisterWxString(wxS("DyeName"), &m_sDyeName);
    RegisterWxString(wxS("Color"), &m_sColor);
  }
private:
  wxString m_sDyeName;
  wxString m_sColor;     // Should be implied by DYE name
  int m_nChannelDisplay;
  int m_nChannelFromFile;  // FSA or HID Channel number
};

//************************************************** class CSiteKitNoLadder

class CSiteKitNoLadder : public nwxXmlPersist
{
public:
  CSiteKitNoLadder() : nwxXmlPersist(), m_pLock(NULL)
  {
    RegisterAll(true);
    _initLocal();
  }
  CSiteKitNoLadder(const CSiteKitNoLadder &x) :
    nwxXmlPersist()
  {
    RegisterAll(true);
    *this = x;
  }
  static void SetSiteIndicatorOnName(wxString *ps)
  {
    if (ps->EndsWith(g_sNAME_SITE_INDICATOR))
    {
      ps->Remove(ps->Len() - g_sNAME_SITE_INDICATOR.Len());
      nwxString::Trim(ps);
    }
  }
  static void ClearSiteIndicatorOnName(wxString *ps)
  {
    if(!ps->EndsWith(g_sNAME_SITE_INDICATOR))
    {
      nwxString::Trim(ps);
      ps->Append(g_sNAME_SITE_INDICATOR);
    }
  }
  virtual ~CSiteKitNoLadder()
  {
    Unlock();
    m_mapChannelInfo.Clear();
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sROOT_NODE;
  }
  const wxString &GetFileName() const
  {
    if (m_sFileName.IsEmpty())
    {
      return GetLastFileName();
    }
    return m_sFileName;
  }
  CSiteKitNoLadder &operator = (const CSiteKitNoLadder &x)
  {
    const wxString &sThisFile = GetFileName();
    const wxString &sXfile = x.GetFileName();
    if (sXfile != sThisFile)
    {
      Unlock();
      m_sFileName = sXfile;
    }
    SK_COPY(m_sKitName);
    SK_COPY(m_sOSIRIS_Version);
    SK_COPY(m_dLastMod);
    SK_COPY(m_sILSgroup);
    SK_COPY(m_sILS);
    SK_COPY(m_mapChannelInfo);
    return *this;
  }
  bool operator == (const CSiteKitNoLadder &x) const
  {
    // skipping m_sOSIRIS_Version and m_dLastMod
    // when comparing
    bool bRtn = SK_EQ(m_sKitName) &&
      SK_EQ(m_sILSgroup) &&
      SK_EQ(m_sILS) &&
      SK_EQ(m_mapChannelInfo);
    return bRtn;
  }
  bool operator != (const CSiteKitNoLadder &x)
  {
    return !((*this) == x);
  }
  bool IsOK() const
  {
    bool bRtn = false;
    if (m_sKitName.IsEmpty()) {}
    else if (m_sILSgroup.IsEmpty()) {}
    else if (!m_mapChannelInfo.Size()) {}
    else if (!IsChannelListComplete()) {}
    else
    {
      std::map<int, CSiteKitChannelDetails *>::const_iterator itr;
      const std::map<int, CSiteKitChannelDetails *> *pmapChannel = m_mapChannelInfo.Get();
      bRtn = true;
      for(itr = pmapChannel->begin();
        (itr != pmapChannel->end()) && bRtn;
        ++itr)
      {
        if (!itr->second->IsOK())
        {
          bRtn = false;
        }
      }
    }
    return bRtn;
  }
  virtual bool SaveFile(const wxString &sFileName)
  {
    _initLocal();
    return nwxXmlPersist::SaveFile(sFileName);
  }
  bool Save()
  {
    const wxString &sFileName(GetFileName());
    bool bRtn = sFileName.IsEmpty() ? false : SaveFile(sFileName);
    return bRtn;
  }
  static bool IsChannelListComplete(const std::map<int, CSiteKitChannelDetails *> *pmap)
  {
    int nCount = (int)pmap->size();
    bool bRtn = true;
    for (int i = 1; (i <= nCount) && bRtn; ++i)
    {
      if(pmap->find(i) == pmap->end())
      {
        bRtn = false;
      }
    }
    return bRtn;
  }
  bool IsChannelListComplete() const
  {
    const std::map<int, CSiteKitChannelDetails *> *pmap = m_mapChannelInfo.Get();
    return IsChannelListComplete(pmap);
  }
  void ClearChannels()
  {
    m_mapChannelInfo.Clear();
  }
  virtual void Init()
  {
    nwxXmlPersist::Init();
    _initLocal();
  }
  // Set
  void SetChannel(const CSiteKitChannelDetails &x)
  {
    m_mapChannelInfo.InsertOrReplace(x);
  }
  void SetKitName(const wxString &s)
  {
    m_sKitName = s;
  }
  void SetILSgroup(const wxString &s)
  {
    m_sILSgroup = s;
  }
  void SetILS(const wxString &s)
  {
    m_sILS = s;
  }
  // Get
  const wxString &GetKitName() const
  {
    return m_sKitName;
  }
  const wxString &GetKey() const
  {
    return GetKitName();
  }
  const wxString &GetILSgroup() const
  {
    return m_sILSgroup;
  }
  const wxString &GetILS() const
  {
    return m_sILS;
  }
  int GetChannelCount() const
  {
    return (int)(m_mapChannelInfo.Get()->size());
  }
  const CSiteKitChannelDetails *GetChannel(int n) const
  {
    const CSiteKitChannelDetails *p = m_mapChannelInfo.Find(n);
    return p;
  }
  std::map<int, CSiteKitChannelDetails *> *GetChannelInfo()
  {
    return m_mapChannelInfo.Get();
  }
  bool UnLock()
  {
    bool bRtn = false;
    if (m_pLock != NULL)
    {
      bRtn = m_pLock->IsLocked() ? m_pLock->ReleaseLock() : false;
      delete m_pLock;
      m_pLock = NULL;
    }
    return bRtn;
  }
  bool IsLocked()
  {
    return (m_pLock == NULL) ? false : m_pLock->IsLocked();
  }
  int GetLastLockError()
  {
    return (m_pLock == NULL) ? -1 : m_pLock->GetLastError();
  }
  bool Lock()
  {
    bool bRtn = false;
    if (IsLocked())
    {
      bRtn = true;
    }
    else
    {
      const wxString &sFile = GetFileName();
      if (!sFile.IsEmpty())
      {
        wxFileName fn(sFile);
        wxString sDir = fn.GetPath();
        if (m_pLock == NULL)
        {
          m_pLock = new nwxLock();
        }
        bRtn = m_pLock->Lock(fn.GetPath());
      }
    }
    return bRtn;
  }

protected:
  virtual void RegisterAll(bool = false)
  {
    m_mapChannelInfo.SetNodeName(wxS("Channel"));
    RegisterWxString(wxS("KitName"), &m_sKitName);
    RegisterWxString(wxS("Version"), &m_sOSIRIS_Version);
    RegisterWxDateTime(wxS("LastModified"), &m_dLastMod);
    RegisterWxString(wxS("ILSgroup"), &m_sILSgroup);
    RegisterWxString(wxS("ILSdefault"), &m_sILS);
    Register(wxS("Channels"), &m_mapChannelInfo);
  }
private:
  void _initLocal()
  {
    m_sFileName.Empty();
    m_sOSIRIS_Version = OSIRIS_VERSION;
    m_dLastMod = wxDateTime::Now();
  }
  static const wxString g_sROOT_NODE;
  static const wxString g_sNAME_SITE_INDICATOR;
  nwxLock *m_pLock;
  wxString m_sKitName;
  wxString m_sOSIRIS_Version;
  wxString m_sILSgroup;
  wxString m_sILS;
  wxString m_sFileName;
  wxDateTime m_dLastMod;
  TnwxXmlPersistMap<int, CSiteKitChannelDetails> m_mapChannelInfo;
};

class CSiteKitNoLadderCollection : public wxDirTraverser
{
  //  needs update kit, new kit, remove kit

private:
  CSiteKitNoLadderCollection() : m_nErrorCount(0)
  {
    Load();
  };
protected:
  virtual ~CSiteKitNoLadderCollection()
  {
    _cleanup();
  }
public:
  static wxString GetTopDirectory(bool bAppendSeparator = false);
  virtual wxDirTraverseResult OnDir(const wxString &dirname);
  virtual wxDirTraverseResult OnFile(const wxString &filename);
  size_t GetNames(wxArrayString *pasNames, bool bWithSite = false);
  bool Load(bool bForce = false);
  bool RemoveKit(const CSiteKitNoLadder &);
  bool CreateKit(const CSiteKitNoLadder &);
  bool UpdateKit(const CSiteKitNoLadder &);

  int GetErrorCount()
  {
    return m_nErrorCount;
  }
  const CSiteKitNoLadder *FindByName(const wxString &sKitName)
  {
    _setupNames();
    NAME_TO_KIT::const_iterator itr = m_mapKitName.find(sKitName);
    return (itr == m_mapKitName.end()) ? NULL : itr->second;
  }
  size_t GetSize() const
  {
    return m_mapFileNameKit.size();
  }
  const wxString &GetLastMessage() const
  {
    // returns error message from last unsuccessful operation
    // (Remove, Create, Update)
    return m_sLastMessage;
  }
private:
  typedef std::map<wxString, CSiteKitNoLadder *, nwxStringLessFileName> FILE_TO_KIT;
  typedef mapptr<wxString, CSiteKitNoLadder, nwxStringLessFileName> pFILE_TO_KIT;
  typedef std::map<wxString, CSiteKitNoLadder *, nwxStringLessNoCase> NAME_TO_KIT;
  CSiteKitNoLadder * _findByFileName(const wxString &sFileName);
  void _setupNames();
  bool _checkReload();
  bool _hasFiles(const wxString &dirname);
  void _cleanup();
  void _clearErrorCount()
  {
    m_nErrorCount = 0;
  }
  void _incrementErrorCount()
  {
    m_nErrorCount++;
  }
  wxString m_sLastMessage;

  // file name to kit, pointers are 'delete'd upon cleanup
  FILE_TO_KIT m_mapFileNameKit;

  // kit name to kit, same pointer as above,
  //  cleanup simply wipes out the std::map
  NAME_TO_KIT m_mapKitName;

  int m_nErrorCount;
  static const wxChar *g_psNames[]; // file names w/in volume
  static const wxString g_sKITSdir;
  nwxDECLARE_GLOBAL_OBJECT(CSiteKitNoLadderCollection)
};
#undef SK_EQ
#undef SK_COPY

#ifdef __C_SITE_KIT_NO_LADDER_CPP__

const wxString CSiteKitNoLadder::g_sROOT_NODE(wxS("SiteKitNoLadder"));

#endif

#endif
