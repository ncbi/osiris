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
*  FileName: CPageName.cpp
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#include "CDialogExportSetup.h"
#include "CPageName.h"
#include "CParmOsiris.h"
#include "extractCXSLExportFile.h"
#include "wxXsl/wxXslSheet.h"
#include <wx/mstream.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/filename.h>
#include <wx/button.h>
#include <wx/dir.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>

#include "nwx/nwxString.h"
#include "wxIDS.h"
#include "ConfigDir.h"

IMPLEMENT_ABSTRACT_CLASS(CPageName,wxWizardPageSimple)
#define MIN_WIDTH 500

CPageName::CPageName(
  CDialogExportSetup *parent, 
  CXSLExportFileType *pExportEdit,
  bool bReadOnly) :
    wxWizardPageSimple(parent),
    m_pComboLocation(NULL),
    m_pTextLocation(NULL),
    m_pExportEdit(pExportEdit),
    m_bReadOnly(bReadOnly)
{
  wxStaticText *pLabelName;
  wxStaticText *pLabelExt;
  wxStaticText *pLabelLocation;
  wxStaticText *pLabelXSL;
  wxButton *pButtonBrowse;
  int nStyleText = 
    m_bReadOnly ? wxTE_READONLY : 0;
  m_sNameOnCreate = m_pExportEdit->GetName();


  pLabelXSL = new wxStaticText(this,wxID_ANY,_T("XSL File"));
  m_pTextXSL = new nwxTextCtrl(
    this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxDefaultSize, nStyleText);

  wxBoxSizer *pSizerXSL = new wxBoxSizer(wxHORIZONTAL);
  if(!m_bReadOnly)
  {
    m_pButtonBrowseXSL = new wxButton(
      this,IDbrowseInput,_T("Browse..."),
      wxDefaultPosition, wxDefaultSize,wxBU_EXACTFIT);
    pSizerXSL->Add(m_pTextXSL,1,wxEXPAND | wxRIGHT,ID_BORDER);
    pSizerXSL->Add(m_pButtonBrowseXSL,0,wxALIGN_CENTER_VERTICAL);
  }
  else
  {
    m_pButtonBrowseXSL = NULL;
    pSizerXSL->Add(m_pTextXSL,1,wxEXPAND,0);
  }


  pLabelName = new wxStaticText(
    this,wxID_ANY,_T("Export file description"));
  m_pTextName = new wxTextCtrl(
    this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, wxDefaultSize,
    nStyleText);

  pLabelExt = new wxStaticText(
    this,wxID_ANY,
    CDialogExportSetup::LABEL_FILE_NAME_EXTS);
  m_pTextFileExt = new wxTextCtrl(
    this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, wxDefaultSize,
    nStyleText);
  m_pCheckFileExtOverride = new wxCheckBox(this,wxID_ANY,
    CDialogExportSetup::LABEL_ALLOW_OVERRIDE);

  pLabelLocation = new wxStaticText(
    this,wxID_ANY,CDialogExportSetup::LABEL_DEFAULT_LOC_OUT);
  int nStyleCombo = wxCB_DROPDOWN;
  wxBoxSizer *pSizerLoc = new wxBoxSizer(wxHORIZONTAL);
  if(m_bReadOnly)
  {
    m_pTextLocation = new wxTextCtrl(
      this,wxID_ANY,wxEmptyString);
    pSizerLoc->Add(m_pTextLocation,1,wxEXPAND,0);
  }
  else
  {
    wxArrayString as;
    as.Alloc(2);
//    if(m_bReadOnly)
//    {
//      as.Add(sLoc);
//      nStyleCombo |= wxCB_READONLY;
//    }
    as.Add(CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION);
    as.Add(CDialogExportSetup::OPTION_SAME_AS_ANALYSIS);
    m_pComboLocation = new wxComboBox(
      this,wxID_ANY,wxEmptyString,
      wxDefaultPosition, wxDefaultSize,
      as,nStyleCombo);

    pButtonBrowse = new wxButton(
      this,IDbrowseOutput,_T("Browse..."),
      wxDefaultPosition, wxDefaultSize,wxBU_EXACTFIT);
    pSizerLoc->Add(m_pComboLocation,1,wxEXPAND | wxRIGHT,ID_BORDER);
    pSizerLoc->Add(pButtonBrowse,0,wxALIGN_CENTER);
  }


  m_pCheckFileExtOverride->Enable(!m_bReadOnly);
  _TransferToWindow();
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);

  const int FLAG_LABEL = wxALIGN_LEFT | wxTOP;
  const int FLAG_WIDGET = wxEXPAND;
  pSizerAll->Add(pLabelXSL,0,FLAG_LABEL ^ wxTOP,ID_BORDER);
  pSizerAll->Add(pSizerXSL,0,FLAG_WIDGET,ID_BORDER);
  pSizerAll->Add(pLabelName,0,FLAG_LABEL, ID_BORDER);
  pSizerAll->Add(m_pTextName,0,FLAG_WIDGET, ID_BORDER);
  pSizerAll->Add(pLabelExt,0,FLAG_LABEL, ID_BORDER);
  pSizerAll->Add(m_pTextFileExt,0,FLAG_WIDGET, ID_BORDER);
  pSizerAll->Add(
    m_pCheckFileExtOverride,0,FLAG_LABEL, 1);
  pSizerAll->Add(pLabelLocation,0,FLAG_LABEL,ID_BORDER + 1);
  pSizerAll->Add(pSizerLoc,0,FLAG_WIDGET,ID_BORDER);
  pSizerAll->AddStretchSpacer(1);
  SetSizer(pSizerAll);
  pSizerAll->Layout();
}
void CPageName::_TransferToWindow()
{
  wxString sLoc;
  m_pTextName->SetValue(m_pExportEdit->GetKey());
  m_pTextFileExt->SetValue(m_pExportEdit->GetFileExtJoined());
  m_pCheckFileExtOverride->SetValue(m_pExportEdit->AllowExtOverride());
  if(m_pExportEdit->IsDefaultLocationLast())
  {
    sLoc = CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION;
  }
  else if(m_pExportEdit->IsDefaultLocationAnalysisFile())
  {
    sLoc = CDialogExportSetup::OPTION_SAME_AS_ANALYSIS;
  }
  else
  {
    sLoc = m_pExportEdit->GetDefaultLocation();
  }
  if(m_pComboLocation != NULL)
  {
    m_pComboLocation->SetValue(sLoc);
  }
  else
  {
    m_pTextLocation->SetValue(sLoc);
  }
  m_pTextXSL->SetValue(m_pExportEdit->GetXSLfile());
}
bool CPageName::ValidateXSL(const wxString &s, wxString *psError)
{
  bool bRtn = false;
  if(s.IsEmpty())
  {
    psError->Append(
      _T("XSL file is not specified.\n"));
  }
  else if(!wxFileName::IsFileReadable(s))
  {
    psError->Append(
      _T("XSL file does not exist or cannot be opened.\n"));
  }
  else if(!m_pExportEdit->SetXSLFile(s))
  {
    psError->Append(
      _T("XSL file is invalid due to a parsing error.\n"));
  }
  else
  {
    bRtn = true;
  }
  return bRtn;
}
bool CPageName::_ExtractParameters(const wxString &s)
{
  wxMemoryInputStream istr(
    (const char *)extractCXSLExportFile,
    sizeof(extractCXSLExportFile)/sizeof(extractCXSLExportFile[0]));
  wxXslSheet sheet(istr);
  bool bRtn = false;
#ifdef __WXDEBUG__
  wxString sError(_T("CPageName::_ExtractParameters(\""));
  sError.Append(s);
  sError.Append(_T("\")  "));
#endif
  if(sheet.IsOK())
  {
    wxXml2Document XSLin;
    XSLin.Load(s);
    if(XSLin.IsOk())
    {
      auto_ptr<wxXml2Document> apDoc(sheet.TransformToDOM(&XSLin));
      if(apDoc.get() != NULL)
      {
        CXSLExportFileType xp;
        if(xp.LoadFromNode(apDoc->GetRoot()))
        {
          m_pExportEdit->Update(xp);
          m_pExportEdit->SetXSLFile(s);
          bRtn = true;
        }
#ifdef __WXDEBUG__
        else
        {
          sError.Append(_T("Problem loading export info"));
        }
#endif
      }
#ifdef __WXDEBUG__
      else
      {
        sError.Append(_T("Problem extracting export info from xsl file"));
      }
#endif
    }
#ifdef __WXDEBUG__
    else
    {
      sError.Append(_T("Problem loading xsl file as document"));
    }
#endif
  }
#ifdef __WXDEBUG__
  else
  {
    sError.Append(_T("Problem with internal xsl"));
  }
  wxASSERT_MSG(bRtn,sError);
#endif
  return bRtn;
}
bool CPageName::TransferDataFromWindow()
{
  CDialogExportSetup *pWin = 
    wxDynamicCast(GetParent(),CDialogExportSetup);
  if(pWin)
  {
    pWin->SetupParameterPage();
  }
  return m_bReadOnly ? true : _TransferDataFromWindow();
}
bool CPageName::_TransferDataFromWindow()
{
  wxString sError;
  wxString s;
  wxWindow *pFirstError(NULL);
  sError.Alloc(256);

  //  name

  s = m_pTextName->GetValue();
  nwxString::Trim(&s);
  if(s.IsEmpty())
  {
    sError.Append(
      _T("No description for the export files.\n"));
    SET_IFNULL(pFirstError,m_pTextName);
  }
  else if(s == m_sNameOnCreate)
  {}
  else 
  {
    CDialogExportSetup *pParent =
      wxDynamicCast(GetParent(),CDialogExportSetup);
    if(pParent == NULL)
    {
      sError.Append(_T("Problem finding parent window. BUG.\n"));
      SET_IFNULL(pFirstError,m_pTextName);
      wxASSERT_MSG(0,
      _T("wxDynamicCast(GetParent(),CDialogExportSetup) failed"));
    }
    else if( pParent->IsNameUsed(s) )
    {
      sError.Append(_T("Export file description is already used.\n"));
      SET_IFNULL(pFirstError,m_pTextName);
    }
    else
    {
      m_pExportEdit->SetName(s);
    }
  }
  // file extension
  
  s = m_pTextFileExt->GetValue();
  set<wxString> ss;
#if 0
  /*
  const wxChar *KILL[] = 
  {
    ".",
    ",",
    ";",
    ":",
    NULL
  };
  const wxChar **pKILL = &KILL[0];
  for(pKILL = &KILL[0]; (*pKILL) != NULL; pKILL++)
  {
    s.Replace(*pKILL,_T(" "));
  }
  nwxString::Trim(&s);
  while(s.Replace(_T("  "),_T(" "))) {;}

  nwxString::Split(s.c_str(),&ss,_T(" "));
  */
#endif

  nwxString::Trim(&s);
  size_t nLen = s.Len();
  int nProblem = 0;
  size_t nCount = CXSLExportFileType::ParseFileExtList(s,&ss,&nProblem);
  if(nProblem || (nLen && !nCount))
  {
    sError.Append(CXSLExportFileType::FILE_TYPE_ERROR);
    SET_IFNULL(pFirstError,m_pTextName);
  }
  m_pExportEdit->SetFileExt(ss,false);
  m_pExportEdit->SetAllowExtOverride(
    m_pCheckFileExtOverride->GetValue());

  // location
  s = m_pComboLocation->GetValue();
  nwxString::Trim(&s);
  if(s.IsEmpty())
  {
    sError.Append(_T("No location for output files specified.\n"));
    SET_IFNULL(pFirstError,m_pTextName);
  }
  else if(s == CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION)
  {
    m_pExportEdit->SetDefaultLocationLast();
  }
  else if(s == CDialogExportSetup::OPTION_SAME_AS_ANALYSIS)
  {
    m_pExportEdit->SetDefaultLocationAnalysisFile();
  }
  else if(wxDir::Exists(s))
  {
    m_pExportEdit->SetDefaultLocation(s);
  }
  else
  {
    sError.Append(
      _T("Invalid directory name for location of output file.\n"));
    SET_IFNULL(pFirstError,m_pTextName);
  }

  // xsl file

  s = m_pTextXSL->GetValue();
  nwxString::Trim(&s);
  if(!ValidateXSL(s,&sError))
  {
    SET_IFNULL(pFirstError,m_pTextName);
  }

  if(!sError.IsEmpty())
  {
    mainApp::ShowError(sError,this);
    pFirstError->SetFocus();
  }
  return sError.IsEmpty();
}

