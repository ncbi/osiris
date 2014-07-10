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
*  FileName: COARreview.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __COAR_REVIEW_H__
#define __COAR_REVIEW_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
class IAppendReview
{
public:
  virtual ~IAppendReview() {}
  virtual bool AppendReview(const wxString &sName) = 0;
};

class COARreview : public nwxXmlPersist
{
public:
  COARreview()
  {
    RegisterAll(true);
    m_time.Set((time_t)0);
  }
  COARreview(const COARreview &x)
  {
    RegisterAll(true);
    Set(x);
  }
  COARreview(const wxString &sUser)
  {
    RegisterAll(true);
    m_sUserID = sUser;
    m_time.SetToCurrent();
  }
  COARreview(const wxString &sUser, const wxDateTime &t)
  {
    RegisterAll(true);
    m_sUserID = sUser;
    m_time = t;
  }
  virtual ~COARreview();
  void SetTime(const wxDateTime &t)
  {
    m_time = t;
  }
  void SetCurrentTime()
  {
    m_time.SetToCurrent();
  }
  void SetUserID(const wxString &sUser)
  {
    m_sUserID = sUser;
  }
  const wxDateTime &GetTime() const;
  const wxString &GetUserID() const
  {
    return m_sUserID;
  }
  virtual void Set(const COARreview &x)
  {
    m_sUserID = x.m_sUserID;
    m_time = x.m_time;
  }
  COARreview & operator = (const COARreview &x)
  {
    Set(x);
    return (*this);
  }
  bool Equal(const COARreview &x) const
  {
    bool b = (m_sUserID == x.m_sUserID) &&
        (m_time == x.m_time);
    return b;
  }
  bool operator == (const COARreview &x) const
  {
    return Equal(x);
  }
  bool operator != (const COARreview &x) const
  {
    return !Equal(x);
  }
  wxString Format(bool bReview) const
  {
    wxString sRtn;
    const wxChar *psAction =
      bReview ? wxS(" Reviewed by ") : wxS(" Accepted by ");
    _Format(&sRtn,psAction);
    return sRtn;
  }
  static const wxChar * const TIME_FORMAT;
protected:
  virtual void RegisterAll(bool = false);
  void _Format(wxString *ps, const wxChar *psAction) const
  {
    *ps = m_time.Format(TIME_FORMAT);
    ps->Append(psAction);
    if(m_sUserID.IsEmpty())
    {
      ps->Append("[unknown]");
    }
    else
    {
      ps->Append(m_sUserID);
    }
  }
private:
  wxString m_sUserID;
  wxDateTime m_time;
};

class COARenabled : public COARreview
{
public:
  COARenabled() : m_bEnabled(false)
  {
    RegisterAll(true);
  }
  COARenabled(const COARreview &x)
  {
    RegisterAll(true);
    Set(x);
  }
  COARenabled(const wxString &sUser, bool bEnabled = false)
      : COARreview(sUser), m_bEnabled(bEnabled)
  {
    RegisterAll(true);
  }
  COARenabled(
    const wxString &sUser, const wxDateTime &t, bool bEnabled = false)
      : COARreview(sUser,t), m_bEnabled(bEnabled)
  {
    RegisterAll(true);
  }
  virtual ~COARenabled();
  virtual void Set(const COARenabled &x)
  {
    COARreview::Set(x);
    m_bEnabled = x.m_bEnabled;
  }
  COARenabled & operator = (const COARenabled &x)
  {
    Set(x);
    return (*this);
  }
  void SetEnabled(bool b = true)
  {
    m_bEnabled = b;
  }
  void SetDisabled(bool b = true)
  {
    m_bEnabled = !b;
  }
  bool GetEnabled() const
  {
    return m_bEnabled;
  }
  bool IsEnabled() const
  {
    return m_bEnabled;
  }
  bool Equal(const COARenabled &x) const
  {
    bool b = (m_bEnabled == x.m_bEnabled)
      && COARreview::Equal(x);
    return b;
  }
  bool operator == (const COARenabled &x) const
  {
    return Equal(x);
  }
  bool operator != (const COARenabled &x) const
  {
    return !Equal(x);
  }
  wxString Format() const
  {
    wxString sRtn;
    const wxChar *psAction =
      m_bEnabled ? wxS(" Enabled by ") : wxS(" Disabled by ");
    _Format(&sRtn,psAction);
    return sRtn;
  }
protected:
  virtual void RegisterAll(bool b = false);
private:
  bool m_bEnabled;
};

