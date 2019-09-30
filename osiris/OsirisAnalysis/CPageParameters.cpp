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
*  FileName: CPageParameters.cpp
*  Author:   Douglas Hoffman
*
*    wxWizardPageSimple subclass used by CDialogExportSetup
*
*/

#include "CDialogExportSetup.h"
#include "CPageParameters.h"
#include "CPageName.h"
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>
#include <wx/dirdlg.h>
#include <wx/button.h>
#include "nwx/nwxRound.h"
#include "nwx/nwxString.h"
#include "nwx/CIncrementer.h"
#include "wxIDS.h"

IMPLEMENT_ABSTRACT_CLASS(CPageParameters,wxWizardPageSimple)

CPageParameters::~CPageParameters()
{
//  if(m_pDirInput != NULL)
//  {
//    m_pDirInput->Destroy();
//  }
  _ClearSizerType();
  if(m_pSizerTypeNumber != NULL)
  {
    delete m_pSizerTypeNumber;
  }
  if(m_pSizerInputFile != NULL)
  {
    delete m_pSizerInputFile;
  }
}

CPageParameters::CPageParameters(
    CDialogExportSetup *parent,
    CPageName *prev,
    CXSLExportFileType *pExportEdit,
    bool bReadOnly) :
  wxWizardPageSimple(parent,prev),
  m_pExportEdit(pExportEdit),
  m_pParamCurrent(NULL),
  m_pLabelNames(NULL),
  m_pListNames(NULL),
  m_pLabelDescription(NULL),
  m_pDescription(NULL),
  m_pLabelType(NULL),
  m_pType(NULL),
  m_pTypeReadOnly(NULL),
  m_pLabelChoices(NULL),
  m_pChoices(NULL),
  m_pLabelMin(NULL),
  m_pMin(NULL),
  m_pLabelMax(NULL),
  m_pMax(NULL),
  m_pLabelChecked(NULL),
  m_pCheckedValue(NULL),
  m_pLabelUnchecked(NULL),
  m_pUncheckedValue(NULL),
  m_pLabelFixed(NULL),
  m_pFixedValue(NULL),
  m_pLabelLocation(NULL),
  m_pLabelExt(NULL),
  m_pComboLocation(NULL),
  m_pTextFileExt(NULL),
  m_pCheckRequired(NULL),
  m_pCheckOverrideExtension(NULL),
  m_pButtonBrowse(NULL),
//  m_pDirInput(NULL),
  m_pSizerType(NULL),
  m_pSizerInputFile(NULL),
  m_pSizerTypeNumber(NULL),
  m_nParameterChange(0),
  m_bReadOnly(bReadOnly)
{
  prev->SetNext(this);
  _LayoutPage();
}




