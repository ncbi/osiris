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
*  FileName: CDialogMaxLadderLabel.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_MAX_LADDER_LABEL_H__
#define __C_DIALOG_MAX_LADDER_LABEL_H__

#include <wx/dialog.h>
#include <wx/textctrl.h>


class CDialogMaxLadderLabel : public wxDialog
{
public:
  CDialogMaxLadderLabel(wxWindow *parent);
  bool EditValue(); // return true if modified
  static bool EditValue(wxWindow *parent)
  {
    CDialogMaxLadderLabel x(parent);
    bool bRtn = x.EditValue();
    return bRtn;
  }
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
private:
  wxTextCtrl *m_pText;
  int m_n;
};

#endif
