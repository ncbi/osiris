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
*  FileName: CDialogCellHistory.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include <wx/sizer.h>
#include <wx/debug.h>
#include "CDialogApprove.h"
#include "COARfile.h"
#include "CPanelSampleAlertNotebook.h"
#include "CPanelLocusDetails.h"
#include "wxIDS.h"
#include "CSplitterCellHistory.h"
#include "CPanelUserID.h"

CDialogApprove::~CDialogApprove()
{
  if(m_pApprove != NULL)
  {
    delete m_pApprove;
    m_pApprove = NULL;
  }
}

CDialogApprove::CDialogApprove(
    const COARsample &Sample,
    int nChannel,
    COARlocus *pLocus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    bool bAllowUserNameOverride,
    wxWindow *parent, 
    wxWindowID id,
    const wxSize &sz)
    : wxDialog(parent,id,wxEmptyString,
        wxDefaultPosition,sz,
        mainApp::DIALOG_STYLE),
      m_bAllowUserNameOverride(bAllowUserNameOverride)
{
  CSplitterCellHistory *pSplitter = 
    new CSplitterCellHistory(
      Sample,
      nChannel,
      *pLocus,
      Msg,
      setDateTime,
      this, 
      IDhistoryPanel);
  wxString sTitle("Approve Editing for ");
  sTitle.Append(pLocus->GetName());
  const wxString &sSampleName(Sample.GetName());
  if(!sSampleName.IsEmpty())
  {
    sTitle.Append(" on ");
    sTitle.Append(sSampleName);
  }
  SetTitle(sTitle);
  m_pApprove = new CAppendReviewLocus(pLocus);
  _Setup(pSplitter);
}
CDialogApprove::CDialogApprove(
    int nSelect,
    COARfile *pFile,
    COARsample *pSample,
    const map<int,wxString> &mapChannelNames,
    wxWindow *parent,
    wxWindowID id,
    const wxSize &sz)
    : wxDialog(parent,id,wxEmptyString,
        wxDefaultPosition,sz,
        mainApp::DIALOG_STYLE),
      m_bAllowUserNameOverride(pFile->GetReviewerAllowUserOverride())
{
  CSplitterCellHistory *pSplitter = 
    new CSplitterCellHistory(
      nSelect,
      true,
      *pFile,
      *pSample,
      mapChannelNames,
      this,
      IDhistoryPanel);
  wxString sType;

  switch(nSelect)
  {
  case SA_NDX_SAMPLE:
    sType = CPanelSampleAlertNotebook::g_sLabelSample;
    m_pApprove = new COARSampleAppendReview(pSample);
    break;
  case SA_NDX_ILS:
    sType = CPanelSampleAlertNotebook::g_sLabelILS;
    m_pApprove = new COARsampleILSAppendReview(pSample);
    break;
  case SA_NDX_CHANNEL:
    sType = CPanelSampleAlertNotebook::g_sLabelChannel;
    m_pApprove = new COARsampleChannelAppendReview(pSample);
    break;
  case SA_NDX_DIR:
    sType = CPanelSampleAlertNotebook::g_sLabelDir;
    m_pApprove = new COARfileDirReview(pFile);
    break;
  default:
    m_pApprove = NULL;
    {
      wxString sErr;
      sErr.sprintf(
        "CDialogCellHistory::CDialogCellHistory()\n"
        "nSelect (%d) is invalid\n",nSelect);
      mainApp::LogMessage(sErr);
      wxASSERT_MSG(0,sErr);
    }
    break;
  }
  wxString s("Approve Editing on ");
  s.Append(sType);
  s.Append(" Alerts for ");
  s.Append(pSample->GetName());
  SetTitle(s);
  _Setup(pSplitter);
}
void CDialogApprove::_Setup(
  CSplitterCellHistory *pSplitter)
{
  const int USER_FLAG =
    UID_BTN_APPROVE | UID_BTN_CANCEL | UID_BTN_EDIT |
    UID_SPACER_BTN_CENTER | UID_SEND_BTN_EVENTS;

  CPanelUserID *pPanel = new CPanelUserID(
    this,wxID_ANY,wxID_ANY,m_pApprove,USER_FLAG,
    !m_bAllowUserNameOverride);

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pSplitter,1,wxEXPAND);
  pSizer->Add(pPanel,0,wxALL | wxEXPAND,ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
  pSplitter->Split();
}
void CDialogApprove::OnEdit(
  wxCommandEvent &e)
{
  int n = e.GetId();
  EndModal(n);
}

BEGIN_EVENT_TABLE(CDialogApprove,wxDialog)
EVT_BUTTON(IDmenuEditCell,CDialogApprove::OnEdit)
END_EVENT_TABLE()
