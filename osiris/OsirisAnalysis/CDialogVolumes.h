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
*  FileName: CDialogVolumes.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_VOLUMES_H__
#define __C_DIALOG_VOLUMES_H__

#include "CPanelLabSettings.h"
#include "CVolumes.h"
#include <wx/dialog.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include "nwx/nwxTimerReceiver.h"
#include "nwx/PersistentSize.h"

class CDialogVolumes : public wxDialog, public nwxTimerReceiver
{
public:
  CDialogVolumes(
    wxWindow *parent, 
    const wxString &sSelect = wxEmptyString,
    bool bReadOnly = false);
  virtual ~CDialogVolumes();
  virtual bool TransferDataFromWindow();
  virtual void OnTimer(wxTimerEvent &e);
  const wxString &GetSelection()
  {
    return m_sSelection;
  }

  static int FindString(const wxString &s, const wxArrayString &as);
private:
  bool _UpdateVolumeNames();
  bool _SetupAll();
  bool _SetVolume(/*bool bFit = false*/);
  bool _ResetVolume(CVolume *);
  bool _CurrentVolumeModified();
  bool _Apply(bool bWarnUser = true);
  bool _SaveCurrent(bool bWarnUser = true);
  void _EnableRenameRemove(bool bEnable)
  {
    if(m_pButtonRemove != NULL)
    {
      m_pButtonRemove->Enable(bEnable);
    }
    if(m_pButtonRename != NULL)
    {
      m_pButtonRename->Enable(bEnable);
    }
  }
  void Restore()
  {
    CLabSettings *pLab(m_pVolumeCurrent->GetLabSettings());
    *pLab = m_labCopy;
  }
  bool _ChangeVolumeWarningOK();
  void OnOK(wxCommandEvent &e);
  void OnCancel(wxCommandEvent &e);
  void OnApply(wxCommandEvent &e);
  void OnLock(wxCommandEvent &e);
  void OnAdd(wxCommandEvent &e);
  void OnRemove(wxCommandEvent &e);
  void OnRename(wxCommandEvent &e);
  void OnChangeVolume(wxCommandEvent &e);
  void OnGridEditStart(wxGridEvent &);
  void OnGridEditEnd(wxGridEvent &);
  bool _IsGridEditorShown();

  CVolumes m_volumes;
  CLabSettings m_labCopy;
  wxString m_sSelection;
  CVolume *m_pVolumeCurrent;

  wxChoice *m_pChoice;
  CPanelLabSettings *m_pPanelLab;
  wxButton *m_pButtonAdd;
  wxButton *m_pButtonRemove;
  wxButton *m_pButtonRename;
  wxGrid *m_pGridObject;
  bool m_bGridEditorShown;
  bool m_bReadOnly;
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_EVENT_TABLE()
};


#endif
