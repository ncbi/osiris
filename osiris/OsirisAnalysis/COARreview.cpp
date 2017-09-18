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
*  FileName: COARreview.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "COARreview.h"
#include "nwx/vectorptr.h"

// this should probably be in site settings
const wxChar * const COARreview::TIME_FORMAT(wxS("%Y-%m-%d %H:%M:%S"));

const wxString &IAppendReview::FormatError(wxString *psBuffer, IAppendReview *pRA, const wxString &sUser)
{
  const wxChar *ps;
  if(pRA->IsAccept())
  {
    ps = wxT("accepted");
  }
  else if(pRA->IsReview())
  {
    ps = wxT("reviewed");
  }
  else
  {
    ps = wxT("accepted or approved");  // 20
  }
  psBuffer->Alloc(128);
  *psBuffer = sUser;
  psBuffer->Append(" has already ");  // 13
  psBuffer->Append(ps);
  psBuffer->Append(" this data."); // 11
  return *psBuffer;
}


COARenabled::~COARenabled() {;}
COARreview::~COARreview() {;}

void COARenabled::RegisterAll(bool b)
{
  COARreview::RegisterAll(b);
  RegisterBoolTrue("Enabled",&m_bEnabled);
}

void COARreview::RegisterAll(bool)
{
  RegisterWxString("UserID",&m_sUserID);
  RegisterWxDateTime("Time",&m_time);
}
const wxDateTime &COARreview::GetTime() const
{
  //  mainApp::LogMessage does nothing with level=20000
  //  but removing it causes it to crash on the macintosh, with:
  //     OS X 19.5.8; Darwin Kernel Version 9.8.0:
  //     gcc version 4.0.1 (Apple Inc. build 5465) 
  mainApp::LogMessage("COARreviewGetTime()",20000);
  return m_time;
}

wxString COARreviewList::FormatList(
  bool bReview, const wxDateTime *pHistory) const
{
  wxString sRtn;
  wxString s;
  const wxChar *psSpacer(wxS(""));
  vector<const COARreview *> vl;
  vector<const COARreview *>::iterator itr;
  GetReviews(&vl,NULL,pHistory);
  for(itr = vl.begin();
    itr != vl.end();
    ++itr)
  {
    s = (*itr)->Format(bReview);
    sRtn.Append(psSpacer);
    sRtn.Append(s);
    psSpacer = wxS("\n");
  }
  return sRtn;
}
wxString COARreviewList::FormatReviewAcceptance(
    const COARreviewList &accept,
    const COARreviewList &review,
    const wxDateTime *pTime)
{
  wxString sRtn = accept.FormatList(false,pTime);
  wxString sRtn2 = review.FormatList(true,pTime);
  if(!(sRtn.IsEmpty() || sRtn2.IsEmpty()))
  {
    sRtn.Append("\n");
  }
  sRtn.Append(sRtn2);
  return sRtn;
}

bool COARreviewList::FindName(const wxString &sName)
{
  bool bFound = false;
  vector<COARreview *> *pList = Get();
  for(vector<COARreview *>::iterator itr = pList->begin();
      (!bFound) && (itr != pList->end());
      ++itr)
  {
    const wxString &s((*itr)->GetUserID());
    if(!s.CmpNoCase(sName))
    {
      bFound = true;
    }
  }
  return bFound;
}

bool COARreviewList::AppendReview(const wxString &sName)
{
  COARreview r(sName);
  bool bRtn = AppendReview(r);
  return bRtn;
}

void COARreviewList::Sort()
{
  vectorptrSort<COARreview,COARreviewLess>::Bsort(Get(),m_cmp);
}

