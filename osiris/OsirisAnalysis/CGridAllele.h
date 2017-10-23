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
*  FileName: CGridAllele.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_GRID_ALLELE_H__
#define __C_GRID_ALLELE_H__

#include "nwx/nwxGrid.h"
#include "nwx/nwxString.h"
#include "CGridLocusColumns.h"
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include "nwx/nsstd.h"
#include "nwx/stde.h"

class CLabLocusCollection;
class CLabLocus;
class CLabPositiveControls;
class CLabPositiveControl;

class CGridAlleleBase : public nwxGrid
{
public:
  enum
  {
    // used for nCellType
    ALLELE_SINGLE = 1, // one allele per cell
    ALLELE_MULTIPLE,   // one or more alleles per cell
    ALLELE_TRIPLE      // three alleles per cell
  };

  CGridAlleleBase(
    wxWindow *parent, 
    wxWindowID id, 
    int nCellType = CGridAlleleBase::ALLELE_SINGLE,
    int nPreAlleleColumns = 0) :
      nwxGrid(parent,id),
      m_psLastError(wxS("")),
      m_nAmelColumn(-1),
      m_nPreAlleleColumns(nPreAlleleColumns),
      m_nCellType(nCellType),
      m_bLastValidationOK(true)
  {}
  void Clear();
  static const wxChar * const TRIALLELE_FIT;
  static const wxChar * const ALLELE_FIT;
protected:
  static CLabLocus *_NewLabLocus(
    vector<CLabLocus *> *pvpLocus, const wxString &sLocusName);
  static CLabLocus *_BuildLabLocus(
    vector<CLabLocus *> *pvpLocus,
    const wxString &sLocusName,
    const wxString &sAlleles, 
    bool bRequire3 = true);

  virtual bool AutoExpand();
  void CheckRowCount();
  void _FitString(const wxString &s, 
    int nStartColumn = -1);
  void _FitString()
  {
    _FitString(
      (m_nCellType == ALLELE_SINGLE) ? ALLELE_FIT : TRIALLELE_FIT,
      m_nPreAlleleColumns);
  }
  
  bool SetupKit(const wxString &sKitName, bool bAllowAmel);
  int _GetLocusColumn(const wxString &s);

  void CellChangeExpand(wxGridEvent &e);
  virtual bool ValidateCell(
    int nRow, int nCol, wxString *psPrompt);

  wxString m_sSaveCellValue;
  vector<int> m_vnLastRowUsed;
  map<wxString,int,nwxStringLessNoCase> m_mapLocusColumn;
  vector<wxString> m_vsLeftColumns;
  const wxChar *m_psLastError;
  int m_nAmelColumn;
  int m_nPreAlleleColumns;
  int m_nCellType;
  bool m_bLastValidationOK;
  DECLARE_EVENT_TABLE()
};



class CGridAllele : public CGridAlleleBase
{
public:
  CGridAllele(
    wxWindow *parent, wxWindowID id, 
    int nRows, bool bTriAlleles, bool bAllowAmel = true) : 
  CGridAlleleBase(
      parent,
      id,
      bTriAlleles 
        ? CGridAlleleBase::ALLELE_TRIPLE
        : CGridAlleleBase::ALLELE_SINGLE,
        0),
    m_pData(NULL),
    m_bTriAlleles(bTriAlleles),
    m_bAllowAmel(bAllowAmel && !bTriAlleles)
  {
    CreateGrid(nRows,1);
    //SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);
    SetRowLabelSize(2);
  }
  virtual ~CGridAllele()
  {}
  void SetData(CLabLocusCollection *pData,
    const wxString &sKitName)
  {
    m_pData = pData;
    SetupKit(sKitName,m_bAllowAmel);
    _FitString();
  }
  CLabLocusCollection *GetData()
  {
    return m_pData;
  }
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
protected:
  // single alleles
  bool _TransferDataToWindow1();
  bool _TransferDataFromWindow1(
    vector<CLabLocus *> *pvpLocus);
  // tri alleles
  bool _TransferDataToWindow3();
  bool _TransferDataFromWindow3(
    vector<CLabLocus *> *pvpLocus, 
    int nStartCol = 0, bool bRequire3 = true);
  
  CLabLocusCollection *m_pData;
  bool m_bTriAlleles;
  bool m_bAllowAmel;
};

class CGridAllelePosCtrl : public CGridAlleleBase
{
public:
  CGridAllelePosCtrl(wxWindow *parent, wxWindowID id, int nRows) :
      CGridAlleleBase(
        parent,id,CGridAlleleBase::ALLELE_MULTIPLE,2),
      m_pData(NULL),
      m_nRowNextControl(-1),
      m_bReadOnly(false)

  {
    CreateGrid(nRows,1);
    SetRowLabelSize(2);
    m_vsLeftColumns.reserve(2);
    m_vsLeftColumns.push_back("Control Name");
    m_vsLeftColumns.push_back("File Name Search");
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetGridReadOnly(bool bReadOnly = true);

  void SetData(CLabPositiveControls *pData,
    const wxString &sKitName)
  {
    m_pData = pData;
    SetupKit(sKitName,true);
    _FitString(TRIALLELE_FIT,0);
  }
  CLabPositiveControls *GetData()
  {
    return m_pData;
  }
  virtual bool ValidateCell(
    int nRow, int nCol, wxString *psPrompt);
protected:
  virtual bool AutoExpand();
private:
  void OnCellSelect(wxGridEvent &e);
  bool _NameIsUsed(int nRow, const wxString &s = wxEmptyString);
  bool _PromptDeleteRow(int nRow);
  void _SetRowEnabled(int nRow, bool bEnabled = true);
  void _SetRowNextControl(int nRow = -1);
  void _DeleteRow(int nRow);
  int _FindNextControlRow(int nStartRow = -1);

  CLabPositiveControl *_GetPosCtrl();

  CLabPositiveControls *m_pData;
  int m_nRowNextControl;
  bool m_bReadOnly;
  DECLARE_EVENT_TABLE()
};


#endif
