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
*
*  FileName: nwxButtonMenu.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxButtonMenu.h"
void nwxButtonMenu::ShowMenu(wxWindow *pWin, wxMenu *pMenu)
{
  if(pWin != NULL && pMenu != NULL)
  {
    wxWindow *parent = pWin->GetParent();
    if(parent == NULL)
    {
      parent = pWin;
    }
    wxPoint pos = pWin->GetPosition();
    wxSize sz = pWin->GetSize();
    pos.y += sz.GetHeight();
    parent->PopupMenu(pMenu,pos);
  }
}
void nwxButtonMenu::ShowMenu()
{
  if(m_pMenu != NULL)
  {
    ShowMenu(this,m_pMenu);
  }
#ifdef _DEBUG
  else
  {
    wxASSERT_MSG(0,_T("nwxButtonMenu::ShowMenu() - no menu specified."));
  }
#endif
}

void nwxButtonMenu::OnLeftMouseDown(wxMouseEvent &e)
{
  if(m_pMenu != NULL)
  {
    ShowMenu();
  }
  else
  {
    e.Skip(true);
  }
}

void nwxButtonMenu::OnChar(wxKeyEvent &e)
{
  int n = e.GetKeyCode();
  switch(n)
  {
  case '\n':
  case '\r':
  case ' ':
    if(m_pMenu != NULL) { ShowMenu(); }
    else { e.Skip(true); }
    break;
  default:
    e.Skip(true);
  };
}
BEGIN_EVENT_TABLE(nwxButtonMenu,wxButton)
EVT_LEFT_DOWN(nwxButtonMenu::OnLeftMouseDown)
EVT_CHAR(nwxButtonMenu::OnChar)
EVT_KEY_DOWN(nwxButtonMenu::OnChar)
END_EVENT_TABLE()
