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
*
*  FileName: nwxPlotCtrl.h
*  Author:   Douglas Hoffman
*
*   need to remove m_pTimer and inherit nwxTimerReceiver 5/27/16, 
*   done 5/21/2020
*
*/
#ifndef __NWX_PLOT_CTRL_H__
#define __NWX_PLOT_CTRL_H__

#include <wx/dc.h>
#include <wx/stattext.h>
#include <wx/colour.h>
#include <wx/tooltip.h>
#include <wx/scrolbar.h>
#include <wx/panel.h>
#ifdef __WXMSW__
#pragma warning( push )
#pragma warning( disable : 4267 )
#endif
#include <wx/plotctrl/plotctrl.h>
#ifdef __WXMSW__
#pragma warning( pop )
#endif

#include "nwx/nwxPlotDrawerLabel.h"
#include "nwx/nwxPlotDrawerShade.h"
#include "nwx/nwxRound.h"
#include "nwx/nwxTimerReceiver.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

#define __TOOLTIP_TO_FRAME__ 0
//  tried to set the tooltip parent to the frame
//  but it created more problmes than it solved
//  set to 1 if attempted again in the future

class nwxPlotBin
{
public:
  nwxPlotBin(double x0, double x1, const wxColour &colour) :
    m_dX0(x0),
    m_dX1(x1),
    m_colour(colour)
  {}
  nwxPlotBin(const nwxPlotBin &x) :
    m_dX0(x.GetMin()),
    m_dX1(x.GetMax()),
    m_colour(x.GetColour())
  {}
  virtual ~nwxPlotBin() {}
  double GetMin() const
  {
    return m_dX0;
  }
  double GetMax() const
  {
    return m_dX1;
  }
  const wxColour &GetColour() const
  {
    return m_colour;
  }
  bool operator < (const nwxPlotBin &b2) const
  {
    return (GetMin() < b2.GetMin()) ||
      ((GetMin() == b2.GetMin()) &&
      (GetMax() < b2.GetMax()));
  }
private:
  double m_dX0, m_dX1;
  wxColour m_colour;
};
class nwxPlotBinLess
{
public:
  nwxPlotBinLess() {}
  bool operator()(const nwxPlotBin &x1, const nwxPlotBin &x2) const
  {
    return x1 < x2;
  }
  bool operator()(const nwxPlotBin *x1, const nwxPlotBin *x2) const
  {
    return (*x1) < (*x2);
  }
};
typedef std::multiset<nwxPlotBin, nwxPlotBinLess> nwxPlotBinSet;


class WXDLLIMPEXP_PLOTCTRL nwxPlotCtrl : public wxPlotCtrl, nwxTimerReceiver
{
public:
  nwxPlotCtrl() : wxPlotCtrl(),
    m_Labels(this),
    m_XLabels(this),
    m_Xshade(this),
    m_fontLabel(8,wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL),
    m_PositionMouse(-1,-1),
    m_nDisableToolTip(0),
    m_nTimerSinceSize(0),
    m_bNotRenderingToWindow(false),
    m_bClear(false),
    m_bStopTooltipTimer(false),
    m_bUpdatePosition(false),
    m_nTimeHere(0),
    m_pToolTip(NULL),
    m_pMousePositionText(NULL),
    m_pToolText(NULL),
    m_pLastXLabel(NULL),
    m_pLastLabel(NULL),
    m_pSetBins(NULL),
    m_pToolTipParent(NULL)
  { }
  nwxPlotCtrl( wxWindow *parent, wxWindowID win_id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        wxPlotCtrlAxis_Type flags = wxPLOTCTRL_DEFAULT,
        const wxString& name = wxT("wxPlotCtrl") ) :
    wxPlotCtrl( parent, win_id, pos, size, flags, name),
      m_Labels(this),
      m_XLabels(this),
      m_Xshade(this),
      m_fontLabel(8,wxFONTFAMILY_DEFAULT,
        wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL),
      m_PositionMouse(0,0),
      m_nDisableToolTip(0),
      m_nTimerSinceSize(0),
      m_bNotRenderingToWindow(false),
      m_bClear(false),
      m_bStopTooltipTimer(false),
      m_bUpdatePosition(false),
      m_nTimeHere(0),
      m_pToolTip(NULL),
      m_pMousePositionText(NULL),
      m_pToolText(NULL),
      m_pLastXLabel(NULL),
      m_pLastLabel(NULL),
      m_pSetBins(NULL),
      m_pToolTipParent(NULL)
      {
        _Init();
      }
  virtual ~nwxPlotCtrl();

