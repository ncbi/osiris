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
*  FileName: CPanelLabMarkerSet.cpp
*  Author:   Douglas Hoffman
*
*/
//#include "CPanelLabMarkerSet.h"
#include "CPanelLabSettings.h"
#include <wx/dialog.h>
#include <wx/sizer.h>

typedef map<wxString, size_t> mapSort;

#if 0
//********************************************************************
//
//    CDialogName
//
//     choose a name for a new item
//
class CDialogName : public wxDialog
{
public:
  CDialogName(
    wxWindow *pParent,
    INameValidator *pv,
    const wxString &sPrompt,
    const wxString &sTitle,
    const wxArrayString *paListValues = NULL,
    bool bSkipAmel = false,
    const wxString *psCurrent = NULL);
  virtual ~CDialogName();
  wxString GetName();
  virtual bool TransferDataFromWindow();
private:
  const wxString m_sPrompt;
  wxString m_sCurrent;
  wxTextCtrl *m_pText;
  wxComboBox *m_pCombo;
  INameValidator *m_pv;
};


//********************************************************************
//
//    CDialogName
//

CDialogName::~CDialogName() {;}

CDialogName::CDialogName(
  wxWindow *pParent, INameValidator *pv,
  const wxString &sPrompt,
  const wxString &sTitle,
  const wxArrayString *paListValues,
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
  size_t nCount = (paListValues == NULL) ? 0 : paListValues->GetCount();
  if(nCount > 1)
  {
    m_pCombo = new wxComboBox(
      this,wxID_ANY,wxEmptyString,
      wxDefaultPosition, wxSize(120,-1),
      *paListValues,
      wxCB_DROPDOWN | wxCB_SORT | wxCB_READONLY);
    if(bSkipAmel)
    {
      // remove AMEL
      size_t i = 0;
      while(i > 0)
      {
        --i;
        const wxString &s(paListValues->Item(i));
        if(CLabLocus::IsAmel(s))
        {
          m_pCombo->Delete((unsigned int)i);
          nCount--;
        }
      }
    }
    if(psCurrent != NULL)
    {
      // should never happen
      m_pCombo->SetStringSelection(*psCurrent);
      wxASSERT_MSG(0,*psCurrent);
    }
    pWin = m_pCombo;
  }
  if(nCount < 2)
  {
    if(m_pCombo != NULL)
    {
      // should never happen
      m_pCombo->Destroy();
      wxASSERT_MSG(0,"Combo with bSkipAmel and < 2 items");
    }
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
    sMsg = "This name is already used";
    bRtn = false;
  }
  if(!bRtn)
  {
    mainApp::ShowError(sMsg,this);
  }
  return bRtn;
}
#endif

//********************************************************************
//
//    CPanelLabLocusCollection
//
const int CPanelLabLocusCollection::GRIDROWS = 13;

CPanelLabLocusCollection::CPanelLabLocusCollection(
  wxWindow *parent, 
  int nGridRows, bool bTriallele, bool bSkipAmel) :
      nwxPanel(parent,wxID_ANY)
{
  m_pGrid = new CGridAllele(this,wxID_ANY,nGridRows,bTriallele,!bSkipAmel);
  const wxChar *ps;
  if(bTriallele)
  {
    ps = wxS("Enter three alleles per table cell");
  }
  else
  {
    ps = wxS("Enter one allele per table cell");
  }
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(new wxStaticText(this, wxID_ANY,ps),
    0,wxALIGN_LEFT);
  pSizer->Add(m_pGrid,1,wxEXPAND);
  SetSizer(pSizer);
}
CPanelLabLocusCollection::~CPanelLabLocusCollection() {;}

bool CPanelLabLocusCollection::TransferDataFromWindow()
{
  bool bRtn = m_pGrid->TransferDataFromWindow();
  return bRtn;
}
bool CPanelLabLocusCollection::TransferDataToWindow()
{
  bool bRtn = m_pGrid->TransferDataToWindow();
  return bRtn;
}

//********************************************************************
//
//    CPanelPosCtrlMarkerSet
//

CPanelPosCtrlMarkerSet::CPanelPosCtrlMarkerSet(
  wxWindow *parent) :
      nwxPanel(parent,wxID_ANY)
{
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(new wxStaticText(this,wxID_ANY,"Enter multiple alleles per table cell"),
    0, wxALIGN_LEFT);
  m_pGrid = new CGridAllelePosCtrl(this,wxID_ANY,CPanelLabLocusCollection::GRIDROWS);
  pSizer->Add(m_pGrid,1,wxEXPAND, ID_BORDER);
  SetSizer(pSizer);
}



//********************************************************************
//
//    CPanelLabMarkerSet
//

CPanelLabMarkerSet::~CPanelLabMarkerSet() {;}

CPanelLabMarkerSet::CPanelLabMarkerSet
  (wxWindow *parent) :
        nwxPanel(parent,wxID_ANY),
          m_pData(NULL)
{
  m_pChoiceBook = new nwxChoicebook(this);
  m_pPanelOffLadder = new CPanelLabLocusCollection
    (m_pChoiceBook, CPanelLabLocusCollection::GRIDROWS,false,true);
  m_pPanelSampleTrialleles = new CPanelLabLocusCollection(
    m_pChoiceBook, CPanelLabLocusCollection::GRIDROWS,
    true);
  m_pPanelCtrlTrialleles = new CPanelLabLocusCollection(
    m_pChoiceBook, CPanelLabLocusCollection::GRIDROWS,
    true);
  m_pPanelPosCtrl = new CPanelPosCtrlMarkerSet(m_pChoiceBook);
  m_pChoiceBook->AddPage(m_pPanelOffLadder,"Off ladder alleles",true);
  m_pChoiceBook->AddPage(m_pPanelSampleTrialleles,"Sample trialleles");
  m_pChoiceBook->AddPage(m_pPanelCtrlTrialleles,"Positive control trialleles");
  m_pChoiceBook->AddPage(m_pPanelPosCtrl,"Positive controls");
  m_pChoiceBook->SetCaption("Select Allele Assignments:");
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pChoiceBook,1,wxEXPAND ^ wxALL, ID_BORDER);

  SetSizer(pSizer);
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
  if(!m_pPanelCtrlTrialleles->TransferDataFromWindow())
  {
    bRtn = false;
  }
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
  if(!m_pPanelCtrlTrialleles->TransferDataToWindow())
  {
    bRtn = false;
  }
  if(!m_pPanelPosCtrl->TransferDataToWindow())
  {
    bRtn = false;
  }
  return bRtn;
}

