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
*  FileName: CDialogToggleEnabled.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CDialogToggleEnabled.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"
#include "CPanelUserID.h"

CDialogToggleEnabled::CDialogToggleEnabled(
  COARsample *pSample, 
  bool bAllowUserNameOverride,
  wxWindow *parent)
  : wxDialog(parent,wxID_ANY,_T(""),wxDefaultPosition, wxDefaultSize,
      mainApp::DIALOG_STYLE & (~wxRESIZE_BORDER) ),
    m_toggler(pSample)
{
  wxString sTitle;
  wxString sMessage;
  const int FLAG =
    UID_BTN_OK |
    UID_BTN_CANCEL |
    UID_SPACER_BTN_RIGHT |
    UID_SEND_BTN_EVENTS;
  bool bEnabled = pSample->IsEnabled();

  sTitle = COARsample::EnableDisableSampleStr(!bEnabled);
  sMessage = COARsample::EnableDisableStr(!bEnabled);
  SetTitle(sTitle);
  sMessage.Append(pSample->GetName());
  sMessage.Append(_T("?"));

  wxStaticText *pMessage = new wxStaticText(this,wxID_ANY,sMessage);
  CPanelUserID *pPanel = new CPanelUserID(
    this,wxID_ANY,wxID_ANY,&m_toggler,FLAG,!bAllowUserNameOverride);
  pPanel->UpdateSize();

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(
    pMessage,1,(wxALL ^ wxBOTTOM) | wxCENTER, ID_BORDER);
  pSizer->Add(
    pPanel,0,wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
  Fit();
  CentreOnParent();
}
