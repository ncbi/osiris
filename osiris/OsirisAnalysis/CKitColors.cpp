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

*  FileName: CKitColors.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "CKitColors.h"
#include "CKitColors2.h"
#include "ConfigDir.h"
#include "CKitList.h"
#include "CILSLadderInfo.h"
#include "nwx/mapptr.h"
#include "nwx/nwxLog.h"
/////////////////////////////////////////////////////////////////////

//         CChannelColors


CChannelColors::CChannelColors(
  const CKitChannel *pChannel, const CKitColors2 *pColors2)
{
  m_nr = pChannel->GetKitNr();
  m_sDyeName = pChannel->GetDyeName();
  const CKitColorDye *pDye = pColors2->GetColorByName(pChannel->GetColor());
  if(pDye == NULL)
  {
    pDye = pColors2->GetColorByDyeName(m_sDyeName);
  }
  _SetColors(pDye);
}

CChannelColors::CChannelColors(
  unsigned int nChannel,
  const wxString &sDyeName,
  const CKitColors2 *pColors2)
{
  m_nr = nChannel;
  m_sDyeName = sDyeName;
  const CKitColorDye *pDye = pColors2->GetColorByDyeName(m_sDyeName);
  _SetColors(pDye);
}
const wxColour *CChannelColors::GetColorPtr(DATA_TYPE n) const
{
  const wxColour *pRtn=NULL;
  switch(n)
  {
  case ANALYZED_DATA:
    pRtn = GetColorAnalyzedPtr();
    break;
  case RAW_DATA:
    pRtn = GetColorRawPtr();
    break;
  case BASELINE_DATA:
  case LADDER_DATA:
    pRtn = GetColorLadderPtr();
    break;
  default:
    pRtn = &CKitColors::g_BLACK;
    break;
  }
  return pRtn;
}

void CChannelColors::_SetColors(const CKitColorDye *pDye)
{
  if(pDye == NULL)
  {
    m_ColorAnalyzed = CKitColors::g_BLACK;
    m_ColorRaw.Set(wxT("#999999"));
    m_ColorLadder.Set(wxT("#C0C0C0"));
  }
  else
  {
    m_ColorAnalyzed = pDye->GetColorAnalyzed();
    m_ColorRaw = pDye->GetColorRaw();
    m_ColorLadder = pDye->GetColorLadder();
  }
}

/////////////////////////////////////////////////////////////////////

//         CSingleKitColors

const CChannelColors *CSingleKitColors::GetColorChannel(
        unsigned int nChannel) const
{
  std::map<unsigned int, CChannelColors *>::const_iterator itr =
    m_mapChannelColors.find(nChannel);
  const CChannelColors *pRtn =
    (itr != m_mapChannelColors.end()) ? itr->second : NULL;
  return pRtn;
}
const CChannelColors *CSingleKitColors::GetColorChannelFromLS(
  const wxString &sLSname) const
{
  CILSLadderInfo *pILS = mainApp::GetILSLadderInfo();
  const CILSfamily *pFamily = pILS->GetFamilyFromLS(sLSname);
  const wxString &sILS = (pFamily != NULL) ? pFamily->GetILSname() : mainApp::EMPTY_STRING;
  const CChannelColors *pCC = GetILSChannel(sILS);
  return pCC;
}
const CChannelColors *CSingleKitColors::GetILSChannel(const wxString &sILSFamily) const
{
  std::map<const wxString, CChannelColors *>::const_iterator itr =
    m_mapILSColors.find(sILSFamily);
  const CChannelColors *pRtn = NULL;
  if(itr != m_mapILSColors.end())
  { pRtn = itr->second;
  }
  else
  { pRtn = GetColorChannel(m_nILSchannel);
  }
  return pRtn;
}

