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
*  FileName: CGridEdit.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CGridEdit.h"
#include "nwx/nwxString.h"
#include "CEventScroll.h"

DEFINE_EVENT_TYPE(CEventScroll)

// if __C_GRID_EDIT_CPP__ is defined, then static variables
// are defined in CGridEdit.h

const wxColour _CGridEdit::GRAY(240,240,240);
const char _CGridEdit::s1[2] = "1";

#if 0
// this didn't work :(
void _CGridEdit::SetupScrollRates()
{
  int nRows = GetNumberRows();
  int nCols = GetNumberCols();
  if( nRows > 1 && nCols > 0 )
  {
    int nX;
    int nY;
    int nY2;
    int nT0;
    int nT1;
    wxRect r0 = CellToRect(0,0);
    wxRect r1 = CellToRect(1,0);
    nT0 = r0.GetTop();
    nT1 = r1.GetTop();
    nY2 = nT1 - nT0;
    GetScrollPixelsPerUnit(&nX,&nY);

    if(nY2 != nY)
    {
      SetScrollRate(nX,nY2);
    }
  }
}
bool _CGridEdit::Show(bool show)
{
  bool bRtn = wxGrid::Show(show);
  if(show && bRtn)
  {
    SetupScrollRates();
  }
  return bRtn;
}
#endif

void _CGridEdit::SetTextLength(int nRow, int nCol, size_t nLength)
{
  if(IsCell(nRow,nCol))
  {
    if(nLength < 1)
    {
      nLength = 1;
    }
    wxGridCellTextEditor *ped(new wxGridCellTextEditor);
    wxString s = nwxString::FormatNumber((int) nLength);
    ped->SetParameters(s);
    SetCellEditor(nRow,nCol,ped);
  }
}
bool _CGridEdit::IsCell(int nRow, int nCol)
{
  bool bRtn = true;

  if(nRow < 0 || nCol < 0)
  {
    bRtn = false;
  }
  else if(nRow >= GetNumberRows())
  {
    bRtn = false;
  }
  else if(nCol >= GetNumberCols())
  {
    bRtn = false;
  }
  return bRtn;
}

bool _CGridEdit::GetBoolValue(int nRow, int nCol)
{
  bool b = false;
  wxString s(GetCellValue(nRow,nCol));
  b = IsTrue(s.c_str());
  return b;
}
void _CGridEdit::SetBoolValue(int nRow, int nCol, bool b)
{
  const char *ps = s1;
  if(!b) { ps++;}
  SetCellValue(nRow,nCol,_T(ps));
}
void _CGridEdit::ToggleBoolValue(int nRow, int nCol)
{
  bool b = GetBoolValue(nRow,nCol);
  SetBoolValue(nRow,nCol,!b);
}

void _CGridEdit::SetBoolCell(int nRow, int nCol)
{
  SetCellEditor(nRow,nCol,new wxGridCellBoolEditor);
  SetCellRenderer(nRow,nCol,new wxGridCellBoolRenderer);
  SetCellAlignment(nRow,nCol,wxALIGN_CENTRE,wxALIGN_CENTRE);
  SetCellFont(nRow,nCol,GetDefaultCellFont());
  SetReadOnly(nRow,nCol,true);
  _SetBoolCell(nRow,nCol);
}
bool _CGridEdit::IsBoolCell(int nRow, int nCol)
{
  bool bRtn = _IsBoolCell(nRow,nCol);
  return bRtn;
}
bool _CGridEdit::IsGrayBackground(int nRow, int nCol)
{
  bool b = false;
  wxColour c(GetCellBackgroundColour(nRow,nCol));
  b = (c == GRAY);
  return b;
}
bool _CGridEdit::IsCellDisabled(int nRow,int nCol)
{
  bool bRtn = false;
  if(IsTableReadOnly())
  {
    bRtn = true;
  }
  else if(IsBoolCell(nRow,nCol))
  {
    bRtn = IsGrayBackground(nRow,nCol);
  }
  else
  {
    bRtn = IsReadOnly(nRow,nCol);
  }
  return bRtn;
}
void _CGridEdit::DisableEdit(int nRow, int nCol, bool bDisable)
{
  if(!IsTableReadOnly())
  {
    if( bDisable || !IsBoolCell(nRow,nCol))
    {
      SetReadOnly(nRow,nCol,bDisable);
    }
  }
  SetGrayBackground(nRow,nCol,bDisable);
}
void _CGridEdit::DisableEditRow(int nRow, bool bDisable)
{
  int nCount = GetNumberCols();
  for(int nCol = 0; nCol < nCount; ++nCol)
  {
    DisableEdit(nRow,nCol,bDisable);
  }
}
void _CGridEdit::SetGrayBackground(int nRow, int nCol, bool bGray)
{
  const wxColour *pc(bGray ? &GRAY : wxWHITE);
  SetCellBackgroundColour(nRow,nCol,*pc);
}
void _CGridEdit::SetGrayBackgroundCol(int nCol, bool bGray)
{
  int nRows = GetNumberRows();
  const wxColour *pc(bGray ? &GRAY : wxWHITE);
  for(int nRow = 0; nRow < nRows; nRow++)
  {
    // faster than SetGrayBackground
    SetCellBackgroundColour(nRow,nCol,*pc);
  }
}
void _CGridEdit::SetGrayBackgroundRow(int nRow, bool bGray)
{
  int nCols = GetNumberCols();
  const wxColour *pc(bGray ? &GRAY : wxWHITE);
  for(int nCol = 0; nCol < nCols; nCol++)
  {
    // faster than SetGrayBackground
    SetCellBackgroundColour(nRow,nCol,*pc);
  }
}


