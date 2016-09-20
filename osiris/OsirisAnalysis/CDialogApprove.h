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
*  FileName: CDialogApprove.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_DIALOG_APPROVE_H__
#define __C_DIALOG_APPROVE_H__

#include <wx/dialog.h>
#include "nwx/stdb.h"
#include <set>
#include <map>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/PersistentSize.h"

class CSplitterCellHistory;
class COARsample;
class COARlocus;
class COARmessages;
class IAppendReview;
class COARfile;
class CPanelUserID;

class CDialogApprove : public wxDialog
{
public:
  CDialogApprove(
    const COARsample &Sample,
    int nChannel,
    COARlocus *pLocus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    bool bAllowUserNameOverride,
    bool bShowUserID,
    wxWindow *parent, 
    wxWindowID id = wxID_ANY,
    const wxSize &sz = wxDefaultSize);
  CDialogApprove(
    int nSelect,
    COARfile *pFile,
    COARsample *pSample,
    bool bShowUserID,
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const wxSize &sz = wxDefaultSize
    );
  virtual ~CDialogApprove();
  void SetUserID(const wxString &sUser);
  const wxString &GetUserID();
  DECLARE_PERSISTENT_SIZE_POSITION

private:
  void _Setup(CSplitterCellHistory *pSplitter);
  void OnEdit(wxCommandEvent &e);
  wxString m_sPersistName;
  IAppendReview *m_pApprove;
//  IAppendReview *m_pIARmanaged;
  CPanelUserID *m_pPanelUser;
  bool m_bAllowUserNameOverride;
  bool m_bShowUserID;
  DECLARE_EVENT_TABLE()
};


#endif