  bool Create( wxWindow *parent, wxWindowID id = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 wxPlotCtrlAxis_Type flags = wxPLOTCTRL_DEFAULT,
                 const wxString& name = wxT("wxPlotCtrl") )
  {
    bool bRtn = wxPlotCtrl::Create(parent,id,pos,size,flags,name);
    if(bRtn)
    {
      _Init();
    }
    return bRtn;
  }
  int GetAreaMouseCursor()
  {
    return m_area_mouse_cursorid;
  }
  void AddLabel(const nwxPointLabel &x)
  {
    m_Labels.AddLabel(x);
  }
  void AddXLabel(const nwxPointLabel &x)
  {
    m_XLabels.AddLabel(x);
  }
  bool GetDrawXLabelBoxes()
  {
    return m_XLabels.GetDrawBoxes();
  }
  void SetDrawXLabelBoxes(bool b)
  {
    m_XLabels.SetDrawBoxes(b);
  }
  void AddXShade(const nwxPlotShade &x)
  {
    m_Xshade.AddArea(x);
  }
  void RemoveLabel(const nwxPointLabel &x)
  {
    m_Labels.RemoveLabel(x);
  }
  void RemoveXLabel(const nwxPointLabel &x)
  {
    m_XLabels.RemoveLabel(x);
  }
  void RemoveAllLabels()
  {
    m_Labels.Clear();
    m_XLabels.Clear();
    m_Xshade.Clear();
  }
  void SetLabelFont(const wxFont &x)
  {
    m_fontLabel = x;
  }
  const wxFont &GetLabelFont() const
  {
    return m_fontLabel;
  }
  bool IsTimerRunning()
  {
    return !m_bStopTooltipTimer;
  }
  void StartTimer()
  {
    m_bStopTooltipTimer = false;
  }
  void StopTimer()
  {
    m_bStopTooltipTimer = true;
  }
  void ShowScrollbars(bool b);

  bool AreScrollbarsShown()
  {
    bool b =
      (m_xAxisScrollbar != NULL) &&
      (m_xAxisScrollbar->IsShown());
    return b;
  }
  virtual bool RenderScrollbars();
  bool RenderingToWindow()
  {
    return !m_bNotRenderingToWindow;
  }
  void SetRenderingToWindow(bool b)
  {
    m_bNotRenderingToWindow = !b;
  }
  static const wxColor &GridColor()
  {
    return g_ColorGrid;
  }
  void _SetupShortLabel(const wxString &sLabel, const wxString &sShort, wxString *psSetup)
  {
    wxString s;
    if (sShort.IsEmpty() && !sLabel.IsEmpty())
    {
      s = sLabel;
      wxChar x[2] = { wxT(' '), wxT('(') };
      size_t l;
      for (int i = 0; i < 2; ++i)
      {
        l = s.Find(x[i]);
        if (l != wxNOT_FOUND)
        {
          s = s.Mid(0, l);
        }
      }
    }
    else
    {
      s = sShort;
    }
    if (s != (*psSetup))
    {
      *psSetup = s;
      m_sCoordLabel.Clear();
    }
  }
  void SetXLabels(const wxString &sLabel, const wxString &sShort = wxEmptyString)
  {
    SetXAxisLabel(sLabel);
    _SetupShortLabel(sLabel, sShort, &m_sXCoordLabel);
  }
  void SetYLabels(const wxString &sLabel, const wxString &sShort = wxEmptyString)
  {
    SetYAxisLabel(sLabel);
    _SetupShortLabel(sLabel, sShort, &m_sYCoordLabel);
  }
  const wxString &GetXShortLabel()
  {
    return m_sXCoordLabel;
  }
  const wxString &GetYShortLabel()
  {
    return m_sYCoordLabel;
  }
  const wxString &GetCoordLabel()
  {
    if (m_sCoordLabel.IsEmpty() &&
      (!m_sXCoordLabel.IsEmpty()) &&
      (!m_sYCoordLabel.IsEmpty()))
    {
      m_sCoordLabel = wxT('(');
      m_sCoordLabel.Append(GetXShortLabel());
      m_sCoordLabel.Append(wxT(", "));
      m_sCoordLabel.Append(GetYShortLabel());
      m_sCoordLabel.Append(wxT(")="));
    }
    return m_sCoordLabel;
  }
  virtual void OnClickXLabel(const nwxPointLabel &x, const wxPoint &pt);
  virtual void OnClickLabel(const nwxPointLabel &x, const wxPoint &pt);
  void SetupToolTip();
  void ClearToolTip()
  {
    if( (m_pToolTip != NULL) && m_pToolTip->IsShown() )
    {
      m_bClear = true; // timer will take it from here
      StartTimer();
    }
  }
  void DisableToolTip()
  {
    m_nDisableToolTip++;
    if(m_nDisableToolTip == 1) { ClearToolTip(); }
  }
  void EnableToolTip()
  {
    m_nDisableToolTip--;
    if(!m_nDisableToolTip) { StartTimer(); }
  }
  void OnViewChanged(wxPlotCtrlEvent &e);
  void nwxOnMouse(wxMouseEvent &event);
  void DrawBins(wxDC *dc);
  void SetBins(const nwxPlotBinSet *pSetBins)
  {
    m_pSetBins = pSetBins;
  }
  void ClearBins()
  {
    m_pSetBins = NULL;
  }
  virtual void OnTimer(wxTimerEvent &e);
  virtual void DrawTickMarks(wxDC *dc, const wxRect& rect);
  virtual void DrawAreaWindow( wxDC *dc, const wxRect& rect );
  virtual void DrawPlotCtrl( wxDC *dc );
  virtual void ProcessAreaEVT_MOUSE_EVENTS( wxMouseEvent &event );
  virtual void OnSizeCallback(wxSizeEvent &);
  void DrawEntirePlot( wxDC *dc, const wxRect &boundingRect, double dpi = 72, bool bForcePrintFont = false);
  //void DrawXLabels(wxDC *pDC);
  //wxString GetToolTipText(const wxPoint &pos);

#if __TOOLTIP_TO_FRAME__
  wxWindow *GetToolTipParent()
  {
    if(m_pToolTipParent == NULL)
    {
      m_pToolTipParent = this;
      //m_pToolTipParent = _FindFrameParent();
    }
    return m_pToolTipParent;
  }
  void SetToolTipParent(wxWindow *p)
  {
    if(m_pToolTip == NULL)
    {
      m_pToolTipParent = p;
    }
  }


