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
*  FileName: CGridAllele.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CGridAllele.h"
#include "CLabSettings.h"
#include "nwx/nwxRound.h"
#include <wx/msgdlg.h>

const wxChar * const CGridAlleleBase::TRIALLELE_FIT(wxS("00.0, 00.0, 00.0"));
const wxChar * const CGridAlleleBase::ALLELE_FIT(wxS("000.0"));

//***********************************************  CGridAllele

bool CGridAllele::TransferDataToWindow()
{
  Clear();
  bool bRtn = m_bTriAlleles
    ? _TransferDataToWindow3()
    : _TransferDataToWindow1();
  if(bRtn)
  {
    CheckRowCount();
  }
  return bRtn;
}
bool CGridAllele::TransferDataFromWindow()
{
  vector<CLabLocus *> *pvpLocus = m_pData->Get();
  bool bRtn = m_bTriAlleles
    ? _TransferDataFromWindow3(pvpLocus)
    : _TransferDataFromWindow1(pvpLocus);
  return bRtn;
}

bool CGridAllele::_TransferDataToWindow1()
{
#ifdef __WXDEBUG__
  set<wxString,nwxStringLessNoCase> ssInvalid;
  set<wxString,nwxStringLessNoCase>::iterator itrInvalid;
#endif
  CLabLocusCollection::iterator itr;
  CLabLocus *pLocus;
  bool bRtn = true;
  int nRowCount = GetNumberRows();
  int nCol;
  int nAlleles;
  int i;
  int nNeeded;
  for(itr = m_pData->begin();
    itr != m_pData->end();
    ++itr)
  {
    pLocus = *itr;
    const wxString &sName(pLocus->GetName());
    nCol = _GetLocusColumn(sName);
    if(nCol < 0)
    {
#ifdef __WXDEBUG__
      ssInvalid.insert(sName);
#endif
      mainApp::LogMessageV(
        wxS("CGridAllele::TransferDataToWindow1() invalid locus %ls"),
        sName.wc_str());
    }
    else
    {
      int &nRow = m_vnLastRowUsed[nCol];
      nAlleles = (int) pLocus->GetAlleleCount();
      nNeeded = nRow + nAlleles;
      if(nNeeded >= nRowCount)
      {
        nRowCount = nNeeded + 3;
        nwxGrid::SetRowCount(this,nRowCount);
      }
      for(i = 0; i < nAlleles; i++)
      {
        nRow++;
        SetCellValue(nRow,nCol,pLocus->GetAllele(i));
      }
    }
  }
#ifdef __WXDEBUG__
  if(ssInvalid.size())
  {
    wxString sError;
    sError.reserve(ssInvalid.size() << 4);
    sError = "Invalid locus:\n";
    for(itrInvalid = ssInvalid.begin();
      itrInvalid != ssInvalid.end();
      ++itrInvalid)
    {
      sError.Append("\n");
      sError.Append(*itrInvalid);
    }
    wxASSERT_MSG(0,sError);
  }
#endif
  return bRtn;
}
bool CGridAllele::_TransferDataToWindow3()
{
#ifdef __WXDEBUG__
  set<wxString,nwxStringLessNoCase> ssInvalid;
  set<wxString,nwxStringLessNoCase>::iterator itrInvalid;
#endif
  wxString sCell;
  CLabLocusCollection::iterator itr;
  CLabLocus *pLocus;
  int nRowCount = GetNumberRows();
  int nCol;
  bool bRtn = true;
  for(itr = m_pData->begin();
    itr != m_pData->end();
    ++itr)
  {
    pLocus = *itr;
    const wxString &sName(pLocus->GetName());
    nCol = _GetLocusColumn(sName);
    if(nCol < 0)
    {
#ifdef __WXDEBUG__
      ssInvalid.insert(sName);
#endif
      mainApp::LogMessageV(
			  wxS("CGridAllele::TransferDataToWindow1() invalid locus %ls"),
        sName.wc_str());
    }
    else
    {
      int &nRow = m_vnLastRowUsed[nCol];
      nRow++;
      if(nRow >= nRowCount)
      {
        nRowCount <<= 1;
        nwxGrid::SetRowCount(this,nRowCount);
      }
      nwxString::Join(*pLocus->GetAlleles(),&sCell,", ");
      SetCellValue(nRow,nCol,sCell);
    }
  }
#ifdef __WXDEBUG__
  if(ssInvalid.size())
  {
    wxString sError;
    sError.reserve(ssInvalid.size() << 4);
    sError = "Invalid locus:\n";
    for(itrInvalid = ssInvalid.begin();
      itrInvalid != ssInvalid.end();
      ++itrInvalid)
    {
      sError.Append("\n");
      sError.Append(*itrInvalid);
    }
    wxASSERT_MSG(0,sError);
  }
#endif
  return bRtn;
}

