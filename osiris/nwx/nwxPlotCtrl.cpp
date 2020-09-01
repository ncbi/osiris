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
#include "nwx/nwxLog.h"
#include "clear15.xpm"

IMPLEMENT_CLASS(nwxPlotCtrl, wxPlotCtrl )

const unsigned int nwxPlotCtrl::TIMER_COUNT(3);

nwxPlotCtrl::~nwxPlotCtrl()
{
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
void nwxPlotCtrl::DrawBins(wxDC *dc)
{
  if ((m_pSetBins != NULL) && m_pSetBins->size())
  {
    nwxPlotBinSet::const_iterator itr;
    wxRect clientRect(GetPlotAreaRect());
    wxRect rect(0, clientRect.GetTop(), 1, clientRect.GetHeight());
    wxColour colour(*wxWHITE);
    int nx1, nx2;
    double dx1, dx2;
    wxBrush brush(*wxWHITE_BRUSH);
#define USE_GRID_COLOR
#ifdef USE_GRID_COLOR
    brush.SetColour(GetGridColour());
    dc->SetBrush(brush);
#endif
    dc->SetPen(*wxTRANSPARENT_PEN);
    for (itr = m_pSetBins->begin(); itr != m_pSetBins->end(); ++itr)
    {
      dx1 = (*itr).GetMin();
      dx2 = (*itr).GetMax();
      if (dx1 < m_viewRect.GetRight() && dx2 > m_viewRect.GetLeft())
      {
        nx1 = GetClientCoordFromPlotX(dx1);
        nx2 = GetClientCoordFromPlotX(dx2);
        nx1 = wxMax(nx1, clientRect.GetLeft());
        nx2 = wxMin(nx2, clientRect.GetRight());
        if (nx1 < nx2)
        {
#ifndef USE_GRID_COLOR
          const wxColour &c((*itr).GetColour());
          if (c != colour)
          {
            colour = c;
            brush.SetColour(c);
            dc->SetBrush(brush);
          }
#endif
          rect.SetLeft(nx1);
          rect.SetWidth(nx2 - nx1);
          dc->DrawRectangle(rect);
        }
      }        
    }
  }
}

void nwxPlotCtrl::DrawTickMarks(wxDC *dc, const wxRect& rect)
{
  DrawBins(dc);
  // OS-1432 - vertical bars
  wxPlotCtrl::DrawTickMarks(dc, rect);
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
  }

  // clipping region
  pdc->GetClippingBox(&nX, &nY, &nW, &nH);
  bool bClipped = !!(nX | nY | nW | nH);
  if (bClipped)
  {
    pdc->DestroyClippingRegion();
  }
  pdc->SetClippingRegion(rClip);
  m_Labels.Draw(pdc,true);
  pdc->DestroyClippingRegion();
  if(bClipped)
  {
      pdc->SetClippingRegion(nX,nY,nW,nH);
  }

//  wxWindowDC wdc(this);
//  m_XLabels.Draw(&wdc,true);
//  m_Xshade.Draw(pdc,true);

}

void nwxPlotCtrl::DrawPlotCtrl(wxDC *dc)
{
  wxPlotCtrl::DrawPlotCtrl(dc);
  if (GetBatchCount() && RenderingToWindow())
  {
    // there is a problem with wxPlotCtrl::m_areaRect
    // so the XLabels will be drawn upon EndBatch()
    RefreshOnEndBatch();
  }
  else
  {
    m_XLabels.Draw(dc, true);
  }
}

