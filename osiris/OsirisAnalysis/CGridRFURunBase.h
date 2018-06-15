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
*  FileName: CGridRFURunBase.h
*  Author:   Douglas Hoffman
*
*  wxGrid for entering and/or viewing RFU limits for sample, ILS, channel,
*    for an osiris analysis
*
*  This is inherited by CGridRFURun, used for running a new analysis
*  and CGridRFUParam, used for showing runtime parameters
*  The main difference is the former loads data from 
*
*/
#ifndef __C_GRID_RFU_RUN_BASE__
#define __C_GRID_RFU_RUN_BASE__

#include "CGridRFUbase.h"

class CGridRFURunBase : public CGridRFUbase
{
public:
  CGridRFURunBase(wxWindow *parent,wxWindowID id = wxID_ANY);
  bool SetupKit(const wxString &sKitName);

protected:
  enum
  {
    COL_ANALYSIS = 0,
    COL_DETECTION,
    COL_INTERLOCUS,
    COL_COUNT
  };
  void _Build();  // called from _SetupKit();
  virtual bool _DisabledCell(int nRow, int nCol);

  void _TransferToChannelRows(
    const vector<int> &anChannelRFU, 
    const vector<int> &anChannelDetection,
    int nDefaultRFU = -1,
    int nDefaultDetection = -1);
  void _ClearChannelRows()
  {
    int nRow;
    int nCol;
    for(nCol = 0; nCol < COL_COUNT; ++nCol)
    {
      for(nRow = m_nROW_CHANNEL_START; nRow < m_nROW_AFTER_CHANNEL; ++nRow)
      {
        SetCellValue(nRow,nCol,wxEmptyString);
      }
    }
  }

  void _SetupChannelRow(int nRow, const wxColor &color, const wxFont &font)
  {
    _SetupChannelCell(nRow, COL_ANALYSIS,color,font);
    _SetupChannelCell(nRow, COL_DETECTION,color,font);
  }
  void _SetupDefaultChannelRow(int nRow)
  {
    _SetupDefaultCell(nRow, COL_ANALYSIS);
    _SetupDefaultCell(nRow, COL_DETECTION);
  }
  static void SetVectorChannel(vector<int> *pv, size_t nChannel, int nValue)
  {
    while(pv->size() < nChannel) pv->push_back(-1);
    (*pv)[nChannel - 1] = nValue;
  }
  static int GetVectorChannel(const vector<int> &v, size_t nChannel)
  {
    int nRtn = (nChannel <= v.size()) ? v[nChannel - 1] : -1;
    return nRtn;
  }

  int m_nROW_SAMPLE;
  int m_nROW_CHANNEL_START;
  int m_nROW_AFTER_CHANNEL;
  int m_nROW_LADDER;
  int m_nROW_ILS;
  int m_nROW_COUNT;
};

#endif
