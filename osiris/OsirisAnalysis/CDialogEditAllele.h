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
*  FileName: CDialogEditAllele.h
*  Author:   Douglas Hoffman
*
*   THIS SHOULD HAVE BEEN NAMED CDialogEditLocus
*/
#ifndef __C_DIALOG_EDIT_ALLELE_H__
#define __C_DIALOG_EDIT_ALLELE_H__

#include "wx/dialog.h"
#include "COARfile.h"
#include "CPanelLocusDetails.h"
class CPanelUserID;
class CPanelApplyCancel;

class CDialogEditAllele : public wxDialog
{
public:
  CDialogEditAllele(
    const COARlocus &Locus,
    const COARmessages &Messages,
    bool bAllowUserNameOverride,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sSampleName,
    const wxSize &sz = wxDefaultSize);
  CDialogEditAllele(
    COARsample *pSample,
    int nChannel,
    const COARlocus &Locus,
    const COARmessages &Messages,
    bool bAllowUserNameOverride,
    wxWindow *parent,
    wxWindowID id,
    const wxString &sSampleName,
    const wxSize &sz = wxDefaultSize);

  virtual ~CDialogEditAllele();
  virtual bool Validate();
  bool IsModified()
  {
    return m_pPanel->IsModified();
  }
  const COARlocus &GetLocus() const
  {
    return m_pPanel->GetLocus();
  }
  const COARmessages &GetMessages() const
  {
    return m_pPanel->GetMessages();
  }
  bool EditData(COARfile *pf, COARsample *pSample = NULL);
  bool CheckCancel();
  void OnCancel(wxCommandEvent &e);
  void OnChange(wxCommandEvent &e);
private:
  void _InitCommon(
    const wxString &sLocusName, const wxString &sSampleName);
  CPanelApplyCancel *m_pButtonBar;
  CPanelUserID *m_pButtonUser
    ;
  CPanelLocusDetails *m_pPanel;
  COARsample *m_pSample;
  bool m_bAllowUserNameOverride;
  DECLARE_EVENT_TABLE();
};
#endif
