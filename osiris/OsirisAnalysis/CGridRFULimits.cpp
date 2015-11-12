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
*  FileName: CGridRFULimits.cpp
*  Author:   Douglas Hoffman
*
*  wxGrid for entering RFU limits for sample, ILS, channel, and the 
*    ability to overide sample for each channel
*
*/
//***************************                     CGridRFULimits

#include "CGridRFULimits.h"
#include "CGridLocusColumns.h"

CGridRFULimits::CGridRFULimits(wxWindow *parent, wxWindowID id) :
  CGridRFUbase(parent,id),
  m_nCOL_SAMPLE(0),
  m_nCOL_CHANNEL_START(1),
  m_nCOL_AFTER_CHANNEL(-1),
  m_nCOL_LADDER(-1),
  m_nCOL_ILS(-1),
  m_pData(NULL)
{
  CreateGrid(ROW_COUNT,7); 
  // 7 columns = sample + ils + ladder + 4 channels (guess)
  //  adjusted in _Build();
}


void CGridRFULimits::_Build()
{
  wxFont fontChannel = GetDefaultCellFont();
  wxFont fontDefault = fontChannel;
  fontChannel.SetWeight(wxFONTWEIGHT_BOLD);
  const CChannelColors *pChannelColors = NULL;
  int nCurrentColCount = GetNumberCols();
  int i;
  int j;
  if(nCurrentColCount < m_nCOL_COUNT)
  {
    InsertCols(m_nCOL_CHANNEL_START, m_nCOL_COUNT - nCurrentColCount);
    _UpdateReadOnly();
  }
  else if(nCurrentColCount > m_nCOL_COUNT)
  {
    DeleteCols(m_nCOL_CHANNEL_START,nCurrentColCount - m_nCOL_COUNT);
  }
  SetDefaultCellValidator(
    new nwxGridCellUIntRangeValidator(
      mainApp::RFU_MIN_ENTER,mainApp::RFU_MAX_ENTER,true));
  EnableDragColSize(false);
  EnableDragRowSize(false);
  SetDefaultCellAlignment(wxALIGN_RIGHT,wxALIGN_CENTRE);
  for(i = 0; i < ROW_COUNT; i++)
  {
    for(j = 0; j < m_nCOL_COUNT; j++)
    {
      SetCellValue(i,j,"00000000"); // used for size
      if(_DisabledCell(i,j))
      {
        SetCellBackgroundColour(i,j,GetGridLineColour());
      }
    }
  }
  SetDefaultEditor(new wxGridCellFloatEditor(1,0));
  SetColLabelValue(m_nCOL_SAMPLE,"Sample");
  SetColLabelValue(m_nCOL_LADDER,"Ladder");
  SetColLabelValue(m_nCOL_ILS,"   ILS   ");
  SetRowLabelValue(ROW_RFU_MIN,"Analysis Threshold (RFU)");
  SetRowLabelValue(ROW_RFU_INTERLOCUS,"Min. Interlocus RFU");
  SetRowLabelValue(ROW_RFU_MAX,"Max. RFU");
  SetRowLabelValue(ROW_DETECTION,"Detection Threshold (RFU)");
  SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
  SetMargins(0,0);
  ChannelNumberIterator itrChannelCol;
  int nCol;
  const wxChar *psDye = NULL;
  wxString sLabel;
  for(itrChannelCol = m_vnChannelNumbers.begin(), 
          nCol = m_nCOL_CHANNEL_START;
    itrChannelCol != m_vnChannelNumbers.end();
    ++itrChannelCol, ++nCol)
  {
    if(m_pKitColors != NULL)
    {
      pChannelColors = m_pKitColors->GetColorChannel(*itrChannelCol);
      psDye = (pChannelColors == NULL) ? NULL : (const wxChar *) pChannelColors->m_sDyeName;
    }
    CGridLocusColumns::FORMAT_CHANNEL_DYE(&sLabel,*itrChannelCol,psDye);
    SetColLabelValue(nCol,sLabel);
    if(pChannelColors != NULL)
    {
      _SetupChannelColumn(nCol,pChannelColors->m_ColorAnalyzed,fontChannel);
    }
    else
    {
      _SetupDefaultChannelColumn(nCol);
    }
  }
  nwxGrid::UpdateLabelSizes(this);
  AutoSize();
  _DisableUnused();
}

