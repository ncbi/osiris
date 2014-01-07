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

const wxString CMenuBar::LABEL_GRAPH(_T("&Graph"));
const wxString CMenuBar::LABEL_TABLE(_T("&Table"));
const int CMenuBar::NDX_TABLE(1);
const int CMenuBar::NDX_GRAPH(2);



CMenuBar::~CMenuBar()
{
  Cleanup();
}

CMenuBar::CMenuBar()
{
  wxMenu *pMenuTools(new wxMenu);
  wxMenu *pMenuHelp(new wxMenu);
  m_pMenuFile = new CMenuFile;
  m_pMenuTable = new wxMenu;
  m_pMenuGraph = new wxMenu;

  pMenuTools->Append(IDlab,_T("Lab Settings\tAlt+L"));
  pMenuTools->Append(IDexport,_T("Export File Settings\tAlt+E"));
  pMenuTools->Append(IDeditColours,_T("Edit Grid Colors\tAlt+C"));
  pMenuTools->Append(IDlog,_T("Message Log"));
  pMenuHelp->Append(IDhelp,_T("&Documentation\tF1"));
  pMenuHelp->Append(IDhelpContactUs,_T("&Contact Us..."));
  pMenuHelp->Append(IDcheckForUpdates,_T("Check f&or Updates..."));
  pMenuHelp->Append(wxID_ABOUT,_T("&About Osiris..."));
  Append(m_pMenuFile,_T("&File"));
  Append(m_pMenuTable,LABEL_TABLE);
  Append(m_pMenuGraph,LABEL_GRAPH);
  Append(pMenuTools,_T("T&ools"));
  Append(pMenuHelp,_T("&Help"));
  EnableSave(false,false,false);
}

wxMenu *CMenuBar::_Replace(int nPos, wxMenu *pMenu)
{
  wxMenu *pRtn = NULL;
  wxMenu *pCurrent = GetMenu(nPos);
  if(pCurrent != pMenu)
  {
    wxString sLabel = GetMenuLabel(nPos);
    pRtn = Replace(nPos,pMenu,sLabel);
  }
  return pRtn;
}
wxMenu *CMenuBar::_Replace(int nPos, wxMenu *pMenu, bool bEnable)
{
  wxMenu *pRtn = _Replace(nPos,pMenu);
  EnableTop(nPos,bEnable);
  return pRtn;
}
