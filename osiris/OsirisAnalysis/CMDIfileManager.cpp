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
*  FileName: CMDIfileManager.cpp
*  Author:   Douglas Hoffman
*  Purpose:  Used for managing CMDIFrame and COARfile objects when
*    they are used by one or more windows.  When a window is deleted,
*    its association with a COARfile object is removed and
*    when the COARfile object is no longer used it is deleted.
*/
#include <wx/menu.h>
#include <wx/utils.h>
#include "nwx/nwxString.h"
#include "CMDIfileManager.h"
#include "CMDIFrame.h"
#include "CFrameRunAnalysis.h"
#include "CFrameAnalysis.h"
#include "COARfile.h"
#include "CFramePlot.h"
#include "mainApp.h"

CMDIfileManager::~CMDIfileManager()
{
  wxASSERT_MSG(
    m_mapFileWindows.empty(),
    "in CMDIfileManager destructor and NOT EMPTY!!!");
}


void CMDIfileManager::UpdateHistory(COARfile *pFile)
{
  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    set<CMDIFrame *>::iterator itrF;
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      (*itrF)->UpdateHistory();
    }
  }
}
void CMDIfileManager::UpdateSamplePlot(COARfile *pFile, const wxString &sSampleName)
{
  // call this after a sample using this OAR file
  // has been edited in the analysis window
  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    set<CMDIFrame *>::iterator itrF;
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      if((*itrF)->GetType() == CMDIFrame::FRAME_PLOT)
      {
        CFramePlot *pFrame = (CFramePlot *)(*itrF);
        pFrame->UpdateOARfile(sSampleName);
      }
    }
  }
}
void CMDIfileManager::UpdateOARfile(COARfile *pFile, bool bUpdateAnalysis)
{
  // call this when the OAR file has been reloaded,
  // modified, etc

  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    set<CMDIFrame *>::iterator itrF;
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      if((*itrF)->GetType() == CMDIFrame::FRAME_PLOT)
      {
        CFramePlot *pFrame = (CFramePlot *)(*itrF);
        pFrame->UpdateHistory();
      }
      else if(bUpdateAnalysis && (*itrF)->GetType() == CMDIFrame::FRAME_ANALYSIS)
      {
        CFrameAnalysis *pFrame = (CFrameAnalysis *)(*itrF);
        pFrame->TransferDataToWindow();
        pFrame->SetupTitle();
      }
    }
  }
}

void CMDIfileManager::UpdateLadderLabels()
{
  CMDI_WF::iterator itr;
  for(itr = m_mapWindowFile.begin();
    _IteratorOK(itr);
    ++itr)
  {
    itr->first->UpdateLadderLabels();
  }
}

void CMDIfileManager::UpdateFileMenu()
{
  CMDI_WF::iterator itr;
  for(itr = m_mapWindowFile.begin();
    _IteratorOK(itr);
    ++itr)
  {
    itr->first->UpdateFileMenu();
  }
}

void CMDIfileManager::KillOARfile(COARfile *pFile)
{
  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    set<CMDIFrame *>::iterator itrF;
    vector<CMDIFrame *> vpFrame;
    vpFrame.reserve(itr->second.size());
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      vpFrame.push_back(*itrF);
    }
    for(vector<CMDIFrame *>::iterator itrX = vpFrame.begin();
      itrX != vpFrame.end();
      ++itrX)
    {
      (*itrX)->Destroy();
    }
  }
}
CFrameAnalysis *CMDIfileManager::FindAnalysisFrame(COARfile *pFile)
{
  CFrameAnalysis *pRtn(NULL);
  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    set<CMDIFrame *>::iterator itrF;
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      if((*itrF)->GetType() == CMDIFrame::FRAME_ANALYSIS)
      {
        pRtn = (CFrameAnalysis *)(*itrF);
        break;
      }
    }
  }
  return pRtn;
}

void CMDIfileManager::DiscardChanges(COARfile *pFile)
{
  CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
  if(_IteratorOK(itr))
  {
    bool bReloaded = false;
    bool bKill = false;
    set<CMDIFrame *>::iterator itrF;
    for(itrF = itr->second.begin();
      itrF != itr->second.end();
      ++itrF)
    {
      if((*itrF)->GetType() != CMDIFrame::FRAME_PLOT) {;}
      else if(bReloaded || pFile->ReloadFile())
      {
        CFramePlot *pFrame = (CFramePlot *)(*itrF);
        pFrame->UpdateHistory();
        bReloaded = true;
      }
      else
      {
        bKill = true;
        break;
      }
    }
    if(bKill)
    {
      // we had a problem reloading the OAR file,
      // so set all plots to have NO oar file
      // since this removes the frame from the itr->second
      // set<> we will first copy all frames to a vector
      // otherwise looping through a set<>::iterator
      // while the set<> is changing will cause a crash
      //
      vector<CFramePlot *> vpFrame;
      vpFrame.reserve(itr->second.size());
      for(itrF = itr->second.begin();
        itrF != itr->second.end();
        ++itrF)
      {
        if((*itrF)->GetType() == CMDIFrame::FRAME_PLOT)
        {
          vpFrame.push_back((CFramePlot *)(*itrF));
        }
      }
      for(vector<CFramePlot *>::iterator itrV = vpFrame.begin();
        itrV != vpFrame.end();
        ++itrV)
      {
        (*itrV)->SetOARfile(NULL);
      }
    }
  }
}


