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
#include "wxIDS.h"

#ifdef __WINDOW_LIST__
class CMenuWindow;
#endif


class CMenuBar : public wxMenuBar
{
public:
  CMenuBar(bool bCreateFileMenu = true,bool bClose = true);
  virtual ~CMenuBar();
  bool SetFileMenu(wxMenu *p);
  wxMenu *GetFileMenu()
  {
    return m_pMenuFile;
  }
  static size_t GetPosition(wxMenu *p, int ID);
#ifdef __WINDOW_LIST__
  void SetWindowMenu(CMenuWindow *p = NULL);
  CMenuWindow *GetWindowMenu()
    {
      return m_pMenuWindow;
    }
#endif
private:
  bool _okToAccessSiteSettings();
  wxMenu *m_pMenuFile;
#ifdef __WINDOW_LIST__
  int m_nRecursive;
  CMenuWindow *m_pMenuWindow;
  static wxString g_sWindowLabel;
#endif

public:
  DECLARE_ABSTRACT_CLASS(CMenuBar)
};


#endif
