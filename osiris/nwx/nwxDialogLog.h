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

*  FileName: nwxDialogLog.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_DIALOG_LOG_H__
#define __NWX_DIALOG_LOG_H__

#include <wx/log.h>
#include <wx/dialog.h>
#include <wx/textctrl.h>
#include <wx/button.h>

class nwxDialogLog : public wxDialog
{
public:
  nwxDialogLog(
    wxWindow *parent, 
    wxWindowID id = wxID_ANY, 
    const wxString &sTitle = _T("Error Log"), 
    long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
  virtual ~nwxDialogLog();
  wxLogTextCtrl *GetLog()
  {
    return m_pLog;
  }
  virtual bool Show(bool show = true);
  void OnClear(wxCommandEvent &);
  wxLog *SetTarget()
  {
    return wxLog::SetActiveTarget(m_pLog);
  }
private:
  wxLogTextCtrl *m_pLog;
  wxTextCtrl *m_pTextCtrl;
  wxButton *m_pButtonOK;
  DECLARE_EVENT_TABLE()
};


#endif
