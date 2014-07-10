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
*  FileName: CDialogExportFile.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "mainFrame.h"
#include "CDialogExportFile.h"
#include "CDirList.h"
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/panel.h>
#include "nwx/nwxBatch.h"
#include "nwx/nwxString.h"
#include "nwx/nwxFileUtil.h"

IMPLEMENT_ABSTRACT_CLASS(CDialogExportFile,wxDialog)

#define END_LINE(xx) if(!xx.IsEmpty()) { xx.Append("\n");}

CDialogExportFile::CDialogExportFile(
  wxWindow *parent, 
  CXSLExportFileType *pExportType, 
  const wxString &sInputFileName, 
  const wxString &sOutputFileName) :
    wxDialog(parent,wxID_ANY,wxEmptyString,
      wxDefaultPosition,
      wxDefaultSize,
      mainApp::DIALOG_STYLE),
    m_sInputFileName(sInputFileName),
    m_sOutputFileName(sOutputFileName),
    m_pExportType(pExportType),
    m_pFiles(CExportFiles::GetGlobal()),
    m_nRow(0),
    m_bOK(false),
    m_bCanceled(false)
{
  if(m_pFiles != NULL)
  {
    m_pFiles->BeginBatch();
    {
      wxString sTitle = "Export ";
      sTitle.Append(m_pExportType->GetName());
      SetTitle(sTitle);
    }
    wxGridBagSizer *pGridSizer = new wxGridBagSizer(ID_BORDER,ID_BORDER);
    pGridSizer->AddGrowableCol(1,1);
    m_pPanel = new wxPanel(this,wxID_ANY);

    const CXSLmapParam &mapParam = m_pExportType->GetMapParam();
    CXSLmapParam::const_iterator itr;
    for(itr = mapParam.begin(); itr != mapParam.end(); ++itr)
    {
      _AddParameter(itr->second,pGridSizer);
    }
    m_pPanel->SetSizer(pGridSizer);
    m_pCheckShowLocation = new wxCheckBox(
      this,wxID_ANY,"View file location");
    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    pSizer->Add(m_pPanel,0,wxALL | wxEXPAND,ID_BORDER);
    pSizer->AddStretchSpacer(1);
    pSizer->Add(m_pCheckShowLocation,0,wxALL | wxALIGN_LEFT, ID_BORDER);
    pSizer->Add(
      CreateButtonSizer(wxOK | wxCANCEL),0,
      (wxALL ^ wxTOP) | wxALIGN_CENTRE, ID_BORDER);

    SetMaxSize(mainFrame::Size90());
    SetSizer(pSizer);
    CentreOnParent();
    Layout();
    Fit();
    m_bOK = true;
  }
  else // m_pFiles == NULL
  {
    mainApp::ShowError("Cannot find export configuration.",parent);
  }
}

CDialogExportFile::~CDialogExportFile()
{
  if(m_pFiles != NULL)
  {
    m_pFiles->EndBatch();
  }
}

