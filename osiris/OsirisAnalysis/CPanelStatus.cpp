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
*  FileName: CPanelStatus.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "CPanelStatus.h"
#include "CFrameAnalysis.h"
#include "COARfile.h"
#include "CLabSettings.h"
#include "wxIDS.h"

class CStatusInfo
{
public:
  CStatusInfo(int nID, COARlocus *pLocus = NULL) :
    m_pLocus(pLocus),
    m_nID(nID)
  {}
  virtual ~CStatusInfo() 
  {}
  COARlocus *GetLocus() const
  {
    return m_pLocus;
  }
  int GetID() const
  {
    return m_nID;
  }
private:
  COARlocus *m_pLocus;
  int m_nID;
};

CPanelStatus::~CPanelStatus()
{
  vectorptr<CStatusInfo>::cleanup(&m_vpStatus);
}

CPanelStatus::CPanelStatus(
  COARfile *pFile, 
  COARsample *pSample, 
  CFrameAnalysis *parent, 
  wxWindowID id) :
    wxScrolledWindow(parent->GetInfoPanel(),id),
    m_pFile(pFile),
    m_pSample(pSample),
    m_pParent(parent)
{
  _Build();
}

void CPanelStatus::_AddLink(
  const wxString &s, 
  int nID, 
  COARlocus *pLocus, 
  wxBoxSizer *pSizer)
{
  if(!s.IsEmpty())
  {
    wxWindowID nLinkID =
      (wxWindowID)(m_vpStatus.size() + IDhyperlinkStatus);
    wxHyperlinkCtrl *pLink = new wxHyperlinkCtrl(this, nLinkID,s,wxEmptyString,
      wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxHL_ALIGN_LEFT);
    pLink->SetVisitedColour(pLink->GetNormalColour());
    pSizer->Add(pLink,0,wxALIGN_LEFT,0);
    m_vpStatus.push_back(new CStatusInfo(nID,pLocus));
  }
}
void CPanelStatus::_Build()
{
  wxString sStatus;
  wxString sLocusName;
  COARlocus *pLocus;
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  size_t nLocusCount = m_pFile->GetLocusCount();
  size_t nReserve(nLocusCount + 4);
  size_t i;
  int nReview;
  int nAccept;
  bool bReview(false);
  bool bDisabled = false;

  m_vpStatus.reserve(nReserve);
  m_pFile->GetReviewerCounts(&nReview, &nAccept, CLabReview::REVIEW_DIR);
  sStatus = m_pFile->CheckDirStatus(nReview,nAccept,NULL,&bReview);
  _AddLink(
    sStatus,
    bReview ? IDmenuReviewDirectory : IDmenuAcceptDirectory,
    NULL,
    pSizer);

  if(m_pSample->IsEnabled())
  {
    m_pFile->GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_SAMPLE);

    sStatus = m_pFile->CheckSampleStatus(
      m_pSample,nReview,nAccept,NULL,&bReview);
    _AddLink(
      sStatus,
      bReview ? IDmenuReviewSample : IDmenuAcceptSample,
      NULL,
      pSizer);

    m_pFile->GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_ILS);
    sStatus = m_pFile->CheckILSStatus(
      m_pSample,nReview,nAccept,NULL,&bReview);
    _AddLink(
      sStatus,
      bReview ? IDmenuReviewILS : IDmenuAcceptILS,
      NULL,
      pSizer);

    m_pFile->GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_CHANNEL);
    sStatus = m_pFile->CheckChannelStatus(
      m_pSample,nReview,nAccept,NULL,&bReview);
    _AddLink(
      sStatus,
      bReview ? IDmenuReviewChannels : IDmenuAcceptChannels,
      NULL,
      pSizer);

    m_pFile->GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_LOCUS);
    for(i = 0; i < nLocusCount; i++)
    {
      sLocusName = m_pFile->GetLocusName(i);
      pLocus = m_pSample->FindLocus(sLocusName);
      if(pLocus != NULL)
      {
        sStatus = m_pFile->CheckLocusStatus(
          pLocus,nReview,nAccept,NULL,&bReview);
        _AddLink(
          sStatus,
          bReview ? IDmenuReviewLocus : IDmenuAcceptLocus,
          pLocus,
          pSizer);
      }
    }
  }
  else
  {
    bDisabled = true;
    pSizer->Add(
      new wxStaticText(this,wxID_ANY,"This sample is disabled"),
      0,wxALIGN_LEFT,0);
  }
  //if(bDisabled || (!m_vpStatus.empty()))
  {
    wxString sRunStart = m_pSample->FormatRunStart();
    int nRunStart = sRunStart.IsEmpty() ? 0 : 1;
    wxStaticText *pLabelSampleName =
      new wxStaticText(this,wxID_ANY,"Sample:");
    wxStaticText *pSampleName =
      new wxStaticText(this,wxID_ANY,m_pSample->GetSampleName());
    wxStaticText *pLabelFileName =
      new wxStaticText(this,wxID_ANY,"File:");
    wxStaticText *pFileName =
      new wxStaticText(this,wxID_ANY,m_pSample->GetName());
    wxStaticText *pLabelRunStart = NULL;
    wxStaticText *pRunStart = NULL;

    wxFont fn = pLabelSampleName->GetFont();
    fn.SetPointSize(fn.GetPointSize() + 1);
    wxFont fnLabel(fn);

    fnLabel.SetWeight(wxFONTWEIGHT_BOLD);
    pLabelSampleName->SetFont(fnLabel);
    pLabelFileName->SetFont(fnLabel);
    pSampleName->SetFont(fn);
    pFileName->SetFont(fn);

    wxFlexGridSizer *pSizerGrid = new wxFlexGridSizer(2,0,6);
    pSizerGrid->Add(pLabelSampleName);
    pSizerGrid->Add(pSampleName);
    pSizerGrid->Add(pLabelFileName);
    pSizerGrid->Add(pFileName);

    if(nRunStart)
    {
      pLabelRunStart =
        new wxStaticText(this,wxID_ANY,"Run Start:");
      pRunStart =
        new wxStaticText(this,wxID_ANY,sRunStart);
      pLabelRunStart->SetFont(fnLabel);
      pRunStart->SetFont(fn);
      pSizerGrid->Add(pLabelRunStart);
      pSizerGrid->Add(pRunStart);
    }
    // creating pSizerTop because pSizerGrid
    // seems to be wider that requested
    wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
    pSizerAll->Add(pSizerGrid,0, (wxALL ^ wxBOTTOM) | wxALIGN_CENTRE,ID_BORDER);
    pSizerAll->Add(pSizer,0,wxALL | wxALIGN_CENTRE,ID_BORDER);
    SetSizer(pSizerAll);
    SetScrollRate(8,8);
    Layout();
  }
