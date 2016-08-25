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
*  FileName: CPanelSampleAlertDetails.h
*
*    this should have been named CPanelAlertDetails
*    it is used to edit various alerts:
*       sample, ils, channel, and directory
*
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CPanelSampleAlertDetails.h"
#include "COARsample.h"
#include "COARnotes.h"
#include "COARmessage.h"
#include "COARchannelAlert.h"
#include "COARfile.h"
#include <wx/panel.h>
#include <wx/datetime.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/dialog.h>
#include "nwx/nwxColorUtil.h"
#include "nwx/nwxString.h"
#include "wxIDS.h"
#include "Platform.h"
#include "CLabels.h"

CPanelSampleAlertDetails::CPanelSampleAlertDetails(
    wxWindow *parent,
    COARmessages *pMsgEdit,
    const wxString &_sNotesLabel,
    int nGridType,
    bool bSplitHorizontal,
    bool bReadOnly) : wxSplitterWindow
      ( parent,wxID_ANY,
        wxDefaultPosition,wxDefaultSize, 
        ID_SPLITTER_STYLE
      ),
      m_pMsgEdit(pMsgEdit),
//      m_bChannelGrid(bChannelGrid),
      m_bSplitHorizontal(bSplitHorizontal)
{
  wxString sNotesLabel(_sNotesLabel);
  sNotesLabel.Append(CLabels::NOTES);
  m_pPanelGrid = new wxPanel(this);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  m_pPanelNotes = new wxPanel(this);
  wxStaticText *pLabelNotes = new wxStaticText(m_pPanelNotes,wxID_ANY,sNotesLabel);

  m_pGridAlerts = new CGridAlerts(
    pMsgEdit,m_pPanelGrid,wxID_ANY,nGridType,bReadOnly);
  pSizer->Add(m_pGridAlerts,1,wxALL | wxEXPAND, ID_BORDER);
  m_pPanelGrid->SetSizer(pSizer);
  m_pTextNewNotes = NULL;
  m_pTextNotes = new wxTextCtrl(m_pPanelNotes,wxID_ANY,wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_DONTWRAP | wxTE_READONLY);
  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(
    pLabelNotes,0, (wxALL ^ wxBOTTOM) | wxALIGN_LEFT,ID_BORDER);
  pSizer->Add(
    m_pTextNotes,2,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  if(!bReadOnly)
  {
    wxStaticText *pLabelNewNotes = new wxStaticText(
      m_pPanelNotes,wxID_ANY,CLabels::ADDITIONAL_NOTES);
    m_pTextNewNotes = new wxTextCtrl(m_pPanelNotes,wxID_ANY,
        wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_DONTWRAP);
    pSizer->Add(pLabelNewNotes,0,
       wxLEFT | wxRIGHT | wxALIGN_LEFT, ID_BORDER);
    pSizer->Add(m_pTextNewNotes,1,
      (wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  }
  m_pPanelNotes->SetSizer(pSizer);

  // this may be a problem because it is done
  // before the parent calls Layout

  SetSashGravity(1.0);
  SetMinimumPaneSize(10);
  _SetupColours();
}

void CPanelSampleAlertDetails::_SetupColours()
{
  wxClassInfo *pClass[] = 
  {
    CLASSINFO(wxPanel),
    CLASSINFO(wxDialog),
    CLASSINFO(wxTopLevelWindow)
  };
  wxWindow *parent;
  for(parent = GetParent();
      (parent != NULL) &&
        !parent->IsKindOf(pClass[0]) &&
        !parent->IsKindOf(pClass[1]) &&
        !parent->IsKindOf(pClass[2]);
      parent = parent->GetParent()
      )
  {;}
  if(parent != NULL)
  {
    wxColour clr = parent->GetBackgroundColour();
    m_pPanelGrid->SetBackgroundColour(clr);
    m_pPanelNotes->SetBackgroundColour(clr);
  }
  nwxColorUtil::BackgroundParent(m_pTextNotes,true);
}

bool CPanelSampleAlertDetails::Show(bool bShow)
{
  bool bRtn = wxSplitterWindow::Show(bShow);
  bool bIsShown = bShow && IsShownOnScreen();
  if(bShow && bIsShown && !IsSplit())
  {
    Split();
  }
  return bRtn;
}
void CPanelSampleAlertDetails::Split()
{
  if(IsSplit())
  {
    Unsplit();
  }
  if(m_bSplitHorizontal)
  {
    SplitHorizontally(
      m_pPanelGrid,m_pPanelNotes,0);
  }
  else
  {
    SplitVertically(
      m_pPanelGrid,m_pPanelNotes,0);
  }
}
void CPanelSampleAlertDetails::SetupChannelColumn(
    const COARsample *pSample,
    const map<int,wxString> *pmapChannelNames)
{
  m_pGridAlerts->SetupChannelColumn(pSample,pmapChannelNames);
#if 0
  wxASSERT_MSG(
    m_bChannelGrid,
    "CPanelSampleAlertDetails::SetupChannelColumn()\n"
    "THIS IS NOT A CHANNEL GRID");
  CGridChannelCount vpCount;
  vector<COARchannelAlert *>::const_iterator itrch;
  const vector<COARchannelAlert *> *pchAlerts =
    pSample->GetChannelAlerts();
  const COARchannelAlert *pcha;
  int nChannel;
  size_t nCount;

  // set up vpCount for channel alert grid, column 0
  for(itrch = pchAlerts->begin(); itrch != pchAlerts->end(); ++itrch)
  {
    pcha = *itrch;
    nCount = pcha->GetMessageNumbers()->size();
    nChannel = pcha->GetNumber();
    vpCount.Append(nChannel,nCount);
  }
  m_pGridAlerts->SetupChannelColumn(
      vpCount,pmapChannelNames);
#endif
}
bool CPanelSampleAlertDetails::IsNotesModified()
{
  bool bRtn = false;
  if(m_pTextNewNotes != NULL)
  {
    wxString s = m_pTextNewNotes->GetValue();
    nwxString::Trim(&s);
    bRtn = !s.IsEmpty();
  }
  return bRtn;
}
/*
// commented 1/30/11
wxString CPanelSampleAlertDetails::GetNotesText(const wxString &sUser)
{
  wxString sRtn;
  if(m_pTextNewNotes == NULL)
  {
    sRtn = m_pTextNotes->GetValue();
    nwxString::Trim(&sRtn);
  }
  else
  {
    sRtn = COARfile::AppendNotes(
      m_pTextNotes->GetValue(),m_pTextNewNotes->GetValue(),sUser);
  }
  return sRtn;
}
*/

wxString CPanelSampleAlertDetails::GetNewNotesValue()
{
  wxString sRtn;
  if(m_pTextNewNotes != NULL)
  {
    sRtn = m_pTextNewNotes->GetValue();
    nwxString::Trim(&sRtn);
  }
  return sRtn;
}
/*
// commented 1/31/11
wxString CPanelSampleAlertDetails::GetNewNotes(const COARnotes *pCurrent, const wxString &sUser)
{
  wxString sRtn;
  if(m_pTextNewNotes != NULL)
  {
    sRtn = COARfile::AppendNotes(pCurrent->GetText(),m_pTextNewNotes->GetValue(),sUser);
  }
  else
  {
    sRtn = pCurrent->GetText();
  }
  return sRtn;
}
*/
IMPLEMENT_ABSTRACT_CLASS(CPanelSampleAlertDetails,wxSplitterWindow)