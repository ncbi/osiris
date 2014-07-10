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
*  FileName: CPanelLabGeneral.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CPanelLabGeneral.h"
#include "CParmOsiris.h"
#include "CVolumes.h"
#include "CKitList.h"
#include "CDirList.h"
#define ALLOW_USER_OVERRIDE "Allow User to Override"

//********************************************************************
//
//    CPanelLabGeneral
//
CPanelLabGeneral::~CPanelLabGeneral() {}

CPanelLabGeneral::CPanelLabGeneral(
        wxWindow *parent) :
  nwxPanel(parent, wxID_ANY),
  m_pInfo(NULL),
  m_pMarker(NULL)
{
  vector<wxString> vsFileTypes;
  wxRadioButton *pRadio;
  m_pLabelVolumeName = new wxStaticText(this,wxID_ANY,wxEmptyString);
  m_pLabelKitName = new wxStaticText(this,wxID_ANY,wxEmptyString);
  m_pLabelLastUpdateUser = new wxStaticText(this,wxID_ANY,wxEmptyString);

  wxFont fn = m_pLabelVolumeName->GetFont();
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  m_pLabelVolumeName->SetFont(fn);
  m_pLabelKitName->SetFont(fn);

  m_pChoiceILS = new wxChoice(this,wxID_ANY);

  int nFileTypeStyle = wxRB_GROUP;
  int nSizerStyle = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT;
  wxBoxSizer *pSizerRadioFileType = new wxBoxSizer(wxHORIZONTAL);
  CDirList::GetTypes(&vsFileTypes);
  m_vpRadioFileType.reserve(vsFileTypes.size());
  for(vector<wxString>::iterator itr = vsFileTypes.begin();
    itr != vsFileTypes.end();
    ++itr)
  {
    pRadio = new wxRadioButton(
      this,
      wxID_ANY, 
      *itr,
      wxDefaultPosition, 
      wxDefaultSize, 
      nFileTypeStyle);
    pSizerRadioFileType->Add(pRadio,0,nSizerStyle,ID_BORDER << 1);
    nFileTypeStyle = 0;
    nSizerStyle = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT;
    m_vpRadioFileType.push_back(pRadio);
  }

  m_pRadioRaw = new wxRadioButton(
      this,
      wxID_ANY, 
      CParmOsiris::LABEL_DATA_RAW,
      wxDefaultPosition, 
      wxDefaultSize, 
      wxRB_GROUP);
  m_pRadioAnalyzed = new wxRadioButton(
      this,
      wxID_ANY,
      CParmOsiris::LABEL_DATA_ANALYZED,
      wxDefaultPosition,
      wxDefaultSize);

  m_pCheckUserOverride = new wxCheckBox(this,wxID_ANY, ALLOW_USER_OVERRIDE);
  m_pTextProtocol = new wxTextCtrl(this, wxID_ANY);
  m_pTextLot = new wxTextCtrl(this,wxID_ANY);
  m_pTextNotes = new wxTextCtrl(this,wxID_ANY,
    wxEmptyString, wxDefaultPosition,
    wxDefaultSize, wxTE_MULTILINE);

  wxGridBagSizer *pGridSizer = new wxGridBagSizer(ID_BORDER,ID_BORDER);
  wxGBPosition gbPos(0,0);
  _AddLine(wxS(VOLUME_STRING " Name: "),m_pLabelVolumeName,pGridSizer, &gbPos);
  _AddLine(wxS("Reagent Kit: "),m_pLabelKitName,pGridSizer, &gbPos);
  _AddLine(wxS("Last Update: "),m_pLabelLastUpdateUser, pGridSizer, &gbPos);
  _AddLine(wxS("File Type: "), pSizerRadioFileType,pGridSizer,&gbPos);

  wxBoxSizer *pSizerRadio = new wxBoxSizer(wxHORIZONTAL);
  pSizerRadio->Add(m_pRadioRaw,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  pSizerRadio->Add(m_pRadioAnalyzed,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,ID_BORDER << 1);
  _AddLine(wxS("Data:"),pSizerRadio,pGridSizer,&gbPos);
  _AddLine(wxS("Internal Lane Standard: "), m_pChoiceILS,pGridSizer,&gbPos);

  {
    wxGBSpan gbSpan(1,2);
    int nRow = gbPos.GetRow();
    pGridSizer->Add(
      m_pCheckUserOverride,gbPos, gbSpan,
      wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,
      ID_BORDER);
    gbPos.SetRow(++nRow);
  }


  _AddLine(wxS("Protocol: "), m_pTextProtocol,pGridSizer, &gbPos);
  _AddLine(wxS("Lot Nr.: "), m_pTextLot, pGridSizer, &gbPos);

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pGridSizer,0,wxEXPAND | wxALL, ID_BORDER);
  pSizer->Add(
    new wxStaticText(this,wxID_ANY,"Notes:"),
    0,wxLEFT | wxTOP | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(m_pTextNotes,1,wxEXPAND | (wxALL ^ wxTOP), ID_BORDER);
  SetSizer(pSizer);
  // do not Layout() because TransferDataToWindow() will modify the sizes
}
void CPanelLabGeneral::_AddLine(
  const wxChar *psLabel,wxObject *pobj,
  wxGridBagSizer *pSizer,
  wxGBPosition *pPos
  )
{
  wxWindow *pWin = wxDynamicCast(pobj,wxWindow);
  wxSizer *pSz = 
    (pWin == NULL) ? wxDynamicCast(pobj,wxSizer) : NULL;
  wxStaticText *pLabel =
    new wxStaticText(this,wxID_ANY,psLabel);
  int nRow = pPos->GetRow();
  pPos->SetCol(0);
  pSizer->Add(
    pLabel,*pPos, wxDefaultSpan,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  pPos->SetCol(1);
  if(pWin != NULL)
  {
    pSizer->Add(
      pWin,*pPos,wxDefaultSpan,
      wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  }
  else if(pSz != NULL)
  {
    pSizer->Add(
      pSz,*pPos,wxDefaultSpan,
      wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  }
  else
  {
    wxASSERT_MSG(0,"invalid object");
  }
  pPos->SetRow(++nRow);
}

void CPanelLabGeneral::SetReadOnly(bool b)
{
  bool bEnable = !b;
  m_pChoiceILS->Enable(bEnable);
  m_pRadioRaw->Enable(bEnable);
  m_pRadioAnalyzed->Enable(bEnable);
  for(vector<wxRadioButton *>::iterator itr = m_vpRadioFileType.begin();
    itr != m_vpRadioFileType.end();
    ++itr)
  {
    (*itr)->Enable(bEnable);
  }
  m_pCheckUserOverride->Enable(bEnable);
  m_pTextProtocol->SetEditable(bEnable);
  m_pTextLot->SetEditable(bEnable);
  m_pTextNotes->SetEditable(bEnable);
}

bool CPanelLabGeneral::TransferDataFromWindow()
{
  wxString sError;
  bool bRtn = (m_pInfo != NULL) && (m_pMarker != NULL);
  if(bRtn)
  {
    wxString s = m_pChoiceILS->GetStringSelection();
    if(s.IsEmpty())
    {
      bRtn = false;
      sError = "No ILS has been selected";
    }
    else
    {
      m_pMarker->SetILSName(s);

      m_pMarker->SetAllowUserOverrideILS(
        m_pCheckUserOverride->GetValue());
      m_pMarker->SetAnalyzed(m_pRadioAnalyzed->GetValue());

      s = m_pTextProtocol->GetValue();
      nwxString::Trim(&s);
      m_pInfo->SetProtocol(s);

      s = m_pTextLot->GetValue();
      nwxString::Trim(&s);
      m_pInfo->SetLot(s);

      s = m_pTextNotes->GetValue();
      nwxString::Trim(&s);
      m_pInfo->SetNotes(s);

      s.Clear();
      for(vector<wxRadioButton *>::iterator itr = this->m_vpRadioFileType.begin();
        itr != m_vpRadioFileType.end();
        itr++)
      {
        if((*itr)->GetValue())
        {
          s = (*itr)->GetLabelText();
          break;
        }
      }
      m_pInfo->SetDataFileType(s);
    }
  }
  else
  {
    sError = "Cannot save " Volume_string " information";
  }
  if(!bRtn)
  {
    mainApp::ShowError(sError,this);
  }
  return bRtn;
}

bool CPanelLabGeneral::TransferDataToWindow()
{
  bool bRtn = (m_pInfo != NULL) && (m_pMarker != NULL);
  if(bRtn)
  {
    wxString sValue;
    wxString s;
    const wxString &sKitName = m_pMarker->GetMarkerSetName();
    const wxString &sILS = m_pMarker->GetILSName();
    CPersistKitList *pKitList = mainApp::GetKitList();
    const wxArrayString *psILS = pKitList->GetLsArray(sKitName);
    size_t nSize = 
      (psILS == NULL)
      ? 0
      : psILS->GetCount();

    m_pChoiceILS->Clear();
    if(!nSize)
    {
      wxString sError;
      bRtn = false;
      sError = "Cannot file ILS for kit, ";
      sError.Append(sKitName);
      mainApp::LogMessage(sError);
      wxASSERT_MSG(0,sError);
    }
    else
    {
      m_pChoiceILS->Append(*psILS);
      if(sILS.IsEmpty() || !m_pChoiceILS->SetStringSelection(sILS))
      {
        m_pChoiceILS->Select(0);
      }
      m_pLabelVolumeName->SetLabel(m_pInfo->GetVolumeName());
      m_pLabelKitName->SetLabel(sKitName);
      const wxDateTime &dt = m_pInfo->GetLastUpdate();
      sValue.Empty();
      if(dt.GetTicks() > 100)
      {
        const wxString &sUser = m_pInfo->GetUserID();
        sValue = dt.Format();
        if(!sUser.IsEmpty())
        {
          sValue.Append(" by ");
          sValue.Append(sUser);
        }
      }
      m_pLabelLastUpdateUser->SetLabel(sValue);
      m_pCheckUserOverride->SetValue(m_pMarker->GetAllowUserOverrideILS());
      m_pTextProtocol->SetValue(m_pInfo->GetProtocol());
      m_pTextLot->SetValue(m_pInfo->GetLot());
      m_pTextNotes->SetValue(m_pInfo->GetNotes());

      bool bDataAnalyzed = m_pMarker->GetAnalyzed();
      m_pRadioRaw->SetValue(!bDataAnalyzed);
      m_pRadioAnalyzed->SetValue(bDataAnalyzed);

      wxRadioButton *pRadio;
      wxString sValue = m_pInfo->GetDataFileType();
      size_t nLEN = m_vpRadioFileType.size();
      bool bFound = false;
      bool b;
      if(sValue.Len())
      {
        for(size_t n = 0; n < nLEN; n++)
        {
          pRadio = m_vpRadioFileType.at(n);
          b = (pRadio->GetLabelText() == sValue);
          if(b)
          {
            bFound = true;
          }
          pRadio->SetValue(b);
        }
      }
      if(!bFound)
      {
        m_vpRadioFileType.at(0)->SetValue(true);
      }
    }
  }
  return bRtn;  
}
