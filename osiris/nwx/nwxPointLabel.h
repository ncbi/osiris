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

*  FileName: nwxPointLabel.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_POINT_LABEL_H__
#define __NWX_POINT_LABEL_H__

#include <wx/colour.h>
#include <wx/cursor.h>
class nwxPointLabel
{
public:

  static const int ALIGN_DEFAULT;
  static const int STYLE_BOX;
  static const int STYLE_DISABLED;
  nwxPointLabel()
    : m_color(0,0,0), 
      m_dx(0.0), 
      m_dy(0.0),  
      m_nAlign(ALIGN_DEFAULT),
      m_nSortGroup(0),
      m_nLabelStyle(0),
      m_nCursor(wxCURSOR_NONE),
      m_pData(NULL) {;}
  nwxPointLabel(
    const wxString &sLabel,
    double dx,
    double dy,
    const wxColour &c,
    const wxString sToolTip = wxEmptyString,
    int nAlign = ALIGN_DEFAULT,
    int nSortGroup = 0,
    int nLabelStyle = 0,
    wxStockCursor cur = wxCURSOR_NONE,
    void *pData = NULL) :
        m_sLabel(sLabel),
        m_sToolTip(sToolTip),
        m_color(c),
        m_dx(dx),
        m_dy(dy),
        m_nAlign(nAlign),
        m_nSortGroup(nSortGroup),
        m_nLabelStyle(nLabelStyle),
        m_nCursor(cur),
        m_pData(pData)
          {;}

  nwxPointLabel(const nwxPointLabel &x)
  {
    (*this) = x;
  }
  nwxPointLabel &operator = (const nwxPointLabel &x)
  {
    m_sLabel = x.m_sLabel;
    m_sToolTip = x.m_sToolTip;
    m_color = x.m_color;
    m_dx = x.m_dx;
    m_dy = x.m_dy;
    m_nAlign = x.m_nAlign;
    m_nSortGroup = x.m_nSortGroup;
    m_nLabelStyle = x.m_nLabelStyle;
    m_nCursor = x.m_nCursor;
    m_pData = x.m_pData;
    return *this;
  }
  bool operator < (const nwxPointLabel &x) const; 
      // in nwxPlotDrawerLabel.cpp used for set<nwxPointLabel>
  bool operator > (const nwxPointLabel &x) const
  {
    return x < *this;
  }
  bool operator == (const nwxPointLabel &x) const; // nwxPlotDrawerLabel.cpp
  bool operator != (const nwxPointLabel &x) const
  {
    return !((*this) == x);
  }

  // Get functions

  const wxString &GetLabel() const
  {
    return m_sLabel;
  }
  const wxString &GetToolTip() const
  {
    return m_sToolTip;
  }
  const wxColour &GetColour() const
  {
    return m_color;
  }
  double GetX() const
  {
    return m_dx;
  }
  double GetY() const
  {
    return m_dy;
  }
  int GetAlign() const
  {
    return m_nAlign;
  }
  int GetSortGroup() const
  {
    return m_nSortGroup;
  }
  void *GetData() const
  {
    return m_pData;
  }
  wxStockCursor GetCursor() const
  {
    return m_nCursor;
  }
  int GetStyle() const
  {
    return m_nLabelStyle;
  }

  // Set functions

  void SetLabel(const char *ps)
  {
    m_sLabel = ps;
  }
  void SetLabel(const wxString &x)
  {
    m_sLabel = x;
  }
  void SetColour(const wxColour &x)
  {
    m_color = x;
  }
  void SetX(double d)
  {
    m_dx = d;
  }
  void SetY(double d)
  {
    m_dy = d;
  }
  void SetAlign(int n)
  {
    m_nAlign = n;
  }
  void SetSortGroup(int n)
  {
    m_nSortGroup = n;
  }
  void SetData(void *p)
  {
    m_pData = p;
  }
  void SetStyle(int b)
  {
    m_nLabelStyle = b;
  }
  void SetToolTip(const wxString &x)
  {
    m_sToolTip = x;
  }
  void SetToolTip(const char *ps)
  {
    m_sToolTip = ps;
  }
  void SetCursor(wxStockCursor nCur)
  {
    m_nCursor = nCur;
  }

private:
  wxString m_sLabel;
  wxString m_sToolTip;
  wxColour m_color;
  double m_dx;
  double m_dy;
  int m_nAlign;
  int m_nSortGroup; // for sorting labels in the same location
  int m_nLabelStyle;
  wxStockCursor m_nCursor;
  void *m_pData;
};

#endif
