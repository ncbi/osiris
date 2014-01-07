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

*  FileName: nwxPlotCtrl.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/wx.h>
#include <wx/cursor.h>
#include "nwx/nwxPlotCtrl.h"
#include "nwx/nwxBatch.h"
#include "clear15.xpm"

IMPLEMENT_CLASS(nwxPlotCtrl, wxPlotCtrl )

const int nwxPlotCtrl::TIMER_ELAPSE(200);
const unsigned int nwxPlotCtrl::TIMER_COUNT(3);

nwxPlotCtrl::~nwxPlotCtrl()
{
  if(m_pTimer != NULL)
  {
    delete m_pTimer;
  }
  if(m_pToolTip != NULL)
  {
    delete m_pToolTip;
  }
}

void nwxPlotCtrl::ShowScrollbars(bool b)
{
  if(m_xAxisScrollbar != NULL)
  {
    bool bCurrent = AreScrollbarsShown();
    if(b != bCurrent)
    {
      m_xAxisScrollbar->Show(b);
      m_yAxisScrollbar->Show(b);
      if(b)
      {
        m_activeBitmap = m_bmActiveBackup;
        m_inactiveBitmap = m_bmInactiveBackup;
      }
      else
      {
        m_activeBitmap = m_bmClear;
        m_inactiveBitmap = m_bmClear;
      }
      Refresh();
    }
  }
}

void nwxPlotCtrl::DrawAreaWindow(wxDC *pdc,const wxRect &rect)
{
  wxRect rClip(rect);
  int nOffset = m_Labels.GetMinY();
  wxCoord nX = 0;
  wxCoord nY = 0;
  wxCoord nW = 0;
  wxCoord nH = 0;

  wxPlotCtrl::DrawAreaWindow(pdc,rect);

  if(nOffset > 0)
  {
    int nOffset2 = nOffset << 1;
    rClip.x += nOffset;
    rClip.height -= nOffset;
    rClip.width -= nOffset2;
    pdc->GetClippingBox(&nX,&nY,&nW,&nH);
    pdc->SetClippingRegion(rClip);
  }

  // clipping region
  pdc->SetClippingRegion(rClip);
  m_Labels.Draw(pdc,true);
  if(nOffset > 0)
  {
    pdc->DestroyClippingRegion();
    if(nW > 0 && nH > 0)
    {
      pdc->SetClippingRegion(nX,nY,nW,nH);
      // this isn't necessarily the original clipping region
    }
  }

//  wxWindowDC wdc(this);
//  m_XLabels.Draw(&wdc,true);
//  m_Xshade.Draw(pdc,true);

}
void nwxPlotCtrl::DrawPlotCtrl(wxDC *dc)
{
//  ClearSelectedRanges(-1, true);
  wxPlotCtrl::DrawPlotCtrl(dc);
  m_XLabels.Draw(dc,true);
}



void nwxPlotCtrl::nwxOnMouse(wxMouseEvent &e)
{
  wxPoint pt(e.GetPosition());
  wxPoint ptArea = GetPlotArea()->GetPosition();
  bool bSkip = false;
  if(e.ButtonUp(wxMOUSE_BTN_RIGHT))
  {
    bSkip = true;
    SendContextMenuEvent(e);
  }
  else if(pt.y < ptArea.y)
  {
    pt.y -= ptArea.y;
    pt.x -= ptArea.x;
    if(e.ButtonDown(wxMOUSE_BTN_LEFT))
    {
      m_bClear = true;
      m_pLastXLabel = m_XLabels.FindLabel(pt);
    }
    else if(e.ButtonUp(wxMOUSE_BTN_LEFT))
    {
      m_bClear = true;
      if(m_pLastXLabel != NULL)
      {
        const nwxPointLabel *pLabel = m_XLabels.FindLabel(pt);
        if(pLabel == m_pLastXLabel)
        {
          OnClickXLabel(*pLabel,pt);
        }
        m_pLastXLabel = NULL;
      }
    }
    else if(e.Leaving())
    {
      m_bClear = true;
      m_pLastXLabel = NULL;
    }
    else if(!( e.Dragging() || e.Button(wxMOUSE_BTN_ANY) ))
    {
      ProcessMousePosition(pt);
    }
    else
    {
      bSkip = true;
    }
  }
  else
  {
    bSkip = true;
  }
  e.Skip(bSkip);
}

