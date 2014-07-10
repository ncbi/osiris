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
*  FileName: CPageAuto.cpp
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#include "CDialogExportSetup.h"
#include "CPageAuto.h"
#include "CPageParameters.h"
#include "CPageName.h"
#include "CParmOsiris.h"
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/arrstr.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/dir.h>
#include "wxIDS.h"
#include "nwx/nwxString.h"

IMPLEMENT_ABSTRACT_CLASS(CPageAuto,wxWizardPageSimple)

const wxChar * const CPageAuto::ANALYSIS_FILE_NAME
(wxS("[Same as analysis file]"));
const wxChar * const CPageAuto::DATE_FILE_NAME
(wxS("[Use file date and time]"));

wxTextCtrl *CPageAuto::_CreateTextCtrl()
{
  wxTextCtrl *pRtn = new wxTextCtrl(
    this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxDefaultSize,
    wxTE_READONLY);
  return pRtn;
}
wxComboBox *CPageAuto::_CreateComboBox()
{
  wxComboBox *pRtn = new wxComboBox(
    this,wxID_ANY,wxEmptyString,wxDefaultPosition, wxDefaultSize,
    0,NULL,wxCB_DROPDOWN);
  return pRtn;
}

wxWindow *CPageAuto::_CreateLocationWindow()
{
  wxWindow *pRtn;
  if(m_bReadOnly)
  {
    m_pTextLocation = _CreateTextCtrl();
    pRtn = wxDynamicCast(m_pTextLocation,wxWindow);
  }
  else
  {
    m_pComboLocation = _CreateComboBox();
    pRtn = wxDynamicCast(m_pComboLocation,wxWindow);
  }
  return pRtn;
}
wxWindow *CPageAuto::_CreateFileNameWindow(const wxArrayString &as)
{
  wxWindow *pRtn;
  if(m_bReadOnly)
  {
    m_pTextFileName = _CreateTextCtrl();
    pRtn = wxDynamicCast(m_pTextFileName,wxWindow);
  }
  else
  {
    m_pComboFileName = _CreateComboBox();
    m_pComboFileName->Append(as);
    pRtn = wxDynamicCast(m_pComboFileName,wxWindow);
  }
  return pRtn;
}
wxWindow *CPageAuto::_CreateFileExtWindow()
{
  wxWindow *pRtn;
  if(m_bReadOnly)
  {
    m_pTextFileExt = _CreateTextCtrl();
    pRtn = wxDynamicCast(m_pTextFileExt,wxWindow);
  }
  else
  {
    m_pComboFileExt = _CreateComboBox();
    pRtn = wxDynamicCast(m_pComboFileExt,wxWindow);
  }
  return pRtn;
}
void CPageAuto::_SetLocationValue(const wxString &s)
{
  if(m_bReadOnly)
  {
    m_pTextLocation->SetValue(s);
  }
  else
  {
    m_pComboLocation->SetValue(s);
  }
}
void CPageAuto::_SetFileNameValue(const wxString &s)
{
  if(m_bReadOnly)
  {
    m_pTextFileName->SetValue(s);
  }
  else
  {
    m_pComboFileName->SetValue(s);
  }
}

void CPageAuto::_SetFileExtValue(const wxString &s)
{
  if(m_bReadOnly)
  {
    m_pTextFileExt->SetValue(s);
  }
  else
  {
    m_pComboFileExt->SetValue(s);
  }
}



