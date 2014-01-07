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
*  FileName: CGridAlerts.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_GRID_ALERTS_H__
#define __C_GRID_ALERTS_H__

#include "nwx/stdb.h"
#include <vector>
#include <utility>
#include <map>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "CGridEdit.h"
#include "CHistoryTime.h"

class COARmessage;
class COARmessages;
class COARsample;

class CGridChannelCount : public vector< pair<int,size_t> >
{
public:
  void Append(int nChannel, size_t nCount)
  {
    pair<int,size_t> x(nChannel,nCount);
    push_back(x);
  }
};

class CGridAlerts : public _CGridEdit
{
public:
  static const int TYPE_CHANNEL;
  static const int TYPE_LOCUS;
  static const wxString LABEL_CHANNEL;
  static const wxString LABEL_ILS;
  static const wxString LABEL_SAMPLE;
  static const wxString LABEL_LOCUS;

  CGridAlerts(
    COARmessages *pMsgEdit,
    wxWindow *parent,
    wxWindowID id,
    int nType = 0,
    bool bReadOnly = false);
  virtual ~CGridAlerts();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SizeGrid()
  {
    int nXv;
    int nYv;
    int nX;
    int nY;
    GetVirtualSize(&nXv,&nYv);
    GetSize(&nX,&nY);
    if(nXv > nX)
    {
      SetSize(nXv,nY);
    }
  }
  bool SetDateTime(const wxDateTime *pTime, bool bForce = false)
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
  void SetupLocusColumn(const vector<wxString> &vsLocus);
  void SetupChannelColumn(
    const COARsample *pSample,
    const map<int,wxString> *pmapChannelNames = NULL);
  void SetupChannelColumn(
    const CGridChannelCount &vpChannelCount,
    const map<int,wxString> *pmapChannelNames = NULL);


private:
  void UpdateDisabledFromRow(int nRow, COARmessage *pMsg);
  void UpdateTextFromRow(int nRow, COARmessage *pMsg);
  bool TransferDataToRow(int nRow, const COARmessage *pMsg);
  bool TransferDataFromRow(int nRow,COARmessage *pMsg)
  {
    UpdateDisabledFromRow(nRow,pMsg);
    UpdateTextFromRow(nRow,pMsg);
    return true;
  }
  COARmessages *m_pMsgEdit;
  CHistoryTime m_dtHistory;
  int m_nChannelColumn;
  int m_nLocusColumn;
  int m_nInCellChangeEvent;

public:
  void OnCellChange(wxGridEvent &e);
  DECLARE_EVENT_TABLE()
};

#endif
