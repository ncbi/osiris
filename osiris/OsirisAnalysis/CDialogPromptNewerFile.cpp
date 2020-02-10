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
*  FileName: CDialogPromptNewerFile.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CDialogPromptNewerFile.h"
#include "wxIDS.h"
#include "mainApp.h"
#include <wx/utils.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "nwx/nwxFileUtil.h"
#define AFTER_FILE_NAMES(s,sNewFile) \
  s.Append("contains a newer file,\n  "); \
  s.Append(sNewFile); \
  s.Append("\n\nWould you like to open the newer file?") \


CDialogPromptNewerFile::CDialogPromptNewerFile(
  wxWindow *parent, const SET_FILE_NAMES &setFileNames, const wxString &sNewest) :
    wxDialog(
      parent,wxID_ANY,"Newer file available",
      wxDefaultPosition, wxDefaultSize,
      mainApp::DIALOG_STYLE)
{
  wxFileName fnNew(sNewest);
  wxString sPrompt;
  sPrompt.Alloc(1024);
  const wxChar *ButtonText[4] = 
  {
    wxS("Open the newer file, also"),
    wxS("Open the selected files"),
    wxS("Do not open any of the above files"),
    wxS("View folder before deciding")
  };
  SetupFileToShow(fnNew);

  sPrompt = "The folder containing the selected files:\n";
  for(SET_FILE_NAMES::const_iterator itr = setFileNames.begin(); itr != setFileNames.end(); ++itr)
  {
    wxFileName fn(*itr);
    sPrompt.Append("  ");
    sPrompt.Append(fn.GetFullName());
    sPrompt.Append("\n");
  }
  AFTER_FILE_NAMES(sPrompt,fnNew.GetFullName());
  Setup(sPrompt,ButtonText);
}

CDialogPromptNewerFile::CDialogPromptNewerFile(
  wxWindow *parent, const wxString &sFileName, const wxString &sNewest) :
    wxDialog(
      parent,wxID_ANY,"Newer file available",
      wxDefaultPosition, wxDefaultSize,
      mainApp::DIALOG_STYLE)
{
  wxFileName fn1(sFileName);
  wxFileName fn2(sNewest);

  wxString sPrompt;
  const wxChar *ButtonText[4] = 
  {
    wxS("Open the newer file"),        // yes
		wxS("Open the selected file"),     // no
		wxS("Do not open either file"),    // cancel
		wxS("View folder before deciding") // view
  };
  SetupFileToShow(fn2);
  sPrompt.Alloc(1024);
  sPrompt = "The folder containing the selected file,\n  ";
  sPrompt.Append(fn1.GetFullName());
  sPrompt.Append("\n");
  AFTER_FILE_NAMES(sPrompt,fn2.GetFullName());
  Setup(sPrompt,ButtonText);
}

void CDialogPromptNewerFile::Setup(const wxString &sPrompt, const wxChar *ButtonText[4])
{
  int nFlagLabel = wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL;
  int nFlagButton = wxEXPAND;

  wxStaticText *pPrompt = new wxStaticText(this,wxID_ANY,sPrompt);
  wxButton *pButtons[4];
  pButtons[0] = new wxButton(this,wxID_YES,"Yes");
  pButtons[1] = new wxButton(this,wxID_NO,"No");
  pButtons[2] = new wxButton(this,wxID_CANCEL,"Cancel");
  pButtons[3] = new wxButton(this,IDbuttonView,"View");
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  wxFlexGridSizer *pSizerButtons = new wxFlexGridSizer(4,2,ID_BORDER, ID_BORDER);
  for(size_t i = 0; i < 4; i++)
  {
    pSizerButtons->Add(
      pButtons[i],0,nFlagButton);
    pSizerButtons->Add(
      new wxStaticText(this,wxID_ANY,ButtonText[i]),
      1,nFlagLabel);
  }
  if (m_sFileToShow.IsEmpty())
  {
    pButtons[3]->Disable();
  }
  pSizerAll->Add(pPrompt,0,wxALL | wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pSizerAll->AddStretchSpacer(1);
  pSizerAll->Add(pSizerButtons,0,
    wxALL | wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL, 
    ID_BORDER);
  SetSizer(pSizerAll);
  Fit();
  CentreOnParent();
}

void CDialogPromptNewerFile::SetupFileToShow(const wxFileName &fn)
{
  if (fn.Exists())
  {
    m_sFileToShow = fn.GetFullPath();
  }
  else
  {
    m_sFileToShow.Empty();
  }
}

void CDialogPromptNewerFile::OnShowFiles(wxCommandEvent &)
{
  if (!m_sFileToShow.IsEmpty())
  {
    nwxFileUtil::ShowFileFolder(m_sFileToShow, false);
  }
}
void CDialogPromptNewerFile::OnClick(wxCommandEvent &e)
{
  EndModal(e.GetId());
}

BEGIN_EVENT_TABLE(CDialogPromptNewerFile,wxDialog)
EVT_BUTTON(IDbuttonView,CDialogPromptNewerFile::OnShowFiles)
EVT_BUTTON(wxID_YES,CDialogPromptNewerFile::OnClick)
EVT_BUTTON(wxID_NO,CDialogPromptNewerFile::OnClick)
END_EVENT_TABLE()
