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

*  FileName: CListProcess.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CListProcess.h"
#include <wx/imaglist.h>
#include <wx/icon.h>

CListProcess::CListProcess(
  CDirList *pDirList, wxWindow *parent, wxWindowID id, const wxSize &size) :
    wxListCtrl(parent,id,
      wxDefaultPosition, size,
      wxLC_REPORT |wxLC_VRULES),
    m_pDirList(pDirList)
{
}
CListProcess::~CListProcess() {;}

bool CListProcess::TransferDataToWindow()
{
  wxString sTime;
  bool bStartTime = false;
  bool bStopTime = false;
  bool bRtn = true;
  bool bIsShown = IsShownOnScreen();
  if(bIsShown)
  {
    Show(false);
  }

  size_t n = m_pDirList->GetCount();
  long nItem;
  CDirEntry *pDirEntry;
  ClearAll();
  InsertColumn(ITEM_DIR,"Directory",wxLIST_FORMAT_LEFT);
  InsertColumn(ITEM_START,"Start",wxLIST_FORMAT_LEFT);
  InsertColumn(ITEM_STOP,"Stop",wxLIST_FORMAT_LEFT);
  InsertColumn(ITEM_STATUS,"Status",wxLIST_FORMAT_LEFT);
  for(size_t i = 0; (i < n) && bRtn; i++)
  {
    pDirEntry = m_pDirList->At(i);
    const wxString &sInput(pDirEntry->GetInputDir());
    const wxString &sStatus(pDirEntry->GetStatusString());
    nItem = InsertItem((long)i,sInput);
    sTime = pDirEntry->FormatStartTime();
    SetItem(nItem,ITEM_START,sTime);
    if(!sTime.IsEmpty())
    {
      bStartTime = true;
    }

    sTime = pDirEntry->FormatStopTime();
    SetItem(nItem,ITEM_STOP,sTime);
    if(!sTime.IsEmpty())
    {
      bStopTime = true;
    }
    SetItem(nItem,ITEM_STATUS,sStatus);
    SetItemTextColour(nItem,pDirEntry->GetStatusColour());
  }
  SetColumnWidth(ITEM_DIR,wxLIST_AUTOSIZE );
#define HEADER_SIZE 80
  SetColumnWidth(ITEM_START,bStartTime ? wxLIST_AUTOSIZE : HEADER_SIZE);
  SetColumnWidth(ITEM_STOP,bStopTime ? wxLIST_AUTOSIZE : HEADER_SIZE);
#undef HEADER_SIZE
  SetColumnWidth(ITEM_STATUS,wxLIST_AUTOSIZE );
  if(bIsShown)
  {
    Show(true);
  }
  Refresh();
  return bRtn;
}

bool CListProcess::IsSelected(long ndx)
{
  GetInfo(m_itm,ndx);
  long nState = m_itm.GetState();
  bool bRtn = !!(nState & wxLIST_STATE_SELECTED);
  return bRtn;
}
void CListProcess::Select(long ndx, bool bSelect)
{
  GetInfo(m_itm,ndx);
  long nState = m_itm.GetState();
  if(bSelect != !!(nState & wxLIST_STATE_SELECTED))
  {
    if(bSelect)
    {
      nState |= wxLIST_STATE_SELECTED;
    }
    else
    {
      nState ^= wxLIST_STATE_SELECTED;
    }
    m_itm.SetState(nState);
    SetItem(m_itm);
  }
}
void CListProcess::SelectAll(bool bSelect)
{
  long nCount = (long) GetItemCount();
  for(long i = 0; i < nCount; i++)
  {
    Select(i,bSelect);
  }
}
void CListProcess::SelectOne(long ndx)
{
  long nCount = (long) GetItemCount();
  for(long i = 0; i < nCount; i++)
  {
    Select(i,i == ndx);
  }
}
void CListProcess::UpdateStatus(int nItem)
{
  wxString sTime;
  bool bStartTime = false;
  bool bStopTime = false;
  long nMin;
  long nMax;
  long nCount = (long) m_pDirList->GetCount();
  if(nItem >= 0)
  {
    nMin = (long) nItem;
    nMax = nMin;
    nMax++;
    if(nMax > nCount)
    {
      nMax = nCount;
    }
  }
  else
  {
    nMin = 0;
    nMax = nCount;
  }
  wxListItem info;
  CDirEntry *pDirEntry;
  for(long i = nMin; i < nMax; i++)
  {
    pDirEntry = m_pDirList->At((size_t)i);
    const wxString &sStatus(pDirEntry->GetStatusString());
    sTime = pDirEntry->FormatStartTime();
    SetItem(i,ITEM_START,sTime);
    if(!sTime.IsEmpty())
    {
      bStartTime = true;
    }
    sTime = pDirEntry->FormatStopTime();
    SetItem(i,ITEM_STOP,sTime);
    if(!sTime.IsEmpty())
    {
      bStopTime = true;
    }
    SetItem(i,ITEM_STATUS,sStatus);
    SetItemTextColour(i,pDirEntry->GetStatusColour());
  }
  if(bStartTime)
  {
    SetColumnWidth(ITEM_START,wxLIST_AUTOSIZE );
  }
  if(bStopTime)
  {
    SetColumnWidth(ITEM_STOP,wxLIST_AUTOSIZE );
  }
  SetColumnWidth(ITEM_STATUS,wxLIST_AUTOSIZE);
  mainApp::ReRender(GetParent());
//  Refresh();
}