bool CGridAllele::_TransferDataFromWindow1(vector<CLabLocus *> *pvpLocus)
{
  wxString sLocusName;
  wxString sAllele;

  vector<CLabLocus *> vpLocus;
  int nRows = GetNumberRows();
  int nCols = GetNumberCols();
  int nRow;
  int nCol;
  size_t nSize = pvpLocus->size();
  bool bAmel;
  vpLocus.reserve((nSize + 2) << 1);
  CLabLocus *plc;
  for(nCol = 0; nCol < nCols; nCol++)
  {
    plc = NULL;
    sLocusName = GetColLabelValue(nCol);
    bAmel = CLabLocus::IsAmel(sLocusName);
    for(nRow = 1; nRow < nRows; nRow++)
    {
      sAllele = GetCellValueTrimmed(nRow,nCol);
      if(CLabLocus::ValidateAllele(&sAllele,bAmel))
      {
        if(plc == NULL)
        {
          plc = _NewLabLocus(pvpLocus,sLocusName);
          vpLocus.push_back(plc);
          plc->SetName(sLocusName);
        }
        plc->AddAllele(sAllele);
      }
    }
    if(plc != NULL)
    {
      plc->SortAlleles();
    }
  }
  vectorptr<CLabLocus>::cleanup(pvpLocus);
  *pvpLocus = vpLocus;
  return true;
}

bool CGridAllele::_TransferDataFromWindow3(
  vector<CLabLocus *> *pvpLocus,
  int nStartCol, bool bRequire3)
{
  wxString sLocusName;
  wxString sAllele;
  vector<CLabLocus *> vpLocus;
  int nRows = GetNumberRows();
  int nCols = GetNumberCols();
  int nRow;
  int nCol;
  size_t nSize = pvpLocus->size();
  bool bRtn = true;
  vpLocus.reserve((nSize + 2) << 1);
  CLabLocus *plc;
  for(nCol = nStartCol; nCol < nCols; nCol++)
  {
    sLocusName = GetColLabelValue(nCol);
    for(nRow = 1; nRow < nRows; nRow++)
    {
      sAllele = GetCellValueTrimmed(nRow,nCol);
      if(sAllele.IsEmpty()) {}
      else if(
        (plc = _BuildLabLocus(
          pvpLocus,
          sLocusName,
          sAllele,
          bRequire3)) != NULL)
      {
        vpLocus.push_back(plc);
      }
      else
      {
        bRtn = false;
      }
    }
  }
  vectorptr<CLabLocus>::cleanup(pvpLocus);
  *pvpLocus = vpLocus;
  return bRtn;
}

//***********************************************  CGridAllelePosCtrl


CLabPositiveControl *CGridAllelePosCtrl::_GetPosCtrl()
{
  CLabPositiveControl *pRtn(NULL);
  vector<CLabPositiveControl *> *pvpCtrl = m_pData->Get();
  if(pvpCtrl->empty())
  {
    pRtn = new CLabPositiveControl;
  }
  else
  {
    pRtn = *(pvpCtrl->rbegin());
    pvpCtrl->pop_back();
  }
  return pRtn;
}


