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
*  FileName: COARchannelAlert.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __COAR_CHANNEL_ALERT_H__
#define __COAR_CHANNEL_ALERT_H__

#include "COARmisc.h"
#include "COARpeak.h"

//************************************************* COARartifactAllele
class COARartifactAllele : public nwxXmlPersist
{
public:
  COARartifactAllele()
  {
    _Init();
    RegisterAll(true);
  }
  COARartifactAllele(const COARartifactAllele &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARartifactAllele(){;}
  const wxString &GetName() const
  {
    return m_sName;
  }
  const wxString &GetLocus() const
  {
    return m_sLocus;
  }
  double GetBPS() const
  {
    return m_dBPS;
  }
  int GetLocation() const
  {
    return m_nLocation;
  }
  int GetCount() const
  {
    return m_nCount;
  }
  const wxString &GetOffLadderString() const
  {
    return m_sOffLadder;
  }

  void SetName(const wxString &s)
  {
    m_sName = s;
  }
  void SetLocus(const wxString &s)
  {
    m_sLocus = s;
  }
  void SetBPS(double d)
  {
    m_dBPS = d;
  }
  void SetLocation(int n)
  {
    m_nLocation =
      (n < 0)
      ? -1
      : ((n > 0) ? 1 : 0);
  }
  void SetCount(int n)
  {
    m_nCount =
      (n < 0)
      ? 0
      : ((n > 4) ? 4 : n);
  }
  void SetOffLadderString(const wxString s = "true")
  {
    m_sOffLadder = s;
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  void _Init()
  {
    m_sName.Empty();
    m_sLocus.Empty();
    m_dBPS = 0.0;
    m_nLocation = 0;
    m_nCount = 0;
    m_sOffLadder = IOARpeak::OL_FALSE;
  }
  wxString m_sName;
  wxString m_sLocus;
  wxString m_sOffLadder;
  double m_dBPS;
  int m_nLocation;
  int m_nCount;
};
//************************************************* COARartifact

class COARartifact : public nwxXmlPersist, public IOARpeak
{
protected:
  bool _SetupSingleAllele() const
  {
    if(m_pAlleleCurrent == NULL && m_vpAllele.size() == 1)
    {
      m_pAlleleCurrent = *(m_vpAllele.begin());
    }
    return (m_pAlleleCurrent != NULL);
  }
public:
  COARartifact()
  {
    m_bDisabled = false;
    m_pAlleleCurrent = NULL;
    RegisterAll(true);
  }
  COARartifact(const COARartifact &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARartifact();
  COARartifact &operator = (const COARartifact &x);

  void ClearAmbiguousAllele() const
  {
    if(m_vpAllele.size() == 1)
    {
      m_pAlleleCurrent = *m_vpAllele.begin();
    }
    else
    {
      m_pAlleleCurrent = NULL;
    }

  }
  bool HasBeenModified() const
  {
    bool b = (m_dtUpdate.GetTicks() > 0);
    return b;
  }

  //************ virtual IOARpeak functions

  virtual bool IsEditable() const
  {
    return m_bIsEditable;
  }

  virtual const wxString &GetLocusName() const
  {
    if(!_SetupSingleAllele())
    {
      return COARpeak::EMPTY_STRING;
    }
    return m_pAlleleCurrent->GetLocus();
  }
  virtual const wxString &GetArtifactLabel() const
  {
    return m_sLabel;
  }
  virtual const wxString &GetAlleleName() const
  {
    if(!_SetupSingleAllele())
    {
      return COARpeak::EMPTY_STRING;
    }
    return m_pAlleleCurrent->GetName();
  }
  virtual const wxDateTime &GetUpdateTime() const
  {
    return m_dtUpdate;
  }

  virtual int GetID() const
  {
    return m_nID;
  }
  virtual int GetAlleleCount() const
  {
    int nRtn =
      _SetupSingleAllele()
      ? m_pAlleleCurrent->GetCount()
      : 0;
    return nRtn;
  }
  virtual int GetCriticalLevel() const
  {
    return m_nLevel;
  }
  virtual double GetBPS() const
  {
    double d = _SetupSingleAllele()
      ? m_pAlleleCurrent->GetBPS()
      : 0.0;
    return d;
  }
  virtual double GetMeanBPS() const
  {
    return m_dMeanBPS;
  }
  virtual double GetRFU() const
  {
    return m_dRFU;
  }
  virtual double GetTime() const
  {
    return m_dTime;
  }
  virtual double GetPeakArea() const
  {
    return m_dPeakArea;
  }
  virtual double GetFit() const
  {
    return m_dFit;
  }
  virtual bool IsArtifact() const
  {
    return !m_bDisabled;
  }
  virtual bool IsAllele() const
  {
    return false;
  }
  virtual bool IsDisabled() const
  {
    return m_bDisabled;
  }
  virtual bool GetCountBool() const
  {
    return COARpeak::GetCountBool(*this);
  }
  virtual const wxString &GetOffLadderString() const
  {
    const wxString &s = _SetupSingleAllele()
      ? m_pAlleleCurrent->GetOffLadderString()
      : IOARpeak::OL_FALSE;
    return s;
  }

  virtual void SetIsEditable(bool b)
  {
    m_bIsEditable = b;
  }
  virtual void SetID(int n)
  {
    m_nID = n;
  }
  virtual void SetAlleleCount(int n)
  {
    if(_SetupSingleAllele())
    {
      m_pAlleleCurrent->SetCount(n);
    }
  }
  virtual void SetCriticalLevel(int n)
  {
    m_nLevel = n;
  }
  virtual void SetMeanBPS(double d)
  {
    m_dMeanBPS = d;
  }
  virtual void SetBPS(double d)
  {
    if(_SetupSingleAllele())
    {
      m_pAlleleCurrent->SetBPS(d);
    }
  }
  virtual void SetRFU(double d)
  {
    m_dRFU = d;
  }
  virtual void SetTime(double d)
  {
    m_dTime = d;
  }
  virtual void SetPeakArea(double d)
  {
    m_dPeakArea = d;
  }
  virtual void SetFit(double d)
  {
    m_dFit = d;
  }
  virtual void SetIsArtifact(bool b)
  {
    m_bDisabled = !b;
  }
  virtual void SetIsAllele(bool)
  {}
  virtual void SetCountBool(bool b)
  {
    COARpeak::SetCountBool(this,b);
  }
  virtual void SetOffLadderString(const wxString &s)
  {
    if(_SetupSingleAllele())
    {
      m_pAlleleCurrent->SetOffLadderString(s);
    }
  }
  virtual void SetAlleleName(const wxString &_s,bool bAmel)
  {
    if(_SetupSingleAllele())
    {
      wxString s(COARpeak::SetupAlleleName(_s,bAmel));
      m_pAlleleCurrent->SetName(s);
    }
  }
  virtual void SetUpdateTime(const wxDateTime &x)
  {
    m_dtUpdate = x;
  }
  virtual void SetUpdateTimeCurrent()
  {
    COARpeak::SetUpdateTimeCurrent(this);
  }

  virtual void Set(const IOARpeak &x)
  {
    COARpeak::Copy(this,x);
  }
  virtual void SetLocusName(const wxString &s)
  {
    SetLocus(s);
  }
  virtual void SetArtifactLabel(const wxString &s)
  {
    m_sLabel = s;
  }

  //************ END virtual IOARpeak functions


  bool SetLocus(const wxString &s) const;
//  const wxString &GetLabel() const
//  {
//    return m_sLabel;
//  }
  const wxDateTime &GetUpdate() const
  {
    return m_dtUpdate;
  }
  const vector<int> &GetMessages() const
  {
    return m_vnMessageNumber;
  }
  const vector<int> &GetAlerts() const
  {
    return m_vnMessageNumber;
  }
  size_t GetNumberOfAlleles() const
  {
    return m_vpAllele.size();
  }
  const COARartifactAllele *GetAllele(size_t ndx) const
  {
    return m_vpAllele.at(ndx);
  }

  int GetLevel() const
  {
    // deprecated, use GetCriticalLevel()
    return  GetCriticalLevel();
  }
  void SetDisabled(bool b)
  {
    m_bDisabled = b;
  }
  void SetLevel(int n)
  {
    // deprecated, use SetCriticalLevel
    SetCriticalLevel(n);
  }
//  void SetLabel(const wxString &s)
//  {
//    m_sLabel = s;
//  }
  void AddNewAllele(const wxString &sLocus)
  {
    // create new allele for locus and set current
    // to the new allele
    m_pAlleleCurrent = new COARartifactAllele;
    m_pAlleleCurrent->SetLocus(sLocus);
    m_vpAllele.push_back(m_pAlleleCurrent);
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sLabel;
  wxDateTime m_dtUpdate;
  vector<int> m_vnMessageNumber;
  vector<COARartifactAllele *> m_vpAllele;
  TnwxXmlIOPersistVector<COARartifactAllele> m_ioAllele;
  double m_dRFU;
  double m_dMeanBPS;
  double m_dPeakArea;
  double m_dTime;
  double m_dFit;
  mutable COARartifactAllele *m_pAlleleCurrent;
      // chosen allele after calling SetLocus()
      //  not persistent
  int m_nID;
  int m_nLevel;
  bool m_bDisabled;
  bool m_bIsEditable;
};

//************************************************* COARchannelAlert


class COARchannelAlert : public nwxXmlPersist
{
public:
  COARchannelAlert(int nChannel) : m_nChannelNr(nChannel)
  {
    RegisterAll(true);
  }
  COARchannelAlert() : m_nChannelNr(0)
  {
    RegisterAll(true);
  }
  COARchannelAlert(const COARchannelAlert &x)
  {
    RegisterAll(true);
    (*this) = x;
  }
  virtual ~COARchannelAlert();
  virtual void RegisterAll(bool = false);

  COARartifact *GetArtifactByID(int nID);
  bool UpdateArtifact(const wxString &sLocus, IOARpeak *pPeak);
  void GetArtifactsByTime(
    const wxString &sLocus,
    vector<const COARartifact *> *pva,
    const wxDateTime *pTime) const;

  void GetArtifactsByTime(
    vector<const COARartifact *> *pva,
    const wxDateTime *pTime) const
  {
    const wxString s("");
    GetArtifactsByTime(s,pva,pTime);
  }


  int GetNumber() const
  {
    return m_nChannelNr;
  }
  const vector<int> *GetMessageNumbers() const
  {
    return &m_vnMessageNumber;
  }
size_t GetArtifactCount() const
  {
    return m_vpArtifact.size();
  }
  const COARartifact *GetArtifact(size_t ndx) const
  {
    return m_vpArtifact.at(ndx);
  }
  size_t GetOldArtifactCount() const
  {
    return m_vpOldArtifact.size();
  }
  const COARartifact *GetOldArtifact(size_t ndx) const
  {
    return m_vpOldArtifact.at(ndx);
  }

  // bug workaround, some <OldArtifact> elements contain <AllowPeakEdit>false</AllowPeakEdit>
  //  call parent function and then _CleanupOldArtifact
  virtual bool LoadFromNode(wxXmlNode *, void *pObj);
  virtual bool LoadFromNode(wxXmlNode *);

private:
  // bug workaround, some <OldArtifact> elements contain <AllowPeakEdit>false</AllowPeakEdit>
  void _CleanupOldArtifact()
  {
    vector<COARartifact *>::iterator itr;
    for(itr = m_vpOldArtifact.begin();
      itr != m_vpOldArtifact.end();
      ++itr)
    {
      (*itr)->SetIsEditable(true);
    }
  }
  TnwxXmlIOPersistVector<COARartifact> m_ioArtifact;
  vector<COARartifact *> m_vpArtifact;
  vector<COARartifact *> m_vpOldArtifact;
  vector<int> m_vnMessageNumber;
  int m_nChannelNr;
};

#endif
