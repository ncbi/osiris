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
*  FileName: CSplitterCellHistory.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"
#include "nwx/vectorptr.h"
#include "CPanelSampleAlertNotebook.h"
#include "wxIDS.h"
#include "CSplitterCellHistory.h"
#include "CPanelHistory.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/debug.h>
#include "CEventScroll.h"
#include "Platform.h"

CSplitterCellHistory::~CSplitterCellHistory()
{
  vectorptr<nwxSyncScrollwin>::cleanup(&m_apSyncScroll);
}


CSplitterCellHistory::CSplitterCellHistory(
  int nSelect,
  bool bSingle,
  const COARfile &File,
  const COARsample &Sample,
  const map<int,wxString> &, // future use
  wxWindow *parent,
  wxWindowID id) :
    wxSplitterWindow(
      parent,id,
      wxDefaultPosition, wxDefaultSize,
      ID_SPLITTER_STYLE)
{
  set<wxDateTime> setDateTimeKeep;

  _SetupSetDateTime(File,Sample,&setDateTimeKeep);
  _BuildWindow(File,Sample,setDateTimeKeep,nSelect,bSingle);
}


CSplitterCellHistory::CSplitterCellHistory(
  const COARlocus &Locus,
  const COARmessages &Msg,
  const set<wxDateTime> &setDateTime,
  wxWindow *parent,
  wxWindowID id) :
    wxSplitterWindow(
      parent,id,
      wxDefaultPosition, wxDefaultSize,
      ID_SPLITTER_STYLE)
{
  set<wxDateTime> setDateTimeKeep;
  _SetupSetDateTime(Locus,Msg,setDateTime,&setDateTimeKeep);
  _BuildWindow(&Locus,&Msg,setDateTimeKeep);
}

CSplitterCellHistory::CSplitterCellHistory(
  const COARsample &Sample,
  int nChannel,
  const COARlocus &Locus,
  const COARmessages &Msg,
  const set<wxDateTime> &setDateTime,
  wxWindow *parent,
  wxWindowID id) :
    wxSplitterWindow(
      parent,id,
      wxDefaultPosition, wxDefaultSize,
      ID_SPLITTER_STYLE)
{
  set<wxDateTime> setDateTimeKeep;
  _SetupSetDateTime(
    Sample,nChannel,Locus,Msg,setDateTime,&setDateTimeKeep);
  _BuildWindow(
    &Locus,&Msg,setDateTimeKeep,&Sample,nChannel);
}


bool CSplitterCellHistory::_CheckMessageUpdate(
  const COARmessages &Msg,
  const vector<int> *pvn,
  const wxDateTime &dtLast,
  const wxDateTime *pdt)
{
  // return true if there is at least one message
  // updated after 'dtLast' and before *pdt

  bool bRtn = false;
  // check for a newer message change
  for(vector<int>::const_iterator itrn = pvn->begin();
    (!bRtn) && (itrn != pvn->end());
    ++itrn)
  {
    if(Msg.IsEditedSince(*itrn,dtLast,pdt))
    {
      bRtn = true;
    }
  }
  return bRtn;
}

