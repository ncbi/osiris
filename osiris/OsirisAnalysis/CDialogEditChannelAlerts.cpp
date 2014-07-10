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
*  FileName: CDialogEditChannelAlerts.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CDialogEditChannelAlerts.h"
#include "COARfile.h"
#include <wx/sizer.h>
#include "wxIDS.h"
#include "CPanelUserID.h"

CDialogEditChannelAlerts::~CDialogEditChannelAlerts() {;}

CDialogEditChannelAlerts::CDialogEditChannelAlerts(
  const COARfile *pFile,
  const COARsample *pSample,
  wxWindow *parent,
  wxWindowID id,
  const wxSize &sz,
  const map<int,wxString> *, // future use
  int nSelect) :
    wxDialog(
      parent,id,wxEmptyString,
      wxDefaultPosition, sz,
      mainApp::DIALOG_STYLE)
{
  wxString sTitle("Edit Alerts for ");
  wxBoxSizer *pSizer;
  sTitle.Append(pSample->GetName());
  SetTitle(sTitle);
  m_pPanel = new CPanelSampleAlertNotebook(
    pFile, pSample, this,wxID_ANY,NULL,true,false,nSelect);
  m_pPanelUser = new CPanelUserID(this,wxID_ANY,IDeditUser,NULL,
      UID_BTN_OK | UID_BTN_CANCEL | 
        UID_SPACER_BTN_CENTER | UID_SEND_BTN_EVENTS,
        !pFile->GetReviewerAllowUserOverride());
  m_pPanelUser->Enable(false);
  m_pPanel->SetPanelUserID(m_pPanelUser);
  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pPanel,1,(wxALL ^ wxBOTTOM) | wxEXPAND, ID_BORDER);
  pSizer->Add(m_pPanelUser,0,wxALL | wxEXPAND, ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
  CentreOnParent();
  m_pPanel->TransferDataToWindow();
}

bool CDialogEditChannelAlerts::Validate()
{
  return m_pPanel->Validate();
}

bool CDialogEditChannelAlerts::EditData(
  COARfile *pf, COARsample *pSample)
{
  int n = ShowModal();
  bool bRtn =
    (n == wxID_OK || n == wxID_YES) &&
    IsModified();
  if(bRtn)
  {
    wxDateTime t;
    wxString sUser = m_pPanelUser->GetValue();
    t.SetToCurrent();
    pf->SetIsModified(true);
    if(m_pPanel->IsModified(SA_NDX_CHANNEL))
    {
      pf->UpdateMessages(GetChannelMessages(),t);
      pSample->AppendNotesChannel(m_pPanel->GetNewChannelNotes(),sUser);
      pSample->AppendChannelReview(sUser);
    }
    if(m_pPanel->IsModified(SA_NDX_ILS))
    {
      pf->UpdateMessages(GetILSMessages(),t);
      pSample->AppendNotesILS(m_pPanel->GetNewILSNotes(),sUser);
      pSample->AppendILSReview(sUser);
    }
    if(m_pPanel->IsModified(SA_NDX_SAMPLE))
    {
      pf->UpdateMessages(GetSampleMessages(),t);
      pSample->AppendNotesSample(m_pPanel->GetNewSampleNotes(),sUser);
      pSample->AppendSampleReview(sUser);
    }
    if(m_pPanel->IsModified(SA_NDX_DIR))
    {
      pf->UpdateMessages(GetDirMessages(),t);
      pf->AppendNotesDir(m_pPanel->GetNewDirNotes(),sUser);
      pf->AppendDirReview(sUser);
    }
  }
  return bRtn;
}

bool CDialogEditChannelAlerts::CheckCancel()
{
  bool bRtn = true;
  if(IsModified())
  {
    bRtn = mainApp::ConfirmModificationsLost(this);
  }
  if(bRtn)
  {
    EndModal(wxID_CANCEL);
  }
  return bRtn;
}
void CDialogEditChannelAlerts::OnCancel(wxCommandEvent &)
{
  CheckCancel();
}

void CDialogEditChannelAlerts::OnChange(wxCommandEvent &)
{
  m_pPanelUser->Enable(IsModified());
}


BEGIN_EVENT_TABLE(CDialogEditChannelAlerts,wxDialog)
EVT_BUTTON(wxID_CANCEL,CDialogEditChannelAlerts::OnCancel)
EVT_COMMAND_ENTER(wxID_ANY, CDialogEditChannelAlerts::OnChange)
END_EVENT_TABLE()
