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
*  FileName: CNotebookEditSample.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_NOTEBOOK_EDIT_SAMPLE_H__
#define __C_NOTEBOOK_EDIT_SAMPLE_H__

#include <wx/datetime.h>
#include <wx/splitter.h>
#include <wx/treebook.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "CLabSettings.h"
#include "CGridAlerts.h"
#include "COARfile.h"
#include "CHistoryTime.h"
#include "ISetDateTime.h"
#include "CPanelSampleAlertDetails.h"

class CPageEditSample;
class CPanelUserID;
class CNotebookEditSample;

#ifndef __WINDOW_ARRAY_INDEX__
#define __WINDOW_ARRAY_INDEX__
typedef enum
{
  SA_NDX_DIR,
  SA_NDX_SAMPLE,
  SA_NDX_ILS,
  SA_NDX_CHANNEL,
  SA_WINDOW_COUNT
} ALERT_WINDOW_TYPE; // used in arrays below

#endif


class CNotebookEditSample : public wxPanel //, public ISetDateTime
{
public:
  CNotebookEditSample(
    COARfile *pFile, // dont' need pFile, get it from pSample
    COARsample *pSample, 
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const map<int,wxString> *pmapChannelNames = NULL
    );
  virtual ~CNotebookEditSample();
  virtual bool TransferDataToWindow();
  virtual bool Validate();
  bool IsModified();

  void Select(int n)
  {
    // select notebook page, Sample, ILS, or Channel
    m_pNotebook->ChangeSelection((size_t)n);
  }
  int GetSelection()
  {
    return m_pNotebook->GetSelection();
  }
  wxTreebook *GetNotebookWindow()
  {
    return m_pNotebook;
  }
  /*
  virtual bool SetDateTime(const wxDateTime *pTime);
  virtual const wxDateTime *GetDateTime()
  {
    return m_HistTime.GetDateTime();
  }

  CPanelSampleAlertDetails **GetSplitterWindows()
  {
    return &m_pSplitter[0];
  }
  CPanelSampleAlertDetails *GetSplitterWindow(int n)
  {
    return m_pSplitter[n];
  }
  const COARmessages &GetDirMessages()
  {
    return m_MsgDir;
  }
  const COARmessages &GetSampleMessages()
  {
    return m_MsgSample;
  }
  const COARmessages &GetILSMessages()
  {
    return m_MsgILS;
  }
  const COARmessages &GetChannelMessages()
  {
    return m_MsgChannel;
  }
  wxString GetNewSampleNotes()
  {
    return _GetNewNotes(SA_NDX_SAMPLE);
  }
  wxString GetNewILSNotes()
  {
    return _GetNewNotes(SA_NDX_ILS);
  }
  wxString GetNewChannelNotes()
  {
    return _GetNewNotes(SA_NDX_CHANNEL);
  }
  wxString GetNewDirNotes()
  {
    return _GetNewNotes(SA_NDX_DIR);
  }

  bool IsAlertsModified(int n);

  bool IsIndexUsed(int n)
  {
    return (m_pSplitter[n] != NULL);
  }
  */
  const wxString &GetCurrentLocus();
  const wxString &GetUserID();
  void RepaintData();
  void InitiateRepaintData();
private:
  bool _ProcessEvent();
  /* wxString _GetReviewAcceptance(int n, const wxDateTime *pdt = NULL);
  */
  std::vector<const wxString> m_asLocus;
  std::vector<CPageEditSample *> m_vpPanels;
  wxString m_sUserID;
  /*
  CHistoryTime m_HistTime;
  COARmessages m_MsgDir;
  COARmessages m_MsgSample;
  COARmessages m_MsgILS;
  COARmessages m_MsgChannel;
  */
  COARsample *m_pSample;
  COARfile *m_pFile;

  wxTreebook *m_pNotebook;
//  CPanelSampleAlertDetails *m_pSplitter[SA_WINDOW_COUNT];
  bool m_bReadOnly;


public:
  void OnCellChange(wxGridEvent &e);
  void OnNotesChange(wxCommandEvent &e);
  DECLARE_EVENT_TABLE();
};


#endif
