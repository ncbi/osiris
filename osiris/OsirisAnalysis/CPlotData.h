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

*  FileName: CPlotData.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PLOT_DATA_H__
#define __C_PLOT_DATA_H__
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxRound.h"
#include "CParmOsiris.h"
#include "COARpeak.h"

#define FREEPTR(x) if((x) != NULL) { free(x); x = NULL;}

class CPlotLocus;

class CSamplePeak : public nwxXmlPersist, public IOARpeak
{
public:
  friend class CPlotData;
  CSamplePeak()
  {
    RegisterAll(true);
  }
  virtual ~CSamplePeak() {}

  virtual void Init(void *)
  {
    m_dBPS = 0.0;
    m_dMeanBps = 0.0;
    m_dPeakArea = 0.0;
    m_dWidth = 0.0;
    m_nHeight = 0;
    m_dPullupHeightCorrection = 0;
    m_dMean = 0.0;
    m_dFit = 0.0;
    m_sAllele.Clear();
  }
  virtual void Init()
  {
    Init((void *)this);
  }

  virtual const wxString &GetArtifactLabel() const
  {
    return COARpeak::EMPTY_STRING;
  }
  virtual const wxString &GetArtifactUserDisplay() const
  {
    return COARpeak::EMPTY_STRING;
  }
  virtual const wxString &GetLocusName() const
  {
    return COARpeak::EMPTY_STRING;
  }
  virtual const wxString &GetAlleleName() const
  {
    return m_sAllele;
  }
  virtual const wxDateTime &GetUpdateTime() const
  {
    return COARpeak::ZERO_TIME;
  }
  virtual int GetID() const
  {
    return 0;
  }
  virtual int GetAlleleCount() const
  {
    return m_sAllele.IsEmpty() ? 0 : 1;
  }
  virtual int GetCriticalLevel() const
  {
    return 0;
  }
  virtual double GetTime() const
  {
    return m_dMean;
  }
  virtual double GetBPS() const
  {
    double d = (m_dBPS == 0.0) ? m_dMeanBps : m_dBPS;
    return d;
  }
  virtual double GetMeanBPS() const
  {
    return m_dMeanBps;
  }
  virtual double GetRFU() const
  {
    return (double) m_nHeight;
  }
  virtual double GetPullupHeightCorrection() const
  {
    return m_dPullupHeightCorrection;
  }
  virtual double GetPeakArea() const
  {
    return m_dPeakArea;
  }
  virtual double GetWidth() const
  {
    return m_dWidth;
  }
  virtual double GetFit() const
  {
    return m_dFit;
  }
  virtual bool IsArtifact() const
  {
    return false;
  }
  virtual bool IsAllele() const
  {
    return !m_sAllele.IsEmpty();
  }
  virtual bool IsEditable() const
  {
    return true;
  }
  virtual bool IsDisabled() const
  {
    return false;
  }
  virtual bool GetCountBool() const
  {
    return false;
  }
  virtual const wxString &GetOffLadderString() const
  {
    bool b = COARpeak::IsOffLadderAlleleName(m_sAllele);
    if(b)
    {
      return IOARpeak::OL_TRUE;
    }
    return IOARpeak::OL_FALSE;
  }

  // virtual 'Set' functions

  virtual void SetID(int) {;}
  virtual void SetAlleleCount(int) {;}
  virtual void SetCriticalLevel(int) {;}
  virtual void SetBPS(double d)
  {
    m_dMeanBps = d;
  }
  virtual void SetMeanBPS(double d)
  {
    m_dMeanBps = d;
  }
  virtual void SetRFU(double d)
  {
    m_nHeight = nwxRound::Round(d);
  }
  virtual void SetPullupHeightCorrection(double d)
  {
    m_dPullupHeightCorrection = d;
  }
  virtual void SetTime(double d)
  {
    m_dMean = d;
  }

