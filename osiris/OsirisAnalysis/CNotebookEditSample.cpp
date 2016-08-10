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
*  FileName: CNotebookEditSample.h
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CNotebookEditSample.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"
#include "nwx/stdb.h"
#include <vector>
#include <map>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxString.h"
#include "CPanelUserID.h"
#include "CPanelLocusDetails.h"
#include "CLabels.h"
#include "CFrameSample.h"

#define S_DIR "Directory "
#define S_SAMPLE "Sample "
#define S_ILS "ILS "
#define S_CHANNEL "Channel "
#define S_NOTES "Notes"
#define COAR_NOTICE_DISPLAY_CAP "Notices"


//
//  BEGIN CEditAlertsBase
//
CEditAlertsBase::~CEditAlertsBase()
{
  if(m_pReview != NULL) { delete m_pReview; }
  if(m_pAccept != NULL) { delete m_pAccept; }
}
wxWindow *CEditAlertsBase::GetParentWindow()
{
  return m_pParent->GetNotebookWindow();
}
bool CEditAlertsBase::NeedsApply()
{
  GetPanel();
  bool bRtn = m_pPanel->IsNotesModified();
  if(!bRtn)
  { bRtn = m_Msg.IsModified(*m_pFile->GetMessages());
  }
  return bRtn;
}
void CEditAlertsBase::DoAccept()
{
  if(GetAcceptReceiver()->AppendReview(m_pParent->GetUserID()))
  {
    m_pParent->InitiateRepaintData();
  }
}
void CEditAlertsBase::DoReview()
{
  if(GetReviewReceiver()->AppendReview(m_pParent->GetUserID()))
  {
    m_pParent->InitiateRepaintData();
  }
}
//
//  END CEditAlertsBase
//  BEGIN CEditAlertsDir
//


wxWindow *CEditAlertsDir::GetPanel()
{
  if(m_pPanel == NULL)
  {
    vector<wxString> vsLocus;
    m_pFile->GetDirectoryAlerts()->BuildMessageList(&m_Msg,&vsLocus,*m_pFile->GetMessages());
    m_pPanel = new CPanelSampleAlertDetails(
      GetParentWindow(),
      &m_Msg,
      wxString(S_DIR COAR_NOTICE_DISPLAY_CAP),
      CGridAlerts::TYPE_LOCUS,
      true,
      IsReadOnly());
    m_pPanel->SetupLocusColumn(vsLocus);
  }
  return m_pPanel;
}
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
//  END CEditAlertsDir
//  BEGIN CEditAlertsSample
//
wxWindow *CEditAlertsSample::GetPanel()
{
  if(m_pPanel == NULL)
  {
    m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetSampleAlerts()->Get());
    m_pPanel = new CPanelSampleAlertDetails(
        GetParentWindow(),
        &m_Msg,
        wxString(S_SAMPLE COAR_NOTICE_DISPLAY_CAP),
        0,
        true,
        IsReadOnly());
  }
  return m_pPanel;
}
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
//  END CEditAlertsSample
//  BEGIN CEditAlertsILS
//
wxWindow *CEditAlertsILS::GetPanel()
{
  if(m_pPanel == NULL)
  {
    m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetILSAlerts()->Get());
    m_pPanel = new CPanelSampleAlertDetails(
        GetParentWindow(),
        &m_Msg,
        wxString(S_ILS COAR_NOTICE_DISPLAY_CAP),
        0,
        true,
        IsReadOnly());
  }
  return m_pPanel;
}
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
//  END CEditAlertsILS
//  BEGIN CEditAlertsChannel
//
wxWindow *CEditAlertsChannel::GetPanel()
{
  if(m_pPanel == NULL)
  {
    m_Msg.CopyOnly(*m_pFile->GetMessages(),m_pSample->GetILSAlerts()->Get());
    m_pPanel = new CPanelSampleAlertDetails(
        GetParentWindow(),
        &m_Msg,
        wxString(S_CHANNEL COAR_NOTICE_DISPLAY_CAP),
        CGridAlerts::TYPE_CHANNEL,
        true,
        IsReadOnly());

  }
  return m_pPanel;
}
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
//  END CEditAlertsChannel
//

