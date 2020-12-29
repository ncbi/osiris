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
*  FileName: nwxGrid.h
*  Author:   Douglas Hoffman
*
*   This was originially created to extend the wxGrid class
*   to have a function SetAllReadonly which turns out to be
*   unneeded because of wxGrid::EnableEditing() and
*   wxGrid::IsEditable()
*   Since then, cell validation has been added.
*   Also, there are many static utility functions which 
*   can be used for an instance of wxGrid
*/
#ifndef __NWX_GRID_H__
#define __NWX_GRID_H__

#include <wx/grid.h>
#include "nwx/nwxString.h"
#include "nwx/stdb.h"
#include <map>
#include <set>
#include "nwx/stde.h"
#include "nwx/nwxBatch.h"


class nwxGridCellValidator
{
public:
  virtual ~nwxGridCellValidator() {}
  virtual bool Validate(
    const wxString &sCellValue, 
    wxString *pErrorMessage = NULL,
    wxGrid *pGrid = NULL,
    int nRow = -1,
    int nCol = -1
    ) = 0;
};

template<class T> class TnwxGridCellRangeValidator 
  : public nwxGridCellValidator
{
public:
  virtual ~TnwxGridCellRangeValidator<T>() {}
  TnwxGridCellRangeValidator<T>(const T &Lo, const T &Hi, bool bAllowEmpty = true)
  {
    SetAllowEmpty(bAllowEmpty);
    SetRange(Lo, Hi);
  }
  virtual wxString ValueToString(const T &x)
  {
    return nwxString::FormatNumber(x);
  }
  virtual bool IsType(const wxString &s) = 0;
  virtual T StringToValue(const wxString &s) = 0;
  virtual const wxChar *InvalidTypeMessage() const = 0;

  virtual bool Validate(
    const wxString &sCellValue, 
    wxString *psError = NULL,
    wxGrid * = NULL,
    int = -1,
    int = -1
    )
  {
    wxString s(sCellValue);
    bool bRtn = true;
    nwxString::Trim(&s);
    if(s.IsEmpty())
    {
      bRtn = AllowEmpty();
      if((!bRtn) && (psError != NULL))
      {
        *psError = wxS("Data is required for this table cell.");
      }
    }
    else if(!IsType(s))
    {
      bRtn = false;
      *psError = InvalidTypeMessage();
    }
    else
    {
      T n = StringToValue(s);
      if(n < m_Lo || n > m_Hi)
      {
        bRtn = false;
        if(psError != NULL)
        {
          psError->Empty();
          psError->Alloc(128);
          *psError = wxS("Value is not within range, ");
          psError->Append(nwxString::FormatNumber(m_Lo));
          psError->Append(" to ");
          psError->Append(nwxString::FormatNumber(m_Hi));
          psError->Append(".");
        }
      }
    }
    return bRtn;
  }
  void SetRange(const T &Lo, const T &Hi)
  {
    m_Lo = Lo;
    m_Hi = Hi;
  }
  bool AllowEmpty() const
  {
    return m_bAllowEmpty;
  }
  void SetAllowEmpty(bool b)
  {
    m_bAllowEmpty = b;
  }
  const T &GetLo() const
  {
    return m_Lo;
  }
  const T &GetHi() const
  {
    return m_Hi;
  }
private:
  TnwxGridCellRangeValidator<T>() {}; 
  // private, therefore no default constructor
protected:
  bool m_bAllowEmpty;
  T m_Lo;
  T m_Hi;
};

