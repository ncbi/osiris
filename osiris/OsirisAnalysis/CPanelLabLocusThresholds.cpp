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
*  FileName: CPanelLabLocusThresholds.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CPanelLabLocusThresholds.h"
#include <wx/stattext.h>
#include <wx/sizer.h>


//********************************************************************
//
//    CChoiceHomozygote
//
CChoiceHomozygote::CChoiceHomozygote(wxWindow *parent, wxWindowID id) :
  wxChoice(parent,id)
{
  Append("RFU");
  Append("% of Min. RFU");
}
CChoiceHomozygote::~CChoiceHomozygote() {}
void CChoiceHomozygote::SetSelectionByValue(wxChar x)
{
  int n = (x == VALUE_PCT) ? 1 : 0;
  Select(n);
}
wxChar CChoiceHomozygote::GetSelectionByValue()
{
  int n = GetSelection();
  wxChar c = (n == 1) ? VALUE_PCT : VALUE_RFU;
  return c;
}

const wxChar CChoiceHomozygote::VALUE_PCT = '%';
const wxChar CChoiceHomozygote::VALUE_RFU = 'R';



//********************************************************************
//
//    CPanelLabLocusThresholds
//

CPanelLabLocusThresholds::CPanelLabLocusThresholds(
  wxWindow *parent, wxWindowID id) : 
    SUPER_CPanelLabLocusThresholds(parent,id,wxDefaultPosition,wxDefaultSize,wxVSCROLL),
    m_pWinFocusOnSelect(NULL),
    m_pData(NULL)
{
  wxArrayString aChoices;
  aChoices.Alloc(2);
  aChoices.Add("RFU");
  aChoices.Add("% of Min. RFU");
  m_pPanel = new wxPanel(this);
  wxWindow *PANEL = m_pPanel;

  m_pGridRFU = new CGridRFULimits(PANEL,wxID_ANY);
  m_pAllowOverride = new wxCheckBox(PANEL,wxID_ANY,"Allow User to Override Min. RFU");

  wxStaticText *pTextRFU = new wxStaticText(
    PANEL,wxID_ANY,"RFU Limits");
  wxStaticText *psSampleLabel = 
    new wxStaticText(PANEL,wxID_ANY,
    "Locus Limits for Samples");
  wxStaticText *psLadderLabel =
    new wxStaticText(PANEL,wxID_ANY,
    "Locus Limits for Ladders/ILS Limits");
  wxStaticText *psStutterLabel =
    new wxStaticText(PANEL,wxID_ANY,
    "Non-Standard Stutter");
  mainApp::SetBoldFont(pTextRFU);
  mainApp::SetBoldFont(psSampleLabel);
  mainApp::SetBoldFont(psLadderLabel);
  mainApp::SetBoldFont(psStutterLabel);
  wxStaticText *psDisclaimer =
    new wxStaticText(PANEL,wxID_ANY,
    "* Note: The default settings for "
      "ladder do not affect the ILS.");
  wxStaticText *psChoice = 
    new wxStaticText(PANEL,wxID_ANY,
    "Min. homozygote threshold units:");
  m_pGridSample = new CGridLabThresholdsSample(PANEL,wxID_ANY);
  m_pGridLadder = new CGridLabThresholdsLadder(PANEL,wxID_ANY);
  m_pGridStutter = new CGridLabNsStutter(PANEL,wxID_ANY);
  m_pChoiceHomozygoteUnits = new CChoiceHomozygote(PANEL,wxID_ANY);
  
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerChoice = new wxBoxSizer(wxHORIZONTAL);
  pSizerChoice->Add(psChoice,0,wxALIGN_CENTRE_VERTICAL);
  pSizerChoice->Add(m_pChoiceHomozygoteUnits,
    0,wxLEFT | wxALIGN_CENTRE_VERTICAL, ID_BORDER);
  pSizer->Add(pTextRFU,0,
    wxALIGN_LEFT | (wxALL ^ wxBOTTOM),ID_BORDER);
  pSizer->Add(m_pGridRFU,0,
    wxALIGN_LEFT | wxLEFT | wxRIGHT,ID_BORDER);
  pSizer->Add(m_pAllowOverride,0,
    wxALIGN_LEFT | wxALL, ID_BORDER);
  pSizer->Add(psSampleLabel,0, wxALIGN_LEFT | (wxALL ^ wxBOTTOM), ID_BORDER);
  pSizer->Add(m_pGridSample,0,  /* wxEXPAND | */ (wxALL ^ wxTOP), ID_BORDER);
  pSizer->Add(pSizerChoice,0,wxALIGN_LEFT | (wxALL ^ wxTOP), ID_BORDER);
  pSizer->AddSpacer(ID_BORDER << 2);
  pSizer->Add(psLadderLabel,0,wxALIGN_LEFT | wxLEFT | wxRIGHT,ID_BORDER);
  pSizer->Add(m_pGridLadder,0, /* wxEXPAND  | */ wxLEFT | wxRIGHT, ID_BORDER);
  pSizer->Add(psDisclaimer,0,wxALIGN_LEFT | (wxALL ^ wxTOP),ID_BORDER);

  pSizer->AddSpacer(ID_BORDER << 2);
  pSizer->Add(psStutterLabel,0,wxALIGN_LEFT | wxLEFT | wxRIGHT,ID_BORDER);
  pSizer->Add(m_pGridStutter,0, /* wxEXPAND  | */ wxLEFT | wxRIGHT, ID_BORDER);


  PANEL->SetSizer(pSizer);
  pSizer = new wxBoxSizer(wxHORIZONTAL);
  pSizer->Add(PANEL,0,wxALIGN_TOP | wxALIGN_LEFT,0);
  SetSizer(pSizer);
}

