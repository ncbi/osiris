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
*  FileName: nwxTreebook.h
*  Author:   Douglas Hoffman
*  Purpose:  subclass of wxTreebook with new features:
*             1. ability to change the text and image in a tree item
*             2. ability to automatically set the selected item to bold text
*
*/

#ifndef __NWX_TREEBOOK_H__
#define __NWX_TREEBOOK_H__

#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include <wx/treebook.h>

class nwxTreebook : public wxTreebook
{
public:
  nwxTreebook(
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const wxPoint &pos=wxDefaultPosition,
    const wxSize &size=wxDefaultSize,
    long style=wxBK_DEFAULT,
    bool bAutoBold = false,
    const wxString &name=wxEmptyString) :
      wxTreebook (parent, id, pos,size, style, name),
      m_bAutoBold(bAutoBold)
      {}
    nwxTreebook(wxWindow *parent, bool bAutoBold) :
      wxTreebook (parent, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxBK_DEFAULT | wxNB_LEFT, wxEmptyString),
        m_bAutoBold(bAutoBold)
      {}
    nwxTreebook() : wxTreebook(),
      m_bAutoBold(false)
      {}
    virtual ~nwxTreebook() {}

    virtual bool InsertPage(
      size_t pagePos,
      wxWindow * page,
      const wxString & text,
      bool bSelect = false,
      int imageId = wxNOT_FOUND
      );
    virtual bool InsertSubPage(
      size_t pagePos,
      wxWindow * page,
      const wxString & text,
      bool bSelect = false,
      int imageId = wxNOT_FOUND
      );
    virtual bool AddPage (
      wxWindow *page,
      const wxString &text,
      bool bSelect=false,
      int imageId=wxNOT_FOUND
      );
    virtual bool AddSubPage (
      wxWindow *page,
      const wxString &text,
      bool bSelect=false,
      int imageId=wxNOT_FOUND
      );
    virtual bool DeleteAllPages();
    virtual bool RemovePage(size_t page);

    void SetBold(int nPos,bool bBold);
    void SetBold(wxWindow *pWin, bool bBold)
    {
      SetBold(FindPage(pWin),bBold);
    }
    void OnChangingSetBold(wxBookCtrlEvent &e);

    bool GetAutoBold()
    {
      return m_bAutoBold;
    }
    void SetAutoBold(bool b = true)
    {
      if(b != m_bAutoBold)
      {
        m_bAutoBold = b;
        SetBold(GetSelection(),b);
      }
    }
    bool ValidPosition(int nPos)
    {
      return (nPos >= 0) && ((size_t)nPos < GetPageCount());
    }
    wxTreeItemId GetTreeItem(int nPos)
    {
      wxTreeItemId idRtn;
      _CheckSetupItems();
      if(ValidPosition(nPos))
      {
        idRtn = m_vItems.at((size_t) nPos);
      }
      return idRtn;
    }
    DECLARE_ABSTRACT_CLASS(nwxTreebook)
private:
  void _SetupItem(wxTreeCtrl *pTree, const wxTreeItemId &id, int nLevel);
  void _SetupItems();
  void _CheckSetupItems()
  {
    if(m_vItems.empty())
    {
      _SetupItems();
    }
  }
  void _SetModified()
  {
    m_vItems.clear();
    m_vnLevel.clear();
  }
  //void _BoldReset();
  std::vector<wxTreeItemId> m_vItems;
  std::vector<int> m_vnLevel;  // not needed
  bool m_bAutoBold;
  DECLARE_EVENT_TABLE();
};

#endif