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

*  FileName: nwxPlotDrawerShade.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxPlotDrawerShade.h"
#include <wx/wx.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include "wx/plotctrl/plotctrl.h"

void nwxPlotDrawerXShade::Draw(wxDC *dc, bool )
{
  wxPen penSave = dc->GetPen();
  wxBrush brushSave = dc->GetBrush();
  wxBrush brush;
  int nPenScale = (int)(GetPenScale() + 0.5);
  dc->SetPen(*wxTRANSPARENT_PEN);
  int nx1;
  int nx2;

  brush.SetStyle(wxSOLID);
  VECTOR_SHADE::iterator itr;
  wxPlotCtrlArea *pPlotArea = m_owner->GetPlotArea();
  wxSize sz = pPlotArea->GetSize();
  int nMaxX = sz.GetWidth() - nPenScale + 1;
  int nMaxY = sz.GetHeight() - nPenScale + 1;
  int nMin = nPenScale;
  for(itr = m_vArea.begin();
    itr != m_vArea.end();
    ++itr)
  {
    const nwxPlotShade &shade(*itr);
    nx1 = m_owner->GetClientCoordFromPlotX(shade.GetLo()) - 1;
    nx2 = m_owner->GetClientCoordFromPlotX(shade.GetHi()) + 1;
    if(nx1 < nMin) { nx1 = nMin; }
    if(nx2 >= nMaxX) { nx2 = nMaxX; }
    brush.SetColour(shade.GetColour());
    dc->SetBrush(brush);
    dc->DrawRectangle(nx1,nMin, nx2, nMaxY);
  }
}

const unsigned char nwxPlotShade::DEFAULT_ALPHA = 0;
