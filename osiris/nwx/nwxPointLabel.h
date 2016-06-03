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

class nwxPointLabel
{
public:

#define nwxDEF_ALIGN (wxALIGN_CENTRE_HORIZONTAL | wxALIGN_BOTTOM)

  nwxPointLabel()
    : m_color(0,0,0), m_dx(0.0), m_dy(0.0),  m_nAlign(nwxDEF_ALIGN) {;}
  nwxPointLabel(
    const wxString &sLabel,
    double dx,
    double dy,
    const wxColour &c,
    const wxString sToolTip = wxEmptyString,
    int nAlign = nwxDEF_ALIGN,
    bool bStrikeThrough = false,
    void *pData = NULL) :
        m_sLabel(sLabel),
        m_sToolTip(sToolTip),
        m_color(c),
        m_dx(dx),
        m_dy(dy),
        m_nAlign(nAlign),
        m_pData(pData),
        m_bStrikeThrough(bStrikeThrough)
          {;}

#undef nwxDEF_ALIGN

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
    m_bStrikeThrough = x.m_bStrikeThrough;
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
  void *GetData() const
  {
    return m_pData;
  }
  bool GetStrikeThrough() const
  {
    return m_bStrikeThrough;
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
  void SetData(void *p)
  {
    m_pData = p;
  }
  void SetStrikeThrough(bool b)
  {
    m_bStrikeThrough = b;
  }
  void SetToolTip(const wxString &x)
  {
    m_sToolTip = x;
  }
  void SetToolTip(const char *ps)
  {
    m_sToolTip = ps;
  }

private:
  wxString m_sLabel;
  wxString m_sToolTip;
  wxColour m_color;
  double m_dx;
  double m_dy;
  int m_nAlign;
  void *m_pData;
  bool m_bStrikeThrough;
};

#endif