CMDIFrame *CMDIfileManager::FindWindowByName(
  const wxString &sPath, bool bRaise)
{
  CMDIFrame *pRtn(NULL);
  wxFileName fn(sPath);
  fn.MakeAbsolute();
  wxString sFileName = fn.GetFullPath();
  wxString sCheck;
  for(CMDI_WF::iterator itr = m_mapWindowFile.begin();
    _IteratorOK(itr);
    ++itr)
  {
    wxFileName fnCheck( itr->first->GetFileName() );
    fnCheck.MakeAbsolute();
    sCheck = fnCheck.GetFullPath();
    if(nwxString::FileNameStringEqual(sCheck,sFileName))
    {
      pRtn = itr->first;
      if(bRaise)
      {
        pRtn->RaiseWindow();
      }
      break;
    }
  }
  return pRtn;
}
int CMDIfileManager::RunningFrameCount()
{
  int nRtn = 0;
  CMDI_WF::iterator itr;
  CMDIFrame *pFrame;

  for(itr = m_mapWindowFile.begin();
    _IteratorOK(itr);
    ++itr)
  {
    pFrame = itr->first;
    if(pFrame->GetType() == CMDIFrame::FRAME_RUN)
    {
      CFrameRunAnalysis *pFrameRun = (CFrameRunAnalysis *)(pFrame);
      if(!pFrameRun->CheckIsDone())
      {
        nRtn++;
      }
    }
  }
  return nRtn;
}

int CMDIfileManager::EditedFiles()
{
  int nRtn = 0;
  CMDI_WF::iterator itr;
  for(itr = m_mapWindowFile.begin();
      _IteratorOK(itr);
      ++itr)
  {
    if(itr->first->IsEdited())
    {
      nRtn++;
    }
  }
  return nRtn;
}

void CMDIfileManager::RefreshAllOAR()
{
  wxBusyCursor xxx;
  CMDI_WF::iterator itr;
  CFrameAnalysis *pf;
  for(itr = m_mapWindowFile.begin();
      _IteratorOK(itr);
      ++itr)
  {
    if(itr->first->GetType() == CMDIFrame::FRAME_ANALYSIS)
    {
      pf = (CFrameAnalysis *)(itr->first);
      pf->RepaintData();
    }
  }
}
bool CMDIfileManager::CloseAll()
{
  size_t nSize = m_mapWindowFile.size();
  int nRunning = nSize ? RunningFrameCount() : 0;
  bool bRtn = !nRunning;
  if(nSize && bRtn)
  {
    CMDI_WF::iterator itr;
    CMDIFrame *pf;

    //  copy all frame pointers because m_mapWindowFile
    //  will change each time a window is closed

    vector<CMDIFrame *> vpFrame;
    vpFrame.reserve(nSize);

    for(itr = m_mapWindowFile.begin();
      bRtn && _IteratorOK(itr);
      ++itr)
    {
      vpFrame.push_back(itr->first);
    }

    for(vector<CMDIFrame *>::iterator itrv = vpFrame.begin();
      itrv != vpFrame.end();
      ++itrv)
    {
      pf = *itrv;
      if(!pf->Close(false))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}
/*

// replaced with use of nwxTimerReceiver

void CMDIfileManager::OnTimer(wxTimerEvent &e)
{
  CMDI_WF::iterator itr;
  for(itr = m_mapWindowFile.begin();
      _IteratorOK(itr);
      ++itr)
  {
    itr->first->OnTimer(e);
  }
}
*/
void CMDIfileManager::InsertWindow(CMDIFrame *pWin, COARfile *pFile)
{
  CMDI_WF::iterator itr = m_mapWindowFile.find(pWin);
  bool bDone = false;
  if(!_IteratorOK(itr))
  {}
  else if(itr->second == pFile)
  {
    bDone = true;
  }
  else
  {
    _RemoveIterator(itr);
  }
  if(!bDone)
  {
    m_mapWindowFile.insert(CMDI_WF::value_type(pWin,pFile));
    _InsertCOARfile(pFile,pWin);
  }
}

void CMDIfileManager::RemoveWindow(CMDIFrame *pWin)
{
  CMDI_WF::iterator itr = m_mapWindowFile.find(pWin);
  if(_IteratorOK(itr))
  {
    _RemoveIterator(itr);
  }
}

COARfile *CMDIfileManager::FindOARfile(const wxString &sName)
{
  CMDI_FW::iterator itr;
  COARfile *pRtn(NULL);
  for(itr = m_mapFileWindows.begin();
    itr != m_mapFileWindows.end();
    ++itr)
  {
    if(nwxString::FileNameStringEqual(
      itr->first->GetLastFileName(),sName))
    {
      pRtn = itr->first;
      break;
    }
  }
  return pRtn;
}

bool CMDIfileManager::FindWindow(CMDIFrame *pWin)
{
  CMDI_WF::iterator itr = m_mapWindowFile.find(pWin);
  bool bRtn = _IteratorOK(itr);
  return bRtn;
}
void CMDIfileManager::_RemoveCOARfile(COARfile *pFile, CMDIFrame *pWin)
{
  if(pFile != NULL)
  {
    CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
    if(_IteratorOK(itr))
    {
      set<CMDIFrame *>::iterator itrw = itr->second.find(pWin);
      if(itrw != itr->second.end())
      {
        itr->second.erase(itrw);
        if(!itr->second.size())
        {
          // COARobject is no longer used, delete it
          delete pFile;
          m_mapFileWindows.erase(itr);
        }
      }
    }
  }
}

void CMDIfileManager::_InsertCOARfile(COARfile *pFile, CMDIFrame *pWin)
{
  if(pFile != NULL)
  {
    CMDI_FW::iterator itr = m_mapFileWindows.find(pFile);
    if(!_IteratorOK(itr))
    {
      set<CMDIFrame *> x;
      pair<CMDI_FW::iterator,bool> pr;
      pr = m_mapFileWindows.insert(CMDI_FW::value_type(pFile,x));
      itr = pr.first;
    }
    itr->second.insert(pWin);
  }
}

