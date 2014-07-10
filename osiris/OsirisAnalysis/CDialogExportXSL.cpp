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
*  FileName: CDialogExportXSL.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CDialogExportXSL.h"
#include "CXSLExportFileType.h"
#include "CParmOsiris.h"
#include "wxIDS.h"
#include <wx/listbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include "nwx/CIncrementer.h"
#include "CDialogExportSetup.h"

#define LABEL_NEW "New..."

#define MAX_EXPORTS ID_MAX_USER_EXPORT

CExportFiles *CDialogExportXSL::GetGlobal()
{
  CExportFiles *pXSL = CExportFiles::GetGlobal();
  if(pXSL != NULL)
  {
    m_sError.Empty();
  }
  else if(CExportFiles::IsGlobalLocked())
  {
    m_sError =
      "The export file setup is locked by another user,\n"
      "please try again later";
  }
  else
  {
    m_sError = "Cannot retrieve export file information.";
  }
  return pXSL;
}

CDialogExportXSL::~CDialogExportXSL()
{
  CExportFiles *pXSL = GetGlobal();
  if(pXSL != NULL)
  {
    if(m_bModified)
    {
      pXSL->SaveExportFile();
    }
    pXSL->Unlock();
  }
}

CDialogExportXSL::CDialogExportXSL(wxWindow *parent) :
  wxDialog(
      parent,
      wxID_ANY,
      "Export File Types",
      wxDefaultPosition,
      wxDefaultSize,
      mainApp::DIALOG_STYLE),
  m_nLock(0),
  m_nTimer(0),
  m_bFirstTransfer(true),
  m_bShowingList(false),
  m_bModified(false),
  m_bNoShow(false)
{

  wxButton *pButtonNew;
  wxButton *pButtonDone;
  wxGridSizer *pSizerButton;
  wxBoxSizer *pSizerBottom;
  wxBoxSizer *pSizerAll;
  wxBoxSizer *pSizerDone;

  SetMinSize(wxSize(300,200));  

  m_pLabelNone1 = new wxStaticText(
    this,wxID_ANY,
    "There are no export file types available.");
  m_pLabelNone2 = new wxStaticText(
    this,wxID_ANY,
    "Select \"" LABEL_NEW "\" below to create one.");
  m_pLabelList = new wxStaticText(
    this,wxID_ANY,
    "Available export file types");
  m_pListBox = new wxListBox(
    this, wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    0, NULL,
    wxLB_SINGLE | wxLB_NEEDED_SB  | wxLB_HSCROLL
#ifndef __WXMAC__
    | wxLB_SORT
#endif
    );

  m_pSizerList = new wxBoxSizer(wxVERTICAL);

  pButtonNew = new wxButton(this,IDadd,LABEL_NEW);
  m_pButtonRemove = new wxButton(this,IDremove,"Remove...");
  m_pButtonEdit = new wxButton(this,IDedit,"Edit...");
  pButtonDone = new wxButton(this,wxID_OK,"Done");

  pSizerButton = new wxGridSizer(1,0,ID_BORDER,0);
  pSizerButton->Add(pButtonNew,0,wxEXPAND,0);
  pSizerButton->Add(m_pButtonRemove,0,wxEXPAND,0);
  pSizerButton->Add(m_pButtonEdit,0,wxEXPAND,0);

  pSizerDone = new wxBoxSizer(wxHORIZONTAL);
  pSizerDone->AddStretchSpacer(1);
  pSizerDone->Add(pButtonDone,
    0,wxLEFT | wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER << 2);

  pSizerBottom = new wxBoxSizer(wxHORIZONTAL);
  pSizerBottom->AddStretchSpacer(1);
  pSizerBottom->Add(pSizerButton,0, wxALIGN_CENTRE);
  pSizerBottom->Add(pSizerDone,1,wxEXPAND,0);

  pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(m_pSizerList,1,wxALL | wxEXPAND,ID_BORDER);
  pSizerAll->Add(pSizerBottom,0,(wxALL ^ wxTOP) | wxEXPAND,ID_BORDER);
  SetSizer(pSizerAll);

  // do not layout yet,
  // wait until TransferToWindow()
}