bool CGridAllelePosCtrl::TransferDataFromWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    wxString sName;
    wxString sFileString;
    wxString sAlleles;
    vector<wxString> vsAlleles;
    vector<CLabLocus *> vpNewLoci;
    vector<CLabPositiveControl *> vpNewPosCtrl;

    vector<CLabLocus *> *pvpLoci(NULL);
    CLabPositiveControl *pCtrl;
    CLabLocus *pLocus;

    int nRows = GetNumberRows();
    int nRow;
    int nCols = GetNumberCols();
    int nCol;

    for(nRow = 1; nRow < nRows; nRow++)
    {
      sName = GetCellValueTrimmed(nRow,0);
      if(!sName.IsEmpty())
      {
        sFileString = GetCellValueTrimmed(nRow,1);
        pCtrl = NULL;
        for(nCol = 2; nCol < nCols; nCol++)
        {
          sAlleles = GetCellValueTrimmed(nRow,nCol);
          if(!sAlleles.IsEmpty())
          {
            if(CLabLocus::BuildList(
              sAlleles,&vsAlleles,nCol == m_nAmelColumn))
            {
              if(pCtrl == NULL)
              {
                pCtrl = _GetPosCtrl();
                pCtrl->SetName(sName);
                pCtrl->SetFileNameString(sFileString);
                vpNewPosCtrl.push_back(pCtrl);
                pvpLoci = pCtrl->Get();
                vpNewLoci.clear();
              }
              pLocus = _NewLabLocus(pvpLoci,GetColLabelValue(nCol)); // recycle loci or create new
              pLocus->SetAlleles(vsAlleles);
              vpNewLoci.push_back(pLocus);
            }
          }
        } //end nCol loop
        if(pCtrl != NULL)
        {
          // clean up remaining old loci in this positive control
          // if any
          vectorptr<CLabLocus>::cleanup(pvpLoci);
          // copy the new loci to the new positive control
          (*pvpLoci) = vpNewLoci; // copy pointers
          vpNewLoci.clear();
        }
      }
    } // end nRow loop
    m_pData->SetPositiveControls(&vpNewPosCtrl);
  }
  return bRtn;
}
bool CGridAllelePosCtrl::TransferDataToWindow()
{
  bool bRtn = (m_pData != NULL);
  if(bRtn)
  {
    wxString sAlleles;
#ifdef __WXDEBUG__
    wxString sErrors;
#endif
    vector<CLabPositiveControl *>::iterator itrCtrl;
    vector<CLabLocus *>::iterator itrLocus;
    vector<CLabPositiveControl *> *vpCtrl = m_pData->Get();
    vector<CLabLocus *> *vpLoci;

    CLabPositiveControl *pCtrl;
    int nMinSize = (int)vpCtrl->size() + 2;
    int nRowCount = GetNumberRows();
    int nRow = 1;
    int nCol;

    if(nMinSize > nRowCount)
    {
      SetRowCount(nMinSize);
      nRowCount = nMinSize;
    }
    Clear();
    nRow = 1;
    for(itrCtrl = vpCtrl->begin(); itrCtrl != vpCtrl->end(); ++itrCtrl)
    {
      pCtrl = *itrCtrl;
      vpLoci = (*itrCtrl)->Get();
      if((vpLoci != NULL) && !vpLoci->empty())
      {
        _SetRowEnabled(nRow,true);
        SetCellValue(nRow,0,pCtrl->GetName());
        SetCellValue(nRow,1,pCtrl->GetFileNameString());
        for(itrLocus = vpLoci->begin();
          itrLocus != vpLoci->end();
          ++itrLocus)
        {
          const wxString &sName((*itrLocus)->GetName());
          nCol = _GetLocusColumn(sName);
          if(nCol > 0)
          {
            sAlleles = (*itrLocus)->FormatAlleleList();
            SetCellValue(nRow,nCol,sAlleles);
          }
#ifdef __WXDEBUG__
          else
          {
            sErrors.Append("\nCannot find locus column: ");
            sErrors.Append(sName);
          }
#endif
        }
        nRow++;
      }
    }
#ifdef __WXDEBUG__
    wxASSERT_MSG(sErrors.IsEmpty(),sErrors);
#endif
    
    _SetRowNextControl(nRow);
    for(++nRow; nRow < nRowCount; ++nRow)
    {
      _SetRowEnabled(nRow,false);
    }
  }
  return bRtn;
}