void CPageName::OnBrowseXSL(wxCommandEvent &)
{
  CParmOsirisGlobal parm;
  wxString sCurrent = m_pTextXSL->GetValue();
  wxString sSearch = CDialogExportSetup::GetSearchDir(
    false, sCurrent, parm->GetLastExportXSLsearch());

  wxFileDialog dlg(
    this,_T("Choose an XSLT file"),
    sSearch,wxEmptyString,
    _T("XSLT files (*.xsl)|*.xsl|All Files (*.*)|*.*"),
    wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxRESIZE_BORDER);
  bool bDone = false;

  while(!bDone)
  {
    if(dlg.ShowModal() != wxID_OK)
    {
      bDone = true;
    }
    else
    {
      wxString s = dlg.GetPath();
      wxString sError;
      if(ValidateXSL(s,&sError))
      {
        wxFileName fn(s);
        parm->SetLastExportXSLsearch(fn.GetPath());
        bDone = true;
        if(_ExtractParameters(s))
        {
          m_pExportEdit->SetXSLFile(s);  // STOP HERE
          _TransferToWindow();
        }
        else
        {
          m_pTextXSL->SetValue(s);
        }
      }
      else
      {
        mainApp::ShowError(sError,this);
      }
    }
  }
}
bool CPageName::Show(bool show)
{
  bool bRtn = wxWizardPageSimple::Show(show);
  if(show && bRtn && m_pTextXSL->GetValue().IsEmpty() && (m_pButtonBrowseXSL != NULL))
  {
    // if no file, send event to open the file dialog
    wxCommandEvent e(wxEVT_COMMAND_BUTTON_CLICKED,IDbrowseInput);
    e.SetEventObject(m_pButtonBrowseXSL);
    AddPendingEvent(e);
  }
  return bRtn;
}
void CPageName::OnBrowseLocation(wxCommandEvent &)
{
  CParmOsirisGlobal parm;
  wxString s = m_pComboLocation->GetValue();
  const wxString &s2(parm->GetLastExportDirectory());
  nwxString::Trim(&s);
  if((s == CDialogExportSetup::OPTION_SAME_AS_ANALYSIS) || (s == CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION))
  {
    s.Empty();
  }
  wxString sPath = CDialogExportSetup::PromptSearchDir(
    this,s, s2);
  if(!sPath.IsEmpty())
  {
    parm->SetLastExportDirectory(sPath);
    m_pComboLocation->SetValue(sPath);
  }
}
BEGIN_EVENT_TABLE(CPageName,wxWizardPageSimple)
EVT_BUTTON(IDbrowseInput,CPageName::OnBrowseXSL)
EVT_BUTTON(IDbrowseOutput,CPageName::OnBrowseLocation)
END_EVENT_TABLE()

