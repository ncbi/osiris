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
*  FileName: CGridAlerts.cpp
*  Author:   Douglas Hoffman
*/
#include "mainApp.h"
#include "CGridAlerts.h"
#include "COARfile.h"
#include "nwx/nwxGrid.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxString.h"
#include "nwx/CIncrementer.h"

//*************************************  CGridAlerts

const wxString CGridAlerts::LABEL_CHANNEL("Channel");
const wxString CGridAlerts::LABEL_LOCUS("Locus");
const wxString CGridAlerts::LABEL_ILS("ILS");
const wxString CGridAlerts::LABEL_SAMPLE("Sample");
const int CGridAlerts::TYPE_CHANNEL(1);
const int CGridAlerts::TYPE_LOCUS(2);


CGridAlerts::CGridAlerts(
  COARmessages *pMsgEdit,
  wxWindow *parent,
  wxWindowID id,
  int nType,
  bool bReadOnly) :
    _CGridEdit(parent,id,bReadOnly),
    m_pMsgEdit(pMsgEdit),
    m_nChannelColumn(-1),
    m_nLocusColumn(-1),
    m_nInCellChangeEvent(0)
{
  int bChannel = !!(nType & TYPE_CHANNEL);
  int bLocus = !!(nType & TYPE_LOCUS);
  size_t nCount = m_pMsgEdit->GetMessageCount();
  if(nCount)
  {
    const wxString sDisable("Enabled");
    int nColCount = 2 + bChannel + bLocus;
    int nCol = nColCount;
    CreateGrid((int)nCount,nColCount);
    SetColLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
    SetColLabelValue(--nCol,"Alert");
    SetColLabelValue(--nCol,sDisable);
    if(bChannel)
    {
      SetColLabelValue(--nCol,LABEL_CHANNEL);
      m_nChannelColumn = nCol;
    }
    if(bLocus)
    {
      SetColLabelValue(--nCol,LABEL_LOCUS);
      m_nLocusColumn = nCol;
    }
    SetRowLabelSize(2);
    for(size_t j = 0; j < nCount; j++)
    {
      nCol = nColCount;
      SetCellAlignment((int)j,--nCol,wxALIGN_LEFT,wxALIGN_CENTRE);
      SetBoolCell((int)j,--nCol);
      SetCellAlignment((int)j,nCol,wxALIGN_CENTRE,wxALIGN_CENTRE);
      if(bChannel)
      {
        SetCellAlignment((int)j,m_nChannelColumn,wxALIGN_CENTRE,wxALIGN_CENTRE);
        SetReadOnly((int)j,m_nChannelColumn,true);
      }
      if(bLocus)
      {
        SetCellAlignment((int)j,m_nLocusColumn,wxALIGN_LEFT,wxALIGN_CENTRE);
        SetReadOnly((int)j,m_nLocusColumn,true);
      }
      SetRowLabelValue((int)j,wxEmptyString);
    }
    TransferDataToWindow();
    SetColLabelSize(GetRowSize(0));
  }
  else
  {
    CreateGrid(1,1);
    nwxGrid::SetMessageGrid(this,"There are no " COAR_NOTICE_DISPLAY);
  }
  AutoSizeColumns();
  DisableDragRowSize();
  if(bReadOnly)
  {
    DisableDragColSize();
  }
}

CGridAlerts::~CGridAlerts() {;}

bool CGridAlerts::TransferDataToWindow()
{
  size_t nCount = m_pMsgEdit->GetMessageCount();
  if(nCount)
  {
    const COARmessage *pMsg;
    for(size_t j = 0; j < nCount; j++)
    {
      pMsg = m_pMsgEdit->GetMessage(j,m_dtHistory);
      TransferDataToRow((int)j,pMsg);
    }
  }
  return true;
}
bool CGridAlerts::TransferDataFromWindow()
{
  if(!IsTableReadOnly())
  {
    size_t nCount = m_pMsgEdit->GetMessageCount();
    if(nCount)
    {
      COARmessage *pMsg;
      for(size_t j = 0; j < nCount; j++)
      {
        pMsg = m_pMsgEdit->GetMessage(j);
        TransferDataFromRow((int)j,pMsg);
      }
    }
  }
  return true;
}

void CGridAlerts::SetupLocusColumn(const vector<wxString> &vsLocus)
{

  int nRows = GetNumberRows();
  if((m_nLocusColumn >= 0) && (nRows > 0))
  {
    int nRow = 0;
    vector<wxString>::const_iterator itr;
    for(itr = vsLocus.begin();
      (nRow < nRows) && (itr != vsLocus.end());
      ++itr)
    {
      const wxString &s(*itr);
      SetCellValue(nRow++,m_nLocusColumn,s);
    }
    wxASSERT_MSG(
      (vsLocus.size() == (size_t) nRows),
      "Number of rows does not match number of loci");
  }
  AutoSizeColumns();
}

