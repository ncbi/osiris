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
*  FileName: nwxDialogBitmap.cpp
*  Author:   Douglas Hoffman
*
*/

#include "nwx/nwxDialogBitmap.h"
#include "nwx/nwxRound.h"
#include "nwx/nwxString.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/settings.h>

//********************************************************* nwxScrolledBitmap
const int nwxScrolledBitmap::BORDER_WIDTH = 1;
const int nwxScrolledBitmap::BORDER_WIDTH2 = (BORDER_WIDTH + BORDER_WIDTH);

nwxScrolledBitmap::~nwxScrolledBitmap() {;}

void nwxScrolledBitmap::OnDraw(wxDC &dc)
{
  bool bResize = false;
  wxSize szWin = GetSize();
  int nWidthWin = szWin.GetWidth() - BORDER_WIDTH2;
  int nHeightWin = szWin.GetHeight() - BORDER_WIDTH2;
  int nWidthImg = m_pBitmap->GetWidth();
  int nHeightImg = m_pBitmap->GetHeight();

  if(m_nSizeType == SZ_ACTUAL)
  {
    bResize = false;
  }
  else if(m_nSizeType == SZ_FIT_TO_WINDOW)
  {
    if( (nHeightWin >= nHeightImg) &&
        (nWidthWin >= nWidthImg) )
    {
      // the image fits
      bResize = false;
    }
    else
    {
      double dX = (double)nWidthWin / (double) nWidthImg;
      double dY = (double)nHeightWin / (double)nHeightImg;
      bResize = true;
      m_dScale  = (dX < dY) ? dX : dY;
    }
  }
  else // SZ_PCT
  {
    bResize = (m_dScale != 1.0);
  }
  if(bResize)
  {
    if(m_dScale != m_dScaleResize)
    {
      // scale image only if needed

      m_nSizeType = SZ_PCT;
      m_imgResize = m_image;
      if(m_dScale < .1) { m_dScale = 0.1; }
      m_dScaleResize = m_dScale;
      int nWidth = nwxRound::Round(m_dScale * nWidthImg);
      int nHeight = nwxRound::Round(m_dScale * nHeightImg);
      m_imgResize = m_image.Scale(nWidth,nHeight);
    }
    wxBitmap bTemp(m_imgResize,32);
    _DrawBitmap(dc,bTemp);
  }
  else
  {
    m_dScale = 1.0;
    _DrawBitmap(dc,*m_pBitmap);
  }

  // send an event to notify the parent
  // that something may have changed

  wxCommandEvent e(wxEVT_COMMAND_ENTER,GetId());
  e.SetEventObject(this);
  GetEventHandler()->AddPendingEvent(e);

  return;
};

void nwxScrolledBitmap::_DrawBitmap(wxDC &dc, wxBitmap &bitmap)
{
  wxSize szWindow = GetSize();
  wxSize szBitmap(
    bitmap.GetWidth() + BORDER_WIDTH2,
    bitmap.GetHeight() + BORDER_WIDTH2);
  wxSize szVirtual(szWindow);
  wxPoint pt(szWindow.GetWidth() - szBitmap.GetWidth(),
    szWindow.GetHeight() - szBitmap.GetHeight());
  int nScrollX = 0;
  int nScrollY = 0;
  int nPosX = 0;
  int nPosY = 0;
  int nScrollWidth = 0;
  int nScrollHeight = 0;

  for(int nn = 0; nn < 2; nn++)
  {
    // if a horizontal scrollbar is created in the first loop,
    // then the height of the scrollbar may cause the need
    // for a vertical scrollbar in the second loop

    nn++; // this should be the last loop,
          // we will decrement otherwise

    // check is vertical scrollbar is needed

    if(nScrollY)
    {} // second loop, already have a vertical scrollbar
    if(pt.y >= nScrollHeight)
    {
      nPosY = (pt.y - nScrollHeight) >> 1;
    }
    else if(!nScrollY)
    {
      // image is taller than window
      // there will be a vertical scrollbar

      nScrollY = 10;
      nPosY = 0;
      nScrollWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
      szVirtual.SetHeight(szBitmap.GetHeight());
    }

    if(nScrollX)
    {} // second loop, already have a horizontal scrollbar
    if(pt.x >= nScrollWidth)
    {
      nPosX = (pt.x - nScrollWidth) >> 1;
    }
    else
    {
      // image is wider than window,
      // there will be a horizontal scrollbar

      nScrollX = 10;
      nPosX = 0;
      nScrollHeight = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
      szVirtual.SetWidth(szBitmap.GetWidth());
      nn--; // allow a second loop to check for vertical scrollbar
    }
  }
  SetVirtualSize(szVirtual);
  SetScrollRate(nScrollX,nScrollY);
  dc.SetPen(*wxBLACK_PEN);
  dc.SetBrush(*wxTRANSPARENT_BRUSH);
  dc.DrawRectangle(nPosX,nPosY,szBitmap.GetWidth(),szBitmap.GetHeight());
  dc.DrawBitmap(bitmap,nPosX + BORDER_WIDTH,nPosY + BORDER_WIDTH,false);
}

