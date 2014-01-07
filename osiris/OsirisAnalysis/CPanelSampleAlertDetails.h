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
*  FileName: CPanelSampleAlertDetails.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_SAMPLE_ALERT_DETAILS_H__
#define __C_PANEL_SAMPLE_ALERT_DETAILS_H__

#include <wx/splitter.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include "CGridAlerts.h"

class COARnotes;
class COARsample;
class COARmessages;
class wxTextCtrl;
class wxPanel;
class wxDateTime;
#include "nwx/nsstd.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"

class CPanelSampleAlertDetails : public wxSplitterWindow
{
public:
  CPanelSampleAlertDetails(
    wxWindow *parent,
    COARmessages *pMsgEdit,
    const wxString &sNotesLabel = _T("Notes"),
    int nGridType = 0,
    bool bSplitHorizontal = true,
    bool bReadOnly = false);

  void SetupChannelColumn(
    const COARsample *pSample,
    const map<int,wxString> *pmapChannelNames);
  void SetupLocusColumn(const vector<wxString> &vsLocus)
  {
    m_pGridAlerts->SetupLocusColumn(vsLocus);
  }
  virtual bool Show(bool show = true);
  void Split();

  CGridAlerts *GetGridAlerts()
  {
    return m_pGridAlerts;
  }
  bool IsNotesCtrl(wxTextCtrl *p)
  {
    return (m_pTextNewNotes == p);
  }
  bool IsNotesModified();
  bool IsNotesEmpty(); // Is Notes? Are Notes?
  COARmessages *GetMsgEdit()
  {
    return m_pMsgEdit;
  }
  void SetDateTime(const wxDateTime *pHistTime, bool bForce = true)
  {
    m_pGridAlerts->SetDateTime(pHistTime,bForce);
  }
  void SetNotesText(const wxString &sNotes)
  {
    m_pTextNotes->SetValue(sNotes);
  }
  wxString GetNewNotesValue();
  // wxString GetNotesText(const wxString &sUser = wxEmptyString);
  //wxString GetNewNotes(const COARnotes *pCurrent, const wxString &sUser = wxEmptyString);
private:
  void _SetupColours();
  wxPanel *m_pPanelGrid;
  wxPanel *m_pPanelNotes;
  CGridAlerts *m_pGridAlerts;
  wxTextCtrl *m_pTextNotes;
  wxTextCtrl *m_pTextNewNotes;
  COARmessages *m_pMsgEdit;
//  bool m_bChannelGrid;
  bool m_bSplitHorizontal;
};

#endif