CSingleKitColors::~CSingleKitColors() 
{
  mapptr<unsigned int, CChannelColors>::cleanup(&m_mapChannelColors);
  mapptr<const wxString,CChannelColors>::cleanup(&m_mapILSColors);
}
CSingleKitColors::CSingleKitColors(
    const wxString &sKitName,
    CPersistKitList *pKitList,
    const CKitColors2 *pColors2) : m_bISOK(true)
{
  m_sKitName = sKitName;
  m_nILSchannel = pKitList->GetILSchannelNumber(sKitName);
  const wxArrayString *pILSfamily = pKitList->GetIlsArray(sKitName);
  const std::map<unsigned int, CKitChannel *> *pChannelMap =
    pKitList->GetChannelColorMap(sKitName);
  std::map<unsigned int, CKitChannel *>::const_iterator itrColor;
  CChannelColors *pChannelColors;
  if(pChannelMap == NULL)
  {
    wxString sMsg;
    m_bISOK = false;
    sMsg.Append(wxS("\nCannot find channel map for kit: "));
    sMsg.Append(sKitName);
    nwxLog::LogMessage(sMsg);
  }
  else
  {
    for(itrColor = pChannelMap->begin();
      itrColor != pChannelMap->end();
      ++itrColor)
    {
      pChannelColors = new CChannelColors(itrColor->second,pColors2);
      m_mapChannelColors.insert(
        std::map<unsigned int, CChannelColors *>::value_type(
          itrColor->first,pChannelColors));
    }
    if(pILSfamily != NULL)
    {
      size_t nCount = pILSfamily->Count();
      size_t i;
      CILSLadderInfo *pILS = pKitList->GetILSLadderInfo();
      for(i = 0; i < nCount; ++i)
      {
        const wxString &sILSname(pILSfamily->Item(i));
        const wxString &sDyeName = pILS->GetDyeName(sILSname);
        pChannelColors = new CChannelColors(m_nILSchannel,sDyeName,pColors2);
        m_mapILSColors.insert(
          std::map<const wxString,CChannelColors *>::value_type(
            sILSname,pChannelColors));
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////

//         CKitColors

const wxColour CKitColors::g_BLACK(0,0,0);

CKitColors::~CKitColors()
{
  if(m_pKitColors2 != NULL)
  {
    delete m_pKitColors2;
  }
  mapptr<const wxString,CSingleKitColors>::cleanup(&m_mapKitColors);
}

const CSingleKitColors *CKitColors::GetKitColors(
              const wxString &sKitName) const
{
  std::map<const wxString,CSingleKitColors *>::const_iterator itr =
     m_mapKitColors.find(sKitName);
  const CSingleKitColors *pRtn =
    (itr != m_mapKitColors.end()) ? itr->second : NULL;
  return pRtn;
}
const wxColour &CKitColors::GetColor(
  const wxString &sKitName, DATA_TYPE n, unsigned int nChannel) const
{
  const CSingleKitColors *pkc = GetKitColors(sKitName);
  const CChannelColors *pcc = (pkc != NULL) ? pkc->GetColorChannel(nChannel) : NULL;
  const wxColour *pRtn = (pcc != NULL) ? pcc->GetColorPtr(n) : &g_BLACK;
  return *pRtn;
}
const wxColour &CKitColors::GetColorFromLS(
  const wxString &sKitName, DATA_TYPE n, const wxString &sLSname) const
{
  const CSingleKitColors *pkc = GetKitColors(sKitName);
  const CChannelColors *pcc = (pkc != NULL) ? pkc->GetColorChannelFromLS(sLSname) : NULL;
  const wxColour *pRtn = (pcc != NULL) ? pcc->GetColorPtr(n) : wxBLACK;
  return *pRtn;
}
const wxColour &CKitColors::GetColorByDye(const wxString &sDyeName, DATA_TYPE n) const
{
  const CKitColorDye *p = m_pKitColors2->GetColorByDyeName(sDyeName);
  const wxColour &rtn = (p == NULL) ? *wxBLACK : p->GetColor(n);
  return rtn;
}

bool CKitColors::_Load()
{
  bool bRtn = true;
  CPersistKitList *pKitList = mainApp::GetKitList();
  CILSLadderInfo *pILS = pKitList->GetILSLadderInfo();
  const vector<CILSkit *> *pKits = pILS->GetKits();
  vector<CILSkit *>::const_iterator itrKits;
  m_pKitColors2 = new CKitColors2;
  for (itrKits = pKits->begin(); itrKits != pKits->end(); ++itrKits)
  {
    const wxString &sKitName = (*itrKits)->GetKitName();
    CSingleKitColors *pColors = new CSingleKitColors(sKitName,pKitList,m_pKitColors2);
    if(pColors->IsOK())
    {
      m_mapKitColors.insert(
        std::map<const wxString,CSingleKitColors *>::value_type(sKitName,pColors));
    }
    else
    {
      delete pColors;
    }
  }
  return bRtn;
}

const wxColour &CKitColors::GetColorByName(const wxString &sName, DATA_TYPE n) const
{
  const CKitColorDye *pDye = 
    (m_pKitColors2 == NULL) ? NULL : m_pKitColors2->GetColorByName(sName.Upper());
  const wxColor &Rtn = 
    (pDye != NULL) ? pDye->GetColor(n) : g_BLACK;
  return Rtn;
}

const wxString &CKitColors::GetColorName(const wxColour &c) const
{
  return m_pKitColors2->GetColorName(c);
}