bool nwxPlotCtrl::_OnMouseDown(wxMouseEvent &e)
{
  wxPoint pt(e.GetPosition());
  m_pLastLabel = m_Labels.FindLabel(pt);
  return false;
}
bool nwxPlotCtrl::_OnMouseDownXLabel(wxMouseEvent &e)
{
  wxPoint pt(e.GetPosition());
  wxPoint ptArea = GetPlotArea()->GetPosition();
  bool bSkip = false;
  if(pt.y < ptArea.y)
  {
    pt.y -= ptArea.y;
    pt.x -= ptArea.x;
    m_pLastXLabel = m_XLabels.FindLabel(pt);
  }
  else
  {
    bSkip = _OnMouseDown(e);
  }
  return bSkip;
}
bool nwxPlotCtrl::_OnMouseUp(wxMouseEvent &e)
{
  if(m_pLastLabel != NULL)
  {
    wxPoint pt(e.GetPosition());
    const nwxPointLabel *pLabel = m_Labels.FindLabel(pt);
    if(pLabel == m_pLastLabel)
    {
      OnClickLabel(*pLabel,pt);
    }
  }
  return false;
}
bool nwxPlotCtrl::_OnMouseUpXLabel(wxMouseEvent &e)
{
  wxPoint pt(e.GetPosition());
  wxPoint ptArea = GetPlotArea()->GetPosition();
  bool bSkip = false;
  bool bOutside = (pt.y < ptArea.y);
  if(!bOutside)
  {
    bSkip = _OnMouseUp(e);
  }
  else if(m_pLastXLabel != NULL)
  {
    pt.y -= ptArea.y;
    pt.x -= ptArea.x;
    const nwxPointLabel *pLabel = m_XLabels.FindLabel(pt);
    if(pLabel == m_pLastXLabel)
    {
      m_pLastXLabel = NULL;
      _ClearToolTip();
      OnClickXLabel(*pLabel,pt);
    }
  }
  return bSkip;
}

void nwxPlotCtrl::nwxOnMouse(wxMouseEvent &e)
{
  // mouse event outside the plot area
  wxPoint pt(e.GetPosition());
  wxPoint ptArea = GetPlotArea()->GetPosition();
  bool bSkip = false;
  if(e.ButtonUp(wxMOUSE_BTN_RIGHT))
  {
    bSkip = true;
    SendContextMenuEvent(e);
  }
  else if(e.ButtonDown(wxMOUSE_BTN_LEFT))
  {
    bSkip = _OnMouseDownXLabel(e);
  }
  else if(e.ButtonUp(wxMOUSE_BTN_LEFT))
  {
    bSkip = _OnMouseUpXLabel(e);
  }
  else if(e.Leaving())
  {
    ClearToolTip();
    m_pLastXLabel = NULL;
    m_pLastLabel = NULL;
  }
  else if(!( e.Dragging() || e.Button(wxMOUSE_BTN_ANY) ))
  {
    pt.x -= ptArea.x;
    pt.y -= ptArea.y;
    ProcessMousePosition(pt);
  }
  else
  {
    bSkip = true;
  }
  e.Skip(bSkip);
}
void nwxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS( wxMouseEvent &event )
{
  // mouse events inside the plot area
  wxPlotCtrl::ProcessAreaEVT_MOUSE_EVENTS(event);
  wxPoint pt = event.GetPosition();
  if(event.ButtonUp(wxMOUSE_BTN_LEFT))
  {
    _OnMouseUp(event);
  }
  else if(event.ButtonDown(wxMOUSE_BTN_LEFT))
  {
    _OnMouseDown(event);
  }
  else if(event.ButtonUp(wxMOUSE_BTN_RIGHT))
  {
    SendContextMenuEvent(event);
  }
  else
  {
    ProcessMousePosition(pt);
  }
}

#ifdef __WXDEBUG__
void nwxPlotCtrl::_LogMouseUp(const wxPoint &pt,const nwxPointLabel *pLabel)
{
  wxString s = wxString::Format( wxT("Button up %d,%d\n"),pt.x,pt.y);

  if(pLabel != NULL)
  {
    s += pLabel->GetLabel();
    s += wxChar('\n');
    s += pLabel->GetToolTip();
  }
  nwxLog::LogMessage(s);
}
#endif
void nwxPlotCtrl::DrawEntirePlot(wxDC *dc, const wxRect &rect, double dpi, bool bForcePrintFont)
{
  TnwxBatch<nwxPlotCtrl> XXXX(this);
  int nX ;
  int n;
  nX = m_XLabels.GetOffset();
  n = m_Labels.GetMinY();
  m_XLabels.SetDPIoffset(dpi);
  m_Labels.SetMinYDPI(dpi);
  DrawWholePlot(dc,rect,dpi,true, bForcePrintFont);
  m_XLabels.SetOffset(nX);
  m_Labels.SetMinY(n);
}

