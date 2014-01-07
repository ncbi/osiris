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
*  FileName: CPageAuto.h
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#ifndef __C_PAGE_AUTO_H__
#define __C_PAGE_AUTO_H__

#include <wx/wizard.h>

class CPageParameters;
class CDialogExportSetup;
class CPageParameters;
class wxCheckBox;
class wxComboBox;
class wxTextCtrl;
class wxButton;
class CXSLExportFileType;

class CPageAuto : public wxWizardPageSimple
{
public:
  CPageAuto(
    CDialogExportSetup *parent,
    wxWizardPageSimple *prev,
    CXSLExportFileType *pExportEdit,
    bool bReadOnly = false);
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  void OnCheckActive(wxCommandEvent &);
  void OnButtonBrowse(wxCommandEvent &);
private:
  bool _TransferDataFromWindow();
  void _SetupFileExtList();
  void _SetupLocationList();
  void _SetupActive();
  wxTextCtrl *_CreateTextCtrl();
  wxComboBox *_CreateComboBox();
  wxWindow *_CreateLocationWindow();
  wxWindow *_CreateFileNameWindow(const wxArrayString &as);
  wxWindow *_CreateFileExtWindow();
  void _SetLocationValue(const wxString &s);
  void _SetFileNameValue(const wxString &s);
  void _SetFileExtValue(const wxString &s);

  static const wxChar * const ANALYSIS_FILE_NAME;
  static const wxChar * const DATE_FILE_NAME;
  CXSLExportFileType *m_pExportEdit;
  wxCheckBox *m_pCheckActive;
  wxCheckBox *m_pCheckAppendDate;
  wxComboBox *m_pComboLocation;
  wxComboBox *m_pComboFileName;
  wxComboBox *m_pComboFileExt;
  wxTextCtrl *m_pTextLocation;
  wxTextCtrl *m_pTextFileName;
  wxTextCtrl *m_pTextFileExt;
  wxButton *m_pButtonBrowse;
  bool m_bReadOnly;

  DECLARE_ABSTRACT_CLASS(CPageAuto)
  DECLARE_EVENT_TABLE()
};

#endif
