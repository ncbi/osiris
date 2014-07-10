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
    nwxPanel(parent,id),
    m_pData(NULL)
{
  wxArrayString aChoices;
  aChoices.Alloc(2);
  aChoices.Add("RFU");
  aChoices.Add("% of Min. RFU");
  wxStaticText *psSampleLabel = 
    new wxStaticText(this,wxID_ANY,
    "Locus Limits for Samples");
  wxStaticText *psLadderLabel =
    new wxStaticText(this,wxID_ANY,
    "Locus Limits for Ladders/ILS Limits");
  mainApp::SetBoldFont(psSampleLabel);
  mainApp::SetBoldFont(psLadderLabel);
  wxStaticText *psDisclaimer =
    new wxStaticText(this,wxID_ANY,
    "* Note: The default settings for "
      "ladder do not affect the ILS.");
  wxStaticText *psChoice = 
    new wxStaticText(this,wxID_ANY,
    "Min. homozygote threshold units:");
  m_pGridSample = new CGridLabThresholdsSample(this,wxID_ANY);
  m_pGridLadder = new CGridLabThresholdsLadder(this,wxID_ANY);
  m_pChoiceHomozygoteUnits = new CChoiceHomozygote(this,wxID_ANY);
  
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerChoice = new wxBoxSizer(wxHORIZONTAL);
  pSizerChoice->Add(psChoice,0,wxALIGN_CENTRE_VERTICAL);
  pSizerChoice->Add(m_pChoiceHomozygoteUnits,
    0,wxLEFT | wxALIGN_CENTRE_VERTICAL, ID_BORDER);
  pSizer->Add(psSampleLabel,0, wxALIGN_LEFT | (wxALL ^ wxBOTTOM), ID_BORDER);
  pSizer->Add(m_pGridSample,0,wxEXPAND | (wxALL ^ wxTOP), ID_BORDER);
  pSizer->Add(pSizerChoice,0,wxALIGN_LEFT | (wxALL ^ wxTOP), ID_BORDER);
  pSizer->AddSpacer(ID_BORDER << 2);
  pSizer->Add(psLadderLabel,0,wxALIGN_LEFT | wxLEFT | wxRIGHT,ID_BORDER);
  pSizer->Add(m_pGridLadder,0,wxEXPAND | wxLEFT | wxRIGHT, ID_BORDER);
  pSizer->Add(psDisclaimer,0,wxALIGN_LEFT | (wxALL ^ wxTOP),ID_BORDER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
}

CPanelLabLocusThresholds::~CPanelLabLocusThresholds() {}

void CPanelLabLocusThresholds::SetReadOnly(bool bReadOnly)
{
  m_pChoiceHomozygoteUnits->Enable(!bReadOnly);
  m_pGridSample->SetAllReadOnly(bReadOnly);
  m_pGridLadder->SetAllReadOnly(bReadOnly);
}
bool CPanelLabLocusThresholds::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    m_pChoiceHomozygoteUnits->SetSelectionByValue(
      m_pData->GetMinBoundHomozygoteUnit());
    if(!m_pGridSample->TransferDataToWindow())
    {
      bRtn = false;
    }
    if(!m_pGridLadder->TransferDataToWindow())
    {
      bRtn = false;
    }
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
    if(!m_pGridSample->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pGridLadder->TransferDataFromWindow())
    {
      bRtn = false;
    }
  }

  return bRtn;
}