void nwxPlotCtrl::ProcessMousePosition(const wxPoint &pt)
{
  if(pt != m_PositionMouse)
  {
    m_PositionMouse = pt;
    m_nTimeHere = 0;
    StartTimer();
  }
}

const wxColour nwxPlotCtrl::g_ColorGrid(232, 232, 232);

void nwxPlotCtrl::_Init()
{
  SetSelectionType(wxPLOTCTRL_SELECT_NONE);
  SetGridColour(g_ColorGrid);
  //    SetAreaMouseMarker(wxPLOTCTRL_MARKER_NONE);

  m_bmActiveBackup = m_activeBitmap;
  m_bmInactiveBackup = m_inactiveBitmap;
  m_bmClear = wxBitmap(clear15);

}

void nwxPlotCtrl::OnTimer(wxTimerEvent &)
{
  if (m_bStopTimer)
  {}  // do nothing
  else if(m_bClear || m_nDisableToolTip)
  {
    _ClearToolTip();
    m_bClear = false;
    m_nTimeHere = 0;
    StopTimer();
  }
  else
  {
    bool bSetup = !m_nTimeHere && (m_pToolTip != NULL);
    m_nTimeHere++;
    if(bSetup || (m_nTimeHere == TIMER_COUNT))
    {
      SetupToolTip();
      //StopTimer();
      m_nTimeHere = 0;
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
      wxS("wxFrame"),
      wxS("wxMDIChildFrame"),
      wxS("wxDialog"),
      wxS("wxTopLevelWindow")
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
  const nwxPointLabel *pLabel = NULL;
  wxRect rect(0,0,0,0);
  if(m_PositionMouse.y < 0)
  {
    pLabel = m_XLabels.FindLabel(m_PositionMouse,&rect);
  }
  else
  {
    pLabel = m_Labels.FindLabel(m_PositionMouse,&rect);
  }
  if(pLabel != NULL)
  {
    sText = pLabel->GetToolTip();
  }
  if(sText.IsEmpty())
  {
    if(m_pToolTip != NULL)
    {
      ClearToolTip();
    }
  }
  else
  {
    bool bLayout = true;
    bool bRefresh = false;
    bool bMove = true;
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
      // tool tip for label is showing but the cursor has moved
      // and is still within range.  Do not layout or move the
      // tool tip.
      bLayout = false;
      bMove = !m_pToolTip->IsShown(); // move only if not shown
    }
    if(bLayout)
    {
      m_pToolTip->Layout();
      m_pToolTip->Fit();
    }

    // set position
    if(bMove)
    {
      /*
        Hovering tool tip:
        IF fits above label -> show above at mouse x, scoot left if needed
                            (if it doesn't fit on the right)
        ELSE IF fits below label -> show at mouse x, scoot left if needed
        ELSE
          set y = 0
          IF fits on right -> show it
          ELSE IF fits on left -> show it
          ELSE -> show at x = 0

          plot ctrl - entire plot control
          plot area - inside plot control, contains lines, grid, etc
      */
      wxWindow *pArea = GetPlotArea();
      wxPoint pos(pArea->GetPosition());
      wxPoint posMouse(m_PositionMouse); // position of mouse
      rect.x += pos.x;        // label rectangle in plot ctrl
      rect.y += pos.y;
      posMouse += pos; // mouse position in plot ctrl 
      wxSize tipSize = m_pToolTip->GetSize();
      wxSize winSize = m_pToolTip->GetParent()->GetSize();

      wxPoint posLOW(rect.x,rect.y); // corners of label box
      wxPoint posHI(posLOW);
      const int DELTA = 2;
      bool bXout = false;
      posHI.x += rect.width + DELTA;
      posHI.y += rect.height + DELTA;
      posLOW.x = (posLOW.x > DELTA) ? posLOW.x - DELTA : 0;
      posLOW.y = (posLOW.y > DELTA) ? posLOW.y - DELTA : 0;
      if(posHI.y > winSize.y) { posHI.y = winSize.y; }
      if(posHI.x > winSize.x) { posHI.x = winSize.x; }
#if __TOOLTIP_TO_FRAME__
      _OffsetToolTipPosition(&posLOW);
      _OffsetToolTipPosition(&posHI
#endif
      const int CURSOR_SIZE = 24;
      int nHiY = posMouse.y + CURSOR_SIZE;
      if(nHiY < posHI.y) { nHiY = posHI.y; }
      if(posLOW.y > tipSize.y)
      {
        pos.y = posLOW.y - tipSize.y;
      }
      else if((nHiY + tipSize.y) < winSize.y)
      {
        pos.y = nHiY;
      }
      else
      {
        pos.y = 0;
        bXout = true; // make sure x position is outside of label
      }
      if(bXout)
      {
        int nHiX = posMouse.x + CURSOR_SIZE;
        if(nHiX < posHI.x) { nHiX = posHI.x; }
        if( (nHiX + tipSize.x) < winSize.x )
        {
          pos.x = nHiX;
        }
        else if(posLOW.x > tipSize.x)
        {
          pos.x = posLOW.x - tipSize.x;
        }
        else
        {
          pos.x = 0;
        }
      }
      else if( (posMouse.x + tipSize.x) < winSize.x )
      {
        pos.x = posMouse.x;
      }
      else
      {
        pos.x = winSize.x - tipSize.x;
        if(pos.x < 0) { pos.x = 0;}
      }
      if(m_pToolTip->GetPosition() != pos)
      {
        m_pToolTip->Move(pos);
        bRefresh = true;
      }
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
    _SetupCursor(pLabel);
  }
  return;
}
void nwxPlotCtrl::_ClearToolTip()
{
  if(m_pToolTip != NULL)
  {
    m_pToolTip->Show(false);
    _ResetMousePosition();
    _SetupCursor(NULL);
    // SetCursor(wxNullCursor);
  }
  return;
}

void nwxPlotCtrl::_SetupCursor(const nwxPointLabel *p)
{
  wxStockCursor nCur = (p == NULL) ? wxCURSOR_NONE : p->GetCursor();
  if(nCur == wxCURSOR_NONE)
  {
    if(m_cursorDefault.IsOk())
    {
      SetCursor(m_cursorDefault);
    }
    if(m_cursorAreaDefault.IsOk())
    {
      GetPlotArea()->SetCursor(m_cursorAreaDefault);
    }
  }
  else if(!CursorInPlotArea())
  {
    if(!m_cursorDefault.IsOk())
    {
      m_cursorDefault = GetCursor();
    }
    SetCursor(wxCursor(nCur));
  }
  else
  {
    wxWindow *pArea = GetPlotArea();
    if(!m_cursorAreaDefault.IsOk())
    {
      m_cursorAreaDefault = pArea->GetCursor();
    }
    pArea->SetCursor(wxCursor(nCur));
  }
}


void nwxPlotCtrl::OnViewChanged(wxPlotCtrlEvent &e)
{
  ClearToolTip();
  e.Skip();
}

bool nwxPlotCtrl::RenderScrollbars()
{
  return RenderingToWindow() && AreScrollbarsShown();
}
void nwxPlotCtrl::OnClickXLabel(const nwxPointLabel &, const wxPoint &) {;}
void nwxPlotCtrl::OnClickLabel(const nwxPointLabel &, const wxPoint &) {;}

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

EVT_PLOTCTRL_VIEW_CHANGED(wxID_ANY, nwxPlotCtrl::OnViewChanged)
EVT_MOUSE_EVENTS     ( nwxPlotCtrl::nwxOnMouse )
END_EVENT_TABLE()
