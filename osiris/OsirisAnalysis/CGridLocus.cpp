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
*  FileName: CGridLocus.cpp
*  Author:   Douglas Hoffman
*
*   this is used for older file versions where the user could not edit artifacts
*   the newer grid is CGridLocusPeaks
*/
#include "mainApp.h"
#include "CGridLocus.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxString.h"
#include "nwx/nwxGrid.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

typedef enum
{
  ROW_ALLELE = 0,
  ROW_BPS,
  ROW_OFF_LADDER,
  ROW_HOMOZYGOUS,
  ROW_RFU,
  ROW_TIME,
  ROW_PEAK_AREA,
  ROW_DISABLE
} ROW_NUMBER;

// read only row numbers
#define ROW_RO_FIRST ROW_RFU
#define ROW_RO_LAST  ROW_PEAK_AREA

static const char *labels[] = 
{
  "Allele",
  "BPS",
  "Off Ladder",
  "Homozygote",
  "RFU",
  "Time",
  "Peak Area",
  "Disabled"
};

static const int nLABELS = sizeof(labels) / sizeof(labels[0]);

CGridLocus::CGridLocus(
  COARlocus *pLocusEdit,
  wxWindow *parent,
  wxWindowID id,
  bool bReadOnly) : 
    _CGridEdit(parent,id,bReadOnly),
    m_pLocusEdited(pLocusEdit)
{
  wxSize sz;
  size_t nCount = m_pLocusEdited->AlleleCount();
  size_t j;
  int nCol = nCount ? (int) nCount : 1;
  int nRow = nCount ? nLABELS : 1;
  int i;
  int k;

  CreateGrid(nRow,nCol);
  if(!nCount)
  {
    SetRowLabelValue(0,wxEmptyString);
    SetColLabelValue(0,wxEmptyString);
    SetReadOnly(0,0,true);
    SetReadOnly(0,1,true);
    SetReadOnly(1,0,true);
    SetReadOnly(1,1,true);
  }
  else
  {
    bool bAmel = m_pLocusEdited->IsAmel();
    for(i = 0; i < nRow; i++)
    {
      s = labels[i];
      SetRowLabelValue(i,s);
      SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    }
    for(j = 0; j < nCount; j++)
    {
      k = (int)j;
      _ImplementColumn(k,bAmel);
    }
    TransferDataToWindow();
    nwxGrid::UpdateLabelSizes(this);
  }
  DisableDragRowSize();
  DisableDragColSize();
  SetColLabelSize(GetRowSize(0));
  s.Empty();
}

void CGridLocus::_ImplementColumn(int nCol,bool bAmel)
{
  int nStart = nCol;
  int nStop = nCol;
  int i;
  int k;
  int n = GetNumberCols();
  if(nCol >= n)
  {
    AppendCols(nCol - n + 1);
    nStart = n;
  }
  for(k = nStart; k <= nStop; ++k)
  {
    s.Printf("Peak %d",k+1);
    SetColLabelValue(k,s);
    for(i = ROW_RO_FIRST; i <= ROW_RO_LAST; i++)
    {
      DisableEdit(i,k);
    }
    SetBoolCell(ROW_HOMOZYGOUS,k);
    SetBoolCell(ROW_OFF_LADDER,k);
    SetBoolCell(ROW_DISABLE,k);

    SetCellEditor(ROW_BPS,k,new wxGridCellFloatEditor(12,7));
    SetCellRenderer(ROW_BPS,k,new wxGridCellFloatRenderer(12,7));
    if(!bAmel)
    {
      wxGridCellFloatEditor *pfe1(new wxGridCellFloatEditor(5,1));
      wxGridCellFloatRenderer *pfr1(new wxGridCellFloatRenderer(5,1));
      SetCellEditor(ROW_ALLELE,k,pfe1);
      SetCellRenderer(ROW_ALLELE,k,pfr1);
    }
    else
    {
      wxGridCellTextEditor *pe(new wxGridCellTextEditor);
      pe->SetParameters("1");
      SetCellEditor(ROW_ALLELE,k,pe);
    }

    SetCellAlignment(wxALIGN_RIGHT,  ROW_ALLELE,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_BPS,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_RFU,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_TIME,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_PEAK_AREA,k);
  }
}

void CGridLocus::_SetNumberCols(int n)
{
  int nCol = GetNumberCols();
  if(nCol == n) {;} // do nothing
  else if(nCol < n)
  {
    _ImplementColumn(n - 1);
  }
  else
  {
    DeleteCols(n,nCol - n);
  }
}

bool CGridLocus::TransferDataToWindow()
{
  nwxGridBatch xxxx(this);
  const COARallele *pAllele;
  bool bAmel = m_pLocusEdited->IsAmel();
  vector<const COARallele *> va;
  m_pLocusEdited->GetAllelesByTime(&va,m_dtHistory);
  int nCol = (int)(va.size());
  _SetNumberCols(nCol);
  for(int k = 0; k < nCol; k++)
  {
    pAllele = va.at(k);
    SetCellValue(ROW_ALLELE,     k, pAllele->FormatName(bAmel,false));
    SetCellValue(ROW_BPS,        k, pAllele->FormatBPS());
    SetCellValue(ROW_OFF_LADDER, k, pAllele->FormatOffLadder());
    SetCellValue(ROW_HOMOZYGOUS, k, pAllele->FormatCount());
    SetCellValue(ROW_RFU,        k, pAllele->FormatRFU());
    SetCellValue(ROW_TIME,       k, pAllele->FormatTime());
    SetCellValue(ROW_PEAK_AREA,  k, pAllele->FormatPeakArea());
    SetCellValue(ROW_DISABLE,    k, pAllele->FormatDisabled());
    UpdateDisabledAlleles(k);
  }
  AutoSizeColumns();
  return true;
}

