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
*  FileName: COARlocus.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_LOCUS_H__
#define __COAR_LOCUS_H__

#include "COARmisc.h"
#include "COARallele.h"
#include "COARnotes.h"
#include "COARreview.h"
class COARsample;

//************************************************* COARlocus

class COARlocus : public nwxXmlPersist
{
public:
  COARlocus()  : 
    m_pvOldNotes(COARnotes::g_sNode),
    m_ReviewList(true),
    m_AcceptanceList(false),
    m_pSample(NULL)

  {
    RegisterAll(true);
  }
  COARlocus(const COARlocus &x) : 
    m_pvOldNotes(COARnotes::g_sNode),
    m_ReviewList(true),
    m_AcceptanceList(false),
    m_pSample(NULL)
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARlocus(const wxString &sName) :
    m_sLocusName(sName),
    m_pvOldNotes(COARnotes::g_sNode),
    m_ReviewList(true),
    m_AcceptanceList(false),
    m_pSample(NULL)
  {
    RegisterAll(true);
  }
  COARlocus &operator = (const COARlocus &x)
  {
    COARcopy(m_sLocusName);
    COARcopy(m_vnLocusAlerts);
    COARcopyVP(COARallele,m_pvAllele);
    COARcopyVP(COARallele,m_pvAlleleEdited);
    COARcopy(m_sNotes);
    COARcopy(m_pvOldNotes);
    COARcopy(m_ReviewList);
    COARcopy(m_AcceptanceList);
    PostProcessFile();
    return *this;
  }
  virtual ~COARlocus() 
  {
    vectorptr<COARallele>::cleanup(&m_pvAllele);
    vectorptr<COARallele>::cleanup(&m_pvAlleleEdited);
  }
 
  void AppendAlerts(vector<int> *pvn, const wxDateTime *pTime = NULL) const;
  const COARmessages *GetMessages() const;
  size_t AlertCount(const COARmessages *pmsgs, const wxDateTime *pTime = NULL, bool bStopAtOne = false) const;
  size_t AlertCount(const wxDateTime *pTime = NULL, bool bStopAtOne = false) const
  {
    const COARmessages *pmsgs = GetMessages();
    size_t nRtn =
      (pmsgs == NULL) ? 0 : AlertCount(pmsgs,pTime,bStopAtOne);
    return nRtn;
  }
  bool HasAlerts(const COARmessages *pmsgs, const wxDateTime *pTime = NULL) const
  {
    return (AlertCount(pmsgs,pTime,true) > 0);
  }
  bool HasAlerts(const wxDateTime *pTime = NULL) const
  {
    return (AlertCount(pTime,true) > 0);
  }
  size_t CountAlerts(
    const COARmessages *pmsgs, const wxDateTime *pTime = NULL) const;
  bool HasBeenEdited(
    const COARmessages *pmsgs, 
    const COARsample *pSample,
    int nChannel,
    const wxDateTime *pTime = NULL) const;
  bool HasBeenEdited(
    const COARmessages *pmsgs,
    const wxDateTime *pTime = NULL) const
  {
    bool bRtn = false;
    if(m_pSample != NULL)
    {
      int nChannel = GetChannelNr();
      bRtn = HasBeenEdited(pmsgs,m_pSample,nChannel,pTime);
    }
    return bRtn;
  }
  bool HasBeenEdited(const wxDateTime *pTime = NULL) const
  {
    const COARmessages *pmsgs = GetMessages();
    bool bRtn = (pmsgs != NULL) ? HasBeenEdited(pmsgs,pTime) : false;
    return bRtn;
  }
  void GetAllelesByTime(vector<const COARallele *> *pva, const wxDateTime *pTime) const;
  int GetChannelNr() const;
  const wxString &GetName() const
  {
    return m_sLocusName;
  }
  size_t EnabledAlleleCount() const;
  const COARallele *GetAlleleByID(int nID) const;
  COARallele *GetAlleleByID(int nID);
  size_t AlleleCount() const
  {
    return m_pvAllele.size();
  }
  const COARallele *GetAllele(size_t n) const
  {
    return m_pvAllele.at(n);
  }

  COARallele *GetAllele(size_t n)
  {
    return m_pvAllele.at(n);
  }
  static bool IsAmel(const wxString &_s)
  {
    wxString s = _s;
    s.MakeUpper();
    bool bRtn = s.Contains("AMEL");
    return bRtn;
  }
  bool IsAmel() const
  {
    return IsAmel(m_sLocusName);
  }
  const COARnotes *GetNotesPtr(const wxDateTime *pTime = NULL) const
  {
    const COARnotes *pNotes =
      COARnotes::_FindNotes(&m_sNotes,m_pvOldNotes.Get(),pTime);
    return pNotes;
  }
  const wxString &GetNotes(const wxDateTime *pTime = NULL) const
  {
    const COARnotes *pNotes = GetNotesPtr(pTime);
    if(pNotes == NULL)
    {
      return mainApp::EMPTY_STRING;
    }
    return pNotes->GetText();
  }
  wxString FormatReviewAcceptance(const wxDateTime *pTime = NULL) const;
/*
  // commented 1/31/11
  void SetNotes(const wxString &s)
  {
    m_sNotes.SetText(s);
  }
*/
  bool UpdateNotes(const COARnotes &x, const wxDateTime &t);

