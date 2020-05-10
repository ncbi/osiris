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
*  FileName: COARsample.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_SAMPLE_H__
#define __COAR_SAMPLE_H__
#include "nwx/vectorptr.h"
#include "nwx/nwxRound.h"
#include "COARmisc.h"
#include "COARpeak.h"
#include "COARreview.h"
#include "COARnotes.h"
#include "COARlocus.h"
#include "COARchannelAlert.h"
#include "COARsampleInfo.h"
#include "CLabSettings.h"
#include "Platform.h"
class COARchannel;
class COARfile;
//************************************************* COARinterlocusAlert


class COARinterlocusAlert : public nwxXmlPersist
{
  // this class is used by COARsample
  // but will probably not be used in the XML and
  // will eventually be discarded
public:
  COARinterlocusAlert()
  {
    RegisterAll(true);
  }
  COARinterlocusAlert(const COARinterlocusAlert &x)
  {
    (*this) = x;
    RegisterAll(true);
  }
  virtual ~COARinterlocusAlert() {;}
  virtual void RegisterAll(bool = false);
  size_t Size() const
  {
    return m_vsLocusName.size();
  }
  const wxString &GetLocus(size_t n) const
  {
    return m_vsLocusName.at(n);
  }
  const int GetMessageNumber(size_t n) const
  {
    return m_vnMessageNumber.at(n);
  }
private:
  vector<wxString> m_vsLocusName;
  vector<int> m_vnMessageNumber;
};
//************************************************** COARsampleReview
class COARsampleReviewAccept : public nwxXmlPersist
{
public:
  COARsampleReviewAccept(bool bReview = true) :
      m_SampleList(bReview),
      m_ChannelList(bReview),
      m_ILSList(bReview)
  {
    RegisterAll(true);
  }
  COARsampleReviewAccept(const COARsampleReviewAccept &x)
  {
    (*this) = x;
    RegisterAll(true);
  }
  COARsampleReviewAccept &operator = (const COARsampleReviewAccept &x)
  {
    COARcopy(m_SampleList);
    COARcopy(m_ChannelList);
    COARcopy(m_ILSList);
    return *this;
  }

  virtual bool Skip(void *pObj);
  virtual bool Skip();

  void GetSampleReviews(
    vector<const COARreview *> *pRtnList,
    const wxDateTime *ptLastEdit,
    const wxDateTime *ptHistory = NULL) const
  {
    m_SampleList.GetReviews(pRtnList,ptLastEdit,ptHistory);
  }
  void GetChannelReviews(
    vector<const COARreview *> *pRtnList,
    const wxDateTime *ptLastEdit,
    const wxDateTime *ptHistory = NULL) const
  {
    m_ChannelList.GetReviews(pRtnList,ptLastEdit,ptHistory);
  }
  void GetILSReviews(
    vector<const COARreview *> *pRtnList,
    const wxDateTime *ptLastEdit,
    const wxDateTime *ptHistory = NULL) const
  {
    m_ILSList.GetReviews(pRtnList,ptLastEdit,ptHistory);
  }
  bool AppendSampleReview(const COARreview &r)
  {
    return m_SampleList.AppendReview(r);
  }
  bool AppendChannelReview(const COARreview &r)
  {
    return m_ChannelList.AppendReview(r);
  }
  bool AppendILSReview(const COARreview &r)
  {
    return m_ILSList.AppendReview(r);
  }
  bool AppendSampleReview(COARreview *p)
  {
    return m_SampleList.AppendReview(p);
  }
  bool AppendChannelReview(COARreview *p)
  {
    return m_ChannelList.AppendReview(p);
  }
  bool AppendILSReview(COARreview *p)
  {
    return m_ILSList.AppendReview(p);
  }