void nwxScrolledBitmap::_SetSizeType(int n, double d)
{
  bool bUpdate = false;
#ifdef __WXMAC__
  // on the macintosh, when zooming out (smaller image), we need to
  // reset scrollbars because otherwise the image may not display
  bool bScroll = false;
#define SET_SCROLL(x) bScroll = x;
#define DO_SCROLL if(bScroll) { Scroll(0,0); }
#else
#define SET_SCROLL(x)
#define DO_SCROLL
#endif
  switch(n)
  {
  case SZ_FIT_TO_WINDOW:
    SET_SCROLL(true);
    bUpdate = true;
    break;
  case SZ_PCT:
    {
      double dScale = (d == 100.0) ? 1.0 : (d * 0.01);
            // try to reduce rounding error if 100.0
      if(dScale != m_dScale)
      {
        SET_SCROLL(dScale < m_dScale);
        m_dScale = dScale;
        bUpdate = true;
      }
    }
    break;
  case SZ_ACTUAL:
  default:
    bUpdate = (m_dScale != 1.0);
    SET_SCROLL(m_dScale > 1.0);
    m_dScale = 1.0;
    n = SZ_ACTUAL;
  };
  m_nSizeType = n;
  if(bUpdate && IsShown())
  {
    DO_SCROLL;
    Refresh();
  }
}

//********************************************************* nwxDialogBitmap

nwxDialogBitmap::~nwxDialogBitmap() {;}

nwxDialogBitmap::nwxDialogBitmap(
  wxBitmap *pBitmap,
  wxWindow *parent,
  wxWindowID id,
  const wxString &sTitle,
  const wxPoint &pos,
  const wxSize &sz,
  long style) : wxDialog(parent,id,sTitle,pos,sz,style)
{
  wxPanel *pPanel = new wxPanel(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxTAB_TRAVERSAL | wxBORDER_RAISED);

  m_pScrollWin = new nwxScrolledBitmap(pBitmap,this,wxID_ANY);
  m_pButtonMinus = new wxButton(pPanel,wxID_ANY," - ");
  m_pButtonPlus = new wxButton(pPanel,wxID_ANY," + ");
  m_pTextPct = new wxTextCtrl(
    pPanel,wxID_ANY,"100%",
    wxDefaultPosition, wxDefaultSize,
    wxTE_READONLY);
  m_pButtonActual = new wxButton(pPanel,wxID_ANY,"Actual");
  m_pButtonFit = new wxButton(pPanel,wxID_ANY,"Fit");
  wxButton *pButtonClose = new wxButton(pPanel,wxID_CANCEL,"Close");
  m_pButtonPlus->SetToolTip("Increase size by 10% of actual size");
  m_pButtonMinus->SetToolTip("Decrease size by 10% of actual size");
  m_pButtonActual->SetToolTip("View the image in actual size");
  m_pButtonFit->SetToolTip(
    "Resize image to fit window if\n"
    "it is larger than the window");

  _SetupTextWidth(m_pTextPct);

  const int nFlag = (wxALL ^ wxLEFT) | wxALIGN_CENTRE_VERTICAL;
  const int nFlagFirst = wxALL | wxALIGN_CENTRE_VERTICAL;
  const int ID_BORDER = 4;
  wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
  pSizer->AddStretchSpacer(1);
  pSizer->Add(m_pButtonMinus,0,nFlagFirst, ID_BORDER);
  pSizer->Add(m_pButtonPlus,0,nFlag, ID_BORDER);
  pSizer->Add(m_pTextPct,0,nFlag, ID_BORDER);
  pSizer->Add(m_pButtonActual,0,nFlag,ID_BORDER);
  pSizer->Add(m_pButtonFit,0,nFlag,ID_BORDER);
  wxBoxSizer *pSizerClose = new wxBoxSizer(wxHORIZONTAL);
  pSizerClose->AddStretchSpacer(1);
  pSizerClose->Add(pButtonClose,0,0,0);
  pSizer->Add(pSizerClose,1,nFlag,ID_BORDER);
  pPanel->SetSizer(pSizer);

  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(m_pScrollWin,1,wxEXPAND,0);
  pSizerAll->Add(pPanel,0,wxEXPAND,0);
  SetSizer(pSizerAll);
  Layout();

}

void nwxDialogBitmap::_SetupTextWidth(wxTextCtrl *p)
{
  wxString s = p->GetValue();
  wxClientDC dc(p);
  dc.SetFont(p->GetFont());
  wxSize sz = dc.GetTextExtent(s);
  wxSize szWin = p->GetSize();
  szWin.SetWidth(sz.GetWidth() + 8);
  p->SetSize(szWin);
  p->SetMinSize(szWin);
}
void nwxDialogBitmap::OnButton(wxCommandEvent &e)
{
  wxButton *pObj = (wxButton *) e.GetEventObject();
  if(pObj == m_pButtonMinus)
  {
    m_pScrollWin->ChangePct(-10.0);
  }
  else if(pObj == m_pButtonPlus)
  {
    m_pScrollWin->ChangePct(10.0);
  }
  else if(pObj == m_pButtonActual)
  {
    m_pScrollWin->SetActualSize();
  }
  else if(pObj == m_pButtonFit)
  {
    m_pScrollWin->SetFitToWindow();
  }
  else
  {
    e.Skip();
  }
}
void nwxDialogBitmap::OnSetPct(wxCommandEvent &)
{
  int n = nwxRound::Round(m_pScrollWin->GetPct());
  wxString s = nwxString::FormatNumber(n);
  s.Append("%");
  m_pTextPct->ChangeValue(s);
}

void nwxDialogBitmap::OnSize(wxSizeEvent &e)
{
  m_pScrollWin->Refresh();
  e.Skip();
}

BEGIN_EVENT_TABLE(nwxDialogBitmap,wxDialog)
EVT_BUTTON(wxID_ANY,nwxDialogBitmap::OnButton)
EVT_COMMAND_ENTER(wxID_ANY,nwxDialogBitmap::OnSetPct)
EVT_SIZE(nwxDialogBitmap::OnSize)
END_EVENT_TABLE()