void CGridAllelePosCtrl::_SetRowEnabled(int nRow, bool bEnabled)
{
  wxColour clr;
  if(bEnabled)
  {
    clr = GetDefaultCellBackgroundColour();
  }
  else
  {
    clr = GetDisabledColour();
    ClearRowValues(nRow);
  }
  SetRowReadOnly(nRow,!bEnabled);
  SetRowBackgroundColour(nRow,clr);
}
int CGridAllelePosCtrl::_FindNextControlRow(int nStart)
{
  wxString s;
  int nRows = GetNumberRows();
  int nRtn = -1;
  if(nStart < 1)
  {
    nStart = 1;
  }
  for(int nRow = nStart; nRow < nRows; ++nRow)
  {
    s = GetCellValueTrimmed(nRow,0);
    if(s.IsEmpty())
    {
      nRtn = nRow;
      nRow = nRows; // loop exit
    }
  }
  if(nRtn < 0)
  {
    nRtn = nRows;
    AppendRows(1);
  }
  return nRtn;
}
void CGridAllelePosCtrl::_SetRowNextControl(int nRow)
{
  if(nRow < 0)
  {
    nRow = _FindNextControlRow();
  }
  if(nRow >= 0)
  {
    _SetRowEnabled(nRow,false);
    m_nRowNextControl = nRow;
    if(!m_bReadOnly)
    {
      SetReadOnly(nRow,0,false);
      SetCellBackgroundColour(
        nRow,0,GetDefaultCellBackgroundColour());
    }
  }
}
void  CGridAllelePosCtrl::SetGridReadOnly(bool bReadOnly)
{
  int nRows = GetNumberRows();
  int nRow;
  m_bReadOnly = bReadOnly;
  if(bReadOnly)
  {
    for(nRow = 1; nRow < nRows; nRow++)
    {
      _SetRowEnabled(nRow,false);
    }
  }
  else
  {
    wxString sName;
    int nRows = GetNumberRows();
    int nRow;
    bool bFoundEmpty = false;
    for(nRow = 1; nRow < nRows; ++nRow)
    {
      sName = GetCellValueTrimmed(nRow,0);
      if(!sName.IsEmpty()) {}
      else if(!bFoundEmpty)
      {
        _SetRowNextControl(nRow);
        bFoundEmpty = true;
      }
      else
      {
        _SetRowEnabled(nRow,false);
      }
    }
  }
}
void CGridAllelePosCtrl::_DeleteRow(int nRow)
{
  if(!m_bReadOnly)
  {
    DeleteRows(nRow,1);
    AppendRows(1);
    SetGridReadOnly(false); // set up enabled/disabled rows
    SetGridCursor(nRow,0);
  }
  wxASSERT_MSG(
    !m_bReadOnly,
    "CGridAllelePosCtrl::_DeleteRow called on readonly table");
}
bool CGridAllelePosCtrl::_PromptDeleteRow(int nRow)
{
    // about to delete a positive control
  wxString sMsg("Do you want to delete ");
  const wxString &sLast(GetSavedValue());
  sMsg.Append(sLast);
  wxMessageDialog dlg(
    this, sMsg,"Warning",
    wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION );
  bool bRtn = false;
  int n = dlg.ShowModal();
  if(n == wxID_OK || n == wxID_YES)
  {
    _DeleteRow(nRow);
    bRtn = true;
  }
  return bRtn;
}

bool CGridAllelePosCtrl::_NameIsUsed(int nRowCheck, const wxString &s)
{
  wxString sValue;
  bool bRtn = false;
  if(s.IsEmpty())
  {
    sValue = GetCellValueTrimmed(nRowCheck,0);
  }
  else
  {
    sValue = s;
  }
  if(!sValue.IsEmpty())
  {
    wxString sCompare;
    int nRows = GetNumberRows();
    int nRow;
    for(nRow = 0; nRow < nRows; nRow++)
    {
      if(nRow != nRowCheck)
      {
        sCompare = GetCellValueTrimmed(nRow,0);
        if(sCompare == sValue)
        {
          bRtn = true;
          nRow = nRows; // loop exit
        }
      }
    }
  }
  return bRtn;
}

