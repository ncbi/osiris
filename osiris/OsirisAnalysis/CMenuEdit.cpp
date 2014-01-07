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
*  FileName: CMenuEdit.cpp
*    edit popup menu for class CFrameAnalysis
*    with static utility functions for the "Table"
*    pulldown menu on the menu bar
*
*    Also contains CMenuReview and CMenuAccept
*
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CMenuEdit.h"
#include "wxIDS.h"
#include "COARfile.h"

#define EDIT_SAMPLE_LABEL "Edit " COAR_NOTICE_DISPLAY_CAP " and Notes..."

const wxString CMenuEdit::LABEL_EDIT_DIRECTORY(_T("Edit Directory Notices..."));
const wxString CMenuEdit::LABEL_ACCEPT_ALERTS(_T("Accept Alerts"));
const wxString CMenuEdit::LABEL_REVIEW_EDIT(_T("Review Editing"));

// the order of the following three arrays must match
// the enum in class CMenuReviewAccept

const wxChar *CMenuReviewAccept::g_asLabels[CMenuReviewAccept::ITEM_COUNT] =
{
  _T("Locus"),
  _T("Sample"),
  _T("ILS"),
  _T("Channels"),
  _T("Directory")
};

const int CMenuReviewAccept::g_anIDAccept[CMenuReviewAccept::ITEM_COUNT] =
{
  IDmenuAcceptLocus,
  IDmenuAcceptSample,
  IDmenuAcceptILS,
  IDmenuAcceptChannels,
  IDmenuAcceptDirectory
};
const int CMenuReviewAccept::g_anIDReview[CMenuReviewAccept::ITEM_COUNT] =
{  
  IDmenuReviewLocus,
  IDmenuReviewSample,
  IDmenuReviewILS,
  IDmenuReviewChannels,
  IDmenuReviewDirectory
};

bool CMenuReviewAccept::AllDisabled()
{
  bool bRtn = true;
  for(size_t i = 0; i < ITEM_COUNT; i++)
  {
    if(IsEnabled(m_pnIDS[i]))
    {
      bRtn = false;
      i = 32000; // loop exit
    }
  }
  return bRtn;
}
void CMenuReviewAccept::SetLocusName(
  wxMenuItem *pItem,const wxString &sLocus, const wxString &sAccel)
{
  wxString s(sLocus);
  bool bEnable = true;
  if(sLocus.IsEmpty())
  {
    s = _T("Locus");
    bEnable = false;
  }
  s.Append(sAccel);
  pItem->SetItemLabel(s);
  pItem->Enable(bEnable);
}


CMenuReviewAccept::CMenuReviewAccept(bool bAccept) :
  m_pnIDS(bAccept ? &g_anIDAccept[0] : &g_anIDReview[0])
{
  m_sAccel = bAccept ? _T("\tAlt+A") : _T("\tAlt+R");
  const int *pn = m_pnIDS;
  const wxChar **psLabel = &g_asLabels[0];
  // order of the following must be consistent with static int arrays
  m_pItemLocus = Append(*pn++,*psLabel++);
  for(int i = 1; i < ITEM_COUNT; i++)
  {
    Append(*pn++,*psLabel++);
  }
}
void CMenuReviewAccept::CopyState(CMenuReviewAccept *pTo)
{
  const int *pnID = m_pnIDS;
  for(size_t i = 0; i < ITEM_COUNT; i++)
  {
    CBaseMenuEdit::CopyState(pTo,this,*pnID);
  }
}
CMenuEdit::CMenuEdit()
{
  Append(
    IDmenuEditDirectory,
    LABEL_EDIT_DIRECTORY
    );
  Append(
    IDmenuEditCell,
    _T("."),
    _T("Edit selected data")
    );
  m_pMenuAccept = new CMenuAccept;
  m_pMenuItemAccept = AppendSubMenu(m_pMenuAccept,LABEL_ACCEPT_ALERTS);
  m_pMenuReview = new CMenuReview;
  m_pMenuItemReview = AppendSubMenu(m_pMenuReview,LABEL_REVIEW_EDIT);
  Append(IDmenuDisableSample,_T("."));
}
CMenuEdit::~CMenuEdit() {;}

