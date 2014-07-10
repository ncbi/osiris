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

