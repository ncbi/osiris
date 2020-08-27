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

*  FileName: nwxPlotDrawerLabel.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/wx.h>

/*
#ifdef __WXMAC__
#include <wx/dcmemory.h>
#include <wx/bitmap.h>
#ifdef _DEBUG
#include <wx/image.h>
#include "nwx/nwxStaticBitmap.h"
#endif // _DEBUG
#endif // __WXMAC__
*/

#include "nwx/nwxPlotDrawerLabel.h"
#include "nwx/nwxPlotShade.h"
#include "wx/plotctrl/plotctrl.h"

const int nwxPointLabel::ALIGN_DEFAULT = (wxALIGN_CENTRE_HORIZONTAL | wxALIGN_BOTTOM);
//  bitwise flags
const int nwxPointLabel::STYLE_BOX = 1;
const int nwxPointLabel::STYLE_DISABLED = 2;
const int nwxPointLabel::STYLE_BLACK_TEXT = 4;

bool nwxPointLabel::operator == (const nwxPointLabel &x) const
{
  return
    (m_nLabelStyle == x.m_nLabelStyle) &&
    (m_pData == x.m_pData) &&
    (m_dy == x.m_dy) &&
    (m_dx == x.m_dx) &&
    (m_sLabel == x.m_sLabel) &&
    (m_color == x.m_color) &&
    (m_nAlign == x.m_nAlign) &&
    (m_nSortGroup == x.m_nSortGroup) &&
    (m_sToolTip == x.m_sToolTip) &&
    (m_nCursor == x.m_nCursor)
    ;
}

