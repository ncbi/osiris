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
*  FileName: CPanelLabReviewAcceptance.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CPanelLabReviewAcceptance.h"
#include <wx/stattext.h>
#include <wx/sizer.h>

//********************************************************************
//
//    CPanelLabReviewAcceptance
//

#define MAX_USER_COUNT 10
const int CPanelLabReviewAcceptance::ROW_REVIEW = 1;
const int CPanelLabReviewAcceptance::ROW_ACCEPT = 0;

CPanelLabReviewAcceptance::CPanelLabReviewAcceptance(
  wxWindow *parent) : 
    nwxPanel(parent),
    m_pReview(NULL)
{
  int i;
  wxStaticText *pTextTitle = 
    new wxStaticText(this,wxID_ANY,"Select the minimum number of users");
  m_pGrid = new wxGrid(this,wxID_ANY);
  m_pGrid->CreateGrid(2,CLabReview::__REVIEW_COUNT);
  m_pGrid->SetDefaultEditor(new wxGridCellFloatEditor(3,0));
  m_pGrid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
  m_pGrid->SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);

  for(i = 0; i < CLabReview::__REVIEW_COUNT; i++)
  {
    m_pGrid->SetColLabelValue(i,CLabReview::g_LABELS[i]);
    m_pGrid->SetCellValue(0,i,"000000");
  }
  m_pGrid->SetRowLabelValue(ROW_ACCEPT,"Accept alerts");
  m_pGrid->SetRowLabelValue(ROW_REVIEW,"Review editing");
  nwxGrid::UpdateLabelSizes(m_pGrid);
  m_pGrid->AutoSize();
  for(i = 0; i < CLabReview::__REVIEW_COUNT; i++)
  {
    m_pGrid->SetCellValue(0,i,wxEmptyString);
  }
  m_pGrid->EnableDragGridSize(false);

  wxFont fn = pTextTitle->GetFont();
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  fn.SetPointSize(fn.GetPointSize() + 1);
  pTextTitle->SetFont(fn);

  m_pCheckAllowUserOverride = new wxCheckBox(
    this,wxID_ANY,"Allow reviewer to modify user name");
  m_pCheckExportIfNeedsAttn = new wxCheckBox(
    this,wxID_ANY,"Allow automated export when analysis needs attention");

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerCheckbox = new wxBoxSizer(wxVERTICAL);

  pSizer->Add(pTextTitle,0,
    wxALL | wxALIGN_CENTER_HORIZONTAL,
    ID_BORDER);
  pSizer->Add(m_pGrid,0,
    wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_CENTER_HORIZONTAL,
    ID_BORDER);
  pSizerCheckbox->Add(m_pCheckAllowUserOverride,0,
    wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_LEFT,
    ID_BORDER);
  pSizerCheckbox->Add(m_pCheckExportIfNeedsAttn,0,
    wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_LEFT,
    ID_BORDER);
  pSizer->Add(pSizerCheckbox,0,wxALIGN_CENTRE_HORIZONTAL,0);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
}

CPanelLabReviewAcceptance::~CPanelLabReviewAcceptance() {}

void CPanelLabReviewAcceptance::_SetCellValue(int nRow, int nCol, int nValue)
{
  wxString s;
  if(nValue >= 0)
  {
    if(nValue > MAX_USER_COUNT)
    {
      nValue = MAX_USER_COUNT;
    }
    m_pGrid->SetCellValue(nRow,nCol,nwxString::FormatNumber(nValue));
  }
  else
  {
    const wxChar *p = CLabReview::IsDefault(nCol) ? wxS("0") : wxS("");
    m_pGrid->SetCellValue(nRow,nCol,p);
  }
}
int CPanelLabReviewAcceptance::_GetCellValue(int nRow, int nCol)
{
  int nRtn = CLabReview::IsDefault(nCol) ? 0 : -1;
    // default value of default column is 0
  wxString s = m_pGrid->GetCellValue(nRow,nCol);
  nwxString::Trim(&s);
  if(!s.IsEmpty())
  {
    nRtn = atoi(s.utf8_str());
    if(nRtn < -1) { nRtn = -1;}
    else if(nRtn > MAX_USER_COUNT) { nRtn = MAX_USER_COUNT;}
  }
  return nRtn;
}

bool CPanelLabReviewAcceptance::TransferDataToWindow()
{
  int nReview;
  int nAccept;
  int i;
  for(i = 0; i < CLabReview::__REVIEW_COUNT; i++)
  {
    nReview = m_pReview->GetReviewerCount(i,false);
    nAccept = m_pReview->GetAcceptanceCount(i,false);
    _SetCellValue(ROW_ACCEPT,i,nAccept);
    _SetCellValue(ROW_REVIEW,i,nReview);
  }
  m_pCheckAllowUserOverride->SetValue(
    m_pReview->GetAllowUserNameOverride());
  m_pCheckExportIfNeedsAttn->SetValue(
    m_pReview->GetAllowExportWithAttnReqd());
  return true;
}

bool CPanelLabReviewAcceptance::TransferDataFromWindow()
{
  int nReview;
  int nAccept;
  int i;
  for(i = 0; i < CLabReview::__REVIEW_COUNT; i++)
  {
    nAccept = _GetCellValue(ROW_ACCEPT,i);
    nReview = _GetCellValue(ROW_REVIEW,i);
    m_pReview->SetReviewerCount(nReview,i);
    m_pReview->SetAcceptanceCount(nAccept,i);
  }
  m_pReview->SetAllowUserNameOverride(
    m_pCheckAllowUserOverride->GetValue());
  m_pReview->SetAllowExportWithAttnReqd(
    m_pCheckExportIfNeedsAttn->GetValue());
  return true;
}
void CPanelLabReviewAcceptance::SetReadOnly(bool b)
{
  m_pGrid->EnableEditing(!b);
  m_pCheckAllowUserOverride->Enable(!b);
  m_pCheckExportIfNeedsAttn->Enable(!b);
}
