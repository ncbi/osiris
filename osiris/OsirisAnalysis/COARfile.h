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
*  FileName: COARfile.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_OAR_FILE_H__
#define __C_OAR_FILE_H__
#include "mainApp.h"
#include "nwx/nwxFileUtil.h"
#include "OsirisFileTypes.h"

#include "COARmisc.h"
#include "COARheading.h"
#include "COARallele.h"
#include "COARchannelAlert.h"
#include "COARnotes.h"
#include "COARlocus.h"
#include "COARsample.h"
#include "COARmessage.h"
#include "COARdirectoryAlerts.h"
#include "COARreview.h"
#include "COARMsgExport.h"
#include "CAlertViewStatus.h"
class nwxXmlCMF;
class nwxXmlCMFSpecimen;
class CXMLmessageBook;
class COmittedAlerts;

//********************************************** COARsample2CMFSpecimen

class COARsample2CMFSpecimen :
  public map<COARsample *, nwxXmlCMFSpecimen *>
{
public:
  COARsample2CMFSpecimen() {;}
  nwxXmlCMFSpecimen *Find(COARsample *p);
  bool Insert(COARsample *p, nwxXmlCMFSpecimen *ps);
};



//********************************************** COARfile


class COARfile : public nwxXmlPersist
{
public:
  COARfile() : 
    m_DirReviews(true),
    m_DirAcceptance(false),
    m_vpTable("Sample"),
    m_pvOldNotes("OldDirectoryNotes")
  {
    m_messages.SetMsgExport(&m_mapExportSpecifications);

    m_pMsgBook = NULL;
    m_bCheckedMsgBook = false;
    m_pLastSampleDisabled = NULL;
    m_dtLastLoad.Set((time_t)0);
    m_bModified = false;
    RegisterAll(true);
  }
  COARfile(const COARfile &x) :
    m_DirReviews(true),
    m_DirAcceptance(false)
  {
    m_pMsgBook = NULL;
    m_bCheckedMsgBook = false;
    RegisterAll(true);
    m_messages.SetMsgExport(&m_mapExportSpecifications);
    (*this) = x;
  }
  COARfile &operator=(const COARfile &x)
  {
    LocalInit();
    COARcopy(m_dtLastLoad);
    COARcopy(m_bModified);
    COARcopy(m_sVersion);
    COARcopy(m_sFullVersion);
    COARcopy(m_sMsgBookBuildTime);
    COARcopy(m_heading);
    COARcopy(m_DirAlerts);
    COARcopy(m_vpTable);
    COARcopy(m_notes);
    COARcopy(m_pvOldNotes);
    COARcopy(m_messages);
    COARcopy(m_DirReviews);
    COARcopy(m_DirAcceptance);
    COARcopy(m_mapExportSpecifications);
    return *this;
  }
  virtual ~COARfile() 
  {
    _ClearMessageBook();
  }
  static wxString FindAnalysisFile(
    const wxString &sDir,const wxString &sSampleName = wxEmptyString);

  void GetReviewerCounts(
    int *pnReviewer, int *pnAcceptance, int nType) const
  {
    const CLabSettings &lab(m_heading.GetLabSettings());
    *pnReviewer = lab.GetReviewerCount(nType);
    *pnAcceptance = lab.GetAcceptanceCount(nType);
  }
  size_t GetGroupsByMsgName(
    const wxString &sMsgName, set<wxString> *psetGroups) const
  {
    return m_mapExportSpecifications.GetGroupsByMsgName(
      sMsgName,psetGroups);
  }
  bool HasGroupByMsgName(
    const wxString &sMsgName, const wxString &sGroup) const
  {
    return m_mapExportSpecifications.HasGroupByMsgName(
      sMsgName,sGroup);
  }