class nwxGridCellDoubleRangeValidator : 
  public TnwxGridCellRangeValidator<double>
{
public:
  nwxGridCellDoubleRangeValidator(double dLo, double dHi, bool bAllowEmpty = true) :
     TnwxGridCellRangeValidator<double>(dLo,dHi,bAllowEmpty)
  {
    m_bAllowExp = false;
  }
  virtual ~nwxGridCellDoubleRangeValidator() {}

  virtual bool IsType(const wxString &s)
  {
    return nwxString::IsNumber(s,m_bAllowExp);
  }
  virtual double StringToValue(const wxString &s)
  {
    return atof(s.utf8_str());
  }
  virtual const wxChar *InvalidTypeMessage() const
  {
    return wxS("A numeric value is required for this table cell.");
  }
  void SetAllowExp(bool bAllow = true)
  {
    m_bAllowExp = bAllow;
  }
  bool AllowExp() const
  {
    return m_bAllowExp;
  }
private:
  bool m_bAllowExp;
};
class nwxGridCellUIntRangeValidator : 
  public TnwxGridCellRangeValidator<unsigned int>
{
public:
  nwxGridCellUIntRangeValidator(
    unsigned int nLo, unsigned int nHi, bool bAllowEmpty = true) :
      TnwxGridCellRangeValidator<unsigned int>(nLo,nHi,bAllowEmpty)
  {}
  virtual ~nwxGridCellUIntRangeValidator() {};
  virtual bool IsType(const wxString &s)
  {
    return nwxString::IsInteger(s,false);
  }
  virtual unsigned int StringToValue(const wxString &s)
  {
    return (unsigned int) atoi(s.utf8_str());
  }
  virtual const wxChar *InvalidTypeMessage() const
  {
    return wxS("A positive integer is required for this table cell.");
  }

};

class PointLess
{
  // pun intended
  //  used for mapping a wxPoint to an object with std::map<wxPoint, object>
public:
  PointLess() {};
  bool operator ()(const wxPoint &x1, const wxPoint &x2) const
  {
    bool bRtn = false;
    if(x1.x < x2.x)
    {
      bRtn = true;
    }
    else if((x1.x == x2.x) && (x1.y < x2.y))
    {
      bRtn = true;
    }
    return bRtn;
  }
};

class nwxGridCellValidatorCollection
{
public:
  nwxGridCellValidatorCollection() : m_pDefaultValidator(NULL)
  {}
  virtual ~nwxGridCellValidatorCollection();
  void Init();
  nwxGridCellValidator *Find(int nRow, int nCol);
  void SetCellValidator(nwxGridCellValidator *p, int nRow, int nCol);
  void SetDefaultCellValidator(nwxGridCellValidator *p)
  {
    if(p != m_pDefaultValidator)
    {
      m_pDefaultValidator = p;
      _STORE(p);
    }
  }
  void SetColumnCellValidator(nwxGridCellValidator *p, int nCol)
  {
    _InsertRowCol(&m_mapColValidator,nCol,p);
  }
  void SetRowCellValidator(nwxGridCellValidator *p, int nRow)
  {
    _InsertRowCol(&m_mapRowValidator,nRow,p);
  }
private:
  nwxGridCellValidator *_FindCell(int nRow, int nCol);
  nwxGridCellValidator *_FindRow(int nRow);
  nwxGridCellValidator *_FindCol(int nCol);
  void _InsertRowCol(map<int, nwxGridCellValidator *> *pmap,
    int n, nwxGridCellValidator *p);
  typedef map<wxPoint,nwxGridCellValidator *,PointLess> MAP2DV;
  typedef map<int, nwxGridCellValidator *> MAPV;

  set<nwxGridCellValidator *> m_setValidator;
  MAP2DV m_mapCellValidator;
  MAPV m_mapRowValidator;
  MAPV m_mapColValidator;
  nwxGridCellValidator *m_pDefaultValidator;


  void _STORE(nwxGridCellValidator *p)
  {
    if(p == NULL)
    {}
    else if(m_setValidator.find(p) == m_setValidator.end())
    {
      m_setValidator.insert(p);
    }
  }
};


class nwxGrid : public wxGrid
{

public:
//  static functions

