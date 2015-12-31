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
*  FileName: CGridRFURunBase.cpp
*  Author:   Douglas Hoffman
*
*  wxGrid for entering and/or viewing RFU limits for sample, ILS, channel,
*    for an osiris analysis
*
*  This is inherited by CGridRFURun, used for running a new analysis
*  and CGridRFUParam, used for showing runtime parameters
*  The main difference is the former loads data from 
*
*/
#include "CGridRFURunBase.h"
#include "CGridLocusColumns.h"
#include "CDialogAnalysis.h"

CGridRFURunBase::CGridRFURunBase(wxWindow *parent,wxWindowID id) :
  CGridRFUbase(parent,id),
  m_nROW_SAMPLE(0),
  m_nROW_CHANNEL_START(1),
  m_nROW_AFTER_CHANNEL(-1),
  m_nROW_LADDER(-1),
  m_nROW_ILS(-1),
  m_nROW_COUNT(-1)
{
  CreateGrid(7,COL_COUNT);
}


void CGridRFURunBase::_Build()  // called from _SetupKit();
{
  wxFont fontChannel = GetDefaultCellFont();
  wxFont fontDefault = fontChannel;
  fontChannel.SetWeight(wxFONTWEIGHT_BOLD);
  const CChannelColors *pChannelColors = NULL;
  int nCurrentRowCount = GetNumberRows();
  int i;
  int j;
  if(nCurrentRowCount < m_nROW_COUNT)
  {
    InsertRows(m_nROW_CHANNEL_START, m_nROW_COUNT - nCurrentRowCount);
    _UpdateReadOnly();
  }
  else if(nCurrentRowCount > m_nROW_COUNT)
  {
    DeleteRows(m_nROW_CHANNEL_START,nCurrentRowCount - m_nROW_COUNT);
  }
  SetDefaultCellValidator(
    new nwxGridCellUIntRangeValidator(
      mainApp::RFU_MIN_ENTER,mainApp::RFU_MAX_ENTER,true));
  EnableDragColSize(false);
  EnableDragRowSize(false);
  SetDefaultCellAlignment(wxALIGN_RIGHT,wxALIGN_CENTRE);
  for(i = 0; i < m_nROW_COUNT; i++)
  {
    for(j = 0; j < COL_COUNT; j++)
    {
      SetCellValue(i,j,"00000000"); // used for size
      if(_DisabledCell(i,j))
      {
        SetCellBackgroundColour(i,j,GetGridLineColour());
      }
    }
  }
  SetDefaultEditor(new wxGridCellFloatEditor(1,0));
  SetRowLabelValue(m_nROW_SAMPLE,"Sample");
  SetRowLabelValue(m_nROW_LADDER,"Ladder");
  SetRowLabelValue(m_nROW_ILS,"   ILS   ");

  SetColLabelValue(COL_ANALYSIS,"Analysis");
  SetColLabelValue(COL_DETECTION,"Detection");
  SetColLabelValue(COL_INTERLOCUS,"Interlocus");
  SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
  SetMargins(0,0);
  ChannelNumberIterator itrChannelCol;
  int nRow;
  const wxChar *psDye = NULL;
  wxString sLabel;
  for(itrChannelCol = m_vnChannelNumbers.begin(), 
          nRow = m_nROW_CHANNEL_START;
    itrChannelCol != m_vnChannelNumbers.end();
    ++itrChannelCol, ++nRow)
  {
    if(m_pKitColors != NULL)
    {
      pChannelColors = m_pKitColors->GetColorChannel(*itrChannelCol);
      psDye = (pChannelColors == NULL) ? NULL : (const wxChar *) pChannelColors->m_sDyeName;
    }
    CGridLocusColumns::FORMAT_CHANNEL_DYE(&sLabel,*itrChannelCol,psDye);
    SetRowLabelValue(nRow,sLabel);
    if(pChannelColors != NULL)
    {
      _SetupChannelRow(nRow,pChannelColors->m_ColorAnalyzed,fontChannel);
    }
    else
    {
      _SetupDefaultChannelRow(nRow);
    }
  }
  nwxGrid::UpdateLabelSizes(this);
  AutoSize();
  _DisableUnused();
}
bool CGridRFURunBase::SetupKit(const wxString &sKitName)
{
  bool bRtn = _SetupKitBase(sKitName);
  if(bRtn)
  {
    m_nROW_AFTER_CHANNEL = m_nROW_CHANNEL_START + m_nChannelColumnCount;
    m_nROW_LADDER = m_nROW_AFTER_CHANNEL;
    m_nROW_ILS = m_nROW_LADDER + 1;
    m_nROW_COUNT = m_nROW_ILS + 1;
    _Build();
  }
  return bRtn;
}
bool CGridRFURunBase::_DisabledCell(int nRow, int nCol)
{
  bool bRtn = false;
  switch(nCol)
  {
  case (int) COL_ANALYSIS:
    //  always enabled
    break;
  case (int) COL_DETECTION:
    bRtn = (nRow == m_nROW_LADDER) ||
        (nRow == m_nROW_ILS);
    break;
  case (int) COL_INTERLOCUS:
    bRtn = (nRow != m_nROW_SAMPLE) &&
      (nRow != m_nROW_LADDER);
    break;
  }
  return bRtn;
}

void CGridRFURunBase::_TransferToChannelRows(
    const vector<int> &anChannelRFU, 
    const vector<int> &anChannelDetection)
{
  if(!( anChannelDetection.empty() && anChannelRFU.empty() ))
  {
    ChannelNumberIterator itrChannelCol;
    int nRow;

    for(itrChannelCol = m_vnChannelNumbers.begin(), 
            nRow = m_nROW_CHANNEL_START;
      itrChannelCol != m_vnChannelNumbers.end();
      ++itrChannelCol, ++nRow)
    {
      _SetCellIntValue( nRow,COL_DETECTION,
        GetVectorChannel(anChannelDetection,(*itrChannelCol)) );
      _SetCellIntValue( nRow,COL_ANALYSIS,
        GetVectorChannel(anChannelRFU,(*itrChannelCol)) );
    }
  }
}
