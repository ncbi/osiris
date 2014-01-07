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
*  FileName: nwxButtonMenu.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_BUTTON_MENU_H__
#define __NWX_BUTTON_MENU_H__

#include <wx/button.h>
#include <wx/menu.h>
class nwxButtonMenu : public wxButton
{
public:
  nwxButtonMenu(
    wxMenu *pMenu, // copies a pointer to the menu and
                   // deletes it in the destructor
    wxWindow *parent,
    wxWindowID id,
    const wxString &label = wxEmptyString,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long nStyle = wxBU_EXACTFIT) :
      wxButton(parent,id,label,pos,size,nStyle),
        m_pMenu(pMenu), m_bManageMemory(true)
  {;}
  nwxButtonMenu(
    wxMenu &Menu,  // keeps a pointer to the menu, but does not
                   // delete the menu in the destructor
    wxWindow *parent,
    wxWindowID id,
    const wxString &label = wxEmptyString,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long nStyle = wxBU_EXACTFIT) :
      wxButton(parent,id,label,pos,size,nStyle),
        m_pMenu(&Menu), m_bManageMemory(false)
  {;}
  nwxButtonMenu(
    wxWindow *parent,
    wxWindowID id,
    const wxString &label = wxEmptyString,
    const wxPoint &pos = wxDefaultPosition,
    const wxSize &size = wxDefaultSize,
    long nStyle = wxBU_EXACTFIT) :
      wxButton(parent,id,label,pos,size,nStyle),
        m_pMenu(NULL), m_bManageMemory(true)
  {;}
  virtual ~nwxButtonMenu()
  {
    _Cleanup();
  }
  static void ShowMenu(wxWindow *pWin, wxMenu *pMenu);
  wxMenu *GetMenu()
  {
    return m_pMenu;
  }
  bool HasMenu()
  {
    return (m_pMenu != NULL);
  }
  void ShowMenu();
  void SetMenu(wxMenu *pMenu, bool bManageMemory = true)
  {
    _Cleanup();
    m_pMenu = pMenu;
    m_bManageMemory = bManageMemory;
  }
  void OnLeftMouseDown(wxMouseEvent &);
  void OnChar(wxKeyEvent &);
  DECLARE_EVENT_TABLE();
#ifdef __WXMAC__
protected:
  virtual void DoSetToolTip(wxToolTip *) {;}
  // do not allow tooltip on macintosh because they cover the menu
#endif
private:
  void _Cleanup()
  {
    if(m_bManageMemory && (m_pMenu != NULL))
    {
      delete m_pMenu;
      m_pMenu = NULL;
    }
  }
  wxMenu *m_pMenu;
  bool m_bManageMemory;
};


#endif