const wxString CNotebookEditSample::g_sLabelDirNotices(S_DIR COAR_NOTICE_DISPLAY_CAP);
const wxString CNotebookEditSample::g_sLabelSampleNotices(S_SAMPLE COAR_NOTICE_DISPLAY_CAP);
const wxString CNotebookEditSample::g_sLabelILSNotices(S_ILS COAR_NOTICE_DISPLAY_CAP);
const wxString CNotebookEditSample::g_sLabelChannelNotices(S_CHANNEL COAR_NOTICE_DISPLAY_CAP);

const wxString CNotebookEditSample::g_sLabelDir(S_DIR);
const wxString CNotebookEditSample::g_sLabelSample(S_SAMPLE);
const wxString CNotebookEditSample::g_sLabelILS(S_ILS);
const wxString CNotebookEditSample::g_sLabelChannel(S_CHANNEL);

const wxString * const CNotebookEditSample::g_psLabels[SA_WINDOW_COUNT] =
{
  &CNotebookEditSample::g_sLabelDirNotices,
  &CNotebookEditSample::g_sLabelSampleNotices,
  &CNotebookEditSample::g_sLabelILSNotices,
  &CNotebookEditSample::g_sLabelChannelNotices
};

const wxString * const CNotebookEditSample::g_psNotesLabels[SA_WINDOW_COUNT] =
{
  &CNotebookEditSample::g_sLabelDir,
  &CNotebookEditSample::g_sLabelSample,
  &CNotebookEditSample::g_sLabelILS,
  &CNotebookEditSample::g_sLabelChannel
};

CNotebookEditSample::~CNotebookEditSample() {;}

CNotebookEditSample::CNotebookEditSample(
  const COARfile *pFile,
  const COARsample *pSample,
  wxWindow *parent,
  wxWindowID id,
  const map<int,wxString> *pmapChannelNames) :
    wxPanel(parent,id),
    m_pSample(pSample),
    m_pFile(pFile),
    m_bReadOnly(false)
{
  vector<wxString> vsLocus;
  vector<int> vn;
  // set up notes
  COARmessages *pMsgEdit[] = 
  {
    &m_MsgDir,
    &m_MsgSample,
    &m_MsgILS,
    &m_MsgChannel
  };

  //  set up sample alert details

  CPanelSampleAlertDetails *pSplitter;
  wxBoxSizer *pSizer;
  int i;

  pSample->AppendChannelAlerts(&vn);
  pFile->GetDirectoryAlerts()->BuildMessageList(&m_MsgDir,&vsLocus,*pFile->GetMessages());
  m_MsgSample.CopyOnly(*pFile->GetMessages(),pSample->GetSampleAlerts()->Get());
  m_MsgILS.CopyOnly(*pFile->GetMessages(),pSample->GetILSAlerts()->Get());
  m_MsgChannel.CopyOnly(*pFile->GetMessages(),&vn);

  m_pNotebook = new wxTreebook(
    this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxNB_LEFT);
  int nType;
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    switch(i)
    {
    case SA_NDX_CHANNEL:
      nType = CGridAlerts::TYPE_CHANNEL;
      break;
    case SA_NDX_DIR:
      nType = CGridAlerts::TYPE_LOCUS;
      break;
    default:
      nType = 0;
      break;
    }
    pSplitter = new CPanelSampleAlertDetails(
      m_pNotebook,
      pMsgEdit[i],
      *g_psNotesLabels[i],
      nType,
      true,
      false);

    m_pSplitter[i] = pSplitter;
    m_pNotebook->AddPage(pSplitter,*g_psLabels[i],!i);
  }
  
  if((pSplitter = m_pSplitter[SA_NDX_CHANNEL]) != NULL)
  {
    pSplitter->SetupChannelColumn(
      pSample,pmapChannelNames);
  }
  if((pSplitter = m_pSplitter[SA_NDX_DIR]) != NULL)
  {
    pSplitter->SetupLocusColumn(vsLocus);
  }

  //****************************************************************
  // done with sample alert details, now set up loci
  //****************************************************************
  size_t nChannelCount = m_pFile->GetChannelCount();
  size_t j,k,nLocusCount;
  const COARchannel *pChannel;
  const COARlocus *pLocus;
  for(k = 1; k <= nChannelCount; ++k)
  {
    pChannel = m_pFile->GetChannelByNr(k);
    nLocusCount = pChannel->GetLocusCount();
    for(j = 0; j < nLocusCount; ++j)
    {
      const wxString &sLocusName = pChannel->GetLocusName(j);
      m_asLocus.push_back(sLocusName);
      pLocus = pSample->FindLocus(sLocusName);
      CPanelLocusDetails *pPanelLocus = new CPanelLocusDetails(
        pSample, (int) k,
        pLocus, m_pFile->GetMessages(),
        m_pNotebook,wxID_ANY,
        true, true, false);
      m_pNotebook->AddPage(pPanelLocus,sLocusName,false);
    }
  }
  // layout this

  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pNotebook,1,wxEXPAND);
  SetSizer(pSizer);
  pSizer->Layout();

  //  split alert editing panels (non locus)
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    pSplitter = m_pSplitter[i];
    if(pSplitter != NULL)
    {
      pSplitter->Split();
    }
  }
}
const wxString &CNotebookEditSample::GetCurrentLocus()
{
  int nSelect = m_pNotebook->GetSelection() - SA_WINDOW_COUNT;
  const wxString *pRtn = NULL;
  if((nSelect < 0) ||
    (nSelect >= (int)m_asLocus.size())
    )
  {
    pRtn = &mainApp::EMPTY_STRING;
  }
  else
  {
    pRtn = &(m_asLocus.at((size_t)nSelect));
  }
  return *pRtn;
}