  bool AppendSampleReview(const wxString &sName)
  {
    return m_SampleList.AppendReview(sName);
  }
  bool AppendChannelReview(const wxString &sName)
  {
    return m_ChannelList.AppendReview(sName);
  }
  bool AppendILSReview(const wxString &sName)
  {
    return m_ILSList.AppendReview(sName);
  }
  int GetSampleCount() const
  {
    return m_SampleList.GetCount();
  }
  int GetILSCount() const
  {
    return m_ILSList.GetCount();
  }
  int GetChannelCount() const
  {
    return m_ChannelList.GetCount();
  }
  int GetSampleCount(const wxDateTime *ptHistory) const
  {
    return m_SampleList.GetCount(ptHistory);
  }
  int GetILSCount(const wxDateTime *ptHistory) const
  {
    return m_ILSList.GetCount(ptHistory);
  }
  int GetChannelCount(const wxDateTime *ptHistory) const
  {
    return m_ChannelList.GetCount(ptHistory);
  }
  const COARreviewList &GetSampleList() const
  {
    return m_SampleList;
  }
  const COARreviewList &GetChannelList() const
  {
    return m_ChannelList;
  }
  const COARreviewList &GetILSList() const
  {
    return m_ILSList;
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  COARreviewList m_SampleList;
  COARreviewList m_ChannelList;
  COARreviewList m_ILSList;
  const wxString m_sChild;
};

//************************************************** COARsample

class COARsample : public nwxXmlPersist
{
public:
  typedef map<int,COARpeakAny *> MAP_ID_PEAK;
  static const wxString g_sCellChannelEdited;
  static const wxString g_sCellILSNotice;
  static const wxChar *EnableDisableSampleStr(bool bEnabled)
  {
    const wxChar *pRtn = (bEnabled) ? wxS("Enable Sample") : wxS("Disable Sample");
    return pRtn;
  }
  static const wxChar *EnableDisableStr(bool bEnabled)
  {
    const wxChar *pRtn = (bEnabled) ? wxS("Enable ") : wxS("Disable ");
    return pRtn;
  }
  COARsample(COARfile *pFile = NULL) :
      m_pvChannelAlerts("Channel"),
      m_pvAlertInterLocus("Alert"),
      m_Reviews(true),
      m_Acceptance(false),
      m_pFile(pFile)
  {
    RegisterAll(true);
  }
  COARsample(const COARsample &x) :
      m_pvChannelAlerts("Channel"),
      m_pvAlertInterLocus("Alert"),
      m_pFile(NULL)
  {
    RegisterAll(true);
    (*this) = x;
  }
  COARsample &operator =(const COARsample &x)
  {
    Init();
    COARcopy(m_sName);
    COARcopy(m_sSampleName);
    COARcopy(m_sSampleComment);
    COARcopy(m_sRunStart);
    COARcopy(m_sType);
    COARcopy(m_alertSample);
    COARcopy(m_alertILS);
    COARcopy(m_pvChannelAlerts);
    COARcopyVP(COARlocus,m_vpLocus);
    COARcopy(m_pvAlertInterLocus);
      // the preceeding will probably be discarded
    COARcopy(m_sPositiveControl);
    COARcopy(m_NotesSample);
    COARcopy(m_NotesChannel);
    COARcopy(m_NotesILS);
    COARcopyVP(COARnotes,m_vpOldNotesSample);
    COARcopyVP(COARnotes,m_vpOldNotesChannel);
    COARcopyVP(COARnotes,m_vpOldNotesILS);
    COARcopy(m_EnableHistory)
    COARcopy(m_Reviews);
    COARcopy(m_Acceptance);
    return *this;
  }
  virtual ~COARsample();
  void LocalInit();
  virtual void Init()
  {
    nwxXmlPersist::Init();
    LocalInit();
  }
  virtual void Init(void *p)
  {
    ((COARsample *)p)->Init();
  }
  virtual void RegisterAll(bool = false);
  const COARfile *GetFile() const
  {
    return m_pFile;
  }
  COARfile *GetFile()
  {
    return m_pFile;
  }
  const wxString &GetPlotFileName() const;
  void PostProcessFile(COARfile *pFile = NULL); // whatever needs to be done after file is loaded
  size_t CountAlerts(const COARmessages *, const wxDateTime *pTime = NULL) const;
  bool HasAnyAlerts(const COARmessages *, const wxDateTime *pTime = NULL) const;
      // used to determine background color for cells without alerts
      // if ther are no alerts for the entire sample, then it is white
      // otherwise bright yellow for cells with alerts and lighter yellow
      //   for cells without alerts.
  bool IsEdited(
    const COARmessages *, 
    const wxDateTime *pTime = NULL,
    bool bCheckLocus = true) const;
  bool HasSampleAlert(const COARmessages *,const wxDateTime *pTime = NULL) const;
  bool HasILSAlert(const COARmessages *,const wxDateTime *pTime = NULL) const;
  // bool HasChannelAlert(const COARmessages *) const;
  bool HasChannelAlert(const COARmessages *,const wxDateTime *pTime = NULL) const ;
  size_t CountChannelAlert(
    const COARmessages *, const wxDateTime *pTime = NULL) const;
  const COARmessages *GetMessages() const;

