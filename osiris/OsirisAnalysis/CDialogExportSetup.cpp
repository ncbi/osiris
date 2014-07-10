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
*  FileName: CDialogExportSetup.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CDialogExportSetup.h"
#include "CPageName.h"
#include "CPageParameters.h"
#include "CPageNoParameters.h"
#include "CPageAuto.h"
#include "ConfigDir.h"
#include "CParmOsiris.h"
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>

IMPLEMENT_ABSTRACT_CLASS(CDialogExportSetup,wxWizard)

#define MIN_WIDTH 520

const wxChar * const CDialogExportSetup::LABEL_FILE_NAME_EXTS(wxS("File name extensions"));
const wxChar * const CDialogExportSetup::LABEL_ALLOW_OVERRIDE(wxS("Allow user to override"));
const wxChar * const CDialogExportSetup::LABEL_DEFAULT_LOC_IN(wxS("Default input file location"));
const wxChar * const CDialogExportSetup::LABEL_DEFAULT_LOC_OUT(wxS("Default output file location"));
const wxChar * const CDialogExportSetup::OPTION_SAME_AS_ANALYSIS(wxS("[Same as Analysis File]"));
const wxChar * const CDialogExportSetup::OPTION_PARENT_OF_ANALYSIS(wxS("[Parent Directory of Analysis File]"));
const wxChar * const CDialogExportSetup::OPTION_REMEMBER_LAST_LOCATION(wxS("[Remember Last Location]"));
const wxChar * const CDialogExportSetup::OPTION_SAME_AS_OUTPUT(wxS("[Same as Output File]"));


CDialogExportSetup::CDialogExportSetup(
  wxWindow *parent,
  CExportFiles *pFiles,
  CXSLExportFileType *pExport,
  bool bReadOnly) :
    wxWizard(parent,wxID_ANY,
      wxEmptyString,
      wxNullBitmap,
      wxDefaultPosition,
      mainApp::DIALOG_STYLE),
    m_pExport(pExport),
    m_pFiles(pFiles),
    m_bReadOnly(bReadOnly)
{
  const wxChar *psTitle = NULL;
  if(bReadOnly)
  {
    psTitle = wxS("View Export File Type");
  }
  else if(pExport == NULL)
  {
    psTitle = wxS("Create Export File Type");
  }
  else
  {
    psTitle = wxS("Edit Export File Type");
  }
  SetTitle(psTitle);
  if(m_pExport != NULL)
  {
    m_ExportEdit = *m_pExport;
  }
  m_pPageName = new CPageName(
    this,&m_ExportEdit,bReadOnly);
  m_pPageParameters = new CPageParameters(
    this,m_pPageName,&m_ExportEdit,bReadOnly);
  m_pPageNoParameters = new CPageNoParameters(
    this, m_pPageParameters);
  m_pPageAuto = new CPageAuto(
    this,m_pPageNoParameters,&m_ExportEdit,bReadOnly);

  m_pPageName->SetNext(m_pPageParameters);
  m_pPageParameters->SetNext(m_pPageNoParameters);
  m_pPageNoParameters->SetNext(m_pPageAuto);

  Layout();
  CentreOnParent();
}
bool CDialogExportSetup::Show(bool show)
{
  // stupid hack because nothing else worked

  bool bRtn = wxWizard::Show(show);
  if(show)
  {
    wxSize sz = GetSize();
    if(sz.GetWidth() < MIN_WIDTH)
    {
      sz.SetWidth(MIN_WIDTH);
      SetSize(sz);
      CentreOnParent();
    }
  }
  return bRtn;
}
bool CDialogExportSetup::Run()
{
  bool b = RunWizard(m_pPageName);
  return b;
}

wxString CDialogExportSetup::GetSearchDir(
  bool bDir, const wxString &s1, const wxString &s2)
{
  wxString sSearch;
  wxString s;
  const wxString *psTry[2] = {&s1,&s2};
  for(int i = 0; sSearch.IsEmpty() && (i < 2); i++)
  {
    s = *psTry[i];
    nwxString::Trim(&s);
    if(!s.IsEmpty())
    {
      if(!bDir)
      {
        if(wxDir::Exists(s))
        {
          sSearch = s;
        }
        else
        {
          wxFileName fn(s);
          s = fn.GetPath(wxPATH_GET_VOLUME);
        }
      }
      if(sSearch.IsEmpty() && wxDir::Exists(s))
      {
        sSearch = s;
      }
    }
  }
  if(sSearch.IsEmpty())
  {
    sSearch = mainApp::GetConfig()->GetFilePath();
  }
  return sSearch;  
}