  virtual void SetPeakArea(double d)  
  {
    m_dPeakArea = d;
  }
  virtual void SetWidth(double d)
  {
    m_dWidth = d;
  }
  virtual void SetFit(double) {;}
  virtual void SetIsArtifact(bool) {;}
  virtual void SetIsAllele(bool) {;}
  virtual void SetIsEditable(bool) {;}
//  virtual void SetIsDisabled(bool b) = 0;
  virtual void SetCountBool(bool) {;}
  virtual void SetOffLadderString(const wxString &s)
  {
    bool b = IsOffLadderString(s);
    COARpeak::SetupOffLadderAlleleName(&m_sAllele,b);
  }

  virtual void SetArtifactLabel(const wxString &) {;}
  virtual void SetArtifactUserDisplay(const wxString &) {;}
  virtual void SetLocusName(const wxString &) {;}
  virtual void SetAlleleName(const wxString &s, bool bAmel = false)
  {
    m_sAllele = COARpeak::SetupAlleleName(s,bAmel);
  }
  virtual void SetUpdateTime(const wxDateTime &) {;}
  virtual void SetUpdateTimeCurrent() {;}
  virtual void Set(const IOARpeak &x)
  {
    COARpeak::Copy(this,x);
  }

  // end of IOARpeak virtuals


  const wxString &GetAllele() const
  {
    // deprecated - use GetAlleleName()
    return m_sAllele;
  }
  const char *GetAlleleCstr() const
  {
    return m_sAllele.utf8_str();
  }
  int GetMean() const
  {
    return nwxRound::Round(m_dMean);
  }
  double GetMeanDouble() const
  {
    // deprecated - use GetTime()
    return m_dMean;
  }
  int GetHeight() const
  {
    return m_nHeight;
  }
  int GetMeanBps() const
  {
    double dRound = (m_dMeanBps < 0.0) ? -0.5 : 0.5;
    return int(m_dMeanBps + dRound);
  }
  double GetBpsResidual() const
  {
    // deprecated - use GetBPS()
    return m_dMeanBps;
  }
  void FixAmel(int nStart, int nEnd);
protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  wxString m_sAllele;
  double m_dPullupHeightCorrection;
  double m_dMeanBps;
  double m_dPeakArea;
  double m_dWidth;
  double m_dBPS;
  double m_dFit;
  double m_dMean;
  int m_nHeight;
};

class CArtifact : public nwxXmlPersist, public IOARpeak
{
public:
  friend class CPlotData;
  CArtifact()
  {
    RegisterAll(true);
  }
  virtual ~CArtifact() {}

  virtual void Init(void *)
  {
    m_nHeight = 0;
    m_dPullupHeightCorrection = 0.0;
    m_dMean = 0.0;
    m_dFit = 0.0;
    m_nLevel = 0;
    m_dMeanBPS = 0;
    m_sLabel.Empty();
    m_sEquivAllele.Empty();
    m_sLocusName.Empty();
  }
  virtual void Init()
  {
    Init((void *)this);
  }

// IOARpeak virtuals

  virtual const wxString &GetLocusName() const
  {
    return m_sLocusName;
  }
  virtual const wxString &GetAlleleName() const
  {
    return m_sEquivAllele;
  }
  virtual const wxDateTime &GetUpdateTime() const
  {
    return COARpeak::ZERO_TIME;
  }
  virtual const wxString &GetArtifactLabel() const
  {
    return m_sLabel;
  }
  virtual const wxString &GetArtifactUserDisplay() const
  {
    return COARpeak::EMPTY_STRING;
  }

  virtual int GetID() const
  {
    return 0;
  }
  virtual int  GetAlleleCount() const
  {
    return m_sEquivAllele.IsEmpty() ? 0 : 1;
  }
  virtual int GetCriticalLevel() const
  {
    return m_nLevel;
  }

  virtual double GetMeanBPS() const
  {
    return m_dMeanBPS;
  }