  //  DJH 3/12/09 -- get and set peaks, whether they are alleles or artifacts
  //    this is used for editing and peak history

  // void SetupAlleleIDs(int *pnID); // used to create allele IDs from 1.1 file

  vectorptr<COARpeakAny> *GetPeaksByLocusName(
    const wxString &sLocus,
    int nChannel,
    const wxDateTime *pTime = NULL,
    bool bIncludeNotEditable = true) const;
      // caller must manage memory in returned
      // pointer and each item in the vector
  vectorptr<COARpeakAny> *GetPeaksByChannel(
    const COARchannel *pChannel,
    const wxDateTime *pTime = NULL,
    bool bIncludeDisabled = false) const;
      // caller must manage memory in returned
      // pointer and each item in the vector
  COARartifact *GetArtifactByID(int nID) const;
      // retrieve artifact stored in the sample
      // no memory is allocated and the caller should
      // not free/delete the object
  size_t GetAllelesByID(int nID, bool bInjectArtifact, std::vector<COARpeakAny *> *pv = NULL) const;
    // stores a new copy of alleles in pv.  The caller must free the memory of each item
  size_t GetAlleleCountByID(int nID) const;
  bool IsPeakEditable(COARpeakAny *p) const
  {
    bool bRtn = (p != NULL) && p->IsEditable() && 
      GetAlleleCountByID(p->GetID()) && true;
    return bRtn;
  }
  bool SetPeaksByLocusName(
    const wxString &sLocus,
    int nChannel,
    vector<COARpeakAny *> *pv);
  // end 3/12/09

  static bool HasChannelAlert(const wxString &sCell)
  {
    bool bRtn = (!sCell.IsEmpty()) && (sCell != g_sCellChannelEdited);
    return bRtn;
  }
  wxString GetCellChannel(const COARmessages *,const wxDateTime *pTime = NULL) const;
  wxString GetCellILS(const COARmessages *pMsgs,const wxDateTime *pTime = NULL) const
  {
    wxString sRtn;
    if(HasILSAlert(pMsgs,pTime))
    {
      sRtn = g_sCellILSNotice;
    }
    else if(IsCellILSEdited(pMsgs,pTime))
    {
      sRtn = g_sCellChannelEdited;
    }
    return sRtn;
  }
  int GetChannelNrFromLocus(const wxString &sLocusName) const;
  int GetChannelNrFromLocus(const COARlocus *pLocus) const;
  bool HasPeaks(int nChannel, const wxString &sLocus, bool bIncludeNotEditable = true) const;

