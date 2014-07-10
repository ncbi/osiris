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
*  FileName: CPanelUserID.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_PANEL_USER_ID_H__
#define __C_PANEL_USER_ID_H__

#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include "COARreview.h"


class CTextUserID : public wxTextCtrl
{
public:
  CTextUserID(
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    bool bReadOnly = true,
    const wxString &value = "");
  virtual ~CTextUserID();
  //
  //  window to be enabled when the text field is not empty
  //  and disabled when this window is disabled or empty
  //
  void SetOKWindow(wxWindow *pWin)
  {
    m_pOK = pWin;
    UpdateStatus();
  }
  virtual bool Enable(bool enable = true);
  void OnTextChange(wxCommandEvent &e);
  virtual void SetValue(const wxString &value);
  virtual void ChangeValue(const wxString &value);
  void UpdateSize();
  void UpdateStatus();
private:
  wxWindow *m_pOK;
  DECLARE_EVENT_TABLE()
};

class CPanelUserID : public wxPanel
{
public:
  CPanelUserID(
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    wxWindowID idText = wxID_ANY,
    IAppendReview *pReview = NULL,
    int nFlag = 0,
    bool bUserReadOnly = false,
    const wxString &value = "");
  virtual ~CPanelUserID();
  CTextUserID *GetTextCtrl()
  {
    return m_pTextCtrl;
  }
  wxString GetValue();
  void SetValue(const wxString &s)
  {
    m_pTextCtrl->ChangeValue(s);
  }
  virtual bool Enable(bool bEnable = true);
  void OnButton(wxCommandEvent &);
  void SetReviewReceiver(IAppendReview *pRev)
  {
    m_pReview = pRev;
  }
  void UpdateSize()
  {
    m_pTextCtrl->UpdateSize();
  }
  bool AddReview(IAppendReview *pReview = NULL,bool bNoDup = true);
  bool IsUserIDValid(wxString *psError = NULL);
private:
  wxString m_sUserError;
  IAppendReview *m_pReview;
  wxStaticText *m_pLabel;
  CTextUserID *m_pTextCtrl;
  wxButton *m_pButtonOK;
  wxButton *m_pButtonCancel;
  bool m_bSendBtnEvents;
  bool m_bDisableOnReview;

  DECLARE_EVENT_TABLE()

};

#define UID_BTN_ACCEPT              1
#define UID_BTN_APPROVE             2
#define UID_BTN_OK                  4
#define UID_BTN_EDIT                8
#define UID_BTN_CANCEL              16
#define UID_SPACER_TEXT_CENTER      32
#define UID_SPACER_BTN_CENTER       64
#define UID_SPACER_BTN_RIGHT        128
#define UID_SEND_BTN_EVENTS         256
#define UID_DISABLE_ON_REVIEW       512

#endif