void COARreviewList::GetReviews(
  vector<const COARreview *> *pRtnList,
  const wxDateTime *ptLastEdit,
  const wxDateTime *ptHistory) const
{
  pRtnList->reserve(pRtnList->size() + 4);
  const vector<COARreview *> *pv = Get();
  vector<COARreview *>::const_iterator itr;
  for(itr = pv->begin(); itr != pv->end(); ++itr)
  {
    // find last one < t
    const wxDateTime &tItr((*itr)->GetTime());
    if( (ptHistory != NULL) && (tItr > *ptHistory) )
    {
      ; // time of this review is after history time - do nothing
    }
    else if( (ptLastEdit != NULL) && (tItr < *ptLastEdit) )
    {
      ; // time of this review is before last edit - do nothing
    }
    else
    {
      // this is within range
      pRtnList->push_back(*itr);
    }
  }
  vectorptrSort<const COARreview,COARreviewLess>::Bsort(pRtnList,m_cmp);
}
bool COARreviewList::Equal(const COARreviewList &x) const
{
    // it is the responsibility of the caller to make sure
    // each list is sorted
  const vector<COARreview *> *pv1 = Get();
  const vector<COARreview *> *pv2 = x.Get();
  bool bRtn = (pv1->size() == pv2->size());
  if(bRtn)
  {
    const COARreview *p1;
    const COARreview *p2;
    vector<COARreview *>::const_iterator itr1;
    vector<COARreview *>::const_iterator itr2;
    for(itr1 = pv1->begin(), itr2 = pv2->begin();
      bRtn && (itr1 != pv1->end());
      ++itr1,++itr2)
    {
      p1 = *itr1;
      p2 = *itr2;
      if((*p1) != (*p2))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}
int COARreviewList::GetCount(
  const wxDateTime *ptHistory) const
{
  int nRtn = 0;
  if(ptHistory == NULL)
  {
    nRtn = GetCount();
  }
  else
  {
    vector<const COARreview *> vr;
    GetReviews(&vr,NULL,ptHistory);
    nRtn = (int) vr.size();
  }
  return nRtn;
}


bool COARenabledList::Equal(const COARenabledList &x) const
{
    // it is the responsibility of the caller to make sure
    // each list is sorted
  const vector<COARenabled *> *pv1 = Get();
  const vector<COARenabled *> *pv2 = x.Get();
  bool bRtn = (pv1->size() == pv2->size());
  if(bRtn)
  {
    const COARenabled *p1;
    const COARenabled *p2;
    vector<COARenabled *>::const_iterator itr1;
    vector<COARenabled *>::const_iterator itr2;
    for(itr1 = pv1->begin(), itr2 = pv2->begin();
      bRtn && (itr1 != pv1->end());
      ++itr1,++itr2)
    {
      p1 = *itr1;
      p2 = *itr2;
      if((*p1) != (*p2))
      {
        bRtn = false;
      }
    }
  }
  return bRtn;
}
void COARenabledList::GetList(
  vector<const COARenabled *> *pRtnList, const wxDateTime *ptHistory)
  const
{
  const vector<COARenabled *> *pv = Get();
  vector<COARenabled *>::const_iterator itr;
  pRtnList->reserve(pv->size());
  for(itr = pv->begin(); itr != pv->end(); ++itr)
  {
    // find last one < t
    const wxDateTime &tItr((*itr)->GetTime());
    if( (ptHistory == NULL) || (tItr <= *ptHistory) )
    {
      // this is within range
      pRtnList->push_back(*itr);
    }
  }
  vectorptrSort<const COARenabled,COARreviewLess>::Bsort(pRtnList,m_cmp);
}
wxString COARenabledList::FormatList(
  const wxDateTime *pHistory) const
{
  wxString sRtn;
  wxString s;
  const wxChar *psSpacer(wxS(""));
  vector<const COARenabled *> vl;
  vector<const COARenabled *>::iterator itr;
  GetList(&vl,pHistory);
  for(itr = vl.begin();
    itr != vl.end();
    ++itr)
  {
    s = (*itr)->Format();
    sRtn.Append(psSpacer);
    sRtn.Append(s);
    psSpacer = wxS("\n");
  }
  return sRtn;
}

void COARenabledList::Sort()
{
  vectorptrSort<COARenabled,COARreviewLess>::Bsort(Get(),m_cmp);
}
const COARenabled *COARenabledList::GetEnabled(const wxDateTime *pt) const
{
  wxDateTime tCmp((time_t)0);
  wxDateTime tItr((time_t)0);
  vector<COARenabled *>::const_reverse_iterator itr;
  const COARenabled *pRtn(NULL);
  const vector<COARenabled *> *pv = Get();
  const COARenabled *pe;
  for(itr = pv->rbegin(); itr != pv->rend(); ++itr)
  {
    pe = *itr;
    tItr = pe->GetTime();

    // find last one <= t
    if((pRtn == NULL) || (tItr > tCmp))
    {
      if((pt == NULL) || (tItr <= *pt))
      {
        tCmp = tItr;
        pRtn = pe;
      }
    }
  }
  return pRtn;
}
bool COARenabledList::IsEnabled(const wxDateTime *pt) const
{
  bool b = true;
  if(Get()->size() > 0)
  {
    const COARenabled *px(GetEnabled(pt));
    b = (px != NULL) ? px->GetEnabled() : true;
  }
  return b;
}
