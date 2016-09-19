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
*  FileName: nwxXmlWindowSizes.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxXmlWindowSizes.h"
#include "nwx/nwxLog.h"
#include "nwx/CleanupGlobal.h"
#include <wx/gdicmn.h>
const wxString nwxXmlWindowSizes::g_sROOT(wxT("sizes"));

#define INTERVAL_WAIT_TIME 10000

nwxXmlWindowSizes::nwxXmlWindowSizes(const wxString &sPath) :
  m_interval(INTERVAL_WAIT_TIME,-1),
  m_sPath(sPath),
  m_bLoaded(false),
  m_bLocallyModified(false),
  m_bCanCheckFile(true),
  m_bCanSaveFile(false)
{
   _Clear(true);
  RegisterAll(true);
  m_sPath = sPath;
  _CheckLoadFile();
}
void nwxXmlWindowSizes::_CheckSaveFile(bool bForce)
{
  if(m_bLocallyModified && (bForce || m_bCanSaveFile))
  {
    if(!SaveFile(m_sPath))
    {
      wxString s(wxT("Failed to save window sizes: "));
      s.Append(m_sPath);
      nwxLog::LogMessage(s);
    }
    else
    {
      m_bLocallyModified = false;
      m_bLoaded = true;
    }
    _ResetInterval();
  }
}
void nwxXmlWindowSizes::_CheckLoadFile(bool bForce)
{
  if(m_bLocallyModified || !(m_bCanCheckFile || bForce))
  {}
  else if(!m_bLoaded)
  {
    wxFileName fn(m_sPath);
    if(fn.FileExists())
    {
      m_sPath = fn.GetFullPath();
      if(LoadFile(m_sPath,true))
      {
        m_bLoaded = true;
      }
      Unlock();
      _ResetInterval();
    }
  }
  else
  {
    if(CheckFileModification(true))
    {
      _ResetInterval();
    }
  }
}

void nwxXmlWindowSizes::OnTimer(wxTimerEvent &e)
{
  if(m_interval.CheckTimer(e))
  {
    m_bCanSaveFile = true;
    m_bCanCheckFile = true;
    if(m_bLocallyModified)
    {
      _CheckSaveFile();
    }
    
  }
}

bool nwxXmlWindowSizes::SizeWithinScreen(const wxSize &sz)
{
  wxRect r = wxGetClientDisplayRect();
  return(
    (sz.GetWidth() <= r.GetWidth()) &&
    (sz.GetHeight() <= r.GetHeight())
    );  
}
bool nwxXmlWindowSizes::PositionWithinScreen(const wxPoint &pt)
{
  wxRect r = wxGetClientDisplayRect();
  return r.Contains(pt);
}

nwxWindowSize *nwxXmlWindowSizes::_FindWindowSize(const wxString &sName, bool bCreate)
{
  _CheckLoadFile();
  nwxWindowSize x(sName);
  nwxWindowSize *pRtn = NULL;
  std::set<nwxWindowSize *,nwxXmlWindowSizesLess> *pSet = m_setSize.Get();
  std::set<nwxWindowSize *,nwxXmlWindowSizesLess>::iterator itr =
    pSet->find(&x);
  std::set<nwxWindowSize *,nwxXmlWindowSizesLess>::iterator itrEnd =
    pSet->end();
  if(itr != itrEnd)
  {
    pRtn = *itr;
  }
  else if(bCreate)
  {
    pRtn = new nwxWindowSize(x);
    pSet->insert(pRtn);
  }
  return pRtn;
}
nwxSplitterPos *nwxXmlWindowSizes::_FindSplitterPos(const wxString &sName, bool bCreate)
{
  _CheckLoadFile();
  nwxSplitterPos x(sName);
  nwxSplitterPos *pRtn = NULL;
  std::set<nwxSplitterPos *,nwxXmlWindowSizesLess> *pSet = m_setSplitter.Get();
  std::set<nwxSplitterPos *,nwxXmlWindowSizesLess>::iterator itr =
    pSet->find(&x);
  if(itr != pSet->end())
  {
    pRtn = *itr;
  }
  else if(bCreate)
  {
    pRtn = new nwxSplitterPos(x);
    pSet->insert(pRtn);
  }
  return pRtn;
}

void nwxXmlWindowSizes::SaveWindowSize(const wxString &sName, const wxSize &sz)
{
  if(SizeWithinScreen(sz))
  {
    nwxWindowSize *pSize = _FindWindowSize(sName,true);
    if(pSize->SetSize(sz)) { m_bLocallyModified = true; }
  }
}
void nwxXmlWindowSizes::SaveWindowPos(const wxString &sName, const wxPoint &pt)
{
  if(PositionWithinScreen(pt))
  {
    nwxWindowSize *pSize = _FindWindowSize(sName,true);
    if(pSize->SetPos(pt)) { m_bLocallyModified = true; }
  }
}

void nwxXmlWindowSizes::SaveWindowSizePos(wxWindow *pwin, const wxString &sName)
{
  wxSize sz(pwin->GetSize());
  wxPoint pt(pwin->GetPosition());
  nwxWindowSize *pSize = _FindWindowSize(sName,true);
  if(PositionWithinScreen(pt) && pSize->SetPos(pt))
  { m_bLocallyModified = true; }
  if(SizeWithinScreen(sz) && pSize->SetSize(sz))
  { m_bLocallyModified = true; }
}

const wxSize &nwxXmlWindowSizes::GetWindowSize(const wxString &sName)
{
  nwxWindowSize *psz = _FindWindowSize(sName,false);
  if(psz == NULL) {  return wxDefaultSize; }
  const wxSize &rtn = psz->GetSize();
  if(SizeWithinScreen(rtn))
  {
    return rtn;
  }
  return wxDefaultSize;
}

const wxPoint &nwxXmlWindowSizes::GetWindowPos(const wxString &sName)
{
  nwxWindowSize *psz = _FindWindowSize(sName,false);
  if(psz == NULL) { return wxDefaultPosition; }
  const wxPoint &rtn = psz->GetPos();
  if(PositionWithinScreen(rtn))
  {
    return rtn;
  }
  return wxDefaultPosition;
}

void nwxXmlWindowSizes::SaveSplitterPos(const wxString &sName, int n)
{
  nwxSplitterPos *psp = _FindSplitterPos(sName,true);
  if(psp->SetPosition(n)) { m_bLocallyModified = true; }
}

int nwxXmlWindowSizes::GetSplitterPos(const wxString &sName)
{
  nwxSplitterPos *psp = _FindSplitterPos(sName,false);
  int nRtn = (psp == NULL) ? -1 : psp->GetPosition();
  return nRtn;
}

nwxXmlWindowSizes *nwxXmlWindowSizes::g_pXWS = NULL;


SETUP_CLEANUP_GLOBAL(nwxXmlWindowSizes)
