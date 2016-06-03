
#include "mainApp.h"
#include "ConfigDir.h"
#include "CArtifactLabels.h"
#include "nwx/vectorSort.h"
#include "nwx/mapptr.h"


CArtifactREcache::~CArtifactREcache()
{
  g_nCount--;
  if(!g_nCount)
  {
    mapptr<wxString,wxRegEx>::cleanup(&g_map);
  }
}
wxRegEx *CArtifactREcache::GetRE(const wxString &s)
{
  std::map<wxString,wxRegEx *>::iterator itr = g_map.find(s);
  wxRegEx *pRtn;
  if(itr == g_map.end())
  {
    pRtn = new wxRegEx(s,wxRE_ICASE | wxRE_ADVANCED );
    if(!pRtn->IsValid())
    {
      delete pRtn;
      pRtn = NULL;
      wxString sMsg(wxT("Invalid regular expression in artifact labels: "));
      sMsg += s;
      mainApp::LogMessage(sMsg);
    }
    g_map.insert(std::pair<wxString,wxRegEx *>(s,pRtn));
  }
  else
  {
    pRtn = itr->second;
  }
  return pRtn;
}

int CArtifactREcache::g_nCount = 0;
std::map<wxString,wxRegEx *> CArtifactREcache::g_map;


CArtifactGroup::CArtifactGroup() : nwxXmlPersist()
{
  RegisterAll(true);
}
CArtifactGroup::CArtifactGroup(const CArtifactGroup &obj) : nwxXmlPersist()
{
  RegisterAll(true);
  Copy(obj);
}
void CArtifactGroup::Copy(const CArtifactGroup &obj)
{
  m_vsSearchString = obj.m_vsSearchString;
  m_pvSearchRE = obj.m_pvSearchRE;
  m_vsMsgName = obj.m_vsMsgName;
  m_sLabel = obj.m_sLabel;
  m_sDisplay = obj.m_sDisplay;
  m_nPriority = obj.m_nPriority;
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

void CArtifactGroup::RegisterAll(bool b)
{
  nwxXmlPersist::RegisterAll(b);
  RegisterWxString(wxT("Label"),  &m_sLabel);
  RegisterWxString(wxT("Display"),&m_sDisplay);
  RegisterInt(wxT("Priority"),    &m_nPriority);
  Register(wxT("MsgName"),     &m_ios, (void *) &m_vsMsgName);
  Register(wxT("SearchString"),&m_ios, (void *) &m_vsSearchString);
}

void CArtifactGroup::_buildRegEx() const
{
  std::vector<wxString>::const_iterator itr;
  for(itr = m_vsSearchString.begin();
    itr != m_vsSearchString.end();
    ++itr)
  {
    wxRegEx *pRE = CArtifactREcache::GetRE(*itr);
    m_pvSearchRE.push_back(pRE); // push back even if null to make count match
  }
}

const wxString CArtifactLabels::g_sROOT(wxT("ArtifactLabels"));
const wxString CArtifactLabels::g_sDEFAULT_LABEL(wxT("A"));


CArtifactLabels::CArtifactLabels() : nwxXmlPersist()
{
  RegisterAll(true);
  wxString sFile = mainApp::GetConfig()->FindArtifactLabelsFileName();
  m_bOK = !sFile.IsEmpty() && LoadFile(sFile,false);
}
CArtifactLabels::~CArtifactLabels()
{
  m_io.Cleanup();
}

bool CArtifactLabels::LoadFile(const wxString &sFileName, bool bLock)
{
  bool bRtn = nwxXmlPersist::LoadFile(sFileName,bLock);
  Sort();
  return bRtn;
}

const wxString &CArtifactLabels::GetDisplayFromString(const wxString &sArtifactString) const
{
  std::vector<CArtifactGroup *>::const_iterator itr;
  for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
  {
    if((*itr)->IsStringMatch(sArtifactString))
    {
      return (*itr)->GetDisplay();
    }
  }
  return g_sDEFAULT_LABEL;
}

const wxString &CArtifactLabels::GetDisplayFromMsgType(const wxString &sMsgType) const
{
  std::vector<CArtifactGroup *>::const_iterator itr;
  for(itr = m_vector.begin(); itr != m_vector.end(); ++itr)
  {
    if((*itr)->IsMsgTypeMatch(sMsgType))
    {
      return (*itr)->GetDisplay();
    }
  }
  return mainApp::EMPTY_STRING;
}

void CArtifactLabels::GetList(vector<CArtifactGroup *> *pv) const
{
  vectorptr<CArtifactGroup>::cleanup(pv);
  for(std::vector<CArtifactGroup *>::const_iterator itr = m_vector.begin();
    itr != m_vector.end();
    ++itr)
  {
    pv->push_back(new CArtifactGroup(*(*itr)));
  }
}

bool CArtifactLabels::CompareList(
  const std::vector<CArtifactGroup *> &vec) const
{
  // return true if the lists are equivalent, false otherwise
  //   used by GUI to determine if the user modified
  //   anything in the list
  //
  size_t t0 = m_vector.size();
  size_t t1 = vec.size();
  bool bRtn = (t0 == t1);
  if(bRtn)
  {
    std::vector<CArtifactGroup *>::const_iterator itr0,itr1;
    for( itr0 = m_vector.begin(), itr1 = vec.begin();
      bRtn && (itr0 != m_vector.end());
      ++itr0,++itr1)
    {
      if(!(*itr0)->SameAs(*(*itr1)))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}


class CArtifactGreater
{
public:
  CArtifactGreater() {;}
  bool operator()(const CArtifactGroup &g1,const CArtifactGroup &g2) const
  {
    int n1 = g1.GetPriority();
    int n2 = g2.GetPriority();
    bool bRtn = (n1 > n2) ||
      ((n1 == n2) && (g1.GetDisplay() > g2.GetDisplay()));
    return bRtn;
  }
  bool operator()(const CArtifactGroup *p1,const CArtifactGroup *p2) const
  {
    return (*this)(*p1,*p2);
  }

};

void CArtifactLabels::Sort()
{
  vectorSort<CArtifactGroup *,CArtifactGreater>::SortMulti(&m_vector);
}
