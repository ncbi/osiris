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
*  FileName: CPanelLabNameStrings.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CPanelLabSettings.h"
#include "nwx/nwxBookCtrlFocus.h"


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
        wxDefaultPosition, CPanelLabSettings::g_SIZE_MULTILINE_TEXT,
        wxTE_MULTILINE | wxTE_DONTWRAP | wxHSCROLL);
    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    pSizer->Add(m_pTextCtrl,1,wxEXPAND);
    SetSizer(pSizer);
    m_pTextCtrl->SetMinSize(CPanelLabSettings::g_SIZE_MULTILINE_TEXT);
  }
  wxTextCtrl *m_pTextCtrl;
};


//********************************************************************
//
//    CPanelLabNameStrings
//

CPanelLabNameStrings::~CPanelLabNameStrings() {;}

CPanelLabNameStrings::CPanelLabNameStrings(
  wxWindow *parent)
  : nwxPanel(parent), m_pTextStdCtrlName(NULL), m_pData(NULL)
{
  wxArrayString as;
  as.Alloc(2);
  as.Add(_T("File name"));
  as.Add(_T("Sample name"));

  this->m_pRadioNameType = new wxRadioBox(
    this,wxID_ANY,_T("Search by:"),wxDefaultPosition, wxDefaultSize,as);
  m_pChoiceBook = new nwxChoicebook(this);
  nwxTextPanel *pPanel = new nwxTextPanel(m_pChoiceBook,wxID_ANY);
  m_pTextLadder = pPanel->GetTextCtrl();
  m_pChoiceBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_LADDER),true);

  // get count immediately before adding positive control

  m_nPosCtrlNdx = (int)m_pChoiceBook->GetPageCount();
  pPanel = new nwxTextPanel(m_pChoiceBook,wxID_ANY);
  m_pTextPos = pPanel->GetTextCtrl();
  m_pChoiceBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_POS_CONTROL));

  pPanel = new nwxTextPanel(m_pChoiceBook,wxID_ANY);
  m_pTextNeg = pPanel->GetTextCtrl();
  m_pChoiceBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_NEG_CONTROL));

  pPanel = new nwxTextPanel(m_pChoiceBook,wxID_ANY);
  m_pTextPossibleMixture = pPanel->GetTextCtrl();
  m_pChoiceBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_POSSIBLE_MIXTURE));

  pPanel = new nwxTextPanel(m_pChoiceBook,wxID_ANY);
  m_pTextSingleSource = pPanel->GetTextCtrl();
  m_pChoiceBook->AddPage(pPanel,_T(CLabNameStrings::TYPE_SINGLE_SOURCE));

  //  2/11/09 added specimen types

  wxString sType;
  size_t nTYPES = CLabSpecimenCategory::TypeCount();
  size_t i;
  m_pvTextSpecimenTypes.reserve(nTYPES);
  for(i = 0; i < nTYPES; ++i)
  {
    sType = _T(CLabSpecimenCategory::TYPES[i]);
    pPanel = new nwxTextPanel(m_pChoiceBook, wxID_ANY);
    m_pvTextSpecimenTypes.push_back(pPanel->GetTextCtrl());
    m_pChoiceBook->AddPage(pPanel,sType);
  }

  //  2/11/09

  m_pTextStdCtrlName = new wxTextCtrl(this,wxID_ANY);
  m_pLabelNameString = new wxStaticText(this,wxID_ANY,
    CPanelLabSettings::g_sFileNameStrPrompt);
  m_pLabelStdCtrlName = new wxStaticText(this,wxID_ANY,_T("Standard Control Name"));
  m_pSizer = new wxBoxSizer(wxVERTICAL);

  m_pSizer->Add(this->m_pRadioNameType,0, wxALL | wxALIGN_LEFT,ID_BORDER);
  m_pSizer->Add(m_pLabelNameString,0, (wxALL ^ wxTOP) | wxALIGN_LEFT, ID_BORDER);
  m_pSizer->Add(m_pChoiceBook,1,wxEXPAND | (wxALL ^ wxTOP),ID_BORDER);

  m_pSizer->Add(m_pLabelStdCtrlName,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  m_pSizer->Add(m_pTextStdCtrlName,0,wxEXPAND | (wxALL ^ wxTOP),ID_BORDER);
  SetSizer(m_pSizer);

  OnChangeCombo(*( (wxChoicebookEvent *) NULL ));
}


