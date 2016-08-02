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
*  FileName: COARallele.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_ALLELE_H__
#define __COAR_ALLELE_H__

#include "COARpeak.h"
#include "COARmisc.h"
#include "nwx/nwxDoubleLess.h"

//************************************************* COARallele
class COARmessages;
class COARlocus;

class COARallele : public nwxXmlPersist, public IOARpeak
{
public:
  COARallele() :
      m_tUpdate((time_t)0),
      m_bDisabled(false),
      m_pLocus(NULL)
  {
    RegisterAll(true);
  }
  COARallele(const COARallele &x, COARlocus *pLocus = NULL) :
      m_tUpdate((time_t)0),
      m_bDisabled(false),
      m_pLocus(NULL)
  {
    RegisterAll(true);
    (*this) = x;
    if(pLocus != NULL) { SetLocus(pLocus); }
  }
  COARallele &operator =(const COARallele &x);
  virtual ~COARallele() {;}
  virtual void RegisterAll(bool = false);
  const vector<int> &GetMessages() const
  {
    return m_vnMessageNumber;
  }

  const vector<int> &GetAlerts() const
  {
    return m_vnMessageNumber;
  }
  const COARlocus *GetLocus() const
  {
    return m_pLocus;
  }
  void SetLocus(COARlocus *pLocus);

  //************   virtual IOARpeak functions

  //       get functions

