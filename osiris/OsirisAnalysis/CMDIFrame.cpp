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
*  FileName: CMDIFrame.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include <wx/msgdlg.h>
#include "CMDIFrame.h"
#include "wxIDS.h"
#include "nwx/CIncrementer.h"
#include "Platform.h"

CMDIFrame::~CMDIFrame()
{
  mainFrame *pParent = (mainFrame *)GetParent();
  pParent->RemoveWindow(this);
}

CMDIFrame::CMDIFrame(
    mainFrame* parent, 
    wxWindowID id, 
    const wxString& title, 
    const wxPoint& pos,
    const wxSize& size,
    long style) : wxMDIChildFrame(
      parent,id, title, 
#ifdef __WXMAC__
      (pos == wxDefaultPosition) ? parent->SelectPosition() : pos
#else
      pos
#endif
      , size, style),
      m_pParent(parent)
{
  m_nFocusRecursive = 0;
  m_bNoPromptReload = false;
  m_bAutoReload = false;
  parent->InsertWindow(this);
}

bool CMDIFrame::MenuEvent(wxCommandEvent &) { return false;}
wxMenu *CMDIFrame::GetMenu() { return NULL; }
wxMenu *CMDIFrame::GetGraphMenu() { return NULL; }
wxMenu *CMDIFrame::GetTableMenu() { return NULL; }


void CMDIFrame::UpdateHistory() {;}
void CMDIFrame::UpdateLadderLabels() {;}

wxString CMDIFrame::GetFileName()
{
  // base class returns empty string
  wxString s;
  return s;
}
bool CMDIFrame::SetToolbarMenuLabel(bool bShow, bool bPlural)
{
  wxMenu *pMenu = GetMenu();
  bool bRtn = false;
  if(pMenu != NULL)
  {
    wxMenuItem *pItem = pMenu->FindItem(IDmenuShowHideToolbar);
    if(pItem != NULL)
    {
      const wxString *psLabel(NULL);
      switch((bPlural ? 1 : 0) + (bShow ? 2 : 0))
      {
      case 0: // !plural && !show
        psLabel = &HIDE_TOOLBAR; break;
      case 1: // plural && !show
        psLabel = &HIDE_TOOLBARS; break;
      case 2: // !plural && show
        psLabel = &SHOW_TOOLBAR; break;
      case 3: // plural && show
        psLabel = &SHOW_TOOLBARS; break;
      }
      pItem->SetText(*psLabel);
      bRtn = true;
    }
  }
  return bRtn;
}
bool CMDIFrame::Destroy()
{
  CIncrementer xxx(m_nFocusRecursive);
  wxBusyCursor xx;
  mainFrame *pParent = (mainFrame *)GetParent();
  if( (pParent != NULL) &&
      (GetMenu() != NULL) && 
      (pParent->GetActiveChild() == this)
    )
  {
    pParent->DisableMenus();
  }
  return wxMDIChildFrame::Destroy();
}

bool CMDIFrame::Show(bool show)
{
  bool bRtn = wxMDIChildFrame::Show(show);
  if(show)
  {
    Raise();
    _NotifyParent();
  }
  return bRtn;
}
void CMDIFrame::CheckFileModification() {}
bool CMDIFrame::IsEdited() { return false; }

bool CMDIFrame::PromptReload(const wxString &sFileName, const wxString &sFileName2)
{
  bool bRtn = false;
  if(m_bAutoReload)
  {
    bRtn = true;
  }
  else if(!m_bNoPromptReload)
  {
    wxString sPrompt;
    if(sFileName.IsEmpty())
    {
      sPrompt = _T(
        "This file has been modified,\n"
        "would you like to reload it?");
    }
    else if(!sFileName2.IsEmpty())
    {
      wxFileName fn(sFileName);
      wxFileName fn2(sFileName2);
      sPrompt = _T("These files:\n  ");
      sPrompt.Append(fn.GetFullName());
      sPrompt.Append(_T("\n  "));
      sPrompt.Append(fn2.GetFullName());
      sPrompt.Append(_T("\nhave been modified,\n"
        "would you like to reload them?"));
    }
    else
    {
      wxFileName fn(sFileName);
      sPrompt = _T("This file, ");
      sPrompt.Append(fn.GetFullName());
      sPrompt.Append(_T(", has been modified,\n"
        "would you like to reload it?"));
    }
    wxMessageDialog dlg(
      this,sPrompt,_T("File Update"),
      wxYES_NO | wxICON_QUESTION);
    int n = dlg.ShowModal();
    bRtn = (n == wxID_YES);
    m_bNoPromptReload = !bRtn; // user said no, do not ask again
  }
  return bRtn;
}

