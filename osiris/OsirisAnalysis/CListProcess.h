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

*  FileName: CListProcess.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_LIST_PROCESS_H__
#define __C_LIST_PROCESS_H__

#include <wx/listctrl.h>
#include "CDirList.h"

class CListProcess : public wxListCtrl
{
public:
  CListProcess(
    CDirList *pDirList, 
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    const wxSize &size = wxDefaultSize);
  virtual ~CListProcess();
  virtual bool TransferDataToWindow();
  void UpdateStatus(int nItem = -1);
  void GetInfo(wxListItem &itm, long ndx = 0)
  {
    itm.SetStateMask(-1);
    itm.SetMask(-1);
    itm.SetId(ndx);
    GetItem(itm);
  }
  bool IsSelected(long ndx);
  void Select(long ndx, bool bSelect = true);
  void SelectOne(long ndx);
  void Clear(long ndx)
  {
    Select(ndx,false);
  }
  void SelectAll(bool bSelect = true);
private:
  enum
  {
    ITEM_DIR = 0,
    ITEM_START,
    ITEM_STOP,
    ITEM_STATUS
  };
  CDirList *m_pDirList;
  wxListItem m_itm;
};
#endif