bool CGridLocus::TransferCellFromWindow(int nRow, int nCol)
{
  COARallele *pAllele = m_pLocusEdited->GetAllele(nCol);
  bool bRtn = true;
  switch (nRow)
  {
  case ROW_ALLELE:
    bRtn = XferName(nCol,pAllele);
    break;
  case ROW_BPS:
    XferBPS(nCol,pAllele);
    break;
  case ROW_OFF_LADDER:
    XferOL(nCol,pAllele);
    break;
  case ROW_HOMOZYGOUS:
    XferHomozygous(nCol,pAllele);
    break;
  case ROW_DISABLE:
    XferDisabled(nCol,pAllele);
    UpdateDisabledAlleles(nCol);
    break;
  default:
    break;
  }
  return bRtn;
}
bool CGridLocus::XferName(int nCol, COARallele *pAllele)
{
  bool bRtn = true;
  if(!GetBoolValue(ROW_DISABLE,nCol))
  {
    bool bAmel = m_pLocusEdited->IsAmel();
    s = GetCellValue(ROW_ALLELE,nCol);
    nwxString::Trim(&s);
    if(bAmel)
    {
      s.MakeUpper();
      const char *ps = s.utf8_str();
      if( (!*ps) || (strchr("XY12",*ps) == NULL) )
      {
        // not x/y
        bRtn = false;
        mainApp::ShowError("Invalid allele name",this);
        SetCellValue(ROW_ALLELE,nCol,pAllele->FormatName(true,false));
        SetGridCursor(ROW_ALLELE,nCol);
        SelectBlock(ROW_ALLELE,nCol,ROW_ALLELE,nCol);
      }
    }
    if(bRtn)
    {
      pAllele->SetName(s,bAmel);
    }
  }
  return bRtn;
}

void CGridLocus::XferBPS(int nCol, COARallele *pAllele)
{
  if(!GetBoolValue(ROW_DISABLE,nCol))
  {
    s = GetCellValue(ROW_BPS,nCol);
    nwxString::Trim(&s);
    pAllele->SetBPS(atof(s.utf8_str()));
  }
}
void CGridLocus::XferHomozygous(int nCol, COARallele *pAllele)
{
  if(!GetBoolValue(ROW_DISABLE,nCol))
  {
    s = GetCellValue(ROW_HOMOZYGOUS,nCol);
    bool b = IsTrue(s.utf8_str());
    pAllele->SetCountBool(b);
  }
}
void CGridLocus::XferOL(int nCol, COARallele *pAllele)
{
  if(!GetBoolValue(ROW_DISABLE,nCol))
  {
    s = GetCellValue(ROW_OFF_LADDER,nCol);
    bool b = IsTrue(s.utf8_str());
    pAllele->SetOffLadderString(b ? IOARpeak::OL_TRUE : IOARpeak::OL_FALSE);
  }
}

void CGridLocus::XferDisabled(int nCol, COARallele *pAllele)
{
  s = GetCellValue(ROW_DISABLE,nCol);
  bool b = IsTrue(s.utf8_str());
  pAllele->SetDisabled(b);
}

bool CGridLocus::TransferColumnFromWindow(int nCol)
{
  COARallele *pAllele = m_pLocusEdited->GetAllele(nCol);
  bool b = XferName(nCol,pAllele);
  if(b)
  {
    XferBPS(nCol,pAllele);
    XferOL(nCol,pAllele);
    XferDisabled(nCol,pAllele);
    XferHomozygous(nCol,pAllele);
  }
  return b;
}


bool CGridLocus::TransferDataFromWindow()
{
  bool bRtn = true;
  if(!IsTableReadOnly())
  {
    int nCol = (int)m_pLocusEdited->AlleleCount();
    for(int k = 0; k < nCol; k++)
    {
      bRtn = TransferColumnFromWindow(k);
      if(!bRtn)
      {
        k = nCol; // loop exit
      }
    }
  }
  return bRtn;
}

void CGridLocus::UpdateDisabledAlleles(int nCol)
{
  bool b;
  nwxGridBatch xxxx(this);
  b = GetBoolValue(ROW_DISABLE,nCol);
  if(IsTableReadOnly())
  {
    SetGrayBackgroundCol(nCol,b);
  }
  else
  {
    DisableEdit(ROW_ALLELE,nCol,b);
    DisableEdit(ROW_BPS,nCol,b);
    DisableEdit(ROW_OFF_LADDER,nCol,b);
    DisableEdit(ROW_HOMOZYGOUS,nCol,b);
  }
  Refresh();
}


CGridLocus::~CGridLocus() {;}

void CGridLocus::OnCellChange(wxGridEvent &e)
{
  if(!IsTableReadOnly())
  {
    int nRow = e.GetRow();
    int nCol = e.GetCol();
    TransferCellFromWindow(nRow,nCol);
    e.Skip(true);
  }
}

BEGIN_EVENT_TABLE(CGridLocus,_CGridEdit)
EVT_GRID_CELL_CHANGE(CGridLocus::OnCellChange)
END_EVENT_TABLE()
