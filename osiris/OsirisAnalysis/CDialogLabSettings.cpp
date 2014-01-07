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

*  FileName: CDialogLabSettings.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/arrstr.h>
#include "mainApp.h"
#include "CDialogLabSettings.h"
#include "nwx/nwxString.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxIntValidator.h"
#include "nwx/nwxDoubleValidator.h"
#include "nwx/nwxBookCtrlFocus.h"
#include "wxIDS.h"
#include "nwx/stdb.h"
#include <memory>
#include <set>
#include "nwx/stde.h"
#include "CParmOsiris.h"

const wxString CDialogLabSettings::g_sFileNameStrPrompt
  (_T("File name search criteria"));
const wxSize CDialogLabSettings::g_SIZE_NUMBER(wxSize(60,-1));
const wxSize CDialogLabSettings::g_SIZE_MULTILINE_TEXT(wxSize(300,200));
typedef map<wxString, size_t> mapSort;

//********************************************************************
//
//    nwxChoicebook
//
void nwxChoicebook::SetupButtons(const wxString &sItemName, bool bRename)
{
  wxSizer *pBookSizer = GetControlSizer();

  m_pButtonAdd = new wxButton(
    this,IDadd,_T("+"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  pBookSizer->Add(
    m_pButtonAdd,0,wxLEFT | wxALIGN_CENTER_VERTICAL,
    ID_BORDER);
  m_pButtonRemove = new wxButton(
    this,IDremove,_T("-"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  pBookSizer->Add(
    m_pButtonRemove,0,wxLEFT | wxALIGN_CENTER_VERTICAL,
    ID_BORDER);
  if(bRename)
  {
    m_pButtonRename = new wxButton(
      this,IDrename,_T("~"),
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    pBookSizer->Add(
      m_pButtonRename,0,wxLEFT | wxALIGN_CENTER_VERTICAL,
      ID_BORDER);
  }

  wxString sItem = sItemName;
  if(sItem.IsEmpty())
  {
    sItem = _T("item");
  }
  wxString sAdd(_T("Add a new "));
  wxString sRemove(_T("Remove selected "));
  sAdd += sItem;
  sRemove += sItem;
  m_pButtonAdd->SetToolTip(sAdd);
  m_pButtonRemove->SetToolTip(sRemove);
  m_pButtonAdd->Enable(!m_bReadOnly);
  m_pButtonRemove->Enable(!m_bReadOnly);
  if(bRename)
  {
    wxString sRename(_T("Rename selected "));
    sRename += sItem;
    m_pButtonRename->SetToolTip(sRename);
    m_pButtonRename->Enable(!m_bReadOnly);
  }
};
bool nwxChoicebook::TransferDataFromWindow()
{
  size_t n = GetPageCount();
  bool bRtn = true;
  for(size_t i = 0; i < n; i++)
  {
    if(!GetPage(i)->TransferDataFromWindow())
    {
      bRtn = false;
    }
  }
  return bRtn;
}
bool nwxChoicebook::TransferDataToWindow()
{
  size_t n = GetPageCount();
  bool bRtn = true;
  for(size_t i = 0; i < n; i++)
  {
    if(!GetPage(i)->TransferDataToWindow())
    {
      bRtn = false;
    }
  }
  return bRtn;
}

size_t nwxChoicebook::FindInsertPosition(const wxString &s)
{
  wxString sNameLower(s);
  wxString sText;
  size_t nRtn = GetPageCount();
  sNameLower.MakeLower();
  sNameLower.Trim(true);
  sNameLower.Trim(false);
  for(size_t i = 0; i < nRtn; i++)
  {
    sText = GetPageText(i);
    sText.MakeLower();
    sText.Trim(true);
    sText.Trim(false);
    if(sNameLower.Cmp(sText) < 0)
    {
      nRtn = i; // loop exit and return position
    }
  }
  return nRtn;
}

bool nwxChoicebook::IsNameUsed(const wxString &sName)
{
  size_t nCount = GetPageCount();
  wxString s(sName);
  wxString sUsed;
  bool bRtn = false;
  nwxString::TrimLower(&s);
  for(size_t i = 0; i < nCount; ++i)
  {
    sUsed = GetPageText(i);
    nwxString::TrimLower(&sUsed);
    if(!sUsed.Cmp(s))
    {
      bRtn = true;
      i = nCount; // loop exit
    }
  }
  return bRtn;
}

bool nwxChoicebook::RenamePage(const wxString &sNewName, int nSelection)
{
  int nCurrent = GetSelection();
  bool bSelect = true;
  bool bRtn = false;
  if(nSelection < 0)
  {
    nSelection = nCurrent;
  }
  else
  {
    bSelect = (nSelection == nCurrent);
  }
  if(nSelection >= 0)
  {
    size_t n = nSelection;
    wxNotebookPage *pCurrent = GetPage(n);
    int nImage = GetPageImage(n);
    if( (pCurrent != NULL) && RemovePage(n) )
    {
      size_t nNew = FindInsertPosition(sNewName);
      bRtn = InsertPage(nNew,pCurrent,sNewName,bSelect,nImage);
      if(!bRtn)
      {
        // this should never happen and otherwise
        // this will hopefully work
        InsertPage(0,pCurrent,sNewName,bSelect,nImage);
      }
    }
  }
  return bRtn;
}

//********************************************************************
//
//    nwxTextPanel
//

class nwxTextPanel : public nwxPanel
{
public:
  nwxTextPanel(wxWindow *parent, wxWindowID id,CLabSynonym &vs) :
      nwxPanel(parent,id)
  {
    wxString sValue;
    nwxString::Join(vs.GetVector(),&sValue,'\n');
    _Build(sValue);
  }
  nwxTextPanel(wxWindow *parent, wxWindowID id,const vector<wxString> &vs) :
      nwxPanel(parent,id)
  {
    wxString sValue;
    nwxString::Join(vs,&sValue,'\n');
    _Build(sValue);
  }
  nwxTextPanel(wxWindow *parent,wxWindowID id) :
    nwxPanel(parent,id)
  {
    wxString sValue;
    _Build(sValue);
  }

  virtual void SetReadOnly(bool b = true)
  {
    m_pTextCtrl->SetEditable(!b);
  }
  wxTextCtrl *GetTextCtrl()
  {
    return m_pTextCtrl;
  }
private:
  void _Build(const wxString &sValue)
  {
    m_pTextCtrl =
      new wxTextCtrl(this,wxID_ANY,sValue,
        wxDefaultPosition, CDialogLabSettings::g_SIZE_MULTILINE_TEXT,
        wxTE_MULTILINE | wxTE_DONTWRAP | wxHSCROLL);
    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    pSizer->Add(m_pTextCtrl,1,wxEXPAND);
    SetSizer(pSizer);
    pSizer->Layout();
    pSizer->Fit(this);
  }
  wxTextCtrl *m_pTextCtrl;
};

//********************************************************************
//
//    CPanelLabNameStrings
//

CPanelLabNameStrings::~CPanelLabNameStrings() {;}

CPanelLabNameStrings::CPanelLabNameStrings(
  wxWindow *parent, CLabNameStrings *pData)
  : nwxPanel(parent), m_pTextStdCtrlName(NULL), m_pData(pData)
{
  m_pBook = new nwxChoicebook(this);
//  nwxTextPanel *pPanel = new nwxTextPanel(m_pBook,wxID_ANY,pData->m_vsSynonymLadder);
  nwxTextPanel *pPanel = new nwxTextPanel(m_pBook,wxID_ANY);
  m_pTextLadder = pPanel->GetTextCtrl();
  m_pBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_LADDER),true);

  // get count immediately before adding positive control

  m_nPosCtrlNdx = (int)m_pBook->GetPageCount();
//  pPanel = new nwxTextPanel(m_pBook,wxID_ANY,pData->m_vsSynonymPosCtrl);
  pPanel = new nwxTextPanel(m_pBook,wxID_ANY);
  m_pTextPos = pPanel->GetTextCtrl();
  m_pBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_POS_CONTROL));

//  pPanel = new nwxTextPanel(m_pBook,wxID_ANY,pData->m_vsSynonymNegCtrl);
  pPanel = new nwxTextPanel(m_pBook,wxID_ANY);
  m_pTextNeg = pPanel->GetTextCtrl();
  m_pBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_NEG_CONTROL));

  //  2/11/09 added specimen types

  wxString sType;
  size_t nTYPES = CLabSpecimenCategory::TypeCount();
  size_t i;
  m_pvTextSpecimenTypes.reserve(nTYPES);
  for(i = 0; i < nTYPES; ++i)
  {
    sType = _T(CLabSpecimenCategory::TYPES[i]);
    pPanel = new nwxTextPanel(m_pBook, wxID_ANY);
    m_pvTextSpecimenTypes.push_back(pPanel->GetTextCtrl());
    m_pBook->AddPage(pPanel,sType);
  }

  //  2/11/09

  wxTextCtrl *pTextStdCtrlName = new wxTextCtrl(this,wxID_ANY,m_pData->m_sStdCtrlName);
  wxStaticText *pText = new wxStaticText(this,wxID_ANY,
    CDialogLabSettings::g_sFileNameStrPrompt);
  m_pLabelStdCtrlName = new wxStaticText(this,wxID_ANY,_T("Standard Control Name"));
  m_pSizer = new wxBoxSizer(wxVERTICAL);
  m_pSizer->Add(pText,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  m_pSizer->Add(m_pBook,1,wxEXPAND | (wxALL ^ wxTOP),ID_BORDER);

  m_pSizer->Add(m_pLabelStdCtrlName,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  m_pSizer->Add(pTextStdCtrlName,0,wxEXPAND | (wxALL ^ wxTOP),ID_BORDER);
  SetSizer(m_pSizer);

  m_pTextStdCtrlName = pTextStdCtrlName;
      // when m_pTextStdCtrlName Select events are ignored
  OnChangeCombo(*( (wxChoicebookEvent *) NULL ));
  Layout();
  Fit();
}

bool CPanelLabNameStrings::TransferDataFromWindow()
{
  wxString s;
  vector<wxString> vs;
  wxTextCtrl *ptext[3] = {m_pTextLadder, m_pTextPos, m_pTextNeg};
  CLabSynonym *pSyn[3] =
  {
    &(m_pData->m_vsSynonymLadder),
    &(m_pData->m_vsSynonymPosCtrl),
    &(m_pData->m_vsSynonymNegCtrl)
  };
  size_t i;
  bool bRtn = true;
  for(i = 0; i < 3; i++)
  {
    s = ptext[i]->GetValue();
    vector<wxString> *pvs(pSyn[i]->GetVectorPtr());
    if(!nwxString::SplitLines(s.c_str(),pvs,true,true))
    {
      //m_pBook->SetSelection(i);
      mainApp::ShowError(
        _T("Please enter at least one identifier"),this);
      nwxBookCtrlFocus::Focus(ptext[i]);
      i = 3; // loop exit
      bRtn = false;
    }
  }
  m_pData->m_sStdCtrlName = m_pTextStdCtrlName->GetValue();

  // Specimen Category

  size_t nTYPES = CLabSpecimenCategory::TypeCount();
  CLabSetSpecimenCategory &setSpec(m_pData->m_setSpecimenCategory);
  for(i = 0; i < nTYPES; i++)
  {
    s = m_pvTextSpecimenTypes.at(i)->GetValue();
    setSpec.SetSynonyms(i,s);
  }

  return bRtn;
}
bool CPanelLabNameStrings::TransferDataToWindow()
{
  wxString s;
  wxTextCtrl *ptext[3] = {m_pTextLadder, m_pTextPos, m_pTextNeg};
  CLabSynonym *pSyn[3] =
  {
    &(m_pData->m_vsSynonymLadder),
    &(m_pData->m_vsSynonymPosCtrl),
    &(m_pData->m_vsSynonymNegCtrl)
  };
  size_t i;
  for(i = 0; i < 3; i++)
  {
    nwxString::Join(pSyn[i]->GetVector(),&s,"\n");
    nwxString::FixEOLU(&s,true);
    ptext[i]->SetValue(s);
  }

  // specimen categories 2/11/09
  const vector<wxString> *pvs;
  size_t nTYPES = CLabSpecimenCategory::TypeCount();
  CLabSetSpecimenCategory &setSpec(m_pData->m_setSpecimenCategory);
  for(i = 0; i < nTYPES; i++)
  {
    pvs = setSpec.GetSynonyms(i);
    if(pvs != NULL)
    {
      nwxString::Join(*pvs,&s,"\n");
      nwxString::FixEOLU(&s,true);
    }
    else
    {
      s.Empty();
    }
    m_pvTextSpecimenTypes.at(i)->SetValue(s);
  }

  return true;
}

void CPanelLabNameStrings::OnChangeCombo(wxChoicebookEvent &)
{
  if(m_pTextStdCtrlName != NULL)
  {
    int nSelect = m_pBook->GetSelection();
    bool bShow = (nSelect == m_nPosCtrlNdx);
    m_pSizer->Show(m_pTextStdCtrlName,bShow);
    m_pSizer->Show(m_pLabelStdCtrlName,bShow);
    m_pSizer->Layout();
  }
}
BEGIN_EVENT_TABLE(CPanelLabNameStrings, wxPanel)
EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,CPanelLabNameStrings::OnChangeCombo)
END_EVENT_TABLE()

