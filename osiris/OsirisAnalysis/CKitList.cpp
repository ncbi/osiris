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

*  FileName: CKitList.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CKitList.h"
#include "CILSLadderInfo.h"
#include "nwx/nwxString.h"
#include "nwx/vectorptr.h"
#include "nwx/mapptr.h"
#include "ConfigDir.h"
#include <memory>


const wxString CKitLadderFiles::g_sEndName(wxT("_ladderinfo.xml"));

void CKitLadderFiles::_Load(const wxString &sPath, bool bSiteFolder)
{
  bool bError = false;
  const int nFlags = wxDIR_FILES | wxDIR_HIDDEN | wxDIR_NO_FOLLOW;
  if (wxDir::Exists(sPath))
  {
    wxDir dir(sPath);
    if (dir.IsOpened())
    {
      if (bSiteFolder)
      {
        wxFileName fn(sPath);
        m_dtDirSite = fn.GetModificationTime();
      }
      dir.Traverse(*this, wxEmptyString, nFlags);
    }
    else
    {
      m_dtDirSite.Set(time_t(0));
      bError = true;
    }
  }
  else
  {
    bError = !bSiteFolder;
  }
  if (bError)
  {
    OnOpenError(sPath);
  }
}

bool CKitLadderFiles::Reload()
{
  m_asList.Clear();
  m_asErrorList.Clear();
  ConfigDir *pDir = mainApp::GetConfig();
  _Load(pDir->GetILSLadderFilePath(), false);
  _Load(pDir->GetSiteILSLadderFilePath(), true);
  return IsOK();
}
wxDirTraverseResult CKitLadderFiles::OnDir(const wxString&)
{
  return wxDIR_CONTINUE;
}
wxDirTraverseResult CKitLadderFiles::OnFile(const wxString& filename)
{
  size_t nLenFile = filename.Len();
  size_t nLenEnd = g_sEndName.Len();
  if ( (nLenFile <= nLenEnd) ||
    nwxFileUtil::IsMacAttrFile(filename) ||
    g_sEndName.CmpNoCase(filename.Mid(nLenFile - nLenEnd)) )
  {
    ; // ignore this file, doesn't end with _ladderinfo.xml
  }
  else if(wxFileName::IsFileReadable(filename))
  {
    // OK
    m_asList.Add(filename);
  }
  else
  {
    // file not readable
    m_asErrorList.Add(filename);
  }
  return wxDIR_CONTINUE;
}
wxDirTraverseResult CKitLadderFiles::OnOpenError(const wxString& openerrorname)
{
  m_asErrorList.Add(openerrorname);
  return wxDIR_CONTINUE;
}



bool CLocusNameChannel::operator <(const CLocusNameChannel &x) const
{
  bool bRtn = false;
  if(m_nMinBP < x.m_nMinBP)
  {
    bRtn = m_nChannel <= x.m_nChannel;
  }
  else if(m_nChannel < x.m_nChannel)
  {
    bRtn = true;
  }
  else if(m_nChannel == x.m_nChannel)
  {
    // we already know the m_nMinBP >= x.m_nMinBP;
    // this code should never be reached
    if(m_nMinBP == x.m_nMinBP) 
    {
      if(m_nMaxBP < x.m_nMaxBP)
      {
        bRtn = true;
      }
      else if(m_nMaxBP == x.m_nMaxBP)
      {
        nwxStringLessNoCaseSort LESS;
        bRtn = LESS(m_sName,x.m_sName);
      }
    }
  }
  return bRtn;
}

CPersistKitList::~CPersistKitList()
{
    if(m_pILS != NULL) 
    {
      delete m_pILS;
      m_pILS = NULL;
    }
    Clear();
}

void CPersistKitList::Clear()
{
  LSitr itr;
  for(itr = m_mLS.begin();
    itr != m_mLS.end();
    ++itr)
  {
    delete itr->second;
  }
  m_mLS.clear();

  for(itr = m_mILS.begin();  // v2.7 ILS family
    itr != m_mILS.end();
    ++itr)
  {
    delete itr->second;
  }
  m_mILS.clear();

  for(KLNCitr itr2 = m_mapKitLocus.begin();
    itr2 != m_mapKitLocus.end();
    ++itr2)
  {
    CLocusNameList *p = itr2->second;
    setptr<const CLocusNameChannel,CLocusNameChannelLess>::cleanup(p);
    delete p;
  }

  m_mapKitLocus.clear();

  mapptr<wxString,CKitChannelMap>::cleanup(&m_mapKitChannels);
  m_as.Clear();
  m_mapKitToLadderInfo.clear();
  m_sLastKit.Empty();
  m_sErrorMsg.Empty();
  m_pLastKitLocus = NULL;
  m_pLastKitLS = NULL;
  m_pLastKit_ILS = NULL;
  m_pLastKitChannelMap = NULL;
  m_setLadderFree.clear();  // v2.12
  m_setAll_ILS.clear();
  m_dtSiteILSLadderDir.Set(time_t(0));
}