bool nwxPointLabel::operator < (const nwxPointLabel &x) const
{
  bool bRtn = false;
  int nCmp;

  // sort by x, then by y descending
  // i.e. left to right then top to bottom

  if(m_dx != x.m_dx)
  {
    bRtn = (m_dx < x.m_dx);
  }
  else if( (m_dy != x.m_dy) )
  {
    bRtn = (m_dy > x.m_dy);
  }
  else if(m_nSortGroup != x.m_nSortGroup)
  {
    bRtn = m_nSortGroup < x.m_nSortGroup;
  }
  else if((nCmp = m_sLabel.Cmp(x.m_sLabel)) != 0)
  {
    bRtn = nCmp < 0;
  }
  else if((nCmp = m_sToolTip.Cmp(x.m_sToolTip)) != 0)
  {
    bRtn = nCmp < 0;
  }
  else if(m_color == x.m_color)
  {
    bRtn = false;
  }
  else if(m_color.Green() != x.m_color.Green())
  {
    bRtn = (m_color.Green() < x.m_color.Green());
  }
  else if(m_color.Red() != x.m_color.Red())
  {
    bRtn = (m_color.Red() < x.m_color.Red());
  }
  else if(m_color.Blue() != x.m_color.Blue())
  {
    bRtn = (m_color.Blue() < x.m_color.Blue());
  }
  else if(m_color.Alpha() != x.m_color.Alpha())
  {
    bRtn = (m_color.Alpha() > x.m_color.Alpha());
        // larger alpha is the darker colour
  }
  else if(m_nAlign != x.m_nAlign)
  {
    bRtn = m_nAlign < x.m_nAlign;
  }
  else if(m_pData != x.m_pData)
  {
    bRtn = m_pData < x.m_pData;
  }
  return bRtn;
}
void nwxPlotDrawerLabel::DrawSemiTransparentRectangle(wxDC *pdc, const wxRect &r)
{
  int nXmin = r.GetLeft();
  int nYmin = r.GetTop();
  int nWidth = r.GetWidth();
  int nHeight = r.GetHeight();
  int nx, ny;
  wxColour c;
  wxColour cNew;
  wxPen pen(*wxBLACK, 1, wxPENSTYLE_SOLID);
  wxBitmap bitmap(nWidth, nHeight);
  wxMemoryDC dc(bitmap);
  dc.Blit(0,0,nWidth,nHeight,pdc, nXmin, nYmin,wxCOPY);
#ifdef __WXMAC__
  wxImage img = bitmap.ConvertToImage();
#endif

  for (nx = 0; nx < nWidth; ++nx)
  {
    for (ny = 0; ny < nHeight; ++ny)
    {
#ifdef __WXMAC__
	c.Set(img.GetRed(nx,ny), img.GetGreen(nx,ny), img.GetBlue(nx,ny));
#else
	dc.GetPixel(nx, ny, &c);
#endif

#if 1
      // 50-50
      cNew.Set(
        (c.Red() >> 1) | 128,
        (c.Green() >> 1) | 128,
        (c.Blue() >> 1) | 128);
#else
      // 25-75
      cNew.Set(
        (c.Red() >> 2) | 192,
        (c.Green() >> 2) | 192,
        (c.Blue() >> 2) | 192);
#endif
      pen.SetColour(cNew);
      dc.SetPen(pen);
      dc.DrawPoint(nx, ny);
    }
  }
  pdc->Blit(nXmin, nYmin, nWidth, nHeight, &dc, 0, 0);
}
void nwxPlotDrawerLabel::Draw(wxDC *pdc, bool)
{
  size_t nSize = m_setLabels.size();
  if(nSize)
  {
    SET_LABEL::iterator itr;
    vector<bool> vbCanMoveUp;
    wxRect rect;
    const int PAD_Y = m_nMinY;
    const int PAD_X = 2 * (m_nMinY > 0 ? m_nMinY : 1);
    const int DELTA_WIDTH = PAD_X + PAD_X;
    const int DELTA_HEIGHT = PAD_Y + PAD_Y;
    int nx;
    int ny;
    int hl;
    int nAlign;
    wxColour cBG = pdc->GetTextBackground();
    wxColour cFG = pdc->GetTextForeground();

    wxBrush brushSave = pdc->GetBrush();
    wxPen penSave = pdc->GetPen();
    wxFont fontSave = pdc->GetFont();
    wxFont fontUse(m_owner->GetAxisFont());
    fontUse.MakeBold();
    pdc->SetFont(fontUse);

    pdc->SetTextBackground(wxColour(255, 255, 255, wxALPHA_TRANSPARENT));
    m_vRect.clear();
    m_vRect.reserve(nSize);
    m_vpLabel.clear();
    m_vpLabel.reserve(nSize);
    vbCanMoveUp.reserve(nSize);
    for(itr = m_setLabels.begin();
      itr != m_setLabels.end();
      ++itr)
    {
      const nwxPointLabel &label(*itr);
      nAlign = label.GetAlign();
      nx = m_owner->GetClientCoordFromPlotX(label.GetX());
      ny = m_owner->GetClientCoordFromPlotY(label.GetY());
      pdc->GetMultiLineTextExtent(
        label.GetLabel(),&rect.width,&rect.height,&hl);
      rect.width += DELTA_WIDTH;
      rect.height += DELTA_HEIGHT;
      if(nAlign & wxALIGN_CENTER)
      {
        nx -= (rect.width >> 1);
      }
      else if(nAlign & wxALIGN_RIGHT)
      {
        nx -= rect.width;
      }
      if(nAlign & wxALIGN_CENTER_VERTICAL)
      {
        ny -= (rect.height >> 1);
      }
      else if(nAlign & wxALIGN_BOTTOM)
      {
        ny -= rect.height;
      }
      if(ny < m_nMinY)
      {
        ny = m_nMinY;
      }
      rect.SetX(nx);
      rect.SetY(ny);
      m_vpLabel.push_back(&label);
      m_vRect.push_back(rect);
    }

    // now move items if they overlap

    int iright;
    int iy;
    int jy;
    int nX;
    int nY;
    int nLoop = 0;
    size_t i;
    size_t j;
    size_t nSize1 = nSize ? (nSize - 1) : 0;
    bool bOverlap = true;
    for(i = 0; i < nSize; i++)
    {
      vbCanMoveUp.push_back(true);
    }

    for(nLoop = 0; bOverlap && (nLoop < 1000); nLoop++)
    {
      nLoop++;
      bOverlap = false;
      // when overlap is found, this will be set to true

      for(i = 0; i < nSize1; i++)
      {
        wxRect &recti(m_vRect.at(i));
        iright = recti.GetRight();
        iy = recti.GetY();
        for(j = i + 1; j < nSize; j++)
        {
          wxRect &rectj(m_vRect.at(j));
          if(recti.Intersects(rectj))
          {
            bOverlap = true;
            jy = rectj.GetY();
            if(jy < iy)
            {
              if(vbCanMoveUp[j])
              {
                jy = iy - rectj.GetHeight() - 1;
                if(jy < m_nMinY)
                {
                  vbCanMoveUp[i] = false;
                  vbCanMoveUp[j] = false;
                  jy = m_nMinY;
                  iy = jy + rectj.GetHeight() + 1;
                  recti.SetY(iy);
                }
                rectj.SetY(jy);
              }
              else
              {
                // cannot move j up, so move i down and
                // never attempt to move i up
                iy = jy + rectj.GetHeight() + 1;
                recti.SetY(iy);
                vbCanMoveUp[i] = false;
              }
            }
            else
            {
              if(vbCanMoveUp[i])
              {
                iy = jy - recti.GetHeight() - 1;
                if(iy < m_nMinY)
                {
                  vbCanMoveUp[i] = false;
                  vbCanMoveUp[j] = false;
                  iy = m_nMinY;
                  jy = iy + recti.GetHeight() + 1;
                  rectj.SetY(jy);
                }
                recti.SetY(iy);
              }
              else
              {
                // cannot move i up, so push j down and
                // never attempt to my j up
                jy = iy + recti.GetHeight() + 1;
                rectj.SetY(jy);
                vbCanMoveUp[j] = false;
              }
            }
            j = nSize;
            // once we have fix an overlap,
            // don't fix another in this loop
          }
          else if(iright < rectj.GetLeft())
          {
            // recti is far to the left of
            // remaining label, so exit j loop
            j = nSize;
          }
        }
      }
    }
    wxPen pen(*wxBLACK_PEN);
    if (m_nMinY > 0)
    {
      pen.SetWidth(m_nMinY);
    }
    pdc->SetBrush(*wxWHITE_BRUSH);
    for(i = 0; i < nSize; i++)
    {
      const nwxPointLabel *pLabel = m_vpLabel.at(i);
      const wxRect &rrect(m_vRect.at(i));
      pdc->SetTextForeground(pLabel->GetTextColour());
      pen.SetColour(pLabel->GetColour());
      pdc->SetPen(pen);
      nX = rrect.GetX();
      nY = rrect.GetY();
      int nStyle = pLabel->GetStyle();
      DrawSemiTransparentRectangle(pdc, rrect);
      if (nStyle & nwxPointLabel::STYLE_BOX)
      {
        pdc->SetBrush(*wxTRANSPARENT_BRUSH);
        pdc->DrawRectangle(rrect);
      }
      pdc->DrawText(pLabel->GetLabel(),nX + PAD_X,nY + PAD_Y);
      if(nStyle & nwxPointLabel::STYLE_DISABLED)
      {
        wxPoint pt1(nX,nY);
        wxPoint pt2(nX + rrect.GetWidth(),nY + rrect.GetHeight());
        pdc->DrawLine(pt1,pt2);
      }
    }
    pdc->SetTextForeground(cFG);
    pdc->SetTextBackground(cBG);
    pdc->SetBrush(brushSave);
    pdc->SetPen(penSave);
    pdc->SetFont(fontSave);
  }
}

