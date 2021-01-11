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

*  FileName: CPanelLabSettings.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "Platform.h"
#include "CILSLadderInfo.h"
#include <wx/arrstr.h>
#include <wx/settings.h>
#include "CPanelLabSettings.h"
#include "nwx/nwxString.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxIntValidator.h"
#include "nwx/nwxDoubleValidator.h"
#include "nwx/nwxBookCtrlFocus.h"
#include "nwx/stdb.h"
#include <memory>
#include <set>
#include "nwx/stde.h"
#include "CParmOsiris.h"
#include "CVolumes.h"
#include "CKitList.h"
#include <stdlib.h>
#include "nwx/nwxLog.h"

const wxString CPanelLabSettings::g_sFileNameStrPrompt
  ("File name search criteria");
const wxString CPanelLabSettings::g_sSampleNameStrPrompt
  ("Sample name search criteria");
const wxSize CPanelLabSettings::g_SIZE_NUMBER(wxSize(60,-1));
const wxSize CPanelLabSettings::g_SIZE_MULTILINE_TEXT(wxSize(300,200));
wxSize CPanelLabSettings::g_SIZE_FRAME(wxSize(0,760));

//********************************************************************
//
//    nwxChoiceButtons
//
const wxChar * const nwxChoiceButtons::LABEL_ADD(wxS("New..."));
const wxChar * const nwxChoiceButtons::LABEL_REMOVE(wxS("Delete..."));
const wxChar * const nwxChoiceButtons::LABEL_RENAME(wxS("Rename..."));

IMPLEMENT_CLASS(nwxPanel,wxPanel)

wxButton *nwxChoiceButtons::_CreateButton(
  wxWindow *parent,
  wxWindowID nID,
  const wxString &sItemName,
  const wxChar *psLabel,
  const wxChar *prefix,
  bool bReadOnly,
  wxSizer *pSizer)
{
  wxButton *pRtn = new wxButton(
    parent,nID,psLabel,
    wxDefaultPosition,wxDefaultSize,
    wxBU_EXACTFIT);
  wxString sToolTip(prefix);
  if(sItemName.IsEmpty())
  {
    sToolTip.Append("item");
  }
  else
  {
    sToolTip.Append(sItemName);
  }
  pRtn->SetToolTip(sToolTip);
  pRtn->Enable(!bReadOnly);
  if(pSizer != NULL)
  {
    pSizer->Add(
      pRtn,0,wxLEFT | wxALIGN_CENTER_VERTICAL,
      ID_BORDER);
  }
  return pRtn;
}


