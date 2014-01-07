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
*  FileName: CDialogExportXSL.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_EXPORT_XSL_H__
#define __C_DIALOG_EXPORT_XSL_H__

#include <wx/dialog.h>
#include <wx/listbox.h>
#include "nwx/nwxTimerReceiver.h"
#include "CParmOsiris.h"

class wxListBox;
class wxStaticText;
class wxBoxSizer;
class wxButton;
class CExportFiles;


class CDialogExportXSL : public wxDialog, public nwxTimerReceiver
{
public:
  CDialogExportXSL(wxWindow *parent);
  virtual ~CDialogExportXSL();
  virtual bool TransferDataToWindow();
  void OnNew(wxCommandEvent &);
  void OnDelete(wxCommandEvent &);
  void OnEdit(wxCommandEvent &);
  virtual void OnTimer(wxTimerEvent &);
  bool IsModified()
  {
    return m_bModified;
  }
private:
  void ShowError(bool bClose = false)
  {
    if(!m_sError.IsEmpty())
    {
      mainApp::ShowError(m_sError,GetParent());
      if(bClose )
      {
        m_bNoShow = true;
        if(IsShown())
        {
          EndModal(wxID_CANCEL);
        }
      }
    }
  }
  CExportFiles *GetGlobal();
  void _SaveParms()
  {
    // save some parameters before opening CDialogExportSetup
    // dialog/wizard
    CParmOsirisGlobal parm;
    m_sSaveXSL = parm->GetLastExportXSLsearch();
    m_sSaveExport = parm->GetLastExportDirectory();
  }
  void _RestoreParms()
  {
    // restore parameters if user cancels from CDialogExportSetup
    CParmOsirisGlobal parm;
    parm->SetLastExportXSLsearch(m_sSaveXSL);
    parm->SetLastExportDirectory(m_sSaveExport);
  }
  wxString m_sError;
  wxString m_sSaveXSL;
  wxString m_sSaveExport;
  bool ShowLockProblem(CExportFiles *pXSL, bool bPrompt = false);
  wxButton *m_pButtonRemove;
  wxButton *m_pButtonEdit;
  wxListBox *m_pListBox;
  wxStaticText *m_pLabelNone1;
  wxStaticText *m_pLabelNone2;
  wxStaticText *m_pLabelList;
  wxBoxSizer *m_pSizerList;
  int m_nLock;
  int m_nTimer;
  bool m_bFirstTransfer;
  bool m_bShowingList;
  bool m_bModified;
  bool m_bNoShow;

  DECLARE_EVENT_TABLE()
};


#endif