bool CGridAllelePosCtrl::ValidateCell(
  int nRow, int nCol, wxString *psPrompt)
{
  bool bRtn = true;
  if(nCol)
  {
    bRtn = CGridAlleleBase::ValidateCell(nRow,nCol,psPrompt);
  }
  else
  {
    wxString s = GetCellValueTrimmed(nRow,nCol);
    if(s.IsEmpty())
    {
      const wxString &sLast(GetSavedValue());
      if( (nRow != m_nRowNextControl) &&
        !sLast.IsEmpty() &&
        !_PromptDeleteRow(nRow) )

      {
        SetCellValue(nRow,0,sLast);
      }
    }
    else if(_NameIsUsed(nRow, s))
    {
      *psPrompt  = "This name, ";
      psPrompt->Append(s);
      psPrompt->Append(", is already used");
      bRtn = false;
    }
    else if(nRow == m_nRowNextControl)
    {
      _SetRowEnabled(nRow,true);
      int n = _FindNextControlRow(nRow + 1);
      _SetRowNextControl(n);
    }
  }
  return bRtn;
}
bool CGridAllelePosCtrl::AutoExpand()
{
  return false;
}

void CGridAllelePosCtrl::OnCellSelect(wxGridEvent &e)
{
  e.Skip();
}

BEGIN_EVENT_TABLE(CGridAllelePosCtrl,CGridAlleleBase)
EVT_GRID_SELECT_CELL(CGridAllelePosCtrl::OnCellSelect)
END_EVENT_TABLE()



//***********************************************  CGridAlleleBase

bool CGridAlleleBase::SetupKit(
  const wxString &sKitName, bool bAllowAmel)
{
  bool bRtn = CGridLocusColumns::SetupKit(
    this,sKitName,m_vsLeftColumns,
    false,bAllowAmel,true);
  m_mapLocusColumn.clear();
  m_nAmelColumn = -1;
  EnableDragRowSize(false);
  if(bRtn)
  {
    wxString s;
    int nCols = GetNumberCols();
    int nCol;
    for(nCol = (int)m_vsLeftColumns.size(); nCol < nCols; nCol++)
    {
      s = GetColLabelValue(nCol);
      nwxString::Trim(&s);
      if(!s.IsEmpty())
      {
        m_mapLocusColumn.insert(
          map<wxString,int,nwxStringLessNoCase>::value_type(
            s,nCol));
        if(CLabLocus::IsAmel(s))
        {
          m_nAmelColumn = nCol;
        }
      }
    }
    nCols = (int)m_vsLeftColumns.size();
    wxColour sClr = GetDisabledColour();
    for(nCol = 0; nCol < nCols; nCol++)
    {
      SetCellBackgroundColour(0,nCol,
        sClr);
    }
  }
  return bRtn;
}

CLabLocus *CGridAlleleBase::_NewLabLocus(
  vector<CLabLocus *> *pvpLocus, const wxString &sLocusName)
{
  CLabLocus *pRtn;
  if(pvpLocus->empty())
  {
    pRtn = new CLabLocus();
  }
  else
  {
    pRtn = *pvpLocus->rbegin();
    pvpLocus->pop_back();
    pRtn->Init();
  }
  pRtn->SetName(sLocusName);
  return pRtn;
}


CLabLocus *CGridAlleleBase::_BuildLabLocus(
  vector<CLabLocus *> *pvpLocus,
  const wxString &sLocusName, 
  const wxString &sAlleles, 
  bool bRequire3)
{
  vector<wxString> vs;
  CLabLocus *pRtn(NULL);
  size_t n = CLabLocus::BuildList(sAlleles,&vs);
  bool bOK = (n > 0) && ((n == 3) || (!bRequire3));
  if(bOK)
  {
    pRtn = _NewLabLocus(pvpLocus,sLocusName);
    *(pRtn->GetAlleles()) = vs;
  }
  return pRtn;
}

void CGridAlleleBase::_FitString(const wxString &s, int nStartColumn)
{
  int nCols = GetNumberCols();
  int nRows = GetNumberRows();
  int nCol;
  int nRow;
  if(nStartColumn < 0)
  {
    nStartColumn = (int)m_vsLeftColumns.size();
  }
  if(nRows > 1 && nCols > 0)
  {
    for(nCol = nStartColumn; nCol < nCols; nCol++)
    {
      SetCellValue(1,nCol,s);
    }
    for(nRow = 0; nRow < nRows; nRow++)
    {
      SetRowLabelValue(nRow,wxEmptyString);
    }
    UpdateLabelSizes();
    SetRowLabelSize(2);
    AutoSize();
  }
}