  void AppendNotes(const wxString &sAppend, const wxString &sUser)
  {
    m_sNotes.Append(sAppend,sUser,m_pvOldNotes.Get());
  }
  const vector<int> *GetAlerts() const
  {
    return m_vnLocusAlerts.Get();
  }

  wxString GetCell(int nLabelType, const wxDateTime *pTime = NULL) const;
  
  bool Update(const COARlocus &Locus, const wxDateTime &t);
  bool UpdateAllele(IOARpeak *pAllele);

  virtual void RegisterAll(bool = false);
  bool operator ==(const COARlocus &x) const;
  bool operator !=(const COARlocus &x) const
  {
    return !((*this) == x);
  }
  void PostProcessFile(COARsample *pSample = NULL);
  COARsample *GetSample()
  {
    return m_pSample;
  }
  const COARsample *GetSample() const
  {
    return m_pSample;
  }

  bool AppendReview(const wxString &sName)
  {
    bool bRtn = m_ReviewList.AppendReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  bool AppendAcceptance(const wxString &sName)
  {
    bool bRtn = m_AcceptanceList.AppendReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  const COARreviewList &GetReviewList() const
  {
    return m_ReviewList;
  }
  const COARreviewList &GetAcceptanceList() const
  {
    return m_AcceptanceList;
  }
  unsigned int GetReviewCount(const wxDateTime *pHistory = NULL) const
  {
    unsigned int nRtn = m_ReviewList.GetCount(pHistory);
    return nRtn;
  }
  unsigned int GetAcceptanceCount(const wxDateTime *pHistory = NULL) const
  {
    unsigned int nRtn = m_AcceptanceList.GetCount(pHistory);
    return nRtn;
  }
  bool NeedAcceptance(
    unsigned int nMin, const wxDateTime *pHistory = NULL) const
  {
    bool bRtn = nMin &&
      !HasBeenEdited(pHistory) &&
      HasAlerts(pHistory) &&
      (nMin > GetAcceptanceCount(pHistory));
    return bRtn;
  }
  bool NeedReview(
    unsigned int nMin, const wxDateTime *pHistory = NULL) const
  {
    bool bRtn = nMin &&
      HasBeenEdited(pHistory) &&
      (nMin > GetReviewCount(pHistory));
    return bRtn;
  }
  void SetIsModified(bool b = true);
  void UpdateVersion();
  bool NeedAttention(
    unsigned int nMinReviewer,
    unsigned int nMinAcceptance) const
  {
    // used to determine if a warning is needed before saving CMF
    bool b = NeedAcceptance(nMinAcceptance) ||
      NeedReview(nMinReviewer);
    return b;
  }
private:

  wxString _GetCellAlleles(const wxDateTime *pTime = NULL) const;
  wxString _GetCellBPS(const wxDateTime *pTime = NULL) const;
  wxString _GetCellRFU(const wxDateTime *pTime = NULL) const;
  wxString _GetCellTime(const wxDateTime *pTime = NULL) const;
  wxString _GetCellPeakArea(const wxDateTime *pTime = NULL) const;
  wxString _GetCellILS_BPS(const wxDateTime *pTime = NULL) const;
  int _FindAlleleByID(int nID) const;

  wxString m_sLocusName;
  COARalerts m_vnLocusAlerts;
  vector<COARallele *> m_pvAllele;
  vector<COARallele *> m_pvAlleleEdited;
  TnwxXmlPersistVector<COARnotes> m_pvOldNotes;
  COARnotes m_sNotes;

  static bool EqualAlleles(
    const vector<COARallele *> *pv1,
    const vector<COARallele *> *pv2);

  TnwxXmlIOPersistVector<COARallele> m_ioallele;
  COARreviewList m_ReviewList;
  COARreviewList m_AcceptanceList;
  COARsample *m_pSample;
};

class CAppendReviewLocus : public IAppendReview
{
public:
  CAppendReviewLocus(COARlocus *p) : m_pLocus(p) {}
  virtual ~CAppendReviewLocus() {}
  virtual bool AppendReview(const wxString &sName)
  {
    bool bRtn = m_pLocus->AppendReview(sName);
    return bRtn;
  }
  virtual bool IsReview() const
  {
    return true;
  }
  virtual bool IsAccept() const
  {
    return !IsReview();
  }
private:
  COARlocus *m_pLocus;

};
class CAppendAcceptanceLocus : public IAppendReview
{
public:
  CAppendAcceptanceLocus(COARlocus *p) : m_pLocus(p) {}
  virtual bool AppendReview(const wxString &sName)
  {
    bool bRtn = m_pLocus->AppendAcceptance(sName);
    return bRtn;
  }
  virtual bool IsReview() const
  {
    return false;
  }
  virtual bool IsAccept() const
  {
    return !IsReview();
  }
private:
  COARlocus *m_pLocus;
};

//************************************************* COARlocusLessByName

class COARlocusLessByName
{
public:
  COARlocusLessByName() {;}
  bool operator ()(const COARlocus &x1, const COARlocus &x2) const
  {
    const wxString &sLocus1(x1.GetName());
    const wxString &sLocus2(x2.GetName());
    int n = sLocus1.CmpNoCase(sLocus2);
    return(n < 0);
  }
  bool operator ()(const COARlocus *px1, const COARlocus *px2) const
  {
    return (*this)(*px1,*px2);
  }
};
#endif
