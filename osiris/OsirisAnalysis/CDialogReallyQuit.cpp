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
*  FileName: CDialogReallyQuit.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CDialogReallyQuit.h"
#include "CParmOsiris.h"
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"

#ifdef __WXMAC__
#define QUIT_TEXT "quit"
#else
#define QUIT_TEXT "exit"
#endif

CDialogReallyQuit::CDialogReallyQuit(wxWindow *parent) :
  wxDialog(parent,wxID_ANY,wxString(_T("OSIRIS")))
{
  wxString sText(_T("Do you want to " QUIT_TEXT " OSIRIS?"));
  wxStaticText *pText = new wxStaticText(this,wxID_ANY,sText);
  wxFont fn(pText->GetFont());
  fn.SetPointSize(fn.GetPointSize() + 2);
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  pText->SetFont(fn);
  wxSizer *pButtons = CreateButtonSizer(wxYES | wxNO | wxNO_DEFAULT); 
  m_pCheckBox = new wxCheckBox(this,wxID_ANY,_T(" Don't show this window again"));
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pText,0,wxALL | wxALIGN_CENTRE,ID_BORDER << 1);
  pSizer->Add(pButtons,0,(wxALL ^ wxTOP) | wxALIGN_CENTRE,ID_BORDER);
  pSizer->Add(m_pCheckBox,0,wxALIGN_LEFT | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
  CentreOnParent();
  SetEscapeId(wxID_NO);
  SetAffirmativeId(wxID_NO);
}

CDialogReallyQuit::~CDialogReallyQuit() {}

bool CDialogReallyQuit::DontShowAgain()
{
  return m_pCheckBox->GetValue();
}

bool CDialogReallyQuit::ReallyQuit(wxWindow *parent)
{
  CParmOsirisGlobal parm;
  bool bRtn = true;
  if(parm->CheckBeforeExit())
  {
    CDialogReallyQuit dlg(parent);
    int n = dlg.ShowModal();
    bRtn = (n == wxID_YES) || (n == wxID_OK);
    if(dlg.DontShowAgain())
    {
      parm->SetCheckBeforeExit(false);
    }
  }
  return bRtn;
}

void CDialogReallyQuit::OnButton(wxCommandEvent &e)
{
  wxWindow *pw = (wxWindow *)e.GetEventObject();
  int n = pw->GetId();
  EndModal(n);
}

BEGIN_EVENT_TABLE(CDialogReallyQuit,wxDialog)
EVT_BUTTON(wxID_NO,  CDialogReallyQuit::OnButton)
EVT_BUTTON(wxID_YES, CDialogReallyQuit::OnButton)
END_EVENT_TABLE()

