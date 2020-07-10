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
*  FileName: nwxShiftSizer.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/cursor.h>
#include <wx/scrolwin.h>
#include "nwxHoldScrollPos.h"
#include "nwxStaticBitmap.h"
#include "nwxShiftSizer.h"
#include "nwxShiftBitmaps.h"
#include "nwxKeyState.h"


class nwxArrowBitmap : public wxStaticBitmap
{
public:
  nwxArrowBitmap(
    wxWindow *parent, 
    nwxShiftSizer *pSizer, 
    bool bLeft);
  virtual ~nwxArrowBitmap();
  virtual bool Enable(bool enable = true);
  void OnBegin(wxMouseEvent &e);
  void OnEnd(wxMouseEvent &e);
  //void OnTimer(wxTimerEvent &e); // EXT TIMER
private:
  wxBitmap m_bitmapEnabled;
  wxBitmap m_bitmapDisabled;
  nwxShiftSizer *m_pSizer;
  DECLARE_EVENT_TABLE();
};


nwxArrowBitmap::~nwxArrowBitmap() {;}

nwxArrowBitmap::nwxArrowBitmap(
  wxWindow *parent, 
  nwxShiftSizer *pSizer, 
  bool bLeft) : 
    m_pSizer(pSizer)
{
  void *pBuffer;
  void *pBufferDisabled;
  size_t nSize;
  size_t nSizeDisabled;
  if(bLeft)
  {
    pBuffer = (void *)shiftLeft;
    nSize = sizeof(shiftLeft);
    pBufferDisabled = (void *) shiftLeftDisabled;
    nSizeDisabled = sizeof(shiftLeftDisabled);
  }
  else
  {
    pBuffer = (void *)shiftRight;
    nSize = sizeof(shiftRight);
    pBufferDisabled = (void *) shiftRightDisabled;
    nSizeDisabled = sizeof(shiftRightDisabled);
  }
  nwxStaticBitmap::AddPngHandler();
  nwxStaticBitmap::LoadImage(
    pBuffer,nSize,&m_bitmapEnabled,wxBITMAP_TYPE_PNG);
  nwxStaticBitmap::LoadImage(
    pBufferDisabled,nSizeDisabled,&m_bitmapDisabled,wxBITMAP_TYPE_PNG);
  Create(parent,wxID_ANY,m_bitmapEnabled);
  wxStockCursor nID = bLeft ? wxCURSOR_POINT_LEFT : wxCURSOR_POINT_RIGHT;
  wxCursor cur(nID);
  SetCursor(cur);
  SetToolTip(
    "Click here to view\n"
    "more controls."
    );
}

bool nwxArrowBitmap::Enable(bool enable)
{
  bool bRtn = wxStaticBitmap::Enable(enable);
  if(bRtn)
  {
    if(enable)
    {
      SetBitmap(m_bitmapEnabled);
    }
    else
    {
      SetBitmap(m_bitmapDisabled);
    }
  }
  return bRtn;
}

void nwxArrowBitmap::OnBegin(wxMouseEvent &)
{
  if(IsEnabled())
  {
    m_pSizer->BeginShift(this);
  }
}
void nwxArrowBitmap::OnEnd(wxMouseEvent &)
{
  if(IsEnabled())
  {
    m_pSizer->EndShift();
  }
}
/*
 // EXT TIMER
void nwxArrowBitmap::OnTimer(wxTimerEvent &e)
{
  m_pSizer->OnTimer(e);
}
*/
BEGIN_EVENT_TABLE(nwxArrowBitmap,wxStaticBitmap)
EVT_LEFT_DOWN(nwxArrowBitmap::OnBegin)
EVT_LEFT_UP(nwxArrowBitmap::OnEnd)
EVT_LEAVE_WINDOW(nwxArrowBitmap::OnEnd)
//EVT_TIMER(wxID_ANY,nwxArrowBitmap::OnTimer) // EXT TIMER
END_EVENT_TABLE()


//**************************************************** nwxShiftSizer
nwxShiftSizer::~nwxShiftSizer()
{
  // EXT TIMER
  /*
  if(m_pTimer != NULL)
  {
    delete m_pTimer;
    m_pTimer = NULL;
  }
  */
}

