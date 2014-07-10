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
*  FileName: CMenuHistory.h
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CMenuHistory.h"
#include "COARfile.h"
#include "nwx/nwxString.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include "CMenuEdit.h"

IMPLEMENT_ABSTRACT_CLASS(CMenuHistory,wxMenu)

CMenuHistory::CMenuHistory(COARfile *pFile, bool bAnalysis) :
  wxMenu(0),
  m_pFile(pFile),
  m_nHistorySize(0),
  m_bAnalysis(bAnalysis)
{
  _Setup();
}

CMenuHistory::CMenuHistory() :
  wxMenu(0),
  m_pFile(NULL),
  m_nHistorySize(0),
  m_bAnalysis(false)
{;}

CMenuHistory::~CMenuHistory() {;}

void CMenuHistory::SetFile(COARfile *pFile,bool bAnalysis)
{
  if( (pFile != m_pFile) || (m_bAnalysis != bAnalysis) )
  {
    m_pFile = pFile;
    m_bAnalysis = bAnalysis;
    _Cleanup();
    _Setup();
  }
}

bool CMenuHistory::IsCurrent()
{
  return m_HistoryTime.IsCurrent();
}

const wxDateTime *CMenuHistory::GetSelected()
{
  return m_HistoryTime.GetDateTime();
}

bool CMenuHistory::_CheckById(int nID)
{
  wxMenuItem *pItem = nID ? FindItem(nID) : NULL;
  bool bRtn = false;
  if(pItem == NULL)
  {
    wxString s("Could not find item in CMenuHistory::_CheckById()\n  time: ");
    s.Append(nwxString::FormatDateTime(*m_HistoryTime.GetDateTime()));
    s.Append("\n  id: ");
    s.Append(nwxString::FormatNumber(nID));
    wxASSERT_MSG(0,s);
    mainApp::LogMessage(s);
  }
  else if(!pItem->IsCheckable())
  {
    wxString s;
    s.Printf(
      "History menu item, %d, offset %d, is not checkable.",
      nID, nID - (int) IDmenuHistory);
    wxASSERT_MSG(0,s);
  }
  else if(!pItem->IsChecked())
  {
    pItem->Check(true);
    bRtn = true;
  }
  return bRtn;
}

const wxDateTime *CMenuHistory::Select(int nID)
{
  if(nID == IDmenuHistoryCurrent)
  {
    m_HistoryTime.SetCurrentTime();
  }
  else if(nID == IDmenuHistoryOriginal)
  {
    m_HistoryTime.SetOriginalTime();
  }
  else if(nID >= IDmenuHistory)
  {
    size_t ndx = (size_t)(nID - IDmenuHistory);
    if(ndx < m_vDate.size())
    {
      wxDateTime dt = m_vDate.at(ndx);
      m_HistoryTime.SetDateTime(&dt);
    }
  }
  _CheckById(nID);
  return m_HistoryTime.GetDateTime();
}

const size_t CMenuHistory::MAXHIST = 7;

bool CMenuHistory::SelectTime(const wxDateTime *pDateTime)
{
  bool bRtn = false;  // return true is a change occurred
  if(!m_HistoryTime.IsEqualTo(pDateTime))
  {
    bRtn = true;
    m_HistoryTime.SetDateTime(pDateTime);
    SetupCheckedItem();
  }
  return bRtn;
}
bool CMenuHistory::SetupCheckedItem()
{
      // find checked item and uncheck it
      //  and check new item
  int idCheck = 0;
  bool bRtn = false; // return true is item is not already checked
  if(m_HistoryTime.IsCurrent()) 
  {
    idCheck = IDmenuHistoryCurrent;
  }
  else if(m_HistoryTime.IsOriginal() && 
    (FindItem(IDmenuHistoryOriginal) != NULL) )
  {
    idCheck = IDmenuHistoryOriginal;
  }
  else if(m_vDate.empty()) {}
  else 
  {
    wxDateTime dtHist = *m_HistoryTime.GetDateTime();
    if(m_vDate.at(m_vDate.size() - 1) > dtHist)
    {
      idCheck = IDmenuHistoryMore;
    }
    else
    {
      vector<wxDateTime>::const_iterator itr;
      int nID = IDmenuHistory;
      for(itr = m_vDate.begin();
        (itr != m_vDate.end() ) && (!idCheck);
        ++itr)
      {
        if(dtHist == *itr)
        {
          idCheck = nID;
        }
        else
        {
          ++nID;
        }
      }
    }
  }
  if(idCheck)
  {
    bRtn = _CheckById(idCheck);
  }
  return bRtn;
}