bool CDialogExportXSL::TransferDataToWindow()
{
  wxArrayString asList;
  CExportFiles *pXSL = GetGlobal();
  bool bLayout = m_bFirstTransfer;
  bool bRtn = true;
  if(pXSL == NULL)
  {
    ShowError(true);
    bLayout = false;
    bRtn = false;
  }
  else if(pXSL->GetItems(&asList))
  {
    wxString sSelection = m_pListBox->GetStringSelection();
    m_pListBox->Clear();
    m_pListBox->InsertItems(asList,0);
    if(!m_bShowingList)
    {
      m_pSizerList->Clear();
      m_pSizerList->Add(m_pLabelList,0,wxALIGN_LEFT,0);
      m_pSizerList->Add(m_pListBox,1,wxEXPAND,0);
      m_bShowingList = true;
      bLayout = true;
    }
    if(sSelection.IsEmpty() || 
      !m_pListBox->SetStringSelection(sSelection))
    {
      m_pListBox->SetSelection(0);
    }
  }
  else if(bLayout || m_bShowingList)
  {
    m_pSizerList->Clear();
    m_pSizerList->AddStretchSpacer(1);
    m_pSizerList->Add(m_pLabelNone1,0,wxALIGN_CENTRE_HORIZONTAL | wxALIGN_BOTTOM,0);
    m_pSizerList->Add(m_pLabelNone2,0,wxALIGN_CENTRE_HORIZONTAL | wxALIGN_TOP,0);
    m_pSizerList->AddStretchSpacer(1);
    m_bShowingList = false;
    bLayout = true;
  }
  if(bLayout)
  {
    m_pLabelNone1->Show(!m_bShowingList);
    m_pLabelNone2->Show(!m_bShowingList);
    m_pLabelList->Show(m_bShowingList);
    m_pListBox->Show(m_bShowingList);
    m_pButtonEdit->Enable(m_bShowingList);
    m_pButtonRemove->Enable(m_bShowingList);

    Layout();
    if(m_bFirstTransfer)
    {
      Fit();
      m_bFirstTransfer = false;
    }
  }
  return bRtn;
}
bool CDialogExportXSL::ShowLockProblem(CExportFiles *pXSL, bool bPrompt)
{
  wxString sMsg;
  bool bRtn = true;
  if(pXSL->IsLocked())
  {
    sMsg = "Export configuration is locked by another user.";
  }
  else
  {
    sMsg =
      "Cannot lock export configuration,\n"
      "possibly due to access privileges";
  }
  if(bPrompt)
  {
    sMsg.Append("\n\nWould you like to view this file type?");
    wxMessageDialog msg(this,sMsg,"Alert",wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    msg.CenterOnParent();
    int n = msg.ShowModal();
    bRtn = (n == wxID_YES) || (n == wxID_OK);
  }
  else
  {
    mainApp::ShowAlert(sMsg,this);
  }
  return bRtn;
}

void CDialogExportXSL::OnNew(wxCommandEvent &)
{
  CExportFiles *pXSL = GetGlobal();
  wxString sMsg;
  if(pXSL == NULL)
  {
    ShowError(false);
  }
  else if(pXSL->Size() >= MAX_EXPORTS)
  {
    wxString sError =
      wxString::Format(
      "You have reached the maximum number \n"
         "of export file formats: %d",MAX_EXPORTS);
    mainApp::ShowError(sError,this);
  }
  else if(pXSL->LockExportFile())
  {
    CIncrementer x(m_nLock);
    _SaveParms();
    CDialogExportSetup dlg(this,pXSL);
    if(dlg.Run())
    {
      m_bModified = true;
      TransferDataToWindow();
    }
    else
    {
      _RestoreParms();
    }
  }
  else
  {
    ShowLockProblem(pXSL);
  }
}
void CDialogExportXSL::OnDelete(wxCommandEvent &)
{
  CExportFiles *pXSL = GetGlobal();
  if(pXSL == NULL)
  {
    ShowError(true);
  }
  else if(pXSL->LockExportFile())
  {
    CIncrementer x(m_nLock);
    wxString sMsg;
    wxString sSelect = m_pListBox->GetStringSelection();
    sMsg = "Do you wish to delete\n";
    sMsg.Append(sSelect);
    wxMessageDialog msg(
      this,sMsg,"Delete File Type",
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    msg.CentreOnParent();
    int n = msg.ShowModal();
    if(!(n == wxID_YES || n == wxID_OK))
    {}
    else if(!pXSL->Delete(sSelect))
    {
      sMsg = "An error occurred when attempting to delete\n";
      sMsg.Append(sSelect);
      mainApp::ShowError(sMsg,this);
    }
    else
    {
      TransferDataToWindow();
      m_bModified = true;
    }
  }
  else
  {
    ShowLockProblem(pXSL);
  }
}
void CDialogExportXSL::OnEdit(wxCommandEvent &)
{
  wxString sSelect = m_pListBox->GetStringSelection();
  CExportFiles *pXSL = GetGlobal();
  if(pXSL == NULL)
  {
    ShowError(false);
  }
  else
  {
    CXSLExportFileType *pType = pXSL->Find(sSelect);
    if(pType == NULL)
    {
      mainApp::ShowError(
        "Cannot find file type.\n"
        "It was possibly deleted by another user."
        ,this);
    }
    else if(pXSL->LockExportFile())
    {
      CIncrementer x(m_nLock);
      _SaveParms();
      CDialogExportSetup dlg(this,pXSL,pType,false);
      if(dlg.Run())
      {
        m_bModified = true;
        TransferDataToWindow();
      }
      else
      {
        _RestoreParms();
      }
    }
    else if(ShowLockProblem(pXSL,true))
    {
      CDialogExportSetup dlg(this,pXSL,pType,true);
      dlg.Run();
    }
  }
}

void CDialogExportXSL::OnTimer(wxTimerEvent &)
{
  if(m_nLock != 0)
  {
    m_nTimer++;
    if(!(m_nTimer & 7))
    {
      CExportFiles *pXSL = GetGlobal();
      if(pXSL != NULL)
      {
        pXSL->UpdateLock();
      }
      m_nTimer = 0;
    }
  }
}


BEGIN_EVENT_TABLE(CDialogExportXSL,wxDialog)
EVT_BUTTON(IDadd,CDialogExportXSL::OnNew)
EVT_BUTTON(IDremove,CDialogExportXSL::OnDelete)
EVT_BUTTON(IDedit,CDialogExportXSL::OnEdit)
END_EVENT_TABLE()