  static void ForceRefreshAll(wxWindow *p);
  static wxColour GetDisabledColour(wxGrid *p);
  static void SetError(wxGrid *p, const wxString &sError);
  static void SetAllReadOnly(wxGrid *p, bool bReadOnly = true);
  static void SetRowReadOnly(wxGrid *p, int nRow, bool bReadOnly = true);
  static void ClearRowValues(wxGrid *p, int nRow);
  static void SetRowBackgroundColour(
    wxGrid *p, int nRow, const wxColour &clr);
  static void ClearAllRowLabels(wxGrid *p);
  static size_t GetColumnValues(wxGrid *p, int nCol, wxArrayString *ps);

  static void SetColReadOnly(wxGrid *p, int nCol, bool bReadOnly = true);
  static void SetColFont(wxGrid *p, int nCol, const wxFont &fn);
  static void SetColBackgroundColour(
    wxGrid *p, int nCol, const wxColour &clr);
  static void SetColAlignment(
    wxGrid *p, int nCol, int nHoriz, int nVert);
  static wxSize ComputeSize(wxGrid *p);
  static wxString GetTabDelimited(wxGrid *p,bool bLabels = true);
  static bool CopyToClipboard(wxGrid *p,bool bLabels = true);
  static void SetRowCount(wxGrid *p,int nRows);
  static void SetColCount(wxGrid *p,int nCols);
  static void SetRowColCount(wxGrid *p, int nRows, int nCols)
  {
    SetRowCount(p,nRows);
    SetColCount(p,nCols);
  }
  static wxSize ComputeSizeSet(wxGrid *p)
  {
    wxSize sz = ComputeSize(p);
    p->SetSize(sz);
    return sz;
  }
  static bool CheckAutoExpandRows(wxGrid *pGrid, int nRowsFromBottom = 2, int nAddCount = 4);
  static void SetMessageGrid(wxGrid *p, const wxString &sMessage);
  static void UpdateLabelSizes(wxGrid *p);

  static bool SetMacFont(wxGrid *p); // OS-1534 - set font that can show bold and italic

//  constructor, destructor, and memeber functions
//  it is recommended that only the static functions be used

  nwxGrid(
    wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = wxWANTS_CHARS,
    const wxString& name = wxPanelNameStr) :
    wxGrid(parent,id,pos,size,style,name),
      m_clrDisabled(0,0,0),
      m_nInCellChange(0),
      m_nEditorRow(-1),
      m_nEditorCol(-1),
      m_nEditorOn(0),
      m_bDO_NOT_SAVE(false)
  {}
  nwxGrid() :
    m_clrDisabled(0,0,0),
    m_nInCellChange(0),
    m_nEditorRow(-1),
    m_nEditorCol(-1),
    m_nEditorOn(0),
    m_bDO_NOT_SAVE(false)
  {;}
  virtual ~nwxGrid() {;}

