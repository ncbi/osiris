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

*  FileName: nwxPlotDrawerLabel.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_PLOT_DRAWER_H__
#define __NWX_PLOT_DRAWER_H__
#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"

#include <wx/font.h>
#include <wx/string.h>
#include <wx/dc.h>
#include "wx/plotctrl/plotdraw.h"
#include "nwx/nsstd.h"
#include "nwx/nwxPointLabel.h"
#include "nwx/nwxRound.h"

class nwxPlotDrawerLabel : public wxPlotDrawerBase
{
public:
  typedef set<nwxPointLabel> SET_LABEL;
  nwxPlotDrawerLabel(wxPlotCtrl *pOwner) :
    wxPlotDrawerBase(pOwner), m_nMinY(1) {}
  virtual ~nwxPlotDrawerLabel() {}
  virtual void Draw(wxDC *dc, bool refresh);
  void AddLabel(const nwxPointLabel &x)
  {
    m_setLabels.insert(x);
    _CleanupLists();
  }
  void RemoveLabel(const nwxPointLabel &x)
  {
    SET_LABEL::iterator itr = m_setLabels.find(x);
    if(itr != m_setLabels.end())
    {
      m_setLabels.erase(itr);
      _CleanupLists();
    }
  }
  void RemoveAllLabels()
  {
    Clear();
  }
  void Clear()
  {
    m_setLabels.clear();
    _CleanupLists();
  }
  static int DPIpixelOffset(double dDPI)
  {
    double d = dDPI * (1.0 / 72.0);
    int n  = nwxRound::Round(d);
    return n;
  }
  void SetMinYDPI(double dDPI)
  {
    m_nMinY = DPIpixelOffset(dDPI);
  }
  void SetMinY(int n)
  {
    m_nMinY = n;
  }
  int GetMinY()
  {
    return m_nMinY;
  }

  wxString GetToolTipText(const wxPoint &pos);
  const nwxPointLabel *FindLabel(const wxPoint &pos, wxRect *pRect = NULL);


protected:
  SET_LABEL m_setLabels;
  vector<wxRect> m_vRect;
  vector<const nwxPointLabel *> m_vpLabel;
  int m_nMinY;

  void _CleanupLists()
  {
    m_vRect.clear();
    m_vpLabel.clear();
  }
};

class nwxPlotDrawerXLabel : public nwxPlotDrawerLabel
{
public:
  nwxPlotDrawerXLabel(wxPlotCtrl *pOwner) :
      nwxPlotDrawerLabel(pOwner),m_nOffset(1) {}
  void SetOffset(int nPixel)
  {
    m_nOffset = nPixel;
  }
  int GetOffset()
  {
    return m_nOffset;
  }
  void SetDPIoffset(double dDPI)
  {
    m_nOffset = nwxPlotDrawerLabel::DPIpixelOffset(dDPI);
  }
  virtual ~nwxPlotDrawerXLabel() {}
  virtual void Draw(wxDC *dc, bool refresh);
private:
  int m_nOffset;
};

#endif
