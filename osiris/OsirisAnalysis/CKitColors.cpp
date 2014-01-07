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

/////////////////////////////////////////////////////////////////////

//         CSingleKitColors

void CChannelColors::RegisterAll(bool)
{
  RegisterUint(_T("nr"),&m_nr);
  RegisterWxString(_T("dye"),&m_sDyeName);
  RegisterWxColour(_T("analyzed"),&m_ColorAnalyzed);
  RegisterWxColour(_T("raw"),&m_ColorRaw);
  RegisterWxColour(_T("ladder"),&m_ColorLadder);
}


/////////////////////////////////////////////////////////////////////

//         CSingleKitColors

const CChannelColors *CSingleKitColors::GetColorChannel(
        unsigned int nChannel) const
{
  const CChannelColors *pRtn(NULL);
  size_t nSize = m_mapChannelColors.size();
  map<unsigned int, CChannelColors *>::iterator itrm;
  if(!nSize)
  {
    if(nChannel < m_vChannelColors.size())
    {
      pRtn = m_vChannelColors.at(nChannel);
      if(pRtn->m_nr != nChannel)
      {
        // we need the map
        pRtn = NULL;
      }
    }
    if(pRtn == NULL)
    {
      // no value for pRtn yet, build the map

      vector<CChannelColors *>::const_iterator itr;
      for(itr = m_vChannelColors.begin();
        itr != m_vChannelColors.end();
        ++itr)
      {
        itrm = m_mapChannelColors.find((*itr)->m_nr);
        if(itrm != m_mapChannelColors.end())
        {
          m_mapChannelColors.erase(itrm);
        }
        m_mapChannelColors.insert(
          map<unsigned int, CChannelColors *>::value_type(
              (*itr)->m_nr,*itr));
        if((*itr)->m_nr == nChannel)
        {
          pRtn = *itr;
        }
      }
    }
  }
  else
  {
    itrm = m_mapChannelColors.find(nChannel);
    if(itrm != m_mapChannelColors.end())
    {
      pRtn = itrm->second;
    }
  }
  return pRtn;
}

void CSingleKitColors::RegisterAll(bool)
{
  RegisterWxString(_T("name"),&m_sKitName);
  Register(_T("channel"),&m_IOChannelColors,(void *)&m_vChannelColors);
}

/////////////////////////////////////////////////////////////////////

//         CKitColors

const wxColour CKitColors::g_BLACK(0,0,0);

void CKitColors::RegisterAll(bool)
{
  Register(_T("kit"),&m_IOkitColors,(void *)&m_vpKitColors);
}

const CSingleKitColors *CKitColors::GetKitColors(
              const wxString &sKitName) const
{
  const CSingleKitColors *pRtn = NULL;
   map<wxString,CSingleKitColors *>::iterator itrc;
  if(!m_mapKitColors.size())
  {
    vector<CSingleKitColors *>::const_iterator itr;
    for(itr = m_vpKitColors.begin();
      itr != m_vpKitColors.end();
      ++itr)
    {
      const wxString &s((*itr)->GetKitName());
      itrc = m_mapKitColors.find(s);
      if(itrc != m_mapKitColors.end())
      {
        m_mapKitColors.erase(itrc);
      }
      m_mapKitColors.insert(
        map<wxString,CSingleKitColors *>::value_type(s,*itr));
    }
  }
  itrc = m_mapKitColors.find(sKitName);
  if(itrc != m_mapKitColors.end())
  {
    pRtn = itrc->second;
  }
  return pRtn;
}

const wxColour &CKitColors::GetColor(
  const wxString &sKitName, DATA_TYPE n, unsigned int nChannel)
{
  const wxColour *pRtn(&g_BLACK);
  const CSingleKitColors *pkc = GetKitColors(sKitName);
  if(pkc != NULL)
  {
    const CChannelColors *pcc = pkc->GetColorChannel(nChannel);
    if(pcc != NULL)
    {
      switch(n)
      {
      case ANALYZED_DATA:
        pRtn = &(pcc->m_ColorAnalyzed);
        break;
      case RAW_DATA:
        pRtn = &(pcc->m_ColorRaw);
        break;
      case BASELINE_DATA:
      case LADDER_DATA:
        pRtn = &(pcc->m_ColorLadder);
        break;
      default:
        break;
      }
    }
  }
  return *pRtn;
}

bool CKitColors::Load()
{
  // need to enhance to load from user file first
  ConfigDir *pDir = mainApp::GetConfig();
  wxString sFileName = pDir->GetILSLadderFilePath();
  sFileName.Append("kitcolors.xml");
  return LoadFile(sFileName);
}
