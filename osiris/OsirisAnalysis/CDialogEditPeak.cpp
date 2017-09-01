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
*  FileName: CDialogEditPeak.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CDialogEditPeak.h"
#include "COARsample.h"
#include "COARlocus.h"
#include "COARpeak.h"
#include "COARfile.h"
#include "CParmOsiris.h"
#include "mainApp.h"
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "wxIDS.h"
#include "CArtifactLabels.h"
#include "CFrameAnalysis.h"
#include "COARreview.h"

//  set up combobox sorting if supported
#ifdef __WXOSX_COCOA__
#define CBSORT 0
#else
#define CBSORT  wxCB_SORT
#endif

//const int CDialogEditPeak::ID_ACCEPT = CDialogEditPeak::m_ID_ACCEPT;
const int CDialogEditPeak::ID_EDIT = CDialogEditPeak::m_ID_EDIT;

CDialogEditPeak::CDialogEditPeak(
  wxWindow *parent, 
  const COARsample *pSample, 
  const COARpeakAny *pPeak,
  bool bAllowUserOverride) :
    wxDialog(parent,wxID_ANY,wxEmptyString,
      wxDefaultPosition, wxDefaultSize,
      mainApp::DIALOG_STYLE),
    m_pSample(pSample),
    m_pPeak(pPeak),
    m_pOriginal(NULL),
    m_pCurrent(NULL),
    m_nCurrentAllele(100000),
    m_pComboLabel(NULL),
    m_pLabelLabel(NULL),
    m_pCheckEnabled(NULL),
    m_pCheckCritical(NULL),
    m_pCheckDontShowAgain(NULL),
    //m_pButtonAccept(NULL),
    m_pButtonEditLocus(NULL),

    m_bCanAcceptHere(false),
    m_bLocusNeedsAcceptance(false),
    m_bOK(true),
    m_bShowUser(false)