  virtual double GetBPS() const
  {
    return m_dMeanBPS;
  }
  virtual double GetRFU() const
  {
    return (double)m_nHeight;
  }
  virtual double GetPullupHeightCorrection() const
  {
    return m_dPullupHeightCorrection;
  }
  virtual double GetTime() const
  {
    return m_dMean;
  }
  virtual double GetPeakArea() const
  {
    return m_dPeakArea;
  }
  virtual double GetWidth() const
  {
    return m_dWidth;
  }
  virtual double GetFit() const
  {
    return m_dFit;
  }

  virtual bool IsEditable() const
  {
    return true;
  }
  virtual bool IsArtifact() const
  {
    return true;
  }
  virtual bool IsAllele() const
  {
    return false;
  }
  virtual bool IsDisabled() const
  {
    return false;
  }
  virtual bool GetCountBool() const
  {
    return false;
  }
  virtual const wxString &GetOffLadderString() const
  {
    bool b = COARpeak::IsOffLadderAlleleName(m_sEquivAllele);
    return b ? IOARpeak::OL_TRUE : IOARpeak::OL_FALSE;
  }


  // Set functions

  virtual void SetIsEditable(bool) {;}
  virtual void SetID(int) {;}
  virtual void SetAlleleCount(int) {;}
  virtual void SetCriticalLevel(int n)
  { m_nLevel = n; }
  virtual void SetBPS(double d)
  {
    m_dMeanBPS = d;
  }
  virtual void SetMeanBPS(double d)
  {
    m_dMeanBPS = d;
  }
  virtual void SetRFU(double d)
  {
    m_nHeight = nwxRound::Round(d);
  }
  virtual void SetPullupHeightCorrection(double d)
  {
    m_dPullupHeightCorrection = d;
  }
  virtual void SetTime(double d)
  {
    m_dMean = d;
  }
  virtual void SetPeakArea(double d) 
  {
    m_dPeakArea = d;
  }
  virtual void SetWidth(double d)
  {
    m_dWidth = d;
  }
  virtual void SetFit(double d)
  {
    m_dFit = d;
  }
  virtual void SetIsArtifact(bool){;}
  virtual void SetIsAllele(bool) {;}
  virtual void SetCountBool(bool) {;}
  virtual void SetOffLadderString(const wxString &s)
  {
    bool b = IsOffLadderString(s);
    COARpeak::SetupOffLadderAlleleName(&m_sEquivAllele,b);
  }

  virtual void SetLocusName(const wxString &s)
  {
    m_sLocusName = s;
  }
  virtual void SetAlleleName(const wxString &s, bool bAmel = false)
  {
    m_sEquivAllele = COARpeak::SetupAlleleName(s,bAmel);
  }
  virtual void SetUpdateTime(const wxDateTime &) {;}
  virtual void SetArtifactLabel(const wxString &s)
  {
    m_sLabel = s;
  }
  virtual void SetArtifactUserDisplay(const wxString &) {}
  virtual void SetUpdateTimeCurrent() {;}
  virtual void Set(const IOARpeak &x)
  {
    COARpeak::Copy(this,x);
  }


// END IOARpeak virtuals


  int GetMean() const
  {
    return nwxRound::Round(m_dMean);
  }
  double GetMeanDouble() const
  {
    // deprecated, use GetBPS();
    return m_dMean;
  }
  int GetHeight() const
  {
    // deprecated, use GetRFU()
    return m_nHeight;
  }
  int GetLevel() const
  {
    // deprecated, use GetCriticalLevel()
    return m_nLevel;
  }
  bool LevelSelected(int nMaxLevel) const
  {
    return m_nLevel <= nMaxLevel;
  }
  const wxString &GetLabel() const
  {
    // deprecated, use GetArtifactLabel()
    return m_sLabel;
  }
  const char *GetLabelCstr() const
  {
    return m_sLabel.utf8_str();
  }
protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  wxString m_sLocusName;
  wxString m_sLabel;
  wxString m_sEquivAllele;
  double m_dMean;
  double m_dMeanBPS;
  double m_dPeakArea;
  double m_dWidth;
  double m_dFit;
  double m_dPullupHeightCorrection;
  int m_nHeight;
  int m_nLevel;
};


