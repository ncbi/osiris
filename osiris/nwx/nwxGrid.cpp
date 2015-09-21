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
*  FileName: nwxGrid.cpp
*  Author:   Douglas Hoffman
*
*   This was originially created to extend the wxGrid class
*   to have a function SetAllReadonly, but is now recommended
*   that the static functions be used instead of instantiation and
*   inheritance
*
*/
#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"
#include <wx/dcclient.h>
#include <wx/dataobj.h>
#include <wx/clipbrd.h>
#include <wx/msgdlg.h>
#include <wx/control.h>
#include <wx/textctrl.h>
#include "nwx/nwxGrid.h"
#include "nwx/nwxString.h"
#include "nwx/nwxColorUtil.h"
#include "nwx/CIncrementer.h"
#include "nwx/setptr.h"
#include <ctype.h>

DEFINE_EVENT_TYPE(wxEVT_GRID_START_EDIT)

//******************************** nwxGridCellValidatorCollection **

nwxGridCellValidatorCollection::~nwxGridCellValidatorCollection()
{
  setptr<nwxGridCellValidator>::cleanup(&m_setValidator);
}
nwxGridCellValidator *nwxGridCellValidatorCollection::_FindCell(
  int nRow, int nCol)
{
  wxPoint pt(nRow,nCol);
  MAP2DV::iterator itr
    = m_mapCellValidator.find(pt);
  nwxGridCellValidator *pRtn = 
    (itr != m_mapCellValidator.end()) ? itr->second : NULL;
  return pRtn;
}
nwxGridCellValidator *nwxGridCellValidatorCollection::_FindRow(
  int nRow)
{
  MAPV::iterator itr = 
    m_mapRowValidator.find(nRow);
  nwxGridCellValidator *pRtn = 
    (itr != m_mapRowValidator.end()) ? itr->second : NULL;
  return pRtn;
}
nwxGridCellValidator *nwxGridCellValidatorCollection::_FindCol(
  int nCol)
{
  MAPV::iterator itr = 
    m_mapColValidator.find(nCol);
  nwxGridCellValidator *pRtn = 
    (itr != m_mapColValidator.end()) ? itr->second : NULL;
  return pRtn;
}

nwxGridCellValidator *nwxGridCellValidatorCollection::Find(
  int nRow, int nCol)
{
  nwxGridCellValidator *pRtn = NULL;
  if((pRtn = _FindCell(nRow,nCol)) != NULL) {}
  else if((pRtn = _FindRow(nRow)) != NULL) {}
  else if((pRtn = _FindCol(nCol)) != NULL) {}
  else
  {
    pRtn = m_pDefaultValidator;
  }
  return pRtn;
}

void nwxGridCellValidatorCollection::SetCellValidator(
  nwxGridCellValidator *p, int nRow, int nCol)
{
  wxPoint pt(nRow,nCol);
  MAP2DV::iterator itr = 
    m_mapCellValidator.find(pt);
  bool bInsert = true;
  
  if(itr == m_mapCellValidator.end()) 
  {}
  else if(itr->second == p)
  { bInsert = false; }
  else
  { m_mapCellValidator.erase(itr); }

  if(bInsert)
  {
    _STORE(p);
    m_mapCellValidator.insert(
      MAP2DV::value_type(
        pt,p));
  }
}

void nwxGridCellValidatorCollection::_InsertRowCol(
  map<int, nwxGridCellValidator *> *pmap,
  int n, nwxGridCellValidator *p)
{
  MAPV::iterator itr = pmap->find(n);
  bool bInsert = true;
  if(itr == pmap->end())
  {}
  else if(itr->second == p)
  {
    bInsert = false;
  }
  else
  {
    pmap->erase(itr);
  }
  if(bInsert)
  {
    _STORE(p);
    pmap->insert(
      MAPV::value_type(
        n,p));
  }
}


//*************************************** nwxGrid



