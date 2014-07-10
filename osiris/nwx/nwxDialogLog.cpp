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

*  FileName: nwxDialogLog.cpp
*  Author:   Douglas Hoffman
*
*/
#include <nwx/nwxDialogLog.h>
#include <wx/sizer.h>
#include "nwx/nwxString.h"

nwxDialogLog::~nwxDialogLog() {;}

nwxDialogLog::nwxDialogLog(
  wxWindow *parent, 
  wxWindowID id, 
  const wxString &sTitle, 
  long style) : 
    wxDialog(
      parent,
      id,
      sTitle,
      wxDefaultPosition,
      wxDefaultSize,
      style)
{
  const int SIZER_BORDER = 
#ifdef __WXMAC__
    8
#else
    4
#endif
    ;
  wxSizer *pButtonSizer(NULL);
  m_pTextCtrl = new wxTextCtrl(
    this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxSize(600,400),
    wxTE_READONLY | wxTE_MULTILINE | wxTE_DONTWRAP);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxButton *pButtonClear = new wxButton(
    this,wxID_CLEAR,"Clear");
  m_pButtonOK = new wxButton(
    this,wxID_OK,"&Close");
  m_pButtonOK->SetDefault();
  pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  pButtonSizer->Add(
    pButtonClear,0,wxRIGHT | wxALIGN_CENTER_VERTICAL,SIZER_BORDER);
  pButtonSizer->Add(
    m_pButtonOK,0,wxALIGN_CENTER_VERTICAL);

  pSizer->Add(m_pTextCtrl,1,wxALL | wxEXPAND, SIZER_BORDER);

  pSizer->Add(pButtonSizer,0,wxALIGN_CENTRE | wxBOTTOM,SIZER_BORDER);
  SetSizer(pSizer);
  Layout();
  m_pLog = new wxLogTextCtrl(m_pTextCtrl);
  m_pLog->SetTimestamp(nwxString::TIME_FORMAT);
}

bool nwxDialogLog::Show(bool show)
{
  bool bRtn = wxDialog::Show(show);
  if(bRtn && show)
  {
    SetFocus();
    m_pButtonOK->SetFocus();
  }
  return bRtn;
}
void nwxDialogLog::OnClear(wxCommandEvent &)
{
  m_pTextCtrl->Clear();
}

BEGIN_EVENT_TABLE(nwxDialogLog,wxDialog)
EVT_BUTTON(wxID_CLEAR,  nwxDialogLog::OnClear)
END_EVENT_TABLE()
