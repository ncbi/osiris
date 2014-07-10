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
*  FileName: CDialogAcceptAlerts.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include <wx/debug.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "CDialogAcceptAlerts.h"
#include "CPanelSampleAlertNotebook.h"
#include "CGridAlerts.h"
#include "CPanelUserID.h"
#include "COARfile.h"
#include "wxIDS.h"

CDialogAcceptAlerts::CDialogAcceptAlerts(
    int nSelect,
    COARfile *pFile,
    COARsample *pSample,
    const map<int,wxString> &,
    wxWindow *parent,
    wxWindowID id,
    wxSize sz) : 
      wxDialog(parent, id, wxEmptyString,
        wxDefaultPosition,sz,
        mainApp::DIALOG_STYLE),
      m_pAccept(NULL)
{
  vector<wxString> vsLocus;
  bool bOK = true;
  int nType = 0;
  switch(nSelect)
  {
  case SA_NDX_SAMPLE:
    m_msgs.CopyOnly(*pFile->GetMessages(),pSample->GetSampleAlerts()->Get());
    m_pAccept = new COARSampleAppendAccept(pSample);
    break;
  case SA_NDX_ILS:
    m_msgs.CopyOnly(*pFile->GetMessages(),pSample->GetILSAlerts()->Get());
    m_pAccept = new COARsampleILSAppendAccept(pSample);
    break;
  case SA_NDX_CHANNEL:
    {
      vector<int> vn;
      pSample->AppendChannelAlerts(&vn);
      m_msgs.CopyOnly(*pFile->GetMessages(),&vn);
      m_pAccept = new COARsampleChannelAppendAccept(pSample);
      nType = CGridAlerts::TYPE_CHANNEL;
    }
    break;
  case SA_NDX_DIR:
    {
      pFile->GetDirectoryAlerts()->BuildMessageList(
        &m_msgs,&vsLocus,*pFile->GetMessages());
      m_pAccept = new COARfileDirAccept(pFile);
      nType = CGridAlerts::TYPE_LOCUS;
    }
    break;
  default:
    {
      wxString sMsg;
      sMsg.Printf(
        "CDialogAcceptAlerts::CDialogAcceptAlerts()\n"
        "Invalid selection %d",nSelect);
      wxASSERT_MSG(0,sMsg);
      mainApp::LogMessage(sMsg);
      bOK = false;
    }
  }
  if(bOK)
  {
    const wxString &sAlertType(
      CPanelSampleAlertNotebook::GetLabel(nSelect));
    wxString sTitle("Accept ");
    wxStaticText *pLabel = NULL;
    CGridAlerts *pGrid;
    CPanelUserID *pPanelUser;
    wxBoxSizer *pSizer;
    int FLAG =
      UID_BTN_ACCEPT |
      UID_BTN_CANCEL |
      UID_BTN_EDIT |
      UID_SPACER_BTN_CENTER |
      UID_SEND_BTN_EVENTS;
    bool bAllowUserOverride = pFile->GetReviewerAllowUserOverride();
    
    sTitle.Append(sAlertType);
    SetTitle(sTitle);
    if(nSelect != SA_NDX_DIR)
    {
      sTitle.Append(" for ");
      sTitle.Append(pSample->GetName());
      pLabel = new wxStaticText(this,wxID_ANY,sTitle);
      mainApp::SetBoldFont(pLabel);
    }

    pGrid = new CGridAlerts(
      &m_msgs,this,wxID_ANY,nType,true);
    pGrid->SetupChannelColumn(pSample);
    pGrid->SetupLocusColumn(vsLocus);

    pPanelUser = new CPanelUserID(
      this,wxID_ANY,wxID_ANY,m_pAccept,FLAG,!bAllowUserOverride);
    pSizer = new wxBoxSizer(wxVERTICAL);
    int nGRID_FLAG = wxLEFT | wxRIGHT | wxEXPAND;
    if(pLabel != NULL)
    {
      pSizer->Add(
        pLabel,0,
        wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
        ID_BORDER);
    }
    else
    {
      nGRID_FLAG |= wxTOP;
    }
    pSizer->Add(
      pGrid,1, nGRID_FLAG, ID_BORDER);
    pSizer->Add(
      pPanelUser,0,wxALL | wxEXPAND, ID_BORDER);
    SetSizer(pSizer);
    pSizer->Layout();
    CentreOnParent();
  }
}
CDialogAcceptAlerts::~CDialogAcceptAlerts()
{
  if(m_pAccept != NULL)
  {
    delete m_pAccept;
    m_pAccept = NULL;
  }
}


void CDialogAcceptAlerts::OnEdit(wxCommandEvent &e)
{
  int n = e.GetId();
  EndModal(n);
}

BEGIN_EVENT_TABLE(CDialogAcceptAlerts,wxDialog)
EVT_BUTTON(IDmenuEditCell,CDialogAcceptAlerts::OnEdit)
END_EVENT_TABLE()