  virtual const wxString &GetLocusName() const
  {
    return m_sLocusName;
  }
  virtual const wxString &GetArtifactLabel() const
  {
    return COARpeak::EMPTY_STRING;
  }
  virtual const wxString &GetArtifactUserDisplay() const
  {
    return COARpeak::EMPTY_STRING;
  }
  virtual const wxString &GetAlleleName() const
  {
    return m_sName;
  }
  virtual const wxDateTime &GetUpdateTime() const
  {
    return m_tUpdate;
  }
  virtual int GetID() const
  {
    return m_nID;
  }
  virtual int GetAlleleCount() const
  {
    return m_nCount;
  }
  virtual int GetCriticalLevel() const
  {
    return m_nArtifactLevel;
  }
  virtual double GetMeanBPS() const
  {
    return m_dMeanBPS;
  }
  virtual double GetBPS() const
  {
    return m_dBPS;
  }
  virtual double GetRFU() const
  {
    return m_dRFU;
  }
  virtual double GetPullupHeightCorrection() const
  {
    return m_dPullupHeightCorrection;
  }
  virtual double GetTime() const // time, in seconds, of the peak
  {
    return m_dTime;
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
  virtual bool IsDisabled() const
  {
    return m_bDisabled;
  }
  virtual bool IsArtifact() const
  {
    return false;
  }
  virtual bool IsAllele() const
  {
    return !m_bDisabled;
  }
  virtual bool GetCountBool() const
  {
    // currently m_nCount is 2 for double allele,
    // 0 or 1 otherwise
    return COARpeak::GetCountBool(*this);
  }
  virtual const wxString &GetOffLadderString() const
  {
     // true if off ladder, false otherwise
    return m_sOffLadder;
  }

  //   set functions
  virtual void SetID(int n)
  {
    m_nID = n;
  }
  virtual void SetAlleleCount(int n)
  {
    m_nCount = n;
  }
  virtual void SetCriticalLevel(int n)
  {
    m_nArtifactLevel = n;
  }
  virtual void SetMeanBPS(double d)
  {
    m_dMeanBPS = d;
  }
  virtual void SetBPS(double d)
  {
    m_dBPS = d;
  }
  virtual void SetRFU(double d)
  {
    m_dRFU = d;
  }
  virtual void SetPullupHeightCorrection(double d)
  {
    m_dPullupHeightCorrection = d;
  }
  virtual void SetTime(double d)
  {
    m_dTime = d; // time of peak in seconds
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
  virtual void SetIsArtifact(bool)
  {;}
  virtual void SetIsEditable(bool)
  {;}
  virtual void SetIsAllele(bool b)
  {
    m_bDisabled = !b;
  }
/*
//  virtual void SetIsDisabled(bool b)
  {
    if(b)
    {
      m_bDisabled = true;
    }
  }
*/
  virtual void SetCountBool(bool b)
  {
    // currently m_nCount is 2 for double allele,
    // 0 or 1 otherwise
    COARpeak::SetCountBool(this,b);
  }

  virtual void SetOffLadderString(const wxString &s)
  {
    m_sOffLadder = s;
  }
  virtual void SetAlleleName(const wxString &_s,bool bAmel = false)
  {
    m_sName = COARpeak::SetupAlleleName(_s,bAmel);
  }
  virtual void SetLocusName(const wxString &s)
  {
    if(m_pLocus == NULL)
    {
      m_sLocusName = s;
    }
  }
  virtual void SetArtifactLabel(const wxString &)
  {;}
  virtual void SetArtifactUserDisplay(const wxString &)
  {;}
  virtual void SetUpdateTime(const wxDateTime &t)
  {
    m_tUpdate = t;
  }
  virtual void SetUpdateTimeCurrent()
  {
    COARpeak::SetUpdateTimeCurrent(this);
  }

  virtual void Set(const IOARpeak &x)
  {
    COARpeak::Copy(this,x);
  }
  //************ end virtual functions

  void SetDisabled(bool b = true)
  { m_bDisabled = b; }

  const wxString &GetName() const // deprecated, use GetAlleleName
  {
    return m_sName;
  }
  void SetCount(int n)
  {
    // deprecated, use SetAlleleCount()
    // currently m_nCount is 2 for double allele,
    // 0 or 1 otherwise
    m_nCount = n;
  }
  int GetCount() const
  {
    // deprecated, use GetAlleleCount()
    // currently m_nCount is 2 for double allele,
    // 0 or 1 otherwise
    return m_nCount;
  }

  void SetName(const wxString &s, bool bAmel = false)
  {
    // deprecated, use SetAlleleName
    SetAlleleName(s,bAmel);
  }

  wxString FormatName(bool bAmel = false, bool bOL = false) const
  {
    wxString s = COARpeak::FormatAlleleName(*this,bAmel,bOL);
    return s;
  }
  static wxString FormatDouble(double d, const char *psFmt = "%g")
  {
    return nwxString::FormatDouble(d,psFmt);
  }
  wxString FormatBPS(bool bOLgrid = false) const
  {
    return COARpeak::FormatBPS(*this,bOLgrid);
  }
  void SetUpdateTime(time_t t = 0)
  {
    if(!t)
    {
      time(&t);
    }
    m_tUpdate = t;
  }
  wxString FormatRFU() const
  {
    return FormatDouble(GetRFU());
  }
  wxString FormatPeakArea() const
  {
    return FormatDouble(GetPeakArea());
  }
  wxString FormatFit() const
  {
    return COARpeak::FormatFit(*this);
  }
  wxString FormatTime() const
  {
    return FormatDouble(GetTime());
  }
  const char *FormatOffLadder
		(const char *psTrue = "1", const char *psFalse = "") const
  {
    // used for wxGrid cell using wxGridCellBoolEditor
    const char *ps = IsOffLadder() ? psTrue : psFalse;
    return ps;
  }
  const char *FormatCount
		(const char *psTrue = "1", const char *psFalse = "") const
  {
    // currently m_nCount is 2 for double allele,
    // 0 or 1 otherwise
    const char *ps = GetCountBool() ? psTrue : psFalse;
    return ps;
  }
  const char *FormatDisabled
		(const char *psTrue = "1", const char *psFalse = "") const
  {
    // used for wxGrid cell using wxGridCellBoolEditor
    const char *ps = IsDisabled() ? psTrue : psFalse;
    return ps;
  }
  bool HasAlerts(const COARmessages *pmsgs, const wxDateTime *pTime = NULL) const;
  bool HasBeenEdited(
    const COARmessages *pmsgs, const wxDateTime *pTime = NULL) const;
  bool operator == (const COARallele &x) const;
  bool operator != (const COARallele &x) const
  {
    return !((*this) == x);
  }
private:
  class XmlIOintArtifact : public nwxXmlIOint
  {
  public:
    virtual bool Skip(void *pObj)
    {
      int &x(CAST(pObj));
      bool bRtn = COARpeak::ArtifactNotCritical(x);
      return bRtn;
    }
  };

  vector<int> m_vnMessageNumber;
  wxString m_sName;
  wxString m_sInterLocus;
  wxString m_sLocusName;
  wxString m_sOffLadder;
  wxDateTime m_tUpdate;
  double m_dBPS;
  double m_dRFU;
  double m_dPullupHeightCorrection;
  double m_dPeakArea;
  double m_dWidth;
  double m_dMeanBPS;
  double m_dTime; // time of peak in seconds
  double m_dFit;
  int m_nCount;   // 2 if tall peak and called homozygous, 0 or 1 otherwise
  int m_nID;
  int m_nArtifactLevel;
  bool m_bDisabled;
  COARlocus *m_pLocus;

  static XmlIOintArtifact g_ioArtifact;
};

//************************************************* COARalleleLessByTime

class COARalleleLessByTime
{
  //
  // used for sorting set<COARallele> or set<COARallele *> by
  //  time in seconds of the peak, then by update time.
  // this will be used to find history
  //
public:
  COARalleleLessByTime() {;}
  virtual ~COARalleleLessByTime() {;}
  bool operator()(const COARallele &a1, const COARallele &a2) const
  {
    double d1 = a1.GetTime();
    double d2 = a2.GetTime();
    int nCmp = m_CMP.cmp(d1,d2);
    bool bRtn = nCmp < 0;
    return bRtn;
  }
  bool operator()(const COARallele *pa1, const COARallele *pa2) const
  {
    return (*this)(*pa1,*pa2);
  }
protected:
  nwxDoubleLess m_CMP;
};

#endif
