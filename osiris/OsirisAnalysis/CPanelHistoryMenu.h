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

*  FileName: CPanelHistoryMenu.cpp
*  Author:   Douglas Hoffman
*
*  Purpose:  This serves two purposes,
*     provide a panel with history buttons
*     provide a history menu to be displayed when the history button
*       is pressed, or to be used by elsewhere, e.g. in the menu bar
*       or context menu
*/
#ifndef __C_PANEL_HISTORY_MENU_H__
#define __C_PANEL_HISTORY_MENU_H__

#include <wx/datetime.h>
#include <wx/panel.h>
#include "nwx/nsstd.h"
#include "nwx/nwxButtonMenu.h"

#include "COARfile.h"
#include "CMenuHistory.h"
#include "CHistoryTime.h"

#include <wx/event.h>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(CEventHistory,-1)
END_DECLARE_EVENT_TYPES()

class CPanelHistoryMenu : public wxPanel
{
public:

  CPanelHistoryMenu(
    COARfile *pFile,
    wxEvtHandler *pEventReceiver,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sLabel = CPanelHistoryMenu::LABEL);
  CPanelHistoryMenu(
    COARfile *pFile,
    CMenuHistory *pMenuHistory,
    wxEvtHandler *pEventReceiver,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sLabel = CPanelHistoryMenu::LABEL);
  CPanelHistoryMenu(
    CMenuHistory *pMenu,
    wxEvtHandler *pEventReceiver,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sLabel = CPanelHistoryMenu::LABEL);

  CPanelHistoryMenu(
    wxEvtHandler *pEventReceiver,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sLabel = CPanelHistoryMenu::LABEL);

  virtual ~CPanelHistoryMenu();
  void SendEvent();

  const wxDateTime *GetSelectedTime()
  {
    return (m_pMenuHistory == NULL)
      ? NULL
      : m_pMenuHistory->GetSelected();
  }
  bool IsCurrent()
  {
    bool bRtn = (m_pMenuHistory == NULL)
      ? true : m_pMenuHistory->IsCurrent();
    return bRtn;
  }
  void EnablePrevNextButtons();
  void EnablePrevNextButtons(const CHistoryTime &dt);
  wxMenu *GetMenu()
  {
    return m_pMenuHistory;
  }
  wxMenu *ReleaseMenu()
  {
    m_bManageMenu = false;
    return m_pMenuHistory;
  }
  void UnReleaseMenu()
  {
    m_bManageMenu = true;
  }
  void SetMenu(CMenuHistory *pMenu, bool bManage = false);
  static const wxString LABEL;
private:
  void _Cleanup();
  void _Build(const wxString &sLabel);
  CMenuHistory *m_pMenuHistory;
  COARfile *m_pFile;
  wxButton *m_pButtonPrev;
  wxButton *m_pButtonNext;
  nwxButtonMenu *m_pButtonHistory;
  wxEvtHandler *m_pEventReceiver;
  bool m_bManageMenu;
  void OnHistoryMore(wxCommandEvent &);
  void OnHistoryDate(wxCommandEvent &);

public:
  void HistoryEvent(wxCommandEvent &);
  void OnButtonPrevNext(wxCommandEvent &);
  DECLARE_EVENT_TABLE()
};

#endif