class CPlotChannel : public nwxXmlPersist
{
public:
  friend class CPlotData;
  CPlotChannel()  : m_IOpeak(true)
  {
    const size_t POINT_COUNT(16384);
    m_pdRaw = NULL;
    m_pdAnalyzed = NULL;
    m_pdLadder = NULL;
    m_pdBaseline = NULL;
    m_pdPeakTime = NULL;
    m_pdPeakBPS = NULL;

    m_vnLadderPoints.reserve(POINT_COUNT);
    m_vnAnalyzedPoints.reserve(POINT_COUNT);
    m_vnRawPoints.reserve(POINT_COUNT);
    m_vnBaselinePoints.reserve(POINT_COUNT);
    RegisterAll(true);
  }
  virtual ~CPlotChannel()
  {
    m_IOpeak.Cleanup();
    ClearDoubles();
  }
  virtual void Init(void *)
  {
    _Cleanup();
    nwxXmlPersist::Init();
  }
  virtual void Init()
  {
    Init((void *)this);
  }
  size_t GetPointCount();
  size_t GetBaselinePointCount() const
  {
    return m_vnBaselinePoints.size();
  }
  const vector<CSamplePeak *> &GetPeaks()
  {
    return m_vSamplePeak;
  }
  size_t GetPeakCount()
  {
    return m_vSamplePeak.size();
  }
  const vector<CSamplePeak *> &GetLadderPeaks()
  {
    return m_vLadderPeak;
  }

  const vector<CArtifact *> &GetArtifacts()
  {
    return m_vArtifact;
  }
  int GetNumber() const
  {
    return m_nr;
  }

  // OS-917 - get ILS bps and time for each peak
  //    used in ILS channel for BPS x-axis
  void CleanupPeakTimeILS()
  {
    FREEPTR(m_pdPeakTime);
    m_pdPeakBPS = NULL;
  }
  double *GetPeakTime()
  {
    if(m_pdPeakTime == NULL)
    {
      _setupPeakTimeBPS();
    }
    return m_pdPeakTime;
  }
  double *GetPeakBps()
  {
    if(m_pdPeakBPS == NULL)
    {
      _setupPeakTimeBPS();
    }
    return m_pdPeakBPS;
  }
  void FixAmel(int nStart, int nEnd);
  void FixBaseline();
#if 0
/*
  void SetBegin(int nBegin)
  {
    if((nBegin >= 0) && (m_nBegin != nBegin))
    {
      m_mmRaw.Init();
      m_mmAnalyzed.Init();
      m_mmLadder.Init();
      m_nBegin = nBegin;
    }
  }
  const MINMAX_RFU &GetMinMaxRaw(int nBegin = -1)
  {
    SetBegin(nBegin);
    m_mmRaw.Setup(m_vnRawPoints,m_nBegin);
    return m_mmRaw;
  }
  const MINMAX_RFU &GetMinMaxLadder(int nBegin = -1)
  {
    SetBegin(nBegin);
    m_mmLadder.Setup(m_vnRawPoints,m_nBegin);
    return m_mmLadder;
  }
  const MINMAX_RFU &GetMinMaxAnalyzed(int nBegin = -1)
  {
    SetBegin(nBegin);
    m_mmAnalyzed.Setup(m_vnRawPoints,m_nBegin);
    return m_mmAnalyzed;
  }
  int GetBegin()
  {
    return m_nBegin;
  }
*/
#endif
protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  void _Cleanup()
  {
    ClearDoubles();
    m_IOpeak.Cleanup();
    m_IOartifact.Cleanup();
    m_nPointCount = 0;
    m_nBaselineStart = 0;
    m_vnRawPoints.clear();
    m_vnAnalyzedPoints.clear();
    m_vnLadderPoints.clear();
    m_vnBaselinePoints.clear();
  }
  void ClearDoubles()
  {
    CleanupPeakTimeILS();
    FREEPTR(m_pdRaw);
    FREEPTR(m_pdAnalyzed);
    FREEPTR(m_pdLadder);
    FREEPTR(m_pdBaseline);
  }
  static void BuildList(vector<int> &vn, double **p, size_t nPointCount);
  double *GetRaw()
  {
    if(m_pdRaw == NULL)
    {
      BuildList(m_vnRawPoints,&m_pdRaw,GetPointCount());
    }
    return m_pdRaw;
  }
  double *GetAnalyzed()
  {
    if(m_pdAnalyzed == NULL)
    {
      BuildList(m_vnAnalyzedPoints,&m_pdAnalyzed,GetPointCount());
    }
    return m_pdAnalyzed;
  }
  double *GetLadder()
  {
    if(m_pdLadder == NULL)
    {
      BuildList(m_vnLadderPoints,&m_pdLadder,GetPointCount());
    }
    return m_pdLadder;
  }
  double *GetBaseline()
  {
    if((m_pdBaseline == NULL) && m_vnBaselinePoints.size())
    {
      BuildList(m_vnBaselinePoints,&m_pdBaseline,GetPointCount());
    }
    return m_pdBaseline;
  }
  bool HasBaseline()
  {
    return !!m_vnBaselinePoints.size();
  }
  void _setupPeakTimeBPS();
  double GetMinRfu()
  {
    return m_dMinRfu;
  }