bool nwxGrid::CheckVirtualSize(wxScrolledWindow *p,wxSize *pSize)
{
  // return true is virtual size >= actual size in width and height
  wxSize sz = p->GetSize();
  wxSize szv;
  bool bRtn = false;
  p->GetVirtualSize(&szv.x,&szv.y);
  if(szv.x >= sz.x && szv.y > sz.y)
  {
    if(pSize != NULL)
    {
      *pSize = szv;
    }
    bRtn = true;
  }
  return bRtn;
}
wxColour nwxGrid::GetDisabledColour(wxGrid *p)
{
  wxColour c1 = p->GetDefaultCellBackgroundColour();
  wxColour c2 = p->GetGridLineColour();
  wxColour c3(nwxColorUtil::Average(c1,c2));
  return c3;
}
wxColour nwxGrid::GetDisabledColour()
{
  if(!(
    m_clrDisabled.Red() || 
    m_clrDisabled.Green() || 
    m_clrDisabled.Blue() ))
  {
    m_clrDisabled = GetDisabledColour(this);
  }
  return m_clrDisabled;
}
wxSize nwxGrid::ComputeSize(wxGrid *p)
{
  wxSize szRtn;
#ifdef __WXDEBUG__
  wxRect r;
  wxSize sz2;
#endif
//  if(!CheckVirtualSize(p,&szRtn))
  {
    int nHeight = p->GetColLabelSize() + 3;
    int nWidth = p->GetRowLabelSize() + 3;
    int nRows = p->GetNumberRows();
    int nCols = p->GetNumberCols();
    int i;
    int n;
#ifdef __WXDEBUG__
    if(nRows > 0 && nCols > 0)
    {
      r = p->CellToRect(nRows - 1, nCols - 1);
      sz2.SetWidth(r.GetX() + nWidth + r.GetWidth());
      sz2.SetHeight(r.GetY() + nHeight + r.GetHeight());
    }
#endif
    for(i = 0; i < nRows; i++)
    {
      n = p->GetRowSize(i);
      n++;
      nHeight += n;
    }
    for(i = 0; i < nCols; i++)
    {
      n = p->GetColSize(i);
      n++;
      nWidth += n;
    }
    szRtn.x = nWidth;
    szRtn.y = nHeight;
  }
  return szRtn;
}
void nwxGrid::SetRowCount(wxGrid *p, int n)
{
  int nRows = p->GetNumberRows();
  if(n == nRows) {} // great, do nothing
  else if(n < nRows)
  {
    // we have too many rows;
    p->DeleteRows(n, nRows - n);
  }
  else
  {
    p->AppendRows(n - nRows);
  }
}
void nwxGrid::SetColCount(wxGrid *p, int n)
{
  int nCols = p->GetNumberCols();
  if(n == nCols) {} // do nothing
  else if(n < nCols)
  {
    p->DeleteCols(n,nCols - n);
  }
  else
  {
    p->AppendCols(n - nCols);
  }
}

void nwxGrid::SetMessageGrid(wxGrid *p, const wxString &sMessage)
{
  SetRowColCount(p,1,1);
  p->SetRowLabelSize(2);
  p->SetColLabelSize(2);
  p->SetRowLabelValue(0,wxEmptyString);
  p->SetColLabelValue(0,wxEmptyString);
  p->SetCellValue(0,0,sMessage);
  p->SetReadOnly(0,0,true);
  p->AutoSizeColumns();
  ComputeSizeSet(p);
}

void nwxGrid::UpdateLabelSizes(wxGrid *p)
{
  int nRow = p->GetNumberRows();
  if(nRow > 0)
  {
    wxClientDC dc(p);
    dc.SetFont(p->GetLabelFont());
    wxString sLabel;
    wxSize sz;
    int n;
    int nWidth = 0;
    p->SetColLabelSize(p->GetRowSize(0) + 2);
    for(int i = 0; i < nRow; i++)
    {
      sLabel = p->GetRowLabelValue(i);
      if(!sLabel.IsEmpty())
      {
        sz = dc.GetTextExtent(sLabel);
        n = sz.GetWidth();
        if(n > nWidth) { nWidth = n; }
      }
    }
    p->SetRowLabelSize(nWidth + 6);
  }
}

