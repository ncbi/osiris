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

*  FileName: CMenuBar.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_BAR_H__
#define __C_MENU_BAR_H__

#include <wx/menu.h>
#include "CMenuFile.h"
#include "wxIDS.h"

class CMenuBar : public wxMenuBar
{
public:
  CMenuBar();
  virtual ~CMenuBar() ;
  void EnableSave(bool bSaveAs, bool bSave, bool bExportCMF)
  {
    m_pMenuFile->EnableSave(bSaveAs,bSave,bExportCMF);
  }
  void EnableGraph(wxMenu *pMenu, bool bDisableOther = true)
  {
    m_pMenuFile->EnableExportGraph(true);
    _Replace(NDX_GRAPH,pMenu,true);
    if(bDisableOther)
    {
      DisableTable();
    }
  }
  void EnableTable(wxMenu *pMenu,bool bDisableOther = true)
  {
    _Replace(NDX_TABLE,pMenu,true);
    if(bDisableOther)
    {
      DisableGraph();
    }
    else
    {
      m_pMenuFile->EnableExportGraph(false);
    }
  }
  void SetTableGraph(wxMenu *pMenuTable, wxMenu *pMenuGraph)
  {
    if(pMenuGraph == NULL)
    {
      DisableGraph();
    }
    else
    {
      EnableGraph(pMenuGraph,false);
    }
    if(pMenuTable == NULL)
    {
      DisableTable();
    }
    else
    {
      EnableTable(pMenuTable,false);
    }
  }
  void DisableGraph()
  {
    m_pMenuFile->EnableExportGraph(false);
    _Replace(NDX_GRAPH,m_pMenuGraph,false);
  }
  void DisableTable()
  {
    m_pMenuFile->EnableTableExports(false);
    _Replace(NDX_TABLE,m_pMenuTable,false);
  }
  void DisableTableGraph()
  {
    DisableTable();
    DisableGraph();
  }
  void Cleanup()
  {
    // same as DisableTableGraph, except without 
    // calls to EnableTop()
    // EnableTop() asserts in the destructor and when
    // the menubar is not associated with a frame
    //
    //  this is called by the destructor in order to
    //  assure that m_pMenuGraph and m_pMenuTable
    //  are also destroyed and no external
    //  menus are destroyed
    _Replace(NDX_GRAPH,m_pMenuGraph);
    _Replace(NDX_TABLE,m_pMenuTable);
  }
  CXSLExportFileType *GetFileTypeByID(int nID)
  {
    return m_pMenuFile->GetFileTypeByID(nID);
  }
  void UpdateFileMenu()
  {
    m_pMenuFile->ForceUpdate();
  }

  //void OnMenu(wxCommandEvent &e);
private:
  wxMenu *_Replace(int nPos, wxMenu *pMenu);
  wxMenu *_Replace(int nPos, wxMenu *pMenu, bool bEnable);
  CMenuFile *m_pMenuFile;
  wxMenu *m_pMenuTable;
  wxMenu *m_pMenuGraph;
  static const wxString LABEL_TABLE;
  static const wxString LABEL_GRAPH;
  static const int NDX_TABLE;
  static const int NDX_GRAPH;

};


#endif
