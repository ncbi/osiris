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

*  FileName: nwxListMRU.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_LIST_MRU_H__
#define __NWX_LIST_MRU_H__

#include "nwx/nwxXmlMRU.h"
#include <wx/string.h>
#include <wx/listctrl.h>
#include <wx/imaglist.h>
#include "nwx/stdb.h"
#include <list>
#include <vector>
#include "nwx/stde.h"

#include "nwx/nsstd.h"

typedef enum
{
  // this needs to be in column order
  MRU_NAME = 0,
  MRU_TYPE,
  MRU_DATE_VIEWED,
  MRU_DATE_MODIFIED,
  MRU_LOCATION,
  __MRU_MAX
}
MRU_SORT;

#ifdef __NWX_LIST_MRU_CPP__
static const wxChar *COLUMNS[] =
{
  // column headings, corresponds to the enum above
  wxS("Name"),
  wxS("Type"),
  wxS("Viewed"),
  wxS("Modified"),
  wxS("Location")
};
#endif

class nwxMRUsort
{
public:
  nwxMRUsort(MRU_SORT n, bool bDescending = false)
  {
    Set(n,bDescending);
  }
  nwxMRUsort()
  {
    Set(MRU_LOCATION);
  }
  nwxMRUsort(const nwxMRUsort &x)
  {
    (*this) = x;
  }

  virtual ~nwxMRUsort() {;}

  nwxMRUsort &operator = (const nwxMRUsort &x)
  {
    Set(x.GetCriteria(),x.IsDescending());
    return *this;
  }
  void ToggleOrder()
  {
    m_bDescending = !m_bDescending;
  }
  void SetDescending(bool bDescending)
  {
    m_bDescending = bDescending;
  }
  void SetCriteria(MRU_SORT n)
  {
    m_criteria = n;
  }
  void Set(MRU_SORT n, bool bDescending = false)
  {
    SetCriteria(n);
    SetDescending(bDescending);
  }
  bool IsDescending() const
  {
    return m_bDescending;
  }
  MRU_SORT GetCriteria() const
  {
    return m_criteria;
  }
private:
  MRU_SORT m_criteria;
  bool m_bDescending;
};

class nwxMRUsortList : public list<nwxMRUsort>
{
public:
  virtual ~nwxMRUsortList() {;}
  nwxMRUsortList()
  {
    nwxMRUsort xx(MRU_DATE_VIEWED,true);
    MRU_SORT ARRAY[] =
    {
      MRU_DATE_VIEWED,
      MRU_LOCATION,
      MRU_NAME,
      MRU_TYPE,
      MRU_DATE_MODIFIED
    };
    size_t nSize(sizeof(ARRAY) / sizeof(ARRAY[0]));
    for(size_t i = 0; i < nSize; i++)
    {
      xx.SetCriteria(ARRAY[i]);
      push_back(xx);
      xx.SetDescending(false);
    }
    SortBy(MRU_DATE_VIEWED,true);
  }
  MRU_SORT GetSortBy() const
  {
    const_iterator itr = begin();
    MRU_SORT nRtn = (*itr).GetCriteria();
    return nRtn;
  }
  bool IsDescending() const
  {
    const_iterator itr = begin();
    bool bRtn = (*itr).IsDescending();
    return bRtn;
  }
  bool SortBy(MRU_SORT n,bool bDescending)
  {
    bool bRtn = true;
    if((GetSortBy() != n) || (IsDescending() != bDescending))
    {
      bRtn = SortBy(n);
      if(bRtn)
      {
        iterator itr = begin();
        (*itr).SetDescending(bDescending);
      }
    }
    return bRtn;
  }
  bool SortBy(MRU_SORT n)
  {
    bool bFound = false;
    bool bTop = true;
    for(iterator itr = begin(); (!bFound) && (itr != end()); ++itr)
    {
      nwxMRUsort &srt(*itr);
      if(srt.GetCriteria() == n)
      {
        bFound = true;
        if(bTop)
        {
          srt.ToggleOrder();
        }
        else
        {
          nwxMRUsort newSort(srt);
          newSort.SetDescending(false);
          erase(itr);
          push_front(newSort);
          break; // prevent ++itr
        }
      }
      bTop = false;
    }
    return bFound;
  }
};

class nwxListMRU : public wxListCtrl
{
public:
  nwxListMRU(nwxXmlMRU *pxMRU, wxWindow *parent, wxWindowID id = wxID_ANY);
  virtual ~nwxListMRU();

  static nwxListMRU *FindByID(long nID);
  // m_nID is the third argument in the
  // sort callback

  bool IsSelected(long i);
  void SetSelected(long i, bool b = true);
  void ClearSelection();
  size_t GetAllSelected(vector<wxString> *pvsFiles);
  wxString GetItem(long i);
  bool GetItem(long i, vector<wxString> *pvsFiles);
  int AddToImageList(void *buffer, size_t bytes);
  bool Load(bool bCheck = false);   // build or rebuild list regardless
  void SetType(const wxString &sExt,const wxString &sText, int nImg = -1);
  const pair<wxString,int> *GetType(const wxString &sFileName);
  int CompareItems(long item1, long item2);
  bool SortList();

  void SetFilter(const wxString &sFilter);
  void OnColumnClick(wxListEvent &e);

  void SortBy(int n,bool bDescending)
  {
    m_SortOrder.SortBy((MRU_SORT)n,bDescending);
    SortList();
  }
  void SortBy(int n)
  {
    m_SortOrder.SortBy((MRU_SORT)n);
    SortList();
  }

  int GetTypeImage(const wxString &sFileName)
  {
    int nRtn = -1;
    const pair<wxString,int> *p = GetType(sFileName);
    if(p != NULL)
    {
      nRtn = p->second;
    }
    return nRtn;
  }
  wxString GetTypeText(const wxString &sFileName)
  {
    wxString sRtn;
    const pair<wxString,int> *p = GetType(sFileName);
    if(p != NULL)
    {
      sRtn = p->first;
    }
    return sRtn;
  }
  size_t Size()
  {
    return m_pMRUfile->Size();
  }
#if 0
/*
  const nwxMRUsortList &GetSortList()
  {
    return m_SortOrder;
  }
*/
#endif
private:
  void _BuildList();
  void _SetupSortIcon(bool bSorted);
  bool _FilterCheck(const wxString &s);
  static wxString _GetExt(const wxString &s);

  vector<wxString> m_vsFilter;
  wxString m_sFilter;
  nwxMRUsortList m_SortOrder;
  wxImageList m_imgList;
  map< wxString, pair<wxString,int> > m_mapTypeToTextImage;
  nwxXmlMRU *m_pMRUfile;

  long m_nID; // unique id for this instance


  int m_nImageDown;
  int m_nImageUp;
  int m_nEmpty;

  static long g_LAST_ID;
  static map<long,nwxListMRU *> g_mapAll; // map of all instances by id, used for sort call back
  DECLARE_EVENT_TABLE()
};


#endif