void nwxGrid::SetError(wxGrid *p, const wxString &sError)
{
  p->SetCellSize(0,0,1,1);
  SetRowColCount(p,1,1);
  p->SetCellBackgroundColour(0,0,p->GetDefaultCellBackgroundColour());
  p->SetCellTextColour(0,0,p->GetDefaultCellTextColour());
  p->SetRowLabelValue(0,wxEmptyString);
  p->SetColLabelValue(0,wxEmptyString);
  p->SetCellValue(0,0,sError);
  p->SetReadOnly(0,0,true);
  p->SetRowLabelSize(1);
  p->SetColLabelSize(1);
  p->AutoSize();

}
void nwxGrid::SetAllReadOnly(wxGrid *p, bool bReadOnly)
{
  int nRows = p->GetNumberRows();
  for(int nRow = 0; nRow < nRows; nRow++)
  {
    SetRowReadOnly(p,nRow,bReadOnly);
  }
}
void nwxGrid::SetRowReadOnly(wxGrid *p, int nRow, bool bReadOnly)
{
  int nCols = p->GetNumberCols();
  for(int nCol = 0; nCol < nCols; nCol++)
  {
    p->SetReadOnly(nRow,nCol,bReadOnly);
  }
}

void nwxGrid::SetRowBackgroundColour(wxGrid *p, int nRow, const wxColour &clr)
{
  int nCols = p->GetNumberCols();
  for(int nCol = 0; nCol < nCols; nCol++)
  {
    p->SetCellBackgroundColour(nRow,nCol,clr);
  }

}
void nwxGrid::ClearRowValues(wxGrid *p, int nRow)
{
  int nCols = p->GetNumberCols();
  for(int nCol = 0; nCol < nCols; nCol++)
  {
    p->SetCellValue(nRow,nCol,wxEmptyString);
  }
}

wxString nwxGrid::GetTabDelimited(wxGrid *pGrid, bool bLabels)
{
  wxString sTab("\t");
  wxString sSpace(" ");
  wxString sEOL(nwxString::EOL);
  wxString sRtn;
  wxString s;
  int nCOLS = pGrid->GetNumberCols();
  int nROWS = pGrid->GetNumberRows();
  int nRow;
  int nCol;
  sRtn.Alloc((nCOLS +1) * (nROWS +1) * 20);
  if(bLabels)
  {
    for(nCol = 0; nCol < nCOLS; nCol++)
    {
      sRtn.Append(sTab);
      s = pGrid->GetColLabelValue(nCol);
      s.Replace(sTab,sSpace,true);
      sRtn.Append(s);
    }
    sRtn.Append(sEOL);
  }
  for(nRow = 0; nRow < nROWS; nRow++)
  {
    if(bLabels)
    {
      s = pGrid->GetRowLabelValue(nRow);
      s.Replace(sTab,sSpace,true);
      s.Append(sTab);
      sRtn.Append(s);
    }
    for(nCol = 0; nCol < nCOLS; nCol++)
    {
      if(nCol)
      {
        sRtn.Append(sTab);
      }
      s = pGrid->GetCellValue(nRow,nCol);
      s.Replace(sTab,sSpace,true);
      sRtn.Append(s);
    }
    sRtn.Append(sEOL);
  }
  return sRtn;
}

bool nwxGrid::CopyToClipboard(wxGrid *pGrid,bool bLabels)
{
  wxString s(GetTabDelimited(pGrid,bLabels));
  auto_ptr<wxTextDataObject> pObj(new wxTextDataObject(s));
  wxClipboard clp;
  bool bRtn = clp.Open();
  if(bRtn)
  {
    clp.SetData(pObj.release());
    clp.Flush();
    clp.Close();
  }
  return bRtn;
}