  bool AppendDirAcceptance(const wxString &sName)
  {
    bool bRtn = m_DirAcceptance.AppendReview(sName);
    if(bRtn)
    {
      m_bModified = true;
    }
    return bRtn;
  }
  bool AppendDirReview(const wxString &sName)
  {
    bool bRtn = m_DirReviews.AppendReview(sName);
    if(bRtn)
    {
      m_bModified = true;
    }
    return bRtn;
  }
  const CLabSettings &GetLabSettings() const
  {
    return m_heading.GetLabSettings();
  }
  CLabSettings &GetLabSettings()
  {
    return m_heading.GetLabSettings();
  }
  bool GetReviewerAllowUserOverride() const
  {
    return GetLabSettings().GetReviewerAllowUserOverride();
  }
  wxString GetExportFileName(const wxString &sExt)
  {
    return nwxFileUtil::SetupFileName(GetFileName(),sExt);
  }
  wxString GetFileExt(bool bDot = false) const;
  virtual bool LoadFile(const wxString &sFileName);
  virtual bool SaveFile(const wxString &sFileName)
  {
    m_heading.SetNewFileName(sFileName);
    m_heading.SetNewFileCreationTime();
    bool bRtn = nwxXmlPersist::SaveFile(sFileName);
    if(bRtn)
    {
      m_bModified = false;
      m_sFileName = sFileName;
    }
    else
    {
      m_sFileName.clear();
    }
    return bRtn;
  }
  virtual bool SaveFile()
  {
    bool bRtn = CanSave() && SaveFile(m_sFileName);
    return bRtn;
  }
  const wxString &GetFileName() const
  {
    return m_sFileName;
  }
  const wxDateTime &GetLastLoad() const
  {
    return m_dtLastLoad;
  }
  const wxDateTime &GetXMLmodTime() const
  {
    return m_heading.GetFileTime();
  }
  unsigned int GetEstimatedTime() const
  {
    // compute a reasonable amount of time between file
    // creation time and file modification time
    // this should be overkill!!!!
    unsigned int n = (unsigned int) GetSampleCount();
    n += 300;
    return n;
  }
  bool IsTampered(unsigned int nSeconds) const;
        // return true if GetXMLmodTime() is not within
        // nSeconds of GetFileModTime() (from class nwxXmlPersist)
  bool IsTampered() const
  {
        // compute a reasonable amount of time between file
        // creation time and file modification time
    unsigned int nEstTime = GetEstimatedTime();
    return IsTampered(nEstTime);
  }
  bool IsOAR() const
  {
    wxString s(m_sFileName);
    s.MakeLower();
    bool bRtn = s.EndsWith(EXT_REPORT);
    return bRtn;
  }
  bool SamplesDisabled() const; // return true if at least one sample is disabled
  size_t GetDisabledSamplesByIndex(std::vector<size_t> *pvNdx = NULL) const;
  size_t DeleteDisabledSamples();
  size_t GetDisabledSamples(
    std::vector<const COARsample *> *pv, 
    bool bIncludeNonSamples = true) const;
  void SetLastSampleDisabled(const COARsample *pSample) const
  {
    m_pLastSampleDisabled = pSample;
  }
  bool CanSave() const
  {
    return !( m_sFileName.IsEmpty() || IsOAR() );
  }
  const wxString &GetHeadingFileName() const
  {
    return m_heading.GetFileName();
  }
  const wxString &GetHeadingNewFileName() const
  {
    return m_heading.GetNewFileName();
  }
  const wxString &GetKitName() const
  {
    return m_heading.GetKitName();
  }
  const wxString GetInputDirectory() const
  {
    wxString sName = m_heading.GetParameters().GetInputDirectory();
    FIX_FILE_NAME(sName);
    return sName;
  }
  /*
  // commented 1/31/11
  bool UpdateNotesSample(COARsample *pSample,const wxString &s,wxDateTime &t)
  {
    bool b = pSample->UpdateNotesSample(s,t);
    if(b)
    {
      m_bModified = true;
    }
    return b;
  }
  bool UpdateNotesILS(COARsample *pSample,const wxString &s,wxDateTime &t)
  {
    bool b = pSample->UpdateNotesILS(s,t);
    if(b)
    {
      m_bModified = true;
    }
    return b;
  }
  bool UpdateNotesChannel(COARsample *pSample,const wxString &s,wxDateTime &t)
  {
    bool b = pSample->UpdateNotesChannel(s,t);
    if(b)
    {
      m_bModified = true;
    }
    return b;
  }
  */
  wxString FindFileByName(const wxString &sName);
  wxString FindMessageBookFile();
  wxString FindPlotFile(COARsample *pSample);
  virtual void RegisterAll(bool = false);
  virtual const wxString &RootNode(void) const;
  const COARchannel *GetChannelFromLocus(const wxString &sLocusName) const;
  size_t GetLocusCount() const
  {
    _BuildLocusMap();
    return m_mapLocusChannel.size();
  }
  const wxString &GetLocusName(size_t n) const
  {
    _BuildLocusMap();
    return m_vsLocus.at(n);
  }
  int GetChannelNr(size_t n) const
  {
    _BuildLocusMap();
    return m_vnChannelNr.at(n);
  }
  size_t GetChannelCount() const
  {
    return m_heading.GetChannelCount();
  }
  const COARchannel *GetChannel(size_t n) const
  {
    return m_heading.GetChannel(n);
  }
  const COARchannel *GetChannelByNr(unsigned int n) const
  {
    return m_heading.GetChannelByNr(n);
  }
  size_t GetSampleCount() const
  {
    return m_vpTable.Size();
  }
  COARsample *GetSample(size_t n)
  {
    return m_vpTable.at(n);
  }
  COARsample *GetSampleByName(const wxString &sName);
  void LocalInit()
  {
    m_dtLastLoad.Set((time_t)0);
    m_pLastSampleDisabled = NULL;
    m_sFileName.Clear();
    m_bModified = false;
    _ClearLocusInfo();
    _ClearMessageBook();
  }
  virtual void Init()
  {
    LocalInit();
    nwxXmlPersist::Init();
  }
  virtual void Init(void *p)
  {
    if((p != NULL) && (p != this))
    {
      ((COARfile *)p)->Init();
    }
    else
    {
      Init();
    }
  }
  CXMLmessageBook *GetMessageBook();

