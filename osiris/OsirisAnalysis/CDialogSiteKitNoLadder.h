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
*  FileName: CDialogSiteKitNoLadder.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_SITE_KIT_NO_LADDER_H__
#define __C_DIALOG_SITE_KIT_NO_LADDER_H__

#include <wx/dialog.h>
#include "CSiteKitNoLadder.h"
#include "nwx/PersistentSize.h"

class wxButton;
class wxRadioButton;
class wxTextCtrl;
class wxChoice;
class _CGridChannelInfo;

class CDialogSiteKitNoLadder : public wxDialog
{
public:
  CDialogSiteKitNoLadder(wxWindow *parent);
  virtual ~CDialogSiteKitNoLadder();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetData(const CSiteKitNoLadder &kitData)
  {
    m_KitLadder = kitData;
    TransferDataToWindow();
  }
  const CSiteKitNoLadder &GetData()
  {
    return m_KitLadder;
  }
  void OnRemove(wxCommandEvent &);
  void OnAdd(wxCommandEvent &);
  void OnRename(wxCommandEvent &);
  void OnOK(wxCommandEvent &);
  void OnCancel(wxCommandEvent &);
  void OnApply(wxCommandEvent &);
  void OnLock(wxCommandEvent &);
  void OnChangeKit(wxCommandEvent &);
  void OnChangeILS(wxCommandEvent &);
private:
  void _initEmpty();
  bool _setupILSlist();
  void _setup();
  bool _currentKitModified();
  wxChoice *m_pChoiceKit;
  wxChoice *m_pChoiceILSgroup;
  wxChoice *m_pChoiceILS;
  wxChoice *m_pChoiceChannelCount;
  wxButton *m_pButtonLock;
  _CGridChannelInfo *m_pGridChannels;
  CSiteKitNoLadder m_KitLadder;
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_EVENT_TABLE()
};

/*
Need events - 
  change kit
  new kit
  rename kit
  change ILS group
  change channel count
  lock, OK, Apply, Cancel
    -- do not lock if created from newer version of OSIRIS
*/
#endif