  void SetRowReadOnly(int nRow, bool bReadOnly = true)
  {
    SetRowReadOnly(this,nRow,bReadOnly);
  }
  void ClearRowValues(int nRow)
  {
    ClearRowValues(this,nRow);
  }
  void SetRowBackgroundColour(int nRow, const wxColour &clr)
  {
    SetRowBackgroundColour(this, nRow, clr);
  }
  void SetRowDisabledColour(int nRow)
  {
    SetRowBackgroundColour(this, nRow, GetDisabledColour());
  }
  void SetRowCount(int nRows)
  {
    SetRowCount(this,nRows);
  }
  void SetColCount(int nCols)
  {
    SetColCount(this,nCols);
  }
  void SetRowColCount(int nRows, int nCols)
  {
    SetRowColCount(this, nRows, nCols);
  }
  void SetMessageGrid(const wxString &s)
  {
    SetMessageGrid(this,s);
  }
  wxString GetCellValueTrimmed(int nRow, int nCol)
  {
    wxString s = GetCellValue(nRow,nCol);
    nwxString::Trim(&s);
    return s;
  }
  bool CheckAutoExpandRows(int nRowsFromBottom = 2, int nAddCount = 4)
  {
    return CheckAutoExpandRows(this,nRowsFromBottom,nAddCount);
  }
  static wxString GetCellValueTrimmed(wxGrid *p, int nRow, int nCol)
  {
    wxString s = p->GetCellValue(nRow,nCol);
    nwxString::Trim(&s);
    return s;
  }
  wxString GetTabDelimited(bool bLabels = true)
  {
    return GetTabDelimited(this,bLabels);
  }
  bool CopyToClipboard(bool bLabels = true)
  {
    return CopyToClipboard(this,bLabels);
  }
  void SetAllReadOnly(bool bReadOnly = true)
  {
    SetAllReadOnly(this,bReadOnly);
  }
  wxSize ComputeSize()
  {
    return ComputeSize(this);
  }
  void ComputeSizeSet()
  {
    ComputeSizeSet(this);
  }
  void UpdateLabelSizes()
  {
    UpdateLabelSizes(this);
  }
  void SetError(const wxString &sError)
  {
    SetError(this,sError);
  }
  const wxString &GetSavedValue()
  {
    return m_sValue;
  }
  void SaveCellValue(int nRow, int nCol)
  {
    if(m_bDO_NOT_SAVE || ((nRow < 0) && (nCol < 0)) )
    {
      ;// do not save
    }
    else
    {
      m_sValue = GetCellValueTrimmed(nRow,nCol);
    }
  }
  void SaveCellValue()
  {
    SaveCellValue(m_nEditorRow,m_nEditorCol);
  }
  void RestoreCellValue(int nRow, int nCol)
  {
    if(nRow >= 0 && nCol >= 0)
    {
      SetCellValue(nRow,nCol,m_sValue);
    }
  }
  void RestoreCellValue()
  {
    RestoreCellValue(m_nEditorRow,m_nEditorCol);
  }
  virtual bool ValidateCell(
    int nRow, int nCol, wxString *pError)
  {
    wxString s = GetCellValue(nRow,nCol);
    bool bRtn = true;
    nwxGridCellValidator *pv = m_validators.Find(nRow,nCol);
    if(pv != NULL)
    {
      bRtn = pv->Validate(s,pError,this,nRow,nCol);
    }
    return bRtn;
  }
  void SetDoNotSave(bool b = true)
  {
    m_bDO_NOT_SAVE = b;
  }
  bool GetDoNotSave()
  {
    return m_bDO_NOT_SAVE;
  }

  void SetCellValidator(nwxGridCellValidator *p, int nRow, int nCol)
  {
    m_validators.SetCellValidator(p,nRow,nCol);
  }
  void SetDefaultCellValidator(nwxGridCellValidator *p)
  {
    m_validators.SetDefaultCellValidator(p);
  }
  void SetColumnCellValidator(nwxGridCellValidator *p, int nCol)
  {
    m_validators.SetColumnCellValidator(p,nCol);
  }
  void SetRowCellValidator(nwxGridCellValidator *p, int nRow)
  {
    m_validators.SetRowCellValidator(p,nRow);
  }
  void InitValidators()
  {
    m_validators.Init();
  }
  wxColour GetDisabledColour();
  virtual void OnEditorStart(wxGridEvent &e);
  virtual void OnEditorEnd(wxGridEvent &e);
  virtual void OnCellChange(wxGridEvent &e);
  virtual void OnSelectCell(wxGridEvent &e);
  void OnEditCell(wxGridEvent &e);
  static bool CheckVirtualSize(wxScrolledWindow *p,wxSize *pSize = NULL);
private:
  void _OnCellChange(wxGridEvent &e);
  void _OnSelectCell(wxGridEvent &e);
  void _OnEditorStart(wxGridEvent &e);
  void _OnEditorEnd(wxGridEvent &e);
  nwxGridCellValidatorCollection m_validators;
  wxColour m_clrDisabled;
  wxString m_sValue;
  wxString m_sEditorStartValue;
  int m_nInCellChange;
  int m_nEditorRow;
  int m_nEditorCol;
  int m_nEditorOn;
  bool m_bDO_NOT_SAVE;
  DECLARE_EVENT_TABLE()
  DECLARE_CLASS(nwxGrid)
};