void nwxPlotCtrl::DrawEntirePlot(wxDC *dc, const wxRect &rect, double dpi)
{
  TnwxBatch<nwxPlotCtrl> XXXX(this);
  int nX ;
  int n;
  nX = m_XLabels.GetOffset();
  n = m_Labels.GetMinY();
  m_XLabels.SetDPIoffset(dpi);
  m_Labels.SetMinYDPI(dpi);
  DrawWholePlot(dc,rect,dpi,true);
  m_XLabels.SetOffset(nX);
  m_Labels.SetMinY(n);
}
void nwxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS( wxMouseEvent &event )
{
  wxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS(event);
  wxPoint pt = event.GetPosition();
  ProcessMousePosition(pt);
  if(event.ButtonUp(wxMOUSE_BTN_RIGHT))
  {
    SendContextMenuEvent(event);
  }
}

void nwxPlotCtrl::ProcessMousePosition(const wxPoint &pt)
{
  if(pt != m_PositionMouse)
  {
    m_PositionMouse = pt;
    m_nTimeHere = 0;
  }
  StartTimer();

}
void nwxPlotCtrl::_Init()
{
  SetSelectionType(wxPLOTCTRL_SELECT_NONE);
  SetGridColour(wxColour(232,232,232));
  //    SetAreaMouseMarker(wxPLOTCTRL_MARKER_NONE);

  m_bmActiveBackup = m_activeBitmap;
  m_bmInactiveBackup = m_inactiveBitmap;
  m_bmClear = wxBitmap(clear15);

}

void nwxPlotCtrl::nwxOnTimer(wxTimerEvent &)
{
  if(m_bClear)
  {
    ClearToolTip();
    m_bClear = false;
    m_nTimeHere = 0;
    StopTimer();
  }
  else
  {
    if(!m_nTimeHere)
    {
      ClearToolTip();
    }
    m_nTimeHere++;
    if(m_nTimeHere == TIMER_COUNT)
    {
      SetupToolTip();
      StopTimer();
    }
  }
}
#if __TOOLTIP_TO_FRAME__

//  tried to set the tooltip parent to the frame
//  but it created more problmes than it solved

void nwxPlotCtrl::_OffsetToolTipPosition(wxPoint *ppt)
{
  wxWindow *pWin = m_pToolTip->GetParent();
  if( pWin != (wxWindow *)this)
  {
    wxPoint ptParent = pWin->GetScreenPosition();
    wxPoint ptThis = GetScreenPosition();
    ptThis -= ptParent;
    *ppt += ptThis;
    *ppt -= ptParent;
  }
}

#endif

wxWindow *nwxPlotCtrl::_FindFrameParent()
{
  wxWindow *pRtn = this;
  if(m_pToolTip != NULL)
  {
    pRtn = m_pToolTip->GetParent();
  }
  else
  {
    wxString s;
    wxClassInfo *pInfo;
    wxWindow *pParent = pRtn->GetParent();
    bool bDone = (pParent == NULL);
    const wxChar * const list[] =
    {
      _T("wxFrame"),
      _T("wxMDIChildFrame"),
      _T("wxDialog"),
      _T("wxTopLevelWindow")
    };
    size_t LSIZE = sizeof(list) / sizeof(list[0]);
    size_t i;
    size_t j;
    while(!bDone)
    {
      pRtn = pParent;
      pInfo = pRtn->GetClassInfo();
      for(i = 0; i < 3; i++)
      {
        switch(i)
        {
        case 0:
          s = pInfo->GetClassName();
          break;
        case 1:
          s = pInfo->GetBaseClassName1();
          break;
        case 2:
          s = pInfo->GetBaseClassName2();
          break;
        }
        for(j = 0; j < LSIZE; j++)
        {
          if( !s.CmpNoCase(list[j]) )
          {
            bDone = true;
            j = LSIZE; // loop exit
            i = 32000; // loop exit
          }
        }
      }
      if(!bDone)
      {
        pParent = pParent->GetParent();
        bDone = (pParent == NULL);
      }
    }
  }
  return pRtn;
}


