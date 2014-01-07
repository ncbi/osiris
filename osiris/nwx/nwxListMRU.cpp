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

*  FileName: nwxListMRU.cpp
*  Author:   Douglas Hoffman
*
*/
#define __NWX_LIST_MRU_CPP__
#include <wx/bitmap.h>
#include <wx/colour.h>
#include <wx/filename.h>
#include <wx/log.h>
#include "nwxSysOpt.h"
#include "nwxListMRU.h"
#include "nwxStaticBitmap.h"
#include "CArrowImages.h"

//#ifdef __WXMAC__
//#include <wx/sysopt.h>
//#endif

long nwxListMRU::g_LAST_ID(0);
map<long,nwxListMRU *> nwxListMRU::g_mapAll; // map of all instances by id, used for sort call back


nwxListMRU::nwxListMRU(
  nwxXmlMRU *pxMRU, wxWindow *parent, wxWindowID id) :
    m_imgList(16,16,true,8),
    m_pMRUfile(pxMRU)
{
  // initialize ID
  nwxSysOpt::Setup();
  Create(parent,id,
      wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
  g_LAST_ID++;
  m_nID = g_LAST_ID;
  g_mapAll.insert(map<long,nwxListMRU *>::value_type(m_nID,this));


  // set up column headings

  for(long nCol = 0; nCol < (long) __MRU_MAX; ++nCol)
  {
    InsertColumn(nCol,COLUMNS[nCol]);
  }

  // setup arrow images
#define IMG_ARGS(x) (void *)x,sizeof(x)/sizeof(x[0])
    m_nImageUp =   AddToImageList( IMG_ARGS(sUarr) );
    m_nImageDown = AddToImageList( IMG_ARGS(sDarr) );
    m_nEmpty =     -1; // AddToImageList( IMG_ARGS(sEmpty));
#undef IMG_ARGS

}

nwxListMRU::~nwxListMRU()
{
  map<long,nwxListMRU *>::iterator itr = g_mapAll.find(m_nID);
  if(itr != g_mapAll.end())
  {
    g_mapAll.erase(itr);
  }
}




nwxListMRU *nwxListMRU::FindByID(long nID)
{
  map<long,nwxListMRU *>::iterator itr = g_mapAll.find(nID);
  nwxListMRU *pRtn( (itr != g_mapAll.end()) ? itr->second : NULL );
  return pRtn;
}

bool nwxListMRU::IsSelected(long i)
{
  int nState = GetItemState(i,wxLIST_STATE_SELECTED);
  int nSelected = nState & wxLIST_STATE_SELECTED;
  return !!nSelected;
}
void nwxListMRU::SetSelected(long i, bool b)
{
  long state = b ? wxLIST_STATE_SELECTED : 0;
  SetItemState(i,state,wxLIST_STATE_SELECTED);
}
void nwxListMRU::ClearSelection()
{
  long nCount = GetItemCount();
  long i;
  for(i = 0; i < nCount; i++)
  {
    SetItemState(i,0,wxLIST_STATE_SELECTED);
  }
}

wxString nwxListMRU::GetItem(long i)
{
  wxString sRtn;
  const vector<nwxMRU *> *pvMRU(m_pMRUfile->GetVector());
  long ndx = GetItemData(i);
  long nSize = (long)pvMRU->size();
  if(ndx >= 0 && ndx < nSize)
  {
    sRtn = pvMRU->at((size_t) ndx)->GetFullPath();
  }
  return sRtn;
}
bool nwxListMRU::GetItem(long i, vector<wxString> *pvsFiles)
{
  wxString s = GetItem(i);
  bool bRtn = !s.IsEmpty();
  if(bRtn)
  {
    pvsFiles->push_back(s);
  }
  return bRtn;
}
size_t nwxListMRU::GetAllSelected(vector<wxString> *pvsFiles)
{
  wxString s;
  int nCount = GetItemCount();
  long i;
  pvsFiles->clear();
  pvsFiles->reserve((size_t)GetSelectedItemCount());

  for(i = 0; i < nCount; i++)
  {
    if(IsSelected(i))
    {
      GetItem(i,pvsFiles);
    }
  }
  return pvsFiles->size();
}

int nwxListMRU::AddToImageList(void *buffer, size_t bytes)
{
  wxBitmap bitmap;
  bool b = nwxStaticBitmap::LoadImage(buffer,bytes,&bitmap,wxBITMAP_TYPE_PNG);
  int nRtn = b ? m_imgList.Add(bitmap,*wxWHITE) : false;
  return nRtn;
}

bool nwxListMRU::Load(bool bCheck)
{
  // if bCheck is true, then Load list only if needed
  bool bReload =
    bCheck 
    ? m_pMRUfile->CheckFileModification(true)
    : m_pMRUfile->ReloadFile();
  bool bRtn =
    (!bCheck) ||
    bReload ||
    ( (size_t)GetItemCount() != m_pMRUfile->Size() )
    ;

  if( bRtn )
  {
    _BuildList();
  }
  return bRtn;
}

void nwxListMRU::SetFilter(const wxString &sFilter)
{
  wxString sLower = sFilter;
  sLower.MakeLower();
  while(sLower.Replace(_T("**"),_T("*"),true)) {};
  if(sLower.EndsWith(_T("*")))
  {
    sLower.Truncate(sLower.Len() - 1);
  }
  if(m_sFilter != sLower)
  {
    m_sFilter = sLower;
    nwxString::Split(m_sFilter.c_str(),&m_vsFilter,"*");
    if(IsShown())
    {
      Load(false);
    }
  }
}
bool nwxListMRU::_FilterCheck(const wxString &_s)
{
  bool bRtn = true;
  if(m_vsFilter.size())
  {
    wxString sLower(_s);
    sLower.MakeLower();
    const char *ps = sLower.c_str();
    const char *psFind = ps;
    for(vector<wxString>::iterator itr = m_vsFilter.begin();
      bRtn && (itr != m_vsFilter.end());
      ++itr)
    {
      wxString &s(*itr);
      if(!s.IsEmpty())
      {
        psFind = strstr(psFind,s.c_str());
        if(psFind == NULL)
        {
          bRtn = false;
        }
        else
        {
          psFind += s.Len();
        }
      }
    }
  }
  return bRtn;
}

wxString nwxListMRU::_GetExt(const wxString &sFileName)
{
  wxString sExt;
  int nDot = sFileName.Find(wxChar('.'),true);
  if(nDot != wxNOT_FOUND)
  {
    nDot++;
    sExt = sFileName.Mid(nDot);
  }
  else
  {
    sExt = sFileName;
  }
  sExt.MakeLower();
  return sExt;
}

void nwxListMRU::SetType(const wxString &sExt, const wxString &sText, int nImg)
{
  wxString sExtl(_GetExt(sExt));
  bool bInsert = true;
  map< wxString, pair<wxString,int> >::iterator itr = m_mapTypeToTextImage.find(sExt);
  if(itr != m_mapTypeToTextImage.end())
  {
    if(
      (itr->second.second != nImg) ||
      (itr->second.first != sText)
      )
    {
      m_mapTypeToTextImage.erase(itr);
    }
    else
    {
      bInsert = false;
    }
  }
  if(bInsert)
  {
    pair<wxString,int> v(sText,nImg);
    m_mapTypeToTextImage.insert(
      map< wxString, pair<wxString,int> >::value_type(sExtl,v));
  }
}
const pair<wxString,int> *nwxListMRU::GetType(const wxString &sFileName)
{
  wxString sExt(_GetExt(sFileName));
  const pair<wxString,int> *pRtn(NULL);
  if(!sExt.IsEmpty())
  {
    map< wxString, pair<wxString,int> >::iterator itr =
      m_mapTypeToTextImage.find(sExt);
    if(itr != m_mapTypeToTextImage.end())
    {
      pRtn = &(itr->second);
    }
  }
  return pRtn;
}



void nwxListMRU::_BuildList()
{
  wxString s;
  wxString sName;
  wxString sType;
  wxString sExt;
  const pair<wxString, int> *ppType;
  const vector<nwxMRU *> *pvMRU(m_pMRUfile->GetVector());
  const nwxMRU *pMRU;
  long nItem = 0;
  long nItemRtn;
  long nItemNdx = 0;
  int nImg;
#ifdef __WXDEBUG__
  int imgCount = m_imgList.GetImageCount();
  s.Printf("Image Count %d",imgCount);
  wxLog::OnLog(wxLOG_Message,(const wxChar *)s,time(NULL));
  s.Clear();
#endif


  SetImageList(&m_imgList,wxIMAGE_LIST_SMALL);
  SetImageList(&m_imgList,wxIMAGE_LIST_NORMAL);
  DeleteAllItems();
  for(vector<nwxMRU *>::const_iterator itr = pvMRU->begin();
    itr != pvMRU->end();
    ++itr)
  {
    pMRU = *itr;
    wxFileName fn(pMRU->GetFullPath());
    sName = fn.GetFullName();
    if(_FilterCheck(sName))
    {
      ppType = GetType(sName);
      if(ppType == NULL)
      {
        sType.Empty();
        nImg = -1;
      }
      else
      {
        sType = ppType->first;
        nImg = ppType->second;
      }
      nItemRtn = InsertItem(nItemNdx,wxEmptyString);
      nItemNdx++;

      if(nItemRtn >= 0)
      {
        SetItem(nItemRtn,MRU_NAME,sName,nImg);
        SetItem(nItemRtn,MRU_TYPE,sType);
        s = pMRU->GetAccessTimeFormat();
        SetItem(nItemRtn,MRU_DATE_VIEWED,s);
        s = pMRU->GetModTimeFormat();
        SetItem(nItemRtn,MRU_DATE_MODIFIED,s);
        s = fn.GetPath();
        SetItem(nItemRtn,MRU_LOCATION,s);
        SetItemData(nItemRtn,nItem);
      }
      else
      {
        s.Printf(
          _T("Error occurred when adding MRU item %ld\n%s"),
          nItem,fn.GetFullPath().c_str() );
        wxASSERT_MSG(false,s);
        time_t t;
        time(&t);
        wxLog::OnLog(wxLOG_Message,(const wxChar *)s,t);
        break;
      }
    }
    nItem++;
  }
  SortList();
  for(int i = 0; i < __MRU_MAX; i++)
  {
    SetColumnWidth(i, wxLIST_AUTOSIZE);
  }
  int nWidth = GetColumnWidth(MRU_TYPE);
  if(nWidth > 0)
  {
    nWidth += 24;
    SetColumnWidth(MRU_TYPE,nWidth);
  }
}

static int wxCALLBACK __ListCompareFunction(long item1, long item2, long sortData)
{
  nwxListMRU *pList = nwxListMRU::FindByID(sortData);
  int nRtn = 0;
  if(pList != NULL)
  {
    nRtn = pList->CompareItems(item1,item2);
  }
  return nRtn;
}

bool nwxListMRU::SortList()
{
  bool bRtn = SortItems(__ListCompareFunction,m_nID);
  _SetupSortIcon(bRtn);
  return bRtn;
}
int nwxListMRU::CompareItems(long item1, long item2)
{
  const vector<nwxMRU *> *pv = m_pMRUfile->GetVector();
  nwxMRU *pm1 = pv->at((size_t)item1);
  nwxMRU *pm2 = pv->at((size_t)item2);
  int nRtn = 0;
  for(nwxMRUsortList::iterator itr = m_SortOrder.begin();
    (!nRtn) && itr != m_SortOrder.end();
    ++itr)
  {
    switch((*itr).GetCriteria())
    {
      case MRU_NAME:
        nRtn = pm1->CompareName(*pm2);
        break;
      case MRU_TYPE:
        {
          wxString s1 = GetTypeText(pm1->GetFullPath());
          wxString s2 = GetTypeText(pm2->GetFullPath());
          nRtn = s1.CmpNoCase(s2);
        }
        break;
      case MRU_DATE_VIEWED:
        nRtn = pm1->CompareTimeAccess(*pm2);
        break;
      case MRU_DATE_MODIFIED:
        nRtn = pm1->CompareTimeModified(*pm2);
        break;
      case MRU_LOCATION:
        nRtn = pm1->CompareLocation(*pm2);
        break;
      default:
        break;
    }
    if(nRtn && (*itr).IsDescending())
    {
      nRtn = -nRtn;
    }
  }
  return nRtn;
}
void nwxListMRU::_SetupSortIcon(bool bSorted)
{
  wxListItem item;
  nwxMRUsortList::iterator itr = m_SortOrder.begin();
  int nSortCol = (int)(*itr).GetCriteria();
  bool bDescending = (*itr).IsDescending();
  int nImage = -1;
  int nThisImage = -1;
  int nCols = GetColumnCount();
  if(!bSorted)
  {
    nImage = m_nEmpty;
  }
  else if(bDescending)
  {
    nImage = m_nImageDown;
  }
  else
  {
    nImage = m_nImageUp;
  }

  item.SetMask(wxLIST_MASK_IMAGE);
  for(int i = 0; i < nCols; i++)
  {
    nThisImage = (i == nSortCol) ? nImage : m_nEmpty;
    item.SetImage(nThisImage);
    SetColumn(i, item);
  }
}

void nwxListMRU::OnColumnClick(wxListEvent &e)
{
  int n = e.GetColumn();
  if(n >= 0) {SortBy(n);}
}

BEGIN_EVENT_TABLE(nwxListMRU,wxListCtrl)
EVT_LIST_COL_CLICK(wxID_ANY,nwxListMRU::OnColumnClick)
END_EVENT_TABLE()
