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
*  FileName: nwxTreebook.cpp
*  Author:   Douglas Hoffman
*  Purpose:  subclass of wxTreebook to
*            automatically set an item to bold text
*            specifically the ability to access a wxTreeItemId associated
*            with the wxTreeCtrl associated with wxTreeBook
*
*/
#include "nwx/nwxTreebook.h"
#include <wx/treectrl.h>

IMPLEMENT_ABSTRACT_CLASS(nwxTreebook,wxTreebook)

bool nwxTreebook::InsertPage(
  size_t pagePos,
  wxWindow * page,
  const wxString & text,
  bool bSelect,
  int imageId
  )
{
  _SetModified();
  bool bRtn = wxTreebook::InsertPage(
    pagePos, page, text, bSelect, imageId);
  return bRtn;
}


bool nwxTreebook::InsertSubPage(
  size_t pagePos,
  wxWindow * page,
  const wxString & text,
  bool bSelect,
  int imageId
  )
{
  _SetModified();
  bool bRtn = wxTreebook::InsertSubPage(
    pagePos, page, text, bSelect, imageId);
  return bRtn;
}



bool nwxTreebook::AddPage(
  wxWindow *page,
  const wxString &text,
  bool bSelect,
  int imageId
  )
{
  _SetModified();
  bool bRtn = wxTreebook::AddPage(page,text,bSelect,imageId);
  return bRtn;
}
bool nwxTreebook::AddSubPage(
  wxWindow *page,
  const wxString &text,
  bool bSelect,
  int imageId
  )
{
  _SetModified();
  bool bRtn = wxTreebook::AddSubPage(page,text,bSelect,imageId);
  return bRtn;
}

bool nwxTreebook::DeleteAllPages()
{
  _SetModified();
  return wxTreebook::DeleteAllPages();
}
bool nwxTreebook::RemovePage(size_t page)
{
  _SetModified();
  return wxTreebook::RemovePage(page);
}

void nwxTreebook::SetBold(int nPos,bool bBold)
{
  wxTreeItemId nID = GetTreeItem(nPos);
  if(nID.IsOk())
  {
    GetTreeCtrl()->SetItemBold(nID,bBold);
  }
}
#if 0
void nwxTreebook::_BoldReset()
{
  int nSel = GetSelection();
  int nCount = (int)GetPageCount();
  bool bBold = false;
  for(int i = 0; i < nCount; i++)
  {
    bBold = m_bAutoBold && (i == nSel);
    SetBold(i,bBold);
  }
}
#endif
void nwxTreebook::OnChangingSetBold(wxBookCtrlEvent &e)
{
  if(m_bAutoBold)
  {
    SetBold(e.GetOldSelection(),false);
    SetBold(e.GetSelection(),true);
  }
  e.Skip();
}
void nwxTreebook::_SetupItem(wxTreeCtrl *pTree,const wxTreeItemId &id, int nLevel)
{
  if(id.IsOk())
  {
    if(nLevel >= 0)
    {
      m_vItems.push_back(id);
      m_vnLevel.push_back(nLevel);
    }
    wxTreeItemIdValue cookie;
    wxTreeItemId idN;
    for(idN = pTree->GetFirstChild(id,cookie);
      idN.IsOk();
      idN = pTree->GetNextChild(id,cookie))
    {
      _SetupItem(pTree,idN,nLevel + 1);
    }
  }
}
void nwxTreebook::_SetupItems()
{
  size_t nCount = GetPageCount();
  _SetModified();
  if(nCount)
  {
    wxTreeCtrl *pTree = GetTreeCtrl();
    long nStyle1 = pTree->GetWindowStyleFlag();
    long nStyle2 = pTree->GetWindowStyle();
    bool bHideRoot = !!((nStyle1 | nStyle2) & wxTR_HIDE_ROOT);
    int nLevel = bHideRoot ? -1 : 0;
    m_vItems.reserve(nCount);
    m_vnLevel.reserve(nCount);
    _SetupItem(pTree,pTree->GetRootItem(),nLevel);
  }
}

BEGIN_EVENT_TABLE(nwxTreebook,wxTreebook)
EVT_TREEBOOK_PAGE_CHANGED(wxID_ANY, nwxTreebook::OnChangingSetBold)
END_EVENT_TABLE()
