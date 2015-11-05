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
*  FileName: CGridRFUbase.h
*  Author:   Douglas Hoffman
*
*  wxGrid for entering or viewing RFU limits for sample, ILS, channel, and the 
*    ability to overide sample for each channel.  This is a base class
*    for CGridRFULimits (LabSettings),
*    CGridRFURun in Analysis dialog class CDialogAnalysis(), and
*    CGridRFUParams for displaying RFU limits in CDialogParameters()
*/
#ifndef __C_GRID_RFU_BASE__
#define __C_GRID_RFU_BASE__

#include "nwx/nwxGrid.h"
#include "CLabSettings.h"
#include "CKitColors.h"

class CGridRFUbase : public nwxGrid
{
public:
  CGridRFUbase(wxWindow *parent, wxWindowID id) :
      nwxGrid(parent,id),
      m_nChannelCount(-1),
      m_nChannelColumnCount(-1),
      m_nILSchannel(-1),
      m_pKitColors(NULL),
      m_bReadOnly(false)
  {}
  void SetAllReadOnly(bool bReadOnly)
  {
    if(bReadOnly != m_bReadOnly)
    {
      m_bReadOnly = bReadOnly;
      _UpdateReadOnly();
    }
  }
  const vector<unsigned int> &GetChannelNumbers() const
  {
    return m_vnChannelNumbers;
  }
protected:
  void _SetupChannelCell(int nRow, int nCol, const wxColor &color, const wxFont &font)
  {
    SetCellFont(nRow,nCol,font);
    SetCellTextColour(nRow,nCol,*wxWHITE);
    SetCellBackgroundColour(nRow,nCol,color);
  }
  void _SetupDefaultCell(int nRow, int nCol)
  {
    SetCellFont(nRow,nCol,GetDefaultCellFont());
    SetCellTextColour(nRow,nCol,GetDefaultCellTextColour());
    SetCellBackgroundColour(nRow,nCol,GetDefaultCellBackgroundColour());
  }
  virtual bool _DisabledCell(int nRow, int nCol) = 0;
  void _UpdateReadOnly();

  bool _SetupKitBase(const wxString &sKitName);
  void _SetCellIntValue(int nRow, int nCol, int nValue);
  int _GetCellIntValue(int nRow, int nCol);
  void _DisableUnused();

  //  data
  CKitColors m_kitColors;
  wxString m_sKitName;
  vector<unsigned int> m_vnChannelNumbers;
  typedef vector<unsigned int>::iterator ChannelNumberIterator;
  int m_nChannelCount;
  int m_nChannelColumnCount;
  int m_nILSchannel;
  const CSingleKitColors *m_pKitColors;
  bool m_bReadOnly;
};

#endif