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
*
*  FileName: CDialogSitePath.cpp
*  Author:   Douglas Hoffman
*  Date:     2018/02/07
*
*  dialog window for the user to select a
*  group for a shared site path (site settings:
*     Volumes, exports, etc)
*
*/
#ifdef __WXMAC__
#include "mainApp.h"
#include "CDialogSitePath.h"
#include "CSitePath.h"
#include "nwx/nwxUserInfo.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxString.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <nwx/stdb.h>
#include <set>
#include <nwx/stde.h>

IMPLEMENT_ABSTRACT_CLASS(CDialogSitePath,wxDialog)

bool CDialogSitePath::_filterGroup(const wxString &sGroupName)
{
  // check to see if group name should be filtered out
  // currently if the name begins with "com.apple." or "_"
  bool bRtn = ( !sGroupName.Find(wxS("com.apple.")) ) || ( !sGroupName.Find(wxS("_")) );
  return bRtn;
}

CDialogSitePath::CDialogSitePath(wxWindow *parent) :
 wxDialog(
      parent,
      wxID_ANY,
      wxS("Site Settings"),
      wxDefaultPosition,
      wxDefaultSize,
      mainApp::DIALOG_STYLE),
 m_bUserOnly(false)
{
  wxString sOperation;
  bool bNeedAdmin = false;
  CSitePath *pSitePath = CSitePath::GetGlobal();
  nwxUserInfo *pUserInfo = nwxUserInfo::GetGlobal();

  //  build list of groups

  const wxArrayString &aAllGroups(pUserInfo->GetGroupNames());
  wxString sDefaultGroup(pSitePath->GetSitePathGroup());
  if(sDefaultGroup.IsEmpty())
  {
    sDefaultGroup = pUserInfo->GetDefaultGroupName();
  }
  wxArrayString asGroupsKeep;
  std::set<wxString, nwxStringLessNoCaseSort> setGroups;
  std::set<wxString, nwxStringLessNoCaseSort>::iterator itr;
  size_t nCount = aAllGroups.GetCount();
  size_t nDefault = 0;
  size_t i;
  asGroupsKeep.Alloc(aAllGroups.GetCount());
  for(i = 0; i < nCount; ++i)
  {
    const wxString &s(aAllGroups.Item(i));
    if(!_filterGroup(s))
    {
      setGroups.insert(s);
    }
  }
  for(itr = setGroups.begin(); itr != setGroups.end(); ++itr)
  {
    if(*itr == sDefaultGroup)
    {
      nDefault = asGroupsKeep.GetCount();
    }
    asGroupsKeep.Add(*itr);
  }
  if(pSitePath->SitePathExists())
  {
    bNeedAdmin = !pSitePath->CurrentUserOwnsAll();
    sOperation = wxS("Set access for OSIRIS site settings");
  }
  else
  {
    bNeedAdmin = !pSitePath->ExistingParentWritable();
    sOperation = wxS(
      "Initialize and set access for OSIRIS site settings");
  }
  if(bNeedAdmin)
  {
    wxString sExistingParent =
      wxS("Existing parent of Osiris-Files:\n");
    sExistingParent.Append(
      pSitePath->GetExistingParent());
    nwxLog::LogMessage(sExistingParent);
    sOperation.Append(
      "\nThis requires administrative priviledges."
      "\nYou will be prompted for a user name and password.");
  }
  wxStaticText *pTextTitle = new wxStaticText(this,wxID_ANY,sOperation);
  m_pRadioUserOnly = new wxRadioButton
    (this,wxID_ANY,wxS("Allow only this user to modify site settings"),
     wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  m_pRadioGroup = new wxRadioButton
    (this,wxID_ANY,wxS("Allow group to modify site settings"));
  if(asGroupsKeep.GetCount())
  {
    m_pRadioGroup->SetValue(true);
    m_pTextGroup = new wxStaticText
      (this,wxID_ANY,wxS("Select group: "));
    m_pChoiceGroup = new wxChoice(this, wxID_ANY,
       wxDefaultPosition, wxDefaultSize, asGroupsKeep);
    m_pChoiceGroup->SetSelection((int) nDefault);       
  }
  else
  {
    m_pTextGroup = NULL;
    m_pChoiceGroup = NULL;
    m_pRadioUserOnly->SetValue(true);
    m_pRadioGroup->Enable(false);
  }

  wxBoxSizer *pSizerMain = new wxBoxSizer(wxVERTICAL);
  pSizerMain->Add(pTextTitle, 0,
    wxALIGN_LEFT | wxALL, ID_BORDER);
  pSizerMain->Add(m_pRadioUserOnly,0,
    wxALIGN_LEFT | wxBOTTOM | wxLEFT | wxRIGHT, ID_BORDER);
  pSizerMain->Add(m_pRadioGroup,0,
    wxALIGN_LEFT | wxBOTTOM | wxLEFT | wxRIGHT, ID_BORDER);
  if(m_pChoiceGroup != NULL)
  {
    wxBoxSizer *pSizerGroup = new wxBoxSizer(wxHORIZONTAL);
    pSizerGroup->Add(m_pTextGroup,0, wxALIGN_LEFT | wxRIGHT, ID_BORDER);
    pSizerGroup->Add(m_pChoiceGroup, 1, wxALIGN_LEFT, 0);
    pSizerMain->Add(pSizerGroup,0,
      wxALIGN_LEFT | wxBOTTOM | wxLEFT | wxRIGHT, ID_BORDER);
  }
  pSizerMain->Add(CreateButtonSizer(wxOK | wxCANCEL), 0,
    (wxALL ^ wxTOP) | wxALIGN_CENTER,ID_BORDER);
  SetSizer(pSizerMain);
  Layout();
  Fit();
}

void CDialogSitePath::OnRadioButton(wxCommandEvent &)
{
  if(m_pChoiceGroup != NULL)
  {
    bool bEnable = !!m_pRadioGroup->GetValue();
    m_pChoiceGroup->Enable(bEnable);
    m_pTextGroup->Enable(bEnable);
  }
}

bool CDialogSitePath::ProcessSitePath(bool bPromptError)
{
  bool bRtn = true;
  wxString sUser;
  wxString sGroup;
  if(m_pRadioUserOnly->GetValue())
  {
    // this user only
    nwxUserInfo *pUserInfo = nwxUserInfo::GetGlobal();
    sUser = pUserInfo->GetUserName();
  }
  else
  {
    int nSelect = m_pChoiceGroup->GetSelection();
    bRtn = (nSelect != wxNOT_FOUND);
    if(bRtn)
    {
      sGroup = m_pChoiceGroup->GetString((unsigned int) nSelect);
      bRtn = !sGroup.IsEmpty();
    }
  }
  if(bRtn)
  {
    CSitePath *pSitePath = CSitePath::GetGlobal();
    pSitePath->LogTestString();
    bRtn = pSitePath->UpdateSitePath(sGroup,sUser);
  }
  if( (!bRtn) && bPromptError )
  {
    _promptError();
  }
  return bRtn;
}
void CDialogSitePath::_promptError()
{
  CSitePath *pSitePath = CSitePath::GetGlobal();
  wxString sMsg;
  sMsg = pSitePath->SitePathExists()
    ? wxS("An error occurred while\nupdating OSIRIS site settings\n\n")
    : wxS("An error occurred while\ncreating OSIRIS site settings\n\n");
  sMsg.Append(pSitePath->LastErrorStr());
  mainApp::ShowError(sMsg, GetParent());
  mainApp::LogMessage(sMsg);
}
BEGIN_EVENT_TABLE(CDialogSitePath,wxDialog)
EVT_RADIOBUTTON(wxID_ANY,CDialogSitePath::OnRadioButton)
END_EVENT_TABLE()

#endif
