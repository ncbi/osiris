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
*  FileName: CFrameSample.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CFrameSample.h"
#include "COARfile.h"
#include "CHistoryTime.h"
#include "CNotebookEditSample.h"
#include "mainFrame.h"
#include "CFrameAnalysis.h"
#include "CMenuSample.h"
#include "CDialogCellHistory.h"
#include "wx/sizer.h"

IMPLEMENT_ABSTRACT_CLASS(CFrameSample,CMDIFrame)

CFrameSample::CFrameSample(
  CFrameAnalysis *pCreator,
  mainFrame *parent,
  wxSize sz,
  COARfile *pFile,
  COARsample *pSample) : 
    CMDIFrame(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,sz),
    m_Hist(NULL),
    m_pNoteBook(NULL),
    m_pCreator(pCreator),
    m_pParent(parent),
    m_pOARfile(pFile),
    m_pSample(pSample)
{
    wxString s = wxPanelNameStr;

  SetupTitle();
  m_pNoteBook = new CNotebookEditSample(m_pOARfile,m_pSample,this,wxID_ANY,NULL);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  m_pToolbar = new CToolbarSample(this,
    !pFile->GetReviewerAllowUserOverride());
  pSizer->Add(m_pToolbar,0,wxEXPAND);
  pSizer->Add(m_pNoteBook,1,wxEXPAND,0);
  SetSizer(pSizer);
  m_pParent->InsertWindow(this,m_pOARfile);
  m_pMenuBar = new CMenuBarSample();
  SetMenuBar(m_pMenuBar);
  CParmOsirisGlobal parm;
  bool b = parm->GetHideSampleToolbar();
  _ShowToolbar(!b);
  Layout();
}
CFrameSample::~CFrameSample() {}

int CFrameSample::GetType()
{
  return FRAME_SAMPLE;
}
wxMenu *CFrameSample::GetMenu()
{
  return m_pMenuBar->GetMenu();
}
void CFrameSample::_ToggleToolbar()
{
  bool bWasShown = m_pToolbar->IsShown();
  _ShowToolbar(!bWasShown);
  CParmOsirisGlobal parm;
  parm->SetHideSampleToolbar(bWasShown);
}
void CFrameSample::SetupTitle()
{
  wxString s = m_pOARfile->GetFileName();
  bool bMod = false;
  const CParmOsiris &parm(m_pOARfile->GetParameters());
  wxString sTitle = mainApp::FormatWindowTitle(s,bMod,&parm,NULL);
  int n = sTitle.Find(s);
  if(n > 0)
  {
    wxString ss = sTitle.Left(n);
    ss.Append(m_pSample->GetSampleName());
    ss.Append(wxT("; "));
    ss.Append(sTitle.Mid(n));
    sTitle = ss;
  }
  SetTitle(sTitle);
}

void CFrameSample::OnTimer(wxTimerEvent &) {}
bool CFrameSample::TransferDataToWindow()
{
  return m_pNoteBook->TransferDataToWindow();
}
bool CFrameSample::MenuEvent(wxCommandEvent &e)
{
  int nID = e.GetId();
  bool bRtn = true;
  switch(nID)
  {
  case IDmenuDisplayGraph:
    _OpenGraphic();
    break;
  case IDmenuTable:
    m_pCreator->Show(true);
    m_pCreator->Raise();
    break;
  case IDmenuHistory:
    _History();
    break;
  case IDmenuShowHideToolbar:
      _ToggleToolbar();
      break;
  case IDSampleApplyAll:
    break;
  case IDmenuDisableSample:
    break;
  default:
    break;
  }
  return bRtn;
}

void CFrameSample::InitiateRepaintData()
{
  m_pCreator->RepaintAllData(m_pSample);
}
void CFrameSample::RepaintData()
{
  m_pNoteBook->RepaintData();
}
const wxString CFrameSample::GetUserID()
{
  return m_pToolbar->GetUserID();
}

void CFrameSample::_OpenGraphic()
{
  const wxString &sLocus = m_pNoteBook->GetCurrentLocus();
  const wxString &sFileName = m_pOARfile->FindPlotFile(m_pSample);
  m_pParent->OpenFile(sFileName,sLocus,m_pOARfile);
}
void CFrameSample::_History()
{
  const wxString &sLocus = m_pNoteBook->GetCurrentLocus();
  int nSelected = m_pNoteBook->GetSelection();
  CDialogCellHistory::ShowHistory(
    this,m_pOARfile, m_pSample,sLocus,nSelected);
}
void CFrameSample::_ShowToolbar(bool bShow)
{
  if(bShow != m_pToolbar->IsShown())
  {
    m_pToolbar->Show(bShow);
    Layout();
    SetToolbarMenuLabel(!bShow);
  }
}

bool CFrameSample::Destroy()
{
  m_pCreator->RemoveSample(m_pSample,this);
  return SUPER::Destroy();
}

IMPLEMENT_PERSISTENT_SIZE(CFrameSample)

BEGIN_EVENT_TABLE(CFrameSample,CMDIFrame)
EVT_PERSISTENT_SIZE(CFrameSample)
END_EVENT_TABLE()



