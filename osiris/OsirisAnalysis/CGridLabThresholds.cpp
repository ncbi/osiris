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
*  FileName: CGridLabThresholds.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CGridLocusColumns.h"
#include "CGridLabThresholds.h"
#include "CLabSettings.h"
#include "mainApp.h"
#include "CKitColors.h"
#include <stdlib.h>

CGridLabThresholds::CGridLabThresholds(
  int nRows,  // row count including channel row
  bool bILS, 
  wxWindow *parent, wxWindowID nID) :
    nwxGrid(parent,nID),
    m_pData(NULL),
    m_nRows(nRows),
    m_nILScolumn(-1),
    m_nILSchannel(0),
    m_bILS(bILS),
    m_bReadOnly(false),
    m_bCreated(false)
{
  _CreateGrid(m_nRows,1);
  SetDefaultCellValidator(
    new nwxGridCellDoubleRangeValidator(0.0,1.0,true));
  SetDefaultEditor(new wxGridCellFloatEditor(-1,4));

}

wxPen CGridLabThresholds::GetColGridLinePen(int nCol)
{
  int nBlackCol = GetNumberCols() - (m_bILS ? 2 : 1);
  if(nCol >= nBlackCol)
  {
    return *wxBLACK_PEN;
  }
  return wxGrid::GetColGridLinePen(nCol);
}
wxPen CGridLabThresholds::GetRowGridLinePen(int nRow)
{
  int nBlackRow = GetNumberRows() - 1;
  if((!nRow) || (nRow == nBlackRow))
  {
    return *wxBLACK_PEN;
  }
  return wxGrid::GetRowGridLinePen(nRow);
}
void CGridLabThresholds::_CreateGrid(int nRows, int nCols)
{
  int nPrevCols = 0;
  int nPrevRows = 0;
  int nCol;
  if(!m_bCreated)
  {
    m_bCreated = true;
    CreateGrid(nRows,nCols);
    SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    EnableDragColSize(false);
    EnableDragRowSize(false);
    SetDefaultCellAlignment(wxALIGN_RIGHT,wxALIGN_CENTRE);
  }
  else
  {
    nPrevRows = GetNumberRows();
    nPrevCols = GetNumberCols();
    nwxGrid::SetRowColCount(this,nRows,nCols);
  }
  for(nCol = nPrevCols; nCol < nCols; nCol++)
  {
    // set alignment for Channel row
    SetCellAlignment(0,nCol,wxALIGN_CENTRE, wxALIGN_CENTRE);
  }
}

void CGridLabThresholds::_ClearColumn(int nCol)
{
  for(int nRow = 1; nRow < m_nRows; nRow++)
  {
    SetCellValue(nRow,nCol,wxEmptyString);
  }
}


bool CGridLabThresholds::_SetData(
  CLabThresholds *pData, 
  const wxString &sKitName,
  const wxChar * const *psLabels)
{
  m_pData = pData;
  m_sKitName = sKitName;
  bool bOK = CGridLocusColumns::SetupKit(
    this,sKitName,true,m_bILS,true,true,&m_nILScolumn,&m_nILSchannel);

      // set row labels
  if(bOK)
  {
    const wxChar * const *pRowLabel;
    int nColCount = GetNumberCols();
    int j = 1;
    for(pRowLabel = psLabels; (*pRowLabel) != NULL; pRowLabel++)
    {
      SetRowLabelValue(j++,*pRowLabel);
    }
    // put junk in row 1 to set cell width
    _CreateGrid(m_nRows,nColCount);
    for(j = 0; j < nColCount ; j++)
    {
      SetCellValue(1,j,"0000000000");  // set cell width
    }
    nwxGrid::UpdateLabelSizes(this);
    AutoSize();
  }
  return bOK;
}
bool CGridLabThresholds::SetILSDyeName(const wxString &sDyeName)
{
  bool bRtn = false;
  if(m_nILSchannel > 0 && m_nILScolumn >= 0)
  {
    wxString sILSdye;
    wxString sCurrent = GetCellValue(0,m_nILScolumn);
    CGridLocusColumns::FORMAT_CHANNEL_DYE(&sILSdye,m_nILSchannel,sDyeName);
    if(sCurrent != sILSdye)
    {
      CKitColors *pColors = mainApp::GetKitColors();
      const wxColour &color = pColors->GetColorByDye(sDyeName);
      SetCellValue(0,m_nILScolumn,sILSdye);
      SetCellBackgroundColour(0,m_nILScolumn,color);
      bRtn = true;
    }
  }
  return bRtn;
}