void CSplitterCellHistory::_SetupSetDateTime(
    const COARfile &File,
    const COARsample &Sample,
    set<wxDateTime> *psetKeep,
    int nSelect)
{
  set<wxDateTime>::const_iterator itr;
  wxString sNotesILS;
  wxString sNotesSample;
  wxString sNotesChannel;
  wxString sNotesDir;
  wxString sNotes;
  wxDateTime dt;
  wxDateTime dtLast;
  vector<int> vnAlerts;
  bool bKeep;
  bool bSample = false;
  bool bILS = false;
  bool bChannel = false;
  bool bDir = false;
  switch(nSelect)
  {
  case SA_NDX_DIR:
    bDir = true;
    break;
  case SA_NDX_SAMPLE:
    bSample = true;
    break;
  case SA_NDX_ILS:
    bILS = true;
    break;
  case SA_NDX_CHANNEL:
    bChannel = true;
    break;
  default:
    bChannel = true;
    bSample = true;
    bILS = true;
    bDir = true;
    break;
  }
  if(bSample)
  {
    Sample.AppendChannelAlerts(&vnAlerts);
  }
  if(bILS)
  {
    COARfile::AppendVectorInt(&vnAlerts,Sample.GetILSAlerts()->Get());
  }
  if(bSample)
  {
    COARfile::AppendVectorInt(&vnAlerts,Sample.GetSampleAlerts()->Get());
  }
  if(bDir)
  {
    // get all message numbers
    const COARdirectoryAlerts *pDirAlert = 
      File.GetDirectoryAlerts();
    const vector<COARbaseLocus *> *pvBaseLoci = 
      pDirAlert->GetBaseLociAlerts();
    vector<COARbaseLocus *>::const_iterator itrL;
    COARfile::AppendVectorInt(
      &vnAlerts,pDirAlert->GetMessagesNumbers());
    for(itrL = pvBaseLoci->begin();
      itrL != pvBaseLoci->end();
      ++itrL)
    {
      COARfile::AppendVectorInt(
        &vnAlerts,(*itrL)->GetMessageNumbers());
    }
  }

  //bool bFirstLoop = true;
  psetKeep->clear();
  dtLast.Set((time_t) 0);
  psetKeep->insert(dtLast);
  const set<wxDateTime> &setDateTime(*File.GetHistory());
  if(setDateTime.size())
  {
    if(bChannel)
    {
      sNotesChannel = COARnotes::GetText(Sample.GetNotesChannel(&dtLast));
    }
    if(bILS)
    {
      sNotesILS = COARnotes::GetText(Sample.GetNotesILS(&dtLast));
    }
    if(bSample)
    {
      sNotesSample = COARnotes::GetText(Sample.GetNotesSample(&dtLast));
    }
    if(bDir)
    {
      sNotesDir = COARnotes::GetText(File.GetNotesDir(&dtLast));
    }
    for(itr = setDateTime.begin(); itr != setDateTime.end(); ++itr)
    {
      dt = *itr;
      bKeep = _CheckMessageUpdate(
        *File.GetMessages(),&vnAlerts,dtLast,&dt);

      if(bChannel)
      {
        sNotes = COARnotes::GetText(Sample.GetNotesChannel(&dt));
        if(sNotes != sNotesChannel)
        {
          sNotesChannel = sNotes;
          bKeep = true;
        }
      }
      if(bILS)
      {
        sNotes = COARnotes::GetText(Sample.GetNotesILS(&dt));
        if(sNotes != sNotesILS)
        {
          sNotesILS = sNotes;
          bKeep = true;
        }
      }
      if(bSample)
      {
        sNotes = COARnotes::GetText(Sample.GetNotesSample(&dt));
        if(sNotes != sNotesSample)
        {
          sNotesSample = sNotes;
          bKeep = true;
        }
      }
      if(bDir)
      {
        sNotes = COARnotes::GetText(File.GetNotesDir(&dt));
        if(sNotes != sNotesDir)
        {
          sNotesDir = sNotes;
          bKeep = true;
        }
      }
      if(bKeep)
      {
        // something changed between this time (dt) and the
        // previous (dtLast), so keep this time
        psetKeep->insert(dt);
        dtLast = dt;
      }
    }
  }
}


void CSplitterCellHistory::_SetupSetDateTime(
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    set<wxDateTime> *psetKeep)
{
  wxString sNotes;
  wxString sNotesLast;
  wxDateTime dt;
  wxDateTime dtLast((time_t)0);
  vector<const COARallele *> vpAllele;
  set<wxDateTime>::const_iterator itr;
  const vector<int> *pvnAlerts = Locus.GetAlerts();
  bool bKeep;

  psetKeep->clear();
  dtLast.Set((time_t) 0);
  psetKeep->insert(dtLast);

  if(setDateTime.size())
  {
    sNotesLast = Locus.GetNotes(&dtLast);
    for(itr = setDateTime.begin(); itr != setDateTime.end(); ++itr)
    {
      bKeep = false;
      dt = *itr;
      Locus.GetAllelesByTime(&vpAllele,&dt);
      for(vector<const COARallele *>::const_iterator itra
                  = vpAllele.begin();
          (itra != vpAllele.end()) && (!bKeep);
          ++itra)
      {
        if((*itra)->GetUpdateTime() > dtLast)
        {
          bKeep = true;
        }
      }

      // check for a newer message change
      if(!bKeep)
      {
        bKeep = _CheckMessageUpdate(Msg,pvnAlerts, dtLast, &dt);
      }

      sNotes = Locus.GetNotes(&dt);
      if(sNotes != sNotesLast)
      {
        sNotesLast = sNotes;
        bKeep = true;
      }
      if(bKeep)
      {
        // something changed between this time (dt) and the
        // previous (dtLast), so keep this time
        psetKeep->insert(dt);
        dtLast = dt;
      }
    }
  }
}


