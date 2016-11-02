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
*  FileName: CPageEditSample.h
*  Author:   Douglas Hoffman
*
*  Interface class for any panel used for editing part of a sample
*
*/

#include "CPageEditSample.h"
#include "CNotebookEditSample.h"
#include "CPanelLocusDetails.h"
#include "CLabels.h"
#include <wx/sizer.h>
#include <wx/panel.h>
#include "nwx/nwxLog.h"

//
// BEGIN CPageEditSample
//
CPageEditSample::~CPageEditSample()
{
  if(m_pReview != NULL) { delete m_pReview; }
  if(m_pAccept != NULL) { delete m_pAccept; }
  if(m_pDisable != NULL) { delete m_pDisable; }
}
wxWindow *CPageEditSample::GetParentWindow()
{
  return m_pParentNotebook->GetNotebookWindow();
}
const wxString &CPageEditSample::GetUserID()
{
  return m_pParentNotebook->GetUserID();
}
bool CPageEditSample::DoAccept()
{
  bool bRtn = false;
  if(m_pAccept == NULL)
  {
    m_pAccept = CreateAcceptReceiver();
  }
  if(m_pAccept == NULL)
  {
    nwxLog::LogMessage("CPageEditSample::DoAccept(): CreateAcceptReceiver() failed");
  }
  else if(m_pAccept->AppendReview(GetUserID()))
  {
    bRtn = true; 
    m_pParentNotebook->InitiateRepaintData();
  }
  else
  {
    wxString sError;
    IAppendReview::FormatError(&sError,m_pAccept,GetUserID());
    mainApp::ShowError(sError,GetParentWindow()); 
  }
  return bRtn;
}
bool CPageEditSample::DoReview()
{
  bool bRtn = false;
  if(m_pReview == NULL)
  {
    m_pReview = CreateReviewReceiver();
  }
  if((m_pReview != NULL) && m_pReview->AppendReview(GetUserID()))
  {
    bRtn = true;
  }
  return bRtn;
}
bool CPageEditSample::DoToggleEnabled()
{
  bool bRtn = false;
  if(m_pDisable == NULL)
  {
    m_pDisable = CreateDisableReceiver();
  }
  if((m_pDisable != NULL) && m_pDisable->AppendReview(GetUserID()))
  {
    bRtn = true;
    m_pParentNotebook->InitiateRepaintData();
  }
  return bRtn;
}


const wxString &CPageEditSample::GetTreePageLabel()
{
  if(NeedsApply())
  {
    if(m_sTreePageLabelModified.IsEmpty())
    {
      m_sTreePageLabelModified = GetPageLabel();
      m_sTreePageLabelModified.Append(wxT(" *"));
    }
    return m_sTreePageLabelModified;
  }
  if(m_sTreePageLabelPlain.IsEmpty())
  {
    m_sTreePageLabelPlain = GetPageLabel();
    m_sTreePageLabelPlain.Append(wxT("    "));
  }
  return m_sTreePageLabelPlain;
}

wxWindow *CPageEditSample::GetPanel()
{
  if(m_pPanel == NULL)
  {
    m_pPanel = CreatePanel(GetParentWindow());
  }
  return m_pPanel;
}
wxWindow *CPageEditSample::GetPanelPage()
{
  return GetPanel();
}

