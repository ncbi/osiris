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
*  FileName: CPageParameters.h
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#ifndef __C_PAGE_PARAMETERS_H__
#define __C_PAGE_PARAMETERS_H__

#include <wx/wizard.h>
class wxStaticText;
class wxListBox;
class wxTextCtrl;
class wxChoice;
class CDialogExportSetup;
class CPageName;
class CXSLExportFileType;
class CXSLParam;
class wxBoxSizer;
class wxFlexGridSizer;
class wxComboBox;
class wxCheckBox;
class wxButton;

class CPageParameters : public wxWizardPageSimple
{
public:
  CPageParameters(
    CDialogExportSetup *parent,
    CPageName *prev,
    CXSLExportFileType *pExportEdit,
    bool bReadOnly = false);
  virtual ~CPageParameters();
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  void OnParameterChange(wxCommandEvent &);
  void OnTypeChange(wxCommandEvent &);
  void OnBrowseFolder(wxCommandEvent &);
private:
  CXSLExportFileType *m_pExportEdit;
  CXSLParam *m_pParamCurrent; // parameter selected in m_pListNames

//  wxStaticText *m_pLabelNoParams;
  wxStaticText *m_pLabelNames;
  wxListBox *m_pListNames;
  wxStaticText *m_pLabelDescription;
  wxTextCtrl *m_pDescription;

  wxStaticText *m_pLabelType;
  wxChoice *m_pType;
  wxTextCtrl *m_pTypeReadOnly;

  wxStaticText *m_pLabelChoices;
  wxTextCtrl *m_pChoices;

  wxStaticText *m_pLabelMin;
  wxTextCtrl *m_pMin;
  wxStaticText *m_pLabelMax;
  wxTextCtrl *m_pMax;

  wxStaticText *m_pLabelChecked;
  wxTextCtrl *m_pCheckedValue;
  wxStaticText *m_pLabelUnchecked;
  wxTextCtrl *m_pUncheckedValue;

  // input file name type

  wxStaticText *m_pLabelLocation;
  wxStaticText *m_pLabelExt;
  wxComboBox *m_pComboLocation;
  wxTextCtrl *m_pTextFileExt;
  wxCheckBox *m_pCheckRequired;
  wxCheckBox *m_pCheckOverrideExtension;
  wxButton *m_pButtonBrowse;

  wxBoxSizer *m_pSizerType;
  wxBoxSizer *m_pSizerInputFile;
  wxFlexGridSizer *m_pSizerTypeNumber;

  int m_nParameterChange; // prevent re-entry of OnParameterChange()

  bool m_bReadOnly;

  bool  _TransferParameterDataToWindow();
  void _LoadParameters();
  wxTextCtrl *_CreateTextCtrl(
    wxWindow *parent = NULL,
    wxWindowID id = wxID_ANY,
    const wxString &sValue = wxEmptyString,
    long style = 0);

  static void _HideList(wxWindow **ppWin, size_t nLEN);
  static void _SetNumber(
    wxTextCtrl *pCtrl, bool bHasNumber, 
    double dValue, bool bInteger);

  void _LayoutPage();
  int _GetType();
  void _HideAllType();
  void _ClearSizerType();
  void _SetupType(bool bChanged);
  void _SetupTypeChoice(bool bChanged);
  void _SetupTypeNumber(bool bChanged, bool bInteger);
  void _SetupTypeEmpty(bool bChanged);
  void _SetupTypeCheckbox(bool bChanged);
  void _SetupTypeInputFile(bool bChanged);
  bool _TransferCurrentParam();

  DECLARE_ABSTRACT_CLASS(CPageParameters)
  DECLARE_EVENT_TABLE()
};

#endif
