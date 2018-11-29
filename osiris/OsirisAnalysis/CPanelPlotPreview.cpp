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
*  FileName: CPanelPlotPreview.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include <wx/filename.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/font.h>
#include <wx/utils.h>
#include "nwx/nwxBatch.h"
#include "nwx/nwxString.h"
#include "CPanelPlotPreview.h"
#include "CPlotData.h"
#include "CPanelPlot.h"
#include "CKitColors.h"
#include "CFrameAnalysis.h"
#include "COARfile.h"
#include <memory>

CPanelPlotPreview::CPanelPlotPreview(
  CFrameAnalysis *pFrame,
  wxWindow *parent, 
  COARfile *pFile,
  CKitColors *pColors,
  bool bExternalTimer,
  size_t nMaxCount)
: wxPanel(parent),
  m_nMaxCount(1),
  m_pTextMessage(NULL),
  m_pSizer(NULL),
  m_pFrameAnalysis(pFrame),
  m_pOARfile(pFile),
  m_pColors(pColors),
  m_bExternalTimer(bExternalTimer),
  m_bLogged(true)
{
  SetBackgroundColour(*wxWHITE);
  SetMaxCount(nMaxCount);
  m_pSizer = new wxBoxSizer(wxVERTICAL);
  SetSizer(m_pSizer);
}
CPanelPlotPreview::~CPanelPlotPreview()
{
  _Cleanup(0);
  if(m_pTextMessage != NULL)
  {
    m_pTextMessage->Destroy(); // probably not needed
  }
}

CPanelPlot *CPanelPlotPreview::SetFileLocus(
  const wxString &sFileName, const wxString &sLocus, bool bReload)
{
  CPanelPlot *pRtn = _Setup(sFileName,bReload);
  if(pRtn != NULL)
  {
    TnwxBatch<CPanelPlot> batch(pRtn);
    CPlotData *pData = pRtn->GetPlotData();
    unsigned int nChannel = pData->GetChannelFromLocus(sLocus);
    if( (!nChannel) && (m_pOARfile != NULL) )
    {
      nChannel = m_pOARfile->GetChannelNrFromLocus(sLocus);
    }
    if(nChannel)
    {
      pRtn->ShowOneChannel(nChannel);
    }
    else
    {
      pRtn->ShowAllChannels(true);
    }
    pRtn->ResetDefaults();
    pRtn->EnableLabelMenu(true);
    pRtn->SetLabelType(m_pFrameAnalysis->GetPlotLabelType());
    pRtn->RebuildCurves();
    pRtn->ZoomToLocus(sLocus);
    pRtn->Refresh();
  }
  return pRtn;
}
CPanelPlot *CPanelPlotPreview::SetFileChannels(
  const wxString &sFileName, 
  const vector<unsigned int> &nChannels, 
  bool bReload)
{
  CPanelPlot *pRtn = _Setup(sFileName,bReload);
  if(pRtn != NULL)
  {
    TnwxBatch<CPanelPlot> batch(pRtn);
    pRtn->ShowAllChannels(false);
    for(vector<unsigned int>::const_iterator itr = nChannels.begin();
      itr != nChannels.end();
      ++itr)
    {
      pRtn->ShowChannel(*itr,true);
    }
    pRtn->ResetDefaults();
    pRtn->SetLabelType(LABEL_NONE);
    pRtn->EnableLabelMenu(true);
    pRtn->SetArtifactLabelNone();
    pRtn->RebuildCurves();
    pRtn->ZoomOut(false);
    pRtn->Refresh();
  }
  return pRtn;
}

CPanelPlot *CPanelPlotPreview::SetFileAllChannels(
    const wxString &sFileName,bool bReload)
{
  CPanelPlot *pRtn = _Setup(sFileName,bReload);
  if(pRtn != NULL)
  {
    TnwxBatch<CPanelPlot> batch(pRtn);
    pRtn->ShowAllChannels(true);
    pRtn->ResetDefaults();
    pRtn->SetLabelType(LABEL_NONE);
//    pRtn->EnableLabelMenu(false);
    pRtn->SetArtifactLabelNone();
    pRtn->RebuildCurves();
    pRtn->ZoomOut(false);
    pRtn->Refresh();
  }
  return pRtn;
}

