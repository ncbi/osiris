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
*  FileName: CPanelUserID.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "nwx/nwxString.h"
#include <wx/dcclient.h>
#include <wx/utils.h>
#include "CPanelUserID.h"
#include "CParmOsiris.h"
#include "wxIDS.h"

//********************************************  CTextUserID

CTextUserID::CTextUserID(
  wxWindow *parent,
  wxWindowID id,
  bool bReadOnly,
  const wxString &value) :
    wxTextCtrl(parent,id,value),
    m_pOK(NULL)
{
  if(!value.IsEmpty())
  {}
  else if(bReadOnly)
  {
    ChangeValue(wxGetUserId());
  }
  else
  {
    CParmOsirisGlobal parm;
    const wxString &s(parm->GetCMFuserID());
    ChangeValue(s);
  }
  SetEditable(!bReadOnly);
//  UpdateSize();
  SetMaxLength(20);
}

CTextUserID::~CTextUserID() {}

bool CTextUserID::Enable(bool enable)
{
  bool bRtn = wxTextCtrl::Enable(enable);
  UpdateStatus();
  return bRtn;
}

void CTextUserID::OnTextChange(wxCommandEvent &e)
{
  UpdateStatus();
  e.Skip(true);
}
void CTextUserID::SetValue(const wxString &value)
{
  ChangeValue(value);
}
void CTextUserID::ChangeValue(const wxString &value)
{
  wxTextCtrl::ChangeValue(value);
  UpdateStatus();
}
void CTextUserID::UpdateSize()
{
  wxClientDC dc(this);
  dc.SetFont(GetFont());
  wxSize sz = dc.GetTextExtent("WWWWWWWWWWWWWWWWWWWWWW");
  wxSize szCurrent = GetMinSize();
  if(sz.GetWidth() != szCurrent.GetWidth())
  {
    sz.SetHeight(szCurrent.GetHeight());
    SetMinSize(sz);
  }
}

void CTextUserID::UpdateStatus()
{
  if(m_pOK != NULL)
  {
    bool bEnable = IsEnabled();
    if(bEnable)
    {
      wxString s(GetValue());
      bEnable = nwxString::ContainsAlphaNumeric(s);
    }
    m_pOK->Enable(bEnable);
  }
}

BEGIN_EVENT_TABLE(CTextUserID,wxTextCtrl)
EVT_TEXT(wxID_ANY,CTextUserID::OnTextChange)
END_EVENT_TABLE()

//********************************************  CPanelUserID

#define UID_BTN_OK_ALL   \
  (UID_BTN_ACCEPT | UID_BTN_APPROVE | UID_BTN_OK)

#define UID_SPACER_ALL   \
  (UID_SPACER_TEXT_CENTER | \
  UID_SPACER_BTN_CENTER | UID_SPACER_BTN_RIGHT)



