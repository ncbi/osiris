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
*  FileName: CDialogHistoryAll.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_HISTORY_ALL_H__
#define __C_DIALOG_HISTORY_ALL_H__

#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxString.h"

class COARfile;

class CDialogHistoryAll : public wxDialog
{
public:
  CDialogHistoryAll(
    const wxDateTime *pCurrentSelection,
    COARfile *pFile,
    wxWindow *parent,
    wxWindowID nID = wxID_ANY);
  virtual ~CDialogHistoryAll();
  void OnListDoubleClick(wxCommandEvent &e);
  virtual bool TransferDataFromWindow();
  const wxDateTime *GetSelection()
  {
    return m_pTimeSelected;
  }
  void SetDateTime(const wxDateTime *pTime);
  void SetCurrentTime()
  {
    SetDateTime(NULL);
  }

private:
  bool _AppendTime(wxArrayString *paStr, const wxDateTime &dt)
  {
    // return true if original time, false otherwise
    bool bRtn = (m_timeSelected == dt);
    m_vTime.push_back(dt);
    paStr->Add(nwxString::FormatDateTime(dt));
    return bRtn;
  }
  vector<wxDateTime> m_vTime;
  wxDateTime m_timeSelected;
  wxDateTime *m_pTimeSelected;
    // NULL if 'current', &m_timeSelected otherwise
  wxListBox  *m_pList;
  DECLARE_EVENT_TABLE()
};

#endif
