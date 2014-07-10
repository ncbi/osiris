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
*  FileName: CGridLocusPeaks.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CGridLocusPeaks.h"
#include "COARfile.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxString.h"
#include "nwx/nwxGrid.h"
#include "nwx/CIncrementer.h"
#include "nwx/vectorptr.h"
#include <wx/dcclient.h>
#include <wx/arrstr.h>

typedef enum
{
  ROW_ALLELE = 0,
  ROW_BPS,
  ROW_OFF_LADDER,
  ROW_HOMOZYGOUS,
  ROW_RFU,
  ROW_TIME,
  ROW_PEAK_AREA,
  ROW_FIT,
  ROW_IS_ALLELE,
  ROW_IS_ARTIFACT,
  ROW_IS_CRITICAL
} ROW_NUMBER;

// read only row numbers
#define ROW_RO_FIRST ROW_RFU
#define ROW_RO_LAST  ROW_FIT

static const wxChar *labels[] =
{
  wxS("Allele"),
  wxS("BPS"),
  wxS("Off Ladder"),
  wxS("Homozygote"),
  wxS("RFU"),
  wxS("Time"),
  wxS("Peak Area"),
  wxS("Fit"),
  wxS("Allele Peak"),
  wxS("Artifact Peak"),
  wxS("Critical")
};

static const int nLABELS = sizeof(labels) / sizeof(labels[0]);


class COLcellEditor : public wxGridCellChoiceEditor
{
public:
  static const wxArrayString &GetList()
  {
    if(!g_list.Count())
    {
      g_list.Alloc(3);
      g_list.Add(IOARpeak::OL_NO);
      g_list.Add(IOARpeak::OL_YES);
      g_list.Add(IOARpeak::OL_ACCEPTED);
    }
    return g_list;
  }

  COLcellEditor() : wxGridCellChoiceEditor(GetList())  {}
  virtual ~COLcellEditor() {}

private:
  static wxArrayString g_list;
};

wxArrayString COLcellEditor::g_list;

bool CGridLocusPeaks::IsColumnDisabled(int n)
{
  bool bEnabled = GetBoolValue(ROW_IS_ALLELE,n) ||
              GetBoolValue(ROW_IS_ARTIFACT,n);
  return !bEnabled;
}

CGridLocusPeaks::CGridLocusPeaks(
  const COARsample *pSample,
  int nChannel,
  const wxString &sLocusName,
  wxWindow *parent,
  wxWindowID id,
  bool bReadOnly,
  const wxDateTime *pTime) :
    _CGridEdit(parent,id,bReadOnly),
    m_sLocusName(sLocusName),
    m_pSample(pSample),
    m_pvpPeak(NULL),
    m_nChannel(nChannel),
    m_nOnCellChange(0),
    m_bAmel(false)
{
  m_bAmel = COARlocus::IsAmel(sLocusName);
  if(pTime != NULL)
  {
    m_dtHistory.SetDateTime(pTime);
  }
  _SetupPeaks();
  size_t nCount = (size_t)GetPeakCount();
  if(nCount)
  {
    wxString s;
//    wxSize sz;
//    size_t j;
//    int nWidth = 0;
//    int n;
//    int k;
    int i;

    CreateGrid(nLABELS,nCount);
//    pdc->SetFont(GetLabelFont());
    for(i = 0; i < nLABELS; i++)
    {
      s = labels[i];
//      sz = pdc->GetTextExtent(s);
//      n = sz.GetWidth();
//      if(n > nWidth) { nWidth = n; };
      SetRowLabelValue(i,s);
    }
//    SetRowLabelSize(nWidth + 6);
    SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    for(i = 0; i < (int)nCount; i++)
    {
      _ImplementColumn(i);
    }
  }
  else
  {
    CreateGrid(1,1);
  }
  TransferDataToWindow();
  DisableDragRowSize();
  DisableDragColSize();
  if(nCount)
  {
    nwxGrid::UpdateLabelSizes(this);
//    SetColLabelSize(GetRowSize(0));
  }
}

void CGridLocusPeaks::_ImplementColumn(int nCol)
{
  wxString s;
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
    SetCellEditor(ROW_OFF_LADDER,k,new COLcellEditor);

    SetBoolCell(ROW_IS_ALLELE,k);
    SetBoolCell(ROW_IS_ARTIFACT,k);
    SetBoolCell(ROW_IS_CRITICAL,k);

    if(!IsTableReadOnly())
    {
      SetCellEditor(ROW_BPS,k,new wxGridCellFloatEditor(8,3));
      SetCellRenderer(ROW_BPS,k,new wxGridCellFloatRenderer(8,3));
      if(!m_bAmel)
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
    }

    SetCellAlignment(wxALIGN_CENTRE, ROW_OFF_LADDER,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_ALLELE,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_BPS,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_RFU,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_TIME,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_PEAK_AREA,k);
    SetCellAlignment(wxALIGN_RIGHT,  ROW_FIT,k);
  }
}

