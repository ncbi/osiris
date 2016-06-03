#ifndef __C_ARTIFACT_LABELS_H__
#define __C_ARTIFACT_LABELS_H__

#include <wx/string.h>
#include <wx/regex.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

class CArtifactREcache
{
public:
  CArtifactREcache() 
  {
    g_nCount++;
  }
  virtual ~CArtifactREcache();
  static wxRegEx *GetRE(const wxString &s);
private:
  static int g_nCount;
  static std::map<wxString,wxRegEx *> g_map;
};


class CArtifactGroup : public nwxXmlPersist
{
public:
  CArtifactGroup();
  CArtifactGroup(const CArtifactGroup &obj);
  void Copy(const CArtifactGroup &obj);
  virtual ~CArtifactGroup() {}
  const wxString &GetLabel() const
  {
    return m_sLabel;
  }
  const wxString &GetDisplay() const
  {
    return m_sDisplay;
  }
  int GetPriority() const
  {
    return m_nPriority;
  }
  void SetPriority(int n)
  {
    m_nPriority = n;
  }
  void SetDisplay(const wxChar *p)
  {
    m_sDisplay = p;
  }
  bool IsStringMatch(const wxString &s) const;
  bool IsMsgTypeMatch(const wxString &s) const;
  void TransferRE(const CArtifactGroup &obj) const;
  bool SameAs(const CArtifactGroup &s) const
  {
    // return true if any of the two class members 
    // that can be changed are not equal
    return 
      (m_nPriority == s.GetPriority()) &&
      (m_sDisplay == s.GetDisplay());
  }

protected:
  virtual void RegisterAll(bool = false);
private:
  void _buildRegEx() const;
  nwxXmlIOPersistVectorWxString m_ios;
  std::vector<wxString> m_vsMsgName;
  std::vector<wxString> m_vsSearchString;
  mutable std::vector<wxRegEx *> m_pvSearchRE;
  wxString m_sLabel;
  wxString m_sDisplay;
  int m_nPriority;
};


class CArtifactLabels : public nwxXmlPersist
{
public:
  CArtifactLabels();
  virtual ~CArtifactLabels();
  virtual bool LoadFile(const wxString &sFileName, bool bLock);
  bool Save();
  virtual const wxString &RootNode(void) const
  {
    return g_sROOT;
  }
  virtual void Init()
  {
    m_io.Cleanup();
    nwxXmlPersist::Init();
    m_bOK = true;
  }
  virtual void Init(void *p)
  {
    nwxXmlPersist::Init(p);
    if(p == (void *) this)
    {
      m_io.Cleanup();
    }
  }
  bool IsOK() const
  {
    return m_bOK;
  }
  void Sort();
  const wxString &GetDisplayFromString(const wxString &sArtifactString) const;
  const wxString &GetDisplayFromMsgType(const wxString &sMsgType) const;
  void GetList(vector<CArtifactGroup *> *pv) const;
  void SetList(const std::vector<CArtifactGroup *> &vec);
  bool CompareList(
    const std::vector<CArtifactGroup *> &vec) const;
  static CArtifactREcache *g_pCache;
protected:
  virtual void RegisterAll(bool = false)
  {
    Register(wxT("SubGroup"),&m_io,&m_vector);
  }
private:
  TnwxXmlIOPersistVector<CArtifactGroup> m_io;
  std::vector<CArtifactGroup *> m_vector;
  bool m_bOK;
  bool m_bModified;
  CArtifactREcache m_RegExCache;
  static const wxString g_sROOT;
  static const wxString g_sDEFAULT_LABEL;
};


#endif