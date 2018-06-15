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
*  FileName: CGridRFULimits.h
*  Author:   Douglas Hoffman
*
*  wxGrid for entering RFU limits for sample, ILS, channel, and the 
*    ability to overide sample for each channel
*
*/
#ifndef __C_GRID_RFU_LIMITS__
#define __C_GRID_RFU_LIMITS__

#include "CGridRFUbase.h"
#include "CLabSettings.h"


//********************************************************************
//
//    CGridRFULimits
//
class CGridRFULimits : public CGridRFUbase
{
public:
  CGridRFULimits(wxWindow *parent, wxWindowID id);
  void SetData(CLabThresholds *pData, const wxString &sKitName);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void OnCellChange(wxGridEvent &e);
private:
  enum
  {
    //ROW_RFU_HEADER = 0,
    ROW_RFU_MIN = 0,
    ROW_DETECTION,
    ROW_RFU_INTERLOCUS,
    ROW_RFU_MAX,
    ROW_COUNT
  };

  void _Build();  // called from _SetupKit();
  bool _SetupKit(const wxString &s);
  void _SetRFUColumn(CLabRFU *pRFU, int nCol);
  void _GetRFUColumn(CLabRFU *pRFU, int nCol);
  bool _DisabledChannelCell(int nRow, int nCol)
  {
    bool bRtn =
      (nCol >= m_nCOL_CHANNEL_START) && 
      (nCol < m_nCOL_AFTER_CHANNEL) &&
      (nRow  != ROW_RFU_MIN) &&
      (nRow != ROW_DETECTION) ;
    return bRtn;
  }
  bool _DisabledILSCell(int nRow,int nCol)
  {
    bool bRtn = (nCol == m_nCOL_ILS) &&
      ( nRow == ROW_DETECTION || nRow == ROW_RFU_INTERLOCUS );
    return bRtn;
  }
  bool _DisabledLadderCell(int nRow,int nCol)
  {
    bool bRtn = (nCol == m_nCOL_LADDER) &&
      (nRow == ROW_DETECTION);
    return bRtn;
  }
  virtual bool _DisabledCell(int nRow, int nCol)
  {
    bool bRtn =
      (nCol != m_nCOL_SAMPLE) &&
      (
        _DisabledChannelCell(nRow,nCol) ||
        _DisabledILSCell(nRow,nCol) ||
        _DisabledLadderCell(nRow,nCol)
      );
    return bRtn;
  }
  void _ClearChannelColumns()
  {
    int nRow;
    int nCol;
    for(nCol = m_nCOL_CHANNEL_START; nCol < m_nCOL_AFTER_CHANNEL; ++nCol)
    {
      for(nRow = 0; nRow < ROW_COUNT; ++nRow)
      {
        SetCellValue(nRow,nCol,wxEmptyString);
      }
    }
  }
  void _SetupChannelColumn(int nCol, const wxColor &color, const wxFont &font)
  {
    _SetupChannelCell(ROW_RFU_MIN,nCol,color,font);
    _SetupChannelCell(ROW_DETECTION,nCol,color,font);
  }
  void _SetupDefaultChannelColumn(int nCol)
  {
    _SetupDefaultCell(ROW_RFU_MIN,nCol);
    _SetupDefaultCell(ROW_DETECTION,nCol);
  }
  void _SetupLabelValues(int nRow, int nValue);

  int m_nCOL_SAMPLE;
  int m_nCOL_CHANNEL_START;
  int m_nCOL_AFTER_CHANNEL;
  int m_nCOL_LADDER;
  int m_nCOL_ILS;
  int m_nCOL_COUNT;
  CLabThresholds *m_pData;
};


#endif