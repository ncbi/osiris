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
*  FileName: CDialogExportFile.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_DIALOG_EXPORT_FILE_H__
#define __C_DIALOG_EXPORT_FILE_H__

#include <wx/dialog.h>
#include <wx/textctrl.h>
#include "nwx/nsstd.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "CXSLExportFileType.h"

class wxGridBagSizer;
class CXSLParam;
class CExportFiles;
class wxPanel;
class wxCheckBox;
class wxXml2Document;

class CDialogExportFile : public wxDialog
{
public:
  CDialogExportFile(
    wxWindow *parent,
    CXSLExportFileType *pExportType,
    const wxString &sInputFileName,
    const wxString &sOutputFileName);
  virtual ~CDialogExportFile();
  
  bool Run();

  static bool Run(
    wxWindow *parent,
    CXSLExportFileType *pExportType,
    const wxString &sInputFileName,
    const wxString &sOutputFileName,
    bool *pbCancel);
  virtual bool TransferDataFromWindow();
  bool IsOK()
  {
    return m_bOK;
  }
  bool IsCanceled()
  {
    return m_bCanceled;
  }

private:
  static void ShowError(wxWindow *parent, const wxString &sError);
  wxWindow *_FindWindow(const CXSLParam * p)
  {
    wxWindow *pRtn = NULL;
    if(!p->IsIgnoreType())
    {
      map<const CXSLParam *,wxWindow *>::iterator itr =
        m_mapParamWindow.find(p);
      if(itr != m_mapParamWindow.end())
      {
        pRtn = itr->second;
      }
      wxASSERT_MSG(pRtn != NULL,"Cannot find window for parameter");
    }
    return pRtn;
  }
  void _SetWindow(const CXSLParam *p, wxWindow *pWin)
  {
    m_mapParamWindow.insert(
      map<const CXSLParam *, wxWindow *>::value_type(
        p, pWin));
  }
  void _AddFileParam(const CXSLParam *p, wxWindow *pButton)
  {
    m_mapFileParam.insert(
      map<wxWindow *, const CXSLParam *>::value_type(
        (wxWindow *)pButton,p));
  }
  const CXSLParam *_FindFileParam(wxWindow *p)
  {

    map<wxWindow *,const CXSLParam *>::iterator itr =
      m_mapFileParam.find(p);
    const CXSLParam *pRtn = 
      (itr == m_mapFileParam.end())
      ? NULL
      : itr->second;
    wxASSERT_MSG(pRtn != NULL,"Cannot find param for browse button");
    return pRtn;
  }
  wxTextCtrl *_CreateTextCtrl(const wxString &sDefault = wxEmptyString)
  {
    wxSize sz(200,-1);
    wxTextCtrl *p = new wxTextCtrl(m_pPanel,wxID_ANY,sDefault,wxDefaultPosition,sz);
    return p;
  }
  void _AddParameter(const CXSLParam *pParam, wxGridBagSizer *);
    // add parameter to dialog window

  bool _SetupParameter(const CXSLParam *pParam);
    // puts the parameter value into m_mapParamValues
  void _BuildDefaultDirFile(
    const CXSLParam *pParam,
    const wxString &sCurrentValue,
    wxString *psDefaultDir,
    wxString *psDefaultFile);
  void _BuildWildcardString(
    const CXSLParam *pParam, wxString *psWildcard);
  void _UpdateInputFileLastLocation();
  void _OnBrowse(wxCommandEvent &);
  wxString _GetParmValue(const wxString &_sName) const
  {
    wxString s;
    map<wxString,wxString>::const_iterator itrV =
        m_mapParamValues.find(_sName);
    if(itrV != m_mapParamValues.end())
    {
      s = itrV->second;
    }
    return s;
  }

  map<wxString,wxString> m_mapParamValues;
    // passed to CXSLExportFileType::TransformToFile()

  map<wxWindow *, const CXSLParam *> m_mapFileParam;
  map<const CXSLParam *,wxWindow *> m_mapParamWindow;
    // used for finding data entered for a parameter

  wxString m_sInputFileName;
  wxString m_sOutputFileName;
  wxString m_sError;
  CXSLExportFileType *m_pExportType;
//  wxScrolledWindow *m_pPanel;
  wxPanel *m_pPanel;
  wxCheckBox *m_pCheckShowLocation;
  CExportFiles *m_pFiles;
  int m_nRow;
  bool m_bOK;
  bool m_bCanceled;
  DECLARE_ABSTRACT_CLASS(CDialogExportFile)
  DECLARE_EVENT_TABLE()
};


#endif