wxString nwxPlotDrawerLabel::GetToolTipText(const wxPoint &pos)
{
  wxString sRtn(wxEmptyString);
  const nwxPointLabel *pLabel = FindLabel(pos);
  if(pLabel != NULL) { sRtn = pLabel->GetToolTip(); }
  return sRtn;
}

const nwxPointLabel *nwxPlotDrawerLabel::FindLabel(const wxPoint &pos,wxRect *pRect)
{
  wxRect rect;
  size_t nSize = m_vRect.size();
  size_t i;
  const nwxPointLabel *pLabel(NULL);
  for(i = 0; i < nSize; i++)
  {
    rect = m_vRect.at(i);
    if(rect.Contains(pos))
    {
      pLabel = m_vpLabel.at(i);
      if(pRect != NULL)
      {
        *pRect = rect;
      }
      i = nSize; // loop exit
    }
  }
  return pLabel;
}

void nwxPlotDrawerXLabel::Draw(wxDC *pdc, bool)
{
  SET_LABEL::iterator itr;
  wxRect rect;
  wxRect rectBox;
  wxRect rectArea = m_owner->GetAreaRect();
  wxPoint ptArea = rectArea.GetPosition();
  int nx;
  int nx1, nx2, nx1b, nx2b;
  int ny;
  int hl;
  int nAlign;
  int nBorder;
  wxColour cBG = pdc->GetTextBackground();
  wxColour cFG = pdc->GetTextForeground();
  wxBrush brushSave = pdc->GetBrush();
  wxPen penSave = pdc->GetPen();
  wxFont fontSave = pdc->GetFont();
  wxFont fontUse = m_owner->GetAxisFont();
  fontUse.MakeBold();
  wxCoord nClipX = 0;
  wxCoord nClipY = 0;
  wxCoord nClipW = 0;
  wxCoord nClipH = 0;
  bool bDrawBoxes = GetDrawBoxes();
  // if there are multiple clipping regions, this could be a problem
  pdc->GetClippingBox(&nClipX,&nClipY,&nClipW,&nClipH);
  if(nClipW)
  {
	  pdc->DestroyClippingRegion();
  }
  pdc->SetClippingRegion(rectArea.GetLeft(), 0, rectArea.GetWidth(), rectArea.GetTop());

  //  clear rectangle
  {
    wxSize winSize = pdc->GetSize();
    wxRect clearRect(
      ptArea.x,
      0,
      winSize.GetWidth() - ptArea.x,
      ptArea.y - 1);
    pdc->SetBrush(*wxWHITE_BRUSH);
    pdc->SetBackground(*wxWHITE_BRUSH);
    pdc->SetPen(*wxWHITE_PEN);
    pdc->DrawRectangle(clearRect);
  }
  pdc->SetFont(fontUse);
  pdc->SetTextBackground(*wxWHITE);
  pdc->SetBackgroundMode(bDrawBoxes ? wxBRUSHSTYLE_SOLID : wxBRUSHSTYLE_TRANSPARENT);

  size_t nSize(m_setLabels.size());
  m_vRect.clear();
  m_vpLabel.clear();
  m_vpLabel.reserve(nSize);
  m_vRect.reserve(nSize);

  wxBrush brushRect(*wxWHITE_BRUSH);
  wxPen penRect(*wxWHITE_PEN);
  penRect.SetStyle(wxPENSTYLE_SOLID);
  penRect.SetWidth(1);
  brushRect.SetStyle(wxBRUSHSTYLE_SOLID);

  for(itr = m_setLabels.begin();
    itr != m_setLabels.end();
    ++itr)
  {
    const nwxPointLabel &label(*itr);
    nAlign = label.GetAlign();
    nx1 = m_owner->GetClientCoordFromPlotX(label.GetX());
    nx2 = m_owner->GetClientCoordFromPlotX(label.GetX2());
    nx = (nx1 + nx2) >> 1;
    ny = ptArea.y;

    pdc->GetMultiLineTextExtent(
      label.GetLabel(),&rect.width,&rect.height,&hl);

    if(nAlign & wxALIGN_CENTER)
    {
      nx -= (rect.width >> 1);
    }
    else if(nAlign & wxALIGN_RIGHT)
    {
      nx -= rect.width;
    }
#if 0
    if (nx < nx1)
    {
      nx = nx1;
    }
#endif
    if(nAlign & wxALIGN_CENTER_VERTICAL)
    {
      ny -= (rect.height >> 1);
    }
    else if(nAlign & wxALIGN_BOTTOM)
    {
      ny -= (rect.height + m_nOffset);
    }
    if((nx1 < rectArea.GetWidth()) && (nx2 > 0))
    {
      bool bRedrawOutline = false;
      // we have some overlap
      if(nx < 0) { nx = 0; }
      if(ny < 0) { ny = 0; }
      nx += ptArea.x;
      nx1 += ptArea.x;
      nx2 += ptArea.x;
      rect.SetX(nx);
      rect.SetY(ny);
      if (rect.GetRight() > rectArea.GetRight())
      {
        // text goes past the right side of the graph
        nx -= (rect.GetRight() - rectArea.GetRight());
        rect.SetX(nx);
      }
      if (bDrawBoxes)
      {
        const wxColour &colourBG(label.GetBackgroundColour());
        nBorder = (rect.height > 8) ? ((rect.height + 4) >> 3) : 1;
        nx1b = nx1 - nBorder;
        nx2b = nx2 + nBorder;
        if (nx2b > nx1b)
        {
          rectBox.SetLeft(nx1b);
          rectBox.SetTop(rect.GetTop() - nBorder);
          rectBox.SetRight(nx2b);
          rectBox.SetBottom(rect.GetBottom() + nBorder);

          penRect.SetColour(label.GetColour());
          pdc->SetPen(penRect);
          brushRect.SetColour(colourBG);
          pdc->SetBrush(brushRect);
          pdc->DrawRectangle(rectBox);
          bRedrawOutline = (nx < rectBox.GetLeft()) ||
            ((nx + rect.GetWidth()) >= rectBox.GetRight());
        }
      }
      pdc->SetTextForeground(label.GetTextColour());
      pdc->DrawText(label.GetLabel(), nx, ny);
      if (bRedrawOutline)
      {
        pdc->SetBrush(*wxTRANSPARENT_BRUSH);
        pdc->DrawRectangle(rectBox);
      }
      rect.x -= ptArea.x;  // set to plot area coordinates
      rect.y -= ptArea.y;
      m_vpLabel.push_back(&label);
      m_vRect.push_back(rect);
    }
  }
  pdc->DestroyClippingRegion();
  if(nClipW)
  {
	  // this will be a problem if there was more than one clipping region
	  pdc->SetClippingRegion(nClipX,nClipY,nClipW,nClipH);
  }

  pdc->SetFont(fontSave);
  pdc->SetTextForeground(cFG);
  pdc->SetTextBackground(cBG);
  pdc->SetBrush(brushSave);
  pdc->SetPen(penSave);
}

