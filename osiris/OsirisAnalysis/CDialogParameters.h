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

*  FileName: CDialogParameters.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_PARAMETERS_H__
#define __C_DIALOG_PARAMETERS_H__

#include <wx/wx.h>
#include "CParmOsiris.h"

class wxHyperlinkEvent;
class CLabSettings;
class CXMLmessageBook;

class CDialogParameters : public wxDialog
{
public:
  CDialogParameters( 
    wxWindow *parent, 
    const CParmOsiris *pOsiris, 
    const wxString *psFileName = NULL,
    const wxString *psLadderInfo = NULL,
    const wxString *psOrigin = NULL,
    CLabSettings *pLabSettings = NULL,
    const CXMLmessageBook *pMsgBook = NULL
    );
  virtual ~CDialogParameters();
  void OnHyperlink(wxHyperlinkEvent &);
private:
  wxWindow *CreateLabSettingsLink(const wxString &sLabel);
  wxWindow *CreateHyperlink(int nID,const wxString &sDir, bool bForce = false);
  wxString m_sFileName;
  CLabSettings *m_pLabSettings;
  const CXMLmessageBook *m_pMsgBook;
  DECLARE_EVENT_TABLE()
};


#endif