class nwxLabelGrid: public nwxGrid
{
  // grid class that displays labels in
  // otherwise empty cells

private:
  // types mapped from cell location (wxPoint)
  typedef std::map<const wxPoint, wxFont, PointLess> MAP_POINT_FONT;
  typedef std::map<const wxPoint, wxColour, PointLess> MAP_POINT_COLOUR;
  typedef std::map<const wxPoint, wxString, PointLess> MAP_POINT_STR;
  typedef std::map<const wxPoint, bool, PointLess> MAP_POINT_BOOL;


  //  label data

  MAP_POINT_BOOL m_mapLabelUsed;
  MAP_POINT_COLOUR m_mapLabelTextColour, m_mapSaveTextColour;
  MAP_POINT_FONT m_mapLabelFont, m_mapSaveFont;
  MAP_POINT_STR m_mapLabels;
  int m_nBatch;
  bool m_bEnabledAfterBatch;
  bool m_bLabelsEnabled;

  // BEGIN class MAP_POINT_SETTER
  template <class T> class MAP_POINT_SETTER
  {
    // class for getting/setting values of std::map<wxPoint, T, PointLess>

  private:
    MAP_POINT_SETTER<T> () {} // private, prevent instantiation
  public:
    static void ClearValue(const wxPoint &pt, std::map<const wxPoint, T, PointLess> *pmap)
    {
      typename std::map<const wxPoint, T, PointLess>::iterator itr = pmap->find(pt);
      if(itr != pmap->end())
      {
        pmap->erase(itr);
      }
    }
    static void SetValue(const wxPoint &pt, const T &t, std::map<const wxPoint, T, PointLess> *pmap)
    {
      typename std::map<const wxPoint, T, PointLess>::iterator itr = pmap->find(pt);
      bool bIns = true;
      if(itr == pmap->end())
      {}
      else if(itr->second != t)
      {
        pmap->erase(itr);
      }
      else
      {
        bIns = false;
      }
      if(bIns)
      {
        pmap->insert(typename std::map<const wxPoint, T>::value_type(pt,t));
      }
    }
    static const T *GetValue(const wxPoint &pt, const std::map<const wxPoint, T, PointLess> &pmap)
    {
      typename std::map<const wxPoint, T, PointLess>::const_iterator itr = pmap.find(pt);
      if(itr == pmap.end())
      {
        itr = pmap.find(wxPoint(-1,-1));
      }
      return (itr != pmap.end()) ? &(itr->second) : NULL;
    }
  };
  // END class MAP_POINT_SETTER
public:
  nwxLabelGrid(
    wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = wxWANTS_CHARS,
    const wxString& name = wxPanelNameStr) :
      nwxGrid(parent,id,pos,size,style,name),
      m_nBatch(0),
      m_bEnabledAfterBatch(false),
      m_bLabelsEnabled(true)  
  {}
  virtual ~nwxLabelGrid()
  {}
  virtual void OnEditorStart(wxGridEvent &e);
  virtual void OnEditorEnd(wxGridEvent &e);
  virtual void OnCellChange(wxGridEvent &e);

  bool IsLabelUsed(int nRow, int nCol)
  {
    bool bRtn = false;
    if(_LabelsEnabled())
    {
      const bool *pbRtn = MAP_POINT_SETTER<bool>::GetValue(wxPoint(nRow,nCol), m_mapLabelUsed);
      bRtn = (pbRtn == NULL) ? false : *pbRtn;
    }
    else if(GetCellValue(nRow, nCol).IsEmpty())
    {
      const wxString *ps = MAP_POINT_SETTER<wxString>::GetValue(wxPoint(nRow,nCol), m_mapLabels);
      bRtn = ps == NULL ? false : !ps->IsEmpty();
    }
    return bRtn;
  }