  vector<int> m_vnRawPoints;
  vector<int> m_vnAnalyzedPoints;
  vector<int> m_vnLadderPoints;
  vector<int> m_vnBaselinePoints;
  vector<int> m_vnBaselinePointsObsolete;
  vector<CSamplePeak *> m_vSamplePeak;
  vector<CSamplePeak *> m_vLadderPeak;
  vector<CArtifact *> m_vArtifact;
  double m_dMinRfu;
  int m_nr;
  unsigned int m_nBaselineStart; // keeping it because it is in the .plt file
  unsigned int m_nBaselineStartObsolete; // keeping it because it is in the .plt file
  size_t m_nPointCount;
  double *m_pdRaw;
  double *m_pdAnalyzed;
  double *m_pdLadder;
  double *m_pdBaseline;
  double *m_pdPeakTime;
  double *m_pdPeakBPS;
  TnwxXmlIOPersistVector<CSamplePeak> m_IOpeak;
  TnwxXmlIOPersistVector<CArtifact> m_IOartifact;
};

class CPlotLocus : public nwxXmlPersist
{
public:
  friend class CPlotData;
  CPlotLocus()
  {
    RegisterAll(true);
  }
  virtual ~CPlotLocus()
  { }

  virtual void Init(void *)
  {
    nwxXmlPersist::Init();
    m_sName.Clear();
    m_nStart = 0;
    m_nEnd = 0;
    m_nStartExtended = 0;
    m_nEndExtended = 0;
    m_nChannel = 0;
  }
  virtual void Init()
  {
    Init((void *)this);
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  int GetStart() const
  {
    return m_nStart;
  }
  int GetEnd() const
  {
    return m_nEnd;
  }
  int GetStartExtended() const
  {
    return (!!m_nStartExtended) ? m_nStartExtended : m_nStart;
  }
  int GetEndExtended() const
  {
    return (!!m_nEndExtended) ? m_nEndExtended : m_nEnd;
  }
  int GetStartPlus() const
  {
    return m_nStart - MARGIN;
  }
  int GetEndPlus() const
  {
    return m_nEnd + MARGIN;
  }
  unsigned int GetChannel() const
  {
    return m_nChannel;
  }
  bool MatchChannelTime(int nChannel, int nTime)
  {
    bool bRtn = false;
    if(nChannel != (int)m_nChannel)
    {
    }
    else if(
      (nTime >= (m_nStart - MARGIN)) &&
      (nTime <= (m_nEnd + MARGIN)) )
    {
      bRtn = true;
    }
    else if(
      (!!m_nStartExtended) && 
      (!!m_nEndExtended) && 
      (nTime >= m_nStartExtended) &&
      (nTime <= m_nEndExtended) )
    {
      bRtn = true;
    }
    return bRtn;
  }
  static const int MARGIN;
protected:
  virtual void RegisterAll(bool bInConstructor = false);
private:
  wxString m_sName;
  int m_nStart;
  int m_nEnd;
  int m_nStartExtended;
  int m_nEndExtended;

  unsigned int m_nChannel;
};

class CPlotData : public nwxXmlPersist
{
public:
  static const int TYPE_ANALYZED;
  static const int TYPE_RAW;
  static const int TYPE_LADDER;
  static const int TYPE_BASELINE;
  static const int TYPE_ALL;