  //   12/09, new features, enable/disable sample
  //   accept and review Sample, Channel, and ILS alerts
  bool IsEnabled(const wxDateTime *pTime = NULL) const;
  bool IsDisabled(const wxDateTime *pTime = NULL) const
  {
    return !IsEnabled(pTime);
  }
  bool SetEnabled(const wxString &sName, bool bEnable);
  //
  //  in order to find review/accept info, the time of last edit
  //  is needed because review and accept info is invalidated
  //  when something is edited with the possible exclusion of notes
  //
  const wxDateTime &GetLastSampleMsgEdit(const wxDateTime *pTime = NULL) const;
  const wxDateTime &GetLastChannelMsgEdit(const wxDateTime *pTime = NULL) const;
  const wxDateTime &GetLastILSMsgEdit(const wxDateTime *pTime = NULL) const;

  bool IsCellILSEdited(const COARmessages *, const wxDateTime *pTime = NULL) const;
  bool IsCellChannelEdited(
    const COARmessages *, const wxDateTime *pTime = NULL) const;
  bool IsSampleLevelEdited(
    const COARmessages *, const wxDateTime *pTime = NULL) const;


  bool IsCellILSEdited(const wxDateTime *pTime = NULL) const
  {
    bool bRtn = IsCellILSEdited(GetMessages(),pTime);
    return bRtn;
  }
  bool IsCellChannelEdited(const wxDateTime *pTime = NULL) const
  {
    bool bRtn = IsCellChannelEdited(GetMessages(),pTime);
    return bRtn;
  }
  bool IsSampleLevelEdited(const wxDateTime *pTime = NULL) const
  {
    bool bRtn = IsSampleLevelEdited(GetMessages(),pTime);
    return bRtn;
  }