  const COARmessage *GetMessageByNumber(int n, const wxDateTime *pTime) const
  {
    return m_messages.GetMessageByNumber(n,pTime);
  }
  const COARmessage *GetMessageByNumber(int n) const
  {
    return m_messages.GetMessageByNumber(n);
  }
  COARmessages *GetMessages()
  {
    return &m_messages;
  }
  const COARdirectoryAlerts *GetDirectoryAlerts() const
  {
    return &m_DirAlerts;
  }
  const COARmessages *GetMessages() const
  {
    return &m_messages;
  }
  bool DirectoryNeedAttention() const
  {
    return m_DirAlerts.NeedAttention(
      *GetMessages(),
      GetLabSettings(),
      m_DirReviews.GetCount(),
      m_DirAcceptance.GetCount());

  }
  wxString FormatMessages(const vector<int> &vn, int indent = 0, const wxDateTime *pTime = NULL) const;
  int CountMessages(const vector<int> &vn, const wxDateTime *pTime = NULL) const;

  bool HasDirectoryMessages() const
  {
    return m_DirAlerts.HasAlerts();
  }
  bool HasDisplayedDirectoryMessages() const
  {
    return m_DirAlerts.FindDisplayed(m_messages);
  }
  void GetDirectoryMessages(vector<int> *pvn) const
  {
    m_DirAlerts.AppendMessagesToList(pvn);
  }
  const vector<COARbaseLocus *> *GetBaseLociAlerts() const
  {
    return m_DirAlerts.GetBaseLociAlerts();
  }
  wxString FormatReviewAcceptance(const wxDateTime *pTime = NULL) const
  {
    return COARreviewList::FormatReviewAcceptance(
          m_DirAcceptance, m_DirReviews,pTime);
  }
  const CParmOsiris &GetParameters() const
  {
    return m_heading.GetParameters();
  }
  bool IsDirNotesEdited(const wxDateTime *pTime = NULL) const
  {
    bool bRtn = COARnotes::_AreNotesEdited(
      &m_notes,m_pvOldNotes.Get(),pTime);
    return bRtn;
  }

  void AppendNotesDir(const wxString &sNotes, const wxString &sUser)
  {
    m_notes.Append(sNotes,sUser,m_pvOldNotes.Get());
  }
  const COARnotes *GetNotesDir(const wxDateTime *pTime = NULL) const
  {
    return COARnotes::_FindNotes(
      &m_notes, m_pvOldNotes.Get(), pTime);
  }

  bool IsDirMessageEdited(const wxDateTime *pTime = NULL) const
  {
    bool bRtn = IsDirNotesEdited(pTime)
      || m_DirAlerts.IsModified(m_messages,pTime);
    return bRtn;
  }
    // return true is at least one directory message
    // was edited or at least one directory level note
    // was entered before the specified time

  bool IsModified() const
  {
    return m_bModified;
  }
  const set<wxDateTime> *GetHistory() const
  {
    return m_heading.GetHistory();
  }
  const wxDateTime &GetCreationTime() const
  {
    return m_heading.GetCreationTime();
  }
  const wxDateTime &GetFileTime() const
  {
    return m_heading.GetFileTime();
  }
  double GetVersion() const
  {
    double d = atof(m_sVersion.utf8_str());
    return d;
  }
  bool CanEditArtifacts() const
  {
    double d = GetVersion();
    return (d > 1.101);  // 1.2 or later
  }

  nwxXmlCMF *CreateCMF(COARsample2CMFSpecimen *pMap = NULL);
  void UpdateVersion();
  void SetIsModified(bool b = true)
  {
    m_bModified = b;
  }
  bool UpdateMessages(const COARmessages &msgs, const wxDateTime &t);
  bool UpdateSampleLocus(
    COARsample *pSample, const COARlocus &Locus, const wxDateTime &t);