  // DrawEntirePlot is a wrapper around DrawWholePlot
private:
  void _OffsetToolTipPosition(wxPoint *ppt);
#endif
  wxWindow *_FindFrameParent();

private:
  static const wxColor g_ColorGrid;
  static const unsigned int TIMER_COUNT;
  bool _InAreaRect(const wxPoint &pt);
  void _UpdateMousePosition();
  wxStaticText *_GetMousePositionText();
  void _ClearMousePosition();
  void _SetupCursor(const nwxPointLabel *pLabel);
  void _ClearToolTip();
  bool _OnMouseDown(wxMouseEvent &e); // called from nwxOnMouse
  bool _OnMouseUp(wxMouseEvent &e);   //
  bool _OnMouseDownXLabel(wxMouseEvent &e); // called from nwxOnMouse
  bool _OnMouseUpXLabel(wxMouseEvent &e);   //
  void SendContextMenuEvent(wxMouseEvent &e);
  void ProcessMousePosition(const wxPoint &pt);
  void _ResetMousePosition()
  {
    m_PositionMouse.x = -1;
    m_PositionMouse.y = -1;
  }
  bool CursorInPlotArea()
  {
    bool bRtn = false;
    if( (m_PositionMouse.x >= 0) && (m_PositionMouse.y >= 0) )
    {
      int x,y;
      GetPlotArea()->GetSize(&x,&y);
      bRtn = (m_PositionMouse.x <= x) &&
        (m_PositionMouse.y <= y);
    }
    return bRtn;
  }
  void _Init();
#ifdef __WXDEBUG__
  void _LogMouseUp(const wxPoint &pt,const nwxPointLabel *pLabel);
#endif
  wxString m_sXCoordLabel;
  wxString m_sYCoordLabel;
  wxString m_sCoordLabel;
  nwxPlotDrawerLabel m_Labels;
  nwxPlotDrawerXLabel m_XLabels;
  nwxPlotDrawerXShade m_Xshade;
  wxFont m_fontLabel;
  wxPoint m_PositionMouse;
  int m_nDisableToolTip;
  int m_nTimerSinceSize;
  bool m_bNotRenderingToWindow;
  bool m_bClear;
  bool m_bStopTooltipTimer;
  bool m_bUpdatePosition;
  unsigned int m_nTimeHere;
  wxPanel *m_pToolTip;
  wxStaticText *m_pMousePositionText;
  wxStaticText *m_pToolText;
  const nwxPointLabel *m_pLastXLabel;
  const nwxPointLabel *m_pLastLabel;
  const nwxPlotBinSet *m_pSetBins;
  wxWindow *m_pToolTipParent;
  wxCursor m_cursorDefault;
  wxCursor m_cursorAreaDefault;
  wxBitmap m_bmActiveBackup;
  wxBitmap m_bmInactiveBackup;
  wxBitmap m_bmClear;

  DECLARE_CLASS(nwxPlotCtrl)
  DECLARE_EVENT_TABLE()
};

class nwxPlotControlToolTipDisabler
{
public:
  nwxPlotControlToolTipDisabler(nwxPlotCtrl *p) : m_pPlotCtrl(p)
  {
    m_pPlotCtrl->DisableToolTip();
  }
  nwxPlotControlToolTipDisabler(const nwxPlotControlToolTipDisabler &x) : m_pPlotCtrl(x.m_pPlotCtrl)
  {
    m_pPlotCtrl->DisableToolTip();
  }
  virtual ~nwxPlotControlToolTipDisabler()
  {
    m_pPlotCtrl->EnableToolTip();
  }
private:
  mutable nwxPlotCtrl *m_pPlotCtrl;
};


#endif
