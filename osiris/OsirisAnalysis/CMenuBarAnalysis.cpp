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

*  FileName: CMenuBarAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#include <wx/frame.h>
#include "CMenuFileBase.h"
#include "wxIDS.h"
#include "CMenuBarAnalysis.h"

const wxString CMenuBarAnalysis::LABEL_TABLE(wxS("&Table"));
const wxString CMenuBarAnalysis::LABEL_GRAPH(wxS("&Preview"));
const int CMenuBarAnalysis::GRAPH_POSITION = 2;

CMenuBarAnalysis::~CMenuBarAnalysis()
{
  SetPlotMenu(NULL); // prevents graphic menus from being deleted
}

CMenuBarAnalysis::CMenuBarAnalysis
  (wxFrame *pWin,wxMenu *p,wxMenu *pPlot) :
    CMenuBar(false),
    m_pMenuFile(new CMenuFileAnalysis),
    m_pMenuTable(p),
    m_pMenuPlot(pPlot),
    m_pMenuPlotNada(new wxMenu)
{
  if(m_pMenuPlot == NULL)
  {
    m_pMenuPlot = m_pMenuPlotNada;
  }
  Insert(0,m_pMenuPlot,LABEL_GRAPH);
  Insert(0,m_pMenuTable,LABEL_TABLE);
  SetFileMenu(m_pMenuFile);
  //  EnablePlotMenu must be last because its 
  //  position would otherwise be incorrect
  pWin->SetMenuBar(this);
  _EnablePlotMenu();
}
void CMenuBarAnalysis::SetPlotMenu(wxMenu *pPlot)
{
  if(pPlot == NULL)
  {
    pPlot = m_pMenuPlotNada;
  }
  if(pPlot != m_pMenuPlot)
  {
    m_pMenuPlot = pPlot;
    Remove(GRAPH_POSITION);
    Insert(GRAPH_POSITION,pPlot,LABEL_GRAPH);
    _EnablePlotMenu();
  }
}
void CMenuBarAnalysis::_EnablePlotMenu()
{
  EnableTop(GRAPH_POSITION,m_pMenuPlot != m_pMenuPlotNada);
}