bool CPersistKitList::Load_V1()
{
  ConfigDir *pDir = mainApp::GetConfig();
  wxString sFile = pDir->GetLadderFileName();
  m_bV1 = true;
  bool bRtn = LoadFile(sFile);
  if(!bRtn)
  {
    _SetLoadError();
  }
  return bRtn;
}
CILSLadderInfo *CPersistKitList::GetILSLadderInfo()
{
  if(m_pILS == NULL)
  {
    m_pILS = new CILSLadderInfo();
    if(!m_pILS->IsOK())
    {
      _SetLoadError();
    }
  }
  else
  {
    m_pILS->CheckFileModification(true);
  }
  return m_pILS;
}
bool CPersistKitList::Load()
{
  wxString sFile;
  CKitLadderFiles asFiles;
  size_t nFileCount = asFiles->GetCount();
  CIncrementer x(m_nInLoad);
  int nCount = 0;
  m_bV1 = false;
  Clear();

  m_dtSiteILSLadderDir = asFiles.GetSiteModTime();

  // load all kit xml files

  for(size_t n = 0; n < nFileCount; ++n)
  {
    sFile = asFiles->Item(n);
    if(LoadFile(sFile))
    {
      Add(m_sLastKit,sFile);
      nCount++;
    }
    else
    {
      _AddError();
    }
  }
  nwxString::Trim(&m_sErrorMsg);
  _HACK_27();
  SortILS();
  return (nCount > 0);
}
bool CPersistKitList::NeedReload()
{
  bool bRtn = true;
  wxString sPath = mainApp::GetConfig()->GetSiteILSLadderFilePath();
  wxDateTime dtPath(time_t(0));
  if (wxDir::Exists(sPath))
  {
    wxFileName fn(sPath);
    dtPath = (fn.IsDir() && fn.IsFileReadable())
      ? fn.GetModificationTime() : wxDateTime(time_t(0));
  }
  if (!dtPath.IsValid())
  {
    // no path, reload if m_dtSiteILSLadderDir has a valid value > 0
    //  i.e. if previously loaded and now doesn't exist, return true
    //  should never happen
    bRtn = m_dtSiteILSLadderDir.IsValid()
      && (m_dtSiteILSLadderDir.GetTicks() != 0);
  }
  else if(m_dtSiteILSLadderDir.IsValid())
  {
    // both times are valid, if changed return true
    bRtn = dtPath != m_dtSiteILSLadderDir;
  }
  else
  {
    // not previously loaded, return true if path is OK
    bRtn = (dtPath.GetTicks() > 0);
  }
  return bRtn;
}

void CPersistKitList::_HACK_27()
{
  // _HACK_27 load ILS families from CILSLadderInfo version 2.7

  const CILSLadderInfo *pILS = GetILSLadderInfo();
  std::map<wxString, wxArrayString *>::iterator itr,itrLS;
  wxArrayString *pa,*paLS;
  const wxString sFamily;
  size_t nCount,n;
  std::vector<CILSname *>::const_iterator itrn;
  for(itr = m_mILS.begin(); itr != m_mILS.end(); ++itr)
  {
    pa = itr->second;
    nCount = pa->GetCount();
    if(nCount)
    {
      itrLS = m_mLS.find(itr->first);
      if(itrLS == m_mLS.end())
      {
        paLS = new wxArrayString;
        paLS->Alloc(12);
        m_mLS.insert(map<wxString, wxArrayString *>::value_type(itr->first,paLS));
      }
      else
      {
        paLS = itrLS->second;
      }
      for(n = 0; n < nCount; n++)
      {
        const CILSfamily *pFam = pILS->GetFamily(pa->Item(n));
        if(pFam != NULL)
        {
          const std::vector<CILSname *> &vn = pFam->GetNames();
          for (itrn = vn.begin(); itrn != vn.end(); ++itrn)
          {
            paLS->Add((*itrn)->GetName());
          }
        }
      }
    }
  }
}

void CPersistKitList::SortILS()
{
  std::map<wxString, wxArrayString *>::iterator itr;
  wxArrayString *pILS;
  for(itr = m_mLS.begin(); itr != m_mLS.end(); ++itr)
  {
    pILS = itr->second;
    pILS->Sort();
  }
  for(itr = m_mILS.begin(); itr != m_mILS.end(); ++itr)
  {
    pILS = itr->second;
    pILS->Sort();
  }
}
void CPersistKitList::_CHECK_BOOL(wxXmlNode *pNode, std::set<wxString> *pSet)
{
  bool b;
  m_XmlBool.LoadFromNode(pNode, &b);
  if (b)
  {
    pSet->insert(m_sLastKit);
  }
}

