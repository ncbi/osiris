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
#include "CKitColors.h"
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
#include "nwx/vectorptr.h"
#include "nwx/nwxLog.h"
#include "CPanelUserID.h"
#include "CPanelLocusDetails.h"
#include "CLabels.h"
#include "CFrameSample.h"
#include "CPageEditSample.h"


CNotebookEditSample::~CNotebookEditSample() 
{
  vectorptr<CPageEditSample>::cleanup(&m_vpPanels);
}

CNotebookEditSample::CNotebookEditSample(
  COARfile *pFile,
  COARsample *pSample,
  wxWindow *parent,
  wxWindowID id,
  const map<int,wxString> *pmapChannelNames) :
    wxPanel(parent,id),
    m_pSample(pSample),
    m_pFile(pFile),
    m_bReadOnly(false)
{
  vector<wxString> vsLocus;
  // set up notes
  nwxLog_I_WAS_HERE;
  //  set up sample alert details

  CPanelSampleAlertDetails *pSplitter;
  wxBoxSizer *pSizer;
  int i;

  m_pNotebook = new wxTreebook(
    this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxNB_LEFT);
  CPageEditSample *pPage;
  m_vpPanels.reserve(30);
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    switch(i)
    {
    case SA_NDX_DIR:
      pPage = new CEditAlertsDir(this,pFile);
      break;
    case SA_NDX_SAMPLE:
      pPage = new CEditAlertsSample(this,pSample);
      break;
    case SA_NDX_ILS:
      pPage = new CEditAlertsILS(this,pSample);
      break;
    case SA_NDX_CHANNEL:
      pPage = new CEditAlertsChannel(this,pSample,pmapChannelNames);
      break;
    default:
      pPage = NULL;
    }
    m_vpPanels.push_back(pPage);
    wxWindow *pWin = pPage->GetPanel();
    pSplitter = wxDynamicCast(pWin,CPanelSampleAlertDetails);
    if(pSplitter == NULL)
    {
      wxString sErr =
        wxString::Format("Cannot create page for type %d",i);
      wxASSERT_MSG(0, sErr);
    }
    m_pNotebook->AddPage(pSplitter,pPage->GetPageLabel(),!i);
  }
  
  //****************************************************************
  // done with sample alert details, now set up loci
  //****************************************************************
  const wxString &sKitName = m_pFile->GetKitName();
  CKitColors *pKitColors = mainApp::GetKitColors();
  const CSingleKitColors *pSingleKit = pKitColors->GetKitColors(sKitName);
  const CChannelColors *pCC;


  size_t nChannelCount = m_pFile->GetChannelCount();
  size_t j,k,nLocusCount;
  const COARchannel *pChannel;
  vector<size_t> vnExpand;
  vector<size_t>::iterator itrExpand;
  vnExpand.reserve(nChannelCount);
  for(k = 1; k <= nChannelCount; ++k)
  {
    // STOP HERE pChanel->GetChannelName() returns nothing
    pCC = pSingleKit->GetColorChannel(k);
    wxString sDyeName = pCC->GetDyeName();
    bool bEmpty = sDyeName.IsEmpty(); // for test/debug
    if(bEmpty)
    {
      sDyeName.Format("Channel %d",(int) k);
    }
    else
    {
      sDyeName.Append(wxString::Format(" - %d",(int)k));
    }
    pChannel = m_pFile->GetChannelByNr(k);
    m_pNotebook->AddPage(NULL,sDyeName);
    vnExpand.push_back(m_vpPanels.size());
    m_vpPanels.push_back(NULL);
    m_asLocus.push_back(wxEmptyString);

    nLocusCount = pChannel->GetLocusCount();
    for(j = 0; j < nLocusCount; ++j)
    {
      const wxString &sLocusName = pChannel->GetLocusName(j);
      m_asLocus.push_back(sLocusName);
      pPage = new CPageEditLocus(this,pSample,sLocusName,(int)k);
      wxWindow *pWin = pPage->GetPanel();
      CPanelLocusDetails *pPanelLocus = wxDynamicCast(pWin,CPanelLocusDetails);
      if(pPanelLocus == NULL)
      {
        wxString sErr(wxT("Cannot create page for "));
        sErr.Append(sLocusName);
        wxASSERT_MSG(0, sErr);
      }
      m_pNotebook->AddSubPage(pPanelLocus,sLocusName,false);
      m_vpPanels.push_back(pPage);
    }
  }
  // layout this

  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pNotebook,1,wxEXPAND);
  SetSizer(pSizer);
  pSizer->Layout();
  for(itrExpand = vnExpand.begin(); itrExpand != vnExpand.end(); ++itrExpand)
  {
    m_pNotebook->ExpandNode(*itrExpand,true);
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
    std::vector<CPageEditSample *>::iterator itr;
    CPageEditSample *pPage;
    int i = 0;
    for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
    {
      pPage = *itr;
      if( (pPage != NULL) && pPage->NeedsApply() && pPage->IsNewNotesEmpty() )
      {
        sMessage += "You haven't entered notes for ";
        sMessage += pPage->GetPageLabel();
        sMessage += LF;
        nCount++;
        if(nPage < 0)
        {
          nPage = i;
        }
      }
      ++i;
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


bool CNotebookEditSample::TransferDataToWindow()
{

  std::vector<CPageEditSample *>::iterator itr;
  CPageEditSample *pPage;
  bool bRtn = true;
  for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
  {
    pPage = *itr;
    if( (pPage != NULL) && !pPage->TransferDataToPage() )
    {
      bRtn = false;
    }
  }
  return bRtn;
}

bool CNotebookEditSample::IsModified()
{
  bool bRtn = false;
  std::vector<CPageEditSample *>::iterator itr;
  CPageEditSample *pPage;
  for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
  {
    pPage = *itr;
    if( (pPage != NULL) && pPage->NeedsApply() )
    {
      bRtn = true;
      break;
    }
  }
  return bRtn;
}
/*

bool CNotebookEditSample::SetDateTime(const wxDateTime *pTime)
{
  bool bRtn = m_HistTime.SetDateTime(pTime);
  TransferDataToWindow();
  return bRtn;
}
*/


void CNotebookEditSample::OnNotesChange(wxCommandEvent &)
{
  if(!m_bReadOnly)
  {
    _ProcessEvent();
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