void nwxGrid::OnCellChange(wxGridEvent &e)
{
  bool bDone = true; // true if done editing
  if(!m_nInCellChange)
  {
    // make sure there is no recursion
    // which seems to happen when calling SetGridCursor() below
    //
    CIncrementer x(m_nInCellChange);
    wxString sPrompt;
    int nRow = e.GetRow();
    int nCol = e.GetCol();
    if( !ValidateCell(nRow,nCol,&sPrompt)
        && !sPrompt.IsEmpty() )
    {
      sPrompt.Append(
        "\nWould you like to continue editiing?");
      wxMessageDialog dlg(this,sPrompt,"Error",
        wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
      int n = dlg.ShowModal();
      if(n == wxID_YES || n == wxID_OK)
      {
        wxGridEvent ee(e);
        ee.SetEventType(wxEVT_GRID_START_EDIT);
        SetDoNotSave(true);
        SetGridCursor(nRow,nCol);
        AddPendingEvent(ee);
        bDone = false;
      }
      else
      {
        RestoreCellValue();
      }
    }
    if(bDone)
    {
      SetDoNotSave(false);
      m_nEditorRow = -1;
      m_nEditorCol = -1;
    }
  }
  e.Skip();
}
void nwxGrid::OnEditorStart(wxGridEvent &e)
{
  m_nEditorOn++;
  m_nEditorRow = e.GetRow();
  m_nEditorCol = e.GetCol();
  m_sEditorStartValue = GetCellValue(m_nEditorRow, m_nEditorCol);
  SaveCellValue();
  e.Skip();
}

void nwxGrid::OnEditorEnd(wxGridEvent &e)
{
  if(m_nEditorOn)
  {
    m_nEditorOn--;

    int nRow = e.GetRow();
    int nCol = e.GetCol();
    // memory leak on following line
    wxGridCellEditor *pEdit = GetCellEditor(nRow, nCol);
    wxControl *pControl = (pEdit == NULL) ? NULL : pEdit->GetControl();
    wxTextCtrl *pText = 
      (pControl == NULL)
      ? NULL
      : wxDynamicCast(pControl,wxTextCtrl);
    if(pText != NULL)
    {
      wxString s;
      s = pText->GetValue();
      if(s == m_sEditorStartValue || s != m_sValue)
      {
        // value hasn't changed but we still want to force a 
        // CELL_CHANGED event
        wxGridEvent ee(e);
        ee.SetEventType(wxEVT_GRID_CELL_CHANGED);
        AddPendingEvent(ee);
      }
      if(pEdit != NULL)
      {
        pEdit->DecRef();
      }
    }
  }
  e.Skip();
}

void nwxGrid::OnSelectCell(wxGridEvent &e)
{
  e.Skip();
}

void nwxGrid::OnEditCell(wxGridEvent &e)
{
  CIncrementer x(m_nInCellChange);
  int nRow = e.GetRow();
  int nCol = e.GetCol();
  SetDoNotSave(true);
  SetGridCursor(nRow,nCol);
  EnableCellEditControl(true);
}

void nwxGrid::ForceRefreshAll(wxWindow *p)
{
  wxGrid *pGrid;
  if((pGrid = wxDynamicCast(p,wxGrid)) != NULL)
  {
    pGrid->ForceRefresh();
    if(pGrid->IsShown())
    {
      pGrid->GetParent()->Layout();
    }
  }
  else
  {
    wxWindowList &a = p->GetChildren();
    size_t n = a.GetCount();
    for(size_t i = 0; i < n; i++)
    {
      ForceRefreshAll(a.Item(i)->GetData());
    }
  }
}

BEGIN_EVENT_TABLE(nwxGrid,wxGrid)
wx__DECLARE_GRIDEVT(START_EDIT,wxID_ANY,nwxGrid::OnEditCell)
EVT_GRID_EDITOR_SHOWN(nwxGrid::OnEditorStart)
EVT_GRID_EDITOR_HIDDEN(nwxGrid::OnEditorEnd)
EVT_GRID_CELL_CHANGED(nwxGrid::OnCellChange)
EVT_GRID_SELECT_CELL(nwxGrid::OnSelectCell)
END_EVENT_TABLE()

IMPLEMENT_CLASS(nwxGrid,wxGrid)