bool CPersistKitList::LoadFromNode(wxXmlNode *pNode)
{
  bool bRtn = true;
  if(!m_nInLoad)
  {
    Clear();
  }
  CIncrementer x(m_nInLoad);
  wxString sNodeName(pNode->GetName());
  if(!sNodeName.Cmp("Name"))
  {
    wxString s;
    m_XmlString.LoadFromNode(pNode,(void *)&s);
    m_pLastKitLocus = NULL;
    m_pLastKitLS = NULL;
    m_pLastKitChannelMap = NULL;
    if(s.Len())
    {
      m_sLastKit = s;
      if (!m_bV1)
      {
        LSitr itrLS = m_mLS.find(m_sLastKit);
        m_pLastKitLS =
          (itrLS == m_mLS.end())
          ? NULL
          : itrLS->second;
        itrLS = m_mILS.find(m_sLastKit);
        m_pLastKit_ILS =
          (itrLS == m_mILS.end())
          ? NULL
          : itrLS->second;

        KLNCitr itrLocus = m_mapKitLocus.find(m_sLastKit);
        m_pLastKitLocus =
          (itrLocus == m_mapKitLocus.end())
          ? NULL
          : itrLocus->second;
      }
    }
  }
  else if( !sNodeName.Cmp("LSName") && !m_sLastKit.IsEmpty() )
  {
    wxString s;
    m_XmlString.LoadFromNode(pNode,(void *)&s);
    if(s.Len())
    {
      if(m_pLastKitLS == NULL)
      {
        m_pLastKitLS = new wxArrayString;
        m_pLastKitLS->Alloc(6);
        m_mLS.insert(
          std::map<wxString, wxArrayString *>::value_type(
                m_sLastKit,m_pLastKitLS));
      }
      m_pLastKitLS->Add(s);
    }
  }
  else if( !sNodeName.Cmp("ILSName") && !m_sLastKit.IsEmpty() )
  {
    wxString s;
    m_XmlString.LoadFromNode(pNode,(void *)&s);
    if(s.Len())
    {
      if(m_pLastKit_ILS == NULL)
      {
        m_pLastKit_ILS = new wxArrayString;
        m_pLastKit_ILS->Alloc(6);
        m_mILS.insert(
          std::map<wxString, wxArrayString *>::value_type(
                m_sLastKit,m_pLastKit_ILS));
      }
      m_pLastKit_ILS->Add(s);
    }
  }
  else if (!sNodeName.Cmp("LadderFree") && !m_sLastKit.IsEmpty())
  {
    _CHECK_BOOL(pNode, &m_setLadderFree);
  }
  else if (!sNodeName.Cmp("AllLaneStandards") && !m_sLastKit.IsEmpty())
  {
    _CHECK_BOOL(pNode, &m_setAll_ILS);
  }
  else if( !sNodeName.Cmp("ChannelNo") && !m_sLastKit.IsEmpty() )
  {
    std::map<wxString,int>::iterator itrC = m_msChannelCount.find(m_sLastKit);
    int nCurrent = 0;
    int n = 0;
    g_IOint.LoadFromNode(pNode,(void *) &n);
    if(itrC != m_msChannelCount.end())
    {
      nCurrent = itrC->second;
      if(nCurrent < n)
      {
        m_msChannelCount.erase(itrC);
      }
    }
    if(nCurrent < n)
    {
      m_msChannelCount.insert(
          std::map<wxString,int>::value_type(m_sLastKit,n));
    }
  }
  else if( !sNodeName.Cmp("Locus") )
  {
    if(m_pLastKitLocus == NULL)
    {
      m_pLastKitLocus = new CLocusNameList;
      m_mapKitLocus.insert(
        std::map< wxString, CLocusNameList * >::value_type(
          m_sLastKit,m_pLastKitLocus));
    }
    unique_ptr<CLocusNameChannel> pLC(new CLocusNameChannel);
    if(pLC->LoadFromNode(pNode))
    {
      m_pLastKitLocus->insert(pLC.release());
    }
    else
    {
      bRtn = false;
    }
  }
  else if(!sNodeName.Cmp("FsaChannelMap"))
  {
    if(m_pLastKitChannelMap == NULL)
    {
      m_pLastKitChannelMap = new CKitChannelMap(wxT("Channel"));
      m_mapKitChannels.insert(
        std::map<wxString, CKitChannelMap *>::value_type(
          m_sLastKit,m_pLastKitChannelMap));
    }
    bRtn = m_pLastKitChannelMap->LoadFromNode(pNode);
  }
  else
  {
    bRtn = nwxXmlPersist::LoadFromNode(pNode);
  }
  return bRtn;
}
const wxArrayString *CPersistKitList::_GetAll_ILSarray() const
{
  if (m_asAll_ILS.IsEmpty())
  {
    m_pILS->BuildAll_ILSarray(&m_asAll_ILS);
  }
  return &m_asAll_ILS;
}

#ifdef __WXDEBUG__
void CPersistKitList::UnitTest()
{
  CPersistKitList kitList;
  if(!kitList.Load())
  {
    wxString sError("CPersistKitList::Load() failed\n");
    wxASSERT_MSG(0,sError);
  }
#undef PP12
}
#endif