//********************************************************************
//
//    CPanelRFU
//
CPanelRFU::~CPanelRFU() {;}

CPanelRFU::CPanelRFU(
    wxWindow *parent,
    wxWindowID id,
    CLabRFU *pData,
    bool *pbAllowOverride) :
  nwxPanel(parent,id),
  m_pTextMinRFUinterlocus(NULL),
  m_pTextPullupFractionFilter(NULL),
  m_pSizer(new wxGridBagSizer(ID_BORDER,ID_BORDER)),
  m_pData(pData),
  m_pbAllowOverride(pbAllowOverride)
{
  bool bSample = pData->UsingPullupFractionFilter();
  nwxUintValidator nval(&m_n,20,60000,0,true);
  nwxDoubleValidator dval(&m_d,0.0,1.0,-1.0,true);
  m_pTextMinRFU = CreateTextCtrl(nval);
  if(bSample)
  {
    m_pTextMinRFUinterlocus = CreateTextCtrl(nval);
  }
  m_pCheckAllowOverride = new wxCheckBox(this,wxID_ANY,_T("Allow User to Override"));
  m_pTextMaxRFU = CreateTextCtrl(nval);
  if(bSample)
  {
    m_pTextPullupFractionFilter = CreateTextCtrl(dval);
  }
  m_pTextFractionOfMaxPeak = CreateTextCtrl(dval);
  m_pTextStutterThreshold = CreateTextCtrl(dval);
  m_pTextAdenylationThreshold = CreateTextCtrl(dval);

  TransferDataToWindow();
  AddLine("Minimum RFU:",m_pTextMinRFU);
  if(m_pTextMinRFUinterlocus != NULL)
  {
    AddLine("Min. Interlocus RFU:",m_pTextMinRFUinterlocus);
  }

  // Add checkbox with a span of 2 cols
  {
    wxGBSpan gbSpan(1,2);
    int nRow = m_gbPos.GetRow();
    m_pSizer->Add(
      m_pCheckAllowOverride,m_gbPos, gbSpan,
      wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    m_gbPos.SetRow(++nRow);
  }

  AddLine("Maximum RFU:",m_pTextMaxRFU);
  AddLine("Fraction of max. peak:",m_pTextFractionOfMaxPeak);
  if(m_pTextPullupFractionFilter != NULL)
  {
    AddLine("Pullup fractional filter:",m_pTextPullupFractionFilter);
  }
  AddLine("Stutter threshold:",m_pTextStutterThreshold);
  AddLine("Adenylation threshold:",m_pTextAdenylationThreshold);

  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pSizer,1,wxALIGN_CENTER | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
}

void CPanelRFU::AddLine(const char *ps, wxWindow *pw)
{
  wxStaticText *pLabel =
    new wxStaticText(this,wxID_ANY,_T(ps));
  int nRow = m_gbPos.GetRow();

  m_pSizer->Add(
    pLabel,m_gbPos, wxDefaultSpan,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  m_gbPos.SetCol(1);
  m_pSizer->Add(
    pw,m_gbPos,wxDefaultSpan,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  m_gbPos.SetRow(++nRow);
  m_gbPos.SetCol(0);
}

bool CPanelRFU::TransferDataFromWindow()
{
  wxString s;
  double d;
  int n;
  int nSampleRFU(0);
  bool bRtn = true; // Validate();
  if(bRtn)
  {
    s = m_pTextMinRFU->GetValue();
    nSampleRFU = CDialogLabSettings::Str2Int(s);

    if(m_pTextMinRFUinterlocus != NULL)
    {
      s = m_pTextMinRFUinterlocus->GetValue();
      n = CDialogLabSettings::Str2Int(s);
      if(n > 0 && n < nSampleRFU)
      {
        mainApp::ShowError(CParmOsiris::ERROR_MSG_INTERLOCUS,this);
        nwxBookCtrlFocus::Focus(m_pTextMinRFUinterlocus);
        bRtn = false;
      }
      else
      {
        m_pData->SetMinRFUinterlocus(n);
      }
    }
  }
  if(bRtn)
  {
    m_pData->SetMinRFU(nSampleRFU);

    s = m_pTextMaxRFU->GetValue();
    n = CDialogLabSettings::Str2Int(s);
    m_pData->SetMaxRFU(n);

    s = m_pTextFractionOfMaxPeak->GetValue();
    d = CDialogLabSettings::Str2Double(s);
    m_pData->SetFractionMaxPeak(d);

    if(m_pTextPullupFractionFilter != NULL)
    {
      s = m_pTextPullupFractionFilter->GetValue();
      d = CDialogLabSettings::Str2Double(s);
      m_pData->SetPullupFractionFilter(d);
    }

    s = m_pTextStutterThreshold->GetValue();
    d = CDialogLabSettings::Str2Double(s);
    m_pData->SetStutterThreshold(d);

    s = m_pTextAdenylationThreshold->GetValue();
    d = CDialogLabSettings::Str2Double(s);
    m_pData->SetAdenylationThreshold(d);

    *m_pbAllowOverride = m_pCheckAllowOverride->GetValue();

  }
  return bRtn;
}
bool CPanelRFU::TransferDataToWindow()
{
  wxString s;
  int n = m_pData->GetMinRFU();
  s = CDialogLabSettings::Number2Str(n);
  m_pTextMinRFU->SetValue(s);
  if(m_pTextMinRFUinterlocus != NULL)
  {
    int ni = m_pData->GetMinRFUinterlocus();
    if(ni < n)
    {
      ni = n;
    }
    s = CDialogLabSettings::Number2Str(ni);
    m_pTextMinRFUinterlocus->SetValue(s);
  }
  s = CDialogLabSettings::Number2Str(m_pData->GetMaxRFU());
  m_pTextMaxRFU->SetValue(s);
  s = CDialogLabSettings::Number2Str(m_pData->GetFractionMaxPeak());
  m_pTextFractionOfMaxPeak->SetValue(s);
  s = CDialogLabSettings::Number2Str(m_pData->GetStutterThreshold());
  m_pTextStutterThreshold->SetValue(s);
  s = CDialogLabSettings::Number2Str(m_pData->GetAdenylationThreshold());
  m_pTextAdenylationThreshold->SetValue(s);
  if(m_pTextPullupFractionFilter != NULL)
  {
    s = CDialogLabSettings::Number2Str(m_pData->GetPullupFractionFilter());
    m_pTextPullupFractionFilter->SetValue(s);
  }
  m_pCheckAllowOverride->SetValue(*m_pbAllowOverride);
  return true;
}

//********************************************************************
//
//    CPanelOtherThresholds
//
CPanelOtherThresholds::CPanelOtherThresholds(
  wxWindow *parent, wxWindowID id, CLabThresholds *pData) :
    nwxPanel(parent,id),

      // the following are set to null becuase they may be conditionally
      // used in the future

    m_pTextMaxPullup(NULL),
    m_pTextMaxStutter(NULL),
    m_pTextMaxAdenyl(NULL),
    m_pTextMaxOffLadder(NULL),
    m_pTextMaxExcessiveResidual(NULL),
    m_pData(pData)
{

  wxArrayString asChoices;
  asChoices.Alloc(2);
  asChoices.Add(_T("RFU"));
  asChoices.Add(_T("% of Min. RFU"));
  wxFlexGridSizer *pSizerFG = new wxFlexGridSizer(8,3,ID_BORDER,ID_BORDER);

  nwxDoubleValidator dval;
  nwxUintValidator nval(NULL,0,20000);
  nwxUintValidator nvalPct(NULL,0,20000);
  m_pTextHetImbalance = CDialogLabSettings::CreateTextCtrl(this,dval);
  m_pTextMinHomozygote = CDialogLabSettings::CreateTextCtrl(this,dval);
  m_pChoiceHomozygoteUnits = new wxChoice(this,wxID_ANY,wxDefaultPosition, wxDefaultSize, asChoices);

  m_pTextMaxPullup = CDialogLabSettings::CreateTextCtrl(this,nval);
  m_pTextMaxStutter = CDialogLabSettings::CreateTextCtrl(this,nval);
//  m_pTextMaxSpike = CDialogLabSettings::CreateTextCtrl(this);
  m_pTextMaxAdenyl = CDialogLabSettings::CreateTextCtrl(this,nval);
  m_pTextMaxOffLadder = CDialogLabSettings::CreateTextCtrl(this,nval);
  
  m_pTextMaxResidual = CDialogLabSettings::CreateTextCtrl(this,dval);
  
  m_pTextMaxExcessiveResidual = CDialogLabSettings::CreateTextCtrl(this,nval);
  m_pTextMinBPSForArtifacts = CDialogLabSettings::CreateTextCtrl(this,nval);

  _AddLine(this,pSizerFG,"Heterozygous imbalance limit:",m_pTextHetImbalance,"Ratio of allele height");
  _AddLine(this,pSizerFG,"Min. homozygous limit:",m_pTextMinHomozygote,m_pChoiceHomozygoteUnits);
  _AddLine(this,pSizerFG,"Max. pull-ups per sample:",m_pTextMaxPullup,"peaks");
  _AddLine(this,pSizerFG,"Max. stutter per sample:",m_pTextMaxStutter,"peaks");
  //_AddLine(this,pSizerFG,"Max. spikes per sample:", m_pTextMaxSpike,"peaks");
  _AddLine(this,pSizerFG,"Max. adenylation per sample:",m_pTextMaxAdenyl,"peaks");
  _AddLine(this,pSizerFG,"Max. off-ladder alleles per sample:",m_pTextMaxOffLadder,"peaks");
  _AddLine(this,pSizerFG,"Max. residual for allele (<0.5 bp):",m_pTextMaxResidual,"Ladder/sample alignment");
  _AddLine(this,pSizerFG,"Max. peaks with excessive residual:",m_pTextMaxExcessiveResidual,"peaks");
  _AddLine(this,pSizerFG,"Min. BPS for artifacts:",m_pTextMinBPSForArtifacts);


  // now use the message book parameters
  set<wxString> setMsgThresholds; // message names for thresholds already used
  size_t nCount = m_msgBook.IsOK() ? m_msgBook.GetMessageCount() : 0;
  wxTextCtrl *pTextCtrl;
  const CXMLmessageBookSM *pmsgSM;
  bool bPct;

  // the following 5 will not use the message book
  setMsgThresholds.insert(CLabThresholds::g_psmNumberOfPullUpsInSample);
  setMsgThresholds.insert(CLabThresholds::g_psmNumberOfStuttersInSample);
  setMsgThresholds.insert(CLabThresholds::g_psmNumberOfAdenylationsInSample);
  setMsgThresholds.insert(CLabThresholds::g_psmNumberOfOffLadderAllelesInSample);
  setMsgThresholds.insert(CLabThresholds::g_psmNumberOfExcessiveResidualsInSample);

  for(size_t i = 0; i < nCount; i++)
  {
    pmsgSM = m_msgBook.GetMessage(i);
    if( (!pmsgSM->GetDescriptor().IsEmpty()) &&
      (setMsgThresholds.find(pmsgSM->GetName()) == setMsgThresholds.end()) )
    {
      bPct = pmsgSM->IsPercent();
      nwxUintValidator &pnval = bPct ? nvalPct : nval;
      pTextCtrl = CDialogLabSettings::CreateTextCtrl(this,pnval);
      setMsgThresholds.insert(pmsgSM->GetName());
      _AddLine(this,pSizerFG,
        pmsgSM->GetDescriptor().c_str(), pTextCtrl,bPct ? "%" : "");
      m_pvTextMsgBookThresholds.push_back(pTextCtrl);
      m_pvMsgBookSM.push_back(pmsgSM);
    }
  }

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pSizerFG,0,wxALL | wxALIGN_CENTER, ID_BORDER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
  Layout();
  Fit();
}

void CPanelOtherThresholds::_AddLine(
  wxWindow *parent,
  wxSizer *pSizer,
  const char *psLabel,
  wxWindow *pwText,
  wxWindow *pwinSuffix)
{
  pSizer->Add(
    new wxStaticText(parent,wxID_ANY,_T(psLabel)),
    0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  pSizer->Add(pwText,1,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  if(pwinSuffix != NULL)
  {
    pSizer->Add(pwinSuffix,
      0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  }
  else
  {
    pSizer->AddSpacer(1);
  }
}
void CPanelOtherThresholds::_AddLine(
  wxWindow *parent,
  wxSizer *pSizer,
  const char *psLabel,
  wxWindow *pwText,
  const char *psSuffix)
{
  wxStaticText *pWinSuffix(NULL);
  if((psSuffix != NULL) && (*psSuffix))
  {
    pWinSuffix = new wxStaticText(parent,wxID_ANY,_T(psSuffix));
  }
  _AddLine(parent,pSizer,psLabel,pwText,pWinSuffix);
}
bool CPanelOtherThresholds::TransferDataFromWindow()
{
  wxString s;
  double d;
  int n;

  n = this->m_pChoiceHomozygoteUnits->GetSelection();
  if(n > 0)
  {
    s = _T("%");
  }
  else
  {
    s = _T("R");
  }
  m_pData->m_sMinBoundHomozygoteUnit = s;

  s = m_pTextHetImbalance->GetValue();
  d = CDialogLabSettings::Str2Double(s);
  m_pData->m_dHeterozygousImbalanceLimit = d;

  s = m_pTextMinHomozygote->GetValue();
  d = CDialogLabSettings::Str2Double(s);
  m_pData->m_dMinBoundForHomozygote = d;

  s = m_pTextMaxResidual->GetValue();
  d = CDialogLabSettings::Str2Double(s);
  m_pData->m_dMaxResidualForAlleleCall = d;

  s = this->m_pTextMaxExcessiveResidual->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->SetMaxExcessiveResidual(n);

  s = m_pTextMaxPullup->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->SetMaxNumberOfPullupsPerSample(n);

  s = m_pTextMaxStutter->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->SetMaxNumberOfStutterPeaksPerSample(n);

//  s = m_pTextMaxSpike->GetValue();
//  n = CDialogLabSettings::Str2Int(s);
//  m_pData->m_nMaxNumberOfSpikesPerSample = n;

  s = m_pTextMaxAdenyl->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->SetMaxNumberOfAdenylationsPerSample(n);

  s = m_pTextMaxOffLadder->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->SetMaxNumberOfOLAllelesPerSample(n);

  s = m_pTextMinBPSForArtifacts->GetValue();
  n = CDialogLabSettings::Str2Int(s);
  m_pData->m_nMinBPSForArtifacts = n;


  // message book parameters

  size_t nCount = m_pvTextMsgBookThresholds.size();
  vector<wxTextCtrl *>::iterator itrText = m_pvTextMsgBookThresholds.begin();
  vector<const CXMLmessageBookSM *>::iterator itrMsg = m_pvMsgBookSM.begin();
  for(size_t i = 0; i < nCount; i++)
  {
    s = (*itrText)->GetValue();
    n = CDialogLabSettings::Str2Int(s,0);
    if(n == (int)(*itrMsg)->GetThreshold())
    {
      // using default value from message book
      n = 0;
    }
    m_pData->SetValue((*itrMsg)->GetName(),n);
    ++itrMsg;
    ++itrText;
  }
  return true;
}
bool CPanelOtherThresholds::TransferDataToWindow()
{
  const char *psHomozygoteUnit = m_pData->m_sMinBoundHomozygoteUnit.c_str();
  int n = ((*psHomozygoteUnit) == '%');
  m_pChoiceHomozygoteUnits->Select(n);
  m_pTextHetImbalance->SetValue(
    CDialogLabSettings::Number2Str(m_pData->m_dHeterozygousImbalanceLimit));
  m_pTextMinHomozygote->SetValue(
    CDialogLabSettings::Number2Str(m_pData->m_dMinBoundForHomozygote));
  m_pTextMaxPullup->SetValue(
    CDialogLabSettings::Number2Str(m_pData->GetMaxNumberOfPullupsPerSample()));
  m_pTextMaxStutter->SetValue(
    CDialogLabSettings::Number2Str(m_pData->GetMaxNumberOfStutterPeaksPerSample()));
//  m_pTextMaxSpike->SetValue(
//    CDialogLabSettings::Number2Str(m_pData->m_nMaxNumberOfSpikesPerSample));
  m_pTextMaxAdenyl->SetValue(
    CDialogLabSettings::Number2Str(m_pData->GetMaxNumberOfAdenylationsPerSample()));
  m_pTextMaxOffLadder->SetValue(
    CDialogLabSettings::Number2Str(m_pData->GetMaxNumberOfOLAllelesPerSample()));
  m_pTextMaxResidual->SetValue(
    CDialogLabSettings::Number2Str(m_pData->m_dMaxResidualForAlleleCall));
  n = m_pData->GetMaxExcessiveResidual();
  m_pTextMaxExcessiveResidual->SetValue(
    CDialogLabSettings::Number2Str(n));
  m_pTextMinBPSForArtifacts->SetValue(
    CDialogLabSettings::Number2Str(m_pData->m_nMinBPSForArtifacts));

  //  message book stuff
  size_t nCount = m_pvTextMsgBookThresholds.size();
  int nValue;
  vector<wxTextCtrl *>::iterator itrText = m_pvTextMsgBookThresholds.begin();
  vector<const CXMLmessageBookSM *>::iterator itrMsg = m_pvMsgBookSM.begin();
  for(size_t i = 0; i < nCount; i++)
  {
    nValue = m_pData->GetValue((*itrMsg)->GetName());
    if(nValue <= 0)
    {
      nValue = (int)(*itrMsg)->GetThreshold();
    }
    if(!nValue) { nValue = -1;}
    (*itrText)->SetValue(
        CDialogLabSettings::Number2Str(nValue));
    ++itrText;
    ++itrMsg;
  }
  return true;
}
//********************************************************************
//
//    CPanelLabThresholds
//


CPanelLabThresholds::~CPanelLabThresholds() {;}

CPanelLabThresholds::CPanelLabThresholds(
  wxWindow *parent,
  wxWindowID id,
  CLabThresholds *pData) :
    nwxPanel(parent,id),
      m_pData(pData)
{
  m_pBook = new nwxChoicebook(this);
  m_pPanelMisc = new CPanelOtherThresholds(m_pBook,wxID_ANY,pData);
  m_pPanelLadder = CreateRFUPanel(pData->m_rfuLadder);
  m_pPanelLS = CreateRFUPanel(pData->m_rfuLS);
  m_pPanelSample = CreateRFUPanel(pData->m_rfuSample);

  m_pBook->AddPage(m_pPanelLadder,_T("Ladder"),false);
  m_pBook->AddPage(m_pPanelLS,_T("Internal Lane Standard"),false);
  m_pBook->AddPage(m_pPanelSample,_T("Sample"),false);
  m_pBook->AddPage(m_pPanelMisc,_T("Other Thresholds"),true);

  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pBook,1,wxEXPAND | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
  m_pBook->SetSelection(0);
}


bool CPanelLabThresholds::TransferDataFromWindow()
{
  bool bRtn = false;
  if(!m_pPanelLadder->TransferDataFromWindow()) {;}
  else if(!m_pPanelLS->TransferDataFromWindow()) {;}
  else if(!m_pPanelSample->TransferDataFromWindow()) {;}
  else if(!this->m_pPanelMisc->TransferDataFromWindow()) {;}
  else
  {
    bRtn = true;
  }
  return bRtn;
}
bool CPanelLabThresholds::TransferDataToWindow()
{
  bool bRtn = true;
  if(!m_pPanelLadder->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelLS->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelSample->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelMisc->TransferDataToWindow())
  {
    bRtn = false;
  }
  return bRtn;
}

void CPanelLabThresholds::OnCheckbox(wxCommandEvent &e)
{
  wxCheckBox *pcb = (wxCheckBox *)e.GetEventObject();
  m_sync.Sync(pcb);
}

BEGIN_EVENT_TABLE(CPanelLabThresholds,nwxPanel)
EVT_CHECKBOX(wxID_ANY,CPanelLabThresholds::OnCheckbox)
END_EVENT_TABLE()

//********************************************************************
//
//    CPanelLabLocusCollection
//
const int CPanelLabLocusCollection::GRIDROWS = 60;

CPanelLabLocusCollection::CPanelLabLocusCollection(
  wxWindow *parent, CLabLocusCollection *pData,const set<wxString> *pSetLoci,
  int nGridRows, bool bTriallele, bool bSkipAmel) :
      nwxPanel(parent,wxID_ANY),
      m_pData(pData),
      m_pSetLoci(pSetLoci),
      m_nGridRows(nGridRows),
      m_bTriallele(bTriallele),
      m_bSkipAmel(bTriallele || bSkipAmel)
{
  m_pBook = new nwxChoicebook(this);
  m_pBook->SetupButtons(_T("locus"));
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(new wxStaticText(this, wxID_ANY,_T("Locus")),
    0,wxLEFT | wxALIGN_LEFT, ID_BORDER << 1);
  pSizer->Add(m_pBook,1,wxEXPAND | wxLEFT, ID_BORDER << 1);
  SetSizer(pSizer);
  Layout();
}
CPanelLabLocusCollection::~CPanelLabLocusCollection() {;}

bool CPanelLabLocusCollection::TransferDataFromWindow()
{
  return m_pBook->TransferDataFromWindow();
}
bool CPanelLabLocusCollection::TransferDataToWindow()
{
  wxString sLocusName;
  mapSort mapName;
  set<size_t> killList;
  vector<CLabLocus *> *pvLocus(m_pData->Get());
  CLabLocus *pLocus;
  nwxPanel *pPanel;
  CPanelAlleleGrid *pa;
  CPanelTrialleleGrid *p3a;
  size_t nSize = pvLocus->size();
  size_t i;
  bool bRtn = true;

  m_pBook->DeleteAllPages();

  // use a set<> to sort locus name, case insensitive

  for(i = 0; i < nSize; i++)
  {
    pLocus = pvLocus->at(i);
    sLocusName = pLocus->GetName();
    sLocusName.Trim(true);
    sLocusName.Trim(false);
    if(sLocusName != pLocus->GetName())
    {
      pLocus->SetName(sLocusName);
    }
    sLocusName.MakeLower();
    if(mapName.find(sLocusName) == mapName.end())
    {
      mapName.insert(mapSort::value_type(sLocusName,i));
    }
    else if(!m_bTriallele)
    {
      killList.insert(i);
      wxASSERT_MSG(0,"found duplicate locus in lab settings");
    }
  }

  // in the future, rebuild array sorted

  i = 0;
  for(mapSort::iterator itr = mapName.begin();
    itr != mapName.end();
    ++itr)
  {
    i = itr->second;
    pLocus = pvLocus->at(i);
    sLocusName = pLocus->GetName();
    if(m_bTriallele)
    {
      p3a = new CPanelTrialleleGrid(m_pBook,m_pData,sLocusName);
      pPanel = p3a;
    }
    else
    {
      pa = new CPanelAlleleGrid(
        m_pBook,pLocus, m_nGridRows);
      pPanel = pa;
    }
    m_pBook->AddPage(pPanel,sLocusName,!i);
    if(m_pBook->IsReadOnly())
    {
      pPanel->SetReadOnly(true);
    }
  }
  if(m_pBook->GetPageCount())
  {
    m_pBook->SetSelection(0);
  }
  return bRtn;
}


void CPanelLabLocusCollection::OnAddLocus(wxCommandEvent &)
{
  CDialogName dlg(this,m_pBook,
    _T("Enter the name of the locus"),
    _T("New Locus"),
    m_pSetLoci,
    m_bSkipAmel
    );
  if(dlg.ShowModal() == wxID_OK)
  {
    wxString sName = dlg.GetName();
    vector<CLabLocus *> *pvLocus(m_pData->Get());
    nwxPanel *pPanel;
    if(m_bTriallele)
    {
      CPanelTrialleleGrid *p = new CPanelTrialleleGrid(
        m_pBook,m_pData,sName,m_nGridRows);
      pPanel = p;
    }
    else
    {
      CLabLocus *pNew = new CLabLocus;
      pNew->SetName(sName);
      pvLocus->push_back(pNew);
      CPanelAlleleGrid *p = new CPanelAlleleGrid(
        m_pBook,pNew,m_nGridRows);
      pPanel = p;
    }
    m_pBook->InsertPageSorted(pPanel,sName,true);
  }
}

void CPanelLabLocusCollection::OnRemoveLocus(wxCommandEvent &)
{
  size_t nSize = m_pBook->GetPageCount();

  if(nSize)
  {
    int nCurrent = m_pBook->GetSelection();
    wxString sName = m_pBook->GetPageText(nCurrent);
    wxString sMsg = _T("Do you want to remove ");
    sMsg += sName;
    sMsg += _T("?");
    if(mainApp::Confirm(this,sMsg) )
    {
      if(!m_bTriallele)
      {
        CPanelAlleleGrid *pPanel =
          (CPanelAlleleGrid *)(m_pBook->GetCurrentPage());
        CLabLocus *pLocus = pPanel->GetLocusData();
        vector<CLabLocus *> *pvLocus = m_pData->Get();
        if(!vectorptr<CLabLocus>::remove(pvLocus,pLocus))
        {
          wxASSERT_MSG(0,_T("Could not remove CLabLocus from list"));
        }
      }
      m_pBook->DeletePage(nCurrent);
    }
  }
}

bool CPanelLabLocusCollection::IsNameUsed(const wxString &s)
{
#if 0
  wxString sNew(s);
  wxString sUsed;
  bool bRtn = false;
  nwxString::TrimLower(&sNew);
  vector<CLabLocus *> *pvLocus = m_pData->Get();
  for(vector<CLabLocus *>::iterator itr = pvLocus->begin();
    itr != pvLocus->end();
    ++itr)
  {
    sUsed = (*itr)->GetName();
    nwxString::TrimLower(&sUsed);
    if(!sUsed.Cmp(sNew))
    {
      bRtn = true;
      break;
    }
  }
  return bRtn;
#else
  return m_pBook->IsNameUsed(s);
#endif
}

BEGIN_EVENT_TABLE(CPanelLabLocusCollection,wxPanel)
EVT_BUTTON(IDadd,CPanelLabLocusCollection::OnAddLocus)
EVT_BUTTON(IDremove,CPanelLabLocusCollection::OnRemoveLocus)
END_EVENT_TABLE()


//********************************************************************
//
//    CPanelPosCtrlLoci
//

const int CPanelPosCtrlLoci::ROWS = 4;


CPanelPosCtrlLoci::CPanelPosCtrlLoci(
  wxWindow *parent,
  CLabPositiveControl *pPosCtrl,
  const set<wxString> *pSetLoci) :
    nwxPanel(parent,wxID_ANY),
    m_pPosCtrl(pPosCtrl)
{
  wxStaticText *pLabelFile = new wxStaticText(this,wxID_ANY,CDialogLabSettings::g_sFileNameStrPrompt);
  m_pTextFileStr = new wxTextCtrl(this,wxID_ANY,pPosCtrl->GetFileNameString(),wxDefaultPosition, wxDefaultSize,0);
  m_pPanelLoci = new CPanelLabLocusCollection(
    this, pPosCtrl->GetLoci(),pSetLoci,
    CPanelPosCtrlLoci::ROWS,
    false,false);
  wxBoxSizer *pSizerTop = new wxBoxSizer(wxVERTICAL);
  pSizerTop->Add(pLabelFile,0,wxLEFT | wxALIGN_LEFT, ID_BORDER << 1);
  pSizerTop->Add(m_pTextFileStr,0,wxLEFT | wxEXPAND, ID_BORDER << 1);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pSizerTop,0,wxEXPAND | wxBOTTOM,ID_BORDER);
  pSizer->Add(m_pPanelLoci,1,wxEXPAND);
  SetSizer(pSizer);
  Layout();
}

CPanelPosCtrlLoci::~CPanelPosCtrlLoci() {}

bool CPanelPosCtrlLoci::TransferDataToWindow()
{
  bool bRtn = m_pPanelLoci->TransferDataToWindow();
  m_pTextFileStr->SetValue(m_pPosCtrl->GetFileNameString());
  return bRtn;
}
bool CPanelPosCtrlLoci::TransferDataFromWindow()
{
  bool bRtn = m_pPanelLoci->TransferDataFromWindow();
  if(bRtn)
  {
    wxString s = m_pTextFileStr->GetValue();
    if(!(s.IsEmpty() || nwxString::FileNameStringOK(s)))
    {
      bRtn = false;
      wxString sError = CDialogLabSettings::g_sFileNameStrPrompt;
      sError.Append(_T(" contains\ninvalid file name characters."));
      mainApp::ShowError(sError,this);
    }
    else
    {
      m_pPosCtrl->SetFileNameString(s);
    }
  }
  return bRtn;
}

//********************************************************************
//
//    CDialogName
//

CDialogName::~CDialogName() {;}

CDialogName::CDialogName(
  wxWindow *pParent, INameValidator *pv,
  const wxString &sPrompt,
  const wxString &sTitle,
  const set<wxString> *pSetValues,
  bool bSkipAmel,
  const wxString *psCurrent) :
    wxDialog(pParent,wxID_ANY, sTitle,
      wxDefaultPosition, wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE),
    m_sPrompt(sPrompt),
    m_pText(NULL),
    m_pCombo(NULL),
    m_pv(pv)
{
  wxWindow *pWin(NULL);
  if((pSetValues != NULL) && (pSetValues->size() > 0))
  {
    wxArrayString as;
    as.Alloc(pSetValues->size());
    for(set<wxString>::const_iterator itr = pSetValues->begin();
      itr != pSetValues->end();
      ++itr)
    {
      if(!( bSkipAmel && CLabLocus::IsAmel(*itr) ))
      {
        as.Add(*itr);
      }
    }
    m_pCombo = new wxComboBox(
      this,wxID_ANY,wxEmptyString,
      wxDefaultPosition, wxSize(120,-1),
      as,
      wxCB_DROPDOWN | wxCB_SORT | wxCB_READONLY);
    pWin = m_pCombo;
  }
  else
  {
    if(psCurrent != NULL)
    {
      m_sCurrent = *psCurrent;
    }
    m_pText = new wxTextCtrl(this,wxID_ANY,m_sCurrent,
      wxDefaultPosition, wxSize(100,-1),
      wxTE_LEFT);
    pWin = m_pText;
  }
  wxStaticText *pLabel = new wxStaticText(this,wxID_ANY,sPrompt);
  wxSizer *pButtons = CreateButtonSizer(wxOK | wxCANCEL);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pLabel,0,wxALIGN_LEFT | wxALL, ID_BORDER);
  pSizer->Add(pWin,0,wxEXPAND | wxLEFT | wxRIGHT, ID_BORDER);
  pSizer->Add(pButtons,0,wxALIGN_CENTER | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
  pWin->SetFocus();
}
wxString CDialogName::GetName()
{
  wxString s;
  if(m_pText != NULL)
  {
    s = m_pText->GetValue();
  }
  else if(m_pCombo != NULL)
  {
    s = m_pCombo->GetValue();
  }
  s.Trim(true);
  s.Trim(false);
  return s;
}



bool CDialogName::TransferDataFromWindow()
{
  wxString sName = GetName();
  wxString sMsg;
  bool bRtn = true;
  if(!sName.Len())
  {
    sMsg = m_sPrompt;
    bRtn = false;
  }
  else if(!sName.CmpNoCase(m_sCurrent))
  {
    ;// name is unchanged in a 'rename'
     // so it is OK
  }
  else if(m_pv->IsNameUsed(sName))
  {
    sMsg = _T("This name is already used");
    bRtn = false;
  }
  if(!bRtn)
  {
    mainApp::ShowError(sMsg,this);
  }
  return bRtn;
}

//********************************************************************
//
//    CPanelAlleleGrid
//

CPanelAlleleGrid::~CPanelAlleleGrid() {;}

CPanelAlleleGrid::CPanelAlleleGrid(
  wxWindow *parent,CLabLocus *pLocus, 
    int nGridRows) :
  nwxPanel(parent,wxID_ANY),
    m_pLocus(pLocus),
    m_nGridRows(nGridRows)
{
  CreateAlleleGrid(pLocus->IsAmel());
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pGrid,1,wxEXPAND | wxLEFT, ID_BORDER << 1);
  TransferDataToWindow();
  SetSizer(pSizer);
  Layout();
}
void CPanelAlleleGrid::CreateAlleleGrid(bool bIsAmel)
{
  m_pGrid = new nwxGrid(this,wxID_ANY);
  m_pGrid->CreateGrid(m_nGridRows,1);
  m_pGrid->SetRowLabelSize(1);
  m_pGrid->SetColLabelValue(0,_T("Allele"));
  for(int i = 0; i < m_nGridRows; i++)
  {
    m_pGrid->SetRowLabelValue(i,wxEmptyString);
  }
  m_pGrid->EnableScrolling(true,true);
  if(!bIsAmel)
  {
    m_pGrid->SetDefaultEditor(new wxGridCellFloatEditor(-1,1));
  }
  //m_pGrid->SetDefaultRenderer(new wxGridCellFloatRenderer(-1,1));
}

bool CPanelAlleleGrid::TransferDataToWindow()
{
  nwxGridBatch batch(m_pGrid);
  wxString sAllele;
  int i = 0;
  int nSize = m_pGrid->GetNumberRows();
  int n;
  vector<wxString> *pData = m_pLocus->GetAlleles();
  m_pGrid->ClearGrid();

  for(vector<wxString>::iterator itr = pData->begin();
    (itr != pData->end()) && (i < nSize);
    ++itr)
  {
    sAllele = (*itr);
    sAllele.Trim(true);
    sAllele.Trim(false);
    n = sAllele.Find(wxChar('\n'));
    if(n != wxNOT_FOUND)
    {
      sAllele = sAllele.Remove(n);
      sAllele.Trim(true);
    }
    if(sAllele.Len())
    {
      m_pGrid->SetCellValue(i,0,sAllele);
      i++;
    }
  }
  m_pGrid->GetSize(&n,&i);
  m_pGrid->SetColSize(0,n);
  return true;
}
bool CPanelAlleleGrid::TransferDataFromWindow()
{
  wxString sName;
  vector<wxString> *pData = m_pLocus->GetAlleles();
  pData->clear();
  int nRows = m_pGrid->GetNumberRows();
  int i;
  for(i = 0; i < nRows; i++)
  {
    sName = m_pGrid->GetCellValue(i,0);
    sName.Trim(true);
    sName.Trim(false);
    if(sName.Len())
    {
      pData->push_back(sName);
    }
  }
  return true;
}
//********************************************************************
//
//    CPanelTrialleleGrid
//

CPanelTrialleleGrid::~CPanelTrialleleGrid() {;}

CPanelTrialleleGrid::CPanelTrialleleGrid(
    wxWindow *parent,CLabLocusCollection *pLocus, const wxString &sLocusName, int nGridRows) :
  nwxPanel(parent,wxID_ANY),
  m_pLocus(pLocus),
  m_sLocusName(sLocusName),
  m_nGridRows(nGridRows)
{
  CreateAlleleGrid(CLabLocus::IsAmel(sLocusName));
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pGrid,1,wxEXPAND | wxLEFT, ID_BORDER << 1);
  TransferDataToWindow();
  SetSizer(pSizer);
  Layout();
}
void CPanelTrialleleGrid::CreateAlleleGrid(bool bIsAmel)
{
  m_pGrid = new nwxGrid(this,wxID_ANY);
  m_pGrid->CreateGrid(m_nGridRows,3);
  m_pGrid->SetRowLabelSize(1);
  m_pGrid->SetColLabelValue(0,_T("Allele 1"));
  m_pGrid->SetColLabelValue(1,_T("Allele 2"));
  m_pGrid->SetColLabelValue(2,_T("Allele 3"));
  for(int i = 0; i < m_nGridRows; i++)
  {
    m_pGrid->SetRowLabelValue(i,wxEmptyString);
  }
  if(!bIsAmel)
  {
    m_pGrid->SetDefaultEditor(new wxGridCellFloatEditor(-1,1));
  }
  m_pGrid->EnableScrolling(true,true);
}

bool CPanelTrialleleGrid::TransferDataToWindow()
{
  nwxGridBatch batch(m_pGrid);
  wxString sAllele;
  int nRow = 0;
  int nRows = m_pGrid->GetNumberRows();
  int n;
  int nCol;
  m_pGrid->ClearGrid();

  for(vector<CLabLocus *>::iterator itr = (*m_pLocus)->begin();
    (itr != (*m_pLocus)->end()) && (nRow < nRows);
    ++itr)
  {
    CLabLocus *pLabLocus(*itr);
    if(pLabLocus->GetName() == m_sLocusName)
    {
      vector<wxString> *pvAlleles(pLabLocus->GetAlleles());
      nCol = 0;
      for(vector<wxString>::iterator itrs = pvAlleles->begin();
        itrs != pvAlleles->end();
        ++itrs)
      {
        sAllele = (*itrs);
        sAllele.Trim(true);
        sAllele.Trim(false);
        n = sAllele.Find(wxChar('\n'));
        if(n != wxNOT_FOUND)
        {
          sAllele = sAllele.Remove(n);
          sAllele.Trim(true);
        }
        if(sAllele.Len())
        {
          m_pGrid->SetCellValue(nRow,nCol,sAllele);
          nCol++;
        }
      }
      if(nCol)
      {
        // we found one or more alleles, therefore
        // we need to increment the row
        nRow++;
      }
    }
  }
  return true;
}
bool CPanelTrialleleGrid::TransferDataFromWindow()
{
  wxString sName;
  vector<CLabLocus *> *pData = m_pLocus->Get();
  vector<CLabLocus *> saveData;
  vector<CLabLocus *> locusData;
  vector<wxString> vsAllele;
  int nRows = m_pGrid->GetNumberRows();
  int nCols = m_pGrid->GetNumberCols();
  int nRow;
  int nCol;
  CLabLocus *pLocus;

  saveData.reserve(pData->size() + 12);
  locusData.reserve(pData->size());
  vsAllele.reserve((size_t)nCols);
  //
  //  copy each CLabLocus pointer
  //  recycle where locus name matches
  //  save and restore others
  //
  for(size_t ndx = 0; ndx < pData->size(); ++ndx)
  {
    CLabLocus *p = pData->at(ndx);
    if(p->GetName() == m_sLocusName)
    {
      locusData.push_back(p);
    }
    else
    {
      saveData.push_back(p);
    }
  }
  for(nRow = 0; nRow < nRows; ++nRow)
  {
    vsAllele.clear();
    for(nCol = 0; nCol < nCols; nCol++)
    {
      sName = m_pGrid->GetCellValue(nRow,nCol);
      sName.Trim(true);
      sName.Trim(false);
      if(sName.Len())
      {
        vsAllele.push_back(sName);
      }
    }
    if(vsAllele.size())
    {
      size_t nSize = locusData.size();
      if(nSize)
      {
        // recycle an old CLabLocus *
        --nSize;
        pLocus = locusData.at(nSize); // take from end of list
        locusData.resize(nSize); // remove from list
        pLocus->ClearAlleles(); // clear old alleles
      }
      else
      {
        // create a new CLabLocus
        pLocus = new CLabLocus();
        pLocus->SetName(m_sLocusName);
      }
      saveData.push_back(pLocus);
      vector<wxString> *pAlleles = pLocus->GetAlleles();
      (*pAlleles) = vsAllele;
    }
  }
  (*pData) = saveData;
  vectorptr<CLabLocus>::cleanup(&locusData);
  return true;
}

//********************************************************************
//
//    CPanelLabMarkerSet
//

CPanelLabMarkerSet::~CPanelLabMarkerSet() {;}

CPanelLabMarkerSet::CPanelLabMarkerSet
  (wxWindow *parent, CLabMarkerSetCollection *pData, const set<wxString> *pSetLoci) :
    nwxPanel(parent,wxID_ANY),
      m_pData(pData),
      m_pSetLoci(pSetLoci)
{
  m_pBook = new nwxChoicebook(this);
  m_pPanelOffLadder = new CPanelLabLocusCollection
    (m_pBook,m_pData->GetOffLadderLoci(),m_pSetLoci,
    CPanelLabLocusCollection::GRIDROWS,false,true);
  m_pPanelSampleTrialleles = new CPanelLabLocusCollection(
    m_pBook,m_pData->GetSampleTriallleles(), m_pSetLoci,
    CPanelLabLocusCollection::GRIDROWS,
    true);
//  m_pPanelPosCtrlTrialleles = new CPanelLabLocusCollection(
//    m_pBook,m_pData->GetPosCtrlTrialleles(),m_pSetLoci,
//    CPanelLabLocusCollection::GRIDROWS,
//    true);
  m_pPanelPosCtrl = new CPanelPosCtrlMarkerSet(
    m_pBook,m_pData->GetPositiveControls(), m_pSetLoci);
  m_pBook->AddPage(m_pPanelOffLadder,_T("Off ladder alleles"),true);
  m_pBook->AddPage(m_pPanelSampleTrialleles,_T("Sample trialleles"));
//  m_pBook->AddPage(m_pPanelPosCtrlTrialleles,_T("Control trialleles"));
//  3/26/10 - Positive control tri-alleles are obsolete in lab settings
//    bacause of positive controls below
  m_pBook->AddPage(m_pPanelPosCtrl,_T("Positive controls"));

  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pBook,1,wxEXPAND | wxLEFT, ID_BORDER << 1);
  SetSizer(pSizer);
  Layout();
  Fit();
}

bool CPanelLabMarkerSet::TransferDataFromWindow()
{
  bool bRtn = true;

  if(!m_pPanelOffLadder->TransferDataFromWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelSampleTrialleles->TransferDataFromWindow())
  {
    bRtn = false;
  }
//  if(!m_pPanelPosCtrlTrialleles->TransferDataFromWindow())
//  {
//    bRtn = false;
//  }
  if(!m_pPanelPosCtrl->TransferDataFromWindow())
  {
    bRtn = false;
  }
  return bRtn;
}

bool CPanelLabMarkerSet::TransferDataToWindow()
{
  bool bRtn = true;

  if(!m_pPanelOffLadder->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelSampleTrialleles->TransferDataToWindow())
  {
    bRtn = false;
  }
//  if(!m_pPanelPosCtrlTrialleles->TransferDataToWindow())
//  {
//    bRtn = false;
//  }
  if(!m_pPanelPosCtrl->TransferDataToWindow())
  {
    bRtn = false;
  }
  return bRtn;
}
//********************************************************************
//
//    CPanelPosCtrlMarkerSet
//

CPanelPosCtrlMarkerSet::~CPanelPosCtrlMarkerSet() {;}

CPanelPosCtrlMarkerSet::CPanelPosCtrlMarkerSet(
  wxWindow *parent, CLabPositiveControls *pData, const set<wxString> *pSetLoci) :
    nwxPanel(parent,wxID_ANY),
      m_pData(pData),
      m_pSetLoci(pSetLoci)
{
  m_pBook = new nwxChoicebook(this);
  m_pBook->SetupButtons(_T("control"),true);
  vector<CLabPositiveControl *> *pvCtrl = pData->Get();
  CLabPositiveControl *pctrl;
  size_t nSize = pvCtrl->size();
  size_t i;
  wxString sName;
  mapSort mapSN;
  for(i = 0; i < nSize; i++)
  {
    pctrl = pvCtrl->at(i);
    sName = pctrl->GetName();
    sName.Trim(true);
    sName.Trim(false);
    sName.MakeLower();
    mapSN.insert(mapSort::value_type(sName,i));
  }
  for(mapSort::iterator itr = mapSN.begin();
    itr != mapSN.end();
    ++itr)
  {
    pctrl = pvCtrl->at(itr->second);
    CPanelPosCtrlLoci *pw =
      new CPanelPosCtrlLoci(m_pBook,pctrl,m_pSetLoci);
    sName = pctrl->GetName();
    sName.Trim(true);
    sName.Trim(false);
    m_pBook->AddPage(pw,sName);
  }
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(new wxStaticText(this,wxID_ANY,_T("Control Name")),
    0, wxLEFT | wxALIGN_LEFT,ID_BORDER << 1);
  pSizer->Add(m_pBook,1,wxEXPAND | wxLEFT, ID_BORDER << 1);
  SetSizer(pSizer);
  Layout();
  Fit();
}
bool CPanelPosCtrlMarkerSet::TransferDataFromWindow()
{
  return m_pBook->TransferDataFromWindow();
}
bool CPanelPosCtrlMarkerSet::TransferDataToWindow()
{
  return m_pBook->TransferDataToWindow();
}

bool CPanelPosCtrlMarkerSet::IsNameUsed(const wxString &s)
{
  vector<CLabPositiveControl *> *pvCtrl(m_pData->Get());
  wxString sCmp(s);
  wxString sName;
  nwxString::TrimLower(&sCmp);
  bool bRtn = false;
  for(vector<CLabPositiveControl *>::iterator itr = pvCtrl->begin();
    itr != pvCtrl->end();
    ++itr)
  {
    sName = (*itr)->GetName();
    nwxString::TrimLower(&sName);
    if(!sName.Cmp(sCmp))
    {
      bRtn = true;
      break;
    }
  }
  return bRtn;
}

void CPanelPosCtrlMarkerSet::OnAdd(wxCommandEvent &)
{
  CDialogName dlg(this,this,
    _T("Enter the name of the new control"),
    _T("New Positive Control"));
  if(dlg.ShowModal() == wxID_OK)
  {
    wxString sName = dlg.GetName();
    nwxString::Trim(&sName);
    vector<CLabPositiveControl *> *pv(m_pData->Get());
    CLabPositiveControl *pNew = new CLabPositiveControl;
    pNew->SetName(sName);
    pv->push_back(pNew);
    CPanelPosCtrlLoci *pPanel =
      new CPanelPosCtrlLoci(m_pBook, pNew,m_pSetLoci);
    m_pBook->InsertPageSorted(pPanel,sName,true);
  }
  return;
}
void CPanelPosCtrlMarkerSet::OnRemove(wxCommandEvent &)
{
  if(m_pBook->GetPageCount())
  {
    int n = m_pBook->GetSelection();
    wxString s = m_pBook->GetPageText(n);
    wxString sMsg = _T("Do you want to remove ");
    sMsg += s;
    sMsg += _T("?");
    if(mainApp::Confirm(this,sMsg))
    {
      CPanelLabLocusCollection *pPanel =
        (CPanelLabLocusCollection *) m_pBook->GetCurrentPage();
      vector<CLabPositiveControl *> *pv(m_pData->Get());
      CLabLocusCollection *pLoci = pPanel->GetData();
      for(vector<CLabPositiveControl *>::iterator itr = pv->begin();
        itr != pv->end();
        ++itr)
      {
        if((*itr)->GetLoci() == pLoci)
        {
          CLabPositiveControl *pKill(*itr);
          pv->erase(itr);
          delete pKill;
          break;
        }
      }
      m_pBook->DeletePage(m_pBook->GetSelection());
    }
  }
  return;
}
void CPanelPosCtrlMarkerSet::OnRename(wxCommandEvent &)
{
  int nSelection = m_pBook->GetSelection();
  if(nSelection >= 0)
  {
    wxString sName = m_pBook->GetPageText((size_t)nSelection);
    nwxString::Trim(&sName);
    wxString sTitle(_T("Rename Positive Control, "));
    sTitle.Append(sName);
    CDialogName dlg(this,this,
      _T("Enter the name for this control"),
      sTitle,
      NULL,
      false,
      &sName);
    if(dlg.ShowModal() == wxID_OK)
    {
      wxString sNewName = dlg.GetName();
      bool bError = false;
      nwxString::Trim(&sNewName);
      if(sName == sNewName) {;} // do nothing
      else if(!m_pData->Rename(sName,sNewName))
      {
        bError = true;
      }
      else if(!m_pBook->RenamePage(sNewName,nSelection))
      {
        bError = true;
        m_pData->Rename(sNewName,sName); // put it back
      }
      if(bError)
      {
        wxString sError(_T(
          "An error occurred when attempting "
          "to rename a positive control"));
        mainApp::ShowError(sError,this);
      }
    }
  }
}

BEGIN_EVENT_TABLE(CPanelPosCtrlMarkerSet,wxPanel)
EVT_BUTTON(IDadd,CPanelPosCtrlMarkerSet::OnAdd)
EVT_BUTTON(IDremove,CPanelPosCtrlMarkerSet::OnRemove)
EVT_BUTTON(IDrename,CPanelPosCtrlMarkerSet::OnRename)
END_EVENT_TABLE()

//********************************************************************
//
//    CPanelLabAllMarkerSets
//
CPanelLabAllMarkerSets::~CPanelLabAllMarkerSets() {;}

CPanelLabAllMarkerSets::CPanelLabAllMarkerSets(
  wxWindow *parent,CLabMarkerSetSpecifications *pData,
  const set<wxString> *pSetLoci,
  const set<wxString> *pSetKits) :
    nwxPanel(parent,wxID_ANY),
      m_pData(pData),
      m_pSetLoci(pSetLoci),
      m_pSetKits(pSetKits)
{
  wxStaticText *pLabelNotebook =
    new wxStaticText(this,wxID_ANY,_T("Reagent Kit"));
  m_pBook = new nwxChoicebook(this);
  m_pBook->SetupButtons(_T("reagent kit"));
  vector<CLabMarkerSetCollection *> *pvMarkers(m_pData->Get());
  CLabMarkerSetCollection *pMarker;
  wxString sName;
  size_t nSize = pvMarkers->size();
  size_t i;
  mapSort mapMarkers;
  for(i = 0; i < nSize; i++)
  {
    pMarker = pvMarkers->at(i);
    sName = pMarker->GetMarkerSetName();
    sName.Trim(true);
    sName.Trim(false);
    sName.MakeLower();
    mapMarkers.insert(mapSort::value_type(sName,i));
  }
  for(mapSort::iterator itr = mapMarkers.begin();
    itr != mapMarkers.end();
    ++itr)
  {
    i = itr->second;
    pMarker = pvMarkers->at(i);
    sName = pMarker->GetMarkerSetName();
    sName.Trim(true);
    sName.Trim(false);
    CPanelLabMarkerSet *pSet = new CPanelLabMarkerSet(m_pBook,pMarker,m_pSetLoci);
    m_pBook->AddPage(pSet,sName);
  }
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(pLabelNotebook, 0, (wxALL ^ wxBOTTOM) | wxRIGHT  | wxALIGN_LEFT,ID_BORDER);
  pSizer->Add(m_pBook,1,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
}
bool CPanelLabAllMarkerSets::IsNameUsed(const wxString &s)
{
  wxString sNew(s);
  wxString sCheck;
  vector<CLabMarkerSetCollection *> *pvMarkers(m_pData->Get());
  bool bRtn = false;
  nwxString::TrimLower(&sNew);
  for(vector<CLabMarkerSetCollection *>::iterator itr = pvMarkers->begin();
    itr != pvMarkers->end();
    ++itr)
  {
    sCheck = (*itr)->GetMarkerSetName();
    nwxString::TrimLower(&sCheck);
    if(!sCheck.Cmp(sNew))
    {
      bRtn = true;
      break;
    }
  }
  return bRtn;
}
bool CPanelLabAllMarkerSets::TransferDataFromWindow()
{
  bool bRtn = m_pBook->TransferDataFromWindow();
  return bRtn;
}
bool CPanelLabAllMarkerSets::TransferDataToWindow()
{
  bool bRtn = m_pBook->TransferDataToWindow();
  return bRtn;
}
void CPanelLabAllMarkerSets::OnAddMarker(wxCommandEvent &)
{
  CDialogName dlg(this,this,
    _T("Select a kit from the list"),
    _T("New Reagent Kit"),
    m_pSetKits);
  if(dlg.ShowModal() == wxID_OK)
  {
    wxString sName = dlg.GetName();
    vector<CLabMarkerSetCollection *> *pvMarkers(m_pData->Get());
    CLabMarkerSetCollection *pNew = new CLabMarkerSetCollection;
    pNew->SetMarkerSetName(sName);
    pvMarkers->push_back(pNew);
    CPanelLabMarkerSet *pSet = new CPanelLabMarkerSet(m_pBook,pNew,m_pSetLoci);
    m_pBook->InsertPageSorted(pSet,sName,true);
  }
  return;
}
void CPanelLabAllMarkerSets::OnRemoveMarker(wxCommandEvent &)
{
  size_t nSize = m_pBook->GetPageCount();

  if(nSize)
  {
    int nCurrent = m_pBook->GetSelection();
    wxString sName = m_pBook->GetPageText(nCurrent);
    wxString sMsg = _T("Do you want to remove ");
    sMsg += sName;
    sMsg += _T("?");
    if( mainApp::Confirm(this,sMsg) )
    {
      CPanelLabMarkerSet *pPanel =
        (CPanelLabMarkerSet *)(m_pBook->GetCurrentPage());
      CLabMarkerSetCollection *pd = pPanel->GetData();
      vector<CLabMarkerSetCollection *> *pvMarkers(m_pData->Get());
      if(!vectorptr<CLabMarkerSetCollection>::remove(pvMarkers,pd))
      {
        wxASSERT_MSG(0,_T("Could not remove CLabMarkerSetCollection from list"));
      }
      m_pBook->DeletePage((size_t) m_pBook->GetSelection());
    }
  }

  return;
}

BEGIN_EVENT_TABLE(CPanelLabAllMarkerSets,wxPanel)
EVT_BUTTON(IDadd,CPanelLabAllMarkerSets::OnAddMarker)
EVT_BUTTON(IDremove,CPanelLabAllMarkerSets::OnRemoveMarker)
END_EVENT_TABLE()

//********************************************************************
//
//    CPanelReviewAcceptance
//

#define MAX_USER_COUNT 10

CPanelReviewAcceptance::CPanelReviewAcceptance(
  wxWindow *parent,CLabReview *pData) : 
    nwxPanel(parent),
    m_pReview(pData)
{
  wxSize sz(65,-1);
  wxStaticText *pTextTitle = 
    new wxStaticText(this,wxID_ANY,_T("Select the minimum number of users to:"));
  wxStaticText *pTextAccept =
    new wxStaticText(this,wxID_ANY,_T("Accept alerts"));
  wxStaticText *pTextReview =
    new wxStaticText(this,wxID_ANY,_T("Review editing"));
  m_pSpinAccept = new wxSpinCtrl(
    this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, sz,wxSP_ARROW_KEYS,0,MAX_USER_COUNT,0);
  m_pSpinReview = new wxSpinCtrl(
    this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, sz,wxSP_ARROW_KEYS,0,MAX_USER_COUNT,0);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxFlexGridSizer *pSizerGrid = 
    new wxFlexGridSizer(2,2,ID_BORDER,ID_BORDER);

  wxFont fn = pTextTitle->GetFont();
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  fn.SetPointSize(fn.GetPointSize() + 1);
  pTextTitle->SetFont(fn);

  pSizerGrid->Add(pTextAccept,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
  pSizerGrid->Add(m_pSpinAccept,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
  pSizerGrid->Add(pTextReview,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
  pSizerGrid->Add(m_pSpinReview,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT);
  pSizer->Add(pTextTitle,0,wxALIGN_CENTER_HORIZONTAL | wxALL,ID_BORDER);
  pSizer->Add(pSizerGrid,0,wxALIGN_CENTER_HORIZONTAL | wxALL,ID_BORDER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
  pSizer->Layout();
}

CPanelReviewAcceptance::~CPanelReviewAcceptance() {}

bool CPanelReviewAcceptance::TransferDataToWindow()
{
  unsigned int nReviewCount = m_pReview->GetReviewerCount();
  unsigned int nAcceptCount = m_pReview->GetAcceptanceCount();
  if(nReviewCount > MAX_USER_COUNT)
  {
    nReviewCount = MAX_USER_COUNT;
  }
  if(nAcceptCount > MAX_USER_COUNT)
  {
    nAcceptCount = MAX_USER_COUNT;
  }

  m_pSpinAccept->SetValue((int)nAcceptCount);
  m_pSpinReview->SetValue((int)nReviewCount);
  return true;
}

bool CPanelReviewAcceptance::TransferDataFromWindow()
{
  int nReviewCount = m_pSpinReview->GetValue();
  int nAcceptCount = m_pSpinAccept->GetValue();
  m_pReview->SetAcceptanceCount((unsigned int) nAcceptCount);
  m_pReview->SetReviewerCount((unsigned int) nReviewCount);
  return true;
}
void CPanelReviewAcceptance::SetReadOnly(bool b)
{
  m_pSpinReview->Enable(!b);
  m_pSpinAccept->Enable(!b);
}



//********************************************************************
//
//    CDialogLabSettings
//
CDialogLabSettings::~CDialogLabSettings() {;}

CDialogLabSettings::CDialogLabSettings(
  wxWindow *parent, CLabSettings *pSettings, CAllLoci *pLoci) :
  wxDialog(parent,wxID_ANY,_T("OSIRIS Lab Settings"),
    wxDefaultPosition, wxDefaultSize,
    mainApp::DIALOG_STYLE),
  m_pSetLoci(NULL),
  m_pSetKits(NULL)
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  if(pLoci != NULL)
  {
    m_pSetLoci = pLoci->GetSetPtr();
    m_pSetKits = pLoci->GetSetPtrKits();
  }
  m_pbBack = new wxButton(
    this,IDback,_T("< &Back"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pbNext = new wxButton(
    this,IDnext,_T("&Next >"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);

  wxButton *pbOK = new wxButton(
    this,wxID_OK,_T("&Save"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  wxButton *pbCancel = new wxButton(
    this,wxID_CANCEL,_T("&Cancel"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pNotebook = new wxNotebook(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxNB_TOP);
  m_pFilenames = new CPanelLabNameStrings(
    m_pNotebook, pSettings->GetLabStrings());
  m_pMarkerSets = new CPanelLabAllMarkerSets(
    m_pNotebook,pSettings->GetMarkers(),m_pSetLoci,m_pSetKits);
  m_pThresholds = new CPanelLabThresholds(
    m_pNotebook,wxID_ANY,pSettings->GetThresholds());
  m_pReview = new CPanelReviewAcceptance(
    m_pNotebook,pSettings->GetReviewAcceptance());
  m_pNotebook->AddPage(
    m_pFilenames,_T("File Names"),true);
  m_pNotebook->AddPage(
    m_pThresholds,_T("Thresholds"),false);
  m_pNotebook->AddPage(
    m_pMarkerSets,_T("Allele Exceptions"),false);
  m_pNotebook->AddPage(
    m_pReview,_T("Accept/Review"),false);
  wxBoxSizer *pSizerAll(new wxBoxSizer(wxVERTICAL));
  wxBoxSizer *pSizerButtons(new wxBoxSizer(wxHORIZONTAL));

  pbOK->SetDefault();

  pSizerButtons->Add(m_pbBack,0,wxRIGHT,ID_BORDER);
  pSizerButtons->Add(m_pbNext,0,wxRIGHT,ID_BORDER);
  pSizerButtons->AddStretchSpacer(1);
  pSizerButtons->Add(pbOK,0,wxRIGHT,ID_BORDER);
  pSizerButtons->Add(pbCancel,0,0,0);
  pSizerAll->Add(m_pNotebook,1,(wxALL ^ wxBOTTOM) | wxEXPAND, ID_BORDER);
  pSizerAll->Add(pSizerButtons,0, wxALL | wxEXPAND, ID_BORDER);
  m_pNotebook->SetFocus();
  SetSizer(pSizerAll);
  Layout();
  Fit();
}


bool CDialogLabSettings::TransferDataFromWindow()
{
  bool bRtn = true;
  if(!m_pFilenames->TransferDataFromWindow())
  {
    bRtn = false;
  }
  if(!m_pThresholds->TransferDataFromWindow())
  {
    bRtn = false;
  }
  if(!m_pMarkerSets->TransferDataFromWindow())
  {
    bRtn = false;
  }
  if(!m_pReview->TransferDataFromWindow())
  {
    bRtn = false;
  }
  return bRtn;
}
bool CDialogLabSettings::TransferDataToWindow()
{
  bool bRtn = true;
  if(!m_pFilenames->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pThresholds->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pMarkerSets->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pReview->TransferDataToWindow())
  {
    bRtn = false;
  }
  return bRtn;
}


double CDialogLabSettings::Str2Double(const wxString &_s, double dDefault)
{
  wxString s(_s);
  nwxString::Trim(&s);
  return s.IsEmpty() ? dDefault : atof(s.c_str());
}
int CDialogLabSettings::Str2Int(const wxString &_s, int nDefault)
{
  wxString s(_s);
  nwxString::Trim(&s);
  return s.IsEmpty() ? nDefault : atoi(s.c_str());
}
wxString CDialogLabSettings::Number2Str(double d, double dDefaultBlank)
{
  bool bDefault =
    (fabs(dDefaultBlank) <= 1.0E-10)
    ? (d == dDefaultBlank)
    : (fabs( (d - dDefaultBlank) / dDefaultBlank ) < 1.0E-10);
  wxString sRtn;
  if(!bDefault)
  {
    sRtn = nwxString::FormatNumber(d);
  }
  return sRtn;
}
wxString CDialogLabSettings::Number2Str(int n, int nDefaultBlank)
{
  wxString sRtn;
  if(n != nDefaultBlank)
  {
    sRtn = nwxString::FormatNumber(n);
  }
  return sRtn;
}

void CDialogLabSettings::OnPageChange(wxNotebookEvent &)
{
  size_t nPage = m_pNotebook->GetSelection();
  bool bNext = true;
  bool bBack = true;
  if(!nPage)
  {
    bBack = false;
  }
  else if((nPage + 1) == m_pNotebook->GetPageCount())
  {
    bNext = false;
  }
  m_pbBack->Enable(bBack);
  m_pbNext->Enable(bNext);
}
void CDialogLabSettings::OnNext(wxCommandEvent &)
{
  size_t nPage = m_pNotebook->GetSelection();
  ++nPage;
  if(nPage < m_pNotebook->GetPageCount())
  {
    m_pNotebook->SetSelection(nPage);
  }
}

void CDialogLabSettings::OnBack(wxCommandEvent &)
{
  size_t nPage = m_pNotebook->GetSelection();
  if(nPage > 0)
  {
    --nPage;
    m_pNotebook->SetSelection(nPage);
  }
}

BEGIN_EVENT_TABLE(CDialogLabSettings,wxDialog)
EVT_BUTTON(IDnext,CDialogLabSettings::OnNext)
EVT_BUTTON(IDback,CDialogLabSettings::OnBack)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, CDialogLabSettings::OnPageChange)
END_EVENT_TABLE()
