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
*  FileName: CGridLocusPeaks.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_GRID_LOCUS_PEAKS_H__
#define __C_GRID_LOCUS_PEAKS_H__

#include "CGridEdit.h"
#include "COARpeak.h"
#include "CHistoryTime.h"
#include "nwx/vectorptr.h"
#include "nwx/nsstd.h"

class wxDC;
class COARpeak;
class COARpeakAny;
class COARsample;

class CGridLocusPeaks : public _CGridEdit
{
public:
  CGridLocusPeaks(
    const COARsample *pSample,
    int nChannel,
    const wxString &sLocusName,
    wxWindow *parent,
    wxWindowID id,
    bool bReadOnly = false,
    const wxDateTime *pTime = NULL);
  virtual ~CGridLocusPeaks();
  bool TransferCellFromWindow(int nRow,int nCol);
  virtual bool TransferDataFromWindow();
  bool TransferDataToSample(COARsample *pSample);
  virtual bool TransferDataToWindow();
  bool SetDateTime(const wxDateTime *pTime,bool bForce = false)
  {
    bool bRtn = m_dtHistory.SetDateTime(pTime);
    if(bRtn || bForce)
    {
      _CleanupPeaks();
      TransferDataToWindow();
    }
    return bRtn;
  }
  bool SetCurrentTime(bool bForce = false)
  {
    return SetDateTime(NULL,bForce);
  }
  const wxDateTime *GetDateTime()
  {
    return m_dtHistory.GetDateTime();
  }
  bool IsModified() const;
private:
  void _ImplementColumn(int nCol);
  int GetPeakCount()
  {
    int n = 0;
    if(m_pvpPeak != NULL)
    {
      n = (int) m_pvpPeak->size();
    }
    return n;
  }
  COARpeak *GetPeak(int nCol)
  {
    COARpeak *pRtn(NULL);
    if( (nCol < GetPeakCount()) && (nCol >= 0) )
    {
      pRtn = m_pvpPeak->at((size_t)nCol);
    }
    return pRtn;
  }
  void _SetNumberCols(int n);
  void _SetupPeaks();
  void _CleanupPeaks();
  void UpdateDisabledAlleles(int nCol);
  
  // need to remove pPeak from Xfer functions
  // because it can be obtained from nCol

  bool XferName(int nCol);
  void XferBPS(int nCol);
  void XferOL(int nCol);
  void XferIsAllele(int nCol);
  void XferIsArtifact(int nCol);
  void XferIsCritical(int nCol);
  void XferUserDisplay(int nCol);
  void XferHomozygous(int nCol);
  bool TransferColumnFromWindow(int nCol);
  bool IsColumnDisabled(int n);
  CHistoryTime m_dtHistory;
  wxString m_sLocusName;
  const COARsample *m_pSample;
  vectorptr<COARpeakAny> *m_pvpPeak;
  vectorptr<COARpeakAny> m_vpPeakCopy;
  int m_nChannel;
  int m_nOnCellChange;
  bool m_bAmel;
public:
  void OnCellChange(wxGridEvent &e);
  DECLARE_EVENT_TABLE()
};

#endif