  CPlotData() :
    m_IOchannel(true),
    m_IOlocus(true),
    m_pdILS_BPs(NULL)
  {
    m_pdX = NULL;
    m_nStart = 0;
    m_nBegin = 0;
    m_nInterval = 0;
    m_nEnd = 0;
    m_nILS = 0;
    m_nPointCount = 0;
    RegisterAll(true);
  }
  virtual ~CPlotData();
  virtual void Init(void *)
  {
    _Cleanup();
    nwxXmlPersist::Init();
    m_nInterval = 1;
  }
  virtual void Init()
  {
    Init(this);
  }
  CPlotLocus *FindLocus(const wxString &sLocusName);
  unsigned int GetChannelFromLocus(const wxString &sLocusName)
  {
    unsigned int nRtn = 0;
    CPlotLocus *p = FindLocus(sLocusName);
    if(p != NULL)
    {
      nRtn = p->GetChannel();
    }
    return nRtn;
  }

  virtual bool LoadFromNode(wxXmlNode *pNode);
  size_t GetPointCount();
  bool HasBaseline();
  unsigned int GetChannelCount()
  {
    return (unsigned int)m_vChannels.size();
  }
  double *GetTimePoints();
  double *GetILSBpsPoints()
  {
    if(m_pdILS_BPs == NULL)
    {
      _setupILSBps();
    }
    return m_pdILS_BPs;
  }
  double TimeToILSBps(double dTime);
  double ILSBpsToTime(double dBPS);

