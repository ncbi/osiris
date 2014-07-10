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
    _Setup();
    if(m_bAnalysis)
    {
      SetCellLabel(s);
    }
  }
  return m_HistoryTime.GetDateTime();
}
size_t CMenuHistory::_GetInsertPoint()
{
  wxMenuItemList &list(GetMenuItems());
  wxMenuItemList::reverse_iterator itr = list.rbegin();
  wxMenuItem *pItem;
  size_t nSize = list.GetCount();
  size_t nRtn = nSize;
  int nID;
  bool bDone = (nSize == 0);
  while((itr != list.rend()) && (!bDone))
  {
    pItem = *itr;
    nID = pItem->GetId();
    if ((nID >= IDmenuHistory) && (nID < IDmenuHistoryMAX))
    {
      bDone = true;
    }
    else
    {
      nRtn--;
      ++itr;
      bDone = (nID == IDmenuHistoryOriginal);
    }
  }
  if(!bDone)
  {
    nRtn = nSize;
  }
  return nRtn;
}
void CMenuHistory::_CleanupTimes(int nIDmax)
{
  size_t nCount = GetMenuItemCount();
  size_t nPos = nCount;
  int nID;
  wxMenuItem *pItem;

  for(size_t i = 0; i < nCount; i++)
  {
    nPos--;
    pItem = FindItemByPosition(nPos);
    nID = ( (pItem != NULL) && pItem->IsRadio() )
      ? pItem->GetId() : 0;
    if((nID > nIDmax) && (nID < IDmenuHistoryMAX))
    {
      Destroy(pItem);
    }
    else if(nID >= IDmenuHistory)
    {
      break;
    }
  }
}

bool CMenuHistory::_AppendTime(const wxDateTime &x, int nID, const wxDateTime *pCheck)
{
  wxString sLabel = nwxString::FormatDateTime(x);
  bool bCheck = (pCheck != NULL) && ((*pCheck) == x);
  wxMenuItem *pItem = FindItem(nID);
  if(pItem != NULL)
  {
    pItem->SetItemLabel(sLabel);
  }
  else
  {
    size_t nInsertPoint = _GetInsertPoint();
    pItem = InsertRadioItem(nInsertPoint,nID,sLabel);
  }
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
  //
  // in the past, the entire menu was cleared and rebuild, but due to
  //  a bug in wxWidgets 3.0 for wxMac, radio button menu 
  //  items cannot be removed
  //  
  //
  wxString sLabel("Current");
  const set<wxDateTime> *pHistory(m_pFile->GetHistory());
  set<wxDateTime>::const_reverse_iterator itr = pHistory->rbegin();
  const size_t MAXHIST = 7;
  size_t nSize = pHistory->size();
  int nID = IDmenuHistory;
  wxMenuItem *pItem;
  bool bMore = false;

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
    pItem = FindItem(IDmenuHistoryView);
    if(pItem != NULL)
    {
      pItem->SetItemLabel(sLabelAllele);
    }
    else
    {
      Insert(0,IDmenuHistoryView,sLabelAllele);
      InsertSeparator(1);
    }
  }
  else if((pItem = FindItem(IDmenuHistoryView)) != NULL)
  {
    Destroy(pItem);
    pItem = FindItemByPosition(0);
    if(pItem->IsSeparator())
    {
      Destroy(pItem);
    }
  }
  pItem = FindItem(IDmenuHistoryCurrent);
  if(pItem == NULL)
  {
    pItem = AppendRadioItem(IDmenuHistoryCurrent,sLabel);
  }
  pItem->Check(dtSave.IsCurrent());
  for(size_t i = 0; i < nSize; i++)
  {
    if((*itr).GetTicks())
    {
      _AppendTime(*itr,nID,dtSave);
      ++nID;
    }
    ++itr;
  }
#if 0
  if((nSize > 0) && !bMore)
  {
    const wxDateTime &dtBegin(m_pFile->GetCreationTime());
    const wxDateTime &dtLow(m_vDate.at(nSize - 1));
    if(dtBegin.GetTicks() && (dtBegin <  dtLow))
    {
      _AppendTime(dtBegin,nID,dtSave);
      ++nID;
    }
  }
#endif
  wxDateTime dt0((time_t)0);
  _AppendTime(dt0,IDmenuHistoryOriginal,dtSave);
  if(bMore)
  {
    sLabel = "More...";
    AppendRadioItem(IDmenuHistoryMore,sLabel);
  }
  //  now clean up unwanted junk
  _CleanupTimes(nID - 1);
  
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
