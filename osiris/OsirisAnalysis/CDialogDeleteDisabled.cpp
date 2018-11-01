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
*  FileName: CDialogDeleteDisabled.cpp
*  Author:   Douglas Hoffman
*/

#include "mainApp.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "CDialogDeleteDisabled.h"
#include "CPanelUserID.h"

CDialogDeleteDisabled::~CDialogDeleteDisabled() {}

CDialogDeleteDisabled::CDialogDeleteDisabled(
  wxWindow *parent, const std::vector<wxString> &vsNames, bool bAllowUserOverride) :
    wxDialog(parent,wxID_ANY,wxT("Delete samples..."),
        wxDefaultPosition,wxDefaultSize,
        mainApp::DIALOG_STYLE),
    m_pPanelUser(NULL)
{
  std::vector<wxString>::const_iterator itr;
  wxString sPlural;
  if(vsNames.size() > 1)
  {
    sPlural = wxT("s");
  }
  wxString sTitle = wxT("The following sample");
  sTitle.Append(sPlural);
  sTitle.Append(wxT(" will be deleted:"));  
  wxStaticText *pStat = new wxStaticText(this,wxID_ANY,sTitle);
  mainApp::SetBoldFont(pStat);
  wxBoxSizer *pSizer1 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizer2 = new wxBoxSizer(wxVERTICAL);
  pSizer1->Add(pStat,0,wxALIGN_CENTRE | wxALL,ID_BORDER);
  for(itr = vsNames.begin(); itr != vsNames.end(); ++itr)
  {
    pStat = new wxStaticText(this,wxID_ANY,*itr);
    pSizer2->Add(pStat,0,wxALIGN_LEFT | wxLEFT | wxRIGHT | wxBOTTOM,ID_BORDER);
  }
  pSizer1->Add(pSizer2,0,wxALIGN_CENTRE,0);
  m_pPanelUser = new CPanelUserID(
    this, wxID_ANY, wxID_ANY, NULL, 
    UID_BTN_OK | UID_BTN_CANCEL | 
      UID_SPACER_BTN_RIGHT | UID_SEND_BTN_EVENTS ,
    !bAllowUserOverride);
  pSizer1->Add(m_pPanelUser,0,wxALIGN_CENTRE | wxLEFT | wxRIGHT | wxBOTTOM,ID_BORDER);
  SetSizer(pSizer1);
  Layout();
  Fit();
  CentreOnParent();
}

const wxString &CDialogDeleteDisabled::GetUserID()
{
  return m_pPanelUser->GetValue();
}