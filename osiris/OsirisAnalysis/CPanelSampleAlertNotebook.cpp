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
*  FileName: CPanelSampleAlertNotebook.h
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CPanelSampleAlertNotebook.h"
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
#include "CLabels.h"

#define S_DIR "Directory "
#define S_SAMPLE "Sample "
#define S_ILS "ILS "
#define S_CHANNEL "Channel "
#define S_NOTES "Notes"

const wxString CPanelSampleAlertNotebook::g_sLabelDirNotices(S_DIR COAR_NOTICE_DISPLAY_CAP);
const wxString CPanelSampleAlertNotebook::g_sLabelSampleNotices(S_SAMPLE COAR_NOTICE_DISPLAY_CAP);
const wxString CPanelSampleAlertNotebook::g_sLabelILSNotices(S_ILS COAR_NOTICE_DISPLAY_CAP);
const wxString CPanelSampleAlertNotebook::g_sLabelChannelNotices(S_CHANNEL COAR_NOTICE_DISPLAY_CAP);

const wxString CPanelSampleAlertNotebook::g_sLabelDir(S_DIR);
const wxString CPanelSampleAlertNotebook::g_sLabelSample(S_SAMPLE);
const wxString CPanelSampleAlertNotebook::g_sLabelILS(S_ILS);
const wxString CPanelSampleAlertNotebook::g_sLabelChannel(S_CHANNEL);

const wxString * const CPanelSampleAlertNotebook::g_psLabels[SA_WINDOW_COUNT] =
{
  &CPanelSampleAlertNotebook::g_sLabelDirNotices,
  &CPanelSampleAlertNotebook::g_sLabelSampleNotices,
  &CPanelSampleAlertNotebook::g_sLabelILSNotices,
  &CPanelSampleAlertNotebook::g_sLabelChannelNotices
};

const wxString * const CPanelSampleAlertNotebook::g_psNotesLabels[SA_WINDOW_COUNT] =
{
  &CPanelSampleAlertNotebook::g_sLabelDir,
  &CPanelSampleAlertNotebook::g_sLabelSample,
  &CPanelSampleAlertNotebook::g_sLabelILS,
  &CPanelSampleAlertNotebook::g_sLabelChannel
};

CPanelSampleAlertNotebook::~CPanelSampleAlertNotebook() {;}

CPanelSampleAlertNotebook::CPanelSampleAlertNotebook(
  const COARfile *pFile,
  const COARsample *pSample,
  wxWindow *parent,
  wxWindowID id,
  const map<int,wxString> *pmapChannelNames,
  bool bSplitHorizontal,
  bool bReadOnly,
  int nSelect,
  bool bShowOne) :
    wxPanel(parent,id),
    m_pSample(pSample),
    m_pFile(pFile),
    m_bReadOnly(bReadOnly)
{
  vector<wxString> vsLocus;
  vector<int> vn;
  bool bTabs[SA_WINDOW_COUNT];
  // set up notes
  COARmessages *pMsgEdit[] = 
  {
    &m_MsgDir,
    &m_MsgSample,
    &m_MsgILS,
    &m_MsgChannel
  };
  CPanelSampleAlertDetails *pSplitter;
  wxBoxSizer *pSizer;
  int i;

  if(bShowOne)
  {
    for(int i = 0; i < SA_WINDOW_COUNT; bTabs[i++] = false) {}
    switch(nSelect)
    {
    case SA_NDX_DIR:
      bTabs[SA_NDX_DIR] = pFile->IsDirMessageEdited();
      break;
    case SA_NDX_SAMPLE:
      bTabs[SA_NDX_SAMPLE] = pSample->IsSampleLevelEdited(pFile->GetMessages());
      break;
    case SA_NDX_ILS:
      bTabs[SA_NDX_ILS] = pSample->IsCellILSEdited(pFile->GetMessages());
      break;
    case SA_NDX_CHANNEL:
      bTabs[SA_NDX_CHANNEL] = pSample->IsCellChannelEdited(pFile->GetMessages());
      break;
    default:
      break;
    }
  }
  else if(m_bReadOnly)
  {
    bTabs[SA_NDX_DIR] = pFile->IsDirMessageEdited();
    bTabs[SA_NDX_SAMPLE] = pSample->IsSampleLevelEdited(pFile->GetMessages());
    bTabs[SA_NDX_ILS] = pSample->IsCellILSEdited(pFile->GetMessages());
    bTabs[SA_NDX_CHANNEL] = pSample->IsCellChannelEdited(pFile->GetMessages());
  }
  else
  {
    bool bEnabled = pSample->IsEnabled();
    for(int i = 0; i < SA_WINDOW_COUNT; bTabs[i++] = bEnabled) {}
    bTabs[SA_NDX_DIR] = true;
  }

  pSample->AppendChannelAlerts(&vn);
  pFile->GetDirectoryAlerts()->BuildMessageList(&m_MsgDir,&vsLocus,*pFile->GetMessages());
  m_MsgSample.CopyOnly(*pFile->GetMessages(),pSample->GetSampleAlerts()->Get());
  m_MsgILS.CopyOnly(*pFile->GetMessages(),pSample->GetILSAlerts()->Get());
  m_MsgChannel.CopyOnly(*pFile->GetMessages(),&vn);

  m_pNotebook = new wxNotebook(
    this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxNB_TOP);
  int nType;
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    if(!bTabs[i])
    {
      m_pSplitter[i] = NULL;
    }
    else
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
        bSplitHorizontal,
        bReadOnly);

      m_pSplitter[i] = pSplitter;
      m_pNotebook->AddPage(pSplitter,*g_psLabels[i],i == nSelect);
    }
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

  // layout this

  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pNotebook,1,wxEXPAND);
  SetSizer(pSizer);
  pSizer->Layout();
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    pSplitter = m_pSplitter[i];
    if(pSplitter != NULL)
    {
      pSplitter->Split();
    }
  }
}