wxString CDialogExportSetup::PromptSearchDir(
  wxWindow *parent, const wxString &s1, const wxString &s2)
{
//  CParmOsirisGlobal parm;  STOP HERE
  wxString sSearch = GetSearchDir(true,s1,s2);
  wxString sRtn;
  wxDirDialog dlg(
    parent,
    "Choose a directory for export",
    sSearch,
    wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if(dlg.ShowModal() == wxID_OK)
  {
    sRtn = dlg.GetPath();
  }
  return sRtn;
}


void CDialogExportSetup::OnPageChanged(wxWizardEvent &e)
{
  e.Skip();
}
void CDialogExportSetup::SetupParameterPage()
{
  wxWizardPageSimple *pMiddle =
    m_ExportEdit.GetMapParam().Size()
    ? (wxWizardPageSimple *)m_pPageParameters
    : (wxWizardPageSimple *)m_pPageNoParameters;
  m_pPageName->SetNext(pMiddle);
  pMiddle->SetPrev(m_pPageName);
  pMiddle->SetNext(m_pPageAuto);
  m_pPageAuto->SetPrev(pMiddle);
}
void CDialogExportSetup::OnPageChanging(wxWizardEvent &e)
{
  e.Skip();
}
void CDialogExportSetup::OnCancel(wxWizardEvent &e)
{
  if( (m_pExport == NULL) || (m_ExportEdit != (*m_pExport)) )
  {
    wxMessageDialog dlg(
      this,"Do you want to cancel and lose your changes?",
      "Cancel",
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dlg.CentreOnParent();
    int nRtn = dlg.ShowModal();
    if(nRtn == wxID_NO || nRtn == wxID_CANCEL)
    {
      e.Veto();
    }
  }
}
void CDialogExportSetup::OnFinished(wxWizardEvent &e)
{
//  bool bSkip = true;
  bool bSave = false;
  if(m_bReadOnly)
  {
    // we are done
  }
  else if(m_pExport == NULL)
  {
    if(!m_pFiles->Insert(new CXSLExportFileType(m_ExportEdit)))
    {
      e.Veto();
//      bSkip = false;
      mainApp::ShowError("Cannot create new file type",this);
    }
    else
    {
      bSave = true;
    }
  }
  else if(m_ExportEdit != (*m_pExport))
  {
    const wxString &sName(m_ExportEdit.GetName());
    if(sName == m_pExport->GetName()) 
    {
      bSave = true;
    }
    else if(m_pFiles->ChangeKey(m_pExport,sName))
    {
      bSave = true;
    }
    else
    {
      e.Veto();
//      bSkip = false;
      mainApp::ShowError(
        "Cannot set export file description because it is already used."
        "\nClick on \"Back\" twice to change the description."
        ,this);

    }
    if(bSave)
    {
      (*m_pExport) = m_ExportEdit;
    }
  }
  if(!bSave) {}
  else if(!m_pFiles->SaveExportFile())
  {
//    bSkip = false;
    e.Veto();
    mainApp::ShowError("Cannot save export file settings",this);
  }
  else
  {
    wxWindow *pWindow = GetParent();
    if(pWindow != NULL)
    {
      pWindow->TransferDataToWindow();
    }
  }
}

BEGIN_EVENT_TABLE(CDialogExportSetup,wxWizard)
EVT_WIZARD_PAGE_CHANGED(wxID_ANY,CDialogExportSetup::OnPageChanged)
EVT_WIZARD_PAGE_CHANGING(wxID_ANY,CDialogExportSetup::OnPageChanging)
EVT_WIZARD_CANCEL(wxID_ANY,CDialogExportSetup::OnCancel)
EVT_WIZARD_FINISHED(wxID_ANY,CDialogExportSetup::OnFinished)
END_EVENT_TABLE()
