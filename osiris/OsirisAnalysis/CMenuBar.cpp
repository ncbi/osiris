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
*  FileName: CMenuBar.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CMenuBar.h"
#include "CMenuFileBase.h"
#include "wxIDS.h"
#include "mainApp.h"
#ifdef __WINDOW_LIST__
#include "nwx/CIncrementer.h"
#include "CMenuWindow.h"
#define INIT_CONSTRUCTOR , m_nRecursive(0), m_pMenuWindow(new CMenuWindow)
wxString CMenuBar::g_sWindowLabel;
#else
#define INIT_CONSTRUCTOR
#endif

IMPLEMENT_ABSTRACT_CLASS(CMenuBar,wxMenuBar)
;

CMenuBar::~CMenuBar()
{}

CMenuBar::CMenuBar(bool bCreateFileMenu, bool bClose) :
  m_pMenuFile(NULL) INIT_CONSTRUCTOR
{
  wxMenu *pMenuTools(new wxMenu);
  wxMenu *pMenuHelp(new wxMenu);

  pMenuTools->Append(IDlab,"Lab Settings\tAlt+L");
  pMenuTools->Append(IDexport,"Export File Settings\tAlt+E");
  pMenuTools->Append(IDeditColours,"Edit Grid Colors\tAlt+C");
  pMenuTools->Append(IDartifactLabels,"Edit Artifact Labels\tCtrl+Alt+A");
  pMenuTools->Append(IDsiteSettings,"Access site settings...");
  pMenuTools->Append(IDsiteShow,"Show site settings folder...");
  pMenuTools->Append(IDlog,"Message Log");
  pMenuHelp->Append(IDhelp,"&Documentation\tF1");
  pMenuHelp->Append(IDhelpContactUs,"&Contact Us...");
  pMenuHelp->Append(IDcheckForUpdates,"Check f&or Updates...");
  pMenuHelp->Append(wxID_ABOUT,"&About Osiris...");
  Append(pMenuTools,"T&ools");
#ifdef __WINDOW_LIST__
  if(g_sWindowLabel.IsEmpty())
  {
    // prepare for strings to be external to code
    g_sWindowLabel = wxS("&Window");
  }
  Append(m_pMenuWindow,g_sWindowLabel);
#endif
  Append(pMenuHelp,"&Help");
  if(bCreateFileMenu)
  {
    SetFileMenu(new CMenuFileBase(bClose));
  }
}

bool CMenuBar::SetFileMenu(wxMenu *p)
{
  bool bRtn = (m_pMenuFile == NULL && p != NULL);
  if(bRtn)
  {
    m_pMenuFile = p;
    Insert(0,p,wxS("&File"));
  }
  return bRtn;
}
//
//  CMenuBar::GetPosition(wxMenu *p, int nID)
//    retrieve the position of the first menu item in *p whose ID = nID
//
//  general purpose menu utility, currently no good place to put it
//
size_t CMenuBar::GetPosition(wxMenu *p, int nID)
{
  wxMenuItemList &list(p->GetMenuItems());
  size_t nRtn = list.GetCount();
  wxMenuItemList::compatibility_iterator itr = list.GetFirst();
  for(size_t i = 0; i < nRtn; ++i)
  {
    if ((*itr).GetData()->GetId() == nID) 
    {
      nRtn = i;  // return position and force loop exit
    }
    else
    {
      itr = itr->GetNext();
    }
  }
  return nRtn;
}

#ifdef __WINDOW_LIST__
void CMenuBar::SetWindowMenu(CMenuWindow *p)
{
  if(m_nRecursive)
  {
    mainApp::LogMessageV(wxS("CMenuBar::SetWindowMenu recursive call %d"),m_nRecursive);
  }
  else if(p != m_pMenuWindow)
  {
    CIncrementer xxx(m_nRecursive);
    int n = FindMenu(g_sWindowLabel);
    if(n == wxNOT_FOUND)
    {
      size_t nItems = GetMenuCount();
      size_t nLoc;
      for(nLoc = 0; nLoc < nItems; nLoc++)
      {
        if(GetMenu(nLoc) == m_pMenuWindow)
        {
          n = (int) nLoc;
          break;
        }
      }
    }
    if(n != wxNOT_FOUND)
    {
      wxMenu *pOld = Replace((size_t)n,p,g_sWindowLabel);
      wxASSERT_MSG(pOld == (wxMenu *)m_pMenuWindow,"Unexpected window menu");
      delete pOld;
      m_pMenuWindow = p;
    }
    else
    {
      wxString s(wxS("CMenuBar::SetWindowMenu - Cannot find window menu for "));
      s += g_sWindowLabel;
      wxASSERT_MSG(0,s);
    }
  }
}
#endif
