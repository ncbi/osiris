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
*  FileName: CDialogWarnHistory.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CDialogWarnHistory.h"
#include "mainApp.h"
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "CParmOsiris.h"
#include "wxIDS.h"

bool CDialogWarnHistory::Continue(wxWindow *parent, bool bAsk)
{
  CParmOsirisGlobal parm;
  bool bRtn = true;
  if(parm.Get()->WarnOnHistory())
  {
    CDialogWarnHistory dlg(parent, wxID_ANY, bAsk);
    int n = dlg.ShowModal();
    if(n == wxID_CANCEL || n == wxID_NO)
    {
      bRtn = false;
    }
    else if(dlg.DontShowAgain())
    {
      parm.Get()->SetWarnOnHistory(false);
    }
  }
  return bRtn;
}

CDialogWarnHistory::~CDialogWarnHistory() {}

CDialogWarnHistory::CDialogWarnHistory(
  wxWindow *parent, wxWindowID id, bool bAsk) :
    wxDialog(parent,id,wxString("Warning"))
{
  wxStaticText *pWarning = new wxStaticText(
    this,wxID_ANY,"Warning!");
  wxString s(
    "The history setting is not set to \"current.\"\n"
    "Any changes will be reflected when saving the file\n"
    "but will not appear on the screen until the history\n"
    "setting is changed to \"current.\""
  );
  if (bAsk)
  {
    s.Append("\n\nDo you wish to continue?");
  }
  wxStaticText *pHistory = new wxStaticText(this,wxID_ANY,
    s, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
  mainApp::SetBoldFont(pWarning);
  wxSizer *pButtons = CreateButtonSizer(bAsk ? (wxYES | wxNO) : wxOK); 
  m_pCheckBox = new wxCheckBox(this,wxID_ANY," Don't show this window again");
  m_pButtonNO = NULL;
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

  pSizer->Add(pWarning, 0, (wxALL ^ wxBOTTOM) | wxALIGN_CENTRE_HORIZONTAL, ID_BORDER);
  pSizer->Add(pHistory, 0, (wxALL ^ wxTOP) | wxALIGN_CENTRE_HORIZONTAL, ID_BORDER);
  pSizer->Add(pButtons, 0,wxALL | wxALIGN_CENTRE_HORIZONTAL,ID_BORDER);
  pSizer->Add(m_pCheckBox,0,wxALIGN_LEFT | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
  CentreOnParent();
  SetEscapeId(wxID_NO);
  SetAffirmativeId(wxID_YES);

}
bool CDialogWarnHistory::DontShowAgain()
{
  return m_pCheckBox->GetValue();
}

void CDialogWarnHistory::OnCancel(wxCloseEvent &)
{
  if(!DontShowAgain())
  {
    EndModal(wxID_NO);
  }
}
void CDialogWarnHistory::OnCheck(wxCommandEvent &)
{
  if(m_pButtonNO == NULL)
  {
    m_pButtonNO = FindWindowById(wxID_NO, this);
  }
  if(m_pButtonNO != NULL)
  {
    m_pButtonNO->Enable(!DontShowAgain());
  }
}
void CDialogWarnHistory::OnButton(wxCommandEvent &e)
{
  wxWindow *pw = (wxWindow *)e.GetEventObject();
  int n = pw->GetId();
  EndModal(n);  
}

BEGIN_EVENT_TABLE(CDialogWarnHistory,wxDialog)
EVT_CHECKBOX(wxID_ANY,CDialogWarnHistory::OnCheck)
EVT_CLOSE(CDialogWarnHistory::OnCancel)
EVT_BUTTON(wxID_YES,CDialogWarnHistory::OnButton)
EVT_BUTTON(wxID_NO,CDialogWarnHistory::OnButton)
EVT_BUTTON(wxID_OK, CDialogWarnHistory::OnButton)
END_EVENT_TABLE()