void CGridAlleleBase::Clear()
{
  int nCols = GetNumberCols();
  int nRows = GetNumberRows();
  int nRow;
  int nCol;
  m_vnLastRowUsed.resize(0);
  m_vnLastRowUsed.reserve((size_t)nCols);

  for(nCol = 0; nCol < nCols; nCol++)
  {
    for(nRow = 1; nRow < nRows; nRow++)
    {
      SetCellValue(nRow,nCol,wxEmptyString);
    }
    m_vnLastRowUsed.push_back(0);
  }
}

int CGridAlleleBase::_GetLocusColumn(const wxString &s)
{
  map<wxString,int,nwxStringLessNoCase>::iterator itr =
    m_mapLocusColumn.find(s);
  int nRtn = (itr == m_mapLocusColumn.end()) ? -1 : itr->second;
  return nRtn;
}


bool CGridAlleleBase::ValidateCell(
    int nRow, int nCol, wxString *psPrompt)
{
  bool bOK = true;
  if(nCol >= m_nPreAlleleColumns)
  {
    wxString s = GetCellValueTrimmed(nRow,nCol);
    if(!s.IsEmpty())
    {
      vector<wxString> vs;
      size_t n = CLabLocus::BuildList(s,&vs,nCol == m_nAmelColumn);
      const wxChar *psError(wxS(""));
      switch(m_nCellType)
      {
      case ALLELE_SINGLE:
        bOK = (n == 1);
        psError = wxS("This is not a valid allele.");
        break;
      case ALLELE_MULTIPLE:
        bOK = (n > 0);
        psError = wxS("The allele(s) are not valid.");
        break;
      case ALLELE_TRIPLE:
        bOK = (n == 3);
        psError = wxS("Three alleles are required.");
        break;
      default:
        bOK = false;
      }
      if(!bOK)
      {
        m_psLastError = psError;
        *psPrompt = m_psLastError;
      }
      else
      {
        s = CLabLocus::FormatAlleleList(vs);
        SetCellValue(nRow,nCol,s);
      }
    }
  }
  return bOK;
}

void CGridAlleleBase::CheckRowCount()
{
#if 1
  nwxGrid::CheckAutoExpandRows(this);
#else
  wxString s;
  int nRows = GetNumberRows();
  int nCols = GetNumberCols();
  int nStart = nRows - 2;
  int nCol;
  int nRow;

  if(nStart < 0)
  {
    nStart = 0;
  }
  for(nRow = nStart; nRow < nRows; nRow++)
  {
    for(nCol = 0; nCol < nCols; nCol++)
    {
      s = GetCellValue(nRow,nCol);
      nwxString::Trim(&s);
      if(s.Len())
      {
        nwxGrid::SetRowCount(nRows + 4);
        nCol = nCols;  // loop exit
        nRow = nRows;
      }
    }
  }
#endif
}
bool CGridAlleleBase::AutoExpand()
{
  return true;
}

void CGridAlleleBase::CellChangeExpand(wxGridEvent &e)
{
  if(AutoExpand())
  {
    int nRows = GetNumberRows();
    int nThisRow = e.GetRow();
    if(nThisRow >= (nRows - 2))
    {
#if 1
      nwxGrid::CheckAutoExpandRows(this);
#else
      // last or second to last row, add some rows
      int nThisCol = e.GetCol();
      wxString sValue = GetCellValue(nThisRow,nThisCol);
      nwxString::Trim(&sValue);
      if(sValue.Len())
      {
        nwxGrid::SetRowCount(nRows + 4);
      }
#endif
    }
  }
  e.Skip();
}


BEGIN_EVENT_TABLE(CGridAlleleBase,nwxGrid)
EVT_GRID_CELL_CHANGED(CGridAlleleBase::CellChangeExpand)
END_EVENT_TABLE()
