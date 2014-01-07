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
*  FileName: CMenuHistory.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_HISTORY_H__
#define __C_MENU_HISTORY_H__

#include <wx/menu.h>
#include <wx/datetime.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include <nwx/nsstd.h>
#include "CHistoryTime.h"
#include "wxIDS.h"

class COARfile;
class CMenuHistory : public wxMenu
{
  // popup menu for selecting a historic time when the file
  // was saved.  This displays a limited number
  // of times, but is also used to store the 
  // time chosen by the user even if it doesn't appear
  // in the list.
  //
  // A time outside the list is chosen when the 
  // user selects 'More...' from this list and 
  // the time is chosen from the resulting dialog window
  //
public:
  CMenuHistory();
  CMenuHistory(COARfile *pFile, bool bAnalysis = true);
  virtual ~CMenuHistory();
  const wxDateTime *GetSelected();
  const wxDateTime *Select(int nID);
  const wxDateTime *UpdateList(bool bForce = false); 
          // check the History set and update menu if necessary
  void SetFile(COARfile *pFile,bool bAnalysis = true);
  bool SelectTime(const wxDateTime *pDateTime);
  bool IsCurrent();
  bool SetupCheckedItem();
  void SetCellLabel(const wxString &s)
  {
    SetLabel(IDmenuHistoryView,s);
  }
  void EnableCell(bool b)
  {
    Enable(IDmenuHistoryView,b);
  }
  COARfile *GetFile()
  {
    return m_pFile;
  }
  void CopyState(CMenuHistory *pTo);
  CMenuHistory *Clone()
  {
    CMenuHistory *pRtn = new CMenuHistory(m_pFile,m_bAnalysis);
    CopyState(pRtn);
    return pRtn;
  }
private:
  bool _CheckById(int nID);
  void _Cleanup();
  void _Setup();
  bool _AppendTime(const wxDateTime &x, int nID, const wxDateTime *pCheck);
  vector<wxDateTime> m_vDate;
  CHistoryTime m_HistoryTime;
    // NULL if 'current' is selected,
    // &m_DateLast otherwise
  COARfile *m_pFile;
  size_t m_nHistorySize;
  bool m_bAnalysis;
  static const size_t MAXHIST;
  DECLARE_ABSTRACT_CLASS(CMenuHistory)
};


#endif