CPanelUserID::CPanelUserID(
  wxWindow *parent,
  wxWindowID id, 
  wxWindowID idText, 
  IAppendReview *pReview,
  int nFlag,
  bool bUserReadOnly,
  const wxString &value) : 
    wxPanel(parent,id), 
    m_pReview(pReview),
    m_pButtonOK(NULL),
    m_pButtonCancel(NULL),
    m_bSendBtnEvents(!!(nFlag & UID_SEND_BTN_EVENTS)),
    m_bDisableOnReview(!!(nFlag & UID_DISABLE_ON_REVIEW))
{
  wxButton *pButtonEdit(NULL);
  const wxChar *psUserLabel = wxS("User ID: ");
  bool btn = false;
  if(nFlag & UID_BTN_OK_ALL)
  {
    const wxChar *psLabel = wxS("&OK");
    if(nFlag & UID_BTN_ACCEPT)
    {
      psLabel = wxS("&Accept");
      psUserLabel = wxS("Accepted by: ");
      m_sUserError = wxS("accepted");
    }
    else if(nFlag & UID_BTN_APPROVE)
    {
      psLabel = wxS("&Approve");
      psUserLabel = wxS("Reviewed by: ");
      m_sUserError = wxS("approved");
    }
    else
    {
      m_sUserError = "accepted or approved";
    }
    m_pButtonOK = new wxButton(this,wxID_OK,psLabel,
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    btn = true;
  }
  m_pLabel = new wxStaticText(this,wxID_ANY,psUserLabel);
  mainApp::SetBoldFont(m_pLabel);
  m_pTextCtrl = new CTextUserID(this,idText,bUserReadOnly,value);
  if(m_pButtonOK != NULL)
  {
    m_pTextCtrl->SetOKWindow(m_pButtonOK);
  }
  if(nFlag & UID_BTN_EDIT)
  {
    pButtonEdit = new wxButton(this,IDmenuEditCell,"Edit",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    btn = true;
  }
  if(nFlag & UID_BTN_CANCEL)
  {
    m_pButtonCancel = new wxButton(this,wxID_CANCEL,"Cancel",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    btn = true;
  }
  wxBoxSizer *pSizerAll(NULL);
  wxBoxSizer *pSizerText = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerTextCtrl = new wxBoxSizer(wxVERTICAL);

  // pSizerTextCtrl allows m_pTextCtrl to expand
  // horizontally without expanding vertically
  pSizerTextCtrl->AddStretchSpacer(1);
  pSizerTextCtrl->Add(m_pTextCtrl,0,wxEXPAND);
  pSizerTextCtrl->AddStretchSpacer(1);
  pSizerText->Add(
    m_pLabel,0,wxRIGHT | wxALIGN_CENTER_VERTICAL,ID_BORDER);
  pSizerText->Add(pSizerTextCtrl,1,wxEXPAND);

  if(btn)
  {
    wxBoxSizer *pSizerBtn = new wxBoxSizer(wxHORIZONTAL);
    int nSizerFlag = wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT;
    int nSizerFlag0 = wxALIGN_CENTER_VERTICAL | wxRIGHT;
    if(m_pButtonOK != NULL)
    {
      pSizerBtn->Add(m_pButtonOK,0,nSizerFlag,ID_BORDER);
      nSizerFlag = nSizerFlag0;
    }
    if(pButtonEdit != NULL)
    {
      pSizerBtn->Add(pButtonEdit,0,nSizerFlag,ID_BORDER);
      nSizerFlag = nSizerFlag0;
    }
    if(m_pButtonCancel != NULL)
    {
      pSizerBtn->Add(m_pButtonCancel,0,nSizerFlag,ID_BORDER);
    }
    pSizerAll = new wxBoxSizer(wxHORIZONTAL);
    if(!(nFlag & UID_SPACER_ALL))
    {
      pSizerAll->Add(pSizerText, 0, 
        wxRIGHT | wxALIGN_CENTER_VERTICAL, ID_BORDER);
      pSizerAll->Add(pSizerBtn, 0, wxALIGN_CENTER_VERTICAL);
    }
    else if(nFlag & UID_SPACER_TEXT_CENTER)
    {
      // text in the center
      pSizerAll->AddStretchSpacer(1);
      pSizerAll->Add(pSizerText,0,
        wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL,ID_BORDER);
      if(!(nFlag & UID_SPACER_BTN_CENTER))
      {
        // buttons on the right
        pSizerAll->Add(pSizerBtn,1,
          wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxEXPAND);
      }
      else
      {
        // text and button are in the center
        pSizerAll->Add(pSizerBtn,0,
          wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
        pSizerAll->AddStretchSpacer(1);
      }
    }
    else if(nFlag & UID_SPACER_BTN_CENTER)
    {
      // text on far left
      // buttons in center
      pSizerAll->Add(pSizerText,1,
        wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxEXPAND);
      pSizerAll->Add(pSizerBtn,0,
        wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT,
        ID_BORDER);
      pSizerAll->AddStretchSpacer(1);
    }
    else
    {
      // text on far left
      // button on far right
      pSizerAll->Add(pSizerText,0,
        wxALIGN_CENTER_VERTICAL | wxRIGHT | wxALIGN_LEFT);
      pSizerAll->AddStretchSpacer(1);
      pSizerAll->Add(pSizerBtn,0,
        wxALIGN_CENTER_VERTICAL | wxLEFT | wxALIGN_RIGHT,
        ID_BORDER);
    }
  }
  else
  {
    if(nFlag & UID_SPACER_TEXT_CENTER)
    {
      pSizerText->InsertStretchSpacer(0,1);
      pSizerText->AddStretchSpacer(1);
    }
    pSizerAll = pSizerText;
  }
  SetSizer(pSizerAll);
  Layout();
}

CPanelUserID::~CPanelUserID() {}

bool CPanelUserID::Enable(bool bEnable)
{
  bool bRtn = true;
  if((m_pButtonCancel != NULL) && (m_pButtonOK != NULL))
  {
    bRtn = m_pButtonOK->Enable(bEnable);
    m_pTextCtrl->Enable(bEnable);
  }
  else
  {
    bRtn = wxPanel::Enable(bEnable);
    if(bEnable)
    {
      m_pTextCtrl->Enable(bEnable);
    }
  }
  return bRtn;
}
wxString CPanelUserID::GetValue()
{
  wxString s = m_pTextCtrl->GetValue();
  nwxString::Trim(&s);
  return s;
}
bool CPanelUserID::IsUserIDValid(wxString *psError)
{
  bool bRtn = true;
  wxString s = GetValue();
  const wxChar *psErr = wxS("");
  if(s.IsEmpty())
  {
    psErr = wxS("Please specify a User ID");
    bRtn = false;
  }
  else if(!nwxString::ContainsAlphaNumeric(s))
  {
    psErr = wxS(
      "User ID must contain at least\n"
      "one alphanumeric character.");
    bRtn = false;
  }
  if((!bRtn) && (psError != NULL))
  {
    *psError = psErr;
  }
  return bRtn;
}
bool CPanelUserID::AddReview(IAppendReview *pReview, bool bNoDup)
{
  bool bRtn = true;
  if(pReview == NULL)
  {
    pReview = m_pReview;
  }
  if(pReview != NULL)
  {
    wxString s = GetValue();
    nwxString::Trim(&s);
    wxString sError;
    if(!IsUserIDValid(&sError))
    {}
    else if( (!pReview->AppendReview(s)) && bNoDup )
    {
      sError = s;
      sError.Append(" has already ");
      sError.Append(m_sUserError);
      sError.Append(" this data.");
    }
    else
    {
      CParmOsirisGlobal parm;
      parm->SetCMFuserID(s);
    }
    if(!sError.IsEmpty())
    {
      bRtn = false;
      mainApp::ShowError(sError,this);
    }
    else if(m_bDisableOnReview)
    {
      Enable(false);
    }
  }
  return bRtn;
}
void CPanelUserID::OnButton(wxCommandEvent &e)
{
  bool bSkip = m_bSendBtnEvents;
  if( (e.GetId() == wxID_OK) && (m_pReview != NULL) )
  {
    if(!AddReview())
    {
      bSkip = false;
    }
  }
  e.Skip(bSkip);
}

BEGIN_EVENT_TABLE(CPanelUserID,wxPanel)
EVT_BUTTON(wxID_ANY,CPanelUserID::OnButton)
END_EVENT_TABLE()