wxMenu *CMDIFrame::GetLastMenuShown()
{
  return m_pParent->GetLastMenuShown();
}

void CMDIFrame::UpdateStatusBar()
{
#if HAS_STATUS_BAR
  m_pParent->SetStatusText(_T(""));
#endif
}

void CMDIFrame::OnFocus(wxFocusEvent &e)
{
  bool bFocus = (e.GetEventType() == wxEVT_SET_FOCUS);
  _NotifyParent();
  if(m_nFocusRecursive) {} // do nothing
  else if(bFocus)
  {
    CIncrementer xxx(m_nFocusRecursive);
    CheckFileModification();
  }
  e.Skip();
}
void CMDIFrame::RaiseWindow()
{
  if(IsIconized())
  {
    Iconize(false);
  }
  Activate();
#ifdef __WXMAC__
  Show(true);
  SetFocus();
#endif
}
void CMDIFrame::_NotifyParent()
{
  mainFrame *pParent = (mainFrame *)GetParent();
  if(pParent != NULL) 
  {
    pParent->CheckActiveFrame();
  }
}

bool CMDIFrame::PopupMenu_(wxMenu *menu, const wxPoint &pos)
{
  m_pParent->SetLastMenuShown(menu);
  bool bRtn = PopupMenu(menu, pos );
  UpdateStatusBar();
  return bRtn;
}
bool CMDIFrame::PopupMenu_(wxMenu *menu, int x, int y)
{
  m_pParent->SetLastMenuShown(menu);
  bool bRtn = PopupMenu(menu,x,y);
  UpdateStatusBar();
  return bRtn;
}
void CMDIFrame::_OnMenuEvent(wxCommandEvent &e)
{
  if(!MenuEvent(e))
  {
    e.Skip();
  }
}
void CMDIFrame::_Kill()
{
  m_pParent->KillWindow(this);
}

bool CMDIFrame::DialogIsShowingOrNoFocus()
{
  wxWindow *pW = wxWindow::FindFocus();
  bool bRtn = (pW == NULL);
  if(!bRtn)
  {
    wxClassInfo *pClass(CLASSINFO(wxTopLevelWindow));
    while((pW != NULL) && (!pW->IsKindOf(pClass)))
    {
      pW = pW->GetParent();
    }
    bRtn = (pW != NULL) &&
      pW->IsKindOf(CLASSINFO(wxDialog));
  }
  return bRtn;
}

const wxString CMDIFrame::HIDE_TOOLBARS(_T("Hide Toolbars\tCtrl+T"));
const wxString CMDIFrame::SHOW_TOOLBARS(_T("Show Toolbars\tCtrl+T"));
const wxString CMDIFrame::HIDE_TOOLBAR(_T("Hide Toolbar\tCtrl+T"));
const wxString CMDIFrame::SHOW_TOOLBAR(_T("Show Toolbar\tCtrl+T"));

BEGIN_EVENT_TABLE(CMDIFrame,wxMDIChildFrame)
EVT_SET_FOCUS(CMDIFrame::OnFocus)
EVT_KILL_FOCUS(CMDIFrame::OnFocus)
EVT_MENU_RANGE(IDmenu_START, IDmenu_END, CMDIFrame::_OnMenuEvent)
END_EVENT_TABLE()