void CGridAlerts::SetupChannelColumn(
    const COARsample *pSample,
    const map<int,wxString> *pmapChannelNames)
{
  if(m_nChannelColumn >= 0)
  {
    CGridChannelCount vpCount;
    vector<COARchannelAlert *>::const_iterator itrch;
    const vector<COARchannelAlert *> *pchAlerts =
      pSample->GetChannelAlerts();
    const COARchannelAlert *pcha;
    int nChannel;
    size_t nCount;

    // set up vpCount for channel alert grid
    for(itrch = pchAlerts->begin(); itrch != pchAlerts->end(); ++itrch)
    {
      pcha = *itrch;
      nCount = pcha->GetMessageNumbers()->size();
      nChannel = pcha->GetNumber();
      vpCount.Append(nChannel,nCount);
    }
    SetupChannelColumn(
        vpCount,pmapChannelNames);
  }
}

void CGridAlerts::SetupChannelColumn(
  const CGridChannelCount &vpChannelCount,
  const map<int,wxString> *pmapChannelNames)
{
  if(m_nChannelColumn >= 0)
  {
    wxString sChannel;
    int nChannel;
    size_t nCount;
    size_t i;
    int nRow = 0;
    int nROWS = GetNumberRows();
    map<int,wxString>::const_iterator itrCh;
    CGridChannelCount::const_iterator itr;

    for(itr = vpChannelCount.begin();
      (itr != vpChannelCount.end()) && (nRow < nROWS);
      ++itr)
    {
      nChannel = (*itr).first;
      nCount = (*itr).second;
      sChannel.Clear();
      if(pmapChannelNames != NULL)
      {
        itrCh = pmapChannelNames->find(nChannel);
        if(itrCh != pmapChannelNames->end())
        {
          sChannel = itrCh->second;
          if(!sChannel.IsEmpty())
          {
            sChannel.Append("-");
          }
        }
      }
      sChannel.Append(nwxString::FormatNumber(nChannel));
      for(i = 0; (i < nCount) && (nRow < nROWS); i++)
      {
        SetCellValue(nRow,m_nChannelColumn,sChannel);
        nRow++;
      }
    }
  }
}



void CGridAlerts::UpdateDisabledFromRow(
  int nRow, COARmessage *pMsg)
{
  int nCols = GetNumberCols() - 2;
  bool bHidden = !GetBoolValue(nRow,nCols++);
  pMsg->SetHidden(bHidden);
  DisableEdit(nRow,nCols,bHidden);
  Refresh();
}
void CGridAlerts::UpdateTextFromRow(
  int nRow, COARmessage *pMsg)
{
  pMsg->SetText(GetCellValue(nRow,GetNumberCols() - 1));
}
bool CGridAlerts::TransferDataToRow(int nRow, const COARmessage *pMsg)
{
  bool bHidden = pMsg->GetHidden();
  int nCol = GetNumberCols() - 2;
  SetBoolValue(nRow,nCol++,!bHidden);
  SetCellValue(nRow,nCol,pMsg->GetText());
  if(IsTableReadOnly())
  {
    SetGrayBackgroundRow(nRow,bHidden);
  }
  else
  {
    DisableEdit(nRow,nCol,bHidden);
  }
  return true;
}

void CGridAlerts::OnCellChange(wxGridEvent &e)
{
  if( !(m_nInCellChangeEvent || IsTableReadOnly()) )
  {
    CIncrementer x(m_nInCellChangeEvent);
    nwxGridBatch xxxx(this);
    int nRow = e.GetRow();
    int nCol = e.GetCol();
    COARmessage *pMsg = m_pMsgEdit->GetMessage((size_t) nRow);
    int nCols = GetNumberCols();
    const wxString &sName(pMsg->GetMessageName());
    if(nCol == (nCols - 1))
    {
      UpdateTextFromRow(nRow,pMsg);
    }
    else if(nCol == (nCols - 2))
    {
      UpdateDisabledFromRow(nRow,pMsg);
      if((!sName.IsEmpty()) && GetBoolValue(nRow,nCol))
      {
        // a row has been enabled
        // check for mutually exclusive rows
        // that are enabled and disable them

        set<wxString> ss;
        const COARmsgExportMap *pMsgExp = m_pMsgEdit->GetMsgExport();
        if(pMsgExp->GetGroupsByMsgName(sName,&ss))
        {
          // we found group names
          size_t nCount = m_pMsgEdit->GetMessageCount();
          size_t iRow;
          for(iRow = 0; iRow < nCount; iRow++)
          {
            if(iRow == (size_t)nRow)
            {}
            else if(!GetBoolValue(iRow,nCol))
            {} // already unchecked, fuhgeddaboudit.
            else
            {
              COARmessage *pMsgA = m_pMsgEdit->GetMessage(iRow);
              const wxString &sNameA(pMsgA->GetMessageName());
              if(sNameA.IsEmpty())
              {}
              else if( (sNameA == sName) ||
                  (pMsgExp->HasGroupByMsgName(sNameA,ss)) )
              {
                // need to uncheck message
                SetBoolValue(iRow,nCol,false);
                UpdateDisabledFromRow(iRow,pMsgA);
              }
            }
          }
        }
      }
    }
  }
  e.Skip(true);
}
BEGIN_EVENT_TABLE(CGridAlerts,_CGridEdit)
EVT_GRID_CELL_CHANGED(CGridAlerts::OnCellChange)
END_EVENT_TABLE()