  static wxString AppendNotes(
    const wxString &sNotes,
    const wxString &sAppend,
    const wxString &sUser)
  {
    return COARnotes::AppendNotes(sNotes,sAppend,sUser);
  }
  static size_t AddLine(vector<wxString> *pvs, const wxString &s)
  {
    size_t nRtn = s.Len();
    if(nRtn)
    {
      pvs->push_back(s);
    }
    return nRtn;
  }
  static wxString CheckStatus(
    int nRequired,
    int nHave,
    const wxString &sMessage);
  static size_t CheckStatus(
    vector<wxString> *pvs,
    int nRequired,
    int nHave,
    const wxString &sMessage)
  {
    wxString sRtn = CheckStatus(nRequired,nHave,sMessage);
    return AddLine(pvs,sRtn);
  }
  bool NeedDirReview();

  bool NeedDirAcceptance();
  wxString CheckDirStatus(
    int nReview,
    int nAccept,
    const wxDateTime *pTime,
    bool *pbReview = NULL) const;

  wxString CheckDirStatus(
    const wxDateTime *pTime = NULL,
    bool *pbReview = NULL) const
  {
    int nReview,nAccept;
    GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_DIR);
    return CheckDirStatus(nReview,nAccept,pTime,pbReview);
  }

  wxString CheckChannelStatus(
    const COARsample *pSample,
    int nReview,
    int nAccept,
    const wxDateTime *pTime,
    bool *pbReview = NULL) const;

  wxString CheckChannelStatus(
    const COARsample *pSample,
    const wxDateTime *pTime = NULL,
    bool *pbReview = NULL) const
  {
    int nReview,nAccept;
    GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_CHANNEL);
    return CheckChannelStatus(pSample,nReview,nAccept,pTime,pbReview);
  }

  wxString CheckILSStatus(
    const COARsample *pSample,
    int nReview,
    int nAccept,
    const wxDateTime *pTime,
    bool *pbReview = NULL) const;

  wxString CheckILSStatus(
    const COARsample *pSample,
    const wxDateTime *pTime = NULL,
    bool *pbReview = NULL) const
  {
    int nReview,nAccept;
    GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_ILS);
    return CheckILSStatus(pSample,nReview,nAccept,pTime,pbReview);
  }
  
  wxString CheckSampleStatus(
    const COARsample *pSample,
    int nReview,
    int nAccept,
    const wxDateTime *pTime,
    bool *pbReview = NULL) const;

  wxString CheckSampleStatus(
    const COARsample *pSample,
    const wxDateTime *pTime = NULL,
    bool *pbReview = NULL) const
  {
    int nReview,nAccept;
    GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_SAMPLE);
    return CheckSampleStatus(pSample,nReview,nAccept,pTime,pbReview);
  }

  wxString CheckLocusStatus(
    const COARlocus *pLocus,
    int nReview,
    int nAccept,
    const wxDateTime *pTime,
    bool *pbReview = NULL) const;

  wxString CheckLocusStatus(
    const COARlocus *pLocus,
    const wxDateTime *pTime = NULL,
    bool *pbReview = NULL) const
  {
    int nReview,nAccept;
    GetReviewerCounts(&nReview,&nAccept,CLabReview::REVIEW_LOCUS);
    return CheckLocusStatus(pLocus,nReview,nAccept,pTime,pbReview);
  }


  size_t CheckChannelStatus(
    const COARsample *pSample,
    vector<wxString> *pvs,
    int nReview,
    int nAccept,
    const wxDateTime *pTime) const
  {
    wxString sRtn = CheckChannelStatus(pSample,nReview,nAccept,pTime);
    return AddLine(pvs,sRtn);
  }

  size_t CheckILSStatus(
    const COARsample *pSample,
    vector<wxString> *pvs,
    int nReview,
    int nAccept,
    const wxDateTime *pTime) const
  {
    wxString sRtn = CheckILSStatus(pSample,nReview,nAccept,pTime);
    return AddLine(pvs,sRtn);
  }

  size_t CheckSampleStatus(
    const COARsample *pSample,
    vector<wxString> *pvs,
    int nReview,
    int nAccept,
    const wxDateTime *pTime) const
  {
    wxString sRtn = CheckSampleStatus(pSample,nReview,nAccept,pTime);
    return AddLine(pvs,sRtn);
  }

  size_t CheckLocusStatus(
    const COARlocus *pLocus,
    vector<wxString> *pvs,
    int nReview,
    int nAccept,
    const wxDateTime *pTime) const
  {
    wxString sRtn = CheckLocusStatus(pLocus,nReview,nAccept,pTime);
    return AddLine(pvs,sRtn);
  }

  wxString FormatLocusAlerts(
    const COARsample *pSample,
    COARlocus *pLocus,
    const CAlertViewStatus &viewStatus,
    const wxDateTime *pTime = NULL) const;

  wxString FormatIlsAlerts(
    const COARsample *pSample, 
    const CAlertViewStatus &viewStatus,
    const wxDateTime *pTime = NULL) const;

  wxString FormatAlertsBySample(
    const COARsample *pSample, 
    const CAlertViewStatus &viewStatus,
    const wxDateTime *pTime = NULL) const;

  wxString FormatChannelAlerts(
    const COARsample *pSample, 
    const CAlertViewStatus &viewStatus,
    const wxDateTime *pTime = NULL) const;


  static void FormatText(wxString *ps, int nIndent);

  static void AppendCellString(wxString *pCell, const wxString &s, int n = 1);
  static void AppendVectorInt(vector<int> *pvn, const vector<int> &vn);
  static void AppendVectorInt(vector<int> *pvn, const vector<int> *pvnAdd)
  {
    AppendVectorInt(pvn,*pvnAdd);
  }

  static TnwxXmlIOPersistVectorSimple<int,nwxXmlIOint> g_iovn;
  static nwxXmlIOPersistVectorWxString g_ioLocusName;
  static const wxString g_sRootOAR;     // root node name
  static const wxString g_sLocusName;   // "LocusName" tag, reusable
  static const wxString g_sMessageNumber; // "MessageNumber" tag, reusable
  static const wxString g_sComma;
    // handles XML IO for vector<wxString>, mainly LocusName