void CGridLabThresholds::SetAllReadOnly(bool bReadOnly)
{
  m_bReadOnly = bReadOnly;
  CGridLocusColumns::SetReadOnly(this,bReadOnly);
}

void CGridLabThresholds::DisableCell(int nRow, int nCol)
{
  SetReadOnly(nRow,nCol,true);
  SetCellBackgroundColour(nRow,nCol,
    GetGridLineColour());
  SetCellValue(nRow,nCol,wxEmptyString);
}


const wxChar * const CGridLabThresholds::FRACTION_MAX_PEAK =
  wxS("Fractional filter (0 - 1.0) ");
const wxChar * const CGridLabThresholds::PULLUP_FRACTIONAL_FILTER =
  wxS("Pullup fractional filter (0 - 1.0) ");
const wxChar * const CGridLabThresholds::STUTTER_THRESHOLD =
  wxS("Max. stutter threshold (0 - 1.0) ");
const wxChar * const CGridLabThresholds::PLUS_STUTTER_THRESHOLD =
  wxS("Max. plus stutter threshold (0 - 1.0) ");
const wxChar * const CGridLabThresholds::ADENYLATION_THRESHOLD =
  wxS("Adenylation threshold (0 - 1.0) ");

double CGridLabThresholds::GetCellValueDouble(int nRow, int nCol)
{
  wxString s = GetCellValue(nRow,nCol);
  double d;
  if(s.IsEmpty())
  {
    d = -1.0;
  }
  else
  {
    d = nwxRound::Round(atof(s.utf8_str()) * 10000.0);
    d *= 0.0001;
  }
  return d;
}
void CGridLabThresholds::SetCellValueDouble(
  int nRow, int nCol, double d)
{
  wxString s;
  if(d != -1.0)
  {
    s = nwxString::FormatDouble(d,"%g",false);
  }
  SetCellValue(nRow,nCol,s);
}

//**************************************************************
//
//    CGridLabThresholdsSample
//

CGridLabThresholdsSample::CGridLabThresholdsSample(
  wxWindow *parent,
  wxWindowID nID) :
    CGridLabThresholds(
      ROWS,
      false, parent, nID)
{
  SetRowCellValidator(
    new nwxGridCellUIntRangeValidator(10,100000,true),
    ROW_MIN_BOUND_HOMOZYGOTE);
}

bool CGridLabThresholdsSample::SetData(CLabThresholds *pData,
    const wxString &sKitName)
{
  const wxChar * const psLabels[] = 
  {
    FRACTION_MAX_PEAK,
    PULLUP_FRACTIONAL_FILTER,
    STUTTER_THRESHOLD,
    PLUS_STUTTER_THRESHOLD,
    ADENYLATION_THRESHOLD,
    wxS("Min. heterozygote balance (0 - 1.0) "),
    wxS("Min. homozygote threshold (RFU) "),
    NULL
  };
  bool bOK = _SetData(pData,sKitName,psLabels);
  if(bOK)
  {
    int nCols = GetNumberCols();
    int i;
    for(i = 0; i < nCols; i++)
    {
      SetCellEditor(
        ROW_MIN_BOUND_HOMOZYGOTE,i,new wxGridCellFloatEditor(-1,0));
    }
  }
  return bOK;
}

