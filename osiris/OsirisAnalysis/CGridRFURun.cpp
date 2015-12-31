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
*  FileName: CGridRFURun.cpp
*  Author:   Douglas Hoffman
*
*  wxGrid for entering and/or viewing RFU limits for sample, ILS, channel,
*    for an osiris analysis
*
*/
#include "CGridRFURun.h"
#include "CDialogAnalysis.h"

CGridRFURun::CGridRFURun(CDialogAnalysis *parent,wxWindowID id) :
  CGridRFURunBase(parent,id),
  m_pParent(parent)
{
  SetDefaultCellValidator(
    new nwxGridCellUIntRangeValidator(
      mainApp::RFU_MIN_ENTER,mainApp::RFU_MAX_ENTER,true));
  nwxGridCellUIntRangeValidator *pVreqd =
    new nwxGridCellUIntRangeValidator(
      mainApp::RFU_MIN_ENTER,mainApp::RFU_MAX_ENTER,false); // does not allow empty

  SetCellValidator(pVreqd,m_nROW_SAMPLE,COL_ANALYSIS);
  SetCellValidator(pVreqd,m_nROW_LADDER,COL_ANALYSIS);
  SetCellValidator(pVreqd,m_nROW_ILS,COL_ANALYSIS);
  SetCellValidator(pVreqd,m_nROW_SAMPLE,COL_INTERLOCUS);
  SetCellValidator(pVreqd,m_nROW_LADDER,COL_INTERLOCUS);
}


bool CGridRFURun::TransferDataFromWindow()
{
  vector<int> anChannelDetection;
  vector<int> anChannelRFU;
  ChannelNumberIterator itrChannelCol;
  int nRow;
  bool bRtn = true;
  
  anChannelDetection.reserve(m_vnChannelNumbers.size());
  anChannelRFU.reserve(m_vnChannelNumbers.size());
  for(itrChannelCol = m_vnChannelNumbers.begin(), 
          nRow = m_nROW_CHANNEL_START;
    itrChannelCol != m_vnChannelNumbers.end();
    ++itrChannelCol, ++nRow)
  {
    SetVectorChannel(
      &anChannelDetection,
      (*itrChannelCol),
      _GetCellIntValue(nRow,COL_DETECTION));
    SetVectorChannel(
      &anChannelRFU,
      (*itrChannelCol),
      _GetCellIntValue(nRow,COL_ANALYSIS));
  }

  m_pParent->SetChannelDetection(anChannelDetection);
  m_pParent->SetChannelRFU(anChannelRFU);

  m_pParent->SetMinRFU(_GetCellIntValue(m_nROW_SAMPLE,COL_ANALYSIS));
  m_pParent->SetMinRFU_SampleDetection(_GetCellIntValue(m_nROW_SAMPLE,COL_DETECTION));
  m_pParent->SetMinRFU_Interlocus(_GetCellIntValue(m_nROW_SAMPLE,COL_INTERLOCUS));
  m_pParent->SetMinRFU_Ladder(_GetCellIntValue(m_nROW_LADDER,COL_ANALYSIS));
  m_pParent->SetMinRFU_LadderInterlocus(_GetCellIntValue(m_nROW_LADDER,COL_INTERLOCUS));
  m_pParent->SetMinRFU_ILS(_GetCellIntValue(m_nROW_ILS,COL_ANALYSIS));

  return bRtn;
}
bool CGridRFURun::TransferDataToWindow()
{
  const vector<int> &anChannelDetection = m_pParent->GetChannelDetection();
  const vector<int> &anChannelRFU = m_pParent->GetChannelRFU();
  bool bRtn = true;
  ClearGrid();
  _TransferToChannelRows(anChannelRFU,anChannelDetection);

  _SetCellIntValue(m_nROW_SAMPLE,COL_ANALYSIS,m_pParent->GetMinRFU());
  _SetCellIntValue(m_nROW_SAMPLE,COL_INTERLOCUS,m_pParent->GetMinRFU_Interlocus());
  _SetCellIntValue(m_nROW_SAMPLE,COL_DETECTION,m_pParent->GetMinRFU_SampleDetection());
  _SetCellIntValue(m_nROW_LADDER,COL_ANALYSIS,m_pParent->GetMinRFU_Ladder());
  _SetCellIntValue(m_nROW_LADDER,COL_INTERLOCUS,m_pParent->GetMinRFU_LadderInterlocus());
  _SetCellIntValue(m_nROW_ILS,COL_ANALYSIS,m_pParent->GetMinRFU_ILS());

  return bRtn;
}