bool CNotebookEditSample::Validate()
{
  bool bRtn = true;
  if(!m_bReadOnly)
  {
    wxString sMessage;
    const wxChar *LF(wxS("\n"));
    int nCount = 0;
    int nPage = -1;
    for(int i = 0; i < SA_WINDOW_COUNT; i++)
    {
      if(IsAlertsModified(i) && !IsNotesModified(i))
      {
        sMessage += "You haven't entered notes for ";
        sMessage += *g_psLabels[i];
        sMessage += LF;
        nCount++;
        if(nPage < 0)
        {
          nPage = i;
        }
      }
    }
    if(nPage >= 0)
    {
      if(nCount > 1)
      {
        sMessage += LF;
      }
      sMessage += "Do you wish to continue?";
      bRtn = mainApp::Confirm(this,sMessage);
      if(!bRtn)
      {
        Select(nPage);
      }
    }
  }
  return bRtn;
}

const COARnotes *CNotebookEditSample::_GetNotes(
  int n, const wxDateTime *pdt)
{
  const COARnotes *pRtn(NULL);
  switch(n)
  {
    case SA_NDX_SAMPLE:
      pRtn = m_pSample->GetNotesSample(pdt);
      break;
    case SA_NDX_ILS:
      pRtn = m_pSample->GetNotesILS(pdt);
      break;
    case SA_NDX_CHANNEL:
      pRtn = m_pSample->GetNotesChannel(pdt);
      break;
    case SA_NDX_DIR:
      pRtn = m_pFile->GetNotesDir(pdt);
      break;
  }
  return pRtn;
}
wxString CNotebookEditSample::_GetReviewAcceptance(int n, const wxDateTime *pdt)
{
  wxString sRtn;
  switch(n)
  {
    case SA_NDX_SAMPLE:
      sRtn = m_pSample->FormatSampleReviewAcceptance(pdt);
      break;
    case SA_NDX_ILS:
      sRtn = m_pSample->FormatILSReviewAcceptance(pdt);
      break;
    case SA_NDX_CHANNEL:
      sRtn = m_pSample->FormatChannelReviewAcceptance(pdt);
      break;
    case SA_NDX_DIR:
      sRtn = m_pFile->FormatReviewAcceptance(pdt);
      break;
  }
  return sRtn;
}

