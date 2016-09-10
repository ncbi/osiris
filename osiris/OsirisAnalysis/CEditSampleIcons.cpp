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
*  FileName: CEditSampleIcons.h
*  Author:   Douglas Hoffman
*
*/
#include "CEditSampleIcons.h"
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
//
//  using static data and functions from CGridAnalysis
//  it is required that at least one CGridAnalysis has been
//  instantiated, but this is not a problem because
//  a sample window is created from the analysis grid window
//
#include "CGridAnalysis.h"

void CEditSampleIcons::CleanupChanges()
{
  std::vector<int>::iterator itr;
  for(itr = m_vnToRemove.begin();
    itr != m_vnToRemove.end();
    ++itr)
  {
    m_List.Remove(*itr);
  }
  m_vnToRemove.clear();
}

int CEditSampleIcons::GetSolidImage(const wxColour &color)
{
  int ndx = -1;
  wxUint32 n = color.GetRGB();
  std::map<wxUint32, int>::iterator itr = m_mapSolids.find(n);
  if(itr == m_mapSolids.end())
  {
    wxBitmap bitmap(m_sizeImg,32);
    {
      wxMemoryDC dc(bitmap);
      wxBrush brush(color);
      dc.SetBackground(brush);
      dc.SetBackgroundMode(wxSOLID);
      dc.Clear();
    }
    ndx = m_List.Add(bitmap);
    m_mapSolids.insert(std::pair<wxUint32,int>(n,ndx));
  }
  else
  {
    ndx = itr->second;
  }
  return ndx;
}

void CEditSampleIcons::_Build()
{
  wxBitmap bitmap(m_sizeImg,32);
  wxMemoryDC dc(bitmap);
  wxASSERT_MSG(dc.IsOk(),wxT("CEditSampleIcons::_Build() - dc.IsOK() returned 'false'"));
  dc.SetFont(CGridAnalysis::GetFontStatus());
  wxSize szOK = dc.GetTextExtent(CGridAnalysis::g_sStatusOK);
  wxSize szNeedsAttn = dc.GetTextExtent(CGridAnalysis::g_sStatusNeedsAttention);
  m_sizeImg.x = (szOK.x > szNeedsAttn.x) ? szOK.x : szNeedsAttn.x;
  m_sizeImg.y = (szOK.y > szNeedsAttn.y) ? szOK.y : szNeedsAttn.y;
  if(m_sizeImg.x < m_sizeImg.y) { m_sizeImg.x = m_sizeImg.y; }

  //m_sizeImg.x += 4;
  //m_sizeImg.y += 4;

  // size is now determined, set text positions
  _CenterSizes(&m_ptOK, szOK, m_sizeImg);
  _CenterSizes(&m_ptNeedsAttn, szNeedsAttn, m_sizeImg);

  // set up image list
  m_List.Create(m_sizeImg.x,m_sizeImg.y,false);
  
  // Set colors
  CParmOsirisGlobal parm;
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  _SetColorNeedsAttn(pParm);
  _SetColorDisabled(pParm);
  _SetColorOK(pParm);

  // build icons

  _BuildNeedsAttnIcon();
  _BuildOKIcon();
  _BuildDisabledIcon();
  m_nChangeCount++;
}
int CEditSampleIcons::_BuildStatusIcon(
  const wxString &sText,
  const wxPoint &pt,
  const wxColor &cFG,
  const wxColor &cBG)
{

  wxBitmap bitmap(m_sizeImg,32);
  {
    wxMemoryDC dc(bitmap);
    wxBrush brush(cBG);
    dc.SetFont(CGridAnalysis::GetFontStatus());
    dc.SetTextForeground(cFG);
    dc.SetTextBackground(cBG);
    dc.SetBackground(brush);
    dc.SetBackgroundMode(wxSOLID);
    dc.Clear();
    dc.DrawText(sText,pt);
  }
  int ndx = m_List.Add(bitmap);
  return ndx;
}
void CEditSampleIcons::_BuildNeedsAttnIcon()
{

  _SetImageListIndex(&m_nNeedsAttn, _BuildStatusIcon(
    CGridAnalysis::g_sStatusNeedsAttention,
    m_ptNeedsAttn,
    m_cNeedsAttn_FG,
    m_cNeedsAttn_BG));
}
void CEditSampleIcons::_BuildOKIcon()
{
  _SetImageListIndex(&m_nOK, _BuildStatusIcon(
    CGridAnalysis::g_sStatusOK,
    m_ptOK,
    m_cOK_FG,
    m_cOK_BG));
}

void CEditSampleIcons::_CenterSizes(wxPoint *ppt, const wxSize &szIn, const wxSize &szOut)
{
  ppt->x = (szOut.x - szIn.x) >> 1;
  ppt->y = (szOut.y - szIn.y) >> 1;
#ifdef __WXMAC__
  ppt->x--;
  ppt->y--;
#endif
}

void CEditSampleIcons::_CheckUpToDate()
{
  if(!_IsBuilt())
  {
    _Build();
  }
  else if(!InBatch())
  {
    CParmOsirisGlobal parm;
    CParmOsiris *pParm = parm.Get();
    bool buildOK = _CheckColorOK(pParm);
    bool buildNeedsAttn = _CheckColorNeedsAttn(pParm);
    bool buildDisabled = _CheckColorDisabled(pParm);
    if(buildOK || buildNeedsAttn)
    {
      if(buildOK) { _BuildOKIcon();}
      if(buildNeedsAttn) { _BuildNeedsAttnIcon(); }
      if(!buildDisabled)
      {
        // if buildDisabled, it will be incremented later
        m_nChangeCount++;
      }
    }
    if(buildDisabled)
    {
      _BuildDisabledIcon();
      m_nChangeCount++;
    }

  }
}

nwxIMPLEMENT_GLOBAL_OBJECT(CEditSampleIcons)