//
//  END CPageEditSample
//  BEGIN CEditAlertsDir, CEditAlertsSample, CEditAlertsILS, CEditAlertsChannel, CPageEditLocus
//
//  BEGIN CreatePanel,CreateSubPanel
//
CPanelSampleAlertDetails *CEditAlertsDir::CreateSubPanel(wxWindow *parent)
{
  UpdateMessages();
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
    parent,
    &m_Msg,
    GetLabelPrefix(),
    CGridAlerts::TYPE_LOCUS,
    true,
    IsReadOnly());
  pPanel->SetupLocusColumn(m_vsLocus);
  return pPanel;
}
CPanelSampleAlertDetails *CEditAlertsSample::CreateSubPanel(wxWindow *parent)
{
  UpdateMessages();
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      parent,
      &m_Msg,
      GetLabelPrefix(),
      0,
      true,
      IsReadOnly());
  return pPanel;
}
CPanelSampleAlertDetails *CEditAlertsILS::CreateSubPanel(wxWindow *parent)
{
  UpdateMessages();
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      parent,
      &m_Msg,
      GetLabelPrefix(),
      0,
      true,
      IsReadOnly());
  return pPanel;
}
CPanelSampleAlertDetails *CEditAlertsChannel::CreateSubPanel(wxWindow *parent)
{
  UpdateMessages();
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      parent,
      &m_Msg,
      GetLabelPrefix(),
      CGridAlerts::TYPE_CHANNEL,
      true,
      IsReadOnly());
  pPanel->SetupChannelColumn(GetSample(),m_pmapChannelNames);
  return pPanel;
}
wxWindow *CPageEditSampleAlerts::CreatePanel(wxWindow *parent)
{
#if 1
  CPanelSampleAlertDetails *pSub = CreateSubPanel(parent);
  return pSub;
#else
  wxWindow *pParent = GetParentWindow();
  wxASSERT_MSG(pParent != NULL,wxT("Cannot get parent window"));
  wxPanel *pPanel = new wxPanel(pParent);
  CPanelSampleAlertDetails *pSub = CreateSubPanel(pPanel);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pSub,1,wxEXPAND | wxALL, 8);
  pPanel->SetSizer(pSizer);
  pSizer->Layout();
  return pPanel;
#endif
}
CPanelSampleAlertDetails *CPageEditSampleAlerts::GetSubPanel()
{
  CPanelSampleAlertDetails *pRtn = wxDynamicCast(GetPanel(),CPanelSampleAlertDetails);
  return pRtn;
}

CPanelLocusDetails *CPageEditLocus::GetLocusPanel()
{
  CPanelLocusDetails *pRtn = wxDynamicCast(GetPanel(),CPanelLocusDetails);
  return pRtn;
}

wxWindow *CPageEditLocus::CreatePanel(wxWindow *parent)
{
  CPanelLocusDetails *pRtn = new CPanelLocusDetails(
    GetSample(), m_nChannel,
    m_pLocus,m_pFile->GetMessages(),
    parent,wxID_ANY,true,true,false);
  return pRtn;
}
//
//  END CreatePanel
//  BEGIN NeedsAcceptance, NeedsReview, HasHistory
//

