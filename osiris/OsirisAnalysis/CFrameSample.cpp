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
#include "CPageEditSample.h"
#include "wx/sizer.h"
#include "nwx/nwxKeyState.h"
#include "nwx/nwxLog.h"

IMPLEMENT_ABSTRACT_CLASS(CFrameSample,CMDIFrame)

#ifdef __WXMSW__
#define INITSIZE(sz) wxSize(sz.x < 0 ? -1 : sz.x - 1 ,sz.y)
#else
#define INITSIZE(sz) sz
#endif

CFrameSample::CFrameSample(
  CFrameAnalysis *pCreator,
  mainFrame *parent,
  const wxSize &sz,
  COARfile *pFile,
  COARsample *pSample,
  int nSelectAlerts,
  const wxString &sSelectLocus) : 
    CMDIFrame(parent,wxID_ANY,wxEmptyString,wxDefaultPosition,INITSIZE(sz)),
    m_sz(sz),
    m_Hist(NULL),
    m_pNoteBook(NULL),
    m_pCreator(pCreator),
    m_pParent(parent),
    m_pOARfile(pFile),
    m_pSample(pSample),
    m_bTitleMod(false)
    //,m_bFirstShow(true)
{
    wxString s = wxPanelNameStr;

  SetupTitle(true);
  wxPanel *pPanel = new wxPanel(this);
  m_pNoteBook = new CNotebookEditSample(m_pOARfile,m_pSample,pPanel,wxID_ANY,NULL);
  if(!sSelectLocus.IsEmpty())
  {
    SelectLocus(sSelectLocus);
  }
  else if(nSelectAlerts >= 0)
  {
    SelectAlerts(nSelectAlerts);
  }
  m_pToolbar = new CToolbarSample(this,pPanel);
  m_pParent->InsertWindow(this,m_pOARfile);
  m_pMenuBar = new CMenuBarSample();
  SetMenuBar(m_pMenuBar);
  SetupMenuItems(); 
  CParmOsirisGlobal parm;
//  bool b = parm->GetHideSampleToolbar();
//  _ShowToolbar(!b);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pNoteBook,1,wxEXPAND,0);
  pSizer->Add(m_pToolbar,0,wxEXPAND);
  pPanel->SetSizer(pSizer);
  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pPanel,1,wxEXPAND | wxALL,0);

  SetSizer(pSizer);
  TransferDataToWindow();
  Layout();  // layout before transferring data
}
CFrameSample::~CFrameSample() {}

void CFrameSample::SelectLocus(const wxString &sLocus)
{
  m_pNoteBook->SelectLocus(sLocus);
}
void CFrameSample::SelectAlerts(int nAlertType)
{
  m_pNoteBook->SelectAlerts(nAlertType);
}

#if 0
bool CFrameSample::Show(bool show)
{
  bool bRtn = CMDIFrame::Show(show);
  if(show && m_bFirstShow)
  {
    Layout();
    TransferDataToWindow();
    m_bFirstShow = false;
  }
  return bRtn;
}
#endif
int CFrameSample::GetType()
{
  return FRAME_SAMPLE;
}
wxMenu *CFrameSample::GetMenu()
{
  return m_pMenuBar->GetMenu();
}
void CFrameSample::SetupTitle(bool bForce)
{
  bool bMod = (m_pNoteBook != NULL) && m_pNoteBook->IsModified();
  if(bForce || (bMod != m_bTitleMod))
  {
    m_bTitleMod = bMod;
    wxString s = m_pSample->GetSampleName();
    s.Append(wxT("; "));
    wxFileName fn(m_pOARfile->GetFileName());
    s.Append(fn.GetFullName());
    const CParmOsiris &parm(m_pOARfile->GetParameters());
    wxString sTitle = mainApp::FormatWindowTitle(s,bMod,&parm,NULL);
    SetTitle(sTitle);
  }
}

