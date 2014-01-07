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
*  FileName: nwxBookCtrlFocus.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxBookCtrlFocus.h"

#include <wx/object.h>
#include <wx/notebook.h>
#include <wx/listbook.h>
#include <wx/choicebk.h>
#include <wx/treebook.h>
#include <wx/toolbook.h>
#ifdef _DEBUG
#include <wx/debug.h>
#endif
#include <memory>

bool nwxBookCtrlFocus::Focus(wxWindow *pWin)
{
  wxWindow *pPrev = pWin;
  wxWindow *pParent = pPrev->GetParent();
  int ndx;
  bool bRtn = true;
  _CheckSetup();
  while((pParent != NULL) && (_GetTypeIndex(pParent,g_vpClassTop) < 0))
  {
    ndx = _GetTypeIndex(pParent,g_vpClassBook);
    if(ndx < 0) {;}
    else if (!_CheckBookCtrl(pParent,pPrev,ndx))
    {
#ifdef _DEBUG
      if(bRtn)
      {
        // first problem found
        wxASSERT_MSG(0,"nwxBookCtrlFocus::Focus() problem found in _CheckBookCtrl()");
      }
#endif
      bRtn = false;  // unexpected problem, should not happen
    }
    pPrev = pParent;
    pParent = pPrev->GetParent();
  }
  pWin->SetFocus();
  return bRtn;
}

class ICheckBookCtrl
{
public:
  ICheckBookCtrl() {;}
  virtual ~ICheckBookCtrl() {;}
  virtual bool SelectPage(wxWindow *pWin, wxWindow *pPage) = 0;
};

template <class T> class CheckBookCtrl : public ICheckBookCtrl
{
public:
  CheckBookCtrl<T>() {;}
  virtual ~CheckBookCtrl() {;}
  virtual bool SelectPage(wxWindow *pWin, wxWindow *pPage)
  {
    bool bRtn = _SetPage((T*)pWin,pPage);
    return bRtn;
  }
private:
  bool _SetPage(T* pBook, wxWindow *pWin)
  {
    size_t nSize = pBook->GetPageCount();
    wxWindow *pPage;
    bool bRtn = false;
    for(size_t i = 0; i < nSize; i++)
    {
      pPage = (wxWindow *) pBook->GetPage(i);
      if(pPage == pWin)
      {
        bRtn = true;
        pBook->ChangeSelection(i);
        i = nSize;
      }
    }
    return bRtn;
  }
};


void nwxBookCtrlFocus::_SetupClassInfoVector(
  vector<wxClassInfo *> *pv,
  const wxChar **pps,
  size_t nSize)
{
  const wxChar **ps = pps;
  wxClassInfo *pClass;
  pv->clear();
  pv->reserve(nSize);
  for(size_t i = 0; i < nSize; i++)
  {
    pClass = wxClassInfo::FindClass(*ps);
    pv->push_back(pClass);
    ps++;
  }
}

enum
{
  // must be in the same order as psBook in _SetupClassInfo() below
  nNOTEBOOK = 0,
  nLISTBOOK,
  nCHOICEBOOK,
  nTREEBOOK,
  nTOOLBOOK
};

void nwxBookCtrlFocus::_SetupClassInfo()
{
  static const wxChar *psBook[] =
  {
    _T("wxNotebook"),
    _T("wxListbook"),
    _T("wxChoicebook"),
    _T("wxTreebook"),
    _T("wxToolbook")
  };
  static const wxChar *psTop[] =
  {
    _T("wxMDIChildFrame"),
    _T("wxDialog"),
    _T("wxFrame"),
    _T("wxTopLevelWindow")
  };
  const size_t SIZE_BOOK = sizeof(psBook) /sizeof(psBook[0]);
  const size_t SIZE_TOP = sizeof(psTop) / sizeof(psTop[0]);
  _SetupClassInfoVector(&g_vpClassBook,psBook,SIZE_BOOK);
  _SetupClassInfoVector(&g_vpClassTop,psTop,SIZE_TOP);
}

bool nwxBookCtrlFocus::_IsTop(wxWindow *pWin)
{
  int n = _GetTypeIndex(pWin,g_vpClassTop);
  return (n >= 0);
}

int nwxBookCtrlFocus::_GetTypeIndex(
  wxObject *pObj,
  const vector<wxClassInfo *> &vnClass)
{
  int nRtn = -1;
  int i = 0;
  for(vector<wxClassInfo *>::const_iterator itr = vnClass.begin();
    itr != vnClass.end();
    ++itr)
  {
    if((*itr) == NULL) {} // skip this one
    else if(pObj->IsKindOf(*itr))
    {
      nRtn = i;
      break;
    }
    ++i;
  }
  return nRtn;
}

bool nwxBookCtrlFocus::_CheckBookCtrl(wxWindow *pWin, wxWindow *pChild, int ndx)
{
  bool bRtn = false;
  auto_ptr<ICheckBookCtrl> ptr(NULL);
  ICheckBookCtrl *pBook;
  switch (ndx)
  {
  case nNOTEBOOK:
    ptr = auto_ptr<ICheckBookCtrl>(new CheckBookCtrl<wxNotebook>);
    break;
  case nLISTBOOK:
    ptr = auto_ptr<ICheckBookCtrl>(new CheckBookCtrl<wxListbook>);
    break;
  case nCHOICEBOOK:
    ptr = auto_ptr<ICheckBookCtrl>(new CheckBookCtrl<wxChoicebook>);
    break;
  case nTREEBOOK:
    ptr = auto_ptr<ICheckBookCtrl>(new CheckBookCtrl<wxTreebook>);
    break;
  case nTOOLBOOK:
    ptr = auto_ptr<ICheckBookCtrl>(new CheckBookCtrl<wxToolbook>);
    break;
  }
  pBook = ptr.get();
  if(pBook != NULL)
  {
    bRtn = pBook->SelectPage(pWin,pChild);
  }
  return bRtn;
}

vector<wxClassInfo *> nwxBookCtrlFocus::g_vpClassBook;
vector<wxClassInfo *> nwxBookCtrlFocus::g_vpClassTop;