void CSplitterCellHistory::_SetupSetDateTime(
  const COARsample &Sample,
  int nChannel,
  const COARlocus &Locus,
  const COARmessages &Msg,
  const set<wxDateTime> &setDateTime,
  set<wxDateTime> *psetKeep)
{
  wxString sNotes;
  wxString sNotesLast;
  wxString sLocusName(Locus.GetName());
  wxDateTime dt;
  wxDateTime dtLast((time_t)0);
  vector<const COARallele *> vpAllele;
  set<wxDateTime>::const_iterator itr;
  const vector<int> *pvnAlerts = Locus.GetAlerts();
  bool bKeep;

  psetKeep->clear();
  dtLast.Set((time_t) 0);
  psetKeep->insert(dtLast);

  if(setDateTime.size())
  {
    sNotesLast = Locus.GetNotes(&dtLast);
    for(itr = setDateTime.begin(); itr != setDateTime.end(); ++itr)
    {
      bKeep = false;
      dt = *itr;
      auto_ptr< vectorptr<COARpeakAny> > pPeaks(
        Sample.GetPeaksByLocusName(
            sLocusName, nChannel, &dt));
      for(vectorptr<COARpeakAny>::iterator itra
                  = pPeaks->begin();
          (itra != pPeaks->end()) && (!bKeep);
          ++itra)
      {
        if((*itra)->GetUpdateTime() > dtLast)
        {
          bKeep = true;
        }
      }

      // check for a newer message change
      if(!bKeep)
      {
        bKeep = _CheckMessageUpdate(Msg,pvnAlerts, dtLast, &dt);
      }

      sNotes = Locus.GetNotes(&dt);
      if(sNotes != sNotesLast)
      {
        sNotesLast = sNotes;
        bKeep = true;
      }
      if(bKeep)
      {
        // something changed between this time (dt) and the
        // previous (dtLast), so keep this time
        psetKeep->insert(dt);
        dtLast = dt;
      }
    }
  }
}




void CSplitterCellHistory::_BuildWindowSkeleton(
  const set<wxDateTime> &setDateTime)
{
  m_pPanelTop = new wxPanel(this);
  m_pPanelBottom = new wxPanel(this);

  m_pPanelHistory = new CPanelHistory(setDateTime,m_pPanelTop,IDhistoryPanel);
  m_pLabelCurrent = new wxStaticText(m_pPanelBottom,wxID_ANY,"Current");

  wxFont fn = m_pLabelCurrent->GetFont();
  int nPointSize = fn.GetPointSize();
  fn.SetPointSize(nPointSize + 4);
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  m_pLabelCurrent->SetFont(fn);
}
wxBoxSizer *CSplitterCellHistory::_MakeSplitPanelSizer(
  wxWindow *parent, wxWindow *topLabel, wxWindow *bottomData)
{
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(topLabel,0,wxALIGN_CENTER | wxALL,ID_BORDER);
  pSizer->Add(bottomData,1, wxEXPAND);
  parent->SetSizer(pSizer);
  pSizer->Layout();
  return pSizer;
}

