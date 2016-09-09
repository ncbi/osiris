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
*  FileName: CMenuSample.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CMenuSample.h"

#include <wx/button.h>
#include <wx/utils.h>
#include <wx/wrapsizer.h>
#include "CMDIFrame.h"
#include "CButtonGraph.h"
#include "nwx/nwxKeyState.h"
#include "wxIDS.h"

CMenuSample::CMenuSample() : wxMenu()
{
  Append(IDmenuDisplayGraph,
    "Display &Graph\tAlt+G",
    "Display channel plots.  Hold the shift key down for a single plot"
    );
  Append(IDmenuTable,"Show &Table\tAlt+T","Display the analysis table");
//  Append(IDmenuShowHideToolbar,CMDIFrame::HIDE_TOOLBAR);
  Append(IDmenuHistory,"&History...\tCtrl+H");
  Append(IDmenuDisableSample,DISABLE);

  Append(IDSampleAccept,"Accept &Alerts\tAlt+A");
  Append(IDSampleApprove,"&Review Alerts\tAlt+R");
  Append(IDSampleApply,"A&pply\tAlt+P");
  Append(IDSampleApplyAll,"A&pply All\tAlt+Shift+P");
};

void CMenuSample::SetSampleEnabled(bool bEnabled)
{
  wxMenuItem *pItem = FindItem(IDmenuDisableSample);
  if(pItem != NULL)
  {
    pItem->SetItemLabel(GetDisabledLabel(bEnabled));
  }
}

const wxChar * const CMenuSample::DISABLE(wxT("&Disable Sample\tAlt+X"));
const wxChar * const CMenuSample::ENABLE(wxT("&Enable Sample\tAlt+X"));


IMPLEMENT_ABSTRACT_CLASS(CToolbarSample,wxPanel)

wxButton *CToolbarSample::_CreateButton(int nID,const wxString &sLabel)
{
  wxButton *pRtn = new wxButton(this,nID,sLabel,
      wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  _StoreButton(pRtn, nID);
  return pRtn;
}
wxButton *CToolbarSample::_FindButton(int nID)
{
  mapButton::iterator itr = m_mapButton.find(nID);
  wxButton *pRtn = (itr == m_mapButton.end()) ? NULL : itr->second;
  return pRtn;
}
void CToolbarSample::EnableItem(int nID,bool bEnable)
{
  wxButton *pBtn = _FindButton(nID);
  if(pBtn != NULL)
  {
    pBtn->Enable(bEnable);
  }
}
void CToolbarSample::SetSampleEnabled(bool bEnabled)
{
  wxButton *pBtn = _FindButton(IDmenuDisableSample);
  if(pBtn != NULL)
  {
    pBtn->SetLabel(GetDisabledLabel(bEnabled));
  }

}
CToolbarSample::CToolbarSample(CMDIFrame *m_pFrame, wxWindow *parent) :
  wxPanel(parent,wxID_ANY),
  m_pParent(m_pFrame)
{
  const int FLAG =
    wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTRE_VERTICAL;
  wxButton *pButtonGraph = new CButtonGraph(this);
  _StoreButton(pButtonGraph, pButtonGraph->GetId());
  wxButton *pButtonTable = _CreateButton(IDmenuTable,wxT("Table"));
  wxButton *pButtonHistory = _CreateButton(IDmenuHistory,wxT("History"));
  wxButton *pButtonDisable = _CreateButton(IDmenuDisableSample,DISABLE);
  wxButton *pButtonAccept = _CreateButton(IDSampleAccept,wxT("Accept"));
  wxButton *pButtonReview = _CreateButton(IDSampleApprove,wxT("Review"));
  wxButton *pButtonApply = _CreateButton(IDSampleApply,wxT("Apply"));
  pButtonApply->SetToolTip(wxString(wxT(
    "Hold down the shift key to apply\nall changes for this sample"
    )));

  wxBoxSizer *pSizer = new wxWrapSizer(wxHORIZONTAL);
  pSizer->Add(pButtonGraph,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonTable,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonHistory,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonDisable,0,FLAG,ID_BORDER);
  pSizer->AddStretchSpacer(2);
  pSizer->Add(pButtonAccept,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonReview,0,FLAG,ID_BORDER);
  pSizer->Add(pButtonApply,0,FLAG | wxRIGHT,ID_BORDER);
  SetSizer(pSizer);
  Layout();
}
void CToolbarSample::OnButton(wxCommandEvent &e)
{
  if( (e.GetId() == IDSampleApply) && nwxKeyState::Shift())
  {
    e.SetId(IDSampleApplyAll);
  }
  m_pParent->MenuEvent(e);
}
const wxChar * const CToolbarSample::DISABLE(wxT("Disable"));
const wxChar * const CToolbarSample::ENABLE(wxT("Enable"));


BEGIN_EVENT_TABLE(CToolbarSample,wxPanel)
EVT_BUTTON(wxID_ANY,CToolbarSample::OnButton)
END_EVENT_TABLE()
