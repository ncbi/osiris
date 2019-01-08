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