void CSplitterCellHistory::_LayoutWindow(
  wxWindow *topPaneData,
  wxWindow *bottomPaneData)
{
  _MakeSplitPanelSizer(m_pPanelTop,m_pPanelHistory,topPaneData);
  _MakeSplitPanelSizer(m_pPanelBottom, m_pLabelCurrent,bottomPaneData);

}
void CSplitterCellHistory::Split()
{
  SplitHorizontally(m_pPanelTop,m_pPanelBottom,0);
  SetSashGravity(0.5);
  SetMinimumPaneSize(1);
}

void CSplitterCellHistory::_BuildWindow(
  const COARlocus *pLocus,
  const COARmessages *pMsg,
  const set<wxDateTime> &setDateTime,
  const COARsample *pSample,
  int nChannel)
{
  CPanelLocusDetails *pLocusPanelTop;
  CPanelLocusDetails *pLocusPanelBottom;

  _BuildWindowSkeleton(setDateTime);

  if(pSample == NULL)
  {
    pLocusPanelTop = new CPanelLocusDetails(pLocus,pMsg,
      m_pPanelTop,wxID_ANY,false,true);
    pLocusPanelBottom = new CPanelLocusDetails(pLocus,pMsg,
      m_pPanelBottom,wxID_ANY,false,true);
  }
  else
  {
    pLocusPanelTop = new CPanelLocusDetails(pSample,nChannel,
      pLocus,pMsg,m_pPanelTop,wxID_ANY,false,true);
    pLocusPanelBottom = new CPanelLocusDetails(pSample,nChannel,
      pLocus,pMsg,m_pPanelBottom,wxID_ANY,false,true);
  }

  _LayoutWindow(pLocusPanelTop,pLocusPanelBottom);

  m_pDateTime = pLocusPanelTop;
  pLocusPanelTop->TransferDataToWindow();
  pLocusPanelBottom->TransferDataToWindow();
  m_pPanelHistory->SetOriginal();

  // set up sync

  m_syncSplitter.Add(pLocusPanelTop->GetSplitterWindow());
  m_syncSplitter.Add(pLocusPanelBottom->GetSplitterWindow());

#if PANEL_LOCUS_NOTEBOOK
  m_syncNotebook.Add(pLocusPanelTop->GetNotebookWindow());
  m_syncNotebook.Add(pLocusPanelBottom->GetNotebookWindow());
#else
  m_syncSplitter2.Add(pLocusPanelTop->GetLocusSplitterWindow());
  m_syncSplitter2.Add(pLocusPanelBottom->GetLocusSplitterWindow());
#endif

  nwxSyncScrollwin *pSync;
  m_apSyncScroll.reserve(2);

  pSync = new nwxSyncScrollwin;
  m_apSyncScroll.push_back(pSync);
  pSync->Add(pLocusPanelTop->GetLocusGrid());
  pSync->Add(pLocusPanelBottom->GetLocusGrid());
  
  pSync = new nwxSyncScrollwin;
  m_apSyncScroll.push_back(pSync);
  pSync->Add(pLocusPanelTop->GetAlertGrid());
  pSync->Add(pLocusPanelBottom->GetAlertGrid());
}

void CSplitterCellHistory::_BuildWindow(
  const COARfile &File,
  const COARsample &Sample,
  const set<wxDateTime> &setDateTime,
  int nSelect,
  bool bShowOne)
{
  CPanelSampleAlertNotebook *pSamplePanelTop;
  CPanelSampleAlertNotebook *pSamplePanelBottom;

  _BuildWindowSkeleton(setDateTime);

  pSamplePanelTop = new CPanelSampleAlertNotebook(&File,&Sample,
    m_pPanelTop,wxID_ANY,NULL,false,true,nSelect,bShowOne);
  pSamplePanelBottom = new CPanelSampleAlertNotebook(&File,&Sample,
    m_pPanelBottom,wxID_ANY,NULL,false,true,nSelect,bShowOne);

  _LayoutWindow(pSamplePanelTop,pSamplePanelBottom);

  m_pDateTime = pSamplePanelTop;
  pSamplePanelTop->TransferDataToWindow();
  pSamplePanelBottom->TransferDataToWindow();
  m_pPanelHistory->SetOriginal();

  // set up sync

  //  splitters

  CPanelSampleAlertDetails **ppSplittersTop =
    pSamplePanelTop->GetSplitterWindows();
  CPanelSampleAlertDetails **ppSplittersBottom =
    pSamplePanelBottom->GetSplitterWindows();
  CPanelSampleAlertDetails **ppSplitter;
  CPanelSampleAlertDetails *pSplitter;
  
  nwxSyncScrollwin *pSyncScroll;
  m_apSyncScroll.reserve(3);

  for(int i = 0; i < SA_WINDOW_COUNT; ++i)
  {
    if(pSamplePanelTop->IsIndexUsed(i))
    {
      pSyncScroll = new nwxSyncScrollwin;
      m_apSyncScroll.push_back(pSyncScroll);

      ppSplitter = ppSplittersTop;
      for(int j = 0; j < 2; j++)
      {
        pSplitter = ppSplitter[i];
        m_syncSplitter.Add(pSplitter);
        pSyncScroll->Add(pSplitter->GetGridAlerts());

        ppSplitter = ppSplittersBottom;
      }
    }
  }

  // notebooks

  m_syncNotebook.Add(pSamplePanelTop->GetNotebookWindow());
  m_syncNotebook.Add(pSamplePanelBottom->GetNotebookWindow());
}