void CFrameSample::OnTimer(wxTimerEvent &) {}
bool CFrameSample::TransferDataToWindow()
{
  //  it seems that this isn't being used
  SetupMenuItems();
  return m_pNoteBook->TransferDataToWindow();
}
void CFrameSample::_ApplyAll()
{

  // STOP HERE
  size_t nCount = m_pNoteBook->GetPanelCount();
  CPageEditSample *pPage;
  bool bRefresh = false;
  for(size_t i = 0; i < nCount; ++i)
  {
    pPage = m_pNoteBook->GetPanel(i);
    if((pPage != NULL) && pPage->NeedsApply() && pPage->DoApply())
    {
      bRefresh = true;
    }
  }
  if(bRefresh)
  {
    InitiateRepaintData();
  }
}
void CFrameSample::_Apply()
{
  CPageEditSample *pPage = m_pNoteBook->GetCurrentPanel();
  if((pPage != NULL) && pPage->DoApply())
  {
    InitiateRepaintData();
  }
}
void CFrameSample::_Accept()
{
  CPageEditSample *pPage = m_pNoteBook->GetCurrentPanel();
  if((pPage != NULL) && pPage->DoAccept())
  {
    InitiateRepaintData();
  }
}
void CFrameSample::_Approve()
{
  CPageEditSample *pPage = m_pNoteBook->GetCurrentPanel();
  if((pPage != NULL) && pPage->DoReview())
  {
    InitiateRepaintData();
  }
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
  case IDSampleApplyAll:
    _ApplyAll();
    break;
  case IDSampleApply:
    _Apply();
    break;
  case IDSampleAccept:
    _Accept();
    break;
  case IDSampleApprove:
    _Approve();
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
  m_pNoteBook->TransferDataToWindow();
  SetupMenuItems();
}
const wxString &CFrameSample::GetUserID()
{
  if(m_sUserID.IsEmpty())
  {
    if(CanOverrideUserID())
    {
      CParmOsiris *parm = CParmOsiris::GetGlobal();
      m_sUserID = parm->GetCMFuserID();
    }
    if(m_sUserID.IsEmpty())
    {
      m_sUserID = wxGetUserId();
    }
  }
  return m_sUserID;
}
bool CFrameSample::CanOverrideUserID()
{
  return m_pOARfile->GetReviewerAllowUserOverride();
}
void CFrameSample::SaveUserID()
{
  if( CanOverrideUserID() && !m_sUserID.IsEmpty() )
  {
    CParmOsiris *parm = CParmOsiris::GetGlobal();
    parm->SetCMFuserID(m_sUserID);
  }
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
#if 0
void CFrameSample::_ShowToolbar(bool bShow)
{
  if(bShow != m_pToolbar->IsShown())
  {
    m_pToolbar->Show(bShow);
    Layout();
    SetToolbarMenuLabel(!bShow);
  }
}
#endif

bool CFrameSample::Destroy()
{
  m_pCreator->RemoveSample(m_pSample,this);
  return SUPER::Destroy();
}
void CFrameSample::OnButton(wxCommandEvent &e)
{
  if( (e.GetId() == IDSampleApply) && nwxKeyState::Shift())
  {
    e.SetId(IDSampleApplyAll);
  }
  MenuEvent(e);
}
void CFrameSample::UpdateSizeHack(bool bForce)
{
  //  this frame does not render properly 
  //  until it is resized, so, here goes
#if 0
  SendSizeEvent(/*wxSEND_EVENT_POST*/);
#else
  wxSize szBeenHere(-5,-5);
  bool bDoIt = (m_sz != szBeenHere);
  if(bDoIt && !bForce)
  {
    bDoIt = IsShown() &&
      !(IsMaximized() || IsIconized());
  }
  if(!bDoIt) {}
  else if(m_sz == wxDefaultSize) 
  {
    m_sz = GetSize();
    m_sz.x--;
  }
  else if(m_sz == GetSize())
  {
    if(bForce)
    {
      m_sz.x--;
    }
    else
    {
      bDoIt = false;
    }
  }
  if(bDoIt)
  {
    SetSize(m_sz);
#ifdef __WXDEBUG__
    wxString s;
    s.Format(wxT("size %d x %d"),m_sz.x,m_sz.y);
    nwxLog_I_WAS_HERE_MSG(s);
#endif
    m_sz = szBeenHere;
  }
#endif
}

void CFrameSample::SetupMenuItems()
{
  bool bSampleEnabled = m_pSample->IsEnabled();
  CPageEditSample *pPage = m_pNoteBook->GetCurrentPanel();
  if(pPage != NULL)
  {
    _EnableItem(IDmenuHistory,pPage->HasHistory());
    _EnableItem(IDSampleAccept, bSampleEnabled && pPage->NeedsAcceptance());
    _EnableItem(IDSampleApprove, bSampleEnabled && pPage->NeedsReview());
    _EnableItem(IDSampleApply,bSampleEnabled && pPage->NeedsApply());
    _EnableItem(IDSampleApplyAll, m_pNoteBook->IsModified());
    _SetSampleEnabled(bSampleEnabled);
  }
}

IMPLEMENT_PERSISTENT_SIZE(CFrameSample)

BEGIN_EVENT_TABLE(CFrameSample,CMDIFrame)
EVT_PERSISTENT_SIZE(CFrameSample)
EVT_BUTTON(wxID_ANY,CFrameSample::OnButton)
END_EVENT_TABLE()