  int GetILSReviewCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Reviews.GetILSCount(pTime);
    return nRtn;
  }
  int GetChannelReviewCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Reviews.GetChannelCount(pTime);
    return nRtn;
  }
  int GetSampleReviewCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Reviews.GetSampleCount(pTime);
    return nRtn;
  }
  bool NeedILSReview(
    int nReviewCount, const wxDateTime *pTime) const
  {
    bool bRtn = nReviewCount &&
      IsCellILSEdited(pTime) && 
      (nReviewCount > GetILSReviewCount(pTime));
    return bRtn;
  }
  bool NeedChannelReview(
    int nReviewCount, const wxDateTime *pTime) const
  {
    bool bRtn = nReviewCount &&
      IsCellChannelEdited(pTime) && 
      (nReviewCount > GetChannelReviewCount(pTime));
    return bRtn;
  }
  bool NeedSampleReview(
    int nReviewCount, const wxDateTime *pTime) const
  {
    bool bRtn = nReviewCount &&
      IsSampleLevelEdited(pTime) && 
      (nReviewCount > GetSampleReviewCount(pTime));
    return bRtn;
  }

  int GetILSAcceptanceCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Acceptance.GetILSCount(pTime);
    return nRtn;
  }
  int GetChannelAcceptanceCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Acceptance.GetChannelCount(pTime);
    return nRtn;
  }
  int GetSampleAcceptanceCount(const wxDateTime *pTime) const
  {
    int nRtn = m_Acceptance.GetSampleCount(pTime);
    return nRtn;
  }


  bool NeedILSAcceptance(
    int nAcceptanceCount, const wxDateTime *pTime) const
  {
    bool bRtn = nAcceptanceCount &&
      !IsCellILSEdited(pTime) && 
      HasILSAlert(GetMessages(),pTime) && 
      (nAcceptanceCount > GetILSAcceptanceCount(pTime));
    return bRtn;
  }
  bool NeedChannelAcceptance(
    int nAcceptanceCount, const wxDateTime *pTime) const
  {
    bool bRtn = nAcceptanceCount &&
      !IsCellChannelEdited(pTime) && 
      HasChannelAlert(GetMessages(),pTime) && 
      (nAcceptanceCount > GetChannelAcceptanceCount(pTime));
    return bRtn;
  }
  bool NeedSampleAcceptance(
    int nAcceptanceCount, const wxDateTime *pTime) const
  {
    bool bRtn = nAcceptanceCount &&
      !IsSampleLevelEdited(pTime) && 
      HasSampleAlert(GetMessages(),pTime) && 
      (nAcceptanceCount > GetSampleAcceptanceCount(pTime));
    return bRtn;
  }
  bool NeedAttention(const CLabSettings &lab) const;


  const COARlocus *FindLocus(const wxString &sName) const
  {
    return _FindLocus(sName);
  }
  COARlocus *FindLocus(const wxString &sName)
  {
    const COARlocus *pRtn = _FindLocus(sName);
    return (COARlocus *)pRtn;
    /*
    COARlocus x(sName);
    set<COARlocus *,COARlocusLessByName>::iterator itr =
      m_spLocus.find(&x);
    COARlocus *pRtn = NULL;
    if(itr != m_spLocus.end()) { pRtn = *itr; }
    return pRtn;
    */
  }
  const wxString GetFullFileName() const;
  const wxString GetFileName(bool bFullName = true) const
  {
    // retrive file name w/o path
    wxString sName =
      bFullName ? GetFullFileName() : m_sName;
    if(sName.Find(wxFileName::GetPathSeparator()) != wxNOT_FOUND)
    {
      wxFileName fn(sName);
      sName = fn.GetFullName();
    }
    return sName;
  }
  const wxString &GetRunStart() const
  {
    return m_sRunStart;
  }

  wxString FormatRunStart() const;

  const wxString GetName() const;
  const wxString GetSampleName() const
  {
    if(m_sSampleName.IsEmpty())
    {
      return GetName();
    }
    return m_sSampleName;
  }
  const wxString GetSampleLabel(int nLabelType) const
  {
    return (nLabelType == IDmenuDisplayNameSample)
      ? GetSampleName()
      : GetName();
  }
  const wxString &GetSampleComment() const
  {
    return m_sSampleComment;
  }
  const wxString &GetType() const
  {
    return m_sType;
  }
  bool IsLadderType() const
  {
    int n = m_sType.CmpNoCase("ladder");
    return !n;
  }
  bool CanDisableSample() const
  {
    return !IsLadderType();
  }
  bool IsSampleType() const
  {
    int n = m_sType.CmpNoCase("sample");
    return !n;
  }
  bool IsPosControl() const
  {
    int n = m_sType.CmpNoCase("+control");
    return !n;
  }
  bool IsNegControl() const
  {
    int n = m_sType.CmpNoCase("-control");
    return !n;
  }
  const COARalerts *GetSampleAlerts() const
  {
    return &m_alertSample;
  }
  const COARalerts *GetILSAlerts() const
  {
    return &m_alertILS;
  }
  const vector<COARchannelAlert *> *GetChannelAlerts() const
  {
    return m_pvChannelAlerts.Get();
  }


  const COARnotes *GetNotesChannel(const wxDateTime *pTime = NULL) const
  {
    return COARnotes::_FindNotes(&m_NotesChannel, &m_vpOldNotesChannel, pTime);
  }
  const COARnotes *GetNotesILS(const wxDateTime *pTime = NULL) const
  {
    return COARnotes::_FindNotes(&m_NotesILS, &m_vpOldNotesILS, pTime);
  }
  const COARnotes *GetNotesSample(const wxDateTime *pTime = NULL) const
  {
    return COARnotes::_FindNotes(&m_NotesSample, &m_vpOldNotesSample, pTime);
  }

  const wxString &GetNotesChannelStr(const wxDateTime *pTime = NULL) const
  {
    const COARnotes *pNotes = GetNotesChannel(pTime);
    return COARnotes::GetText(pNotes);
  }
  const wxString &GetNotesILSStr(const wxDateTime *pTime = NULL) const
  {
    const COARnotes *pNotes = GetNotesILS(pTime);
    return COARnotes::GetText(pNotes);
  }
  const wxString &GetNotesSampleStr(const wxDateTime *pTime = NULL) const
  {
    const COARnotes *pNotes = GetNotesSample(pTime);
    return COARnotes::GetText(pNotes);
  }


  COARnotes *GetNotesChannel(const wxDateTime *pTime = NULL)
  {
    const COARsample *ps(this);
    const COARnotes *pRtn = ps->GetNotesChannel(pTime);
    return (COARnotes *)pRtn;
  }
  COARnotes *GetNotesSample(const wxDateTime *pTime = NULL)
  {
    const COARsample *ps(this);
    const COARnotes *pRtn = ps->GetNotesSample(pTime);
    return (COARnotes *)pRtn;
  }
  COARnotes *GetNotesILS(const wxDateTime *pTime = NULL)
  {
    const COARsample *ps(this);
    const COARnotes *pRtn = ps->GetNotesILS(pTime);
    return (COARnotes *)pRtn;
  }


  bool UpdateNotesSample(const wxString &s,wxDateTime &t)
  {
    return _UpdateNotes(&m_NotesSample,&m_vpOldNotesSample,s,t);
  }
  bool UpdateNotesILS(const wxString &s,wxDateTime &t)
  {
    return _UpdateNotes(&m_NotesILS,&m_vpOldNotesILS,s,t);
  }
  bool UpdateNotesChannel(const wxString &s, wxDateTime &t)
  {
    return _UpdateNotes(&m_NotesChannel,&m_vpOldNotesChannel,s,t);
  }
  void AppendNotesSample(const wxString &s, const wxString &sUser)
  {
    m_NotesSample.Append(s,sUser,&m_vpOldNotesSample);
  }
  void AppendNotesILS(const wxString &s, const wxString &sUser)
  {
    m_NotesILS.Append(s,sUser,&m_vpOldNotesILS);
  }
  void AppendNotesChannel(const wxString &s, const wxString &sUser)
  {
    m_NotesChannel.Append(s,sUser,&m_vpOldNotesChannel);
  }

  const COARchannelAlert *GetChannelAlertsByChannel(int n) const;
  void GetArtifactsByLocus(
    vector<const COARartifact *> *pv,
    const wxString &sLocusName,
    int nChannel,
    const wxDateTime *pt = NULL) const;
  const wxString &GetPositiveControl() const
  {
    return m_sPositiveControl;
  }
  void AppendChannelAlerts(vector<int> *_pvn) const;
  void AppendILSAlerts(vector<int> *pvn) const
  {
    m_alertILS.AppendToList(pvn);
  }
  void AppendSampleAlerts(vector<int> *pvn) const
  {
    m_alertSample.AppendToList(pvn);
  }
  const COARsampleReviewAccept &GetReviews() const
  {
    return m_Reviews;
  }
  const COARsampleReviewAccept &GetAcceptance() const
  {
    return m_Acceptance;
  }
  bool ToggleEnabled(const wxString &sName)
  {
    bool bEnabled = IsEnabled();
    bool bRtn  = SetEnabled(sName,!bEnabled);
    return bRtn;
  }

  // acceptance

  bool AppendSampleAcceptance(const wxString &sName)
  {
    bool bRtn = m_Acceptance.AppendSampleReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  bool AppendILSAcceptance(const wxString &sName)
  {
    bool bRtn = m_Acceptance.AppendILSReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  bool AppendChannelAcceptance(const wxString &sName)
  {
    bool bRtn = m_Acceptance.AppendChannelReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }

  // review

  bool AppendSampleReview(const wxString &sName)
  {
    bool bRtn = m_Reviews.AppendSampleReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  bool AppendILSReview(const wxString &sName)
  {
    bool bRtn = m_Reviews.AppendILSReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  bool AppendChannelReview(const wxString &sName)
  {
    bool bRtn = m_Reviews.AppendChannelReview(sName);
    if(bRtn)
    {
      UpdateVersion();
    }
    return bRtn;
  }
  wxString FormatILSReviewAcceptance(const wxDateTime *pt) const
  {
    wxString s = COARreviewList::FormatReviewAcceptance(
      m_Acceptance.GetILSList(),
      m_Reviews.GetILSList(),
      pt);
    return s;
  }
  wxString FormatChannelReviewAcceptance(const wxDateTime *pt) const
  {
    wxString s = COARreviewList::FormatReviewAcceptance(
      m_Acceptance.GetChannelList(),
      m_Reviews.GetChannelList(),
      pt);
    return s;
  }
  wxString FormatSampleReviewAcceptance(const wxDateTime *pt) const
  {
    wxString s = COARreviewList::FormatReviewAcceptance(
      m_Acceptance.GetSampleList(),
      m_Reviews.GetSampleList(),
      pt);
    wxString s2 = m_EnableHistory.FormatList(pt);
    if(!(s.IsEmpty() || s2.IsEmpty()))
    {
      s.Append("\n");
    }
    s.Append(s2);
    return s;
  }
  void SetIsModified(bool b = true);
  void UpdateVersion();
private:
  class COARoldNotes : public nwxXmlPersist
  {
  public:
    COARoldNotes()
    {
      m_p = NULL;
    }
    COARoldNotes(const COARoldNotes &) {;}
    COARoldNotes &operator = (const COARoldNotes &) { return *this;}
    virtual ~COARoldNotes() {;}
    void SetSample(COARsample *p)
    {
      m_p = p;
      RegisterAll(true);
    }
    virtual void RegisterAll(bool = false);
    virtual bool Skip(void *);
  private:
    static TnwxXmlIOPersistVector<COARnotes> g_ioNotes;
    bool SkipArray(vector<COARnotes *> *p);
    COARsample *m_p;
  };
  friend class COARoldNotes;

  // 3/13/09
  void _CheckForSingleAllele(int nID, COARchannelAlert *pCha, const wxString &sLocus);

  static bool _UpdateNotes(COARnotes *pUpd, vector<COARnotes *> *pvOld,
    const wxString &sNotes, wxDateTime &t)
  {
    COARnotes x;
    x.SetText(sNotes);
    bool b = pUpd->Update(x,pvOld,t);
    return b;
  }
  bool IsNotesILSEdited(const wxDateTime *pTime = NULL) const
  {
    bool bEdited = COARnotes::_AreNotesEdited(
      &m_NotesILS,&m_vpOldNotesILS,pTime);
    return bEdited;
  }
  bool IsNotesChannelEdited(const wxDateTime *pTime = NULL) const
  {
    bool bEdited = COARnotes::_AreNotesEdited(
      &m_NotesChannel,&m_vpOldNotesChannel,pTime);
    return bEdited;
  }
  bool IsNotesSampleEdited(const wxDateTime *pTime = NULL) const
  {
    bool bEdited = COARnotes::_AreNotesEdited(
      &m_NotesSample, &m_vpOldNotesSample,pTime);
    return bEdited;
  }

  COARchannelAlert *_GetChannelAlertsByChannel(int n,bool bCreate = false)
  {
    COARchannelAlert *pRtn = (COARchannelAlert *) GetChannelAlertsByChannel(n);
    if(bCreate && (pRtn == NULL))
    {
      pRtn = new COARchannelAlert(n);
      m_pvChannelAlerts->push_back(pRtn);
    }
    return pRtn;
  }

  static vectorptr<COARpeakAny> *_SortMap(MAP_ID_PEAK *mapPeak);
  void _MapIDPeakByChannel(
    MAP_ID_PEAK *pMapPeak,
    const COARchannel *pChannel,
    const wxDateTime *pTime,
    bool bIncludeDisabled) const;

  static int _MakeID(const IOARpeak *pPeak)
  {
    return nwxRound::Round(pPeak->GetTime() * 100.0);
  }
  void _AddAllelesToMap(
    MAP_ID_PEAK *mapPeak,
    const COARlocus *pLocus,
    const wxDateTime *pTime = NULL,
    int nPeakID = -1) const;
  bool _GetArtifactsByChannel(
    vector<const COARartifact *> *pPeaks,
    int nChannel,
    const wxString &sLocus = wxEmptyString,
    const wxDateTime *pTime = NULL,
    int nPeakID = -1) const;

  void _AddArtifactsToMap(
    MAP_ID_PEAK *mapPeak,
    int nChannel,
    const wxString &sLocus,
    const wxDateTime *pTime = NULL,
    bool bIncludeDisabled = false,
    bool bIncludeNotEditable = true) const;

  void _BuildSetLocus() const;
  void _BuildMapIDchannel() const;
  const COARlocus *_FindLocus(const wxString &sName) const;
  wxString m_sName;
  wxString m_sSampleName;
  wxString m_sSampleComment;
  wxString m_sRunStart;
  wxString m_sType;
  COARsampleInfo m_Info;
  COARalerts m_alertSample;
  COARalerts m_alertILS;
  TnwxXmlPersistVector<COARchannelAlert> m_pvChannelAlerts;

  mutable set<COARlocus *,COARlocusLessByName> m_spLocus;
  vector<COARlocus *> m_vpLocus;
  TnwxXmlPersistVector<COARinterlocusAlert> m_pvAlertInterLocus;
    // currently not used
    // the preceeding will probably be discarded
  wxString m_sPositiveControl;
  mutable wxString m_sPlotFileName;

  COARnotes m_NotesSample;
  COARnotes m_NotesChannel;
  COARnotes m_NotesILS;

  COARoldNotes m_OldNotes;
  COARenabledList m_EnableHistory;
  COARsampleReviewAccept m_Reviews;
  COARsampleReviewAccept m_Acceptance;
  vector<COARnotes *> m_vpOldNotesSample;
  vector<COARnotes *> m_vpOldNotesChannel;
  vector<COARnotes *> m_vpOldNotesILS;
  typedef std::map<int,int> map_ID_CHANNEL;
  mutable map_ID_CHANNEL  m_mapIDchannel;
  COARfile *m_pFile;

private:
  TnwxXmlIOPersistVector<COARlocus> m_ioLocus;
  TnwxXmlIOPersistVector<COARnotes> m_ioNotes;
};

// end COARsample

// Enable, Disable

class COARSampleToggleEnabled : public IAppendReview
{
public:
  COARSampleToggleEnabled(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARSampleToggleEnabled() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->ToggleEnabled(sName);
  }
  virtual bool IsAccept() const
  {
    return false;
  }
  virtual bool IsReview() const
  {
    return false;
  }
private:
  COARsample *m_pSample;
};


// Approval

class COARSampleAppendReview : public IAppendReview
{
public:
  COARSampleAppendReview(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARSampleAppendReview() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendSampleReview(sName);
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
  COARsample *m_pSample;
};

class COARsampleILSAppendReview : public IAppendReview
{
public:
  COARsampleILSAppendReview(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARsampleILSAppendReview() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendILSReview(sName);
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
  COARsample *m_pSample;
};

class COARsampleChannelAppendReview : public IAppendReview
{
public:
  COARsampleChannelAppendReview(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARsampleChannelAppendReview() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendChannelReview(sName);
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
  COARsample *m_pSample;
};

// Acceptance

class COARSampleAppendAccept : public IAppendReview
{
public:
  COARSampleAppendAccept(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARSampleAppendAccept() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendSampleAcceptance(sName);
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
  COARsample *m_pSample;
};

class COARsampleILSAppendAccept : public IAppendReview
{
public:
  COARsampleILSAppendAccept(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARsampleILSAppendAccept() {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendILSAcceptance(sName);
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
  COARsample *m_pSample;
};

class COARsampleChannelAppendAccept : public IAppendReview
{
public:
  COARsampleChannelAppendAccept(COARsample *pSample)
    : m_pSample(pSample) {}
  virtual ~COARsampleChannelAppendAccept () {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pSample->AppendChannelAcceptance(sName);
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
  COARsample *m_pSample;
};


#endif
