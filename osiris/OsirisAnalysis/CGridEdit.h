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
*  FileName: CGridEdit.h
*  Author:   Douglas Hoffman
*/
#ifndef __C_GRID_EDIT_H__
#define __C_GRID_EDIT_H__

#include <wx/grid.h>
#include <wx/colour.h>
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxGrid.h"

class _CGridEdit : public nwxGrid
{
public:
  _CGridEdit(wxWindow *parent, wxWindowID id, bool bReadOnly = false) :
      nwxGrid(parent,id) 
  {
    SetTableReadOnly(bReadOnly);
  }
  virtual ~_CGridEdit() {;}

  bool IsTableReadOnly()
  {
    return m_bReadOnly;
  }
  virtual void SetTableReadOnly(bool b = true)
  {
    m_bReadOnly = b;
    EnableEditing(!m_bReadOnly);
  }
//  virtual bool Show(bool show = true); // this didn't work
//  void SetupScrollRates();
  void SetTextLength(int nRow, int nCol, size_t nLength);
  bool IsCell(int nRow, int nCol);
  bool GetBoolValue(int nRow, int nCol);
  void SetBoolValue(int nRow, int nCol, bool b);
  void ToggleBoolValue(int nRow, int nCol);
  void SetBoolCell(int nRow, int nCol);
  bool IsBoolCell(int nRow, int nCol);
  bool IsGrayBackground(int nRow, int nCol);

  void SetGrayBackground(int nRow, int nCol, bool bGray = true);
  void SetGrayBackgroundCol(int nCol, bool bGray = true);
  void SetGrayBackgroundRow(int nRow, bool bGray = true);

  bool IsCellDisabled(int nRow,int nCol);
  void DisableEdit(int nRow, int nCol, bool bDisable = true);
  void DisableEditRow(int nRow, bool bDisable = true);
  static bool IsTrue(const char *ps);

  void EnableEdit(int nRow, int nCol, bool bEnable = true)
  {
    DisableEdit(nRow,nCol,!bEnable);
  }
  static const char s1[2];
  static const wxColour GRAY;
  void OnChar(wxKeyEvent &e);
  void OnLeftClick(wxGridEvent &e);
  void OnShowEditor(wxGridEvent &e);
private:
  void ToggleBoolEvent(wxEvent &e, int nRow, int nCol);
  long long _ComputeRowCol(int nRow,int nCol)
  {
    long long nr = nRow;
    long long nc = nCol;
    nc &= 0xffffffff;
    nr <<= 32;
    long long nRtn = nr | nc;
    return nRtn;
  }
  bool _IsBoolCell(int nRow,int nCol)
  {
    long long nn = _ComputeRowCol(nRow,nCol);
    return m_setBoolCells.find(nn) != m_setBoolCells.end();
  }
  void _SetBoolCell(int nRow,int nCol)
  {
    long long nn = _ComputeRowCol(nRow,nCol);
    m_setBoolCells.insert(nn);
  }
  void _OnScroll(wxScrollWinEvent &e);
  void OnSelectCell(wxGridEvent &e);
  void OnSelectRange(wxGridRangeSelectEvent &e);

  set< long long > m_setBoolCells;
  bool m_bReadOnly;
  DECLARE_EVENT_TABLE()
};

#endif