void nwxPlotCtrl::SetupToolTip()
{
  wxString sText;
  if(m_PositionMouse.y < 0)
  {
    sText = m_XLabels.GetToolTipText(m_PositionMouse);
  }
  else
  {
    sText = m_Labels.GetToolTipText(m_PositionMouse);
  }

  if(!sText.IsEmpty())
  {
    bool bLayout = true;
    bool bRefresh = false;
    if(m_pToolTip == NULL)
    {
      wxColour cbg =
        wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK);
      wxColour cfg =
        wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT);

#if __TOOLTIP_TO_FRAME__
      m_pToolTip = new wxPanel(GetToolTipParent());
#else
      m_pToolTip = new wxPanel(this);
#endif
      wxPanel *pPanelMargin = new wxPanel(m_pToolTip);
      m_pToolText = new wxStaticText(pPanelMargin,wxID_ANY,sText);

      pPanelMargin->SetBackgroundColour(cbg);
      m_pToolTip->SetBackgroundColour(cfg);
        // border colour is the same as text colour
      m_pToolText->SetForegroundColour(cfg);
      m_pToolText->SetBackgroundColour(cbg);

      wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
      pSizer->Add(m_pToolText,0,wxALL,2);
      pPanelMargin->SetSizer(pSizer);
      wxBoxSizer *pSizer2 = new wxBoxSizer(wxVERTICAL);
      pSizer2->Add(pPanelMargin,0,wxALL,1);
      m_pToolTip->SetSizer(pSizer2);
    }
    else if(m_pToolText->GetLabel() != sText)
    {
      m_pToolText->SetLabel(sText);
    }
    else
    {
      bLayout = false;
    }
    if(bLayout)
    {
      m_pToolTip->Layout();
      m_pToolTip->Fit();
    }

    // set position
    wxWindow *pArea = GetPlotArea();
    wxPoint pos(pArea->GetPosition());
    bool bHideCursor = false;
    pos += m_PositionMouse;
#if __TOOLTIP_TO_FRAME__
    _OffsetToolTipPosition(&pos);
#endif
    wxSize tipSize = m_pToolTip->GetSize();
    wxSize winSize = m_pToolTip->GetParent()->GetSize();
    if(pos.y < tipSize.y)
    {
      pos.y += 24;
    }
    else
    {
      pos.y -= tipSize.y;
    }
    if( (pos.y + tipSize.y) > winSize.y)
    {
      // tool tip clipped on bottom of window,
      // move it up
      pos.y = winSize.y - tipSize.y;
      if(pos.y < 0)
      {
        // clipped by top of window, move down
        pos.y = 0;
      }
      if(pos.x > tipSize.x)
      {
        pos.x -= tipSize.x;
      }
      else
      {
        // cursor will obscure tool tip
        bHideCursor = true;
      }
    }
    if( (pos.x + tipSize.x) > winSize.x)
    {
      // tool tip is clipped on the right
      // move it left
      pos.x -= tipSize.x;
      if(pos.x < 0)
      {
        // tool tip is clipped on the left
        // move right
        pos.x = 0;
        bHideCursor = true;  // cursor will obscure tool tip
      }
    }
    if(m_pToolTip->GetPosition() != pos)
    {
      m_pToolTip->Move(pos);
      bRefresh = true;
    }
    if(!m_pToolTip->IsShown())
    {
      //m_pToolTip->Raise();  // this actually lowered the window
      m_pToolTip->Show(true);
      bRefresh = true;
    }
    if(bLayout || bRefresh)
    {
      Refresh(true);
    }
    if(bHideCursor)
    {
      wxCursor c(wxCURSOR_BLANK);
      SetCursor(c);
    }
  }
  return;
}
void nwxPlotCtrl::ClearToolTip()
{
  if(m_pToolTip != NULL)
  {
    m_pToolTip->Show(false);
    SetCursor(wxNullCursor);
  }
  return;
}


void nwxPlotCtrl::OnViewChanged(wxPlotCtrlEvent &e)
{
  m_bClear = true;
  e.Skip();
}

void nwxPlotCtrl::OnClickXLabel(const nwxPointLabel &, const wxPoint &) {;}

void nwxPlotCtrl::SendContextMenuEvent(wxMouseEvent &e)
{
  wxWindow *pWin = (wxWindow *)e.GetEventObject();
  wxPoint ptTop = e.GetPosition();

  // check if this is a mouse or KB event
  bool bMouse = (ptTop.x >= 0) && (ptTop.y >= 0);
  if(bMouse)
  {
    // set screen coordinates for mouse
    ptTop += pWin->GetScreenPosition();
  }
/*
  for(pWin = (wxWindow *) e.GetEventObject();
    (pWin != this) && (pWin != NULL);
    pWin = pWin->GetParent())
  {;}
*/
  wxWindow *ptParent = _FindFrameParent();
  wxContextMenuEvent ev(wxEVT_CONTEXT_MENU,GetId(),ptTop);
  ev.SetEventObject(ptParent);
  ev.SetExtraLong(0);
  GetEventHandler()->AddPendingEvent(ev);
}

BEGIN_EVENT_TABLE(nwxPlotCtrl,wxPlotCtrl)

EVT_TIMER(wxID_ANY,nwxPlotCtrl::nwxOnTimer)
EVT_PLOTCTRL_VIEW_CHANGED(wxID_ANY, nwxPlotCtrl::OnViewChanged)
EVT_MOUSE_EVENTS     ( nwxPlotCtrl::nwxOnMouse )
END_EVENT_TABLE()
