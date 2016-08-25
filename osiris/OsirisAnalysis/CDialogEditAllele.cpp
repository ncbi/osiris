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
*  FileName: CDialogEditAllele.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CDialogEditAllele.h"
#include "CPanelUserID.h"
#include <wx/sizer.h>
#include "wxIDS.h"
#include "CPanelApplyCancel.h"

//*************************************  CDialogEditAllele

CDialogEditAllele::~CDialogEditAllele() {;}

CDialogEditAllele::CDialogEditAllele(
  const COARlocus &Locus,
  const COARmessages &MsgIn,
  bool bAllowUserNameOverride,
  wxWindow *parent, wxWindowID id,
  const wxString &sSampleName,
  const wxSize &sz) :
    wxDialog(
      parent,id,wxEmptyString,
      wxDefaultPosition, sz,
      mainApp::DIALOG_STYLE),
    m_bAllowUserNameOverride(bAllowUserNameOverride)
{
  m_pSample = NULL;
  m_pPanel = new CPanelLocusDetails(
    &Locus,&MsgIn,this,wxID_ANY,true,false);
  _InitCommon(Locus.GetName(),sSampleName);

}
CDialogEditAllele::CDialogEditAllele(
  COARsample *pSample,
  int nChannel,
  const COARlocus &Locus,
  const COARmessages &MsgIn,
  bool bAllowUserNameOverride,
  wxWindow *parent, wxWindowID id,
  const wxString &sSampleName,
  const wxSize &sz) :
    wxDialog(
      parent,id,wxEmptyString,
      wxDefaultPosition, sz,
      mainApp::DIALOG_STYLE),
    m_bAllowUserNameOverride(bAllowUserNameOverride)
{
  m_pSample = pSample;
  m_pPanel = new CPanelLocusDetails(
    pSample, nChannel,
    &Locus,&MsgIn,this,wxID_ANY,true,true,false);
  _InitCommon(Locus.GetName(),sSampleName);
}
void CDialogEditAllele::_InitCommon(
  const wxString &sLocusName, const wxString &sSampleName)
{
  wxString sTitle("Edit Alleles for ");
  wxBoxSizer *pSizer;
  sTitle.Append(sLocusName);
  if(!sSampleName.IsEmpty())
  {
    sTitle.Append(" on ");
    sTitle.Append(sSampleName);
  }
  SetTitle(sTitle);
  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pPanel,1, wxEXPAND);
  if(m_pSample == NULL)
  {
    m_pButtonBar = new CPanelApplyCancel(this);
    m_pButtonUser = NULL;
    pSizer->Add(m_pButtonBar,0,wxALIGN_CENTRE);
  }
  else
  {
    m_pButtonBar = NULL;
    m_pButtonUser = new CPanelUserID(
      this,wxID_ANY,IDeditUser,
      NULL,
      UID_BTN_OK | UID_BTN_CANCEL | 
        UID_SPACER_BTN_CENTER | UID_SEND_BTN_EVENTS,
      !m_bAllowUserNameOverride);
    m_pPanel->SetPanelUserID(m_pButtonUser);
    pSizer->Add(m_pButtonUser,0,wxALL | wxEXPAND, ID_BORDER);
  }
  SetSizer(pSizer);
  pSizer->Layout();
  CentreOnParent();
  m_pPanel->TransferDataToWindow();
}

bool CDialogEditAllele::Validate()
{
  return m_pPanel->Validate();
}

bool CDialogEditAllele::EditData(COARfile *pf, COARsample *pSample)
{
  //  in the future, pSample will not be a parameter here because the
  //  constructor without pSample will be eliminated
  int n = ShowModal();
  bool bRtn =
    (n == wxID_OK || n == wxID_YES) &&
    IsModified();
  if(bRtn)
  {
    if(pSample == NULL)
    {
      pSample = m_pSample;
    }
    m_pPanel->UpdateFile(pf,pSample);
  }
  return bRtn;
}

bool CDialogEditAllele::CheckCancel()
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

void CDialogEditAllele::OnCancel(wxCommandEvent &)
{
  CheckCancel();
}

void CDialogEditAllele::OnChange(wxCommandEvent &)
{
  bool bModified = IsModified();
  if(m_pButtonBar != NULL)
  {
    m_pButtonBar->EnableApply(bModified);
  }
  else
  {
    m_pButtonUser->Enable(bModified);
  }
}


BEGIN_EVENT_TABLE(CDialogEditAllele,wxDialog)
EVT_BUTTON(wxID_CANCEL,CDialogEditAllele::OnCancel)
EVT_COMMAND_ENTER(wxID_ANY, CDialogEditAllele::OnChange)
END_EVENT_TABLE()