  // enable/disable labels
  // begin/end batch
  //   after calling BeginBatch(), DisableLabels(), or EnableLabels(false)
  //   wxGrid::GetCellValue() will provide accurate results
  //
  void EnableLabels(bool bEnable = true)
  {
    // public function for enabling or disabling labels
    // on empty table cells
    if(_InBatch())
    {
      // if 'in batch' labels are always disabled but
      // m_bEnabledAfterBatch is used to determine
      // if labels should be shown when no longer 
      // 'in batch'
      m_bEnabledAfterBatch = bEnable;
    }
    else
    {
      // not in batch, disable labels if not already done
      _EnableLabels(bEnable);
    }
  }
  void DisableLabels()
  {
    EnableLabels(false);
  }
  bool LabelsEnabled()
  {
    // public method, return true if labels are enable or will be after
    // no longer _InBatch()
    return _InBatch() ? m_bEnabledAfterBatch : m_bLabelsEnabled;
  }
  void BeginBatch()
  {
    // if labels are used:
    // call BeginBatch before modifying any cells (value, attributes, etc)
    // call EndBatch when finished
    // alternatively and recommended, use the nwxLabelGridBatch class
    // defined below to perform these functions
    m_nBatch++;
    if(_InBatchTopLevel())
    {
      wxGrid::BeginBatch();
      m_bEnabledAfterBatch = m_bLabelsEnabled;
      _EnableLabels(false);
    }
  }
  void EndBatch()
  {
    if(_InBatchTopLevel())
    {
      _EnableLabels(m_bEnabledAfterBatch);
      wxGrid::EndBatch();
    }
    m_nBatch--;
  }
  void SetActualValue(int nRow, int nCol, const wxString &s)
  {
    if(_LabelsEnabled() && IsLabelUsed(nRow, nCol))
    {
      TnwxBatch<nwxLabelGrid> x(this);
      SetCellValue(nRow, nCol, s);
    }
    else
    {
      // no batch needed
      SetCellValue(nRow, nCol, s);
    }
  }
  wxString GetActualValue(int nRow, int nCol)
  {
    wxString sRtn;
    if(!( _LabelsEnabled() && IsLabelUsed(nRow, nCol) ))
    {
      sRtn = GetCellValue(nRow, nCol);
    }
    return sRtn;
  }