//
//  CEditAlertsDir
//
bool CEditAlertsDir::NeedsAcceptance()
{
  bool bRtn = m_pFile->NeedDirAcceptance();
  return bRtn;
}
bool CEditAlertsDir::NeedsReview()
{
  bool bRtn = m_pFile->NeedDirReview();
  return bRtn;
}
bool CEditAlertsDir::HasHistory()
{
  bool bRtn = m_pFile->IsDirMessageEdited();
  return bRtn;
}
//
//  CEditAlertsSample
//
bool CEditAlertsSample::NeedsAcceptance()
{
  bool bRtn = GetSample()->NeedSampleAcceptance(m_nAcceptanceCount,NULL)
    && !NeedsApply();
  return bRtn;
}
bool CEditAlertsSample::NeedsReview()
{
  bool bRtn = GetSample()->NeedSampleReview(m_nReviewerCount,NULL)
    && !NeedsApply();
  return bRtn;
}
bool CEditAlertsSample::HasHistory()
{
  bool bRtn = GetSample()->IsSampleLevelEdited();
  return bRtn;
}
//
//  CEditAlertsILS
//
bool CEditAlertsILS::NeedsAcceptance()
{
  bool bRtn = GetSample()->NeedILSAcceptance(m_nAcceptanceCount,NULL) &&
      !NeedsApply();
  return bRtn;
}
bool CEditAlertsILS::NeedsReview()
{
  bool bRtn = GetSample()->NeedILSReview(m_nReviewerCount,NULL) &&
      !NeedsApply();
  return bRtn;
}
bool CEditAlertsILS::HasHistory()
{
  bool bRtn = GetSample()->IsCellILSEdited();
  return bRtn;
}
//
//  CEditAlertsChannel
//
bool CEditAlertsChannel::NeedsAcceptance()
{
  bool bRtn = GetSample()->NeedChannelAcceptance(m_nAcceptanceCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CEditAlertsChannel::NeedsReview()
{
  bool bRtn = GetSample()->NeedChannelReview(m_nReviewerCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CEditAlertsChannel::HasHistory()
{
  bool bRtn = GetSample()->IsCellChannelEdited();
  return bRtn;
}
//
//  CPageEditLocus
//
bool CPageEditLocus::NeedsAcceptance()
{
  bool bRtn = m_pLocus->NeedAcceptance(m_nAcceptanceCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CPageEditLocus::NeedsReview()
{
  bool bRtn = m_pLocus->NeedReview(m_nReviewerCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CPageEditLocus::HasHistory()
{
  bool bRtn = m_pLocus->HasBeenEdited(m_pFile->GetMessages(),GetSample(),m_nChannel);
  return bRtn;
}
//  END NeedsAcceptance, NeedsReview, HasHistory
//  BEGIN CreateReviewReceiver

IAppendReview *CEditAlertsDir::CreateReviewReceiver()
{
  return new COARfileDirReview(m_pFile);
}
IAppendReview *CEditAlertsSample::CreateReviewReceiver()
{
  return new COARSampleAppendReview(GetSample());
}
IAppendReview *CEditAlertsILS::CreateReviewReceiver()
{
  return new COARsampleILSAppendReview(GetSample());
}
IAppendReview *CEditAlertsChannel::CreateReviewReceiver()
{
  return new COARsampleChannelAppendReview(GetSample());
}
IAppendReview *CPageEditLocus::CreateReviewReceiver()
{
  return new CAppendReviewLocus(m_pLocus);
}

//  END CreateReviewReceiver
//  BEGIN CreateAcceptReceiver

IAppendReview *CEditAlertsDir::CreateAcceptReceiver()
{
  return new COARfileDirAccept(m_pFile);
}
IAppendReview *CEditAlertsSample::CreateAcceptReceiver()
{
  return new COARSampleAppendAccept(GetSample());
}
IAppendReview *CEditAlertsILS::CreateAcceptReceiver()
{
  return new COARsampleILSAppendAccept(GetSample());
}
IAppendReview *CEditAlertsChannel::CreateAcceptReceiver()
{
  return new COARsampleChannelAppendAccept(GetSample());
}
IAppendReview *CPageEditLocus::CreateAcceptReceiver()
{
  return new CAppendAcceptanceLocus(m_pLocus);
}
//  END CreateAcceptReceiver
//  BEGIN CreateDisableReceiver

IAppendReview *CPageEditSample::CreateDisableReceiver()
{
  COARsample *pSample = GetSample();
  IAppendReview *pRtn = 
    (pSample == NULL)
    ? NULL
    : new COARSampleToggleEnabled(pSample);
  return pRtn;
}
//  END CreateDisableReceiver
//  BEGIN UpdateNotes

void CEditAlertsDir::UpdateNotes()
{
  m_pFile->AppendNotesDir(GetNewNotes(),GetUserID());
}
void CEditAlertsSample::UpdateNotes()
{
  GetSample()->AppendNotesSample(GetNewNotes(),GetUserID());
}
void CEditAlertsILS::UpdateNotes()
{
  GetSample()->AppendNotesILS(GetNewNotes(), GetUserID());
}
void CEditAlertsChannel::UpdateNotes()
{
  GetSample()->AppendNotesChannel(GetNewNotes(),GetUserID());
}
//  END UpdateNotes
//  BEGIN GetNotes
const COARnotes *CEditAlertsDir::GetNotes()
{
  return m_pFile->GetNotesDir();
}
const COARnotes *CEditAlertsSample::GetNotes()
{
  return GetSample()->GetNotesSample();
}
const COARnotes *CEditAlertsILS::GetNotes()
{
  return GetSample()->GetNotesILS();
}
const COARnotes *CEditAlertsChannel::GetNotes()
{
  return GetSample()->GetNotesChannel();
}
//  END GetNotes
//  BEGIN GetReviewAcceptance() - notes to display for past review and acceptance

#define APPEND_STRINGS(s1,s2) \
  if(!s2.IsEmpty())           \
  { if(!s1.IsEmpty())         \
    { s1.Append(wxT("\n")); } \
    s1.Append(s2);            \
  }

wxString CEditAlertsDir::GetReviewAcceptance()
{
  wxString s1 = m_pFile->FormatReviewAcceptance();
  wxString s2 = m_pFile->CheckDirStatus();
  nwxString::Append(&s1,s2);
  return s1;
}
wxString CEditAlertsSample::GetReviewAcceptance()
{
  COARsample *pSample = GetSample();
  wxString s1 = pSample->FormatSampleReviewAcceptance(NULL);
  wxString s2 = m_pFile->CheckSampleStatus(pSample);
  nwxString::Append(&s1,s2);
  return s1;
}
wxString CEditAlertsILS::GetReviewAcceptance()
{
  COARsample *pSample = GetSample();
  wxString s1 = pSample->FormatILSReviewAcceptance(NULL);
  wxString s2 = m_pFile->CheckILSStatus(pSample);
  nwxString::Append(&s1,s2);
  return s1;
}
wxString CEditAlertsChannel::GetReviewAcceptance()
{
  COARsample *pSample = GetSample();
  wxString s1 = pSample->FormatChannelReviewAcceptance(NULL);
  wxString s2 = m_pFile->CheckChannelStatus(pSample);
  nwxString::Append(&s1,s2);
  return s1;
}
//  END GetReviewAcceptance()
//  BEGIN UpdateMessages
void CEditAlertsDir::UpdateMessages()
{
  m_vsLocus.clear();
  m_pFile->GetDirectoryAlerts()->BuildMessageList(&m_Msg,&m_vsLocus,*m_pFile->GetMessages());
}
void CEditAlertsSample::UpdateMessages()
{
  m_Msg.CopyOnly(*m_pFile->GetMessages(),GetSample()->GetSampleAlerts()->Get());
}
void CEditAlertsILS::UpdateMessages()
{
  m_Msg.CopyOnly(*m_pFile->GetMessages(),GetSample()->GetILSAlerts()->Get());
}
void CEditAlertsChannel::UpdateMessages()
{
  vector<int> vn;
  GetSample()->AppendChannelAlerts(&vn);
  m_Msg.CopyOnly(*m_pFile->GetMessages(),&vn);
}
//  END UpdateMessages

// CEditAlertsDir SyncTo - sync changes to other
//   instances of this class

void CEditAlertsDir::SyncTo(CEditAlertsDir *pFromThis)
{
  CPanelSampleAlertDetails *pPanel = GetSubPanel();
  CPanelSampleAlertDetails *pPanelFrom = pFromThis->GetSubPanel();
  pPanel->CopyState(pPanelFrom);
  m_pParentNotebook->UpdateDirPage();
}

//  class CEditAlertsDir::CDirPanelSet - container
//   for instances of CEditAlertsDir for syncing
//   all instances from the same file

CEditAlertsDir::CDirPanelSet::MAP_ALL
  CEditAlertsDir::CDirPanelSet::g_mapAll;

void CEditAlertsDir::CDirPanelSet::Add(CEditAlertsDir *pPanel)
{
  MAP_ALL_ITERATOR itrMap = _Find(pPanel);
  SET_PANEL *pSet = NULL;
  if(itrMap == g_mapAll.end())
  {
    pSet = new SET_PANEL;
    g_mapAll.insert(
      MAP_ALL::value_type(pPanel->GetFile(),pSet));
  }
  else
  {
    pSet = itrMap->second;
  }
  if(pSet->find(pPanel) == pSet->end())
  {
    pSet->insert(pPanel);
  }
}
void CEditAlertsDir::CDirPanelSet::Remove(CEditAlertsDir *pPanel)
{
  MAP_ALL_ITERATOR itrMap = _Find(pPanel);
  if(itrMap != g_mapAll.end())
  {
    SET_PANEL *pSet = itrMap->second;
    SET_PANEL::iterator itr = pSet->find(pPanel);
    if(itr != pSet->end())
    {
      pSet->erase(itr);
      if(pSet->size() == 0)
      {
        delete pSet;
        g_mapAll.erase(itrMap);
      }
    }
  }
}
void CEditAlertsDir::CDirPanelSet::SyncAllTo(CEditAlertsDir *pPanel)
{
  MAP_ALL_ITERATOR itrMap = _Find(pPanel);
  if(itrMap != g_mapAll.end())
  {
    SET_PANEL *pSet = itrMap->second;
    for(SET_PANEL::iterator itr = pSet->begin();
      itr != pSet->end();
      ++itr)
    {
      if(pPanel != *itr)
      {
        (*itr)->SyncTo(pPanel);
      }
    }
  }
}


//  END CEditAlertsDir, CEditAlertsSample, CEditAlertsILS, CEditAlertsChannel



//  CPageEditSampleAlerts, CPageEditLocus
//  NeedsApply, DoApply, TransferDataToPage

bool CPageEditSampleAlerts::NeedsApply()
{
  // appropriate only if m_pPanel is CPanelSampleAlertDetails
  bool bRtn = false;
  CPanelSampleAlertDetails *pPanel = GetSubPanel();
  if(pPanel == NULL)
  {}
  else if(pPanel->IsNotesModified())
  {
    bRtn = true;
  }
  else
  { 
    bRtn = m_Msg.IsModified(*m_pFile->GetMessages());
  }
  return bRtn;
}

bool CPageEditSampleAlerts::DoApply()
{
  // updates notes and notices, needs to be overridden for locus
  bool bRtn = false;
  if(NeedsApply())
  {
    wxDateTime t;
    t.SetToCurrent();
    m_pFile->UpdateMessages(m_Msg,t);
    DoReview();
    UpdateNotes();
    m_pFile->SetIsModified(true);
    m_pParentNotebook->InitiateRepaintData();
    bRtn = true;
  }
  return bRtn;
}
  // appropriate only if m_pPanel is CPanelSampleAlertDetails
bool CPageEditSampleAlerts::TransferDataToPage()
{
  CPanelSampleAlertDetails *pPanel = GetSubPanel();
  bool bRtn = (pPanel != NULL);
  if(bRtn)
  {
    UpdateMessages();
    wxString s = GetReviewAcceptance();
    const wxString &sNotes(GetNotesText());
    nwxString::Append(&s,sNotes,CLabels::NOTES_AFTER_REVIEW);
    pPanel->SetNotesText(s);
    pPanel->ClearNewNotes();
    bRtn = pPanel->TransferDataToWindow();
  }
  return bRtn;
}

//  CPageEditSampleAlerts, CPageEditLocus
//  SetReadOnly, CheckReadOnly

void CPageEditSampleAlerts::SetReadOnly(bool b)
{
  if(b != IsReadOnly())
  {
    _SetReadOnlyRaw(b);
    CPanelSampleAlertDetails *pPanel = GetSubPanel();
    if(pPanel != NULL)
    {
      pPanel->SetReadOnly(b);
    }
  }
}
void CPageEditLocus::SetReadOnly(bool b)
{
  if(b != IsReadOnly())
  {
    _SetReadOnlyRaw(b);
    CPanelLocusDetails *pPanel = GetLocusPanel();
    if(pPanel != NULL)
    {
      pPanel->SetReadOnly(b);
    }
  }
}

// GetNewNotes

const wxString &CPageEditSampleAlerts::GetNewNotes()
{
  CPanelSampleAlertDetails *pPanel = GetSubPanel();
  if(pPanel == NULL)
  {
    m_sNotes.Empty();
  }
  else
  {
    m_sNotes = pPanel->GetNewNotesValue();
  }
  return m_sNotes;
}
const wxString &CPageEditLocus::GetNewNotes()
{
  CPanelLocusDetails *pPanel = GetLocusPanel();
  if(pPanel == NULL)
  {
    m_sNotes.Empty();
  }
  else
  {
    m_sNotes = pPanel->GetNewNotesValue();
  }
  return m_sNotes;
}


bool CPageEditLocus::NeedsApply()
{
  CPanelLocusDetails *pWindow = GetLocusPanel();
  bool bRtn = pWindow->IsModified();
  return bRtn;
}
bool CPageEditLocus::DoApply()
{
  CPanelLocusDetails *pWindow = GetLocusPanel();
  bool bRtn = false;
  if((pWindow != NULL) && pWindow->IsModified())
  {
    pWindow->UpdateFile(m_pFile,GetSample(),GetUserID());
    //DoReview();
    m_pFile->SetIsModified(true);
    bRtn = true;
  }
  return bRtn;
}
bool CPageEditLocus::TransferDataToPage()
{
  return GetPanel()->TransferDataToWindow();
}

