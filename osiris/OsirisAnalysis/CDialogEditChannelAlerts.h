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
*  FileName: CDialogEditChannelAlerts.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_EDIT_CHANNEL_ALERTS_H__
#define __C_DIALOG_EDIT_CHANNEL_ALERTS_H__

#include <wx/dialog.h>
#include "CPanelSampleAlertNotebook.h"
class CPanelUserID;
class COARsample;
class COARmessages;
class COARfile;

class CDialogEditChannelAlerts : public wxDialog
{
public:
  virtual ~CDialogEditChannelAlerts();

  //  constructor for channel alerts except ILS

  CDialogEditChannelAlerts(
    const COARfile *pFile,
    const COARsample *pSample, 
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const wxSize &size = wxDefaultSize,
    const map<int,wxString> *pmapChannelNames = NULL,
    int nSelect = SA_NDX_SAMPLE
    );
  virtual bool Validate();
  bool IsModified()
  {
    return m_pPanel->IsModified();
  }
  const COARmessages &GetDirMessages()
  {
    return m_pPanel->GetDirMessages();
  }
  const COARmessages &GetSampleMessages()
  {
    return m_pPanel->GetSampleMessages();
  }
  const COARmessages &GetILSMessages()
  {
    return m_pPanel->GetILSMessages();
  }
  const COARmessages &GetChannelMessages()
  {
    return m_pPanel->GetChannelMessages();
  }
/*
  const wxString &GetSampleNotes()
  {
    return m_pPanel->GetSampleNotes();
  }
  const wxString &GetILSNotes()
  {
    return m_pPanel->GetILSNotes();
  }
  const wxString &GetChannelNotes()
  {
    return m_pPanel->GetChannelNotes();
  }
*/
  bool EditData(COARfile *pf, COARsample *pSample);
  bool CheckCancel();
  void OnCancel(wxCommandEvent &e);
  void OnChange(wxCommandEvent &e);
private:
  CPanelUserID *m_pPanelUser;
  CPanelSampleAlertNotebook *m_pPanel;
  DECLARE_EVENT_TABLE();
};

#endif
