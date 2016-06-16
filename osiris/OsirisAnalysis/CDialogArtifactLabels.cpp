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
*  FileName: CDialogArtifactLabels.cpp
*  Author:   Douglas Hoffman
*
*  Dialog window for modifying label for artifact on plots
* 
*/
#include "mainApp.h"
#include "CDialogArtifactLabels.h"
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/font.h>
#include <wx/sizer.h>
#include "CArtifactLabelsUser.h"
#include "CArtifactLabels.h"
#include "wxIDS.h"
#include "ConfigDir.h"

#ifdef __WXMSW__
#ifdef __WXDEBUG__
#include <crtdbg.h>
#endif
#endif

IMPLEMENT_ABSTRACT_CLASS(CDialogArtifactLabels,wxDialog)

CDialogArtifactLabels::CDialogArtifactLabels(wxWindow *parent) :
  wxDialog(parent,
      wxID_ANY,
      wxT("Artifact Labels"),
      wxDefaultPosition, 
      wxDefaultSize, 
      mainApp::DIALOG_STYLE),
        m_pA(NULL),
        m_nSelected(wxNOT_FOUND),
        m_bReadOnly(false),
        m_bModified(false)

{
  const wxChar DN = 0x25bc;
  const wxChar UP = 0x25b2;
  wxStaticText *pLabelArtifact = new wxStaticText(this,wxID_ANY,wxT("Artifact: "));
  m_pLabelArtifactLabel =
    new wxStaticText(this,wxID_ANY,wxEmptyString);
  m_pList = new wxListBox(
    this,
    wxID_ANY,
    wxDefaultPosition, 
    wxDefaultSize,
    0,
    (const wxString *)NULL,
    wxLB_SINGLE | wxLB_NEEDED_SB);
  m_pButtonUP = new wxButton(this,IDback,wxString(UP),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxStaticText *pLabelPriority = new wxStaticText(this,wxID_ANY,wxT("Priority"));
  m_pButtonDN = new wxButton(this,IDnext,wxString(DN),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxStaticText *pLabelDisplay = new wxStaticText(this,wxID_ANY,wxT("Display"));
  m_pCheckDefault = new wxCheckBox(this,wxID_ANY,wxT(" Use Default"));
  m_pTextDisplay = new wxTextCtrl(this,wxID_ANY);

  m_pTextDisplay->SetMaxLength(CArtifactLabelsUser::MAX_LENGTH);
  wxFont font = m_pLabelArtifactLabel->GetFont();
  font.SetWeight(wxFONTWEIGHT_BOLD);
  m_pLabelArtifactLabel->SetFont(font);

  wxBoxSizer *pSizerMain = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerLR = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerPriority = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerRight = new wxBoxSizer(wxVERTICAL);
  pSizerTop->Add(pLabelArtifact,0,wxALL,ID_BORDER);
  pSizerTop->Add(m_pLabelArtifactLabel,1,(wxALL ^ wxLEFT) | wxEXPAND,ID_BORDER);
  pSizerLR->Add(m_pList,1, (wxALL ^ wxTOP) | wxEXPAND,ID_BORDER);
  pSizerPriority->Add(m_pButtonUP,0,(wxALL ^ wxTOP) | wxALIGN_CENTER, ID_BORDER);
  pSizerPriority->Add(pLabelPriority,0,(wxALL ^ wxTOP) | wxALIGN_CENTER, ID_BORDER);
  pSizerPriority->Add(m_pButtonDN,0,(wxALL ^ wxTOP) | wxALIGN_CENTER, ID_BORDER);
  pSizerRight->Add(pSizerPriority,0,(wxALL ^ wxTOP) | wxALIGN_LEFT, ID_BORDER);
  pSizerRight->Add(pLabelDisplay,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  pSizerRight->Add(m_pCheckDefault,0,(wxALL ^ wxTOP) | wxALIGN_LEFT,ID_BORDER);
  pSizerRight->Add(m_pTextDisplay,0,wxALL | wxALIGN_LEFT | wxEXPAND, ID_BORDER);
  pSizerRight->AddStretchSpacer(1);
  pSizerLR->Add(pSizerRight,0,wxALL ^ wxTOP, ID_BORDER);
  pSizerMain->Add(pSizerTop,0,0,0);
  pSizerMain->Add(pSizerLR,0,wxEXPAND,0);
  pSizerMain->Add(CreateButtonSizer(wxOK | wxCANCEL),0,(wxALL ^ wxTOP) | wxALIGN_CENTER, ID_BORDER);
  SetSizer(pSizerMain);
}
CDialogArtifactLabels::~CDialogArtifactLabels()
{
  _Cleanup();
}

bool CDialogArtifactLabels::TransferDataFromWindow()
{
  bool bRtn = true;
  if(!m_bReadOnly)
  {
    int n = m_pList->GetCount();
    int nPriority = n;
    int i;
    CArtifactGroupUser *p;
    _UpdateSelectionData();  // update current selection

    // set priority for each label
    for(i = 0; i < n; ++i)
    {
      p = (CArtifactGroupUser *)(m_pList->GetClientData(i));
      p->SetPriority(nPriority);
      --nPriority;
    }
    m_pA->Sort();
    CArtifactLabels *pLabels = mainApp::GetArtifactLabels();
    if(pLabels->SetUserLabels(*m_pA)) // true if something changed
    {
      m_bModified = true;
      bRtn = m_pA->SaveFile(mainApp::GetConfig()->GetArtifactLabelsUserFileName());
      if(!bRtn)
      {
        mainApp::ShowError(wxT("Could not save artifact labels file"),this);
      }
    }
  }
  return bRtn;
}
void CDialogArtifactLabels::_Cleanup()
{
  if(m_pA != NULL)
  {
    delete m_pA;
    m_pA = NULL;
  }
}
bool CDialogArtifactLabels::TransferDataToWindow()
{
  bool bRtn = false;
  CArtifactLabels *pLabels = mainApp::GetArtifactLabels();
  bool bReadOnly = false;

  m_pList->Clear();
  m_nSelected = wxNOT_FOUND;
  if(pLabels != NULL)
  {
    _Cleanup();
    m_pA = pLabels->BuildUserLabels();
    size_t n = m_pA->GetCount();
    if(n > 0)
    {
      CArtifactGroupUser *pgUser;
      bRtn = true;
      for(size_t i = 0; i < n; i++)
      {
        pgUser = m_pA->Get(i);
        m_pList->Append(pgUser->GetLabel(),(void *)pgUser);
      }
    }
    m_pList->SetSelection(0);
  }
  if(!bRtn)
  {
    wxString sMsg;
    sMsg = wxT("Artifact labels file is either missing or invalid");
    mainApp::ShowError(sMsg,NULL);
  }
  else 
  {
    if(!mainApp::GetConfig()->IsSitePathWritable())
    {
      bReadOnly = true;
      mainApp::ShowAlert(
        wxT("Artifact labels cannot be\nmodified by this user account"),
        NULL);
    }
    SetReadOnly(bReadOnly);
    OnListSelect(*((wxCommandEvent*)NULL));
  }
  return bRtn;
}
bool CDialogArtifactLabels::_CanSaveFile()
{
  bool bRtn = mainApp::GetConfig()->IsSitePathWritable();
  if(bRtn)
  {
    wxString s = mainApp::GetConfig()->GetArtifactLabelsUserFileName();
    wxFileName fn(s);
    if(!fn.Exists()) {} // OK can create
    else if(fn.IsDir())
    {
      bRtn = false; // it's a directory, cannot create file
    }
    else if(!fn.IsFileWritable())
    {
      bRtn = false; // exists and not writable
    }
  }
  return bRtn;
}
void CDialogArtifactLabels::_SetupButtons()
{
  bool bEnableUP = false;
  bool bEnableDN = false;
  if(!m_bReadOnly)
  {
    int n = m_pList->GetSelection();
    int nMax = (int)m_pList->GetCount() - 1;
    bEnableUP = (n > 0);
    bEnableDN = (n < nMax);
  }
  m_pButtonUP->Enable(bEnableUP);
  m_pButtonDN->Enable(bEnableDN);
}
void CDialogArtifactLabels::SetReadOnly(bool bReadOnly)
{
  bool bEnable = !bReadOnly;
  if(bEnable)
  {
    _SetupButtons();
  }
  else
  {
    m_pButtonUP->Enable(false);
    m_pButtonDN->Enable(false);
  }
  m_pCheckDefault->Enable(bEnable);
  m_pTextDisplay->Enable(bEnable);
  m_bReadOnly = bReadOnly;
}
void CDialogArtifactLabels::OnCheck(wxCommandEvent &)
{
  int nSelected = m_pList->GetSelection();
  bool bDefault = m_pCheckDefault->GetValue();
  CArtifactGroupUser *p = (CArtifactGroupUser *)(m_pList->GetClientData(nSelected));
  m_pTextDisplay->SetValue(bDefault ? p->GetDisplayDefault() : p->GetDisplayUser());
  m_pTextDisplay->Enable(!(bDefault || m_bReadOnly));
  if(!bDefault) {m_pTextDisplay->SetFocus();}
}
void CDialogArtifactLabels::_swap(int nSelected, int nAdjacent)
{
  wxString s = m_pList->GetString(nAdjacent);
  void *pData = m_pList->GetClientData(nAdjacent);
  m_pList->Delete(nAdjacent);
  m_pList->Insert(s,nSelected,pData);
  m_nSelected = nAdjacent;
  _SetupButtons();
}
void CDialogArtifactLabels::OnMoveUp(wxCommandEvent &)
{
  int nSelected = m_pList->GetSelection();
  if(nSelected > 0)
  {
    _swap(nSelected,nSelected - 1);
  }
}
void CDialogArtifactLabels::OnMoveDown(wxCommandEvent &)
{
  int nSelected = m_pList->GetSelection();
  int nMax = (int)m_pList->GetCount() - 1;
  if(nSelected < nMax)
  {
    _swap(nSelected,nSelected + 1);
  }
}
void CDialogArtifactLabels::OnListSelect(wxCommandEvent &)
{
  int nSelected = m_pList->GetSelection();
  if(nSelected != m_nSelected)
  {
    if(m_nSelected != wxNOT_FOUND)
    {
      _UpdateSelectionData();
    }
    m_nSelected = nSelected;
    CArtifactGroupUser *p = (CArtifactGroupUser *)(m_pList->GetClientData(nSelected));
    const wxString &sDisplay = p->GetDisplayUser();
    bool bDefault = sDisplay.IsEmpty();
    m_pCheckDefault->SetValue(bDefault);
    m_pTextDisplay->SetValue(bDefault ? p->GetDisplayDefault() : sDisplay);
    m_pTextDisplay->Enable(!(bDefault || m_bReadOnly));
    m_pLabelArtifactLabel->SetLabel(p->GetLabel());
    _SetupButtons();
  }
}
void CDialogArtifactLabels::_UpdateSelectionData()
{
  if(!m_bReadOnly)
  {
    CArtifactGroupUser *p = (CArtifactGroupUser *)(m_pList->GetClientData(m_nSelected));
    wxString s;
    if(!m_pCheckDefault->GetValue())
    {
      s = m_pTextDisplay->GetValue();
    }
    p->SetDisplay(s);
  }
}

BEGIN_EVENT_TABLE(CDialogArtifactLabels,wxDialog)

EVT_CHECKBOX(wxID_ANY,CDialogArtifactLabels::OnCheck)
EVT_LISTBOX(wxID_ANY,CDialogArtifactLabels::OnListSelect)
EVT_BUTTON(IDback,CDialogArtifactLabels::OnMoveUp)
EVT_BUTTON(IDnext,CDialogArtifactLabels::OnMoveDown)

END_EVENT_TABLE()
