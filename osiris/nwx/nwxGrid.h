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


class nwxGridCellValidator
{
public:
  virtual ~nwxGridCellValidator() {}
  virtual bool Validate(
    const wxString &sCellValue, wxString *pErrorMessage = NULL) = 0;
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
    const wxString &sCellValue, wxString *psError = NULL)
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
          *psError = wxS("Value is not within range, [");
          psError->Append(nwxString::FormatNumber(m_Lo));
          psError->Append(", ");
          psError->Append(nwxString::FormatNumber(m_Hi));
          psError->Append("]");
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
  class PointLess
  {
    // pun intended
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
  static void SetMessageGrid(wxGrid *p, const wxString &sMessage);
  static void UpdateLabelSizes(wxGrid *p);

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
      bRtn = pv->Validate(s,pError);
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
  void OnEditorStart(wxGridEvent &e);
  void OnEditorEnd(wxGridEvent &e);
  void OnCellChange(wxGridEvent &e);
  void OnSelectCell(wxGridEvent &e);
  void OnEditCell(wxGridEvent &e);
  static bool CheckVirtualSize(wxScrolledWindow *p,wxSize *pSize = NULL);
private:
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

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_GRID_START_EDIT, -1)
END_DECLARE_EVENT_TYPES()


#endif