bool CPanelSampleAlertNotebook::Validate()
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

const COARnotes *CPanelSampleAlertNotebook::_GetNotes(
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
wxString CPanelSampleAlertNotebook::_GetReviewAcceptance(int n, const wxDateTime *pdt)
{
  wxString sRtn;
  wxString s2;
  switch(n)
  {
    case SA_NDX_SAMPLE:
      sRtn = m_pSample->FormatSampleReviewAcceptance(pdt);
      s2 = m_pFile->CheckSampleStatus(m_pSample,pdt);
      break;
    case SA_NDX_ILS:
      sRtn = m_pSample->FormatILSReviewAcceptance(pdt);
      s2 = m_pFile->CheckILSStatus(m_pSample,pdt);
      break;
    case SA_NDX_CHANNEL:
      sRtn = m_pSample->FormatChannelReviewAcceptance(pdt);
      s2 = m_pFile->CheckChannelStatus(m_pSample,pdt);
      break;
    case SA_NDX_DIR:
      sRtn = m_pFile->FormatReviewAcceptance(pdt);
      s2 = m_pFile->CheckDirStatus(pdt);
      break;
  }
  nwxString::Append(&sRtn,s2);
  return sRtn;
}

bool CPanelSampleAlertNotebook::TransferDataToWindow()
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
  return true;
}
/*

// commented 1/31/11
void CPanelSampleAlertNotebook::_TransferNotes(int i)
{
  CPanelSampleAlertDetails *psd = m_pSplitter[i];
  if(psd != NULL)
  {
    wxString sUser;
    if(m_pPanelUser != NULL)
    {
      sUser = m_pPanelUser->GetValue();
    }
    m_sText[i] = psd->GetNewNotes(_GetNotes(i),sUser);
  }
}
*/

bool CPanelSampleAlertNotebook::IsNotesModified(int n)
{
  CPanelSampleAlertDetails *psd;
  bool bRtn =
    !m_bReadOnly
    && ((psd = m_pSplitter[n]) != NULL)
    && psd->IsNotesModified();
  return bRtn;
}
/*
bool CPanelSampleAlertNotebook::IsNotesEmpty(int n)
{
  bool bRtn = false;
  CPanelSampleAlertDetails *psd;
  if( (!m_bReadOnly) && ((psd = m_pSplitter[n]) != NULL) )
  {
    bRtn = !psd->IsNotesModified();
//    wxString s = psd->GetNotesText();
//    nwxString::Trim(&s);
//    bRtn = s.IsEmpty();
  }
  return bRtn;
}
*/
bool CPanelSampleAlertNotebook::IsAlertsModified(int n)
{
  bool bRtn = false;
  CPanelSampleAlertDetails *psd;
  if( (!m_bReadOnly) && ((psd = m_pSplitter[n]) != NULL) )
  {
    bRtn = psd->GetMsgEdit()->IsModified(*m_pFile->GetMessages());
  }
  return bRtn;
}

bool CPanelSampleAlertNotebook::IsModified(int n)
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

bool CPanelSampleAlertNotebook::IsModified()
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


bool CPanelSampleAlertNotebook::SetDateTime(const wxDateTime *pTime)
{
  bool bRtn = m_HistTime.SetDateTime(pTime);
  TransferDataToWindow();
  return bRtn;
}



void CPanelSampleAlertNotebook::OnNotesChange(wxCommandEvent &e)
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
void CPanelSampleAlertNotebook::OnCellChange(wxGridEvent &)
{
  if(!m_bReadOnly)
  {
    _ProcessEvent();
  }
}

bool CPanelSampleAlertNotebook::_ProcessEvent()
{
  wxCommandEvent eSend(wxEVT_COMMAND_ENTER ,GetId());
  eSend.SetEventObject(this);
  return ProcessEvent(eSend);
}
BEGIN_EVENT_TABLE(CPanelSampleAlertNotebook,wxPanel)
EVT_TEXT(wxID_ANY,CPanelSampleAlertNotebook::OnNotesChange)
EVT_GRID_CMD_CELL_CHANGED(wxID_ANY,CPanelSampleAlertNotebook::OnCellChange)
END_EVENT_TABLE()
