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
*  FileName: CPanelHistory.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_HISTORY__
#define __C_PANEL_HISTORY__

#include <wx/panel.h>
#include <wx/button.h>
#include <wx/combobox.h>
#include <wx/datetime.h>
#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class CPanelHistory : public wxPanel
{
public:
  CPanelHistory(const set<wxDateTime> &setDateTime,
    wxWindow *parent, wxWindowID id);
  virtual ~CPanelHistory();
  void EnableButtons();
  const wxDateTime *GetDateTime();
  void SetCurrent()
  {
    _Select(0);
  }
  void SetOriginal();
private:
  void _Select(int n);
  vector<wxDateTime> m_vTime;
  wxButton *m_pButtonBack;
  wxButton *m_pButtonBack1;
  wxButton *m_pButtonFwd1;
  wxButton *m_pButtonFwd;
  wxComboBox *m_pComboHistory;
public:
  void OnButton(wxCommandEvent &);
  void OnComboBox(wxCommandEvent &);
  DECLARE_EVENT_TABLE();
};

#endif