class COARreviewLess
{
public:
  COARreviewLess() {}
  bool operator()(const COARreview * const p1,const COARreview * const p2) const
  {
    bool b = p1->GetTime() < p2->GetTime();
    return b;
  }
};

class COARreviewList : public TnwxXmlPersistVectorSorted<COARreview>, 
  public IAppendReview
{
public:
  COARreviewList(const wxString &sNodeName) :
    TnwxXmlPersistVectorSorted<COARreview>(sNodeName,false)
    {}
  COARreviewList(bool bReview) :
    TnwxXmlPersistVectorSorted<COARreview>
      (bReview ? "Review" : "Acceptance",
      false)
  {};
  COARreviewList() :
    TnwxXmlPersistVectorSorted<COARreview>
      ("Review",false)
  {};

  bool FindName(const wxString &sName);
  virtual void Sort();
  void GetReviews(
    vector<const COARreview *> *pRtnList,
    const wxDateTime *ptLastEdit,
    const wxDateTime *ptHistory = NULL) const;
  bool AppendReview(COARreview *p)
  {
    bool bRtn = !FindName(p->GetUserID());
    if(bRtn)
    {
      _AppendReview(p);
    }
    else
    {
      delete p;
    }
    return bRtn;
  }
  bool AppendReview(const COARreview &r)
  {
    bool bRtn = !FindName(r.GetUserID());
    if(bRtn)
    {
      _AppendReview(new COARreview(r));
    }
    return bRtn;
  }

  // IAppendReview pure virtual
  virtual bool AppendReview(const wxString &sName);

  bool Equal(const COARreviewList &x) const;
  bool operator == (const COARreviewList &x) const
  {
    // it is the responsibility of the caller to make sure
    // each list is sorted
    return Equal(x);
  }
  bool operator != (const COARreviewList &x) const
  {
    // it is the responsibility of the caller to make sure
    // each list is sorted
    return !Equal(x);
  }
  int GetCount() const
  {
    size_t n = Get()->size();
    return (int) n;
  }
  int GetCount(const wxDateTime *ptHistory) const;
  wxString FormatList(bool bReview, const wxDateTime *ptHistory = NULL) const;

  static wxString FormatReviewAcceptance(
    const COARreviewList &accept,
    const COARreviewList &review,
    const wxDateTime *pTime);

private:
  void _AppendReview(COARreview *p)
  {
    Get()->push_back(p);
    Sort();
  }
  COARreviewLess m_cmp;
};

class COARenabledList : public TnwxXmlPersistVectorSorted<COARenabled>
{
public:
  COARenabledList(const wxString &sNodeName) :
    TnwxXmlPersistVectorSorted<COARenabled>(sNodeName,false)
    {}
  COARenabledList() :
    TnwxXmlPersistVectorSorted<COARenabled>("Enable",false)
    {}

  virtual void Sort();
  const COARenabled *GetEnabled(const wxDateTime *pt = NULL) const;
  bool IsEnabled(const wxDateTime *pt = NULL) const;
  void SetEnabled(const wxString &sUser, bool bEnabled)
  {
    if( (!sUser.IsEmpty()) && (bEnabled != IsEnabled(NULL)) )
    {
      Get()->push_back(new COARenabled(sUser,bEnabled));
    }
  }
  bool ToggleEnabled(const wxString &sUser)
  {
    bool bSet = !IsEnabled();
    if(!sUser.IsEmpty())
    {
      SetEnabled(sUser,bSet);
    }
    else
    {
      bSet = !bSet; // toggle back because there will be no change
    }
    return bSet;
  }
  bool Equal(const COARenabledList &x) const;
  bool operator == (const COARenabledList &x) const
  {
    // it is the responsibility of the caller to make sure
    // each list is sorted
    return Equal(x);
  }
  bool operator != (const COARenabledList &x) const
  {
    // it is the responsibility of the caller to make sure
    // each list is sorted
    return !Equal(x);
  }
  void GetList(vector<const COARenabled *> *pn, const wxDateTime *ptHistory = NULL) const;
  wxString FormatList(const wxDateTime *ptHistory) const;
private:
  COARreviewLess m_cmp;
};



#endif
