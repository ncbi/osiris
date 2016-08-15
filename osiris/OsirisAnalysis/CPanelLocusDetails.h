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
*  FileName: CPanelLocusDetails.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LOCUS_DETAILS_H__
#define __C_PANEL_LOCUS_DETAILS_H__


#include <wx/datetime.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "COARfile.h"
#include "CGridLocus.h"
#include "CGridLocusPeaks.h"
#include "CGridAlerts.h"
#include "CHistoryTime.h"
#include "ISetDateTime.h"

class CPanelUserID;

class CPanelLocusDetails : public wxPanel, public ISetDateTime
{
public:
  CPanelLocusDetails(
    const COARlocus *pLocus,
    const COARmessages *pMessages,
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    bool bSplitHorizontal = true,
    bool bReadOnly = false
    );
  CPanelLocusDetails(
    const COARsample *pSample,
    int nChannel,
    const COARlocus *pLocus,
    const COARmessages *pMessages,
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    bool bSplitHorizontal = true,
    bool bSplitLocusHorizontal = false,
    bool bReadOnly = false
    );
  void SetPanelUserID(CPanelUserID *p)
  {
    m_pPanelUser = p;
  }
  virtual ~CPanelLocusDetails();
  virtual bool TransferDataToWindow();
  virtual bool Validate();

  bool IsModified();
  bool IsNotesModified();
  bool IsNotesEmpty();
  bool IsAlertsModified();
  bool IsLocusModified();
  const wxString GetNewNotesValue()
  {
    return m_pTextNewNotes->GetValue();
  }

  wxSplitterWindow *GetSplitterWindow()
  {
    return m_pSplitter;
  }
  wxSplitterWindow *GetLocusSplitterWindow()
  {
    return m_pSplitterLocus;
  }
  const COARmessages &GetMessages() const
  {
    return m_MsgEdit;
  }
  const COARlocus &GetLocus() const
  {
    return m_LocusEdit;
  }
  wxGrid *GetLocusGrid()
  {
    wxGrid *pRtn = (m_pGridLocusPeaks != NULL) ? m_pGridLocusPeaks : m_pGridLocus;
    return pRtn;
  }
  CGridAlerts *GetAlertGrid()
  {
    return m_pGridAlerts;
  }
  virtual bool SetDateTime(const wxDateTime *pTime);
  void UpdateFile(COARfile *pF, COARsample *pSample = NULL);
private:
  void _UpdateNotes();
  wxPanel *_CreateNotesPanel();
  void _InitCommon(
      bool bSplitHorizontal,
      bool bSplitLocusHorizontal = false,
      const COARsample *pSample = NULL, 
      int nChannel = -1);
  CHistoryTime m_HistTime;
  COARlocus m_LocusEdit;
  COARmessages m_MsgEdit;
  wxSplitterWindow *m_pSplitterLocus;
  wxSplitterWindow *m_pSplitter;
  wxTextCtrl *m_pTextNewNotes;
  wxTextCtrl *m_pTextNotes;
  CGridLocus *m_pGridLocus11; // file version 1.1
  CGridLocusPeaks *m_pGridLocusPeaks; // file version 1.2
  _CGridEdit *m_pGridLocus; 
      // either m_pGridLocus or m_pgridLocusPeaks

  CGridAlerts *m_pGridAlerts;
  CPanelUserID *m_pPanelUser;
  const COARlocus *m_pLocus;
  const COARmessages *m_pMsgs;
  bool m_bReadOnly;
  bool m_bOAR12; // true if using OAR file version 1.2 or later

  void _OnAlertChange(wxGridEvent &e);
  void _OnLocusChange(wxGridEvent &e);
  bool _ProcessEvent();
public:
  void OnCellChange(wxGridEvent &e);
  void OnNotesChange(wxCommandEvent &e);
  DECLARE_ABSTRACT_CLASS(CPanelLocusDetails)
  DECLARE_EVENT_TABLE()
};


#endif