#define NAME_STRING_ARRAYS                    \
    wxString s;                               \
    const size_t COUNT = 5;                   \
    wxTextCtrl *ptext[COUNT] =                \
    {                                         \
      m_pTextLadder,                          \
      m_pTextPos,                             \
      m_pTextNeg,                             \
      m_pTextPossibleMixture,                 \
      m_pTextSingleSource                     \
    };                                        \
    CLabSynonym *pSyn[COUNT] =                \
    {                                         \
      &(m_pData->m_vsSynonymLadder),          \
      &(m_pData->m_vsSynonymPosCtrl),         \
      &(m_pData->m_vsSynonymNegCtrl),         \
      &(m_pData->m_vsSynonymPossibleMixture), \
      &(m_pData->m_vsSynonymSingleSource)     \
    };

#define ALLOW_EMPTY(i) ((i) > 2)

bool CPanelLabNameStrings::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    NAME_STRING_ARRAYS
    size_t i;
    for(i = 0; i < COUNT; i++)
    {
      s = ptext[i]->GetValue();
      vector<wxString> *pvs(pSyn[i]->GetVectorPtr());
      if(ALLOW_EMPTY(i))
      {}
      else if(!nwxString::SplitLines(s.c_str(),pvs,true,true))
      {
        mainApp::ShowError(
          _T("Please enter at least one identifier"),this);
        nwxBookCtrlFocus::Focus(ptext[i]);
        i = COUNT; // loop exit
        bRtn = false;
      }
    }
    m_pData->SetUseSampleName(!!m_pRadioNameType->GetSelection());
    m_pData->m_sStdCtrlName = m_pTextStdCtrlName->GetValue();

    // Specimen Category

    size_t nTYPES = CLabSpecimenCategory::TypeCount();
    CLabSetSpecimenCategory &setSpec(m_pData->m_setSpecimenCategory);
    for(i = 0; i < nTYPES; i++)
    {
      s = m_pvTextSpecimenTypes.at(i)->GetValue();
      setSpec.SetSynonyms(i,s);
    }
  }
  return bRtn;
}
bool CPanelLabNameStrings::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    NAME_STRING_ARRAYS
    size_t i;
    for(i = 0; i < COUNT; i++)
    {
      nwxString::Join(pSyn[i]->GetVector(),&s,"\n");
      nwxString::FixEOLU(&s,true);
      ptext[i]->SetValue(s);
    }
    m_pRadioNameType->SetSelection(m_pData->UseSampleName() ? 1 : 0);
    SyncPrompt();
    m_pTextStdCtrlName->SetValue(m_pData->m_sStdCtrlName);

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
  }
  return bRtn;
}

void CPanelLabNameStrings::OnChangeCombo(wxChoicebookEvent &)
{
  if(m_pTextStdCtrlName != NULL)
  {
    int nSelect = m_pChoiceBook->GetSelection();
    bool bShow = (nSelect == m_nPosCtrlNdx);
    m_pSizer->Show(m_pTextStdCtrlName,bShow);
    m_pSizer->Show(m_pLabelStdCtrlName,bShow);
    m_pSizer->Layout();
  }
}

void CPanelLabNameStrings::SyncPrompt()
{
  m_pLabelNameString->SetLabel(
    CPanelLabSettings::NameStrPrompt(
      !!m_pRadioNameType->GetSelection()
    ));
}

void CPanelLabNameStrings::OnChangeRadio(wxCommandEvent &)
{
  SyncPrompt();
}
BEGIN_EVENT_TABLE(CPanelLabNameStrings, wxPanel)
EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY,CPanelLabNameStrings::OnChangeCombo)
EVT_RADIOBOX(wxID_ANY, CPanelLabNameStrings::OnChangeRadio)
END_EVENT_TABLE()

