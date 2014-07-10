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
*  FileName: CPanelApplyCancel.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_APPLY_CANCEL_H__
#define __C_PANEL_APPLY_CANCEL_H__

#include <wx/panel.h>
#include <wx/button.h>

class CPanelApplyCancel : public wxPanel
{
public:
  CPanelApplyCancel(
    wxWindow *parent,
    wxWindowID idApplyButton = wxID_OK);
  virtual ~CPanelApplyCancel();
  void EnableApply(bool bEnable = true)
  {
    m_pButtonApply->Enable(bEnable);
  }
private:
  wxButton *m_pButtonApply;
};


#ifdef __C_PANEL_APPLY_CANCEL_CPP__

#include <wx/sizer.h>
#include "wxIDS.h"

CPanelApplyCancel::CPanelApplyCancel(
  wxWindow *parent, wxWindowID idApplyButton) : wxPanel(parent)
{
  wxGridSizer *pButtonSizer = new wxGridSizer(1,2,0,ID_BORDER);
  m_pButtonApply = new wxButton(
    this,idApplyButton,"Apply",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  wxButton *pButtonCancel = new wxButton(
    this,wxID_CANCEL,"Cancel",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pButtonSizer->Add(m_pButtonApply,0,wxTOP | wxBOTTOM | wxEXPAND,ID_BORDER);
  pButtonSizer->Add(pButtonCancel,0, wxTOP | wxBOTTOM | wxEXPAND,ID_BORDER);
  EnableApply(false);
  SetSizer(pButtonSizer);
}

CPanelApplyCancel::~CPanelApplyCancel() {;}

#endif

#endif
