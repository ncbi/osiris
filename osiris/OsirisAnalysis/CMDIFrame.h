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

*  FileName: CMDIFrame.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MDI_FRAME_H__
#define __C_MDI_FRAME_H__

#include "Platform.h"
#include <wx/event.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "wxIDS.h"
#include "nwx/nwxTimerReceiver.h"
#include "nwx/stdb.h"
#include <set>
#ifdef __WINDOW_LIST__
#include <list>
#endif


#include "nwx/stde.h"

#include INCLUDE_FRAME

DECLARE_EVENT_TYPE(CEventKillWindow,-1)

class mainFrame;

#ifdef __WINDOW_LIST__
class CMDIFrame;
typedef list<CMDIFrame *> CMDI_LIST;
#endif


class CMDIFrame : public CMDIFrameSuper, public nwxTimerReceiver
{
public:
  enum
  {
    FRAME_NONE,
    FRAME_ANALYSIS,
    FRAME_PLOT,
    FRAME_RUN,
    FRAME_SAMPLE
  };
  static const wxString HIDE_TOOLBARS;
  static const wxString SHOW_TOOLBARS;
  static const wxString HIDE_TOOLBAR;
  static const wxString SHOW_TOOLBAR;
  virtual int GetType() = 0;
  virtual bool MenuEvent(wxCommandEvent &e);
  virtual wxMenu *GetMenu();
  virtual wxMenu *GetTableMenu();
  virtual wxMenu *GetGraphMenu();
  virtual bool Destroy();
  virtual bool SetToolbarMenuLabel(bool bShow, bool bPlural = false);
  virtual wxString GetFileName();
  virtual void UpdateHistory();
  virtual void UpdateLadderLabels();
  virtual void UpdateFileMenu();
  virtual void CheckFileModification();
  virtual bool IsEdited();
  virtual bool PromptReload(
    const wxString &sFileName, 
    const wxString &sFileName2 = wxEmptyString);
  virtual void UpdateStatusBar();

  bool PopupMenu_(wxMenu* menu, const wxPoint& pos = wxDefaultPosition);
  bool PopupMenu_(wxMenu* menu, int x, int y);
#ifdef __WINDOW_LIST__ 
  virtual void SetTitle(const wxString &title);
  void CheckUpdateWindowMenu(long nModCount,const CMDI_LIST *p);
#endif
#ifdef __NO_MDI__
  void Activate()
  {
    Show(true);
    SetFocus();
  }
#endif
  static bool DialogIsShowingOrNoFocus();
  static void CannotTileError(CMDIFrame *pFrame, bool bRaise = false);
  static bool CheckCannotTile(CMDIFrame *pFrame, bool bRaise = false);
  wxMenu *GetLastMenuShown()
  {
    return m_pLastMenuShown;
  }
  void SetLastMenuShown(wxMenu *p);
  void RegisterMenu(wxMenu *p)
  {
    m_setMenus.insert(p);
  }
  bool IsMenuRegistered(wxMenu *p)
  {
    bool b = (m_setMenus.find(p) != m_setMenus.end());
    return b;
  }
  bool CheckRestore()
  {
    bool bRtn = false;
    if(IsIconized() || IsMaximized())
    {
      Restore();
      bRtn = true;
    }
    return bRtn;
  }

protected:
  CMDIFrame(
    mainFrame* parent, 
    wxWindowID id, 
    const wxString& title, 
    const wxPoint& pos = wxDefaultPosition, 
    const wxSize& size = wxDefaultSize, 
    long style = wxDEFAULT_FRAME_STYLE
    );
  virtual ~CMDIFrame();
  int m_nFocusRecursive;
  mainFrame *m_pParent;
  void _OnMenuEvent(wxCommandEvent &);
  void SetAutoReload(bool b = true)
  {
    m_bAutoReload = b;
  }
  void _Kill();
private:
  set<wxMenu *> m_setMenus;
  bool m_bNoPromptReload;
  bool m_bAutoReload;
  void _NotifyParent();
  wxMenu *m_pLastMenuShown;
  int m_nMenuCount;
  bool m_bLastMenuPopup;
public:
  virtual void OnFocusSetCB(wxFocusEvent &);  // callbacks for inherited classes
  virtual void OnFocusKillCB(wxFocusEvent &);
  virtual void OnActivateCB(wxActivateEvent &e);

  void RaiseWindow();
  void OnActivate(wxActivateEvent &e);
  void OnFocusSet(wxFocusEvent &e);
  void OnFocusKill(wxFocusEvent &e);
  void OnDoClose(wxCommandEvent &);
  void OnMenuOpen(wxMenuEvent &e);
  void OnMenuClose(wxMenuEvent &e);
  virtual bool Show(bool show = true);
  DECLARE_EVENT_TABLE()
  DECLARE_ABSTRACT_CLASS(CMDIFrame)
};



#endif
