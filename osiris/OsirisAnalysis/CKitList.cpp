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
#include "nwx/nwxString.h"
#include "nwx/vectorptr.h"
#include "ConfigDir.h"
#include "CILSLadderInfo.h"
#include <memory>

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

void CPersistKitList::Clear()
{
  for(LSitr itr = m_mLS.begin();
    itr != m_mLS.end();
    ++itr)
  {
    delete itr->second;
  }
  for(KLNCitr itr2 = m_mapKitLocus.begin();
    itr2 != m_mapKitLocus.end();
    ++itr2)
  {
    CLocusNameList *p = itr2->second;
    setptr<const CLocusNameChannel,CLocusNameChannelLess>::cleanup(p);
    delete p;
  }
  m_mLS.clear();
  m_as.Clear();
  m_sLastKit.Empty();
  m_sErrorMsg.Empty();
  m_pLastKitLocus = NULL;
  m_pLastKitLS = NULL;
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
bool CPersistKitList::Load()
{
  wxString sFile;
  CILSLadderInfo ldr(true);
  CIncrementer x(m_nInLoad);
  if(!ldr.IsOK())
  {
    _SetLoadError();
  }
  vector<CILSkit *> *pvKits = ldr.GetKits();
  vector<CILSkit *>::iterator itr;
  CILSkit *pKit;
  int nCount = 0;
  m_bV1 = false;
  Clear();

  // load all kit xml files

  for(itr = pvKits->begin();
      itr != pvKits->end();
      ++itr)
  {
    pKit = *itr;
    m_sLastKit = pKit->GetKitName();
    m_pLastKitLocus = NULL;
    m_pLastKitLS = NULL;
    LSitr itrLS = m_mLS.find(m_sLastKit);
    m_pLastKitLS = 
      (itrLS == m_mLS.end())
      ? NULL
      : itrLS->second;
    KLNCitr itrLocus = m_mapKitLocus.find(m_sLastKit);
    m_pLastKitLocus =
      (itrLocus == m_mapKitLocus.end())
      ? NULL
      : itrLocus->second;

    sFile = pKit->GetFilePath();
    if(LoadFile(sFile))
    {
      Add(m_sLastKit);
      nCount++;
    }
    else
    {
      _AddError();
    }
  }
  nwxString::Trim(&m_sErrorMsg);
  SortILS();
  return (nCount > 0);
}

void CPersistKitList::SortILS()
{
  map<wxString, wxArrayString *>::iterator itr;
  wxArrayString *pILS;
  for(itr = m_mLS.begin(); itr != m_mLS.end(); ++itr)
  {
    pILS = itr->second;
    pILS->Sort();
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
  if(!sNodeName.Cmp(_T("Name")))
  {
    if(m_bV1)
    {
      wxString s;
      m_XmlString.LoadFromNode(pNode,(void *)&s);
      if(s.Len())
      {
        m_as.Add(s);
        m_sLastKit = s;
      }
    }
  }
  else if( !sNodeName.Cmp(_T("LSName")) && !m_sLastKit.IsEmpty() )
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
          map<wxString, wxArrayString *>::value_type(
                m_sLastKit,m_pLastKitLS));
      }
      m_pLastKitLS->Add(s);
    }
  }
  else if( !sNodeName.Cmp(_T("ChannelNo")) && !m_sLastKit.IsEmpty() )
  {
    map<wxString,int>::iterator itrC = m_msChannelCount.find(m_sLastKit);
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
          map<wxString,int>::value_type(m_sLastKit,n));
    }
  }
  else if( !sNodeName.Cmp(_T("Locus")) )
  {
    if(m_pLastKitLocus == NULL)
    {
      m_pLastKitLocus = new CLocusNameList;
      m_mapKitLocus.insert(
        map< wxString, CLocusNameList * >::value_type(
          m_sLastKit,m_pLastKitLocus));
    }
    auto_ptr<CLocusNameChannel> pLC(new CLocusNameChannel);
    if(pLC->LoadFromNode(pNode))
    {
      m_pLastKitLocus->insert(pLC.release());
    }
    else
    {
      bRtn = false;
    }
  }
  else
  {
    bRtn = nwxXmlPersist::LoadFromNode(pNode);
  }
  return bRtn;
}
#ifdef __WXDEBUG__
void CPersistKitList::UnitTest()
{
  CPersistKitList kitList;
  wxString sError;
  if(!kitList.Load())
  {
    sError.Append(_T("CPersistKitList::Load() failed\n"));
  }
  else
  {
    const wxArrayString *psKitArray;
    const wxArrayString *psArray;
    psKitArray = &kitList.GetArray();
    size_t nCount = psKitArray->GetCount();
#define PP12 _T("PowerPlex 1.2")
#define PP18D _T("PowerPlex 18D")
#define SGM _T("SGM Plus")
#define SEFILER _T("SEfilerPlus")
#define NGM _T("NGMSElect")
#define PP21 _T("PowerPlex 21")
    struct
    {
      size_t n; // number of loci
      const wxChar *psName;
    } LIST[] = 
    {
      {7,  _T("Cofiler")},
      {16, _T("Identifiler Less ILS250")},
      {16, _T("IdentifilerPlus Less ILS250")},
      {16, _T("IdentifilerPlus")},
      {16, _T("Identifiler")},
      {9,  PP12},
      {18,  PP18D},
      {16, _T("PowerPlex 16")},
      {11, _T("PowerPlex Y")},
      {16, _T("Yfiler")},
      {12, SEFILER},
      {11, SGM},
      {10, _T("ProfilerPlus")},
      {17, NGM},
      {21,PP21}
    };
    size_t N_LIST = sizeof(LIST) / sizeof(LIST[0]);
    if(nCount != N_LIST)
    {
      sError.Append(wxString::Format(
        _T("\nNumber of kits is %d, expected %d"),
        (int)nCount, (int)N_LIST ));
    }
    map<wxString,size_t> kitLocusCount;
    map<wxString,size_t>::iterator itrKL;
    size_t i;
    for(i = 0; i < N_LIST; i++)
    {
      kitLocusCount.insert(
        map<wxString,size_t>::value_type(
          wxString(LIST[i].psName),LIST[i].n));
    }

    for(i = 0; i < nCount; i++)
    {
      // verify LS count

      const wxString &sItem = psKitArray->Item(i);
      psArray = kitList.GetLsArray(sItem);
      if(psArray == NULL)
      {
        sError.Append(_T("\nCannot find ILS for kit: "));
        sError.Append(sItem);
      }
      else
      {
        size_t nExpected = 4;
        if(sItem == PP12 || sItem == PP18D || sItem == NGM || sItem == SEFILER || sItem == PP21)
        { nExpected = 1;
        }
        else if(sItem == SGM)
        { nExpected = 5;
        }
        else if(
          sItem.StartsWith(_T("Identifiler")) &&
          !sItem.Contains(_T("250"))
          )
        { nExpected = 5;
        }

        if(psArray->GetCount() != nExpected)
        {
          sError.Append(
            _T("\nLS count for kit: "));
          sError.Append(sItem);
          sError.Append(wxString::Format(
            _T(", is %d, expected %d"),
            (int)psArray->GetCount(),(int)nExpected));
        }
      }
        
      itrKL = kitLocusCount.find(sItem);
      const CLocusNameList *pList = kitList.GetLocusNameList(sItem);
      if(pList == NULL)
      {
        sError.Append(_T("\nCannot find loci for kit: "));
        sError.Append(sItem);
      }
      if(itrKL == kitLocusCount.end())
      {
        sError.Append(_T("\nUnknown kit name: "));
        sError.Append(sItem);
      }
      else if( (pList != NULL) && (pList->size() != itrKL->second) )
      {
        sError.Append(_T("\nLocus count for kit: "));
        sError.Append(sItem);
        sError.Append(wxString::Format(
          _T(", is %d, expected %d"),
          (int)pList->size(),(int)itrKL->second));
      }
    }
  }
  wxASSERT_MSG(sError.IsEmpty(),sError);
#undef PP12
}
#endif
