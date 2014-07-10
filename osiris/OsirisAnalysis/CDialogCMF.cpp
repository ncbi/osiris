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
*  FileName: CDialogCMF.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include <wx/utils.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>

#include "nwx/nwxFileUtil.h"
#include "nwx/nwxXmlCMF.h"
#include "nwx/nwxGrid.h"
#include "COARfile.h"
#include "CParmOsiris.h"
#include "CDialogCMF.h"
#include "CGridCMF.h"
#include "wxIDS.h"
#include "OsirisFileTypes.h"
#include "Platform.h"
CDialogCMF::~CDialogCMF()
{
  _Cleanup();
}

CDialogCMF::CDialogCMF(
  wxWindow *parent, 
  wxWindowID id, 
  COARfile *pFile,
  COARsampleSort *pSort) : 
    wxDialog(
        parent,
        id,
        "Export CMF File",
        wxDefaultPosition,
        wxDefaultSize,
        mainApp::DIALOG_STYLE | wxMAXIMIZE_BOX ),
    m_pCMF(NULL),
    m_pFile(pFile),
    m_pDialogFile(NULL),
    m_bIncomplete(false),
    m_bTransferred(false)
{
  wxSize szText(200,-1);
  wxStaticText *pLabelSourceLab = 
    new wxStaticText(this,wxID_ANY,"Source Lab: ");
  wxStaticText *pLabelDestLab =
    new wxStaticText(this,wxID_ANY,"Destination Lab: ");
  wxStaticText *pLabelUserID =
    new wxStaticText(this,wxID_ANY,"Submit User ID: ");
  wxStaticText *pLabelBatchID =
    new wxStaticText(this,wxID_ANY,"Batch ID: ");
  wxStaticText *pLabelType = 
    new wxStaticText(this,wxID_ANY,"Default Specimen Type: ");
  m_pTextSourceLab = 
    new wxTextCtrl(this,wxID_ANY,
      wxEmptyString,wxDefaultPosition,szText);
  m_pTextDestLab =
    new wxTextCtrl(this,wxID_ANY,
      wxEmptyString,wxDefaultPosition,szText);
  m_pTextUserID = 
    new wxTextCtrl(this,wxID_ANY,
      wxEmptyString,wxDefaultPosition,szText);
  m_pTextBatchID =
    new wxTextCtrl(this,wxID_ANY,
      wxEmptyString,wxDefaultPosition,szText);

  m_pTextSourceLab->SetMaxLength((unsigned long)nwxXmlCMF::LEN_SOURCE_LAB);
  m_pTextDestLab->SetMaxLength((unsigned long)nwxXmlCMF::LEN_DEST_LAB);
  m_pTextUserID->SetMaxLength((unsigned long)nwxXmlCMF::LEN_SUBMIT_BY);
  m_pTextBatchID->SetMaxLength((unsigned long)nwxXmlCMF::LEN_BATCH_ID);

  wxArrayString asChoices;
  CLabSpecimenCategory::AppendTypeArray(&asChoices,false);

  m_pComboType = 
    new wxComboBox(this,wxID_ANY,
      CLabNameStrings::DEFAULT_SPECIMEN_CATEGORY,
      wxDefaultPosition, szText,asChoices,wxCB_READONLY);

  wxStaticText *pLabelSpecimens = 
    new wxStaticText(this,wxID_ANY,"Specimens");
  m_pGrid = new CGridCMF(this,wxID_ANY,m_pFile,pSort);
      
  wxStaticText *pLabelFile =
    new wxStaticText(this,wxID_ANY,"File Name");
  m_pTextFileName = new wxTextCtrl(this,wxID_ANY);
  wxButton *pButtonBrowse = new wxButton(
    this,IDbrowseOutput, "Browse...",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pCheckViewLocation = new wxCheckBox(this,wxID_ANY,
    CParmOsiris::LABEL_VIEW_LOCATION);

  const int LABEL_ALIGN = 
    wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL;
  const int TEXT_ALIGN = wxEXPAND;

  wxFlexGridSizer *pPanelSizer = new wxFlexGridSizer(5,2,ID_BORDER,0);
  pPanelSizer->Add(pLabelSourceLab,0,LABEL_ALIGN);
  pPanelSizer->Add(m_pTextSourceLab,0,TEXT_ALIGN);
  pPanelSizer->Add(pLabelDestLab,0,LABEL_ALIGN);
  pPanelSizer->Add(m_pTextDestLab,0,TEXT_ALIGN);
  pPanelSizer->Add(pLabelUserID,0,LABEL_ALIGN);
  pPanelSizer->Add(m_pTextUserID,0,TEXT_ALIGN);
  pPanelSizer->Add(pLabelBatchID,0,LABEL_ALIGN);
  pPanelSizer->Add(m_pTextBatchID,0,TEXT_ALIGN);
  pPanelSizer->Add(pLabelType,0,LABEL_ALIGN);
  pPanelSizer->Add(m_pComboType,0,TEXT_ALIGN);


  wxBoxSizer *pSizerFileName = new wxBoxSizer(wxHORIZONTAL);
  pSizerFileName->Add(
    m_pTextFileName,1,wxEXPAND | wxALIGN_CENTRE_VERTICAL);
  pSizerFileName->Add(
    pButtonBrowse,0,wxLEFT | wxALIGN_CENTRE_VERTICAL, ID_BORDER);


  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pPanelSizer,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(pLabelSpecimens,0,wxLEFT | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(m_pGrid,1, (wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  pSizer->Add(pLabelFile,0,wxLEFT | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(pSizerFileName,0,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  pSizer->Add(m_pCheckViewLocation,0,(wxALL ^ wxTOP) | wxALIGN_LEFT, ID_BORDER);

  wxSizer *pButtonSizer = CreateButtonSizer(wxOK | wxCANCEL);
  wxButton *pButtonLater = 
    new wxButton(this,IDbuttonFinishLater,"Finish Later");
  wxSizerItem *pItem = pButtonSizer->GetItem(1);
  pButtonSizer->Add(
    pButtonLater,0,
    pItem->GetFlag(),pItem->GetBorder()
    );
  pSizer->Add(pButtonSizer,0,
    (wxALL ^ wxTOP) | wxALIGN_CENTRE ,ID_BORDER);
  SetSizer(pSizer);
  Layout();
}
void CDialogCMF::_ComputeSize()
{
  // wxSize szParent = GetParent()->GetClientSize();
  wxSize sz = GetSize();
  wxSize szGridV = nwxGrid::ComputeSize(m_pGrid);
  wxSize szGrid = m_pGrid->GetSize();
  wxRect r = wxGetClientDisplayRect();
  wxSize szScreen(r.GetWidth(),r.GetHeight());
  //wxSize szScreen = wxGetDisplaySize();
  int nMaxW = szScreen.GetWidth() - 40;
  int nMaxH = szScreen.GetHeight() - 100 - MENU_BAR_ALLOWANCE;
  sz -= szGrid;
  sz += szGridV;
  sz.SetHeight(sz.GetHeight() + 40);
  if(sz.GetWidth() > nMaxW)
  {
    sz.SetWidth(nMaxW);
  }
  if(sz.GetHeight() > nMaxH)
  {
    sz.SetHeight(nMaxH);
  }
  SetSize(sz);
#ifndef __WXMAC__
  CentreOnParent();
#endif
}
void CDialogCMF::_Cleanup()
{
  if(m_pDialogFile != NULL)
  {
    delete m_pDialogFile;
    m_pDialogFile = NULL;
  }
  _CleanupCMF();
}
void CDialogCMF::_CleanupCMF()
{
  if(m_pCMF != NULL)
  {
    delete m_pCMF;
    m_pCMF = NULL;
  }
}

bool CDialogCMF::TransferDataToWindow()
{
  if(m_bTransferred)
  {
    m_pGrid->UpdateEmptyLoci();
  }
  else
  {
    m_bTransferred = true;
    wxString s;
    wxString s2;
    CParmOsirisGlobal parm;
    bool bAllowUserOverride = m_pFile->GetReviewerAllowUserOverride();
    s = parm->GetCMFsourceLab();
    m_pTextSourceLab->ChangeValue(s);
    s = parm->GetCMFdestLab();
    m_pTextDestLab->ChangeValue(s);
    
    if(bAllowUserOverride)
    {
      s = parm->GetCMFuserID();
    }
    else
    {
      s = wxGetUserId();
      m_pTextUserID->SetEditable(false);
    }
    m_pTextUserID->ChangeValue(s);
    s2 = parm->GetCMFbatchFormat();
    wxDateTime dt;
    dt.SetToCurrent();
    s = dt.Format(s2);
    m_pTextBatchID->ChangeValue(s);
    s = parm->GetCMFdefaultSample();
    if(s.IsEmpty())
    {
      s = CGridCMF::DEFAULT;
    }
    m_pComboType->SetStringSelection(s);
    s = _GetDefaultFileName();
    m_pTextFileName->ChangeValue(s);
    m_pCheckViewLocation->SetValue(parm->GetCMFviewLocation());
    m_pGrid->TransferDataToGrid();
    _ComputeSize();
  }
  return true;
}
bool CDialogCMF::Show(bool show )
{
  bool bRtn = wxDialog::Show(show);
#if 0
  if(bRtn && show)
  {
    _ComputeSize();
  }
#endif
  return bRtn;
}
bool CDialogCMF::_SaveFile(const wxString &sFileName)
{
  wxBusyCursor xxx;
  bool bRtn = m_pCMF->SaveFile(sFileName);
  return bRtn;
}
bool CDialogCMF::TransferDataFromWindow()
{
  vector<wxString> vsErrors;
  wxString sError;
  wxString sSourceLab = m_pTextSourceLab->GetValue();
  wxString sDestLab = m_pTextDestLab->GetValue();
  wxString sUserID = m_pTextUserID->GetValue();
  wxString sBatchID = m_pTextBatchID->GetValue();
  wxString sDefaultType = m_pComboType->GetValue();
  wxString sFileName = m_pTextFileName->GetValue();
  bool bRtn = true;
  if(sSourceLab.IsEmpty())
  {
    vsErrors.push_back("Source lab is not specified");
  }
  if(sDestLab.IsEmpty())
  {
    vsErrors.push_back("Destination lab is not specified");
  }
  if(sUserID.IsEmpty())
  {
    vsErrors.push_back("Submit User ID is not specified");
  }
  if(sFileName.IsEmpty())
  {
    vsErrors.push_back("File name is not specified");
  }
  else
  {
    wxFileName fn(sFileName);
    if(!fn.DirExists())
    {
      vsErrors.push_back("File name is not valid, directory does not exist");
    }
  }
  if(vsErrors.empty())
  {
    COARsample2CMFSpecimen mapOARCMF;
    _CleanupCMF();
    m_pCMF = m_pFile->CreateCMF(&mapOARCMF);
    if(!m_pGrid->TransferDataFromGrid(sDefaultType,&mapOARCMF,&vsErrors))
    {
      bRtn = false;
    }
    else
    {
      m_pCMF->SetSourceLab(sSourceLab);
      m_pCMF->SetDestination(sDestLab);
      m_pCMF->SetSubmitByUserID(sUserID,true);
      m_pCMF->SetBatchId(sBatchID);
    }
  }
  if(!vsErrors.empty())
  {
    bRtn = false;
    nwxString::Join(vsErrors,&sError,'\n');
  }
  else if(m_pCMF->AllExcluded())
  {
    bRtn = false;
    sError = "Cannot create CMF file, no samples are included";
  }
  else if(_SaveFile(sFileName))
  {
    CParmOsirisGlobal parm;
    bool bView = m_pCheckViewLocation->GetValue();
    parm->SetCMFdefaultSample(sDefaultType);
    parm->SetCMFsourceLab(sSourceLab);
    parm->SetCMFdestLab(sDestLab);
    parm->SetCMFuserID(sUserID);
    parm->SetCMFviewLocation(bView);
    if(bView)
    {
      nwxFileUtil::ShowFileFolder(sFileName);
    }
  }
  else
  {
    bRtn = false;
    sError = "An error occurred when writing the CMF file.";
  }
  if(!bRtn)
  {
    if(sError.IsEmpty())
    {
      wxASSERT_MSG(0,
        "CDialogCMF::TransferDataFromWindow "
          "an error occurred and there is no message");
      sError = "Unspecified error.";
    }
    mainApp::ShowError(sError,this);
  }
  return bRtn;
}

void CDialogCMF::OnButtonLater(wxCommandEvent &)
{
  EndModal(IDbuttonFinishLater);
}
wxString CDialogCMF::_GetDefaultFileName()
{
  wxString s = m_pFile->GetExportFileName(EXT_CMF);
  return s;
}
void CDialogCMF::OnButtonBrowse(wxCommandEvent &)
{
  wxString sFileName;
  if(m_pDialogFile == NULL)
  {
    wxString sDefault = _GetDefaultFileName();
    sFileName = m_pTextFileName->GetValue();
    m_pDialogFile = nwxFileUtil::CreateSaveDialog(
      this,"Export CMF File",FILE_TYPE_CMF,sFileName,sDefault);
  }
  if(m_pDialogFile->ShowModal() == wxID_OK)
  {
    sFileName = m_pDialogFile->GetPath();
    m_pTextFileName->ChangeValue(sFileName);
  }
}

BEGIN_EVENT_TABLE(CDialogCMF,wxDialog)
EVT_BUTTON(IDbuttonFinishLater,CDialogCMF::OnButtonLater)
EVT_BUTTON(IDbrowseOutput,CDialogCMF::OnButtonBrowse)
END_EVENT_TABLE()