CPanelLabLocusThresholds::~CPanelLabLocusThresholds() {}

void CPanelLabLocusThresholds::SetReadOnly(bool bReadOnly)
{
  m_pChoiceHomozygoteUnits->Enable(!bReadOnly);
  m_pAllowOverride->Enable(!bReadOnly);
  m_pGridRFU->SetAllReadOnly(bReadOnly);
  m_pGridSample->SetAllReadOnly(bReadOnly);
  m_pGridLadder->SetAllReadOnly(bReadOnly);
  m_pGridStutter->SetAllReadOnly(bReadOnly);
}
bool CPanelLabLocusThresholds::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    m_pChoiceHomozygoteUnits->SetSelectionByValue(
      m_pData->GetMinBoundHomozygoteUnit());
    if(!m_pGridRFU->TransferDataToWindow())
    {
      bRtn = false;
    }
    m_pAllowOverride->SetValue(m_pData->GetAllowMinRFUoverride());

    if(!m_pGridSample->TransferDataToWindow())
    {
      bRtn = false;
    }
    if(!m_pGridLadder->TransferDataToWindow())
    {
      bRtn = false;
    }
    if(!m_pGridStutter->TransferDataToWindow())
    {
      bRtn = false;
    }
    Layout();
  }
  return bRtn;
}
bool CPanelLabLocusThresholds::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    m_pData->SetMinBoundHomozygoteUnit(
      m_pChoiceHomozygoteUnits->GetSelectionByValue());
    m_pData->SetAllowMinRFUoverride(m_pAllowOverride->GetValue());
    wxWindow *pList[] =
    {
      m_pGridRFU,
      m_pGridSample,
      m_pGridLadder,
      m_pGridStutter
    };
    m_pWinFocusOnSelect = NULL;
    const size_t NCOUNT = sizeof(pList) / sizeof(pList[0]);
    for(size_t i = 0; i < NCOUNT; ++i)
    {
      if(!pList[i]->TransferDataFromWindow())
      {
        bRtn = false;
        m_pWinFocusOnSelect = pList[i];
        i = NCOUNT; //break
      }
    }
#if 0
    if(!m_pGridRFU->TransferDataFromWindow())
    {
      bRtn = false;
    }
    else if(!m_pGridSample->TransferDataFromWindow())
    {
      bRtn = false;
    }
    else if(!m_pGridLadder->TransferDataFromWindow())
    {
      bRtn = false;
    }
    else if(!m_pGridStutter->TransferDataFromWindow())
    {
      bRtn = false;
    }
#endif
  }

  return bRtn;
}
void CPanelLabLocusThresholds::OnSize(wxSizeEvent &)
{
  _UpdateView();
}

  
void CPanelLabLocusThresholds::_UpdateView()
{
  int ncx,ncy,nvx,nvy,npx,npy;
  m_pPanel->GetSize(&npx,&npy);
  GetVirtualSize(&nvx,&nvy);
  GetClientSize(&ncx,&ncy);
  int nHeight = (npy > ncy) ? npy : ncy;
  m_pPanel->SetSize(ncx,npy);
  m_pPanel->SetMaxSize(wxSize(ncx,-1));
  SetVirtualSize(ncx,nHeight);
  if(npy > ncy)
  {
    SetScrollRate(0,8);
  }
  else
  {
    SetScrollbars(0,0,0,0,0,0,true);
  }
  Layout();
  Refresh();
  if(m_pWinFocusOnSelect != NULL)
  {
    m_pWinFocusOnSelect->SetFocus();
    m_pWinFocusOnSelect = NULL;
  }
}

bool CPanelLabLocusThresholds::Show(bool show)
{
  bool bUpdate = show && !IsShown();
  bool bRtn = SUPER_CPanelLabLocusThresholds::Show(show);
  if(bUpdate)
  {
    _UpdateView();
  }
  return bRtn;
}
BEGIN_EVENT_TABLE(CPanelLabLocusThresholds,SUPER_CPanelLabLocusThresholds)
EVT_SIZE(CPanelLabLocusThresholds::OnSize)
END_EVENT_TABLE()