  double *GetRawPoints(unsigned int nChannel)
  {
    double *pRtn(NULL);
    CPlotChannel *p = FindChannel(nChannel);
    if(p != NULL)
    {
      pRtn = p->GetRaw();
    }
    return pRtn;
  }
  double *GetAnalyzedPoints(unsigned int nChannel)
  {
    double *pRtn(NULL);
    CPlotChannel *p = FindChannel(nChannel);
    if(p != NULL)
    {
      pRtn = p->GetAnalyzed();
    }
    return pRtn;
  }
  double *GetLadderPoints(unsigned int nChannel)
  {
    double *pRtn(NULL);
    CPlotChannel *p = FindChannel(nChannel);
    if(p != NULL)
    {
      pRtn = p->GetLadder();
    }
    return pRtn;
  }
  double *GetBaselinePoints(unsigned int nChannel)
  {
    double *pRtn(NULL);
    CPlotChannel *p = FindChannel(nChannel);
    if(p != NULL)
    {
      pRtn = p->GetBaseline();
    }
    return pRtn;
  }
  double GetMinRfu(unsigned int nChannel)
  {
    CPlotChannel *p = FindChannel(nChannel);
    return (p == NULL) ? 0.0 : p->GetMinRfu();
  }
  const wxString &GetFullVersion() const
  {
    if (m_sFullVersion.IsEmpty())
    {
      return m_sVersion;
    }
    return m_sFullVersion;
  }
  const wxString &GetFilename() const
  {
    return m_sFilename;
  }
  const wxString &GetKitName() const
  {
    return m_sKit;
  }
  const vector<CSamplePeak *> *GetLadderPeaks(unsigned int nChannel)
  {
    const vector<CSamplePeak *> *pRtn(NULL);
    CPlotChannel *pChan(FindChannel(nChannel));
    if(pChan != NULL)
    {
      const vector<CSamplePeak *> &v(pChan->GetLadderPeaks());
      pRtn = &v;
    }
    return pRtn;
  }
  const vector<CSamplePeak *> *GetSamplePeaks(unsigned int nChannel)
  {
    const vector<CSamplePeak *> *pRtn(NULL);
    CPlotChannel *pChan(FindChannel(nChannel));
    if(pChan != NULL)
    {
      const vector<CSamplePeak *> &v(pChan->GetPeaks());
      pRtn = &v;
    }
    return pRtn;
  }
  const vector<CArtifact *> *GetArtifacts(unsigned int nChannel)
  {
    const vector<CArtifact *> *pRtn(NULL);
    CPlotChannel *pChan(FindChannel(nChannel));
    if(pChan != NULL)
    {
      const vector<CArtifact *> &v(pChan->GetArtifacts());
      pRtn = &v;
    }
    return pRtn;
  }
  const vector<CPlotLocus *> *GetLoci()
  {
    return &m_vLocus;
  }
  wxString GetLocusName(int nChannel, int nTime);
  const CParmOsiris &GetParameters()
  {
    return m_parm;
  }
  const wxString &GetLadder()
  {
    return m_sLadder;
  }
  unsigned int GetBegin() const
  {
    return m_nBegin;
  }
  void GetChannelNumbers(set<int> *psetn)
  {
    psetn->clear();
    for(vector<CPlotChannel *>::iterator itr = m_vChannels.begin();
      itr != m_vChannels.end();
      ++itr)
    {
      psetn->insert((*itr)->GetNumber());
    }
  }
  unsigned int GetILSChannel() const
  {
    return m_nILS;
  }
  double GetMaxTime()
  {
    double dRtn = 0.0;
    double *d = GetTimePoints();
    if(d != NULL)
    {
      size_t n = GetPointCount();
      if(n > 0)
      {
        dRtn = d[n-1];
      }
    }
    return dRtn;
  }
  double GetMaxXBPS()
  {
    double dRtn = 0;
    size_t n = GetPointCount();
    if(n > 0)
    {
      _setupILSBps();
      dRtn = m_pdILS_BPs[n-1];
    }
    return dRtn;
  }
  wxString FindAnalysisFile() const;
  bool GetLocusRange(
    const wxString &sLocusName,
    int *px1, int *px2, int *py1, int *py2,
    int nType = TYPE_ANALYZED);

protected:
  virtual void RegisterAll(bool b = false);
private:
  static double _Interpolate(double dX, const double *pdXlist, const double *pdYlist, size_t nCount);
  void _FixBaseline();
  void _Cleanup();
  void _setupILSBps();
  CPlotChannel *FindChannel(unsigned int n);
  CParmOsirisLite m_parm;
  wxString m_sVersion;
  wxString m_sFullVersion;
  wxString m_sFilename;
  wxString m_sLadder;
  wxString m_sKit;
  vector<CPlotChannel *> m_vChannels;
  vector<CPlotLocus *> m_vLocus;
  map<unsigned int,CPlotChannel *> m_mapChannels;
      // do not use, except through FindChannel()
  double *m_pdILS_BPs; // OS-917, plot by BPs
  unsigned int m_nStart;
  unsigned int m_nBegin;
  unsigned int m_nInterval;
  unsigned int m_nEnd;
  unsigned int m_nILS;
  size_t m_nPointCount;
  double *m_pdX;

  TnwxXmlIOPersistVector<CPlotChannel> m_IOchannel;
  TnwxXmlIOPersistVector<CPlotLocus> m_IOlocus;

};


#endif