private:
  // variables - those with a ** comment are
  // NOT managed by xmlPersist

  CXMLmessageBook *m_pMsgBook;  //**
  wxDateTime m_dtLastLoad;  //**
  wxString m_sFileName; //**
  wxString m_sVersion;
  wxString m_sFullVersion;
  wxString m_sMsgBookBuildTime;
  COARheading m_heading;
  COARdirectoryAlerts m_DirAlerts;
  COARnotes m_notes;

  COARreviewList m_DirReviews;
  COARreviewList m_DirAcceptance;

  TnwxXmlPersistVector<COARsample> m_vpTable;
  TnwxXmlPersistVector<COARnotes> m_pvOldNotes;
  COARmessages m_messages;
  COARmsgExportMap m_mapExportSpecifications;
  mutable map<wxString,const COARchannel *> m_mapLocusChannel; //**
  mutable vector<wxString> m_vsLocus; //**
  mutable vector<int> m_vnChannelNr; //**
  mutable const COARsample *m_pLastSampleDisabled; //**
  bool m_bModified; //**
  bool m_bCheckedMsgBook; //**

private:
  static void _JoinStrings(
    wxString &s,
    const vector<wxString> &vs, 
    COmittedAlerts *pOmitted,
    size_t nLen = 32000);
  void _PostProcessFile();
  void _FormatCommonAlerts(
    COmittedAlerts *pOmitted,
    const COARsample *pSample,
    vector<wxString> &vs,
    size_t &nLen,
    const CAlertViewStatus &viewStatus,
    const wxDateTime *pTime = NULL,
    bool bReverse = false) const;

  void _ClearMessageBook();
  void _ClearLocusInfo() const
  {
    m_mapLocusChannel.clear();
    m_vsLocus.clear();
    m_vnChannelNr.clear();
  }
  void __BuildLocusMap() const;
  void _BuildLocusMap() const
  {
    if(m_mapLocusChannel.empty())
    {
      __BuildLocusMap();
    }
  }
};



//********************************************** COARfileDirReview

class COARfileDirReview : public IAppendReview
{
public:
  COARfileDirReview(COARfile *pFile)
    : m_pFile(pFile) {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pFile->AppendDirReview(sName);
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
  COARfile *m_pFile;
};

//********************************************** COARfileDirAccept

class COARfileDirAccept : public IAppendReview
{
public:
  COARfileDirAccept(COARfile *pFile)
    : m_pFile(pFile) {}
  virtual bool AppendReview(const wxString &sName)
  {
    return m_pFile->AppendDirAcceptance(sName);
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
  COARfile *m_pFile;
};
#define COAR_NOTICE_DISPLAY "notices"
#define COAR_NOTICE_DISPLAY_CAP "Notices"

#endif