bool CGridLabThresholdsSample::_GetColumn(int nCol, CLabLocusThreshold *pLocus)
{
  wxString s;
  pLocus->Init();
  pLocus->SetLocusName(GetColLabelValue(nCol));
  pLocus->SetFractionMaxPeak(
    GetCellValueDouble(ROW_FRACTION_MAX_PEAK,nCol));
  pLocus->SetPullupFractionFilter(
    GetCellValueDouble(ROW_PULLUP_FRACTIONAL_FILTER,nCol));
  pLocus->SetStutter(
    GetCellValueDouble(ROW_STUTTER_THRESHOLD,nCol));
  pLocus->SetPlusStutter(
    GetCellValueDouble(ROW_PLUS_STUTTER_THRESHOLD,nCol));
  pLocus->SetAdenylation(
    GetCellValueDouble(ROW_ADENYLATION_THRESHOLD,nCol));
  pLocus->SetHeterozygousImbalanceLimit(
    GetCellValueDouble(ROW_HETEROZYGOUS_IMBALANCE_LIMIT,nCol));
  pLocus->SetMinBoundForHomozygote(
    GetCellValueDouble(ROW_MIN_BOUND_HOMOZYGOTE,nCol));

  // need validation code here

  return true;
}
void CGridLabThresholdsSample::_SetColumn(
  int nCol, const CLabLocusThreshold &locus)
{
  SetCellValueDouble(ROW_FRACTION_MAX_PEAK,
    nCol,locus.GetFractionMaxPeak());
  SetCellValueDouble(ROW_PULLUP_FRACTIONAL_FILTER,
    nCol,locus.GetPullupFractionFilter());
  SetCellValueDouble(ROW_STUTTER_THRESHOLD,
    nCol,locus.GetStutter());
  SetCellValueDouble(ROW_PLUS_STUTTER_THRESHOLD,
    nCol,locus.GetPlusStutter());
  SetCellValueDouble(ROW_ADENYLATION_THRESHOLD,
    nCol,locus.GetAdenylation());
  SetCellValueDouble(ROW_HETEROZYGOUS_IMBALANCE_LIMIT,
    nCol,locus.GetHeterozygousImbalanceLimit());
  SetCellValueDouble(ROW_MIN_BOUND_HOMOZYGOTE,
    nCol,locus.GetMinBoundForHomozygote());
}

bool CGridLabThresholdsSample::TransferDataFromWindow()
{
  CLabLocusThreshold LocusLimits;
  bool bRtn = 
    (m_pData != NULL) && _GetColumn(0,&LocusLimits);
  if(bRtn)
  {
    CLabRFU &rfu(*m_pData->GetRFUsample());

    // setup defaults

    m_pData->SetHeterozygousImbalanceLimit(
      LocusLimits.GetHeterozygousImbalanceLimit());
    m_pData->SetMinBoundForHomozygote(
      LocusLimits.GetMinBoundForHomozygote());
    rfu.SetFractionMaxPeak(
      LocusLimits.GetFractionMaxPeak());
    rfu.SetPullupFractionFilter(
      LocusLimits.GetPullupFractionFilter());
    rfu.SetStutterThreshold(
      LocusLimits.GetStutter());
    rfu.SetPlusStutterThreshold(
      LocusLimits.GetPlusStutter());
    rfu.SetAdenylationThreshold(
      LocusLimits.GetAdenylation());

    // Setup Loci

    int n = GetNumberCols();
    for(int i = 1; i < n; i++)
    {
      if(_GetColumn(i,&LocusLimits))
      {
        rfu.SetLocusThreshold(LocusLimits);
      }
      else
      {
        bRtn = false;
        i = n; // loop exit
      }
    }
  }
  return bRtn;
}

bool CGridLabThresholdsSample::TransferDataToWindow()
{
  int n = GetNumberCols();
  bool bRtn = (m_pData != NULL) && (n > 1);
  if(bRtn)
  {
    wxString sLocus;
    CLabLocusThreshold locus;
    const CLabRFU &rfu(*m_pData->GetRFUsample());
    const CLabLocusThreshold *pLocus;
    
    // setup defaults
    locus.SetFractionMaxPeak(rfu.GetFractionMaxPeak());
    locus.SetPullupFractionFilter(rfu.GetPullupFractionFilter());
    locus.SetStutter(rfu.GetStutterThreshold());
    locus.SetPlusStutter(rfu.GetPlusStutterThreshold());
    locus.SetAdenylation(rfu.GetAdenylationThreshold());
    locus.SetHeterozygousImbalanceLimit(
      m_pData->GetHeterozygousImbalanceLimit());
    locus.SetMinBoundForHomozygote(
      m_pData->GetMinBoundForHomozygote());
    _SetColumn(0,locus);
    locus.Init();
    for(int i = 1; i < n; i++)
    {
      sLocus = GetColLabelValue(i);
      pLocus = rfu.GetLabLocusThreshold(sLocus);
      if(pLocus != NULL)
      {
        _SetColumn(i,*pLocus);
      }
      else
      {
        _ClearColumn(i);
      }
    }
  }
  return bRtn;
}

//**************************************************************
//
//    CGridLabThresholdsLadder
//

CGridLabThresholdsLadder::CGridLabThresholdsLadder(
  wxWindow *parent, wxWindowID nID) 
  : CGridLabThresholds(ROWS,true,parent,nID)
{}

