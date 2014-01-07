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
*  FileName: CPanelHistory.cpp
*  Author:   Douglas Hoffman
*
*/

#include <wx/sizer.h>
#include "CPanelHistory.h"
#include "CHistoryTime.h"
#include "wxIDS.h"
#include "nwx/nwxString.h"

CPanelHistory::CPanelHistory(const set<wxDateTime> &setDateTime,
    wxWindow *parent, wxWindowID id)  :
      wxPanel(parent,id)
{
  m_pButtonBack = new wxButton(this,wxID_ANY,_T("<<"),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  m_pButtonBack1 = new wxButton(this,wxID_ANY,_T(" < "),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  m_pButtonFwd1 = new wxButton(this,wxID_ANY,_T(" > "),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  m_pButtonFwd = new wxButton(this,wxID_ANY,_T(">>"),
    wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
  wxFont fnt = m_pButtonBack->GetFont();
  fnt.SetWeight(wxFONTWEIGHT_BOLD);
  m_pButtonBack->SetFont(fnt);
  m_pButtonBack1->SetFont(fnt);
  m_pButtonFwd->SetFont(fnt);
  m_pButtonFwd1->SetFont(fnt);
  //
  //  make combo box
  //

  //  copy wxDateTime from set to vector and
  //  format strings for combo box

  wxArrayString as;
  wxString s;
  size_t nSIZE = setDateTime.size();
//  bool bEMPTY = false;
  if(!nSIZE)
  {
    wxDateTime x((time_t) 0);
    nSIZE++;
    m_vTime.push_back(x);
  }
  else
  {
    m_vTime.reserve(nSIZE);
  }
  as.Alloc(nSIZE + 1);
  as.Add(CHistoryTime::CURRENT);

  for(set<wxDateTime>::const_reverse_iterator itr = setDateTime.rbegin();
    itr != setDateTime.rend();
    ++itr)
  {
    m_vTime.push_back(*itr);
    s = nwxString::FormatDateTime(*itr); // format string
    as.Add(s); // add to array for combo box
  }
  m_pComboHistory = new wxComboBox(
    this,wxID_ANY,CHistoryTime::CURRENT,
    wxDefaultPosition, wxDefaultSize,as,wxCB_READONLY);
  //
  //  combo box has been made, now layout
  //

  EnableButtons();
  int nFLAGS = (wxTOP | wxBOTTOM | wxLEFT | wxALIGN_CENTRE);
  wxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
  pSizer->AddStretchSpacer(1);
  pSizer->Add(m_pButtonBack,  0, nFLAGS ^ wxLEFT, ID_BORDER);
  pSizer->Add(m_pButtonBack1, 0, nFLAGS, ID_BORDER);
  pSizer->Add(m_pComboHistory,0, nFLAGS, ID_BORDER);
  pSizer->Add(m_pButtonFwd1,  0, nFLAGS, ID_BORDER);
  pSizer->Add(m_pButtonFwd,   0, nFLAGS, ID_BORDER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
  pSizer->Layout();
}

CPanelHistory::~CPanelHistory() {};

void CPanelHistory::EnableButtons()
{
  int n = m_pComboHistory->GetSelection();
  int nMax = m_pComboHistory->GetCount() - 1;
  bool bFwd = true;
  bool bRev = true;
  if(nMax <= 0)
  {
    bFwd = false;
    bRev = false;
  }
  else if(n == 0)
  {
    bFwd = false;
  }
  else if(n == nMax)
  {
    bRev = false;
  }
  m_pButtonBack->Enable(bRev);
  m_pButtonBack1->Enable(bRev);
  m_pButtonFwd->Enable(bFwd);
  m_pButtonFwd1->Enable(bFwd);
}

const wxDateTime *CPanelHistory::GetDateTime()
{
  // return a pointer to the selected wxDateTime
  // or NULL is "Current" is selected
  int n = m_pComboHistory->GetSelection();
  wxDateTime *pdt(NULL);
  if(n > 0)
  {
    n--;
    pdt = &(m_vTime.at(n));
  }
  return pdt;
}

void CPanelHistory::OnButton(wxCommandEvent &e)
{
  wxButton *pObj = (wxButton *)e.GetEventObject();
  int nCount = m_pComboHistory->GetCount();
  int n = -1;
  if(!nCount) {;} // do nothing
  if(pObj == m_pButtonBack1)
  {
    n = m_pComboHistory->GetSelection() + 1;
  }
  else if(pObj == m_pButtonBack)
  {
    n = nCount - 1;
  }
  else if(pObj == m_pButtonFwd)
  {
    n = 0;
  }
  else if(pObj == m_pButtonFwd1)
  {
    n = m_pComboHistory->GetSelection() - 1;
  }
  if( (n >= 0) && (n < nCount) )
  {
    m_pComboHistory->Select(n);
    OnComboBox(e);
  }
}
void CPanelHistory::SetOriginal()
{
  int nCount = m_pComboHistory->GetCount();
  if(nCount)
  {
    nCount--;
    _Select(nCount);
  }
}
void CPanelHistory::_Select(int n)
{
  if(n != m_pComboHistory->GetSelection())
  {
    wxCommandEvent e(wxEVT_COMMAND_COMBOBOX_SELECTED,wxID_ANY);
    e.SetEventObject(m_pComboHistory);
    m_pComboHistory->Select(n);
    OnComboBox(e);
  }
}
void CPanelHistory::OnComboBox(wxCommandEvent &)
{
  EnableButtons();
  wxCommandEvent ev(wxEVT_COMMAND_ENTER,GetId());
  ProcessEvent(ev);
}

BEGIN_EVENT_TABLE(CPanelHistory,wxPanel)
EVT_BUTTON(wxID_ANY,CPanelHistory::OnButton)
EVT_COMBOBOX(wxID_ANY,CPanelHistory::OnComboBox)
END_EVENT_TABLE()