bool CNotebookEditSample::TransferDataToWindow()
{
  for(int i = 0; i < SA_WINDOW_COUNT; i++)
  {
    CPanelSampleAlertDetails *psd = m_pSplitter[i];
    if(psd != NULL)
    {
      wxString s = _GetReviewAcceptance(i,m_HistTime);
      const wxString &sNotes(_GetNotesText(i,m_HistTime));
      if(!sNotes.IsEmpty())
      {
        if(!s.IsEmpty())
        {
          s.Append(CLabels::NOTES_AFTER_REVIEW);
        }
        s.Append(sNotes);
      }
      psd->SetNotesText(s);
      psd->SetDateTime(m_HistTime,true);
    }
  }
  // STOP HERE transfer locus data
  size_t n,nCount = m_pNotebook->GetPageCount();
  CPanelLocusDetails *pPanel;
  for(n = (size_t) SA_WINDOW_COUNT; n < nCount; ++n)
  {
    pPanel = wxDynamicCast(m_pNotebook->GetPage(n),CPanelLocusDetails);
    wxASSERT_MSG(pPanel != NULL,"Invalid locus page in CNotebookEditSample");
    pPanel->SetDateTime(m_HistTime.GetDateTime());
    pPanel->TransferDataToWindow();
  }
  return true;
}

bool CNotebookEditSample::IsNotesModified(int n)
{
  CPanelSampleAlertDetails *psd;
  bool bRtn =
    !m_bReadOnly
    && ((psd = m_pSplitter[n]) != NULL)
    && psd->IsNotesModified();
  return bRtn;
}
bool CNotebookEditSample::IsAlertsModified(int n)
{
  bool bRtn = false;
  CPanelSampleAlertDetails *psd;
  if( (!m_bReadOnly) && ((psd = m_pSplitter[n]) != NULL) )
  {
    bRtn = psd->GetMsgEdit()->IsModified(*m_pFile->GetMessages());
  }
  return bRtn;
}

bool CNotebookEditSample::IsModified(int n)
{
  bool bRtn = false;
  if(!m_bReadOnly)
  {
    if(IsAlertsModified(n))
    {
      bRtn = true;
    }
    else if(IsNotesModified(n))
    {
      bRtn = true;
    }
  }
  return bRtn;
}

bool CNotebookEditSample::IsModified()
{
  bool bRtn = false;
  for(int i = 0; i < SA_WINDOW_COUNT; ++i)
  {
    if(IsModified(i))
    {
      bRtn = true;
      i = SA_WINDOW_COUNT; // loop exit
    }
  }
  return bRtn;
}


bool CNotebookEditSample::SetDateTime(const wxDateTime *pTime)
{
  bool bRtn = m_HistTime.SetDateTime(pTime);
  TransferDataToWindow();
  return bRtn;
}



void CNotebookEditSample::OnNotesChange(wxCommandEvent &e)
{
  if(!m_bReadOnly)
  {
    wxTextCtrl *p = (wxTextCtrl *)e.GetEventObject();
    CPanelSampleAlertDetails *psd;
    for(int i = 0; i < SA_WINDOW_COUNT; ++i)
    {
      psd = m_pSplitter[i];
      if( (psd != NULL) && psd->IsNotesCtrl(p) )
      {
        _ProcessEvent();
        i = SA_WINDOW_COUNT;
      }
    }
  }
}
void CNotebookEditSample::OnCellChange(wxGridEvent &)
{
  if(!m_bReadOnly)
  {
    _ProcessEvent();
  }
}

bool CNotebookEditSample::_ProcessEvent()
{
  wxCommandEvent eSend(wxEVT_COMMAND_ENTER ,GetId());
  eSend.SetEventObject(this);
  return ProcessEvent(eSend);
}
const wxString &CNotebookEditSample::GetUserID()
{
  CFrameSample *pFrame = wxDynamicCast(GetParent(),CFrameSample);
  if(pFrame != NULL)
  {
    m_sUserID = pFrame->GetUserID();
  }
  return m_sUserID;
}
void CNotebookEditSample::InitiateRepaintData()
{
  CFrameSample *pFrame = wxDynamicCast(GetParent(),CFrameSample);
  if(pFrame != NULL)
  {
    pFrame->InitiateRepaintData();
  }
}
void CNotebookEditSample::RepaintData()
{
  // STOP HERE - need to implement
}

BEGIN_EVENT_TABLE(CNotebookEditSample,wxPanel)
EVT_TEXT(wxID_ANY,CNotebookEditSample::OnNotesChange)
EVT_GRID_CMD_CELL_CHANGED(wxID_ANY,CNotebookEditSample::OnCellChange)
END_EVENT_TABLE()