void CBaseMenuEdit::SetLocus(
  const wxString &s, 
  bool bEnabled
  )
{
  m_pMenuReview->SetLocusName(s);
  m_pMenuAccept->SetLocusName(s);
  if(s.IsEmpty())
  {
    SetLabel(IDmenuEditCell,_T(EDIT_SAMPLE_LABEL));
  }
  else
  {
    wxString sLabel;
    sLabel = _T("Edit ");
    sLabel.Append(s);
    sLabel.Append(_T("..."));
    SetLabel(IDmenuEditCell,sLabel);
  }
  Enable(IDmenuEditCell,bEnabled);
}
void CBaseMenuEdit::EnableEditDirectory(bool bEnable)
{
  Enable(IDmenuEditDirectory,bEnable);
}
void CBaseMenuEdit::EnableEditLocus(bool bEnable)
{
  Enable(IDmenuEditCell,bEnable);
}
void CBaseMenuEdit::EnableAccept(bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir)
{
  EnableByType(m_pMenuItemAccept,m_pMenuAccept,bLocus,bSample,bILS,bChannels,bDir);
}
void CBaseMenuEdit::EnableReview(bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir)
{
  EnableByType(m_pMenuItemReview,m_pMenuReview,bLocus,bSample,bILS,bChannels,bDir);
}

void CBaseMenuEdit::SetSampleEnabled(bool bEnable, bool bAllowDisable)
{
  if(!bAllowDisable) { bEnable = true; }
  wxString sLabel = COARsample::EnableDisableSampleStr(!bEnable);
  sLabel.Append("\tAlt+X");
  SetLabel(IDmenuDisableSample,sLabel);
  m_pMenuItemAccept->Enable(bEnable && m_pMenuAccept->NotAllDisabled());
  m_pMenuItemReview->Enable(bEnable && m_pMenuReview->NotAllDisabled());
  Enable(IDmenuEditCell,bEnable);
  //
  // if bAllowDisable is false, then the sample is enabled
  //   and you cannot disable it.
  //
  Enable(IDmenuDisableSample,bAllowDisable);
}
void CBaseMenuEdit::EnableByType(
  wxMenuItem *pItem, CMenuReviewAccept *pMenu,
  bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir)
{
  bool bSomething = bLocus || bSample || bILS || bChannels || bDir;
  pMenu->EnableLocus(bLocus);
  pMenu->EnableSample(bSample);
  pMenu->EnableILS(bILS);
  pMenu->EnableChannels(bChannels);
  pMenu->EnableDirectory(bDir);
  pItem->Enable(bSomething);
}
void CBaseMenuEdit::CopyState(wxMenuItem *pmTo, wxMenuItem *pmFrom)
{
  pmTo->SetItemLabel(pmFrom->GetItemLabel());
  pmTo->SetHelp(pmFrom->GetHelp());
  pmTo->Enable(pmFrom->IsEnabled());
  if(pmTo->IsCheckable() && pmFrom->IsCheckable())
  {
    pmTo->Check(pmFrom->IsChecked());
  }
}
void CBaseMenuEdit::CopyState(wxMenu *pmTo, wxMenu *pmFrom, int nID)
{
  wxMenuItem *pmiTo = pmTo->FindItem(nID);
  if(pmiTo != NULL)
  {
    wxMenuItem *pmiFrom = pmFrom->FindItem(nID);
    if(pmiFrom != NULL)
    {
      CopyState(pmiTo,pmiFrom);
    }
  }
}

void CBaseMenuEdit::CopyStateEdit(CBaseMenuEdit *pmTo)
{
  CopyState(pmTo,this,IDmenuEditDirectory);
  CopyState(pmTo,this,IDmenuEditCell);
  CopyState(pmTo,this,IDmenuDisableSample);
  CopyState(pmTo->GetMenuItemAccept(),this->GetMenuItemAccept());
  CopyState(pmTo->GetMenuItemReview(),this->GetMenuItemReview());
  GetMenuAccept()->CopyState(pmTo->GetMenuAccept());
  GetMenuReview()->CopyState(pmTo->GetMenuReview());
}