void CGridLocusPeaks::_SetNumberCols(int n)
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
void CGridLocusPeaks::_CleanupPeaks()
{
  if(m_pvpPeak != NULL)
  {
    vectorptr<COARpeakAny>::cleanup(m_pvpPeak);
    delete m_pvpPeak;
    m_pvpPeak = NULL;
  }
  vectorptr<COARpeakAny>::cleanup(&m_vpPeakCopy);
}
void CGridLocusPeaks::_SetupPeaks()
{
  if(m_pvpPeak == NULL)
  {
    m_pvpPeak = m_pSample->GetPeaksByLocusName(
      m_sLocusName,m_nChannel,m_dtHistory,false);
    size_t nSize = m_pvpPeak->size();
    if(nSize)
    {
      m_vpPeakCopy.reserve(nSize);
      vectorptr<COARpeakAny>::iterator itr;
      for(itr = m_pvpPeak->begin();
        itr != m_pvpPeak->end();
        ++itr)
      {
        m_vpPeakCopy.push_back(new COARpeakAny(*(*itr)));
      }
    }
  }
}

bool CGridLocusPeaks::TransferDataToWindow()
{
  nwxGridBatch xxxx(this);
  wxString sBPS;
  const COARpeak *pPeak;
  bool bReadOnly = IsTableReadOnly();
  _SetupPeaks();
  int nCol = GetPeakCount();
  if(nCol)
  {
    _SetNumberCols(nCol);
    for(int k = 0; k < nCol; k++)
    {
      pPeak = m_pvpPeak->at(k);
      if(bReadOnly)
      {
        sBPS = pPeak->FormatBPSint();
      }
      else
      {
        sBPS = pPeak->FormatBPS();
      }
      SetCellValue(ROW_ALLELE,k, pPeak->FormatAlleleName(m_bAmel,false));
      SetCellValue(ROW_BPS, k, sBPS);
      SetCellValue(ROW_OFF_LADDER, k,pPeak->FormatOffLadderString());
      SetCellValue(ROW_HOMOZYGOUS, k,pPeak->FormatCount());
      SetCellValue(ROW_RFU, k, pPeak->FormatRFU());
      SetCellValue(ROW_TIME, k, pPeak->FormatTime());
      SetCellValue(ROW_PEAK_AREA, k, pPeak->FormatPeakArea());
      SetCellValue(ROW_FIT, k, pPeak->FormatFit());
      SetCellValue(ROW_IS_ALLELE, k, pPeak->FormatIsAllele());
      SetCellValue(ROW_IS_ARTIFACT, k, pPeak->FormatIsArtifact());
      SetCellValue(ROW_IS_CRITICAL, k, pPeak->FormatIsCritical());
      UpdateDisabledAlleles(k);
    }
    AutoSizeColumns();
  }
  else
  {
    nwxGrid::SetMessageGrid(this,"There are no peaks for this locus");
  }
  return true;
}

bool CGridLocusPeaks::TransferCellFromWindow(int nRow, int nCol)
{
//  COARpeak *pPeak = GetPeak(nCol);
  bool bRtn = true;
  switch (nRow)
  {
  case ROW_ALLELE:
    bRtn = XferName(nCol);
    break;
  case ROW_BPS:
    XferBPS(nCol);
    break;
  case ROW_OFF_LADDER:
    XferOL(nCol);
    break;
  case ROW_HOMOZYGOUS:
    XferHomozygous(nCol);
    break;
  case ROW_IS_ARTIFACT:
    XferIsArtifact(nCol);
    UpdateDisabledAlleles(nCol);
    break;
  case ROW_IS_CRITICAL:
    XferIsCritical(nCol);
    break;
  case ROW_IS_ALLELE:
    XferIsAllele(nCol);
    UpdateDisabledAlleles(nCol);
    break;
  default:
    break;
  }
  return bRtn;
}

void CGridLocusPeaks::XferIsAllele(int nCol)
{
  COARpeak *pPeak = GetPeak(nCol);
  pPeak->SetIsAllele(GetBoolValue(ROW_IS_ALLELE,nCol));
}
void CGridLocusPeaks::XferIsArtifact(int nCol)
{
  COARpeak *pPeak = GetPeak(nCol);
  bool bIsArtifact = GetBoolValue(ROW_IS_ARTIFACT,nCol);
  pPeak->SetIsArtifact(bIsArtifact);
  if(bIsArtifact)
  {
    XferIsCritical(nCol);
  }
}
void CGridLocusPeaks::XferIsCritical(int nCol)
{
  COARpeak *pPeak = GetPeak(nCol);
  pPeak->SetIsCritical(GetBoolValue(ROW_IS_CRITICAL,nCol));
}

