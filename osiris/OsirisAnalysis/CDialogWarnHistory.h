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
*  FileName: CDialogWarnHistory.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_WARN_HISTORY_H__
#define __C_DIALOG_WARN_HISTORY_H__

#include <wx/dialog.h>
#include "CMDIFrame.h"

class wxButton;
class wxCheckBox;
class CDialogWarnHistory : private wxDialog
{
public:
  static bool Continue(wxWindow *p, bool bAsk = true, int nWhichWindow = CMDIFrame::HISTORY_WARN_THIS);
private:
  CDialogWarnHistory(
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    bool bAsk = true,
    int nWhichWindow = CMDIFrame::HISTORY_WARN_THIS);
  virtual ~CDialogWarnHistory();
  bool DontShowAgain();
  wxCheckBox *m_pCheckBox;
  wxWindow *m_pButtonNO;

  void OnCancel(wxCloseEvent &);
  void OnCheck(wxCommandEvent &);
  void OnButton(wxCommandEvent &);
  DECLARE_EVENT_TABLE()
};


#endif