nwxShiftSizer::nwxShiftSizer(
  wxScrolledWindow *pPeerWindow, 
  InwxShiftReceiver *pReceiver, 
  int nBorder,
  int nWaitInterval
  //,bool bUseExternalTimer // EXT TIMER
  ) : 
    wxBoxSizer(wxHORIZONTAL),
    m_pReceiver(pReceiver),
    //m_pTimer(NULL), // EXT TIMER
    m_pPeerWindow(pPeerWindow),
    m_pCurrent(NULL),
    m_nTotalInterval(0),
    m_nWaitInterval(nWaitInterval),
    m_nBorder(nBorder),
    m_bIgnoreTimer(false)
    //,m_bUseExternalTimer(bUseExternalTimer) // EXT TIMER
{
  wxWindow *parent = pPeerWindow->GetParent();
  m_pButtonLeft = new nwxArrowBitmap(parent,this,true);
  m_pButtonRight = new nwxArrowBitmap(parent,this,false);
  _SetupLayout();
  /*
  if(!m_bUseExternalTimer)
  {
    //m_pTimer = new wxTimer(m_pButtonLeft,wxID_ANY);
  }
  */
  _UpdateButtons();
}
void nwxShiftSizer::_SetupLayout(bool bShow)
{
  Clear();
  if(bShow)
  {
    Add(m_pButtonLeft,0,wxALIGN_CENTER);
    if(m_nBorder)
    {
      AddSpacer(m_nBorder);
    }
    Add(m_pPeerWindow,1,wxEXPAND);
    if(m_nBorder)
    {
      AddSpacer(m_nBorder);
    }
    Add(m_pButtonRight,0,wxALIGN_CENTER);
  }
}
void nwxShiftSizer::BeginShift(wxStaticBitmap *p)
{
  m_pCurrent = p;
  //if(_DoShift() && (m_pTimer != NULL)) // EXT TIMER
  if(_DoShift())
  {
    m_bIgnoreTimer = false;
  //    m_pTimer->Start(m_nWaitInterval); // EXT TIMER
  }
}
void nwxShiftSizer::EndShift()
{
  m_pCurrent = NULL;
  m_nTotalInterval = 0;
  m_bIgnoreTimer = true;
  /*
  // EXT TIMER
  if((m_pTimer != NULL) && m_pTimer->IsRunning())
  {
    m_pTimer->Stop();
  }
  */
  _UpdateButtons();
}

void nwxShiftSizer::OnTimer(wxTimerEvent &e)
{
  if( (m_pCurrent != NULL) && (!m_bIgnoreTimer) )
  {
    m_nTotalInterval += e.GetInterval();
    if(m_nTotalInterval >= m_nWaitInterval)
    {
      _DoShift();
      m_nTotalInterval = 0;
    }
  }
}
bool nwxShiftSizer::Show(bool bShow)
{
  bool bRtn = false;
  if(m_pPeerWindow->IsShown() != bShow)
  {
    bRtn = true;
    m_pPeerWindow->Show(bShow);
    if(bShow)
    {
      _UpdateButtons();
    }
    else
    {
      _ShowButtons(false);
    }
    _SetupLayout(bShow);
    m_pPeerWindow->GetParent()->Layout();
  }
  return bRtn;
}
bool nwxShiftSizer::_ShowButtons(bool bShow)
{
  // return true if this show/hide state was changed
  bool bRtn = false;
  if(m_pButtonLeft->IsShown() != bShow)
  {
    m_pButtonLeft->Show(bShow);
    m_pButtonRight->Show(bShow);
    bRtn = true;
    Layout();
  }
  return bRtn;
}
void nwxShiftSizer::UpdateSize()
{
  _UpdateButtons();
}

bool nwxShiftSizer::_UpdateButtons()
{
  // return true if buttons are shown or hidden
  //  false otherwise
  // i.e. true if a Layout() was called
  //
  bool bRtn = false;
  if(m_pPeerWindow->IsShown())
  {
    bool bLeft = m_pReceiver->CanShiftLeft();
    bool bRight = m_pReceiver->CanShiftRight();
    bool bEither = (bLeft || bRight);
    bRtn = _ShowButtons(bEither);
    if(bEither)
    {
      m_pButtonLeft->Enable(bLeft);
      m_pButtonRight->Enable(bRight);
    }
    Layout();
  }
  return bRtn;
}

bool nwxShiftSizer::_DoShift()
{
  bool bRtn = false;
  if(m_pCurrent != NULL)
  {
    bool bShift = nwxKeyState::Shift();
    if(m_pCurrent == m_pButtonLeft)
    {
      m_pReceiver->ShiftLeft(bShift);
      bRtn = m_pReceiver->CanShiftLeft();
    }
    else if(m_pCurrent == m_pButtonRight)
    {
      m_pReceiver->ShiftRight(bShift);
      bRtn = m_pReceiver->CanShiftRight();
    }
  }
  if(!bRtn)
  {
    EndShift();
  }
  else
  {
    _UpdateButtons();
  }
  return bRtn;
}


