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
*  FileName: CPageName.h
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#ifndef __C_PAGE_NAME_H__
#define __C_PAGE_NAME_H__

#include <wx/wizard.h>
#include "nwx/nwxTextCtrl.h"
class wxCheckBox;
class wxComboBox;
class wxButton;
class CDialogExportSetup;
class CXSLExportFileType;

class CPageName : public wxWizardPageSimple
{
public:
  CPageName(
    CDialogExportSetup *parent, 
    CXSLExportFileType *pExportEdit,
    bool bReadOnly = false);
  virtual bool TransferDataFromWindow();
  void OnBrowseXSL(wxCommandEvent &);
  void OnBrowseLocation(wxCommandEvent &);
  virtual bool Show(bool show = true);
  static const wxChar * const LOCATION_ANALYSIS;
private:
  bool _ExtractParameters(const wxString &s);
  void _TransferToWindow();  // not overriding virtual function
  bool _TransferDataFromWindow();
  bool ValidateXSL(const wxString &s, wxString *psError);
  wxString m_sNameOnCreate;
  wxTextCtrl *m_pTextName;
  wxTextCtrl *m_pTextFileExt;
  wxCheckBox *m_pCheckFileExtOverride;
  wxComboBox *m_pComboLocation;
  wxTextCtrl *m_pTextLocation;
  nwxTextCtrl *m_pTextXSL;
  wxButton *m_pButtonBrowseXSL;
  CXSLExportFileType *m_pExportEdit;
  bool m_bReadOnly;

  DECLARE_ABSTRACT_CLASS(CPageName)
  DECLARE_EVENT_TABLE()
};


#endif
