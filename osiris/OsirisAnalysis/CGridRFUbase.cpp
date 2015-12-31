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
*  FileName: CGridRFUbase.cpp
*  Author:   Douglas Hoffman
*
*  wxGrid for entering or viewing RFU limits for sample, ILS, channel, and the 
*    ability to overide sample for each channel.  This is a base class
*    for CGridRFULimits (LabSettings),
*    CGridRFURun in Analysis dialog class CDialogAnalysis(), and
*    CGridRFUParams for displaying RFU limits in CDialogParameters()
*/
#include "CGridRFUbase.h"
#include "CKitList.h"
#include "mainApp.h"


bool CGridRFUbase::_SetupKitBase(const wxString &sKitName)
{
  wxString sProblem;
  CPersistKitList *pKit = mainApp::GetKitList();
  m_sKitName = sKitName;
  const CLocusNameList *pLocus = 
    pKit->GetLocusNameList(m_sKitName);
  m_pKitColors = m_kitColors.GetKitColors(m_sKitName);
  bool bRtn = true;

  if(m_sKitName.IsEmpty())
  {
    sProblem = wxS("Marker set name is not specified.");
  }
  else if(pLocus == NULL)
  {
    sProblem = wxS("Marker set, ");
    sProblem.Append(m_sKitName);
    sProblem.Append(wxS(", is unknown"));
  }
  else if(pLocus->size() < 1)
  {
    sProblem = wxS("Marker set, ");
    sProblem.Append(m_sKitName);
    sProblem.Append(wxS(", does not have any loci"));
  }
  if(sProblem.IsEmpty())
  {
    set<int> snChannelsUsed;
    int nChannel;
    int nLastChannel = -1;
    m_nILSchannel = -1;
    m_nChannelCount = pKit->GetChannelCount(m_sKitName);
    m_nChannelColumnCount = m_nChannelCount;
    m_vnChannelNumbers.clear();
    m_vnChannelNumbers.reserve(m_nChannelCount);

    //  find all channels that have a locus

    for(CLocusNameList::const_iterator itr = pLocus->begin();
      itr != pLocus->end();
      ++itr)
    {
      nChannel = (*itr)->GetChannel();
      if(nChannel != nLastChannel)
      {
        m_vnChannelNumbers.push_back((unsigned int)nChannel);

        snChannelsUsed.insert(nChannel);
        nLastChannel = nChannel;
      }
    }

    // find highest numbered channel that doesn't have a locus
    // assume it is ILS

    for(nChannel = m_nChannelCount; nChannel > 0; --nChannel)
    {
      if(snChannelsUsed.find(nChannel) == snChannelsUsed.end())
      {
        m_nILSchannel = nChannel; // unused channel, assume ILS
        m_nChannelColumnCount--;
        nChannel = 0; // loop exit
      }
    }
  }
  else
  {
    // STOP HERE BUILD WITH ERROR
    mainApp::ShowError(sProblem,NULL);
    mainApp::LogMessage(sProblem);
    bRtn = false;
  }
  return bRtn;
}

void CGridRFUbase::_DisableUnused()
{
  int nRow,nCol;
  int nColCount = GetNumberCols();
  int nRowCount = GetNumberRows();
  for (nCol = 0; nCol < nColCount; ++nCol)
  {
    for(nRow = 0; nRow < nRowCount; ++nRow)
    {
      if(_DisabledCell(nRow,nCol))
      {
        SetReadOnly(nRow,nCol,true);
      }
    }
  }
}

void CGridRFUbase::_UpdateReadOnly() 
{
  nwxGrid::SetAllReadOnly(this,m_bReadOnly);
  if(!m_bReadOnly)
  {
    _DisableUnused();
  }
}

void CGridRFUbase::_SetCellIntValue(int nRow, int nCol, int nValue)
{
  if(_DisabledCell(nRow,nCol) || (nValue == -1))
  {
    SetCellValue(nRow,nCol,"");
  }
  else
  {
    SetCellValue(nRow,nCol,nwxString::FormatNumber(nValue));
  }
}

int CGridRFUbase::_GetCellIntValue(int nRow, int nCol)
{
  wxString s = GetCellValue(nRow,nCol);
  int nRtn = 
    s.IsEmpty()
    ? -1
    : atoi(s.utf8_str());
  return nRtn;
}