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
*  FileName: CDialogEnableMultiple.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "wxIDS.h"
#include "CDialogEnableMultiple.h"
#include "COARsample.h"
#include "COARreview.h"
#include "CPanelUserID.h"
#include "CGridAnalysis.h" //  for FormatRowLabel()
#include <wx/listbox.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>

class CEnableDisableMultiple : public IAppendReview
{
public:
  CEnableDisableMultiple(CDialogEnableMultiple *pDialog) :
        m_pDialog(pDialog)
  {}
  virtual ~CEnableDisableMultiple()
  {}
  virtual bool AppendReview(const wxString &sName);
private:
  CDialogEnableMultiple *m_pDialog;
};

bool CEnableDisableMultiple::AppendReview(const wxString &sName)
{
  const vector<COARsample *> *pvpSample = m_pDialog->GetSelection();
  vector<COARsample *>::const_iterator itr;
  COARsample *pSample;
  bool bEnable = m_pDialog->IsActionEnable();
  if(pvpSample != NULL)
  {
    for(itr = pvpSample->begin();
      itr != pvpSample->end();
      ++itr)
    {
      pSample = *itr;
      pSample->SetEnabled(sName,bEnable);
    }
  }
  return true;
}

const wxChar *CDialogEnableMultiple::g_psEnable(_T("Enable Samples"));
const wxChar *CDialogEnableMultiple::g_psDisable(_T("Disable Samples"));

