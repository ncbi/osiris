#ifndef __C_MENU_SAMPLE_H__
#define __C_MENU_SAMPLE_H__

#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "CMenuBar.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

class CMDIFrame;
class wxButton;


class CMenuSample : public wxMenu
{
public:
  CMenuSample();
  virtual ~CMenuSample() {};
  void EnableItem(int nID,bool bEnable)
  {
    Enable(nID,bEnable);
  }
  void SetSampleEnabled(bool bEnabled);
  static const wxChar *GetDisabledLabel(bool bEnabled)
  {
    return bEnabled ? DISABLE : ENABLE;
  }
private:
  static const wxChar * const DISABLE;
  static const wxChar * const ENABLE;

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
  void EnableItem(int nID, bool bEnable)
  {
    m_pMenu->EnableItem(nID,bEnable);
  }
  void SetSampleEnabled(bool bEnabled)
  {
    m_pMenu->SetSampleEnabled(bEnabled);
  }
private:
  CMenuSample *m_pMenu;

};

class CToolbarSample : public wxPanel
{
public:
  DECLARE_ABSTRACT_CLASS(CToolbarSample)
  CToolbarSample(CMDIFrame *pFrame, wxWindow *parent);
  virtual ~CToolbarSample() {;}
  void OnButton(wxCommandEvent &);
  void EnableItem(int nID,bool bEnable);
  void SetSampleEnabled(bool bEnabled = true);
  static const wxChar *GetDisabledLabel(bool bEnabled)
  {
    return bEnabled ? DISABLE : ENABLE;
  }
private:
  wxButton *_FindButton(int nID);
  typedef std::map<int, wxButton *> mapButton;
  mapButton m_mapButton;
  wxButton *_CreateButton(int nID, const wxString &sLabel);
  CMDIFrame *m_pParent;
  static const wxChar * const DISABLE;
  static const wxChar * const ENABLE;
  DECLARE_EVENT_TABLE()
};
#endif