  void SetLabelValue(int nRow, int nCol, const wxString &s)
  {
    if(s.IsEmpty())
    {
      ClearLabelValue(nRow, nCol);
    }
    else
    {
      TnwxBatch<nwxLabelGrid> x(this);
      MAP_POINT_SETTER<wxString>::SetValue(wxPoint(nRow, nCol), s, &m_mapLabels);
    }
  }
  void SetLabelFont(int nRow, int nCol, const wxFont &f)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    MAP_POINT_SETTER<wxFont>::SetValue(wxPoint(nRow, nCol), f, &m_mapLabelFont);
  }
  void SetLabelTextColour(int nRow, int nCol, const wxColour &colour)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    MAP_POINT_SETTER<wxColour>::SetValue(wxPoint(nRow, nCol), colour, &m_mapLabelTextColour);
  }

  void ClearLabelValue(int nRow, int nCol)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    wxPoint pt(nRow, nCol);
    MAP_POINT_SETTER<wxString>::ClearValue(pt, &m_mapLabels);
  }
  void ClearLabelFont(int nRow, int nCol)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    wxPoint pt(nRow, nCol);
    MAP_POINT_SETTER<wxFont>::ClearValue(pt, &m_mapLabelFont);
    MAP_POINT_SETTER<wxFont>::ClearValue(pt, &m_mapSaveFont);
  }
  void ClearLabelTextColour(int nRow, int nCol)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    wxPoint pt(nRow, nCol);
    MAP_POINT_SETTER<wxColour>::ClearValue(pt, &m_mapLabelTextColour);
    MAP_POINT_SETTER<wxColour>::ClearValue(pt, &m_mapSaveTextColour);
  }
  void ClearLabel(int nRow, int nCol)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    wxPoint pt(nRow, nCol);
    MAP_POINT_SETTER<wxString>::ClearValue(pt, &m_mapLabels);
    MAP_POINT_SETTER<wxFont>::ClearValue(pt, &m_mapLabelFont);
    MAP_POINT_SETTER<wxFont>::ClearValue(pt, &m_mapSaveFont);
    MAP_POINT_SETTER<wxColour>::ClearValue(pt, &m_mapLabelTextColour);
    MAP_POINT_SETTER<wxColour>::ClearValue(pt, &m_mapSaveTextColour);
  }
  void ClearAll()
  {
    TnwxBatch<nwxLabelGrid> x(this);
    m_mapLabels.clear();
    m_mapLabelFont.clear();
    m_mapSaveFont.clear();
    m_mapLabelTextColour.clear();
    m_mapSaveTextColour.clear();
  }

  //void SetLabelBackgroundColor(int nRow, int nCol, const wxColour &colour);

  void SetDefaultLabelFont(const wxFont &f)
  {
    SetLabelFont(-1, -1, f);
  }
  void SetDefaultLabelTextColour(const wxColour &colour)
  {
    SetLabelTextColour(-1, -1, colour);
  }

  const wxString *GetLabelValue(const wxPoint &pt)
  {
    return MAP_POINT_SETTER<wxString>::GetValue(pt, m_mapLabels);
  }
  const wxString *GetLabelValue(int nRow, int nCol)
  {
    return GetLabelValue(wxPoint(nRow, nCol));
  }
  const wxFont *GetLabelFont(const wxPoint &pt)
  {
    return MAP_POINT_SETTER<wxFont>::GetValue(pt, m_mapLabelFont);
  }
  const wxFont *GetLabelFont(int nRow, int nCol)
  {
    return GetLabelFont(wxPoint(nRow,nCol));
  }
  const wxColour *GetLabelTextColour(const wxPoint &pt)
  {
    return MAP_POINT_SETTER<wxColour>::GetValue(pt, m_mapLabelTextColour);
  }
  const wxColour *GetLabelTextColour(int nRow, int nCol)
  {
    return GetLabelTextColour(wxPoint(nRow,nCol));
  }
private:
  void _EnableLabels(bool bEnable = true);
  bool _InBatch()
  {
    return(m_nBatch > 0);
  }
  bool _InBatchTopLevel()
  {
    return(m_nBatch == 1);
  }
  bool _LabelsEnabled()
  {
    return m_bLabelsEnabled;
  }
  void _SetSavedLabelFont(const wxPoint &pt, const wxFont &f)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    MAP_POINT_SETTER<wxFont>::SetValue(pt, f, &m_mapSaveFont);
  }
  void _SetSavedLabelColour(const wxPoint &pt, const wxColour &colour)
  {
    TnwxBatch<nwxLabelGrid> x(this);
    MAP_POINT_SETTER<wxColour>::SetValue(pt, colour, &m_mapSaveTextColour);
  }
  const wxFont *_GetSavedLabelFont(const wxPoint &pt)
  {
    return MAP_POINT_SETTER<wxFont>::GetValue(pt, m_mapSaveFont);
  }
  const wxColour *_GetSavedLabelColour(const wxPoint &pt)
  {
    return MAP_POINT_SETTER<wxColour>::GetValue(pt, m_mapSaveTextColour);
  }
};
typedef TnwxBatch<nwxLabelGrid> nwxLabelGridBatch;


BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_GRID_START_EDIT, -1)
END_DECLARE_EVENT_TYPES()


#endif