CDialogEnableMultiple::CDialogEnableMultiple(
    const vector<COARsample *> *pvpSamples,
    bool bAllowUserNameOverride,
    wxWindow *parent) : 
  wxDialog(
    parent,wxID_ANY,g_psDisable,
    wxDefaultPosition, wxDefaultSize,
    mainApp::DIALOG_STYLE),
    m_pReview(NULL),
    m_pCombo(NULL),
    m_nListSize((int) pvpSamples->size())
{
  vector<COARsample *>::const_iterator itr;
  size_t nSampleCount = pvpSamples->size();
  int n = 0;
  COARsample *pSample;

  m_vnEnabled.reserve(nSampleCount);
  m_vnDisabled.reserve(nSampleCount);
  m_vpSampleEnabled.reserve(nSampleCount);
  m_vpSampleDisabled.reserve(nSampleCount);
  for(itr = pvpSamples->begin();
    itr != pvpSamples->end();
    ++itr)
  {
    n++;
    pSample = *itr;
    if(!pSample->IsSampleType()) {}
    else if(pSample->IsEnabled())
    {
      m_vnEnabled.push_back(n);
      m_vpSampleEnabled.push_back(pSample);
    }
    else
    {
      m_vnDisabled.push_back(n);
      m_vpSampleDisabled.push_back(pSample);
    }
  }
  if(HasSamples())
  {
    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    wxWindow *pWindowTop(NULL);
    int nSizeTOP = wxALIGN_LEFT | wxALL;
    bool bEnable = false;
    if(m_vpSampleEnabled.empty())
    {
      pWindowTop = new wxStaticText(this,wxID_ANY,g_psEnable);
      bEnable = true;
      SetTitle(g_psEnable);
    }
    else if(m_vpSampleDisabled.empty())
    {
      pWindowTop = new wxStaticText(this,wxID_ANY,g_psDisable);
    }
    else
    {
      const wxChar *pChoices[2] = {g_psDisable,g_psEnable};
      wxArrayString choices(2,pChoices);
      m_pCombo = new wxComboBox(this,wxID_ANY,_T(""),
        wxDefaultPosition, wxDefaultSize, choices,wxCB_READONLY);
      pWindowTop = m_pCombo;
      m_pCombo->SetSelection(0);
      nSizeTOP = wxALL | wxEXPAND;
    }
    m_pListBox = new wxListBox(
      this,wxID_ANY,wxDefaultPosition, wxSize(-1,200),
      0,NULL, wxLB_EXTENDED | wxLB_NEEDED_SB);
    m_pReview = new CEnableDisableMultiple(this);
    m_pPanelUser = new CPanelUserID(
      this,wxID_ANY,wxID_ANY,m_pReview,
      UID_BTN_OK | UID_BTN_CANCEL | 
      UID_SPACER_BTN_RIGHT | UID_SEND_BTN_EVENTS,
      !bAllowUserNameOverride);
    m_pPanelUser->Enable(false);
    pSizer->Add(pWindowTop,0,nSizeTOP,ID_BORDER);
    pSizer->Add(m_pListBox,1,(wxALL ^ wxTOP) | wxEXPAND,ID_BORDER);
    pSizer->Add(m_pPanelUser,0,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
    SetSizer(pSizer);
    pSizer->Layout();
    _SetupList(bEnable);
    CentreOnParent();
#ifdef __WXMAC__
    Fit();
#endif
  }
}

CDialogEnableMultiple::~CDialogEnableMultiple()
{
  if(m_pReview != NULL)
  {
    delete m_pReview;
  }
}

const vector<COARsample *> *CDialogEnableMultiple::GetSelection()
{
  int nCount;
  int i;
  int ndx;
  wxArrayInt vnSelect;
  COARsample *pSample;

  vector<COARsample *> *pList = 
     IsActionEnable()
     ? &m_vpSampleDisabled 
        // if action is enable, then enable some disabled samples
     : &m_vpSampleEnabled;
        // otherwise disable some enabled samples
  vector<COARsample *> *pRtn = NULL;
  m_vpSelection.clear();
  nCount = m_pListBox->GetSelections(vnSelect);
  if(nCount)
  {
    m_vpSelection.reserve((size_t) nCount);
    for(i = 0; i < nCount; i++)
    {
      ndx = vnSelect.Item(i);
      pSample = pList->at((size_t)ndx);
      m_vpSelection.push_back(pSample);
    }
    pRtn = &m_vpSelection;
  }
  return pRtn;
}
bool CDialogEnableMultiple::IsActionEnable()
{
  bool bRtn = false;
  if(m_pCombo != NULL)
  {
    bRtn = !!m_pCombo->GetSelection();
    // GetSelection()
    // 0 is for disable,
    // 1 is for enable
  }
  else if(!m_vpSampleDisabled.empty())
  {
    bRtn = true;
  }
  return bRtn;
}


bool CDialogEnableMultiple::Validate()
{
  int nCount = _GetSelectionCount();
  if(!nCount)
  {
    mainApp::ShowError(
      _T("Select at least one sample to continue."),this);
  }
  return !!nCount;
}

int CDialogEnableMultiple::_GetSelectionCount()
{
  wxArrayInt vn;
  int nCount = m_pListBox->GetSelections(vn);
  return nCount;
}
void CDialogEnableMultiple::_SetupList(bool bEnabled)
{
  wxString sListItem;
  const wxChar *psTitle;
  vector<COARsample *> *pvpSampleList;
  vector<int> *pnNdxList;
  vector<COARsample *>::iterator itrSample;
  vector<int>::iterator itrN;
  int nListSize;
  if(bEnabled)
  {
    // user wants to (re)enable disabled samples
    pvpSampleList = &m_vpSampleDisabled;
    pnNdxList = &m_vnDisabled;
    psTitle = g_psEnable;
  }
  else
  {
    // user wants to disable samples
    pvpSampleList = &m_vpSampleEnabled;
    pnNdxList = &m_vnEnabled;
    psTitle = g_psDisable;
  }
  SetTitle(psTitle);
  m_pListBox->Clear();
  nListSize = (int)pnNdxList->size();
  wxASSERT_MSG(nListSize == (int)pvpSampleList->size(),
    _T("Sample list and row list do not match"));
  itrN = pnNdxList->begin();
  for(itrSample = pvpSampleList->begin();
    itrSample != pvpSampleList->end();
    ++itrSample)
  {
    sListItem = CGridAnalysis::FormatRowLabel(
      *itrN,m_nListSize,(*itrSample)->GetName());
    m_pListBox->Append(sListItem);
    ++itrN;
  }
  m_pPanelUser->Enable(false);
}

void CDialogEnableMultiple::OnComboBox(wxCommandEvent &)
{
  _SetupList();
}
void CDialogEnableMultiple::OnSelect(wxCommandEvent &)
{
  int nCount = _GetSelectionCount();
  m_pPanelUser->Enable(!!nCount);
}

BEGIN_EVENT_TABLE(CDialogEnableMultiple,wxDialog)
EVT_LISTBOX(wxID_ANY,CDialogEnableMultiple::OnSelect)
EVT_COMBOBOX(wxID_ANY,CDialogEnableMultiple::OnComboBox)
END_EVENT_TABLE()