wxTextCtrl *CPageParameters::_CreateTextCtrl(
    wxWindow *parent,
    wxWindowID id,
    const wxString &sValue,
    long style)
{
  if(parent == NULL)
  {
    parent = this;
  }
  if(m_bReadOnly)
  {
    style |= wxTE_READONLY;
  }
  wxTextCtrl *pRtn = new wxTextCtrl(parent,id,sValue,
    wxDefaultPosition,
    wxDefaultSize,
    style);
  return pRtn;
}
void CPageParameters::_LayoutPage()
{
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerTypeName = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerLeft = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerRight = new wxBoxSizer(wxVERTICAL);
  m_pSizerType = new wxBoxSizer(wxVERTICAL);

  m_pLabelNames = new wxStaticText(this,wxID_ANY,
    "External parameters");
  m_pListNames = new wxListBox(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    0,NULL,
    wxLB_SINGLE | wxLB_HSCROLL | wxLB_NEEDED_SB | wxLB_SORT);

  m_pLabelDescription = new wxStaticText(
    this,wxID_ANY,_("Parameter description"));
  m_pDescription = _CreateTextCtrl(this,wxID_ANY);

  m_pLabelType = new wxStaticText(this,wxID_ANY,
    "Parameter type");
  wxWindow *pWinType = NULL;
  if(m_bReadOnly)
  {
    m_pTypeReadOnly = _CreateTextCtrl(this,wxID_ANY);
    pWinType = m_pTypeReadOnly;
  }
  else
  {
    wxArrayString asChoices;
    CXSLParam::GetChoiceArray(&asChoices);
    m_pType = new wxChoice(this,wxID_ANY,
      wxDefaultPosition, wxDefaultSize,
      asChoices);
    pWinType = m_pType;
  }


  pSizerLeft->Add(m_pLabelNames,0,wxALIGN_LEFT);
  pSizerLeft->Add(m_pListNames,1,wxEXPAND);
  
  pSizerRight->Add(m_pLabelDescription,0, wxALIGN_LEFT);
  pSizerRight->Add(m_pDescription, 0, wxEXPAND);

  pSizerTypeName->Add(m_pLabelType,0,
    wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
  pSizerTypeName->Add(pWinType,0,wxALIGN_CENTER_VERTICAL);
  pSizerTypeName->AddStretchSpacer(1);

  pSizerRight->Add(pSizerTypeName,0, wxALIGN_LEFT | wxTOP, ID_BORDER);
  pSizerRight->Add(m_pSizerType,1,wxEXPAND | wxTOP,ID_BORDER);
  
  pSizerAll->Add(pSizerLeft,1, wxRIGHT | wxEXPAND, ID_BORDER);
  pSizerAll->Add(pSizerRight,2,wxLEFT  | wxEXPAND, ID_BORDER);
  SetSizer(pSizerAll);
  Layout();
}
void CPageParameters::_LoadParameters()
{
  const CXSLmapParam &mapParam = m_pExportEdit->GetMapParam();
  size_t nSize = mapParam.Size();
  wxString sSelect;
  bool bFound = false;
  if(m_pListNames->GetCount())
  {
    sSelect = m_pListNames->GetStringSelection();
  }
  m_pListNames->Clear();
  if(nSize)
  {
    wxArrayString asParams;
    asParams.Alloc(nSize);
    for(CXSLmapParam::const_iterator itr = mapParam.begin();
      itr != mapParam.end();
      ++itr)
    {
      asParams.Add(itr->first);
      if(sSelect == itr->first)
      {
        bFound = true;
      }
    }
    m_pListNames->Append(asParams);
    if(!( bFound && m_pListNames->SetStringSelection(sSelect) ))
    {
      m_pListNames->SetSelection(0);
    }
  }
}
bool CPageParameters::_TransferParameterDataToWindow()
{
  bool bRtn = true;
  CXSLParam *pParam;
  if(!m_pListNames->GetCount())
  {
    // should NEVER HAPPEN
    wxASSERT_MSG(0,"No parameters found.");
    bRtn = false;
    Layout();
  }
  else if (
    (pParam = m_pExportEdit->FindParam(
          m_pListNames->GetStringSelection()))
     == NULL)
  {
    // should NEVER happen
    wxASSERT_MSG(0,"Cannot find param");
    bRtn = false;
    Layout();
  }
  else
  {
    const wxString &sType(pParam->GetType());
    bool bChangeType = 
      (m_pParamCurrent == NULL) ||
      (m_pParamCurrent->GetType() != sType);
    m_pParamCurrent = pParam;
    m_pDescription->SetValue(m_pParamCurrent->GetDescription());

    if(!bChangeType) 
    {}
    else if(m_pTypeReadOnly != NULL)
    {
      m_pTypeReadOnly->SetValue(sType);
    }
    else if(!m_pType->SetStringSelection(sType))
    {
      m_pType->Select(0);
      bChangeType = true;
    }
    _SetupType(bChangeType);
  }
  return bRtn;
}

bool CPageParameters::TransferDataToWindow()
{
  _LoadParameters();
  bool bRtn = _TransferParameterDataToWindow();
  return bRtn;
}

void CPageParameters::_SetupTypeEmpty(bool bChanged)
{
  if(bChanged)
  {
    m_pSizerType->AddStretchSpacer(1);
  }
}

void CPageParameters::_SetupTypeChoice(bool bChanged)
{
  if(bChanged)
  {
    if(m_pLabelChoices == NULL)
    {
      m_pLabelChoices = new wxStaticText(this,wxID_ANY,
        "Enter values to be displayed in pulldown menu");
      m_pChoices = _CreateTextCtrl(this,wxID_ANY,
        wxEmptyString,
        wxTE_MULTILINE | wxHSCROLL);
    }
    else
    {
      m_pLabelChoices->Show(true);
      m_pChoices->Show(true);
    }
    m_pSizerType->Add(m_pLabelChoices,0,wxALIGN_LEFT);
    m_pSizerType->Add(m_pChoices,1,wxEXPAND);
  }
  wxString sValue;
  const vector<wxString> &as = m_pParamCurrent->GetChoiceList();
  nwxString::Join(as,&sValue,"\n");
  m_pChoices->SetValue(sValue);
}

void CPageParameters::_SetNumber(
  wxTextCtrl *pCtrl, bool bHasNumber, double dValue, bool bInteger)
{
  if(!bHasNumber)
  {
    pCtrl->SetValue(wxEmptyString);
  }
  else if(bInteger)
  {
    int n = nwxRound::Round(dValue);
    pCtrl->SetValue(nwxString::FormatNumber(n));
  }
  else
  {
    pCtrl->SetValue(nwxString::FormatNumber(dValue));
  }
}

void CPageParameters::_SetupTypeNumber(bool bChanged, bool bInteger)
{
  if(bChanged)
  {
    if(m_pLabelMin == NULL)
    {
      m_pLabelMin = new wxStaticText(
        this,wxID_ANY,"Min.");
      m_pMin = _CreateTextCtrl(
        this,wxID_ANY);
      m_pLabelMax = new wxStaticText(
        this,wxID_ANY,"Max.");
      m_pMax = _CreateTextCtrl(
        this,wxID_ANY);
      m_pSizerTypeNumber = new wxFlexGridSizer(
          2,2,ID_BORDER,ID_BORDER);
      m_pSizerTypeNumber->SetFlexibleDirection(wxBOTH);
      m_pSizerTypeNumber->AddGrowableCol(1,1);
      m_pSizerTypeNumber->Add(m_pLabelMin,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
      m_pSizerTypeNumber->Add(m_pMin,1,wxALIGN_CENTRE_VERTICAL | wxEXPAND);
      m_pSizerTypeNumber->Add(m_pLabelMax,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
      m_pSizerTypeNumber->Add(m_pMax,1,wxALIGN_CENTRE_VERTICAL | wxEXPAND);
      m_pSizerType->Add(m_pSizerTypeNumber,0, wxEXPAND);
      m_pSizerType->AddStretchSpacer(1);
    }
    else
    {
      m_pLabelMin->Show(true);
      m_pMin->Show(true);
      m_pLabelMax->Show(true);
      m_pMax->Show(true);
    }
  }
  _SetNumber(m_pMin,m_pParamCurrent->GetHasMin(),m_pParamCurrent->GetMin(),bInteger);
  _SetNumber(m_pMax,m_pParamCurrent->GetHasMax(),m_pParamCurrent->GetMax(),bInteger);
}

void CPageParameters::_SetupTypeInputFile(bool bChanged)
{


  if(bChanged)
  {
    if(m_pLabelLocation == NULL)
    {
      wxArrayString asLoc;
      asLoc.Alloc(4);
      asLoc.Add(CDialogExportSetup::OPTION_SAME_AS_ANALYSIS);
      asLoc.Add(CDialogExportSetup::OPTION_PARENT_OF_ANALYSIS);
      asLoc.Add(CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION);
      asLoc.Add(CDialogExportSetup::OPTION_SAME_AS_OUTPUT);

      m_pLabelLocation = new wxStaticText(
        this, wxID_ANY, 
        CDialogExportSetup::LABEL_DEFAULT_LOC_IN);
      m_pComboLocation = new wxComboBox(
        this,wxID_ANY,wxEmptyString,
        wxDefaultPosition,
        wxDefaultSize,asLoc,
        wxCB_DROPDOWN);
      m_pButtonBrowse = new wxButton(
        this,IDbrowseInput,"Browse...",
        wxDefaultPosition, wxDefaultSize, 
        wxBU_EXACTFIT );
      m_pLabelExt = new wxStaticText(
        this,wxID_ANY, CDialogExportSetup::LABEL_FILE_NAME_EXTS);
      m_pTextFileExt = _CreateTextCtrl(
          this,wxID_ANY);
      m_pCheckOverrideExtension = new wxCheckBox(this,wxID_ANY,
        CDialogExportSetup::LABEL_ALLOW_OVERRIDE);
      m_pCheckRequired = new wxCheckBox(this,wxID_ANY,
        "A file name is required");
      m_pSizerInputFile = new wxBoxSizer(wxHORIZONTAL);
      m_pSizerInputFile->Add(m_pComboLocation,1,wxEXPAND);
      m_pSizerInputFile->Add(m_pButtonBrowse,
        0,wxLEFT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
    }
    else
    {
      m_pLabelLocation->Show(true);
      m_pComboLocation->Show(true);
      m_pButtonBrowse->Show(true);
      m_pLabelExt->Show(true);
      m_pTextFileExt->Show(true);
      m_pCheckOverrideExtension->Show(true);
      m_pCheckRequired->Show(true);
    }
    m_pSizerType->Add(m_pLabelLocation,0,wxALIGN_LEFT);
    m_pSizerType->Add(m_pSizerInputFile,0,wxEXPAND);
    m_pSizerType->Add(m_pLabelExt,0,wxALIGN_LEFT | wxTOP, ID_BORDER);
    m_pSizerType->Add(m_pTextFileExt,0,wxEXPAND);
    m_pSizerType->Add(m_pCheckOverrideExtension,0,wxALIGN_LEFT | wxTOP, ID_BORDER);
    m_pSizerType->Add(m_pCheckRequired,0,wxALIGN_LEFT | wxTOP | wxBOTTOM, ID_BORDER);
    m_pSizerType->AddStretchSpacer(1);
  }

  wxString sLoc = m_pParamCurrent->GetInFileDefaultDir();
  wxString sValue;
  bool bSelectLoc = true;
  nwxString::Trim(&sLoc);
  if(sLoc.IsEmpty())
  {
    bSelectLoc = false;
  }
  if(CXSLExportFileType::IsUseAnalysisFileName(sLoc))
  {
    sValue = CDialogExportSetup::OPTION_SAME_AS_ANALYSIS;
  }
  else if(CXSLExportFileType::IsUseAnalysisParent(sLoc))
  {
    sValue = CDialogExportSetup::OPTION_PARENT_OF_ANALYSIS;
  }
  else if(CXSLExportFileType::IsUseOutputLocation(sLoc))
  {
    sValue = CDialogExportSetup::OPTION_SAME_AS_OUTPUT;
  }
  else if(CXSLExportFileType::IsUseLastLocation(sLoc))
  {
    sValue = CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION;
  }
  else
  {
    sValue = sLoc;
    bSelectLoc = false;
  }

  if(bSelectLoc)
  {
    m_pComboLocation->SetStringSelection(sValue);
  }
  else
  {
    m_pComboLocation->SetValue(sValue);
  }

  m_pTextFileExt->SetValue(m_pParamCurrent->GetInFileTypeList());
  m_pCheckOverrideExtension->SetValue(
    m_pParamCurrent->GetInFileAllowOverride());
  m_pCheckRequired->SetValue(
    m_pParamCurrent->GetInFileRequired());
}

void CPageParameters::_SetupTypeFixed(bool bChanged)
{
  if (bChanged)
  {
    if (m_pLabelFixed == NULL)
    {
      m_pLabelFixed= new wxStaticText(
        this, wxID_ANY, "Parameter value");
      m_pFixedValue = _CreateTextCtrl(
        this, wxID_ANY);
    }
    else
    {
      m_pLabelFixed->Show(true);
      m_pFixedValue->Show(true);
    }
    m_pSizerType->Add(
      m_pLabelFixed, 0,
      wxALIGN_LEFT);
    m_pSizerType->Add(
      m_pFixedValue, 0,
      wxEXPAND);
  }
  m_pFixedValue->SetValue(m_pParamCurrent->GetFixedValue());
}
void CPageParameters::_SetupTypeCheckbox(bool bChanged)
{
  if(bChanged)
  {
    if(m_pLabelChecked == NULL)
    {
      m_pLabelChecked = new wxStaticText(
        this,wxID_ANY,"Parameter value if checked");
      m_pCheckedValue = _CreateTextCtrl(
        this,wxID_ANY);
      m_pLabelUnchecked = new wxStaticText(
        this,wxID_ANY,"Parameter value if not checked");
      m_pUncheckedValue = _CreateTextCtrl(
        this,wxID_ANY);
    }
    else
    {
      m_pLabelChecked->Show(true);
      m_pCheckedValue->Show(true);
      m_pLabelUnchecked->Show(true);
      m_pUncheckedValue->Show(true);
    }
    m_pSizerType->Add(
      m_pLabelChecked,0,
      wxALIGN_LEFT);
    m_pSizerType->Add(
      m_pCheckedValue, 0,
      wxEXPAND);
    m_pSizerType->Add(
      m_pLabelUnchecked, 0,
      wxALIGN_LEFT | wxTOP, ID_BORDER);
    m_pSizerType->Add(
      m_pUncheckedValue, 0,
      wxEXPAND);
    m_pSizerType->AddStretchSpacer(1);
  }
  m_pCheckedValue->SetValue(m_pParamCurrent->GetCheckedValue());
  m_pUncheckedValue->SetValue(m_pParamCurrent->GetUncheckedValue());
}


void CPageParameters::_HideList(wxWindow **pWin, size_t nLEN)
{
  while(nLEN)
  {
    if((*pWin) != NULL)
    {
      (*pWin)->Show(false);
    }
    ++pWin;
    --nLEN;
  }
};

void CPageParameters::_ClearSizerType()
{
  if(m_pSizerType != NULL)
  {
    while(m_pSizerType->GetChildren().GetCount())
    {
      m_pSizerType->Detach(0);
    }
  }
}

void CPageParameters::_HideAllType()
{
  wxWindow *pWin[] =
  {
    m_pLabelChoices,
    m_pChoices,
    m_pLabelMin,
    m_pMin,
    m_pLabelMax,
    m_pMax,
    m_pLabelChecked,
    m_pCheckedValue,
    m_pLabelUnchecked,
    m_pUncheckedValue,
    m_pLabelFixed,
    m_pFixedValue,
    m_pLabelLocation,
    m_pLabelExt,
    m_pComboLocation,
    m_pTextFileExt,
    m_pCheckRequired,
    m_pCheckOverrideExtension,
    m_pButtonBrowse
  };
  _ClearSizerType();
  _HideList(&pWin[0],sizeof(pWin) / sizeof(pWin[0]));
}


void CPageParameters::_SetupType(bool bChanged)
{
  if(bChanged)
  {
    _HideAllType();
  }
  int n = _GetType();
  switch(n)
  {
  case CXSLParam::INTEGER:
    _SetupTypeNumber(bChanged,true);
    break;
  case CXSLParam::DECIMAL:
    _SetupTypeNumber(bChanged,false);
    break;
  case CXSLParam::CHOICE:
  case CXSLParam::CHOICE_TEXT:
    _SetupTypeChoice(bChanged);
    break;
  case CXSLParam::CHECKBOX:
    _SetupTypeCheckbox(bChanged);
    break;
  case CXSLParam::FIXED:
    _SetupTypeFixed(bChanged);
    break;
  case CXSLParam::INPUT_FILE:
    _SetupTypeInputFile(bChanged);
    break;
  case CXSLParam::TEXT:
  case CXSLParam::IGNORE_PARAM:
  default:
    _SetupTypeEmpty(bChanged);
    break;
  }
  Layout();
}

void CPageParameters::OnBrowseFolder(wxCommandEvent &)
{
  wxString sLoc = m_pComboLocation->GetValue();
  if(!wxDir::Exists(sLoc))
  {
    sLoc.Empty();
  }
  wxDirDialog DirInput(this,
        "Default Input File Folder",sLoc,
        wxDD_DIR_MUST_EXIST | wxDD_DEFAULT_STYLE | wxRESIZE_BORDER);
  if(DirInput.ShowModal() == wxID_OK)
  {
    wxString s = DirInput.GetPath();
    m_pComboLocation->SetValue(s);
  }
}

void CPageParameters::OnTypeChange(wxCommandEvent &)
{
  _SetupType(true);
}
void CPageParameters::OnParameterChange(wxCommandEvent &)
{
  if(!m_nParameterChange) // should NEVER be > 0
    // because m_pListNames->SetStringSelection
    // should not send event
  {
    CIncrementer xxx(m_nParameterChange);
    bool bReset = false;
    if((m_pParamCurrent != NULL) && (!_TransferCurrentParam()))
    {
      bReset = true;
    }
    else 
    {
      bReset = !_TransferParameterDataToWindow();
    }
    if(bReset && (m_pParamCurrent != NULL))
    {
      m_pListNames->SetStringSelection(m_pParamCurrent->GetName());
      _TransferParameterDataToWindow();
    }
  }
}
int CPageParameters::_GetType()
{
  int n = -1;
  if(m_pType != NULL)
  {
    n = m_pType->GetSelection();
  }
  else if(m_pTypeReadOnly != NULL)
  {
    wxString sType = m_pTypeReadOnly->GetValue();
    n = CXSLParam::TypeFromString(sType);
  }
  return n;
}
bool CPageParameters::_TransferCurrentParam()
{
  wxString sError;
  wxWindow *pFocus = NULL;
  if((!m_bReadOnly) && (m_pParamCurrent != NULL))
  {
    wxString s;
    double dMin = 0.0;
    double dMax = 0.0;
    int n = _GetType();
    sError.Alloc(256);
    s = m_pDescription->GetValue();
    nwxString::Trim(&s);
    if(s.IsEmpty() && (n != CXSLParam::IGNORE_PARAM))
    {
      sError.Append(
        "A description of the parameter is required.\n");
      SET_IFNULL(pFocus,m_pDescription);
    }
    else
    {
      m_pParamCurrent->SetDescription(s);
      m_pParamCurrent->SetType((CXSLParam::CXSLParamType)n);
    }
    switch(n)
    {
    case CXSLParam::INTEGER:
      s = m_pMin->GetValue();
      nwxString::Trim(&s);
      if(s.IsEmpty())
      {
        m_pParamCurrent->SetNoMin();
      }
      else if(!nwxString::IsInteger(s))
      {
        m_pParamCurrent->SetNoMin();
        SET_IFNULL(pFocus,m_pMin);
        sError.Append(
          "Minimum value should be an integer or empty.\n");
      }
      else
      {
        dMin = atof(s.utf8_str());
        m_pParamCurrent->SetMin(dMin);
      }
      s = m_pMax->GetValue();
      nwxString::Trim(&s);
      if(s.IsEmpty())
      {
        m_pParamCurrent->SetNoMax();
      }
      else if(!nwxString::IsInteger(s))
      {
        m_pParamCurrent->SetNoMax();
        SET_IFNULL(pFocus,m_pMax);
        sError.Append(
          "Maximum value should be an integer or empty.\n");
      }
      else
      {
        dMax = atof(s.utf8_str());
        m_pParamCurrent->SetMax(dMax);
        if(m_pParamCurrent->GetHasMin() &&
          (dMax <= dMin))
        {
          SET_IFNULL(pFocus,m_pMin);
          sError.Append(
            "Maximum value should be greater than the minumum.\n");
        }
      }
      break;
    case CXSLParam::DECIMAL:
      s = m_pMin->GetValue();
      nwxString::Trim(&s);
      if(s.IsEmpty())
      {
        m_pParamCurrent->SetNoMin();
      }
      else if(!nwxString::IsNumber(s))
      {
        SET_IFNULL(pFocus,m_pMin);
        sError.Append(
          "Minimum value should be a number or empty.\n");
      }
      else
      {
        dMin = atof(s.utf8_str());
        m_pParamCurrent->SetMin(dMin);
      }
      s = m_pMax->GetValue();
      nwxString::Trim(&s);
      if(s.IsEmpty())
      {
        m_pParamCurrent->SetNoMax();
      }
      else if(!nwxString::IsNumber(s))
      {
        SET_IFNULL(pFocus,m_pMax);
        sError.Append(
          "Maximum value should be a number or empty.\n");
      }
      else
      {
        dMax = atof(s.utf8_str());
        m_pParamCurrent->SetMax(dMax);
        if(m_pParamCurrent->GetHasMax() &&
          (dMax <= dMin))
        {
          SET_IFNULL(pFocus,m_pMin);
          sError.Append(
            "Maximum value should be greater than the minumum.\n");
        }
      }
      break;
    case CXSLParam::CHOICE:
    case CXSLParam::CHOICE_TEXT:
      {
        s = m_pChoices->GetValue();
        s.Trim();
        s.Replace("\r\n","\n",true); // shouldn't happen
        s.Replace("\r","\n",true);
        vector<wxString> as;
        nwxString::Split(s.utf8_str(),&as,"\n");
        nwxString::Trim(&as,false);
        if(as.empty())
        {
          SET_IFNULL(pFocus,m_pChoices);
          sError.Append(
            "One or more choices should be entered.\n");
        }
        else
        {
          m_pParamCurrent->SetChoiceList(as);
        }
      }
      break;
    case CXSLParam::CHECKBOX:
      {
        wxString s2 = m_pUncheckedValue->GetValue();
        s = m_pCheckedValue->GetValue();
        nwxString::Trim(&s);
        nwxString::Trim(&s2);
        if(s == s2)
        {
          SET_IFNULL(pFocus,m_pCheckedValue);
          sError.Append(
            "The values for checked and "
              "unchecked should not match.\n");
        }
        else
        {
          m_pParamCurrent->SetCheckedValue(s);
          m_pParamCurrent->SetUncheckedValue(s2);
        }
      }
      break;
    case CXSLParam::FIXED:
      {
        s = m_pFixedValue->GetValue();
        nwxString::Trim(&s);
        m_pParamCurrent->SetFixedValue(s);
      }
      break;
    case CXSLParam::INPUT_FILE:
      {
        wxString s2 = m_pComboLocation->GetValue();
        nwxString::Trim(&s2);
        wxString sSet;
        if(s2.IsEmpty())
        {}
        else if(s2 == CDialogExportSetup::OPTION_SAME_AS_ANALYSIS)
        {
          sSet = CXSLExportFileType::USE_ANALYSIS_FILE;
        }
        else if(s2 == CDialogExportSetup::OPTION_PARENT_OF_ANALYSIS)
        {
          sSet = CXSLExportFileType::USE_ANALYSIS_PARENT;
        }
        else if(s2 == CDialogExportSetup::OPTION_SAME_AS_OUTPUT)
        {
          sSet = CXSLExportFileType::USE_OUTPUT_LOCATION;
        }
        else if(s2 == CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION)
        {
          sSet = CXSLExportFileType::USE_LAST_LOCATION;
        }
        else if(wxDir::Exists(s2))
        {
          sSet = s2;
        }
        else
        {
          sError.Append(
            "The default input file location does not exist.\n");
        }
        if(sSet.Len())
        {
          m_pParamCurrent->SetInFileDefaultDir(sSet);
        }
      }

      m_pParamCurrent->SetInFileTypeList(m_pTextFileExt->GetValue(),&sError);
      m_pParamCurrent->SetInFileRequired(m_pCheckRequired->GetValue());
      m_pParamCurrent->SetInFileAllowOverride(m_pCheckOverrideExtension->GetValue());
      break;
    case CXSLParam::TEXT:
    case CXSLParam::IGNORE_PARAM:
    default:
      break;
    }
  }
  if(!sError.IsEmpty())
  {
    mainApp::ShowError(sError,this);
    if(pFocus != NULL)
    {
      pFocus->SetFocus();
    }
  }
  return sError.IsEmpty();
}
bool CPageParameters::TransferDataFromWindow()
{
  bool bRtn = false;
  if(m_bReadOnly)
  {
    bRtn = true;
  }
  else if(_TransferCurrentParam())
  {
    const CXSLmapParam &mapParam = m_pExportEdit->GetMapParam();
    CXSLmapParam::const_iterator itr;
    CXSLParam *pParam;
    CXSLParam *pSetCurrent(NULL);
    wxString sError;
    wxString sSelect;
    for(itr = mapParam.begin(); itr != mapParam.end(); ++itr)
    {
      pParam = m_pExportEdit->FindParam(itr->first);
      if(pParam == NULL)
      {
        sError.Append("Cannot find parameter information for ");
        sError.Append(itr->first);
        sError.Append("\n");
      }
      else if(pParam->GetDescription().IsEmpty()
        && (pParam->GetTypeEnum() != CXSLParam::IGNORE_PARAM))
      {
        sError.Append("There is no description for parameter ");
        sError.Append(itr->first);
        sError.Append("\n");
        if(pSetCurrent == NULL)
        {
          pSetCurrent = pParam;
          sSelect = itr->first;
        }
      }
    }
    if(!sError.IsEmpty())
    {
      // bRtn = false; // already done
      mainApp::ShowError(sError,this);
      if(pSetCurrent != NULL)
      {
        m_pParamCurrent = pSetCurrent;
        m_pListNames->SetStringSelection(sSelect);
        TransferDataToWindow();
      }
    }
    else
    {
      bRtn = true;
    }
  }
  return bRtn;
}

BEGIN_EVENT_TABLE(CPageParameters,wxWizardPageSimple)
EVT_CHOICE(wxID_ANY,CPageParameters::OnTypeChange)
EVT_LISTBOX(wxID_ANY,CPageParameters::OnParameterChange)
EVT_BUTTON(IDbrowseInput,CPageParameters::OnBrowseFolder)
END_EVENT_TABLE()