bool _CGridEdit::IsTrue(const char *ps)
{
  bool b = false;
  while(isspace(*ps)) { ps++;}
  if(*ps)
  {
    b = (strchr("1TtyY",*ps) != NULL);
  }
  return b;
}

void _CGridEdit::OnChar(wxKeyEvent &e)
{
  if(!IsTableReadOnly())
  {
    int nCode = e.GetKeyCode();
    if(nCode == 32)
    {
      int nRow = GetGridCursorRow();
      int nCol = GetGridCursorCol();
      ToggleBoolEvent(e,nRow,nCol);
    }
    e.Skip();
  }
}


void _CGridEdit::OnLeftClick(wxGridEvent &e)
{
  if(!IsTableReadOnly())
  {
    int nRow = e.GetRow();
    int nCol = e.GetCol();
    ToggleBoolEvent(e,nRow,nCol);    
    e.Skip();
  }
}
void _CGridEdit::OnShowEditor(wxGridEvent &e)
{
  if(m_bReadOnly)
  {
    EnableCellEditControl(false);
  }
  else
  {
    e.Skip();
  }
}

void _CGridEdit::ToggleBoolEvent(wxEvent &, int nRow, int nCol)
{
  if(IsTableReadOnly()) {;} // read only table, do nothing
  else if(!IsCell(nRow,nCol)) {;} // invalid cell, do nothing
  else if(!IsBoolCell(nRow,nCol)) {;} // not bool, do nothing
  else if(!IsCellDisabled(nRow,nCol))
  {
    wxObject *pObj = this;
    int nID = GetId();
    ToggleBoolValue(nRow, nCol);
    wxGridEvent ee(nID,wxEVT_GRID_CELL_CHANGE,pObj,nRow,nCol);
    GetEventHandler()->ProcessEvent(ee);
  }
}
void _CGridEdit::_OnScroll(wxScrollWinEvent &e)
{
  wxWindow *pw = (wxWindow *)e.GetEventObject();
  wxEvtHandler *ph = (pw == NULL) ? NULL : pw->GetEventHandler();
  if(ph != NULL)
  {
    wxCommandEvent ec(CEventScroll,e.GetId());
    ec.SetEventObject(this);
    ph->AddPendingEvent(ec);
  }
  e.Skip(true);
}
void _CGridEdit::OnSelectCell(wxGridEvent &e)
{
  ClearSelection();
  e.Skip();
}
void _CGridEdit::OnSelectRange(wxGridRangeSelectEvent &e)
{
  if(e.Selecting())
  {
    ClearSelection();
  }
  e.Skip();
}

BEGIN_EVENT_TABLE(_CGridEdit,wxGrid)
EVT_GRID_CELL_LEFT_CLICK(_CGridEdit::OnLeftClick)
EVT_CHAR(_CGridEdit::OnChar)
EVT_GRID_EDITOR_SHOWN(_CGridEdit::OnShowEditor)
EVT_SCROLLWIN(_CGridEdit::_OnScroll)
EVT_GRID_RANGE_SELECT(_CGridEdit::OnSelectRange)
EVT_GRID_SELECT_CELL(_CGridEdit::OnSelectCell)
END_EVENT_TABLE()