void CDialogExportFile::_AddParameter(
  const CXSLParam *pParam,
  wxGridBagSizer *pGridSizer)
{
  bool bChoiceReadonly = false;
  wxString sDefault;
  const wxChar *pDefault = m_pExportType->GetDefaultParamValue(pParam);
  if(pDefault != NULL)
  {
    sDefault = pDefault;
  }
  size_t nLen = sDefault.Len();
  wxChar cFirst;
  const wxChar *QUOT = wxS("'\"");
  if( (nLen >= 2) && 
      ((cFirst = sDefault.GetChar(0)) == sDefault.GetChar(nLen - 1)) &&
      ((cFirst == QUOT[0]) || (cFirst == QUOT[1]))
    )
  {
    // remove outer quotes
    if(nLen == 2)
    {
      sDefault.Empty();
    }
    else
    {
      sDefault = sDefault.Mid(1,nLen-2);
    }
  }
  wxWindow *pWindow = NULL;
  wxCheckBox *pCheck = NULL;
  wxButton *pButton = NULL;

  enum
  {
    TEXT_CTRL,
    COMBO_BOX,
    CHECK_BOX,
    FILE_NAME,
    NADA
  };
  int nWinType = TEXT_CTRL;

  CXSLParam::CXSLParamType nType = pParam->GetTypeEnum();
  switch(nType)
  {
  case CXSLParam::INTEGER:
  case CXSLParam::DECIMAL:
    if(sDefault.IsEmpty())
    {}
    else if(!(
      (nType == CXSLParam::INTEGER)
       ? nwxString::IsInteger(sDefault)
       : nwxString::IsNumber(sDefault,false)
       ))
    {
      sDefault.Empty();
    }
    else if(!pParam->CheckRange(atof(sDefault.utf8_str())))
    {
      sDefault.Empty();
    }
  case CXSLParam::TEXT:
    nWinType = TEXT_CTRL;
    break;
  case CXSLParam::INPUT_FILE:
    nWinType = FILE_NAME;
    break;

  case CXSLParam::CHOICE:
    bChoiceReadonly = true;
  case CXSLParam::CHOICE_TEXT:
    nWinType = COMBO_BOX;
    break;
  case CXSLParam::CHECKBOX:
    nWinType = CHECK_BOX;
    break;
  case CXSLParam::IGNORE_PARAM:
  default:
    nWinType = NADA;
    break;
  }
  switch(nWinType)
  {
  case TEXT_CTRL:
    {
      pWindow = _CreateTextCtrl(sDefault);
    }
    break;
  case COMBO_BOX:
    {
      wxArrayString as;
      if(pParam->BuildChoiceList(&as,sDefault))
      {} // parameter found, good!
      else if(!bChoiceReadonly)
      {} // doesn't matter if parameter found
      else if(!as.GetCount())
      {
        sDefault.Empty();
      } // we have an empty list, not good
      else
      {
        sDefault = as.Item(0);
        // set to first item
      }
      wxComboBox *pCombo = new wxComboBox(m_pPanel,wxID_ANY,sDefault,
        wxDefaultPosition, wxDefaultSize, as,
        bChoiceReadonly 
        ? wxCB_READONLY 
        : 0);
      pWindow = pCombo;
    }
    break;
  case CHECK_BOX:
    {
      pCheck = new wxCheckBox(
        m_pPanel,wxID_ANY,pParam->GetDescription());
      bool b = (pParam->GetCheckedValue() == sDefault);
      pCheck->SetValue(b);
      pWindow = pCheck;
    }
    break;
  case FILE_NAME:
    {
      pWindow = _CreateTextCtrl();
      pButton = new wxButton(
        m_pPanel,IDbrowseInput,"Browse...",
        wxDefaultPosition, wxDefaultSize, 
        wxBU_EXACTFIT);
      _AddFileParam(pParam,pButton);
    }
    break;
  default:
    break;
  }
  if(pWindow != NULL)
  {
    if(pButton != NULL)
    {
      wxGBPosition gb1(m_nRow,0);
      wxGBPosition gb2(m_nRow,1);
      wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
      wxStaticText *pLabel = new wxStaticText(m_pPanel,wxID_ANY,pParam->GetDescription());
      pSizer->Add(pWindow,1,wxEXPAND);
      pSizer->Add(pButton,0,wxLEFT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
      pGridSizer->Add(
        pLabel,gb1,wxDefaultSpan,wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
      pGridSizer->Add(
        pSizer,gb2,wxDefaultSpan,wxEXPAND | wxALIGN_CENTRE_VERTICAL);
    }
    else if(pCheck != NULL)
    {
      wxGBPosition pos(m_nRow,0);
      wxGBSpan span(1,2);
      pGridSizer->Add(pWindow,pos,span,wxALIGN_LEFT);
    }
    else
    {
      wxGBPosition gb1(m_nRow,0);
      wxGBPosition gb2(m_nRow,1);
      wxStaticText *pLabel = new wxStaticText(m_pPanel,wxID_ANY,pParam->GetDescription());
      pGridSizer->Add(
        pLabel,gb1,wxDefaultSpan,wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
      pGridSizer->Add(
        pWindow,gb2,wxDefaultSpan,wxEXPAND | wxALIGN_CENTRE_VERTICAL);
    }
    m_nRow++;
    _SetWindow(pParam,pWindow);
  } // if(pWindow != NULL)
}

bool CDialogExportFile::Run()
{
  bool bRtn = false;
  int n = ShowModal();
  if(n == wxID_OK || n == wxID_YES)
  {
    wxString sError;
    m_bCanceled = false;
    {
      // we don't was wxBusyCursor in class CXSLExportFileType 
      // because it doens't have other UI objects
      wxBusyCursor xxx;
      bRtn = m_pExportType->TransformToFile(
        m_sInputFileName,
        m_sOutputFileName,
        &m_mapParamValues
        );
    }
    if(bRtn)
    {
      sError = CXSLExportFileType::CheckOutputFile(m_sOutputFileName);
      bRtn = sError.IsEmpty();
    }
    else // (!bRtn)
    {
      sError = CXSLExportFileType::ERROR_MESSAGE;
    }
    if(bRtn && m_pCheckShowLocation->GetValue())
    {
      nwxFileUtil::ShowFileFolder(m_sOutputFileName);
    }
    if(!sError.IsEmpty())
    {
      wxXml2Object::SendError(sError);
    }
    if(bRtn)
    {
      _UpdateInputFileLastLocation();
    }
  }
  else
  {
    m_bCanceled = true;
  }
  return bRtn;
}
void CDialogExportFile::_UpdateInputFileLastLocation()
{
  // look for an input file parameter that needs to be saved to
  // CParmOsiris
  bool bDone = false;
  const CXSLParam *pParam;
  wxString s;
  for(map<const CXSLParam *,wxWindow *>::reverse_iterator itr = m_mapParamWindow.rbegin();
    (!bDone) && (itr != m_mapParamWindow.rend());
    ++itr)
  {
    pParam = itr->first;
    if(pParam->IsSaveLastLocation())
    {
      s = _GetParmValue(pParam->GetName());
      if(!s.IsEmpty())
      {
        wxFileName fn(s);
        wxString sPath = fn.GetPath();
        if(wxDir::Exists(sPath))
        {
          CParmOsirisGlobal parm;
          parm->SetLastXSLInputFileDir(sPath);
          bDone = true;
        }
      }
    }
  }
}
bool CDialogExportFile::TransferDataFromWindow()
{
  wxWindow *pFocus = NULL;
  const CXSLmapParam &mapParam = m_pExportType->GetMapParam();
  wxString sValue;
  wxString sError;
  wxString sError1;
  CXSLmapParam::const_iterator itr;
  CXSLParam *pParam;
  CXSLParam::CXSLParamType nType;
  wxCheckBox *pCheckBox;
  wxComboBox *pComboBox;
  wxTextCtrl *pTextCtrl;
  wxWindow *pWindow;
  bool bSkip = true;

  m_mapParamValues.clear();
  for(itr = mapParam.begin(); itr != mapParam.end(); ++itr)
  {
    sError1.Empty();
    pParam = itr->second;
    bSkip = true;
    pWindow = _FindWindow(pParam);
    if(pWindow == NULL)
    {}
    else if((pTextCtrl = wxDynamicCast(pWindow,wxTextCtrl)) != NULL)
    {
      sValue = pTextCtrl->GetValue();
      bSkip = false;
    }
    else if((pComboBox = wxDynamicCast(pWindow,wxComboBox)) != NULL)
    {
      sValue = pComboBox->GetValue();
      bSkip = false;
    }
    else if((pCheckBox = wxDynamicCast(pWindow,wxCheckBox)) != NULL)
    {
      sValue = 
        pCheckBox->GetValue()
        ? pParam->GetCheckedValue()
        : pParam->GetUncheckedValue();
      bSkip = false;
    }
    else
    {
      wxASSERT_MSG(0,"Invalid window type.");
    }
    if((!bSkip) && pParam->IsNumericType())
    {
      sError1 = pParam->GetDescription();
      sError1.Append(": ");
      size_t nLen = sError1.Len();
      nType = pParam->GetTypeEnum();
      nwxString::Trim(&sValue);
      if(sValue.IsEmpty())
      {
        bSkip = true;
      }
      else if(
        (nType == CXSLParam::INTEGER) &&
        !nwxString::IsInteger(sValue))
      {
        sError1 = "should have an integer value";
      }
      else if(!nwxString::IsNumber(sValue,false))
      {
        sError1 = "should have a numeric value";
      }
      else if(!pParam->CheckRange(atof(sValue.utf8_str())))
      {
        if(!pParam->GetHasMax())
        {
          sError1.Append("should be greater than ");
          sError1.Append(
            nwxString::FormatNumber(pParam->GetMin()));
        }
        else if(!pParam->GetHasMin())
        {
          sError1.Append("should be less than ");
          sError1.Append(
            nwxString::FormatNumber(pParam->GetMax()));
        }
        else
        {
          sError1.Append("should be between ");
          sError1.Append(nwxString::FormatNumber(pParam->GetMin()));
          sError1.Append(" and ");
          sError1.Append(nwxString::FormatNumber(pParam->GetMax()));
        }
      }
      if(sError1.Len() == nLen)
      {
        sError1.Empty();
      }
    }
    else if(pParam->IsInputFileType())
    {
      sError1.Empty();
      if(sValue.IsEmpty())
      {
        if(pParam->GetInFileRequired())
        {
          END_LINE(sError1);
          sError1.Append("Please specify an input file");
        }
      }
      else
      {
        wxFileName fn(sValue);
        if(!fn.IsFileReadable(sValue))
        {
          END_LINE(sError1);
          sError1.Append("Cannot read file: ");
          sError1.Append(sValue);
        }
        else if(!pParam->IsFileExtOK(fn.GetExt()))
        {
          END_LINE(sError1);
          wxString sType = pParam->GetInFileTypeList();
          if(sType.Contains(CXSLExportFileType::LIST_SEPARATOR))
          {
            sError1.Append(
              "File extension is not valid,\n"
              "please select one of the following:\n");
          }
          else
          {
            sError1.Append(
              "File extention is not valid, please use: ");
          }
          sError1.Append(sType);
        }
      }
    }
    if(!sError1.IsEmpty())
    {
      bSkip = true;
      if(pFocus == NULL)
      {
        pFocus = pWindow;
      }
      END_LINE(sError);
      sError.Append(sError1);
    }
    else if(!bSkip)
    {
      m_mapParamValues.insert(
        map<wxString,wxString>::value_type(
          pParam->GetName(),
          sValue));
    }
  }
  if(!sError.IsEmpty())
  {
    mainApp::ShowError(sError,this);
    if(pFocus == NULL)
    {
      pFocus->SetFocus();
      wxASSERT_MSG(0,"Error message with no focus window");
    }
  }
  return sError.IsEmpty();
}

bool CDialogExportFile::Run(
  wxWindow *parent,
  CXSLExportFileType *pExportType,
  const wxString &sInputFileName,
  const wxString &sOutputFileName,
  bool *pbCancel)
{
  bool bRtn = false;
  if(pExportType->GetUserParamCount())
  {
    CDialogExportFile x(
      parent,pExportType,sInputFileName,sOutputFileName);
    bRtn = x.Run();
    *pbCancel = x.IsCanceled();
  }
  else
  {
    wxString sError;
    {
      // we don't was wxBusyCursor in class CXSLExportFileType 
      // because it doens't have other UI objects
      wxBusyCursor xxx;
      bRtn = pExportType->TransformToFile(
        sInputFileName, sOutputFileName,NULL);
    }
    if(bRtn)
    {
      sError = CXSLExportFileType::CheckOutputFile(sOutputFileName);
      bRtn = sError.IsEmpty();
    }
    else
    {
      sError = CXSLExportFileType::ERROR_MESSAGE;
    }
    if(!sError.IsEmpty())
    {
      wxXslObject::SendError(sError);
    }
  }
  return bRtn;
}

void CDialogExportFile::_BuildDefaultDirFile(
  const CXSLParam *pParam, 
  const wxString &_sCurrentValue,
  wxString *psDefaultDir,
  wxString *psDefaultFile)
{
  wxString sCurrentValue(_sCurrentValue);
  nwxString::Trim(&sCurrentValue);
  psDefaultDir->Empty();
  psDefaultFile->Empty();

  if(sCurrentValue.IsEmpty())
  {}
  else if(wxDir::Exists(sCurrentValue))
  {
    *psDefaultDir = sCurrentValue;
  }
  else
  {
    wxFileName fn(sCurrentValue);
    wxString s = fn.GetPath();
    wxString sExt = fn.GetExt();
    if(wxDir::Exists(s))
    {
      *psDefaultDir = s;
      if(pParam->IsFileExtOK(sExt))
      {
        *psDefaultFile = fn.GetFullName();
      }
    }
  }
  if(psDefaultDir->IsEmpty())
  {
    const wxString &sLoc(pParam->GetInFileDefaultDir());
    if(sLoc.IsEmpty())
    {}
    if(sLoc == CXSLExportFileType::USE_ANALYSIS_FILE)
    {
      wxFileName fnn(m_sInputFileName);
      *psDefaultDir = fnn.GetPath();
    }
    else if(sLoc == CXSLExportFileType::USE_ANALYSIS_PARENT)
    {
      wxFileName fnn(m_sInputFileName);
      wxString sDir = fnn.GetPath();
      wxFileName fnDir(sDir);
      wxString sDirName = fnDir.GetFullName();
      if(CDirList::IsTimeStamp(sDirName))
      {
        nwxFileUtil::UpDir(&sDir); // twice for timestamp subdir
      }
      nwxFileUtil::UpDir(&sDir);
      *psDefaultDir = sDir;
    }
    else if(sLoc == CXSLExportFileType::USE_OUTPUT_LOCATION)
    {
      wxFileName fnn(m_sOutputFileName);
      *psDefaultDir = fnn.GetPath();
    }
    else if(sLoc == CXSLExportFileType::USE_LAST_LOCATION)
    {
      CParmOsirisGlobal parm;
      wxString s = parm->GetLastXSLInputFileDir();
      if(s.IsEmpty())
      {}
      else if(wxDir::Exists(s))
      {
        *psDefaultDir = s;
      }
      else
      {
        nwxFileUtil::UpDir(&s);
        if(wxDir::Exists(s))
        {
          *psDefaultDir = s;
        }
      }
    }
    else if(wxDir::Exists(sLoc))
    {
      *psDefaultDir = sLoc;
    }
  }
}
void CDialogExportFile::_BuildWildcardString(
  const CXSLParam *pParam, wxString *ps)
{
#define ALL_FILES "All Files (*.*)|*.*"

  wxString &sType(*ps);
  const vector<wxString> &vs(pParam->GetInFileTypes());
  bool bOverride = pParam->GetInFileAllowOverride();
  sType.Empty();
  if(vs.size())
  {
    wxString sExt;
    size_t n = (vs.size() << 4) + 32;
    bool bFirst = true;
    sType.Alloc(n);
    sExt.Alloc(n);

    for (vector<wxString>::const_iterator itr = vs.begin();
      itr != vs.end();
      ++itr)
    {
      if(!bFirst)
      {
        sExt.Append(";");
      }
      else
      {
        bFirst = false;
      }
      sExt.Append("*.");
      sExt.Append(*itr);
    }
    sType.Append("(");
    sType.Append(sExt);
    sType.Append(")");
    sType.Append("|");
    sType.Append(sExt);
    if(bOverride)
    {
      sType.Append("|" ALL_FILES);
    }
  }
  else
  {
    sType.Append(ALL_FILES);
  }
}

void CDialogExportFile::_OnBrowse(wxCommandEvent &e)
{
  wxObject *pWindow = e.GetEventObject();
  wxWindow *pWin = NULL;
  wxTextCtrl *pTextCtrl = NULL;
  const CXSLParam *pParam = NULL;
  bool bSkip = false;
  if((pWindow == NULL) || !wxDynamicCast(pWindow,wxButton))
  {
    bSkip = true;
  }
  else if((pParam = _FindFileParam((wxWindow *)pWindow)) == NULL)
  {
    bSkip = true;
  }
  else if(
    ((pWin = _FindWindow(pParam)) == NULL) ||
    ((pTextCtrl = wxDynamicCast(pWin,wxTextCtrl)) == NULL)
    )
  {
    bSkip = true;
  }
  else
  {
    wxString sDir;
    wxString sFile;
    wxString sWildCard;
    _BuildDefaultDirFile(
      pParam,
      pTextCtrl->GetValue(),
      &sDir,&sFile);
    _BuildWildcardString(pParam,&sWildCard);

    wxFileDialog dlg(
      this,
      pParam->GetDescription(),
      sDir,sFile,sWildCard,
      wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if(dlg.ShowModal() == wxID_OK)
    {
      wxString sFile = dlg.GetPath();
      pTextCtrl->SetValue(sFile);
    }
  }
}
/*
// commented out 3/6/11

void CDialogExportFile::ShowError(wxWindow *parent, const wxString &sError)
{
  wxString s(sError);
  s.Append("\n\nCheck the message log in the \n" 
               "\"Tools\" pulldown menu for details.");
  mainApp::ShowError(s,parent);
}
*/

BEGIN_EVENT_TABLE(CDialogExportFile,wxDialog)
EVT_BUTTON(IDbrowseInput,CDialogExportFile::_OnBrowse)
END_EVENT_TABLE()
