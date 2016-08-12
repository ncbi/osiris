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



//
// BEGIN CPageEditSample
//
CPageEditSample::~CPageEditSample()
{
  if(m_pReview != NULL) { delete m_pReview; }
  if(m_pAccept != NULL) { delete m_pAccept; }
}
wxWindow *CPageEditSample::GetParentWindow()
{
  return m_pParent->GetNotebookWindow();
}
const wxString &CPageEditSample::GetUserID()
{
  return m_pParent->GetUserID();
}
bool CPageEditSample::DoAccept()
{
  bool bRtn = false;
  if(m_pAccept == NULL)
  {
    m_pAccept = CreateAcceptReceiver();
  }
  if((m_pAccept != NULL) && m_pAccept->AppendReview(GetUserID()))
  {
    bRtn = true; 
    m_pParent->InitiateRepaintData();
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
    m_pParent->InitiateRepaintData();
  }
  return bRtn;
}
wxWindow *CPageEditSample::GetPanel()
{
  if(m_pPanel == NULL)
  {
    m_pPanel = CreatePanel();
  }
  return m_pPanel;
}


//
//  END CPageEditSample
//  BEGIN CEditAlertsDir, CEditAlertsSample, CEditAlertsILS, CEditAlertsChannel, CPageEditLocus
//
//  BEGIN CreatePanel
//
wxWindow *CEditAlertsDir::CreatePanel()
{
  vector<wxString> vsLocus;
  m_pFile->GetDirectoryAlerts()->BuildMessageList(&m_Msg,&vsLocus,*m_pFile->GetMessages());
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
    GetParentWindow(),
    &m_Msg,
    GetLabelPrefix(),
    CGridAlerts::TYPE_LOCUS,
    true,
    IsReadOnly());
  pPanel->SetupLocusColumn(vsLocus);
  return pPanel;
}
wxWindow *CEditAlertsSample::CreatePanel()
{
  m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetSampleAlerts()->Get());
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      GetParentWindow(),
      &m_Msg,
      GetLabelPrefix(),
      0,
      true,
      IsReadOnly());
  return pPanel;
}
wxWindow *CEditAlertsILS::CreatePanel()
{
  m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetILSAlerts()->Get());
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      GetParentWindow(),
      &m_Msg,
      GetLabelPrefix(),
      0,
      true,
      IsReadOnly());
  return pPanel;
}
wxWindow *CEditAlertsChannel::CreatePanel()
{
  m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetILSAlerts()->Get());
  CPanelSampleAlertDetails *pPanel = new CPanelSampleAlertDetails(
      GetParentWindow(),
      &m_Msg,
      GetLabelPrefix(),
      CGridAlerts::TYPE_CHANNEL,
      true,
      IsReadOnly());
  pPanel->SetupChannelColumn(m_pSample,m_pmapChannelNames);
  return pPanel;
}
wxWindow *CPageEditLocus::CreatePanel()
{
  CPanelLocusDetails *pRtn = new CPanelLocusDetails(
    m_pSample, m_nChannel,
    m_pLocus,m_pFile->GetMessages(),
    GetParentWindow(),wxID_ANY,true,true,false);
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
  bool bRtn = m_pSample->NeedSampleAcceptance(m_nAcceptanceCount,NULL)
    && !NeedsApply();
  return bRtn;
}
bool CEditAlertsSample::NeedsReview()
{
  bool bRtn = m_pSample->NeedSampleReview(m_nReviewerCount,NULL)
    && !NeedsApply();
  return bRtn;
}
bool CEditAlertsSample::HasHistory()
{
  bool bRtn = m_pSample->IsSampleLevelEdited();
  return bRtn;
}
//
//  CEditAlertsILS
//
bool CEditAlertsILS::NeedsAcceptance()
{
  bool bRtn = m_pSample->NeedILSAcceptance(m_nAcceptanceCount,NULL) &&
      !NeedsApply();
  return bRtn;
}
bool CEditAlertsILS::NeedsReview()
{
  bool bRtn = m_pSample->NeedILSReview(m_nReviewerCount,NULL) &&
      !NeedsApply();
  return bRtn;
}
bool CEditAlertsILS::HasHistory()
{
  bool bRtn = m_pSample->IsCellILSEdited();
  return bRtn;
}
//
//  CEditAlertsChannel
//
bool CEditAlertsChannel::NeedsAcceptance()
{
  bool bRtn = m_pSample->NeedChannelAcceptance(m_nAcceptanceCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CEditAlertsChannel::NeedsReview()
{
  bool bRtn = m_pSample->NeedChannelReview(m_nReviewerCount,NULL) &&
    !NeedsApply();
  return bRtn;
}
bool CEditAlertsChannel::HasHistory()
{
  bool bRtn = m_pSample->IsCellChannelEdited();
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
  bool bRtn = m_pLocus->HasBeenEdited(m_pFile->GetMessages(),m_pSample,m_nChannel);
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
  return new COARSampleAppendReview(m_pSample);
}
IAppendReview *CEditAlertsILS::CreateReviewReceiver()
{
  return new COARsampleILSAppendReview(m_pSample);
}
IAppendReview *CEditAlertsChannel::CreateReviewReceiver()
{
  return new COARsampleChannelAppendReview(m_pSample);
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
  return new COARSampleAppendAccept(m_pSample);
}
IAppendReview *CEditAlertsILS::CreateAcceptReceiver()
{
  return new COARsampleILSAppendAccept(m_pSample);
}
IAppendReview *CEditAlertsChannel::CreateAcceptReceiver()
{
  return new COARsampleChannelAppendAccept(m_pSample);
}
IAppendReview *CPageEditLocus::CreateAcceptReceiver()
{
  return new CAppendAcceptanceLocus(m_pLocus);
}
//  END CreateAcceptReceiver
//  BEGIN UpdateNotes

void CEditAlertsDir::UpdateNotes()
{
  m_pFile->AppendNotesDir(GetNewNotes(),GetUserID());
}
void CEditAlertsSample::UpdateNotes()
{
  m_pSample->AppendNotesSample(GetNewNotes(),GetUserID());
}
void CEditAlertsILS::UpdateNotes()
{
  m_pSample->AppendNotesILS(GetNewNotes(), GetUserID());
}
void CEditAlertsChannel::UpdateNotes()
{
  m_pSample->AppendNotesChannel(GetNewNotes(),GetUserID());
}
//  END UpdateNotes
//  BEGIN GetNotes
const COARnotes *CEditAlertsDir::GetNotes()
{
  return m_pFile->GetNotesDir();
}
const COARnotes *CEditAlertsSample::GetNotes()
{
  return m_pSample->GetNotesSample();
}
const COARnotes *CEditAlertsILS::GetNotes()
{
  return m_pSample->GetNotesILS();
}
const COARnotes *CEditAlertsChannel::GetNotes()
{
  return m_pSample->GetNotesChannel();
}
//  END GetNotes
//  BEGIN SetupPageLabel
void CEditAlertsDir::SetupPageLabel(wxString *ps)
{
}
void CEditAlertsDir::SetupPageLabel(wxString *ps)
{
}
void CEditAlertsDir::SetupPageLabel(wxString *ps)
{
}
void CEditAlertsDir::SetupPageLabel(wxString *ps)
{
}
void CEditAlertsDir::SetupPageLabel(wxString *ps)
{
}
//  END SetupPageLabel
//  BEGIN GetReviewAcceptance() - notes to display for past review and acceptance

wxString CEditAlertsDir::GetReviewAcceptance()
{
  return m_pFile->FormatReviewAcceptance();
}
wxString CEditAlertsSample::GetReviewAcceptance()
{
  return m_pSample->FormatSampleReviewAcceptance(NULL);
}
wxString CEditAlertsILS::GetReviewAcceptance()
{
  return m_pSample->FormatILSReviewAcceptance(NULL);
}
wxString CEditAlertsChannel::GetReviewAcceptance()
{
  return m_pSample->FormatChannelReviewAcceptance(NULL);
}
//  END CEditAlertsDir, CEditAlertsSample, CEditAlertsILS, CEditAlertsChannel

const wxString &CPageEditSampleAlerts::GetNewNotes()
{
  CPanelSampleAlertDetails *pPanel = wxDynamicCast(GetPanel(),CPanelSampleAlertDetails);
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


//  CPageEditSampleAlerts, CPageLocus
//  NeedsApply, DoApply, TransferDataToPage


bool CPageEditSampleAlerts::NeedsApply()
{
  // appropriate only if m_pPanel is CPanelSampleAlertDetails
  bool bRtn = false;
  CPanelSampleAlertDetails *pPanel = wxDynamicCast(GetPanel(),CPanelSampleAlertDetails);
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

void CPageEditSampleAlerts::DoApply()
{
  // updates notes and notices, needs to be overridden for locus
  if(NeedsApply())
  {
    wxDateTime t;
    t.SetToCurrent();
    m_pFile->UpdateMessages(m_Msg,t);
    DoReview();
    UpdateNotes();
    m_pFile->SetIsModified(true);
  }
}
  // appropriate only if m_pPanel is CPanelSampleAlertDetails
bool CPageEditSampleAlerts::TransferDataToPage()
{
  CPanelSampleAlertDetails *pPanel = wxDynamicCast(GetPanel(),CPanelSampleAlertDetails);
  bool bRtn = (pPanel != NULL);
  if(bRtn)
  {
    wxString s = GetReviewAcceptance();
    const wxString &sNotes(GetNotesText());
    if(!sNotes.IsEmpty())
    {
      if(!s.IsEmpty())
      {
        s.Append(CLabels::NOTES_AFTER_REVIEW);
      }
      s.Append(sNotes);
    }
    pPanel->SetNotesText(s);
  }
  return bRtn;
}


bool CPageEditLocus::NeedsApply()
{
  CPanelLocusDetails *pWindow = wxDynamicCast(GetPanel(),CPanelLocusDetails);
  bool bRtn = pWindow->IsModified();
  return bRtn;
}
void CPageEditLocus::DoApply()
{
  CPanelLocusDetails *pWindow = wxDynamicCast(GetPanel(),CPanelLocusDetails);
  if((pWindow != NULL) && pWindow->IsModified())
  {
    pWindow->UpdateFile(m_pFile,m_pSample);
    DoReview();
    m_pFile->SetIsModified(true);
  }
}
bool CPageEditLocus::TransferDataToPage()
{
  return GetPanel()->TransferDataToWindow();
}

