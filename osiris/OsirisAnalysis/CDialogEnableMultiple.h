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
*  FileName: CDialogEnableMultiple.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_ENABLE_MULTIPLE_H__
#define __C_DIALOG_ENABLE_MULTIPLE_H__

#include <wx/dialog.h>

class COARsample;
class wxListBox;
class wxTextCtrl;
class wxComboBox;
class IAppendReview;
class CPanelUserID;
class CEnableDisableMultiple;

#include "nwx/nsstd.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"

class CDialogEnableMultiple : public wxDialog
{
public:
  CDialogEnableMultiple(
    const vector<COARsample *> *pvpSamples,
    bool bAllowUserNameOverride,
    wxWindow *parent);
  virtual ~CDialogEnableMultiple();

  const vector<COARsample *> *GetSelection();
  bool IsActionEnable();
  bool HasSamples()
  {
    return !(m_vpSampleEnabled.empty() &&
              m_vpSampleDisabled.empty());
  }
  virtual bool Validate();
private:
  int _GetSelectionCount();
  void _SetupList(bool bEnabled);
  void _SetupList()
  {
    _SetupList(IsActionEnable());
  }
  vector<int> m_vnEnabled;
  vector<int> m_vnDisabled;
  vector<COARsample *> m_vpSampleEnabled;
  vector<COARsample *> m_vpSampleDisabled;
  vector<COARsample *> m_vpSelection;
  CEnableDisableMultiple *m_pReview;
  CPanelUserID *m_pPanelUser;
  wxComboBox *m_pCombo;
  wxListBox *m_pListBox;
  static const wxChar *g_psEnable;
  static const wxChar *g_psDisable;
  int m_nListSize;

  void OnComboBox(wxCommandEvent &);
  void OnSelect(wxCommandEvent &);
  DECLARE_EVENT_TABLE()
};



#endif
