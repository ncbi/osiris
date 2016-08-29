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
#ifndef __C_EDIT_SAMPLE_ICONS_H__
#define __C_EDIT_SAMPLE_ICONS_H__

#include "nwx/nwxGlobalObject.h"
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include "nwx/stde.h"
#include "nwx/nwxBatch.h"
#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/imaglist.h>
#include "CParmOsiris.h"


class CEditSampleIcons
{
public:
  virtual ~CEditSampleIcons() {}

  void CleanupChanges();
  bool InBatch()
  {
    return (m_nBatch > 0);
  }
  void BeginBatch()
  {
    m_nBatch++;
  }
  void EndBatch()
  {
    if(InBatch()) { m_nBatch--;}
  }
  int GetSolidImage(const wxColour &c);

  int ChangeCount()
  {
    return m_nChangeCount;
  }
  bool HasChanged(int *pn)
  {
    _CheckUpToDate();
    bool bRtn = *pn != m_nChangeCount;
    if(bRtn) { *pn = m_nChangeCount; }
    return bRtn;
  }
  bool CheckForUpdates()
  {
    int n = m_nChangeCount;
    return HasChanged(&n);
  }
  int GetOKImage()
  {
    _CheckUpToDate();
    return m_nOK;
  }
  int GetNeedsAttnImage()
  {
    _CheckUpToDate();
    return m_nNeedsAttn;
  }
  int GetStatusImage(bool bNeedsAttn)
  {
    int nRtn = bNeedsAttn ? GetNeedsAttnImage() : GetOKImage();
    return nRtn;
  }
  int GetDisabledImage()
  {
    _CheckUpToDate();
    return m_nDisabled;
  }
  wxImageList *GetList()
  {
    _CheckBuild(); 
    return &m_List;
  }
private:
  wxImageList m_List;
  std::map<wxUint32, int> m_mapSolids;
  std::vector<int> m_vnToRemove;
  wxColour m_cOK_FG;
  wxColour m_cOK_BG;
  wxColour m_cNeedsAttn_FG;
  wxColour m_cNeedsAttn_BG;
  wxColour m_cDisabled_FG;
  wxPoint m_ptOK;
  wxPoint m_ptNeedsAttn;
  wxSize m_sizeImg;
  int m_nChangeCount;
  int m_nOK;
  int m_nNeedsAttn;
  int m_nDisabled;
  int m_nBatch;

  CEditSampleIcons() :
      m_sizeImg(-1,-1),
      m_nChangeCount(0),
      m_nOK(-1),
      m_nNeedsAttn(-1),
      m_nDisabled(-1),
      m_nBatch(0)
    {}

  int _BuildStatusIcon(
    const wxString &sText,
    const wxPoint &pt, 
    const wxColor &cFG, 
    const wxColor &cBG);
  void _BuildNeedsAttnIcon();
  void _BuildOKIcon();
  void _BuildDisabledIcon()
  {
    m_nDisabled = GetSolidImage(m_cDisabled_FG);
    // do not call _SetImageListIndex because
    // we don't want to discard any solid images
  }
  void _Build();
  void _CheckUpToDate();
  bool _IsBuilt()
  {
    return(m_sizeImg.x > 0);
  }
  void _CheckBuild()
  {
    if(!_IsBuilt()) { _Build();}
  }
  void _SetImageListIndex(int *pn, int nNew)
  {
    if(*pn != nNew)
    {
      if(*pn >= 0)
      {
        m_vnToRemove.push_back(*pn);
      }
      *pn = nNew;
    }
  }
  void _SetColorNeedsAttn(CParmOsiris *pParm)
  {
    m_cNeedsAttn_FG = pParm->GetStatusForeground(true);
    m_cNeedsAttn_BG = pParm->GetStatusBackground(true);
  }
  bool _CheckColorNeedsAttn(CParmOsiris *pParm)
  {
    bool bRtn = (pParm->GetStatusForeground(true) != m_cNeedsAttn_FG) ||
        (pParm->GetStatusBackground(true) != m_cNeedsAttn_BG);
    if(bRtn)
    {
      _SetColorNeedsAttn(pParm);
    }
    return bRtn;
  }
  void _SetColorOK(CParmOsiris *pParm)
  {
    m_cOK_FG = pParm->GetStatusForeground(false);
    m_cOK_BG = pParm->GetStatusBackground(false);
  }
  bool _CheckColorOK(CParmOsiris *pParm)
  {
    bool bRtn = (pParm->GetStatusForeground(false) != m_cOK_FG) ||
        (pParm->GetStatusBackground(false) != m_cOK_BG);
    if(bRtn)
    {
      _SetColorOK(pParm);
    }
    return bRtn;
  }
  void _SetColorDisabled(CParmOsiris *pParm)
  {
    m_cDisabled_FG = pParm->GetForeground(GRID_DISABLED);
  }
  bool _CheckColorDisabled(CParmOsiris *pParm)
  {
    bool bRtn = pParm->GetForeground(GRID_DISABLED) != m_cDisabled_FG;
    if(bRtn)
    {
      _SetColorDisabled(pParm);
    }
    return bRtn;
  }
  

  static void _CenterSizes(wxPoint *ppt, const wxSize &szIn, const wxSize &szOut);

  nwxDECLARE_GLOBAL_OBJECT(CEditSampleIcons)
};

typedef TnwxBatchGlobal<CEditSampleIcons> CEditSampleIconsBatch;

#endif