void CSplitterCellHistory::OnNotebookChange(wxNotebookEvent &e)
{
  wxNotebook *p = (wxNotebook *)e.GetEventObject();
  if(m_syncNotebook.Find(p))
  {
    m_syncNotebook.Sync(p);
  }
}

void CSplitterCellHistory::OnHistoryChange(wxCommandEvent &)
{
  const wxDateTime *pdt(m_pPanelHistory->GetDateTime());
  m_pDateTime->SetDateTime(pdt);
}

void CSplitterCellHistory::OnSplitterChange(wxSplitterEvent &e)
{
  wxSplitterWindow *p = (wxSplitterWindow *) e.GetEventObject();
  if(p != this)
  {
    m_syncSplitter.Sync(p);
#if !PANEL_LOCUS_NOTEBOOK
    m_syncSplitter2.Sync(p);
#endif
  }
}
void CSplitterCellHistory::OnSplitterChanging(wxSplitterEvent &e)
{
  // inform m_syncSplitter that the user is moving the sash
  // and not resizing the entire window (frame)

  wxSplitterWindow *p = (wxSplitterWindow *) e.GetEventObject();
  if(p != this)
  {
    m_syncSplitter.SetIsChanging(p);
#if !PANEL_LOCUS_NOTEBOOK
    m_syncSplitter2.SetIsChanging(p);
#endif
  }
}

void CSplitterCellHistory::_OnScroll(wxCommandEvent &e)
{
  wxObject *p = (wxScrolledWindow *)e.GetEventObject();
  if(p->IsKindOf(CLASSINFO(wxScrolledWindow)))
  {
    wxScrolledWindow *pw = (wxScrolledWindow *)p;
    vector<nwxSyncScrollwin *>::iterator itr;
    nwxSyncScrollwin *pSync;
    for(itr = m_apSyncScroll.begin();
        itr != m_apSyncScroll.end();
        ++itr)
    {
      pSync = *itr;
      pSync->Sync(pw);
    }
  }
  else
  {
    const wxChar *ps =
      wxS("CSplitterCellHistory::_OnScroll\n"
					"object is not a scrolled window");
    wxASSERT_MSG(0,ps);
    mainApp::LogMessage(ps);
  }
  e.Skip(true);
}

BEGIN_EVENT_TABLE(CSplitterCellHistory,wxSplitterWindow)
EVT_COMMAND_ENTER(IDhistoryPanel,CSplitterCellHistory::OnHistoryChange)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY,CSplitterCellHistory::OnNotebookChange)
EVT_SPLITTER_SASH_POS_CHANGED(wxID_ANY, CSplitterCellHistory::OnSplitterChange)
EVT_SPLITTER_SASH_POS_CHANGING(wxID_ANY, CSplitterCellHistory::OnSplitterChanging)
EVT_COMMAND(wxID_ANY,CEventScroll,CSplitterCellHistory::_OnScroll) 
END_EVENT_TABLE()

