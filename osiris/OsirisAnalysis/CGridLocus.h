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
*  FileName: CGridLocus.h
*  Author:   Douglas Hoffman
*
*   this is used for older file versions where the user could not edit artifacts
*   the newer grid is CGridLocusPeaks
*
*/
#ifndef __C_GRID_LOCUS_H__
#define __C_GRID_LOCUS_H__

#include "CGridEdit.h"
#include "COARfile.h"
#include "CHistoryTime.h"

class wxDC;

class CGridLocus : public _CGridEdit
{
public:
  CGridLocus(
    COARlocus *pLocusEdit,
    wxWindow *parent,
    wxWindowID id,
    bool bReadOnly = false);
  virtual ~CGridLocus();
  bool TransferCellFromWindow(int nRow,int nCol);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  bool SetDateTime(const wxDateTime *pTime,bool bForce = false)
  {
    bool bRtn = m_dtHistory.SetDateTime(pTime);
    if(bRtn || bForce)
    {
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
private:
  void _ImplementColumn(int nCol, bool bAmel);
  void _ImplementColumn(int nCol)
  {
    _ImplementColumn(nCol, m_pLocusEdited->IsAmel());
  }
  void _SetNumberCols(int n);
  void UpdateDisabledAlleles(int nCol);
  bool XferName(int nCol, COARallele *pAllele);
  void XferBPS(int nCol, COARallele *pAllele);
  void XferOL(int nCol, COARallele *pAllele);
  void XferDisabled(int nCol, COARallele *pAllele);
  void XferHomozygous(int nCol, COARallele *pAllele);
  bool TransferColumnFromWindow(int nCol);

  wxString s;
  CHistoryTime m_dtHistory;
  COARlocus *m_pLocusEdited;
public:
  void OnCellChange(wxGridEvent &e);
  DECLARE_EVENT_TABLE()
};

#endif