CPageAuto::CPageAuto(
  CDialogExportSetup *parent, 
  wxWizardPageSimple *prev, 
  CXSLExportFileType *pExportEdit,
  bool bReadOnly) :
    wxWizardPageSimple(parent,prev,NULL),
    m_pExportEdit(pExportEdit),
    m_pCheckActive(NULL),
    m_pCheckAppendDate(NULL),
    m_pComboLocation(NULL),
    m_pComboFileName(NULL),
    m_pComboFileExt(NULL),
    m_pTextLocation(NULL),
    m_pTextFileName(NULL),
    m_pTextFileExt(NULL),
    m_pButtonBrowse(NULL),
    m_bReadOnly(bReadOnly)
{

  // m_pCheckActive

  prev->SetNext(this);
  m_pCheckActive = new wxCheckBox(
    this,wxID_ANY,"Create export file after each analysis");

  // m_pComboLocation

  wxStaticText *pTextLocation = new wxStaticText(
    this,wxID_ANY,"Location");

  wxWindow *pWinLocation = _CreateLocationWindow();
  wxBoxSizer *pSizerLocation = new wxBoxSizer(wxHORIZONTAL);
  if(!m_bReadOnly)
  {
    m_pButtonBrowse = new wxButton(
      this,IDbrowseOutput,"Browse...",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    pSizerLocation->Add(pWinLocation,1,wxEXPAND | wxRIGHT , ID_BORDER);
    pSizerLocation->Add(m_pButtonBrowse,0,wxALIGN_CENTER_VERTICAL,0);
  }
  else
  {
    pSizerLocation->Add(pWinLocation,1,wxEXPAND,0);
  }
  // m_pComboFileName

  wxArrayString as;
  if(!m_bReadOnly)
  {
    as.Alloc(1);
    as.Add(ANALYSIS_FILE_NAME);
  }

  wxStaticText *pTextFileName = new wxStaticText(
    this,wxID_ANY,"File name");
  wxWindow *pWinFileName = _CreateFileNameWindow(as);
  m_pCheckAppendDate = new wxCheckBox(
    this,wxID_ANY,"Append date to file name");

  // m_pComboFileExt

  wxStaticText *pTextExt = new wxStaticText(
    this,wxID_ANY,"File extension");
  wxWindow *pWinFileExt = _CreateFileExtWindow();

  // layout

  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  wxFlexGridSizer *pGridSizer = new wxFlexGridSizer(3,2,ID_BORDER,ID_BORDER);
  wxBoxSizer *pSizerFileName = new wxBoxSizer(wxVERTICAL);
  pGridSizer->SetFlexibleDirection(wxBOTH);
  pGridSizer->AddGrowableCol(1,1);
  pGridSizer->Add(pTextLocation,0,
    wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGridSizer->Add(pSizerLocation,1,
    wxEXPAND | wxALIGN_CENTRE_VERTICAL | wxEXPAND);
  pGridSizer->Add(pTextFileName,0,
    wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pSizerFileName->Add(pWinFileName,0,wxALIGN_LEFT | wxEXPAND,0);
  pSizerFileName->Add(m_pCheckAppendDate,0,wxTOP | wxALIGN_LEFT,1);
  pGridSizer->Add(pSizerFileName,1,
    wxEXPAND | wxALIGN_CENTRE_VERTICAL | wxEXPAND);
  pGridSizer->Add(pTextExt,0,
    wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGridSizer->Add(pWinFileExt,1,
    wxEXPAND | wxALIGN_CENTRE_VERTICAL | wxEXPAND,0);
  pSizerAll->Add(m_pCheckActive,0, wxALIGN_LEFT | wxBOTTOM, ID_BORDER);
  pSizerAll->Add(pGridSizer, 0, wxEXPAND);
  pSizerAll->AddStretchSpacer(1);
  SetSizer(pSizerAll);
  Layout();
}
  



void CPageAuto::_SetupFileExtList()
{
  if(m_pComboFileExt != NULL)
  {
    const set<wxString> &ss = m_pExportEdit->GetFileExt();
    set<wxString>::const_iterator itr;
    wxArrayString as;
    as.Alloc(ss.size());
    for(itr = ss.begin(); itr != ss.end(); ++itr)
    {
      as.Add(*itr);
    }
    m_pComboFileExt->Clear();
    m_pComboFileExt->Append(as);
  }
}
void CPageAuto::_SetupLocationList()
{
  if(m_pComboLocation != NULL)
  {
    wxArrayString as;
    if(!(
      m_pExportEdit->IsDefaultLocationAnalysisFile() ||
      m_pExportEdit->IsDefaultLocationLast() ||
      m_pExportEdit->GetDefaultLocation().IsEmpty()
      ))
    {
      as.Alloc(2);
      as.Add(m_pExportEdit->GetDefaultLocation());
    }
    as.Add(CDialogExportSetup::OPTION_SAME_AS_ANALYSIS);
    m_pComboLocation->Clear();
    m_pComboLocation->Append(as);
  }
}

void CPageAuto::_SetupActive()
{
  bool bActive = m_pCheckActive->GetValue();
  m_pCheckActive->Enable(!m_bReadOnly);
  m_pCheckAppendDate->Enable(bActive);
  if(m_bReadOnly)
  {
    m_pTextLocation->Enable(bActive);
    m_pTextFileName->Enable(bActive);
    m_pTextFileExt->Enable(bActive);
  }
  else
  {
    m_pButtonBrowse->Enable(bActive);
    m_pComboLocation->Enable(bActive);
    m_pComboFileName->Enable(bActive);
    m_pComboFileExt->Enable(bActive);
  }
}

bool CPageAuto::TransferDataToWindow()
{
  const CXSLAutoCreate ac(m_pExportEdit->GetAutoCreate());
  m_pCheckActive->SetValue(ac.IsActive());
  m_pCheckAppendDate->SetValue(ac.GetAppendDateToFileName());

  _SetupLocationList();
  wxString s;
  if(ac.IsAnalysisLocation())
  {
    s = CDialogExportSetup::OPTION_SAME_AS_ANALYSIS;
  }
  else
  {
    s = ac.GetLocation();
  }
  _SetLocationValue(s);

  if(ac.IsAnalysisFileName())
  {
    s = ANALYSIS_FILE_NAME;
  }
  else
  {
    s = ac.GetFileName();
  }
  _SetFileNameValue(s);

  _SetupFileExtList();
  _SetFileExtValue(ac.GetFileExt());
  _SetupActive();
  return true;
}
bool CPageAuto::TransferDataFromWindow()
{
  bool bRtn = 
    m_bReadOnly 
    ? true
    : _TransferDataFromWindow();
  return bRtn;
}
bool CPageAuto::_TransferDataFromWindow()
{
  bool bRtn = true;
  bool bActive = m_pCheckActive->GetValue();
  CXSLAutoCreate *pAuto = m_pExportEdit->GetAutoCreatePtr();
  pAuto->SetActive(bActive);
  if(bActive)
  {
    wxString s;
    wxString sError;
    wxWindow *pFocus = NULL;

    pAuto->SetAppendDateToFileName(m_pCheckAppendDate->GetValue());
    s = m_pComboLocation->GetValue();
    nwxString::Trim(&s);
    if(s == CDialogExportSetup::OPTION_SAME_AS_ANALYSIS)
    {
      pAuto->SetAnalysisLocation();
    }
    else if(s.IsEmpty())
    {
      sError.Append("Location is not specified.\n");
      SET_IFNULL(pFocus,m_pComboLocation);
    }
    else if(!pAuto->SetLocation(s))
    {
      sError.Append("Location is invalid.\n");
      SET_IFNULL(pFocus,m_pComboLocation);
    }

    s = m_pComboFileName->GetValue();
    nwxString::Trim(&s);
    if(s.IsEmpty())
    {
      sError.Append("File name is not specified.\n");
      SET_IFNULL(pFocus,m_pComboFileName);
    }
    if(s == ANALYSIS_FILE_NAME)
    {
      pAuto->SetUseAnalysisFileName();
    }
    else if(!pAuto->SetFileName(s))
    {
      sError.Append("File name is invalid.\n");
      SET_IFNULL(pFocus,m_pComboFileName);
     }

    s = m_pComboFileExt->GetValue();
    nwxString::Trim(&s);
    if(!pAuto->SetFileExt(s))
    {
      sError.Append("File extension is invalid.\n");
      SET_IFNULL(pFocus,m_pComboFileExt);
    }
    if(sError.Len())
    {
      mainApp::ShowError(sError,this);
      pFocus->SetFocus();
      bRtn = false;
    }
  }
  return bRtn;
}

void CPageAuto::OnCheckActive(wxCommandEvent &e)
{
  if(e.GetEventObject() == (wxObject *) m_pCheckActive)
  {
    _SetupActive();
  }
}

void CPageAuto::OnButtonBrowse(wxCommandEvent &)
{

  wxString sPath;
  CParmOsirisGlobal parm;
  wxString s = m_pComboLocation->GetValue();
  const wxString &s2(parm->GetLastExportDirectory());
  nwxString::Trim(&s);
  if(s == CDialogExportSetup::OPTION_SAME_AS_ANALYSIS)
  {
    s.Empty();
  }
  sPath = CDialogExportSetup::PromptSearchDir(this,s,s2);
  if(!sPath.IsEmpty())
  {
    m_pComboLocation->SetValue(sPath);
    parm->SetLastExportDirectory(sPath);
  }
}

BEGIN_EVENT_TABLE(CPageAuto,wxWizardPageSimple)
EVT_CHECKBOX(wxID_ANY,CPageAuto::OnCheckActive)
EVT_BUTTON(IDbrowseOutput,CPageAuto::OnButtonBrowse)
END_EVENT_TABLE()