bool CGridLocusPeaks::XferName(int nCol)
{
  bool bRtn = true;
  if(!IsColumnDisabled(nCol))
  {
    COARpeak *pPeak = GetPeak(nCol);
    wxString s;
    s = GetCellValue(ROW_ALLELE,nCol);
    nwxString::Trim(&s);
    if(m_bAmel)
    {
      s.MakeUpper();
      const char *ps = s.utf8_str();
      if( (!*ps) || (strchr("XY12",*ps) == NULL) )
      {
        // not x/y
        wxString sName = pPeak->FormatAlleleName(true,false);
        bRtn = false;
        mainApp::ShowError("Invalid allele name",this);
        SetCellValue(ROW_ALLELE,nCol,sName);
        SetGridCursor(ROW_ALLELE,nCol);
        SelectBlock(ROW_ALLELE,nCol,ROW_ALLELE,nCol);
      }
    }
    if(bRtn)
    {
      pPeak->SetAlleleName(s,m_bAmel);
    }
  }
  return bRtn;
}

void CGridLocusPeaks::XferBPS(int nCol)
{
  if(!IsColumnDisabled(nCol))
  {
    COARpeak *pPeak = GetPeak(nCol);
    wxString s = GetCellValue(ROW_BPS,nCol);
    nwxString::Trim(&s);
    pPeak->SetBPS(atof(s.utf8_str()));
  }
}
void CGridLocusPeaks::XferHomozygous(int nCol)
{
  if(!IsColumnDisabled(nCol))
  {
    wxString s = GetCellValue(ROW_HOMOZYGOUS,nCol);
    COARpeak *pPeak = GetPeak(nCol);
    bool b = IsTrue(s.utf8_str());
    pPeak->SetCountBool(b);
  }
}


void CGridLocusPeaks::XferOL(int nCol)
{
  if(!IsColumnDisabled(nCol))
  {
    wxString s = GetCellValue(ROW_OFF_LADDER,nCol);
    const wxString sValue = COARpeak::UnFormatOffLadderString(s);
    COARpeak *pPeak = GetPeak(nCol);
    pPeak->SetOffLadderString(sValue);
  }
}


bool CGridLocusPeaks::TransferColumnFromWindow(int nCol)
{
  bool b = XferName(nCol);
  if(b)
  {
    XferBPS(nCol);
    XferOL(nCol);
    XferIsAllele(nCol);
    XferIsArtifact(nCol);
    XferHomozygous(nCol);
  }
  return b;
}


bool CGridLocusPeaks::TransferDataFromWindow()
{
  bool bRtn = true;
  if(!IsTableReadOnly())
  {
    int nCol = GetPeakCount();
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
bool CGridLocusPeaks::TransferDataToSample(COARsample *pSample)
{
  bool bRtn = (!IsTableReadOnly()) &&
    TransferDataFromWindow();
  if(bRtn)
  {
    pSample->SetPeaksByLocusName(
        m_sLocusName,
        m_nChannel,
        m_pvpPeak);
  }
  return bRtn;
}

bool CGridLocusPeaks::IsModified() const
{
  bool bRtn = false;
  if(m_pvpPeak != NULL)
  {
    vectorptr<COARpeakAny>::const_iterator itr1;
    vectorptr<COARpeakAny>::const_iterator itr2;
    for(itr1 = m_pvpPeak->begin(), itr2 = m_vpPeakCopy.begin();
      (!bRtn) && (itr2 != m_vpPeakCopy.end());
      ++itr1, ++itr2)
    {
      if(!COARpeak::Equal(*(*itr1),*(*itr2)))
      {
        bRtn = true;
      }
    }
  }
  return bRtn;
}

void CGridLocusPeaks::UpdateDisabledAlleles(int nCol)
{
  nwxGridBatch xxxx(this);
  bool bAllele = GetBoolValue(ROW_IS_ALLELE,nCol);
  bool bArtifact = GetBoolValue(ROW_IS_ARTIFACT,nCol);

  DisableEdit(ROW_ALLELE,nCol,!bAllele);
  DisableEdit(ROW_BPS,nCol,!bAllele);
  DisableEdit(ROW_OFF_LADDER,nCol,!bAllele);
  DisableEdit(ROW_HOMOZYGOUS,nCol,!bAllele);
  DisableEdit(ROW_IS_CRITICAL,nCol,!bArtifact);
  Refresh();
}


CGridLocusPeaks::~CGridLocusPeaks()
{
  _CleanupPeaks();
}

void CGridLocusPeaks::OnCellChange(wxGridEvent &e)
{
  if(!( IsTableReadOnly() || m_nOnCellChange ))
  {
    CIncrementer x(m_nOnCellChange);
    int nRow = e.GetRow();
    int nCol = e.GetCol();
    if(!TransferCellFromWindow(nRow,nCol))
    {
      e.Veto();
    }
    else
    {
      e.Skip(true);
    }
  }
}


BEGIN_EVENT_TABLE(CGridLocusPeaks,_CGridEdit)
EVT_GRID_CELL_CHANGE(CGridLocusPeaks::OnCellChange)
END_EVENT_TABLE()
