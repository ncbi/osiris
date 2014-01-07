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
*  FileName: CPanelAlerts.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_ALERTS_H__
#define __C_PANEL_ALERTS_H__

#include <wx/panel.h>
#include <wx/event.h>
#include "CAlertViewStatus.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

class wxToggleButton;
DECLARE_EVENT_TYPE(wxEVT_ALERT_VIEW_CHANGED,-1)

class CPanelAlerts : public wxPanel
{
public:
  CPanelAlerts(wxWindow *parent, wxWindowID id = wxID_ANY);
  virtual ~CPanelAlerts();
  const CAlertViewStatus &GetStatus()
  {
    return m_ViewStatus;
  }

private:
  wxToggleButton *CreateToggleButton(
    const wxString &sLabel, 
    int nFlagStatus,
    const wxString &sToolTip = wxEmptyString);
  bool _InitFromStatus();
  bool _SetupStatus();
  static bool _SetButtonValue(wxToggleButton *p,bool b);
  bool _SetStatusFromButton(bool bButtonSet, int n);
  void OnChange(wxCommandEvent &e);

  CAlertViewStatus m_ViewStatus;

  wxToggleButton *m_pButtonAll;

  map<wxToggleButton *, int> m_mapButtonValue;

  DECLARE_EVENT_TABLE()

};

#endif
