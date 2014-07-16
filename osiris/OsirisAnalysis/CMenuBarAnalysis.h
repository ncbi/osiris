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
#ifndef __C_MENU_BAR_ANALYSIS_H__
#define __C_MENU_BAR_ANALYSIS_H__

#include <wx/menu.h>
#include "CMenuBar.h"
#include "wxIDS.h"
#include "CMenuFileAnalysis.h"
// File menu
class wxFrame;

class CMenuBarAnalysis : public CMenuBar
{
public:
  CMenuBarAnalysis
    (wxFrame *pWin,
     wxMenu *pMenuAnalysis,
     wxMenu *pMenuPlot = NULL);
  virtual ~CMenuBarAnalysis() ;
  CXSLExportFileType *GetFileTypeByID(int nID)
  {
    return m_pMenuFile->GetFileTypeByID(nID);
  }
  void UpdateFileMenu()
  {
    m_pMenuFile->ForceUpdate();
  }
  void EnableSave(bool bSaveAs,bool bSave, bool bExport)
  {
    m_pMenuFile->EnableSave(bSaveAs,bSave,bExport);
  }
  void SetPlotMenu(wxMenu *p);
  void OnTimer(wxTimerEvent &e) // called from CFrameAnalysis::OnTimer
  {
    m_pMenuFile->OnTimer(e);
  }
  //void OnMenu(wxCommandEvent &e);
private:
  void _EnablePlotMenu();
  CMenuFileAnalysis *m_pMenuFile;
  wxMenu *m_pMenuTable;
  wxMenu *m_pMenuPlot;
  wxMenu *m_pMenuPlotNada;
  static const wxString LABEL_TABLE;
  static const wxString LABEL_GRAPH;
  static const int GRAPH_POSITION;

};


#endif