const wxDateTime *CMenuHistory::UpdateList(bool bForce)
{
  const set<wxDateTime> *pHistory(m_pFile->GetHistory());
  if(bForce || (pHistory->size() != m_nHistorySize))
  {
    wxString s;
    if(m_bAnalysis)
    {
      s = GetLabelText(IDmenuHistoryView);
    }
    _Cleanup();
    _Setup();
    if(m_bAnalysis)
    {
      SetCellLabel(s);
    }
  }
  return m_HistoryTime.GetDateTime();
}
void CMenuHistory::_Cleanup()
{
  size_t nCount = GetMenuItemCount();
  while(nCount > 0)
  {
    --nCount;
    wxMenuItem *pItem = FindItemByPosition(nCount);
    Remove(pItem);
    delete pItem;
  }
  m_vDate.clear();
  m_nHistorySize = 0;
}
bool CMenuHistory::_AppendTime(const wxDateTime &x, int nID, const wxDateTime *pCheck)
{
  wxString sLabel = nwxString::FormatDateTime(x);
  wxMenuItem *pItem = AppendRadioItem(nID,sLabel);
  bool bCheck = (pCheck != NULL) && ((*pCheck) == x);
  m_vDate.push_back(x);
  pItem->Check(bCheck);
  if(bCheck)
  {
    m_HistoryTime.SetDateTime(&x);
  }
  return bCheck;
}
void CMenuHistory::_Setup()
{
  wxString sLabel("Current");
  const set<wxDateTime> *pHistory(m_pFile->GetHistory());
  set<wxDateTime>::const_reverse_iterator itr = pHistory->rbegin();
  const size_t MAXHIST = 7;
  size_t nSize = pHistory->size();
  int nID = IDmenuHistory;
  wxMenuItem *pItem;
  bool bMore = false;
  bool bZeroFound = false;
  //
  //  preserve the current selection
  //  by saving m_HistoryTime
  //
  CHistoryTime dtSave(m_HistoryTime);
  m_nHistorySize = nSize;
  if(nSize > MAXHIST)
  {
    nSize = MAXHIST;
    bMore = true;
  }
  m_vDate.clear();
  m_vDate.reserve(nSize);
  if(m_bAnalysis)
  {
    wxString sLabelAllele("View Allele History...");
    Append(IDmenuHistoryView,sLabelAllele);
    AppendSeparator();
  }
  pItem = AppendRadioItem(IDmenuHistoryCurrent,sLabel);
  pItem->Check(dtSave.IsCurrent());
  for(size_t i = 0; i < nSize; i++)
  {
    _AppendTime(*itr,nID,dtSave);
    ++nID;
    ++itr;
  }
  if(nSize > 0)
  {
    const wxDateTime &dtBegin(m_pFile->GetCreationTime());
    const wxDateTime &dtLow(m_vDate.at(nSize - 1));
    bZeroFound = (!dtLow.GetTicks());
    if( (!bMore) && (dtBegin <  dtLow) )
    {
      _AppendTime(dtBegin,nID,dtSave);
      ++nID;
      if(!dtBegin.GetTicks())
      {
        bZeroFound = true;
      }
    }
  }
  if(!bZeroFound)
  {
    wxDateTime dt0((time_t)0);
    sLabel = nwxString::FormatDateTime(dt0);
    AppendRadioItem(IDmenuHistoryOriginal,sLabel);
  }
  if(bMore)
  {
    sLabel = "More...";
    AppendRadioItem(IDmenuHistoryMore,sLabel);
  }
}
void CMenuHistory::CopyState(CMenuHistory *pTo)
{
  UpdateList();
  pTo->UpdateList();
  if(m_bAnalysis && pTo->m_bAnalysis)
  {
    CMenuEdit::CopyState(pTo,this,IDmenuHistoryView);
  }
  pTo->SelectTime(GetSelected());
}