void CGridRFULimits::SetData(CLabThresholds *pData, const wxString &sKitName)
{
  m_pData = pData;
  if(_SetupKitBase(sKitName))
  {
    m_nCOL_AFTER_CHANNEL = m_nCOL_CHANNEL_START + m_nChannelColumnCount;
    m_nCOL_LADDER = m_nCOL_AFTER_CHANNEL;
    m_nCOL_ILS = m_nCOL_LADDER + 1;
    m_nCOL_COUNT = m_nCOL_ILS + 1;
    _Build();
  }
}


void CGridRFULimits::_SetRFUColumn(CLabRFU *pRFU, int nCol)
{
  _SetCellIntValue(ROW_RFU_MIN,       nCol,pRFU->GetMinRFU());
  _SetCellIntValue(ROW_RFU_INTERLOCUS,nCol,pRFU->GetMinRFUinterlocus());
  _SetCellIntValue(ROW_RFU_MAX,       nCol,pRFU->GetMaxRFU());
  _SetCellIntValue(ROW_DETECTION,     nCol,pRFU->GetMinDetection());
}
void CGridRFULimits::_GetRFUColumn(CLabRFU *pRFU, int nCol)
{
  pRFU->SetMinRFU(_GetCellIntValue(ROW_RFU_MIN, nCol));
  pRFU->SetMinRFUinterlocus(_GetCellIntValue(ROW_RFU_INTERLOCUS,nCol));
  pRFU->SetMaxRFU(_GetCellIntValue(ROW_RFU_MAX,nCol));
  pRFU->SetMinDetection(_GetCellIntValue(ROW_DETECTION,nCol));
}

bool CGridRFULimits::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    int nCol;
    _ClearChannelColumns();
    _SetRFUColumn(m_pData->GetRFUsample(),m_nCOL_SAMPLE);
    _SetRFUColumn(m_pData->GetRFUladder(),m_nCOL_LADDER);
    _SetRFUColumn(m_pData->GetRFUls(),m_nCOL_ILS);

    CLabSampleChannelThresholdSet *pChannelSet = m_pData->GetChannelThresholds();
    CLabSampleChannelThresholdSet::iterator itr;
    for (itr = pChannelSet->begin(); itr != pChannelSet->end(); ++itr)
    {
      nCol = (*itr)->GetChannel() - 1 + m_nCOL_CHANNEL_START;
      _SetCellIntValue(ROW_RFU_MIN,nCol,(*itr)->GetMin());
      _SetCellIntValue(ROW_DETECTION,nCol,(*itr)->GetDetection());
    }
  }
  return bRtn;
}
bool CGridRFULimits::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    _GetRFUColumn(m_pData->GetRFUsample(),m_nCOL_SAMPLE);
    _GetRFUColumn(m_pData->GetRFUladder(),m_nCOL_LADDER);
    _GetRFUColumn(m_pData->GetRFUls(),m_nCOL_ILS);

    CLabSampleChannelThresholdSet *pChannelSet = m_pData->GetChannelThresholds();
    int nCol = m_nCOL_CHANNEL_START;
    ChannelNumberIterator ndx; 
    CLabSampleChannelThreshold xCh;
    pChannelSet->Clear();
    for (nCol = m_nCOL_CHANNEL_START, ndx = m_vnChannelNumbers.begin();
        (nCol < m_nCOL_AFTER_CHANNEL) && (ndx != m_vnChannelNumbers.end());
        ++nCol, ++ndx)
    {
      xCh.SetChannel(*ndx);
      xCh.SetMin(_GetCellIntValue(ROW_RFU_MIN,nCol));
      xCh.SetDetection(_GetCellIntValue(ROW_DETECTION,nCol));
      if(!xCh.Skip())
      {
        pChannelSet->Insert(xCh);
      }
    }
    if( (nCol != m_nCOL_AFTER_CHANNEL) || (ndx != m_vnChannelNumbers.end()) )
    {
      wxString s;
      int nColCount = m_nCOL_AFTER_CHANNEL - m_nCOL_CHANNEL_START;
      int nChannelCount = (int)m_vnChannelNumbers.size();
      s.Printf("Number of channels, %d, does not match number of channel columns, %d",nChannelCount,nColCount);
      wxASSERT_MSG(0,s);
      mainApp::LogMessage(s);
    }
  }
  return bRtn;
}