{
  _SetupUser(bAllowUserOverride);
  if(m_pPeak == NULL)
  {
    _BuildNoEdit();
  }
  else if(m_pPeak->IsEditable() && _SetupAlleles())
  {
    _BuildAllele();
    CentreOnParent();
  }
  else
  {
//    _BuildNoEdit();
    _BuildArtifactLabel();
  }
}
CDialogEditPeak::~CDialogEditPeak()
{
  _Cleanup();
}
void CDialogEditPeak::_Cleanup()
{
#define DELETE_PTR(x) if(x != NULL) { delete x; x = NULL; }
  if(m_pOriginal != NULL)
  {
    delete m_pOriginal;
    m_pOriginal = NULL;
  }
  if(m_pCurrent != NULL)
  {
    delete m_pCurrent;
    m_pCurrent = NULL;
  }
  vectorptr<COARpeakAny>::cleanup(&m_vpAlleles);
}
void CDialogEditPeak::_BuildComboLabel()
{
  const wxArrayString &as = mainApp::GetArtifactLabels()->GetChoiceList();
  m_pLabelLabel = new wxStaticText(this,wxID_ANY,wxT("Label "));
  m_pComboLabel = new wxComboBox(
    this,wxID_ANY,wxEmptyString, 
    wxDefaultPosition, wxDefaultSize,
    as,wxCB_DROPDOWN | CBSORT);
}
size_t CDialogEditPeak::_SetupAlleles()
{
  if( (m_pPeak != NULL) && (m_pSample != NULL) && m_vpAlleles.empty() )
  {
    int nID = m_pPeak->GetID();
    m_pSample->GetAllelesByID(nID,true,&m_vpAlleles);
  }
  size_t nRtn = m_vpAlleles.size();
  if(!nRtn) { m_bOK = false; }
  return nRtn;
}
void CDialogEditPeak::_BuildAllele()
{
  wxString sAlleleLabel;
  int nID = m_pPeak->GetID();
  int windowID;
  COARpeakAny *pPeak;
  _SetupAlleles();

  wxRadioButton *pRadio;
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerLR = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerLeft = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerRight = new wxBoxSizer(wxVERTICAL);

  //  set up alleles

  wxStaticText *pLabel(new wxStaticText(this,wxID_ANY,wxT("Allele")));
  wxFont font = pLabel->GetFont();
  font.MakeBold();
  font.MakeLarger();
  pLabel->SetFont(font);
  pSizerLeft->Add(pLabel,0,wxALL,ID_BORDER);

  size_t nAllele = m_vpAlleles.size();
  if(nAllele < 1 || nAllele > 2)
  {
    wxString sProblem = nwxString::FormatNumber((int)nAllele);
    sProblem += wxT(" alleles found for ID ");
    sProblem += nwxString::FormatNumber(nID);
    wxASSERT_MSG(0,sProblem);
    if(nAllele > 2) { nAllele = 2; }
  }
  size_t i;
  int nStyle = wxRB_GROUP;
  windowID = m_ID_ALLELE1;
  for (i = 0; i < nAllele; ++i)
  {
    pPeak = m_vpAlleles.at(i);
    sAlleleLabel.Clear();
    if(pPeak->IsOffLadder() || pPeak->IsOffLadderAccepted())
    {
      sAlleleLabel += wxT("OL ");
    }
    const wxString &sLocus = pPeak->GetLocusName();
    sAlleleLabel += 
      COARpeak::FormatAlleleName(*pPeak,COARlocus::IsAmel(sLocus),false);
    sAlleleLabel += wxT(" - ");
    sAlleleLabel += sLocus;
    pRadio = new wxRadioButton(
      this,windowID,sAlleleLabel,
      wxDefaultPosition,wxDefaultSize,
      nStyle);
    nStyle = 0;
    pSizerLeft->Add(pRadio,0,wxALL ^ wxTOP, ID_BORDER);
    m_vpRadioAllele.push_back(pRadio);
    windowID++;
  }
  // disabled allele
  pRadio = new wxRadioButton(this,m_ID_ALLELE_DISABLED,wxT("Disabled"));
  pSizerLeft->Add(pRadio,0,wxALL ^ wxTOP, ID_BORDER);
  m_vpRadioAllele.push_back(pRadio);

  // STOP HERE OFF LADDER WIDGET
  //  Off Ladder Radio buttons
  m_pCheckBoxLadderAccept = new wxCheckBox(this,m_ID_OFF_LADDER_ACCEPTED, wxT("Accept Off Ladder"));
  pSizerLeft->Add(m_pCheckBoxLadderAccept,0,wxALL,ID_BORDER);

  // end set up alleles
  // set up artifacts

  pLabel = new wxStaticText(this,wxID_ANY,wxT("Artifact"));
  font = pLabel->GetFont();
  font.MakeBold();
  font.MakeLarger();
  pLabel->SetFont(font);
  pSizerRight->Add(pLabel,0,wxALL,ID_BORDER);

  m_pCheckEnabled = new wxCheckBox(this,m_ID_ARTIFACT_ENABLED,wxT("Enabled"));
  pSizerRight->Add(m_pCheckEnabled,0,wxALL ^ wxTOP,ID_BORDER);
  wxBoxSizer *pSizerLabel = new wxBoxSizer(wxHORIZONTAL);
  _BuildComboLabel();

  pSizerLabel->Add(m_pLabelLabel,0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pSizerLabel->Add(m_pComboLabel,0,wxALIGN_CENTRE_VERTICAL,0);
  pSizerRight->Add(pSizerLabel,0,wxALL ^ wxTOP,ID_BORDER);
  m_pCheckCritical = new wxCheckBox(this,m_ID_ARTIFACT_CRITICAL,wxT("Critical"));
  pSizerRight->Add(m_pCheckCritical,0,wxALL ^ wxTOP,ID_BORDER);
  pSizerLR->AddStretchSpacer(1);
  pSizerLR->Add(pSizerLeft,0,wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL | wxRIGHT, ID_BORDER);
  pSizerLR->AddStretchSpacer(1);
  pSizerLR->Add(pSizerRight,0,wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL | wxLEFT, ID_BORDER);
  pSizerLR->AddStretchSpacer(1);
  wxSizer *pSizerButtons = new wxBoxSizer(wxHORIZONTAL);
//  m_pButtonAccept = new wxButton(this,m_ID_ACCEPT,wxT("&Accept Locus"));
//  pSizerButtons->Add(m_pButtonAccept,0,wxLEFT | wxRIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  m_pButtonEditLocus = new wxButton(this,m_ID_EDIT,wxT("&Edit Locus"));
  pSizerButtons->Add(m_pButtonEditLocus,0,wxLEFT | wxRIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pSizerButtons->AddStretchSpacer(1);
  wxButton *pOK = new wxButton(this,wxID_OK,wxT("OK"));
  pSizerButtons->Add(pOK,
    0,wxLEFT | wxRIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pSizerButtons->Add(new wxButton(this,wxID_CANCEL,wxT("Cancel")),
    0,wxLEFT | wxRIGHT | wxALIGN_CENTRE_VERTICAL,ID_BORDER);
  pOK->SetDefault();

  pSizerAll->Add(pSizerLR,1,
    wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL);
  if(m_bShowUser)
  {
    wxString sUser(wxT("User: "));
    sUser.Append(m_sUser);
    wxStaticText *pUser = new wxStaticText(this,wxID_ANY,sUser);
    pSizerAll->Add(pUser,0,
      wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTER_VERTICAL | (wxALL ^ wxBOTTOM),
      ID_BORDER);
  }
  pSizerAll->Add(pSizerButtons,0,
    wxALIGN_CENTRE_HORIZONTAL | wxALIGN_CENTRE_VERTICAL | wxALL | wxEXPAND
    ,ID_BORDER);
  SetSizer(pSizerAll);
  SetTitle(wxT("Edit Peak"));
  Fit();
}
bool CDialogEditPeak::TransferDataToWindow()
{
  bool bRtn = IsOK();
  if(bRtn)
  {
    //  select allele
    size_t nAllele = m_vpAlleles.size();
    size_t i;
    COARpeakAny *pPeak;
    wxRadioButton *pRadio = NULL;
    if(m_pOriginal != NULL)
    {
      delete m_pOriginal;
      m_pOriginal = NULL;
      wxASSERT_MSG(0,wxT("CDialogEditPeak::TransferDataToWindow, m_pOriginal != NULL"));
    }
    if(m_pCurrent != NULL)
    {
      delete m_pCurrent;
      m_pCurrent = NULL;
      wxASSERT_MSG(0,wxT("CDialogEditPeak::TransferDataToWindow, m_pCurrent != NULL"));
    }
    for(i = 0; i < nAllele; ++i)
    {
      pPeak = m_vpAlleles.at(i);
      if(pPeak->IsAllele())
      {
        m_pOriginal = new COARpeakAny(*pPeak);
        pRadio = m_vpRadioAllele.at(i);
        m_nCurrentAllele = i;
        i = nAllele;
      }
    }
    if(pRadio == NULL)
    {
      m_nCurrentAllele = m_vpRadioAllele.size();
      if(m_nCurrentAllele) 
      {
        --m_nCurrentAllele; 
        pRadio = m_vpRadioAllele.at(m_nCurrentAllele);
      } // should always happen
      COARartifact *pArt = m_pSample->GetArtifactByID(m_pPeak->GetID());
      if(pArt != NULL)
      {
        m_pOriginal = new COARpeakAny(*pArt);
        delete pArt;
      }
      else
      {
        m_pOriginal = new COARpeakAny(*(*m_vpAlleles.begin()));
        wxASSERT_MSG(0,"CDialogEditPeak::TransferDataToWindow, could not find allele nor artifact");
      }
      m_pOriginal->SetIsAllele(false);
    }
    m_pCurrent = new COARpeakAny(*m_pOriginal);
    pRadio->SetValue(true);
    _TransferCurrentToOffLadder();

    m_sDefaultArtifactLabel = 
        mainApp::GetArtifactLabels()->GetDisplayFromString
            (m_pCurrent->GetArtifactLabel());
    m_sOriginalLabel = m_pCurrent->GetArtifactUserDisplay();
    const wxString &sLabel =
      (m_sOriginalLabel.IsEmpty() ? m_sDefaultArtifactLabel : m_sOriginalLabel);
    if(!m_pComboLabel->SetStringSelection(sLabel))
    {
      m_pComboLabel->Append(sLabel);
      m_pComboLabel->SetStringSelection(sLabel);
    }
    m_pCheckEnabled->SetValue(m_pCurrent->IsArtifact());
    m_pCheckCritical->SetValue(m_pCurrent->IsCritical());
    _SetupWidgets();
  }
  return bRtn;
}

void CDialogEditPeak::_TransferArtifactToCurrent()
{
  bool bEnabled = m_pCheckEnabled->GetValue();
  m_pCurrent->SetIsArtifact(bEnabled);
  if(bEnabled)
  {
    m_pCurrent->SetIsCritical(m_pCheckCritical->GetValue());
    wxString sLabel = m_pComboLabel->GetValue();
    if((sLabel == m_sDefaultArtifactLabel) && (sLabel != m_sOriginalLabel))
    {
      sLabel.Clear();
    }
    m_pCurrent->SetArtifactUserDisplay(sLabel);
  }
}


void CDialogEditPeak::_SetupCanAccept()
{
  const COARlocus *pLocus(NULL);
  if( COARpeak::Equal(*m_pOriginal,*m_pCurrent) &&
    (m_pCurrent->IsAllele() || (m_vpAlleles.size() == 1)) &&
    ((pLocus = m_pSample->FindLocus(m_pCurrent->GetLocusName())) != NULL) )
  {
    const COARfile *pFile = m_pSample->GetFile();
    int nA,nR;
    pFile->GetReviewerCounts(
      &nR, &nA,
      CLabReview::REVIEW_LOCUS);
    m_bLocusNeedsAcceptance = pLocus->NeedAcceptance(nA);
#if 0
    m_bCanAcceptHere =
      m_bLocusNeedsAcceptance &&
      m_pCurrent->IsArtifact() &&
      (pLocus->AlertCount() == 1);
#endif
  }
  else
  {
    m_bLocusNeedsAcceptance = false;
    m_bCanAcceptHere = false;
  }
  //m_pButtonAccept->Enable(m_bLocusNeedsAcceptance);
}
void CDialogEditPeak::_BuildNoEdit()
{
  m_bOK = false;
}
void CDialogEditPeak::_BuildArtifactLabel()
{
  m_bOK = false;
}
void CDialogEditPeak::OnWidget(wxCommandEvent &e)
{
  switch(e.GetId())
  {
    case m_ID_ALLELE1:
    case m_ID_ALLELE2:
    case m_ID_ALLELE_DISABLED:
      _UpdateCurrent();
      break;
    case m_ID_OFF_LADDER_ACCEPTED:
      _TransferOffLadderToCurrent();
      break;
  }
  _SetupWidgets();
}
#if 0
void CDialogEditPeak::OnAccept(wxCommandEvent &)
{
  EndModal(ID_ACCEPT);
}
#endif
void CDialogEditPeak::OnEdit(wxCommandEvent &)
{
  EndModal(ID_EDIT);
}
void CDialogEditPeak::_SetupWidgets()
{
  size_t nSize = m_vpRadioAllele.size();
  bool bEnableEditLocus = false;
  bool bIsAllele = !m_vpRadioAllele.at(nSize - 1)->GetValue();
  bEnableEditLocus = ( bIsAllele || ( nSize < 3) );
    // if locus is disabled and there is more than one
    // locus, then editing locus is ambiguous
  m_pButtonEditLocus->Enable(bEnableEditLocus);

  bool bEnable = m_pCheckEnabled->GetValue();
  m_pCheckCritical->Enable(bEnable);
  m_pComboLabel->Enable(bEnable);

  _TransferArtifactToCurrent();
  _SetupCanAccept();
}
const wxString &CDialogEditPeak::_GetOffLadderValue()
{
  const wxString *pRtn = &IOARpeak::OL_NO;
  if(m_pCheckBoxLadderAccept->IsEnabled())
  {
    pRtn = m_pCheckBoxLadderAccept->GetValue()
      ? &IOARpeak::OL_ACCEPTED
      : &IOARpeak::OL_TRUE;
  }
  return *pRtn;
}
void CDialogEditPeak::_TransferOffLadderToCurrent()
{
  // when the off ladder selected changes, this
  // function saved it in the current allele
  const wxString &s = _GetOffLadderValue();
  if(!s.IsEmpty()) { m_pCurrent->SetOffLadderString(s); }
    
}
size_t CDialogEditPeak::_GetSelectedAllele()
{
  size_t nSize = m_vpRadioAllele.size();
  size_t nRtn = 0;
  if(nSize)
  {
    nRtn = nSize - 1;
    for(size_t i = 0; i < nSize; ++i)
    {
      if(m_vpRadioAllele.at(i)->GetValue())
      {
        nRtn = i;
        break;
      }
    }
  }
  return nRtn;
}

void CDialogEditPeak::_UpdateCurrent()
{
  COARpeakAny *p;
  if(m_nCurrentAllele < m_vpAlleles.size())
  {
    p = m_vpAlleles.at(m_nCurrentAllele);
    m_pCurrent->SetIsAllele(false);
    *p = *m_pCurrent;
  }
  size_t nAllele = _GetSelectedAllele();
  bool bAllele = false;
  if(nAllele < m_vpAlleles.size())
  {
    p = m_vpAlleles.at(nAllele);
    bAllele = true;
  }
  else
  {
    p = m_pOriginal;
  }
  *m_pCurrent = *p;
  m_pCurrent->SetIsAllele(bAllele);
  _TransferCurrentToOffLadder();
}
void CDialogEditPeak::_TransferCurrentToOffLadder()
{
  bool bAccepted = false;
  bool bEnable = false;
  if(m_pCurrent->IsAllele())
  {
    bAccepted = m_pCurrent->IsOffLadderAccepted();
    bEnable = bAccepted || m_pCurrent->IsOffLadder();
  }
  m_pCheckBoxLadderAccept->Enable(bEnable);
  m_pCheckBoxLadderAccept->SetValue(bAccepted);
}
const wxString CDialogEditPeak::GetLocusName()
{
  wxString sRtn;
  if(m_pCurrent->IsAllele())
  {
    sRtn = m_pCurrent->GetLocusName();
  }
  else if(m_vpAlleles.size() == 2)
  {
    sRtn = (*m_vpAlleles.begin())->GetLocusName();
  }
  return sRtn;
}
#define SETUP_SAMPLE_LOCUS_NAME \
COARsample *pSample = \
  pFile->GetSampleByName(m_pSample->GetName()); \
const wxString &sLocusName(m_pCurrent->GetLocusName())

#define SETUP_SAMPLE_LOCUS \
COARsample *pSample = \
  pFile->GetSampleByName(m_pSample->GetName()); \
COARlocus *pLocus = \
  pSample->FindLocus(m_pCurrent->GetLocusName())

void CDialogEditPeak::_SetupUser(bool bAllowOverride)
{
  wxString sUserID = wxGetUserId();
  m_sUser.Empty();
  if(bAllowOverride)
  {
    CParmOsirisGlobal parm;
    m_sUser = parm->GetCMFuserID();
  }
  if(m_sUser.IsEmpty())
  {
    m_sUser = sUserID;
  }
  m_bShowUser = (m_sUser != sUserID);
}
bool CDialogEditPeak::_Do_UpdatePeak(
    COARfile *pFile, CFrameAnalysis *pFrame)
{
  bool bRtn = true;
  if(!COARpeak::Equal(*m_pCurrent,*m_pOriginal))
  {
    wxString sErr;
    wxString sComma;
    SETUP_SAMPLE_LOCUS;
    std::vector<COARpeakAny *> vp;
    vp.push_back(m_pCurrent);
    CAppendReviewLocus x(pLocus);
    x.AppendReview(m_sUser); // will return false if user already edited
    pLocus->AppendNotes(wxT("Edit Peak from plot"),m_sUser);
    if(!pSample->SetPeaksByLocusName(
      pLocus->GetName(),
      pLocus->GetChannelNr(),
      &vp))
    {
      sErr += sComma;
      sErr += wxT("CDialogEditPeak::UpdateFile - cannot transfer peak data");
      sComma = wxT(",");
    }
    if(!sErr.IsEmpty())
    {
      wxASSERT_MSG(0,sErr);
      mainApp::LogMessage(sErr);
      bRtn = false;
    }
    pFile->SetIsModified(true);
    pFrame->RepaintAllData(pSample);
  }
  return bRtn;
}
#undef SETUP_SAMPLE_LOCUS

bool CDialogEditPeak::UpdateFile(
  COARfile *pFile,
  CFrameAnalysis *pFrame)
{
  int nRtnCode = GetReturnCode();
  bool bRtn = true;
  // need pointers that are not constants
//  if(nRtnCode == ID_ACCEPT)
//  {
//    bRtn = _Do_Accept(pFile,pFrame,parent);
//  }
//  else 
  if(nRtnCode == ID_EDIT)
  {
    SETUP_SAMPLE_LOCUS_NAME;
    pFrame->ShowSampleFrame(pSample,sLocusName,-1);
    bRtn = true;
  }
  else if(nRtnCode == wxID_OK)
  {
    bRtn = _Do_UpdatePeak(pFile,pFrame);
  }
  else 
  {
    bRtn = false;
    if(nRtnCode != wxID_CANCEL)
    {
      wxString sErr = wxT("Unknown return code from CDialogEditPeak::EndModal() ");
      sErr += nwxString::FormatNumber(nRtnCode);
      wxASSERT_MSG(0,sErr);
      mainApp::LogMessage(sErr);
    }
  }
  return bRtn;
}

BEGIN_EVENT_TABLE(CDialogEditPeak,wxDialog)
EVT_BUTTON(m_ID_EDIT, CDialogEditPeak::OnEdit)
EVT_CHECKBOX(wxID_ANY,CDialogEditPeak::OnWidget)
EVT_RADIOBUTTON(wxID_ANY,CDialogEditPeak::OnWidget)
EVT_COMBOBOX(wxID_ANY,CDialogEditPeak::OnWidget)
EVT_TEXT(wxID_ANY,CDialogEditPeak::OnWidget)
END_EVENT_TABLE();
