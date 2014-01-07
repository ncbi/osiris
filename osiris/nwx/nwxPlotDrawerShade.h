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

*  FileName: nwxPlotDrawerShade.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_PLOT_DRAWER_SHADE_H__
#define __NWX_PLOT_DRAWER_SHADE_H__
#include "nwx/nwxPlotShade.h"
#include "wx/plotctrl/plotdraw.h"
#include <wx/string.h>
#include <wx/dc.h>
#include <vector>
#include "nsstd.h"

class nwxPlotDrawerShade : public wxPlotDrawerBase
{
public:
  typedef vector<nwxPlotShade> VECTOR_SHADE;
  nwxPlotDrawerShade(wxPlotCtrl *pOwner) : wxPlotDrawerBase(pOwner)
  {
    m_vArea.reserve(18);
  }
  virtual ~nwxPlotDrawerShade() {;}
  void AddArea(const nwxPlotShade &x)
  {
    m_vArea.push_back(x);
  }
  void RemoveAllAreas()
  {
    m_vArea.clear();
  }
  void Clear()
  {
    m_vArea.clear();
  }
protected:
  VECTOR_SHADE m_vArea;

};

class nwxPlotDrawerXShade : public nwxPlotDrawerShade
{
public:
  nwxPlotDrawerXShade(wxPlotCtrl *pOwner) : 
      nwxPlotDrawerShade(pOwner)
      {}
  virtual void Draw(wxDC *dc, bool refresh);
};


#endif
