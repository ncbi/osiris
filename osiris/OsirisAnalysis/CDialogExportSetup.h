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
*  FileName: CDialogExportSetup.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_EXPORT_SETUP_H__
#define __C_DIALOG_EXPORT_SETUP_H__

#include "mainApp.h"
#include <wx/wizard.h>
#include "CXSLExportFileType.h"

class CPageName;
class CPageParameters;
class CPageNoParameters;
class CPageAuto;

#define SET_IFNULL(pFocus,pWin) if(pFocus == NULL) { pFocus = pWin; }


class CDialogExportSetup : public wxWizard
{
public:
  CDialogExportSetup(
    wxWindow *parent, 
    CExportFiles *pFiles,
    CXSLExportFileType *pExport = NULL,
    bool bReadOnly = false);

  bool IsNameUsed(const wxString &s)
  {
    return (m_pFiles->Find(s) != NULL);
  }

  static wxString GetSearchDir(
    bool bDir,
    const wxString &s1,  // first try
    const wxString &s2 = wxEmptyString); // second try
  static wxString PromptSearchDir(
    wxWindow *parent, 
    const wxString &s1,
    const wxString &s2 = wxEmptyString);

  bool Run();
  virtual bool Show(bool show = true);

  void OnPageChanging(wxWizardEvent &e);
  void OnPageChanged(wxWizardEvent &e);
  void OnCancel(wxWizardEvent &e);
  void OnFinished(wxWizardEvent &e);
  void SetupParameterPage();

  static const wxChar * const LABEL_FILE_NAME_EXTS;
  static const wxChar * const LABEL_ALLOW_OVERRIDE;
  static const wxChar * const LABEL_DEFAULT_LOC_IN;
  static const wxChar * const LABEL_DEFAULT_LOC_OUT;
  static const wxChar * const OPTION_SAME_AS_ANALYSIS;
  static const wxChar * const OPTION_SAME_AS_OUTPUT;
  static const wxChar * const OPTION_PARENT_OF_ANALYSIS;
  static const wxChar * const OPTION_REMEMBER_LAST_LOCATION;

private:
  CXSLExportFileType m_ExportEdit;
  CXSLExportFileType *m_pExport;
  CExportFiles *m_pFiles;  //used to prevent duplicate names
  CPageName *m_pPageName;
  CPageParameters *m_pPageParameters;
  CPageNoParameters *m_pPageNoParameters;
  CPageAuto *m_pPageAuto;
  bool m_bReadOnly;

  DECLARE_ABSTRACT_CLASS(CDialogExportSetup)
  DECLARE_EVENT_TABLE()
};

#endif
