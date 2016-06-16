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
    (m_sToolTip == x.m_sToolTip)
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

void nwxPlotDrawerLabel::Draw(wxDC *pdc, bool)
{
  size_t nSize = m_setLabels.size();
  if(nSize)
  {
    SET_LABEL::iterator itr;
    vector<bool> vbCanMoveUp;
    wxRect rect;
    const int PAD_X = 2;
    const int DELTA_WIDTH = PAD_X + PAD_X;
    int nx;
    int ny;
    int hl;
    int nAlign;
    wxColour cBG = pdc->GetTextBackground();
    wxColour cFG = pdc->GetTextForeground();

    wxBrush brushSave = pdc->GetBrush();
    wxPen penSave = pdc->GetPen();
    wxFont fontSave = pdc->GetFont();
    pdc->SetFont(m_owner->GetAxisFont());

    pdc->SetTextBackground(wxColour(255,255,255,wxALPHA_TRANSPARENT));
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
  //    pdc->SetTextForeground(label.GetColour());
  //    pdc->DrawText(label.GetLabel(),nx,ny);
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
    pdc->SetBrush(*wxWHITE_BRUSH);
    for(i = 0; i < nSize; i++)
    {
      const nwxPointLabel *pLabel = m_vpLabel.at(i);
      const wxRect &rrect(m_vRect.at(i));
      pdc->SetTextForeground(pLabel->GetColour());
      pen.SetColour(pLabel->GetColour());
      pdc->SetPen(pen);
      nX = rrect.GetX();
      nY = rrect.GetY();
      int nStyle = pLabel->GetStyle();
      if(nStyle & nwxPointLabel::STYLE_BOX)
      {
        pdc->DrawRectangle(rrect);
      }
      pdc->DrawText(pLabel->GetLabel(),nX + PAD_X,nY);
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

const nwxPointLabel *nwxPlotDrawerLabel::FindLabel(const wxPoint &pos)
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
      i = nSize; // loop exit
    }
  }
  return pLabel;
}

void nwxPlotDrawerXLabel::Draw(wxDC *pdc, bool)
{
  SET_LABEL::iterator itr;
  wxRect rect;
  wxPoint ptArea = m_owner->GetPlotArea()->GetPosition();
  int nx;
  int ny;
  int hl;
  int nAlign;
  wxColour cBG = pdc->GetTextBackground();
  wxColour cFG = pdc->GetTextForeground();
  wxBrush brushSave = pdc->GetBrush();
  wxPen penSave = pdc->GetPen();
  wxFont fontSave = pdc->GetFont();
  wxCoord nClipX;
  wxCoord nClipY;
  wxCoord nClipW;
  wxCoord nClipH;
  // if there are multiple clipping regions, this could be a problem
  pdc->GetClippingBox(&nClipX,&nClipY,&nClipW,&nClipH);
  if(nClipW)
  {
	  pdc->DestroyClippingRegion();
  }

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
  pdc->SetFont(m_owner->GetAxisFont());
  pdc->SetTextBackground(wxColour(255,255,255,wxALPHA_OPAQUE));

  size_t nSize(m_setLabels.size());
  m_vRect.clear();
  m_vpLabel.clear();
  m_vpLabel.reserve(nSize);
  m_vRect.reserve(nSize);

  for(itr = m_setLabels.begin();
    itr != m_setLabels.end();
    ++itr)
  {
    const nwxPointLabel &label(*itr);
    nAlign = label.GetAlign();
    nx = m_owner->GetClientCoordFromPlotX(label.GetX());
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
    if(nAlign & wxALIGN_CENTER_VERTICAL)
    {
      ny -= (rect.height >> 1);
    }
    else if(nAlign & wxALIGN_BOTTOM)
    {
      ny -= (rect.height + m_nOffset);
    }
    if(nx > -rect.width)
    {
      if(nx < 0) { nx = 0; }
      if(ny < 0) { ny = 0; }
      nx += ptArea.x;
      rect.SetX(nx);
      rect.SetY(ny);
      pdc->DrawRectangle(rect);
      pdc->SetTextForeground(label.GetColour());
      pdc->DrawText(label.GetLabel(),nx,ny);
      rect.x -= ptArea.x;  // set to plot area coordinates
      rect.y -= ptArea.y;
      m_vpLabel.push_back(&label);
      m_vRect.push_back(rect);
    }
  }
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

