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
*  FileName: CPanelLabSampleThresholds.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CPanelLabSettings.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "nwx/nwxBatch.h"

//********************************************************************
//
//    CPanelLabSampleThresholds - CSizerSearchBox, CGridSampleLimits
//

//***************************                     CSizerSearchBox
class CSizerSearchBox : public wxBoxSizer
{
public:
  CSizerSearchBox(wxWindow *parent, wxWindowID id = wxID_ANY);
  virtual ~CSizerSearchBox() {}
  size_t FindMatches(const wxArrayString &aStr, wxArrayInt *pnMatches);
  int FindNextMatch(int nCurrent, const wxArrayInt &nMatches, bool bForward = true, bool bWrap = true);
private:
  wxWindow *m_pParent;
  wxStaticText *m_pLabelSearch;
  wxStaticText *m_pLabelCount;
  wxTextCtrl *m_pTextSearch;
  wxButton *m_pButtonPrev;
  wxButton *m_pButtonNext;
  wxWindowID m_nIDSearch;
  wxWindowID m_nIDPrev;
  wxWindowID m_nIDNext;
  static const wxString FORMAT;
};

const wxString CSizerSearchBox::FORMAT(wxT("%d match%s"));

CSizerSearchBox::CSizerSearchBox(wxWindow *parent, wxWindowID id) :
  wxBoxSizer(wxHORIZONTAL),
  m_pParent(parent),
  m_nIDSearch(id),
  m_nIDPrev(id),
  m_nIDNext(id)
{
  // arrows for buttons
  // http://xahlee.info/comp/unicode_geometric_shapes.html
  // in above look for triangles
  wxChar sLeft[] = { ID_CHAR_TRIANGLE_LEFT, 0 };
  wxChar sRight[] = { ID_CHAR_TRIANGLE_RIGHT, 0 };
  if (id != wxID_ANY)
  {
    m_nIDPrev++;
    m_nIDNext += 2;
  }
  m_pLabelSearch = new wxStaticText(parent, wxID_ANY, wxString("Search"));
  m_pTextSearch = new wxTextCtrl(
    parent, m_nIDSearch, wxEmptyString, wxDefaultPosition,
    wxSize(parent->GetTextExtent("WWWWWWWWWWWWWWWWWWWW").GetWidth(), -1));
  m_pButtonPrev = new wxButton(parent, m_nIDPrev, sLeft,
    wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
  m_pButtonNext = new wxButton(parent, m_nIDNext, sRight,
    wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
  mainApp::SetBoldFont(m_pButtonPrev);
  mainApp::SetBoldFont(m_pButtonNext);
  m_pLabelCount = new wxStaticText(parent, wxID_ANY, wxEmptyString,
    wxDefaultPosition, 
    wxSize(parent->GetTextExtent("WWWW matches WW").GetWidth(), -1),
    wxST_NO_AUTORESIZE);
  this->Add(m_pLabelSearch, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
  this->Add(m_pTextSearch, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
  this->Add(m_pButtonPrev, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
  this->Add(m_pButtonNext, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
  this->Add(m_pLabelCount, 0, wxALIGN_CENTER_VERTICAL, 0);
}

size_t CSizerSearchBox::FindMatches(const wxArrayString &aStr, wxArrayInt *pnMatches)
{
  wxString s(m_pTextSearch->GetValue());
  wxString sItem;
  size_t nCount = aStr.GetCount();
  size_t nRtn = 0;
  nwxString::TrimLower(&s);
  pnMatches->Empty();
  if (s.Len())
  {
    pnMatches->Alloc(nCount);
    for (size_t i = 0; i < nCount; ++i)
    {
      sItem = aStr.Item(i);
      nwxString::TrimLower(&sItem);
      if (sItem.Find(s) != wxNOT_FOUND)
      {
        pnMatches->Add((int)i);
      }
    }
    nRtn = pnMatches->GetCount();
    m_pLabelCount->SetLabelText(
      wxString::Format(FORMAT, (int)nRtn, nRtn == 1 ? "" : "es"));
  }
  else
  {
    m_pLabelCount->SetLabelText(wxEmptyString);
  }
  return pnMatches->GetCount();
}

int CSizerSearchBox::FindNextMatch(
  int nCurrent, 
  const wxArrayInt &nMatches,
  bool bForward,
  bool bWrap)
{
  int nSize = int(nMatches.GetCount());
  int nRtn = -1;
  if (nSize)
  {
    // find current match in list or closest
    int nTop = 0;
    int nBottom = nSize - 1;
    int nMid = 0;
    int nItem = 0;
    while (nBottom >= nTop)
    {
      nMid = (nTop + nBottom) >> 1;
      nItem = nMatches.Item(nMid);
      if (nItem < nCurrent)
      {
        nTop = nMid + 1;
      }
      else if (nItem > nCurrent)
      {
        nBottom = nMid - 1;
      }
      else
      {
        // found - loop exit
        nTop = nMid + 1; // loop exit
        nBottom = nMid;
      }
    }
    if (bForward && (nItem <= nCurrent))
    {
      nMid++;
    }
    else if ((!bForward) && (nItem >= nCurrent))
    {
      nMid--;
    }
    if (!bWrap)
    {
      if (nMid >= nSize)
      {
        nMid = -1;
      }
    }
    else if (nMid < 0)
    {
      nMid = nSize - 1;
    }
    else if (nMid >= nSize)
    {
      nMid = 0;
    }
    if (nMid >= 0)
    {
      nRtn = nMatches.Item(nMid);
    }
  }
  return nRtn;
}

//***************************                     CGridSampleLimits

CGridSampleLimits::CGridSampleLimits(wxWindow *parent, wxWindowID id) :
  _CGridEdit(parent,id), 
  m_pValidatePct(NULL), 
  m_pData(NULL),
  m_pMessageBook(NULL)
  //,m_bReadOnly(false)
{
  wxString PEAKS("peaks");
  CreateGrid(ROW_MESSAGE_BOOK_OFFSET,COL_COUNT);
  EnableDragColSize(false);
  EnableDragRowSize(false);

  EnableScrolling(false,true);

  _SetRowLabelValue(ROW_PULLUP,"Max. No. of pullups peaks per sample");
  SetCellValue(ROW_PULLUP, COL_UNITS, PEAKS);

  _SetRowLabelValue(ROW_STUTTER,"Max. No. of stutter peaks per sample");
  SetCellValue(ROW_STUTTER, COL_UNITS, PEAKS);

  _SetRowLabelValue(ROW_ADENYLATION,"Max. No. of adenylation peaks per sample");
  SetCellValue(ROW_ADENYLATION, COL_UNITS, PEAKS);

  _SetRowLabelValue(ROW_OFF_LADDER,"Max. off-ladder alleles per sample:");
  SetCellValue(ROW_OFF_LADDER, COL_UNITS, PEAKS);
  
  _SetRowLabelValue(ROW_RESIDUAL,"Max. residual for allele (<0.5 bp):");
  SetCellValue(ROW_RESIDUAL, COL_UNITS, "Sample/Ladder BP alignment");
  SetCellEditor(ROW_RESIDUAL, COL_VALUE, new wxGridCellFloatEditor(-1,4));
  SetDefaultCellValidator(new nwxGridCellUIntRangeValidator(0,1000000,true));
  SetCellValidator(
    new nwxGridCellDoubleRangeValidator(0.0,0.5,true),
    ROW_RESIDUAL, COL_VALUE);
  
  _SetRowLabelValue(ROW_EXCESSIVE_RESIDUAL,"Max. No. of peaks with excessive residual:");
  SetCellValue(ROW_EXCESSIVE_RESIDUAL, COL_UNITS, PEAKS);

  _SetRowLabelValue(ROW_RFU_INCOMPLETE_SAMPLE,"Incomplete profile threshold for Reamp More/Reamp Less:");
  SetCellValue(ROW_RFU_INCOMPLETE_SAMPLE, COL_UNITS, "RFU");

  _SetRowLabelValue(ROW_MIN_BPS_ARTIFACTS,"Ignore artifacts smaller than:");
  SetCellValue(ROW_MIN_BPS_ARTIFACTS, COL_UNITS, "bps");
  m_clrBackground = GetLabelBackgroundColour();
  m_clrForeground = GetLabelTextColour();
  for(int nRow = 0; nRow < ROW_MESSAGE_BOOK_OFFSET; nRow++)
  {
    SetCellAlignment(nRow, COL_VALUE, wxALIGN_RIGHT, wxALIGN_CENTRE);
  }

  SetDefaultEditor(new wxGridCellFloatEditor(1,0));
  SetColLabelValue(COL_ROW_LABEL, wxEmptyString);
  SetColLabelValue(COL_VALUE,"Value");
  SetColLabelValue(COL_UNITS,"Units");
  _SetupLabelColumns();
}
void CGridSampleLimits::_SetupLabelColumns()
{
  wxFont fnLabel = GetLabelFont();
  nwxGrid::SetColReadOnly(this, COL_UNITS, true);
  nwxGrid::SetColReadOnly(this, COL_ROW_LABEL, true);
  nwxGrid::SetColBackgroundColour(this, COL_UNITS, m_clrBackground);
  nwxGrid::SetColBackgroundColour(this, COL_ROW_LABEL, m_clrBackground);
  nwxGrid::SetColFont(this, COL_UNITS, fnLabel);
  nwxGrid::SetColFont(this, COL_ROW_LABEL, fnLabel);
  nwxGrid::SetColAlignment(this, COL_ROW_LABEL, wxALIGN_LEFT, wxALIGN_CENTRE);
  nwxGrid::ClearAllRowLabels(this);
  _ClearRowLabelList();
}
void CGridSampleLimits::SetData(
  CLabThresholds *pData, const CXMLmessageBook *pMessageBook)
{
  m_pData = pData;
  m_pMessageBook = pMessageBook;
  set<wxString> setMsgThresholds; // message names for thresholds already used
  size_t nCount = 
    ((m_pMessageBook != NULL) && m_pMessageBook->IsOK())
    ? m_pMessageBook->GetMessageCount()
    : 0;
  if(nCount)
  {
    const wxString sEmpty;
    const wxString sUnitPCT("%");
    const CXMLmessageBookSM *pmsgSM;
    size_t nCountKeep = 0;
    bool bPct;

    // the following 5 will not use the message book
    setMsgThresholds.insert(CLabThresholds::g_psmNumberOfPullUpsInSample);
    setMsgThresholds.insert(CLabThresholds::g_psmNumberOfStuttersInSample);
    setMsgThresholds.insert(CLabThresholds::g_psmNumberOfAdenylationsInSample);
    setMsgThresholds.insert(CLabThresholds::g_psmNumberOfOffLadderAllelesInSample);
    setMsgThresholds.insert(CLabThresholds::g_psmNumberOfExcessiveResidualsInSample);

    m_vpMsgBookSM.clear();
    m_vpMsgBookSM.reserve(nCount);
    for(size_t i = 0; i < nCount; i++)
    {
      pmsgSM = m_pMessageBook->GetMessage(i);
      if(setMsgThresholds.find(pmsgSM->GetName()) == setMsgThresholds.end())
      {
        m_vpMsgBookSM.push_back(pmsgSM);
        setMsgThresholds.insert(pmsgSM->GetName());
        if(!pmsgSM->EmptyDescriptor())
        {
          nCountKeep++;
        }
      }
    }
    SetMessageBookRows((int)nCountKeep);
    if(nCountKeep)
    {
      wxFont fn = GetLabelFont();
      vector<const CXMLmessageBookSM *>::iterator itrSM;
      int nRow = ROW_MESSAGE_BOOK_OFFSET;
      for(itrSM = m_vpMsgBookSM.begin();
        itrSM != m_vpMsgBookSM.end();
        ++itrSM)
      {
        pmsgSM = *itrSM;
        if(!pmsgSM->EmptyDescriptor())
        {
          bPct = pmsgSM->IsPercent();
          if(bPct)
          {
            SetCellValidator(GetPctValidator(),nRow, COL_VALUE);
          }
          _SetRowLabelValue(nRow,pmsgSM->GetDescriptor());
          SetCellValue(nRow, COL_UNITS, bPct ? sUnitPCT : sEmpty);
          if(pmsgSM->IsPreset())
          {
            SetCellAlignment(nRow, COL_VALUE, wxALIGN_RIGHT,wxALIGN_CENTRE);
          }
          nRow++;
        }
      }
    }
  }
  _SetupLabelColumns();
  SetCellValue(0, COL_VALUE, "00000000");
  SetMargins(0,0);
  nwxGrid::UpdateLabelSizes(this);
  AutoSize();
}
const wxArrayString &CGridSampleLimits::GetRowLabels()
{
  int nRows = GetNumberRows();
  if (nRows != (int)m_asRowLabels.GetCount())
  {
    nwxGrid::GetColumnValues(this, COL_ROW_LABEL, &m_asRowLabels);
  }
  return m_asRowLabels;
}
void CGridSampleLimits::SetMessageBookRows(int n)
{
//  int nOld = GetNumberRows();
  int nNew = n + ROW_MESSAGE_BOOK_OFFSET;
  nwxGrid::SetRowCount(this,ROW_MESSAGE_BOOK_OFFSET);// bug workaround
  nwxGrid::SetRowCount(this,nNew);
  _SetupLabelColumns();
}
bool CGridSampleLimits::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL) && (m_pMessageBook != NULL);
  if(bRtn)
  {
    wxString s;
    double d;
    int n;
    bool bCreateIfNeeded;
    s = GetCellValue(ROW_PULLUP, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxNumberOfPullupsPerSample(n);

    s = GetCellValue(ROW_STUTTER, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxNumberOfStutterPeaksPerSample(n);

    s = GetCellValue(ROW_ADENYLATION, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxNumberOfAdenylationsPerSample(n);

    s = GetCellValue(ROW_OFF_LADDER, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxNumberOfOLAllelesPerSample(n);

    s = GetCellValue(ROW_RESIDUAL, COL_VALUE);
    d = CPanelLabSettings::Str2Double(s);
    m_pData->SetMaxResidualForAlleleCall(d);

    s = GetCellValue(ROW_EXCESSIVE_RESIDUAL, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxExcessiveResidual(n);

    s = GetCellValue(ROW_RFU_INCOMPLETE_SAMPLE, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMaxRFUForIncompleteSample(n);

    s = GetCellValue(ROW_MIN_BPS_ARTIFACTS, COL_VALUE);
    n = CPanelLabSettings::Str2Int(s);
    m_pData->SetMinBPSForArtifacts(n);


    // message book parameters

    int nRow = ROW_MESSAGE_BOOK_OFFSET;
    vector<const CXMLmessageBookSM *>::iterator itrMsg;
    for(itrMsg = m_vpMsgBookSM.begin();
      itrMsg != m_vpMsgBookSM.end();
      ++itrMsg)
    {
      if(!(*itrMsg)->EmptyDescriptor())
      {
        if((*itrMsg)->IsPreset())
        {
          n = GetBoolValue(nRow, COL_VALUE) ? 1 : 0;
        }
        else
        {
          s = GetCellValue(nRow, COL_VALUE);
          n = CPanelLabSettings::Str2Int(s,-1);
        }
        bCreateIfNeeded = (n != (*itrMsg)->GetThreshold());
        m_pData->SetValue((*itrMsg)->GetName(),n,bCreateIfNeeded);
        ++nRow;
      }

      // need validation
    }
  }
  return bRtn;
}
void CGridSampleLimits::GoToRow(int nRow)
{
  if (nRow >= 0 && nRow < GetNumberRows())
  {
    GoToCell(nRow, COL_VALUE);
    SetFocus();
  }
}

void CGridSampleLimits::HighlightRows(const wxArrayInt &anRows)
{
  size_t ndx = 0;
  int nNextHighlight = (ndx < anRows.GetCount() ? anRows.Item(ndx) : -1);
  int nRowCount = GetNumberRows();
  TnwxBatch<wxGrid> Batch(this);
  for (int nRow = 0; nRow < nRowCount; ++nRow)
  {
    if (nRow == nNextHighlight)
    {
      SetCellTextColour(nRow, COL_ROW_LABEL, m_clrBackground);
      SetCellBackgroundColour(nRow, COL_ROW_LABEL, m_clrForeground);
      ndx++;
      nNextHighlight = (ndx < anRows.GetCount() ? anRows.Item(ndx) : -1);
    }
    else
    {
      SetCellTextColour(nRow, COL_ROW_LABEL, m_clrForeground);
      SetCellBackgroundColour(nRow, COL_ROW_LABEL, m_clrBackground);
    }
  }
}
bool CGridSampleLimits::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL) && (m_pMessageBook != NULL);
  if(bRtn)
  {
    SetCellValue(ROW_PULLUP, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxNumberOfPullupsPerSample()));
    SetCellValue(ROW_STUTTER, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxNumberOfStutterPeaksPerSample()));
    SetCellValue(ROW_ADENYLATION, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxNumberOfAdenylationsPerSample()));
    SetCellValue(ROW_OFF_LADDER, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxNumberOfOLAllelesPerSample()));
    SetCellValue(ROW_RESIDUAL, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxResidualForAlleleCall()));
    SetCellValue(ROW_EXCESSIVE_RESIDUAL, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxExcessiveResidual()));

    SetCellValue(ROW_RFU_INCOMPLETE_SAMPLE, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMaxRFUForIncompleteSample()));
    SetCellValue(ROW_MIN_BPS_ARTIFACTS, COL_VALUE, CPanelLabSettings::Number2Str(
      m_pData->GetMinBPSForArtifacts()));

    //  message book stuff
    int nRow = ROW_MESSAGE_BOOK_OFFSET;
    int nValue;
    vector<const CXMLmessageBookSM *>::iterator itrMsg = m_vpMsgBookSM.begin();
    for(itrMsg = m_vpMsgBookSM.begin();
      itrMsg != m_vpMsgBookSM.end();
      ++itrMsg)
    {
      if(!(*itrMsg)->EmptyDescriptor())
      {
        nValue = m_pData->GetValue((*itrMsg)->GetName(),(*itrMsg)->GetThreshold());
        if((*itrMsg)->IsPreset())
        {
          SetBoolCell(nRow, COL_VALUE);
          SetBoolValue(nRow, COL_VALUE, !!nValue);
        }
        else
        {
          SetCellValue(nRow, COL_VALUE, 
            CPanelLabSettings::Number2Str(nValue));
        }
        ++nRow;
      }
    }
  }
  return bRtn;
}

//********************************************************************
//
//    CPanelLabSampleThresholds
//
CPanelLabSampleThresholds::CPanelLabSampleThresholds(
  wxWindow *parent, wxWindowID id) :
    nwxPanel(parent,id),
    m_pData(NULL)
{
  m_pSizerSearch = new CSizerSearchBox(this, IDsearchBox);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  //wxBoxSizer *pSearchBox = _CreateSearchBox();
  m_pGridSample = new CGridSampleLimits(this,wxID_ANY);
  pSizer->Add(m_pSizerSearch, 0, wxEXPAND | (wxALL ^ wxBOTTOM), ID_BORDER);
  pSizer->Add(m_pGridSample,1,wxEXPAND | wxALL,ID_BORDER);
  SetSizer(pSizer);
}

bool CPanelLabSampleThresholds::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL);
  if(!m_pGridSample->TransferDataFromWindow())
  {
    bRtn = false;
  }
  return bRtn;
}
bool CPanelLabSampleThresholds::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(!m_pGridSample->TransferDataToWindow())
  {
    bRtn = false;
  }
  wxCommandEvent e;
  _OnSearchText(e);
  return bRtn;
}

void CPanelLabSampleThresholds::_OnSearchText(wxCommandEvent &)
{
  m_pSizerSearch->FindMatches(m_pGridSample->GetRowLabels(), &m_anSearchMatches);
  m_pGridSample->HighlightRows(m_anSearchMatches);
}
void CPanelLabSampleThresholds::_OnSearchButton(wxCommandEvent &e)
{
  int nRow = m_pSizerSearch->FindNextMatch(m_pGridSample->GetCursorRow(), m_anSearchMatches,
    e.GetId() == IDsearchBoxNext);
  m_pGridSample->GoToRow(nRow);
}

BEGIN_EVENT_TABLE(CPanelLabSampleThresholds, wxPanel)
EVT_BUTTON(IDsearchBoxNext, CPanelLabSampleThresholds::_OnSearchButton)
EVT_BUTTON(IDsearchBoxPrev, CPanelLabSampleThresholds::_OnSearchButton)
EVT_TEXT(IDsearchBox, CPanelLabSampleThresholds::_OnSearchText)
END_EVENT_TABLE()