//********************************************************************
//
//    nwxChoicebook
//
#if 0
// can't find references so commenting out 2/5/2020
void nwxChoicebook::SetupButtons(const wxString &sItemName, bool bRename)
{
  wxSizer *pBookSizer = GetControlSizer();

  m_pButtonAdd = CreateAddButton(sItemName,pBookSizer);
  m_pButtonRemove = CreateRemoveButton(sItemName,pBookSizer);
  if(bRename)
  {
    m_pButtonRename = CreateRenameButton(sItemName,pBookSizer);
  }
}
#endif
bool nwxChoicebook::TransferDataFromWindow()
{
  size_t n = GetPageCount();
  bool bRtn = true;
  for(size_t i = 0; i < n; i++)
  {
    if(!GetPage(i)->TransferDataFromWindow())
    {
#ifdef TMP_DEBUG
      nwxLog::LogMessageV(wxS("nwxChoicebook::TransferDataFromWindow SetSelection(%d)"),i);
#endif
      SetSelection(i);
      bRtn = false;
      i = n; // break
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
void nwxChoicebook::SetReadOnly(bool b)
{
  m_bReadOnly = b;
  if(m_pButtonAdd != NULL)
  {
    m_pButtonAdd->Enable(!b);
  }
  if(m_pButtonRemove != NULL)
  {
    m_pButtonRemove->Enable(!b);
  }
  if(m_pButtonRename != NULL)
  {
    m_pButtonRename->Enable(!b);
  }
  size_t n = GetPageCount();
  for(size_t i = 0; i < n; i++)
  {
    nwxPanel *pr = (nwxPanel *)GetPage(i);
    pr->SetReadOnly(b);
  }
}

void nwxChoicebook::SetCaption(const wxString &s)
{
  wxSizer *pSizer = GetControlSizer();
  if(m_pLabelCaption == NULL)
  {
    m_pLabelCaption = new wxStaticText(this,wxID_ANY,s);
    mainApp::SetBoldFont(m_pLabelCaption);
    pSizer->Insert(0,m_pLabelCaption,
      0,wxALIGN_CENTRE_VERTICAL | wxRIGHT, ID_BORDER);
  }
  else
  {
    m_pLabelCaption->SetLabel(s);
    pSizer->Layout();
  }
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

#if 0
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
}
void CPanelAlleleGrid::CreateAlleleGrid(bool bIsAmel)
{
  m_pGrid = new nwxGrid(this,wxID_ANY);
  m_pGrid->CreateGrid(m_nGridRows,1);
  m_pGrid->SetRowLabelSize(1);
  m_pGrid->SetColLabelValue(0,"Allele");
  for(int i = 0; i < m_nGridRows; i++)
  {
    m_pGrid->SetRowLabelValue(i,wxEmptyString);
  }
  m_pGrid->EnableScrolling(true,true);
  if(!bIsAmel)
  {
    m_pGrid->SetDefaultEditor(new wxGridCellFloatEditor(-1,1));
  }
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
}
void CPanelTrialleleGrid::CreateAlleleGrid(bool bIsAmel)
{
  m_pGrid = new nwxGrid(this,wxID_ANY);
  m_pGrid->CreateGrid(m_nGridRows,3);
  m_pGrid->SetRowLabelSize(1);
  m_pGrid->SetColLabelValue(0,"Allele 1");
  m_pGrid->SetColLabelValue(1,"Allele 2");
  m_pGrid->SetColLabelValue(2,"Allele 3");
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

#endif



//********************************************************************
//
//    CPanelLabSettings
//
const wxSize &CPanelLabSettings::GetDefaultFrameSize()
{
  if (g_SIZE_FRAME.GetWidth() < 1)
  {
    wxRect r = wxGetClientDisplayRect();
    g_SIZE_FRAME.SetWidth((r.GetWidth() << 2) / 5);
  }
  return g_SIZE_FRAME;
}


CPanelLabSettings::~CPanelLabSettings() {;}

CPanelLabSettings::CPanelLabSettings(
  wxWindow *parent, bool bReadOnly, bool bShowCancel) :
  nwxPanel(parent,wxID_ANY),
    m_pData(NULL),
    m_pMessageBook(NULL),
    m_pbLock(NULL),
    m_pbApply(NULL),
    m_bReadOnly(bReadOnly),
    m_bAlwaysReadOnly(bReadOnly)
{

  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  m_pbBack = new wxButton(
    this,IDback,"< &Back",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pbNext = new wxButton(
    this,IDnext,"&Next >",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  if(!m_bAlwaysReadOnly)
  {
    m_pbLock = new wxButton(
      this,IDlock,"Lock",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
  }

  wxButton *pbOK = new wxButton(
    this,wxID_OK,"&OK",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  wxButton *pbCancel = NULL;
  if(bShowCancel || !m_bAlwaysReadOnly)
  {
    pbCancel = new wxButton(
      this,wxID_CANCEL,"&Cancel",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
  }
  if(!m_bAlwaysReadOnly)
  {
    m_pbApply = new wxButton(
      this,wxID_APPLY,"&Apply",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
  }
  m_pNotebook = new wxNotebook(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxNB_TOP);
  m_pGeneral = new CPanelLabGeneral(m_pNotebook);
  m_pFilenames = new CPanelLabNameStrings(
    m_pNotebook);
  m_pMarkerSet = new CPanelLabMarkerSet(
    m_pNotebook); 
  m_pSampleThresholds = new CPanelLabSampleThresholds(
    m_pNotebook,wxID_ANY);
  m_pLocusThresholds = new CPanelLabLocusThresholds(
    m_pNotebook,wxID_ANY);
  m_pReview = new CPanelLabReviewAcceptance(
    m_pNotebook);
  m_pNotebook->AddPage(
      m_pGeneral,"General",true);
  m_pNotebook->AddPage(
    m_pFilenames,"File/Sample Names",false);
  m_nLocusThresholds = m_pNotebook->GetPageCount();
  m_pNotebook->AddPage(
    m_pLocusThresholds,"Thresholds",false);
  m_pNotebook->AddPage(
    m_pSampleThresholds,"Sample Limits",false);
  m_pNotebook->AddPage(
    m_pMarkerSet,"Assignments",false);
  m_pNotebook->AddPage(
    m_pReview,"Accept/Review",false);
  wxBoxSizer *pSizerAll(new wxBoxSizer(wxVERTICAL));
  wxBoxSizer *pSizerButtons(new wxBoxSizer(wxHORIZONTAL));

  pbOK->SetDefault();

  pSizerButtons->Add(m_pbBack,0,wxRIGHT,ID_BORDER);
  pSizerButtons->Add(m_pbNext,0,wxRIGHT,ID_BORDER);
  if(!m_bAlwaysReadOnly)
  {
    pSizerButtons->Add(m_pbLock,0,wxRIGHT,ID_BORDER);
  }
  pSizerButtons->AddStretchSpacer(1);
  if(m_bAlwaysReadOnly)
  {
    pSizerButtons->Add(pbOK,0,0,0);
    if(pbCancel != NULL)
    {
      pSizerButtons->Add(pbCancel,0,wxLEFT,ID_BORDER);
    }
  }
  else
  {
    // put space after cancel button
    pSizerButtons->Add(pbOK,0,wxRIGHT,ID_BORDER);
    pSizerButtons->Add(pbCancel,0,wxRIGHT,ID_BORDER);
    pSizerButtons->Add(m_pbApply,0,0,0);
  }
  pSizerAll->Add(m_pNotebook,1,(wxALL ^ wxBOTTOM) | wxEXPAND, ID_BORDER);
  pSizerAll->Add
    (pSizerButtons,0, 
     wxALL | wxEXPAND, 
     ID_BORDER);
  m_pNotebook->SetFocus();
  SetSizer(pSizerAll);
  SetReadOnly(true);  // read only until data is sent
}

bool CPanelLabSettings::TransferDataFromWindow()
{
#if 1
  bool bRtn = true;
  if(!IsReadOnly())
  {
    size_t n = m_pNotebook->GetPageCount();
    for(size_t i = 0; i < n; i++)
    {
      if(!m_pNotebook->GetPage(i)->TransferDataFromWindow())
      {
        if(m_pNotebook->GetSelection() != int(i))
        {
#ifdef TMP_DEBUG
          nwxLog::LogMessageV(wxS("nwxChoicebook::TransferDataFromWindow SetSelection(%d)"),i);
#endif
          m_pNotebook->SetSelection(i);
        }
        bRtn = false;
        i = n; // break
      }
    }
  }
  return bRtn;
#else
  bool bRtn = true;
  if(!IsReadOnly())
  {
    if(!m_pGeneral->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pFilenames->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pSampleThresholds->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pLocusThresholds->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pMarkerSet->TransferDataFromWindow())
    {
      bRtn = false;
    }
    if(!m_pReview->TransferDataFromWindow())
    {
      bRtn = false;
    }
  }
  return bRtn;
#endif
}
bool CPanelLabSettings::TransferDataToWindow()
{
  bool bRtn = true;
  if(!m_pGeneral->TransferDataToWindow())
  {
    bRtn = false;
  }
  else if(!m_pFilenames->TransferDataToWindow())
  {
    bRtn = false;
  }
  else if(!m_pSampleThresholds->TransferDataToWindow())
  {
    bRtn = false;
  }
  else if(!m_pLocusThresholds->TransferDataToWindow())
  {
    bRtn = false;
  }
  else if(!m_pMarkerSet->TransferDataToWindow())
  {
    bRtn = false;
  }
  else if(!m_pReview->TransferDataToWindow())
  {
    bRtn = false;
  }
  return bRtn;
}
void CPanelLabSettings::SetReadOnly(bool b)
{
  if(m_bAlwaysReadOnly) { b = true; }
  m_bReadOnly = b;
  if(m_pbApply != NULL)
  {
    m_pbApply->Enable(!b);
  }
  m_pGeneral->SetReadOnly(b);
  m_pSampleThresholds->SetReadOnly(b);
  m_pLocusThresholds->SetReadOnly(b);
  m_pFilenames->SetReadOnly(b);
  m_pMarkerSet->SetReadOnly(b);
  m_pReview->SetReadOnly(b);
}

bool CPanelLabSettings::SetData(
  CLabSettings *pData, 
  const CXMLmessageBook *pBook,
  bool bReadOnly)
{
  bool bRtn = true;
  CLabMarkerSetCollection *pMarker = pData->GetMarkerCollection();
  if(pMarker == NULL)
  {
    wxString sMsg("No marker sets in lab settings file: ");
    sMsg.Append(pData->GetLastFileName());
    wxASSERT_MSG(0,sMsg);
    mainApp::LogMessage(sMsg);
    bRtn = false;
  }
  else
  {
    if(m_pbLock != NULL)
    {
      m_pbLock->Enable(false);
    }
    m_pMessageBook = pBook;
    m_pData = pData;
    m_pGeneral->SetData(pData->GetLabSettingsInfo(),pMarker);
    m_pSampleThresholds->SetData(pData->GetThresholds(),pBook);
    if(!m_pLocusThresholds->SetData(pData->GetThresholds(),
      pData->GetKitName(),_GetILSDyeName()))
    {
      bRtn = false;
    }
    m_pFilenames->SetData(pData->GetLabStrings());
    m_pReview->SetData(pData->GetReviewAcceptance());
    m_pMarkerSet->SetData(pMarker);
    SetReadOnly(bReadOnly);
  }
  return bRtn;
}
bool CPanelLabSettings::SetupAll(CVolume *pVolume)
{
  bool bRtn;
  pVolume->CheckReload();
  bRtn = pVolume->IsOK();
  if(bRtn)
  {
    if( (pVolume->GetLabSettings() == m_pData) &&
      (pVolume->GetMessageBook() == m_pMessageBook) )
    {
      SetReadOnly(!pVolume->HasLock());
    }
    else
    {
      bRtn = SetData(
          pVolume->GetLabSettings(),
          pVolume->GetMessageBook(),
          !pVolume->HasLock());
    }
    if(bRtn)
    {
      bRtn = TransferDataToWindow();
    }
    if(m_pbLock != NULL)
    {
      m_pbLock->Enable(!( pVolume->IsReadOnly() || pVolume->HasLock() ));
    }
  }
  return bRtn;
}

double CPanelLabSettings::Str2Double(const wxString &_s, double dDefault)
{
  wxString s(_s);
  nwxString::Trim(&s);
  return s.IsEmpty() ? dDefault : atof(s.utf8_str());
}
int CPanelLabSettings::Str2Int(const wxString &_s, int nDefault)
{
  wxString s(_s);
  nwxString::Trim(&s);
  return s.IsEmpty() ? nDefault : atoi(s.utf8_str());
}
wxString CPanelLabSettings::Number2Str(double d, double dDefaultBlank)
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
wxString CPanelLabSettings::Number2Str(int n, int nDefaultBlank)
{
  wxString sRtn;
  if(n != nDefaultBlank)
  {
    sRtn = nwxString::FormatNumber(n);
  }
  return sRtn;
}
wxString CPanelLabSettings::_GetILSDyeName()
{
  wxString sDyeName;
  if(m_pData == NULL)
  {
    mainApp::LogMessage(wxT("CPanelLabSettings::_GetILSDyeName - m_pData == NULL"));
  }
  else
  {
    wxString sKitName = m_pData->GetKitName();
    CPersistKitList *pKitList = mainApp::GetKitList();
    bool bFam = pKitList->ILSDyeNamesByFamily(sKitName);
    const wxChar * const NO_DYE_NAME =
      wxT("Cannot find ILS Dye name for kit: ");
    if(!bFam)
    {
      unsigned int nChannel = pKitList->GetILSchannelNumber(sKitName);
      const CSingleKitColors *pKC = mainApp::GetKitColors()->GetKitColors(sKitName);
      const CChannelColors *pCC =
          (pKC == NULL) ? NULL : pKC->GetColorChannel(nChannel);
      if(pCC != NULL)
      {
        sDyeName = pCC->GetDyeName();
      }
      if(sDyeName.IsEmpty())
      {
        wxString sErr;
        if(pKC == NULL)
        {
          sErr = wxT("Cannot find kit colors for kit: ");
          sErr.Append(sKitName);
          mainApp::LogMessage(sErr);
        }
        sErr = NO_DYE_NAME;
        sErr.Append(sKitName);
        sErr.Append(wxT(", trying ILSAndLadderInfo.xml"));
        mainApp::LogMessage(sErr);
      }
    }
    if(bFam || sDyeName.IsEmpty())
    {
      wxString sILS = m_pGeneral->GetILS();
      CILSLadderInfo *pILS = mainApp::GetILSLadderInfo();
      sDyeName = pILS->GetDyeNameFromLS(sILS);
      if(sDyeName.IsEmpty())
      {
        wxString sErr(NO_DYE_NAME);
        sErr.Append(sKitName);
        mainApp::LogMessage(sErr);
      }
    }
  }
  return sDyeName;
}
void CPanelLabSettings::OnPageChange(wxNotebookEvent &)
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
  if(nPage == m_nLocusThresholds)
  {
    if(UpdateILSDyeName())
    {
      Layout();
    }
  }
  m_pbBack->Enable(bBack);
  m_pbNext->Enable(bNext);
}
void CPanelLabSettings::OnNext(wxCommandEvent &)
{
  size_t nPage = m_pNotebook->GetSelection();
  ++nPage;
  if(nPage < m_pNotebook->GetPageCount())
  {
    m_pNotebook->SetSelection(nPage);
  }
}

void CPanelLabSettings::OnBack(wxCommandEvent &)
{
  size_t nPage = m_pNotebook->GetSelection();
  if(nPage > 0)
  {
    --nPage;
    m_pNotebook->SetSelection(nPage);
  }
}


BEGIN_EVENT_TABLE(CPanelLabSettings,nwxPanel)
EVT_BUTTON(IDnext,CPanelLabSettings::OnNext)
EVT_BUTTON(IDback,CPanelLabSettings::OnBack)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, CPanelLabSettings::OnPageChange)
END_EVENT_TABLE()

