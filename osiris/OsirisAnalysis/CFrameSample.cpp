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
    m_pPanel(NULL),
    m_pCreator(pCreator),
    m_pParent(parent),
    m_pOARfile(pFile),
    m_pSample(pSample)
{
  SetupTitle();
  m_pPanel = new CNotebookEditSample(m_pOARfile,m_pSample,this,wxID_ANY,NULL);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pPanel,1,wxEXPAND,0);
  SetSizer(pSizer);
  Layout();
  m_pParent->InsertWindow(this,m_pOARfile);
  SetMenuBar(new CMenuBarSample());
}
CFrameSample::~CFrameSample() {}

int CFrameSample::GetType()
{
  return FRAME_SAMPLE;
}

void CFrameSample::SetupTitle()
{
  wxString s = m_pOARfile->GetFileName();
  bool bMod = false;
  const CParmOsiris &parm(m_pOARfile->GetParameters());
  SetTitle(mainApp::FormatWindowTitle(s,bMod,&parm,NULL));
}

void CFrameSample::OnTimer(wxTimerEvent &) {}
bool CFrameSample::TransferDataToWindow()
{
  return m_pPanel->TransferDataToWindow();
}
bool CFrameSample::MenuEvent(wxCommandEvent &e)
{
  int nID = e.GetId();
  bool bRtn = true;
  switch(nID)
  {
  case IDmenuDisplayGraph:
    break;
  case IDmenuTable:
    break;
  case IDmenuHistory:
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
void CFrameSample::OnClose(wxCloseEvent &)
{
  m_pCreator->RemoveSample(m_pSample,this);
  Destroy();
}

BEGIN_EVENT_TABLE(CFrameSample,CMDIFrame)
EVT_SET_FOCUS(CFrameSample::OnFocusSet)
EVT_KILL_FOCUS(CFrameSample::OnFocusKill)
EVT_CLOSE(CFrameSample::OnClose)
END_EVENT_TABLE()