//  else
//  {
//    delete pSizer;
//  }
}
#if 0
void CPanelStatus::OnClick(wxHyperlinkEvent &e)
{
  CStatusInfo *pStatus = m_vpStatus.at(e.GetId() - IDhyperlinkStatus);
  int nID = pStatus->GetID();
  if(m_pParent->CheckIfHistoryOK())
  {
    switch(nID)
    {
      case IDmenuAcceptLocus:
        m_pParent->DoAcceptLocus(m_pSample,pStatus->GetLocus());
        break;
      case IDmenuReviewLocus:
        m_pParent->DoReviewLocus(m_pSample,pStatus->GetLocus());
        break;
      case IDmenuAcceptSample:
      case IDmenuAcceptILS:
      case IDmenuAcceptChannels:
      case IDmenuAcceptDirectory:
        m_pParent->DoAcceptSample(nID,m_pSample);
        break;
      case IDmenuReviewSample:
      case IDmenuReviewILS:
      case IDmenuReviewChannels:
      case IDmenuReviewDirectory:
        m_pParent->DoReviewSample(nID,m_pSample);
        break;
      default:
        wxASSERT_MSG(0,"CPanelStatus::OnClick() invalid ID");
    }
  }
}
#else
void CPanelStatus::OnClick(wxHyperlinkEvent &e)
{
  CStatusInfo *pStatus = m_vpStatus.at(e.GetId() - IDhyperlinkStatus);
  int nID = pStatus->GetID();
  if(m_pParent->CheckIfHistoryOK())
  {
    wxCommandEvent ee(wxEVT_MENU,nID);
    switch(nID)
    {
      case IDmenuAcceptLocus:
      case IDmenuReviewLocus:
        ee.SetClientData((void *)pStatus->GetLocus());
      case IDmenuAcceptSample:
      case IDmenuAcceptILS:
      case IDmenuAcceptChannels:
      case IDmenuAcceptDirectory:
      case IDmenuReviewSample:
      case IDmenuReviewILS:
      case IDmenuReviewChannels:
      case IDmenuReviewDirectory:
        GetEventHandler()->AddPendingEvent(ee);
        break;
      default:
        wxASSERT_MSG(0,"CPanelStatus::OnClick() invalid ID");
    }
  }
}

#endif
BEGIN_EVENT_TABLE(CPanelStatus,wxScrolledWindow)
EVT_HYPERLINK(wxID_ANY,CPanelStatus::OnClick)
END_EVENT_TABLE()
