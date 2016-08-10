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
*  FileName: IEditPanel.cpp
*  Author:   Douglas Hoffman
*
*  Interface class for any panel used for editing in order to determine state
*
*/

#ifndef __I_EDIT_PANEL_H__
#define __I_EDIT_PANEL_H__

/*
#include <wx/event.h>
#include <wx/button.h>
*/

class wxWindow;

class IEditPanel
{
public:
  virtual ~IEditPanel() {};
  /* virtual void OnChange(wxCommandEvent &e) = 0; */
  virtual operator wxWindow *() = 0;
  virtual bool NeedsApply() = 0;
  virtual bool NeedsAcceptance() = 0;
  virtual bool NeedsReview() = 0;
  virtual bool HasHistory() = 0;
  virtual void DoApply() = 0;
  virtual void DoReview() = 0;
  virtual void DoAccept() = 0;
  /*
  virtual wxButton *GetApplyButton() = 0;
  virtual wxButton *GetAcceptButton() = 0;
  virtual wxButton *GetReviewButton() = 0;
  static void UpdateButtonPanel(IEditPanel *p)
  {
    bool bModified = p->IsModified();
    _EnableButton(p->GetApplyButton(),bModified);
    _EnableButton(p->GetAcceptButton(),!bModified && p->NeedsAcceptance());
    _EnableButton(p->GetReviewButton(), p->NeedsReview());
  }
  */
protected:
  /* static void _EnableButton(wxButton *pButton, bool bEnable)
  { if(pButton != NULL) { pButton->Enable(bEnable); }
  }
  */
  IEditPanel() {};
};

#endif