CPanelPlot *CPanelPlotPreview::SetFileILS(
  const wxString &sFileName, bool bReload)
{
  CPanelPlot *pRtn = _Setup(sFileName,bReload);
  if(pRtn != NULL)
  {
    TnwxBatch<CPanelPlot> batch(pRtn);
    CPlotData *pData = pRtn->GetPlotData();
    unsigned int nILS = pData->GetILSChannel();
    pRtn->ShowOneChannel(nILS);
    pRtn->ResetDefaults();
    pRtn->EnableLabelMenu(true);
    pRtn->SetLabelType(m_pFrameAnalysis->GetPlotLabelType());
    pRtn->RebuildCurves();
    pRtn->ZoomOut();
    pRtn->Refresh();
  }
  return pRtn;
}

void CPanelPlotPreview::SetMaxCount(size_t nMaxCount)
{
  if(nMaxCount < 1)
  { nMaxCount = 1;
  }
  m_nMaxCount = nMaxCount;
  _Cleanup(m_nMaxCount);
}
CPanelPlot *CPanelPlotPreview::_FindCurrent()
{
  CPanelPlot *pRtn(NULL);
  if(!IsShown()) {} // nothing
  else if(m_listPlots.empty()) {} // do nothing
  else if( (m_pTextMessage != NULL) && 
    m_pTextMessage->IsShown() ) {} // do nothing
  else
  {
#ifdef _DEBUG
    int nCount = 0;
#endif
    list<CPanelPlot *>::iterator itr;
    for(itr = m_listPlots.begin();
      itr != m_listPlots.end();
      ++itr)
    {
      if((*itr)->IsShown())
      {
        pRtn = *itr;
#ifdef _DEBUG
        nCount++;
#else
        break; // done
#endif
      }
    }
#ifdef _DEBUG
    if( (nCount != 1) && (!m_bLogged) )
    {
      wxString s = wxString::Format(
        "CPanelPlotPreview::OnTimer window count = %d",
        nCount);
      mainApp::LogMessage(s);

      wxASSERT_MSG(0,s);
      m_bLogged = true;
    }
#endif
  }
  return pRtn;
}


bool CPanelPlotPreview::MenuEvent(wxCommandEvent &e)
{
  bool bRtn = false;
  CPanelPlot *pPlot = _FindCurrent();
  if(pPlot != NULL)
  {
    bRtn = pPlot->MenuEvent(e);
  }
  return bRtn;
}
void CPanelPlotPreview::OnTimer(wxTimerEvent &e)
{
  CPanelPlot *pPlot = _FindCurrent();
  if(pPlot != NULL)
  {
    pPlot->OnTimer(e);
  }
}

CMenuPlot *CPanelPlotPreview::GetMenuPlot()
{
  CMenuPlot *pMenu(NULL);
  if(IsShown())
  {
    CPanelPlot *pPlot = _FindCurrent();
    if(pPlot != NULL)
    {
      pMenu = pPlot->GetMenuPlot();
    }
  }
  return pMenu;
}
wxMenu *CPanelPlotPreview::GetMenu()
{
  CMenuPlot *pRtn = GetMenuPlot();
  return (wxMenu *)pRtn;
}

void CPanelPlotPreview::_Cleanup(size_t n)
{
  list<CPanelPlot *>::reverse_iterator itr;
  CPlotData *pData;
  CPanelPlot *pPanel;
  while(m_listPlots.size() > n)
  {
    itr = m_listPlots.rbegin();
    pPanel = *itr;
    pData = pPanel->GetPlotData();
    pPanel->Destroy();
    delete pData;
    m_listPlots.pop_back();
  }
}

bool CPanelPlotPreview::_RemoveFile(const wxString &sFileName)
{
  list<CPanelPlot *>::iterator itr;
  CPlotData *pData;
  CPanelPlot *pPanel;
  bool bRtn = false;
  for(itr = m_listPlots.begin();
    itr != m_listPlots.end();
    ++itr)
  {
    pPanel = *itr;
    pData = pPanel->GetPlotData();
    if(nwxString::FileNameStringEqual(
          pData->GetLastFileName(),sFileName))
    {
      if(pPanel->IsShown())
      {
        m_pSizer->Detach(pPanel);
      }
      pPanel->Destroy();
      delete pData;
      m_listPlots.erase(itr);
      bRtn = true;
      break;
    }
  }
  return bRtn;
}

