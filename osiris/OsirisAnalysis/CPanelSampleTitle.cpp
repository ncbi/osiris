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
*  FileName: CPanelSampleTitle.cpp
*  Author:   Douglas Hoffman
*
*/

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "COARfile.h"
#include "CPanelSampleTitle.h"
#include "nwx/nwxString.h"
#include "CPanelUserID.h" // for CTextUserID
#include "mainApp.h"
#include "wxIDS.h"

class CDialogUserID : public wxDialog
{
public:
  CDialogUserID(
    wxWindow *parent, const wxString &sUser);
  virtual ~CDialogUserID() {}
  wxString GetUserID()
  {
    wxString s = m_pUser->GetValue();
    nwxString::Trim(&s);
    return s;
  }
private:
  CTextUserID *m_pUser;
};

CDialogUserID::CDialogUserID(
    wxWindow *parent,
    const wxString &sUser) : wxDialog(parent, wxID_ANY, wxT("User ID"),
    wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE)
   
{
  wxStaticText *pText = new wxStaticText(this,wxID_ANY,wxT("User ID: "));
  m_pUser = new CTextUserID(this,wxID_ANY,false,sUser);
  m_pUser->SetMinSize(wxSize(250,-1));
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);
  pSizerTop->Add(pText,0,wxALL | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pSizerTop->Add(m_pUser,1, (wxALL ^ wxLEFT) | wxALIGN_CENTER_VERTICAL | wxEXPAND, ID_BORDER);
  pSizer->Add(pSizerTop,0,wxEXPAND,0);
  pSizer->Add(CreateButtonSizer(wxOK | wxCANCEL),0,
    wxALIGN_CENTRE_HORIZONTAL | wxLEFT | wxRIGHT | wxBOTTOM,ID_BORDER);
  wxWindow *pOK = FindWindow(wxID_OK);
  m_pUser->SetOKWindow(pOK);
  SetSizer(pSizer);
  pSizer->Layout();
  Fit();
  m_pUser->SetFocus();
  m_pUser->SelectAll();
}


CPanelSampleTitle::CPanelSampleTitle(
  wxWindow *parent,
  const COARsample *pSample,
  const wxString &sUserID,
  bool bReadOnly) : wxPanel(parent),
    m_sUser(sUserID),
    m_pwUser(NULL)
{
  wxString sRunStart = pSample->FormatRunStart();
  wxStaticText *pLabelSampleName =
    new wxStaticText(this,wxID_ANY,"Sample:");
  wxStaticText *pSampleName =
    new wxStaticText(this,wxID_ANY,pSample->GetSampleName());
  wxStaticText *pLabelFileName =
    new wxStaticText(this,wxID_ANY,"File:");
  wxStaticText *pFileName =
    new wxStaticText(this,wxID_ANY,pSample->GetName());

  wxFont fnBold = pLabelSampleName->GetFont();
  fnBold.SetWeight(wxFONTWEIGHT_BOLD);
  pLabelSampleName->SetFont(fnBold);
  pLabelFileName->SetFont(fnBold);

  wxFlexGridSizer *pSizerGrid = new wxFlexGridSizer(2,0,6);
  pSizerGrid->Add(pLabelSampleName,0,wxALIGN_BOTTOM);
  pSizerGrid->Add(pSampleName,0,wxALIGN_BOTTOM);
  pSizerGrid->Add(pLabelFileName,0,wxALIGN_BOTTOM);
  pSizerGrid->Add(pFileName,0,wxALIGN_BOTTOM);
  if(!sRunStart.IsEmpty())
  {
    wxStaticText *pLabelRunStart =
      new wxStaticText(this,wxID_ANY,"Run Start:");
    wxStaticText *pRunStart =
      new wxStaticText(this,wxID_ANY,sRunStart);
    pLabelRunStart->SetFont(fnBold);
    pSizerGrid->Add(pLabelRunStart,0,wxALIGN_BOTTOM);
    pSizerGrid->Add(pRunStart,0,wxALIGN_BOTTOM);
  }
  wxStaticText *pUserLabel = new wxStaticText(this,wxID_ANY,"User ID:");
  pUserLabel->SetFont(fnBold);
  wxWindow *pUser;
  if(bReadOnly)
  {
    pUser = new wxStaticText(this,wxID_ANY,m_sUser);
  }
  else
  {
    m_pwUser = new wxHyperlinkCtrl(this,wxID_ANY,m_sUser,wxEmptyString,
      wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxHL_ALIGN_LEFT);
    pUser = m_pwUser;
  }
  pSizerGrid->Add(pUserLabel,0,wxALIGN_BOTTOM);
  pSizerGrid->Add(pUser,0,wxALIGN_BOTTOM);
  wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
//  pSizer->AddStretchSpacer(1);
  pSizer->Add(pSizerGrid,0,wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL,ID_BORDER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
  pSizer->Layout();
}

void CPanelSampleTitle::OnClickUser(wxHyperlinkEvent &)
{
  CDialogUserID dlg(this,m_sUser);
  if(dlg.ShowModal() == wxID_OK)
  {
    m_sUser = dlg.GetUserID();
    m_pwUser->SetLabel(m_sUser);
    Layout();
    CParmOsiris *parm = CParmOsiris::GetGlobal();
    parm->SetCMFuserID(m_sUser);
  }
}

IMPLEMENT_ABSTRACT_CLASS(CPanelSampleTitle,wxPanel)

BEGIN_EVENT_TABLE(CPanelSampleTitle,wxPanel)
EVT_HYPERLINK(wxID_ANY,CPanelSampleTitle::OnClickUser)
END_EVENT_TABLE()