bool CGridLabThresholdsLadder::SetData(
  CLabThresholds *pData, const wxString &sKitName)
{
  const wxChar * const psLabels[] = 
  {
    FRACTION_MAX_PEAK,
    PULLUP_FRACTIONAL_FILTER,
    STUTTER_THRESHOLD,
    ADENYLATION_THRESHOLD,
    NULL
  };
  bool bOK = _SetData(pData,sKitName,psLabels);
  return bOK;
}
bool CGridLabThresholdsLadder::_GetColumn(
  int nCol, CLabLocusThreshold *pLocus)
{
  wxString s;
  pLocus->Init();
  pLocus->SetLocusName(GetColLabelValue(nCol));
  pLocus->SetFractionMaxPeak(
    GetCellValueDouble(ROW_FRACTION_MAX_PEAK,nCol));
  pLocus->SetPullupFractionFilter(
    GetCellValueDouble(ROW_PULLUP_FRACTIONAL_FILTER,nCol));
  pLocus->SetStutter(
    GetCellValueDouble(ROW_STUTTER_THRESHOLD,nCol));
  pLocus->SetAdenylation(
    GetCellValueDouble(ROW_ADENYLATION_THRESHOLD,nCol));

  // need validation code here

  return true;
}
void CGridLabThresholdsLadder::_SetColumn(
  int nCol, const CLabLocusThreshold &locus)
{
  SetCellValueDouble(ROW_FRACTION_MAX_PEAK,
    nCol,locus.GetFractionMaxPeak());
  SetCellValueDouble(ROW_PULLUP_FRACTIONAL_FILTER,
    nCol,locus.GetPullupFractionFilter());
  SetCellValueDouble(ROW_STUTTER_THRESHOLD,
    nCol,locus.GetStutter());
  SetCellValueDouble(ROW_ADENYLATION_THRESHOLD,
    nCol,locus.GetAdenylation());
}

bool CGridLabThresholdsLadder::TransferDataFromWindow()
{
  bool bRtn = m_pData != NULL;
  if(bRtn)
  {
    CLabLocusThreshold LocusLimits;
    int nILS = GetNumberCols() - 1;
    int nCol;

    CLabRFU &rfu(*m_pData->GetRFUladder());
    CLabRFU &rfuILS(*m_pData->GetRFUls());
    CLabRFU *pRFU = &rfu;

    // get defaults and ILS
    for(nCol = 0; bRtn && (nCol <= nILS); nCol += nILS)
    {
      _GetColumn(nCol,&LocusLimits);
      // setup defaults

      pRFU->SetFractionMaxPeak(
        LocusLimits.GetFractionMaxPeak());
      pRFU->SetPullupFractionFilter(
        LocusLimits.GetPullupFractionFilter());
      pRFU->SetStutterThreshold(
        LocusLimits.GetStutter());
      pRFU->SetAdenylationThreshold(
        LocusLimits.GetAdenylation());

      //  perform validation here

      pRFU = &rfuILS;
    }
    for(nCol = 1; nCol < nILS; nCol++)
    {

      if(_GetColumn(nCol,&LocusLimits))
      {
        rfu.SetLocusThreshold(LocusLimits);
      }
      else
      {
        bRtn = false;
        nCol = nILS; // loop exit
      }
    }
  }
  return bRtn;
}

bool CGridLabThresholdsLadder::TransferDataToWindow()
{
  bool bRtn = m_pData != NULL;
  if(bRtn)
  {

    wxString sLocus;
    CLabLocusThreshold locus;
    const CLabLocusThreshold *pLocus;
    int nILS = GetNumberCols() - 1;
    int nCol;
    const CLabRFU &rfu(*m_pData->GetRFUladder());
    const CLabRFU &rfuILS(*m_pData->GetRFUls());
    const CLabRFU *pRFU = &rfu;

    for(nCol = 0; nCol <= nILS; nCol += nILS)
    {
      // setup defaults
      locus.SetFractionMaxPeak(pRFU->GetFractionMaxPeak());
      locus.SetPullupFractionFilter(pRFU->GetPullupFractionFilter());
      locus.SetStutter(pRFU->GetStutterThreshold());
      locus.SetAdenylation(pRFU->GetAdenylationThreshold());
      _SetColumn(nCol,locus);
      pRFU = &rfuILS;
    }

    for(nCol = 1; nCol < nILS; nCol++)
    {
      sLocus = GetColLabelValue(nCol);
      pLocus = rfu.GetLabLocusThreshold(sLocus);
      if(pLocus != NULL)
      {
        _SetColumn(nCol,*pLocus);
      }
      else
      {
        _ClearColumn(nCol);
      }
    }
  }
  return bRtn;
}
