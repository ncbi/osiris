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

*  FileName: nwxPlotShade.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_PLOT_SHADE_H__
#define __NWX_PLOT_SHADE_H__

#include <wx/colour.h>
#include <wx/font.h>

class nwxPlotShade
{
public:
  nwxPlotShade(double dLo,double dHi, const wxColour &colour, bool bDefaultAlpha = true)
  {
    m_dLo = dLo;
    m_dHi = dHi;
    m_colour = colour;
    if(bDefaultAlpha)
    {
      SetDefaultAlpha();
    }
  }
  nwxPlotShade(const nwxPlotShade &nps)
  {
    (*this) = nps;
  }
  nwxPlotShade()
  {
    m_dLo = 0.0;
    m_dHi = 0.0;
    m_colour.Set(255,255,255,DEFAULT_ALPHA);

  }
  nwxPlotShade &operator = (const nwxPlotShade &nps)
  {
    m_dLo = nps.m_dLo;
    m_dHi = nps.m_dHi;
    m_colour = nps.m_colour;
    return *this;
  }
  double GetLo() const
  {
    return m_dLo;
  }
  double GetHi() const
  {
    return m_dHi;
  }
  const wxColour &GetColour() const
  {
    return m_colour;
  }
  void SetDefaultAlpha()
  {
    SetAlpha(DEFAULT_ALPHA);
  }
  void SetAlpha(const unsigned char alpha)
  {
    m_colour.Set(
      m_colour.Red(),
      m_colour.Green(),
      m_colour.Blue(),
      alpha);
  }
  void SetColour(const wxColour &c)
  {
    m_colour = c;
  }
  void SetLo(double dLo)
  {
    m_dLo = dLo;
  }
  void SetHi(double dHi)
  {
    m_dHi = dHi;
  }
  static const unsigned char DEFAULT_ALPHA;
private:
  wxColour m_colour;
  double m_dLo;
  double m_dHi;
};


#endif
