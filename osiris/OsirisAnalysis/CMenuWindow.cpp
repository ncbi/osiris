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
*  FileName: CMenuWindow.cpp
*  Author:   Douglas Hoffman
*  Purpose:  Create a Window menu for the menu bar for implementations
*    that do not do it automatically because wxMDI is not used --
*    currently wxMac
*/
#include "CMenuWindow.h"
#ifdef __WINDOW_LIST__
#include "CMDIfileManager.h"

IMPLEMENT_ABSTRACT_CLASS(CMenuWindow,wxMenu)
  ;


CMenuWindow::~CMenuWindow() {}

void CMenuWindow::Build(const CMDI_LIST *pList,CMDIFrame *pCheck, long nModCount)
{
  if(GetMenuItemCount())
  {
    wxASSERT_MSG(0,"CMenuWindow::Build - not empty");
  }
  else
  {
    wxString sLabel;
    CMDI_LIST::const_iterator itr;
    int nID;
    wxChar sAccel[] = wxS("\tCtrl+0");
    size_t nAccelLast = (sizeof(sAccel) / sizeof(sAccel[0])) - 2;
    wxChar zero('0');
    int nMaxAccel = IDmenuWindow_Frame + 9;
    m_nModCount = nModCount;
#ifdef __WXMAC__
    Append(IDmenuWindow_Minimize,wxS("Minimize\tCtrl+M"));
    Append(IDmenuWindow_Zoom,wxS("Zoom"));
    AppendSeparator();
    Append(IDmenuWindow_AllToFront,wxS("Bring All to Front"));
    AppendSeparator();
#endif
    for(itr = pList->begin(), nID = IDmenuWindow_Frame;
        (itr != pList->end()) && (nID < IDmenuWindow_Frame_END);
        ++itr, ++nID)
    {
      sLabel = (*itr)->GetTitle();
      if(nID <= nMaxAccel)
      {
        if(nID < nMaxAccel)
        {
          sAccel[nAccelLast]++;
        }
        else
        {
          sAccel[nAccelLast] = zero;
        }
        sLabel.Append(sAccel);
      }
      if(*itr == pCheck)
      {
        AppendCheckItem(nID,sLabel)->Check();
      }
      else
      {
        Append(nID,sLabel);
      }
    }
  }
}


#endif
