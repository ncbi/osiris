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
*  FileName: nwxMenuItem.h
*  Author:   Douglas Hoffman
*
*    subclass of wxMenuItem that removes itself from its
*    parent in the destructor.  This is in order to 
*    prevent the destructor from being called twice
*
*/
#ifndef __NWX_MENU_ITEM_H__
#define __NWX_MENU_ITEM_H__

#include <wx/menu.h>

class nwxMenuItem : public wxMenuItem
{
public:
  nwxMenuItem(
    wxMenu* parentMenu = NULL, 
    int id = wxID_SEPARATOR, 
    const wxString& text = "", 
    const wxString& helpString = "", 
    wxItemKind kind = wxITEM_NORMAL, 
    wxMenu* subMenu = NULL) :
      wxMenuItem(
        parentMenu,
        id,
        text,
        helpString,
        kind, 
        subMenu) {;}
  virtual ~nwxMenuItem()
  {
    wxMenu *p = GetMenu();
    if(p != NULL)
    {
      p->Remove(this);
    }
  }
};

#endif
