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
#include "CMenuBar.h"
#include "CMenuFileBase.h"
#include "wxIDS.h"

CMenuBar::~CMenuBar()
{
}

CMenuBar::CMenuBar(bool bCreateFileMenu, bool bClose) : m_pMenuFile(NULL)
{
  wxMenu *pMenuTools(new wxMenu);
  wxMenu *pMenuHelp(new wxMenu);

  pMenuTools->Append(IDlab,"Lab Settings\tAlt+L");
  pMenuTools->Append(IDexport,"Export File Settings\tAlt+E");
  pMenuTools->Append(IDeditColours,"Edit Grid Colors\tAlt+C");
  pMenuTools->Append(IDlog,"Message Log");
  pMenuHelp->Append(IDhelp,"&Documentation\tF1");
  pMenuHelp->Append(IDhelpContactUs,"&Contact Us...");
  pMenuHelp->Append(IDcheckForUpdates,"Check f&or Updates...");
  pMenuHelp->Append(wxID_ABOUT,"&About Osiris...");
  Append(pMenuTools,"T&ools");
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
