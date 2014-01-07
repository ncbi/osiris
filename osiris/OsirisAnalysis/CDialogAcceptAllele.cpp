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
*  FileName: CDialogAcceptAllele.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include <wx/sizer.h>
#include <wx/splitter.h>
#include "CGridAlerts.h"
#include "CGridLocusPeaks.h"
#include "CDialogAcceptAllele.h"
#include "CParmOsiris.h"
#include "CPanelLocusDetails.h"
#include "CPanelUserID.h"
#include "COARlocus.h"
#include "COARmessage.h"
#include "wxIDS.h"
#include "Platform.h"

CDialogAcceptAllele::~CDialogAcceptAllele()
{
  delete m_pRev;
  m_pRev = NULL;
}

CDialogAcceptAllele::CDialogAcceptAllele(
  COARlocus *pLocus,
  const COARmessages *pMessages,
  wxWindow *parent,
  wxWindowID id,
  bool bAllowUserOverride,
  const wxString &sSampleName,
  const wxSize &sz) :
    wxDialog(parent,
         id, 
         wxEmptyString, 
         wxDefaultPosition, 
         sz,
         mainApp::DIALOG_STYLE),
    m_pRev(new CAppendAcceptanceLocus(pLocus))
{
  const int USER_FLAG =
    UID_BTN_ACCEPT | UID_BTN_CANCEL | UID_BTN_EDIT |
    UID_SPACER_BTN_CENTER | UID_SEND_BTN_EVENTS;

  m_pSplitter = new wxSplitterWindow(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,ID_SPLITTER_STYLE);
  m_pPeaks = new CGridLocusPeaks(
    pLocus->GetSample(),
    pLocus->GetChannelNr(),
    pLocus->GetName(),
    m_pSplitter,
    wxID_ANY,
    true);
  m_MsgEdit.CopyOnly(*pMessages,pLocus->GetAlerts());
  m_pGridAlerts = new CGridAlerts(
    &m_MsgEdit,m_pSplitter,wxID_ANY,0,true);
  CPanelUserID *pPanelUser = new CPanelUserID(
    this,wxID_ANY,wxID_ANY,m_pRev,USER_FLAG,!bAllowUserOverride);

  wxString sTitle(_T("Accept Alleles for "));
  sTitle.Append(pLocus->GetName());
  if(!sSampleName.IsEmpty())
  {
    sTitle.Append(_T(" on "));
    sTitle.Append(sSampleName);
  }
  SetTitle(sTitle);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pSplitter,1,wxEXPAND);
  pSizer->Add(pPanelUser,0,wxEXPAND | wxALL, ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
  m_pSplitter->SplitHorizontally(m_pPeaks,m_pGridAlerts,0);
  m_pSplitter->SetMinimumPaneSize(1);
  CentreOnParent();
}
void CDialogAcceptAllele::OnEdit(wxCommandEvent &e)
{
  int n = e.GetId();
  EndModal(n);
}

BEGIN_EVENT_TABLE(CDialogAcceptAllele,wxDialog)
EVT_BUTTON(IDmenuEditCell,CDialogAcceptAllele::OnEdit)
END_EVENT_TABLE()