void CPanelPlotPreview::_HideCurrent()
{
  if(m_pSizer != NULL)
  {
    wxSizerItemList &siList(m_pSizer->GetChildren());
    size_t nSize = siList.GetCount(); // nSize should be 0 or 1
    for(size_t i = 0; i < nSize; i++)
    {
      wxSizerItem *pItem = siList.Item(i)->GetData();
      if(pItem->IsWindow()) // should ALWAYS be 'true'
      {
        pItem->GetWindow()->Show(false);
      }
    }
    m_pSizer->Clear();
  }
}

CPanelPlot *CPanelPlotPreview::_Create(const wxString &sFileName)
{
  CPanelPlot *pRtn(NULL);
  if(wxFileName::IsFileReadable(sFileName))
  {
    wxBusyCursor xxx;
    auto_ptr<CPlotData> pData(new CPlotData());
    if(pData->LoadFile(sFileName))
    {
      pRtn = new CPanelPlot(
        this,
        m_pFrameAnalysis,
        pData.release(),
        m_pOARfile,
        m_pColors,
        m_bExternalTimer);
      m_listPlots.push_front(pRtn);
      _Cleanup(m_nMaxCount);
    }
  }
  return pRtn;
}

CPanelPlot *CPanelPlotPreview::_Setup(const wxString &sFileName,bool bReload)
{
  if(bReload)
  {
    _RemoveFile(sFileName);
  }
  list<CPanelPlot *>::iterator itr;
  CPanelPlot *pPanel(NULL);
  CPanelPlot *pRtn(NULL);
  CPlotData *pData;
  bool bFirst = true;
  bool bLayout = false;
  for(itr = m_listPlots.begin();
    itr != m_listPlots.end();
    ++itr)
  {
    pPanel = *itr;
    pData = pPanel->GetPlotData();
    if(nwxString::FileNameStringEqual(
          pData->GetLastFileName(),sFileName))
    {
      if(!bFirst)
      {
        m_listPlots.erase(itr);
        m_listPlots.push_front(pPanel);
      }
      pRtn = pPanel;
      bLayout = !pRtn->IsShown();
      break;
    }
    bFirst = false;
  }
  if(pRtn == NULL)
  {
    pRtn = _Create(sFileName);
    bLayout = (pRtn != NULL);
  }
  if(bLayout)
  {
    _HideCurrent();
    m_pSizer->Add(pRtn,1,wxEXPAND);
    pRtn->Show(true);
    m_pSizer->Layout();
  }
  else if(pRtn == NULL)
  {
    _SetupMessage();
  }
  return pRtn;
}
void CPanelPlotPreview::SetFileMissing()
{
  _SetupMessage();
}
void CPanelPlotPreview::SetPeakLabelType(int n)
{
  CPanelPlot *pPlot = _FindCurrent();
  if(pPlot != NULL)
  {
    if(n < 0 || n > LABEL_PLOT_TYPE_MAX)
    {
      n = 0;
    }
    pPlot->SetLabelType((LABEL_PLOT_TYPE)n);
  }
}

void CPanelPlotPreview::UpdateLadderLabels()
{
  CPanelPlot *pPlot = _FindCurrent();
  if(pPlot != NULL)
  {
    pPlot->UpdateLadderLabels();
  }
}

wxStaticText *CPanelPlotPreview::_SetupMessage()
{
  bool bLayout = false;
  if(m_pTextMessage == NULL)
  {
    m_pTextMessage = new wxStaticText(
      this,wxID_ANY,"Cannot find plot data");
    m_pTextMessage->SetBackgroundColour(*wxWHITE);
    m_pTextMessage->SetForegroundColour(*wxBLACK);
    wxFont fnt = m_pTextMessage->GetFont();
    fnt.SetPointSize(12);
    m_pTextMessage->SetFont(fnt);
    bLayout = true;
  }
  else
  {
    bLayout = !m_pTextMessage->IsShown();
  }
  if(bLayout)
  {
    _HideCurrent();
    m_pSizer->AddStretchSpacer(1);
    m_pSizer->Add(m_pTextMessage,0,
      wxALL | wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL, 
      ID_BORDER);
    m_pSizer->AddStretchSpacer(1);
    m_pTextMessage->Show(true);
    m_pSizer->Layout();
  }
  return m_pTextMessage;
}

