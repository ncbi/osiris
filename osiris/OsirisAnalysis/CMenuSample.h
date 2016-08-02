#ifndef __C_MENU_SAMPLE_H__
#define __C_MENU_SAMPLE_H__

#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "CMenuBar.h"
class CMDIFrame;

class CMenuSample : public wxMenu
{
public:
  CMenuSample();
  virtual ~CMenuSample() {};
};

class CMenuBarSample : public CMenuBar
{
public:
  CMenuBarSample() : CMenuBar(true,true)
  {
    m_pMenu = new CMenuSample;
    Insert(1,m_pMenu,wxT("&Sample"));
  }
  virtual ~CMenuBarSample() {};
  wxMenu *GetMenu()
  {
    return m_pMenu;
  }
private:
  CMenuSample *m_pMenu;
};

class CToolbarSample : public wxPanel
{
public:
  DECLARE_ABSTRACT_CLASS(CToolbarSample)
  CToolbarSample(CMDIFrame *parent,bool bUserReadOnly);
  virtual ~CToolbarSample() {;}
  wxString GetUserID();
  void OnButton(wxCommandEvent &);
private:
  CMDIFrame *m_pParent;
  wxTextCtrl *m_pTextUser;
  DECLARE_EVENT_TABLE()
};
#endif
