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
*  FileName: CMenuSample.h
*  Author:   Douglas Hoffman
*
*/
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
  void _StoreButton(wxButton *p, int nID)
  {
    m_mapButton.insert(mapButton::value_type(nID,p));
  }
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
