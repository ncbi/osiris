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
*  FileName: IPageEditSample.h
*  Author:   Douglas Hoffman
*
*  Interface class for any panel used for editing part of a sample
*
*/

#ifndef __I_EDIT_PANEL_H__
#define __I_EDIT_PANEL_H__

/*
#include <wx/event.h>
#include <wx/button.h>
*/

class wxWindow;

class IPageEditSample
{
public:
  virtual ~IPageEditSample() {};
  /* virtual void OnChange(wxCommandEvent &e) = 0; */
  virtual operator wxWindow *() = 0;
  virtual bool NeedsApply() = 0;
  virtual bool NeedsAcceptance() = 0;
  virtual bool NeedsReview() = 0;
  virtual bool HasHistory() = 0;
  virtual void DoApply() = 0;
  virtual bool DoReview() = 0;
  virtual bool DoAccept() = 0;
  virtual bool TransferDataToPage() = 0;
protected:
  /* static void _EnableButton(wxButton *pButton, bool bEnable)
  { if(pButton != NULL) { pButton->Enable(bEnable); }
  }
  */
  IPageEditSample() {};
};

#endif