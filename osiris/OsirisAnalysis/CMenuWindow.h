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
*  FileName: CMenuWindow.h
*  Author:   Douglas Hoffman
*  Purpose:  Create a Window menu for the menu bar for implementations
*    that do not do it automatically because wxMDI is not used --
*    currently wxMac
*
*/
#ifndef __C_MENU_WINDOW_H__
#define __C_MENU_WINDOW_H__

#include <wx/menu.h>
#include "wxIDS.h"
#ifdef __WINDOW_LIST__
#include "CMDIFrame.h"

class CMenuBar;

class CMenuWindow : public wxMenu
{
public:
  friend class CMenuBar;
  CMenuWindow(const CMDI_LIST *pList, CMDIFrame *pCheck, long nModCount) : 
    m_nModCount(-1)
  {
    Build(pList,pCheck,nModCount);
  }
  virtual ~CMenuWindow();
  void Build(const CMDI_LIST *pList, CMDIFrame *pCheck, long nModCount);
  long GetModCount()
  {
    return m_nModCount;
  }
  bool IsEmpty()
  {
    bool b = !GetMenuItemCount();
    return b;
  }
private:
  CMenuWindow() : m_nModCount(-1) {};  // empty menu 
  long m_nModCount;
  DECLARE_ABSTRACT_CLASS(CDialogExportFile)
};





#endif

#endif
