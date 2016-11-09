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
*  FileName: CGridLabThresholds.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __CGRID_LAB_THRESHOLDS_H__
#define __CGRID_LAB_THRESHOLDS_H__

#include <wx/string.h>
#include "nwx/stdb.h"
#include <utility>
#include <set>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxGrid.h"
#include <wx/pen.h>

class CLabThresholds;
class CLabLocusThreshold;

class CGridLabThresholds : public nwxGrid
{
public:
  CGridLabThresholds(
    int nRows,

    bool bILS,
    wxWindow *parent,
    wxWindowID nID);
  virtual ~CGridLabThresholds() {}
  bool IsAllReadOnly()
  {
    return m_bReadOnly;
  }
  bool SetILSDyeName(const wxString &sDyeName);
  virtual wxPen GetColGridLinePen(int nCol);
  virtual wxPen GetRowGridLinePen(int nRow);
  void SetAllReadOnly(bool bReadOnly = true);
  void DisableCell(int nRow, int nCol);
  double GetCellValueDouble(int nRow, int nCol);
  void SetCellValueDouble(int nRow, int nCol, double d);
  static const wxChar * const FRACTION_MAX_PEAK;
  static const wxChar * const PULLUP_FRACTIONAL_FILTER;
  static const wxChar * const STUTTER_THRESHOLD;
  static const wxChar * const PLUS_STUTTER_THRESHOLD;
  static const wxChar * const ADENYLATION_THRESHOLD;
protected:
  void _CreateGrid(int nRows, int nCols);
  void _ClearColumn(int nCol);
  //void _SetColCount(int nCols);
  bool _SetData(CLabThresholds *pData,
    const wxString &sKitName,
    const wxChar * const *psLabels);
  wxString m_sKitName;
  CLabThresholds *m_pData;
  int m_nRows;
  int m_nILScolumn;
  unsigned int m_nILSchannel;
  bool m_bILS;
  bool m_bReadOnly;
  bool m_bCreated;
};


class CGridLabThresholdsSample : public CGridLabThresholds
{
public:
  CGridLabThresholdsSample(
    wxWindow *parent, 
    wxWindowID nID = wxID_ANY);
  virtual ~CGridLabThresholdsSample() {}
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  bool SetData(CLabThresholds *pData, const wxString &sKitName);

private:
  bool _GetColumn(int nCol, CLabLocusThreshold *pLocus);
  void _SetColumn(int nCol, const CLabLocusThreshold &locus);
  enum
  {
    ROW_FRACTION_MAX_PEAK = 1,
    ROW_PULLUP_FRACTIONAL_FILTER,
    ROW_STUTTER_THRESHOLD,
    ROW_PLUS_STUTTER_THRESHOLD,
    ROW_ADENYLATION_THRESHOLD,
    ROW_HETEROZYGOUS_IMBALANCE_LIMIT,
    ROW_MIN_BOUND_HOMOZYGOTE,
    ROWS
  };
};
class CGridLabThresholdsLadder : public CGridLabThresholds
{
public:
  CGridLabThresholdsLadder(
    wxWindow *parent,
    wxWindowID nID = wxID_ANY);
  virtual ~CGridLabThresholdsLadder() {}
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
  bool SetData(CLabThresholds *pData, const wxString &sKitName);
private:
  bool _GetColumn(int nCol, CLabLocusThreshold *pLocus);
  void _SetColumn(int nCol, const CLabLocusThreshold &locus);
  enum
  {
    ROW_FRACTION_MAX_PEAK = 1,
    ROW_PULLUP_FRACTIONAL_FILTER,
    ROW_STUTTER_THRESHOLD,
    ROW_ADENYLATION_THRESHOLD,
    ROWS
  };
};


#endif
