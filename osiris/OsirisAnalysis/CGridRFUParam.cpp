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
*  FileName: CGridRFUParam.cpp
*  Author:   Douglas Hoffman
*
*  wxGrid for viewing RFU limits for sample, ILS, channel,
*    in the Parameters window (CDialogParameters.cpp/h)
*
*  This is inherited by CGridRFURun, used for running a new analysis
*  and CGridRFUParam, used for showing runtime parameters
*  The main difference is the former loads data from 
*
*/
#include "CGridRFUParam.h"
#include "CParmOsiris.h"

bool CGridRFUParam::TransferDataToWindow()
{
  bool bRtn = (m_pParms != NULL) && !m_sKitName.IsEmpty();
  if(bRtn)
  {
    nwxLabelGridBatch x(this);
    const vector<int> &anChannelDetection = m_pParms->GetChannelDetection();
    const vector<int> &anChannelRFU = m_pParms->GetChannelRFU();
    int nMinRFU = m_pParms->GetMinRFU_Sample();
    int nMinDetection = m_pParms->GetSampleDetectionThreshold();
    ClearGrid();
    _TransferToChannelRows(anChannelRFU,anChannelDetection, nMinRFU, nMinDetection);

    _SetCellIntValue(m_nROW_SAMPLE,COL_ANALYSIS,nMinRFU);
    _SetCellIntValue(m_nROW_SAMPLE,COL_INTERLOCUS,m_pParms->GetMinRFU_Interlocus());
    _SetCellIntValue(m_nROW_SAMPLE,COL_DETECTION,nMinDetection);
    _SetCellIntValue(m_nROW_LADDER,COL_ANALYSIS,m_pParms->GetMinRFU_Ladder());
    _SetCellIntValue(m_nROW_LADDER,COL_INTERLOCUS,m_pParms->GetMinLadderInterlocusRFU());
    _SetCellIntValue(m_nROW_ILS,COL_ANALYSIS,m_pParms->GetMinRFU_ILS());
  }
  return bRtn;
}