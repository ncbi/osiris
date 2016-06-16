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
*  FileName: COARpeak.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_OAR_PEAK_H__
#define __C_OAR_PEAK_H__

#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

#include <wx/string.h>
#include <wx/datetime.h>
#include "nwx/nwxString.h"
#include "nwx/nwxRound.h"
#include "nwx/nwxXmlPersist.h"

#define ARTIFACT_NOT_CRITICAL 20


class IOARpeak
{
public:
  static const wxString OL_TRUE;
  static const wxString OL_FALSE;
  static const wxString OL_ACCEPTED;
  static const wxString OL_YES;
  static const wxString OL_NO;
  static const wxString OL_NO_DISPLAYED;
  virtual ~IOARpeak();
  // Get... functions
  virtual const wxString &GetLocusName() const = 0;
  virtual const wxString &GetAlleleName() const = 0;
  virtual const wxDateTime &GetUpdateTime() const = 0;
  virtual const wxString &GetArtifactLabel() const = 0;
  virtual const wxString &GetArtifactUserDisplay() const = 0;

  virtual int GetID() const = 0;
  virtual int  GetAlleleCount() const = 0;
  virtual int GetCriticalLevel() const = 0;

  virtual double GetMeanBPS() const = 0;
  virtual double GetBPS() const = 0;
  virtual double GetRFU() const = 0;
  virtual double GetTime() const = 0;
  virtual double GetPeakArea() const = 0;
  virtual double GetFit() const = 0;

  virtual bool IsArtifact() const = 0;
  virtual bool IsAllele() const = 0;
  virtual bool IsDisabled() const = 0;
  virtual bool IsEditable() const = 0;
  virtual bool GetCountBool() const = 0;
  virtual const wxString &GetOffLadderString() const = 0;
  static bool IsOffLadderString(const wxString &sOL)
  {
    bool bRtn = (sOL == "1") || (!sOL.CmpNoCase(IOARpeak::OL_TRUE));
    return bRtn;
  }
  virtual bool IsOffLadder() const
  {
    const wxString &sOL(GetOffLadderString());
    bool bRtn = IsOffLadderString(sOL);
    return bRtn;
  }
  virtual bool IsOffLadderAccepted() const
  {
    const wxString &sOL(GetOffLadderString());
    bool bRtn = !sOL.CmpNoCase(OL_ACCEPTED);
    return bRtn;
  }


  // Set functions

  virtual void SetID(int n) = 0;
  virtual void SetAlleleCount(int n) = 0;
  virtual void SetCriticalLevel(int n) = 0;
  virtual void SetMeanBPS(double d) = 0;
  virtual void SetBPS(double d) = 0;
  virtual void SetRFU(double d) = 0;
  virtual void SetTime(double d) = 0;
  virtual void SetPeakArea(double d) = 0;
  virtual void SetFit(double d) = 0;
  virtual void SetIsArtifact(bool b) = 0;
  virtual void SetIsAllele(bool b) = 0;
//  virtual void SetIsDisabled(bool b) = 0;
  virtual void SetIsEditable(bool) = 0;
  virtual void SetCountBool(bool b) = 0;
  virtual void SetOffLadderString(const wxString &s) = 0;

  virtual void SetLocusName(const wxString &s) = 0;
  virtual void SetAlleleName(const wxString &s, bool bAmel = false) = 0;
  virtual void SetUpdateTime(const wxDateTime &x) = 0;
  virtual void SetArtifactLabel(const wxString &s) = 0;
  virtual void SetArtifactUserDisplay(const wxString &s) = 0;
  virtual void SetUpdateTimeCurrent() = 0;

  virtual void Set(const IOARpeak &x) = 0;

};

class COARpeakLessTime
{
public:
  COARpeakLessTime() {;}
  bool operator()(const IOARpeak &x1, const IOARpeak &x2) const
  {
    double d1;
    double d2;
    int n1;
    int n2;
    bool b1;
    bool bRtn = false;
    if( (d1 = x1.GetTime()) != (d2 = x2.GetTime()) )
    {
      bRtn = d1 < d2;
    }
    else if((b1 = x1.IsAllele()) != x2.IsAllele())
    {
      bRtn = b1;
    }
    else if( (n1 = x1.GetID()) != (n2 = x2.GetID()) )
    {
      bRtn = n1 < n2;
    }
    else
    {
      bRtn = ((void *)&x1) < ((void *)&x2);
    }
    return bRtn;
  }
  bool operator()(const IOARpeak *p1, const IOARpeak *p2) const
  {
    return (*this)(*p1,*p2);
  }
};

class COARIOstringOL : public nwxXmlIOwxString
{
public:
  COARIOstringOL() : nwxXmlIOwxString(true)
  {
    SetDefault(IOARpeak::OL_FALSE);
  }
  virtual ~COARIOstringOL() {}
  virtual bool Skip(void *pObj)
  {
    wxString &x(CAST(pObj));
    bool b = (x == m_DEFAULT) || x.IsEmpty();
    return b;
  }
};

class COARpeak  : public IOARpeak
{
public:
  virtual ~COARpeak();
  virtual void SetCountBool(bool b);
  virtual bool GetCountBool() const;
  virtual void Set(const IOARpeak &x);

  void SetIsCritical(bool bCritical)
  {
    SetIsCritical(this,bCritical);
  }
  bool IsCritical() const
  {
    return IsCritical(*this);
  }

  static void Copy(IOARpeak *pTo, const IOARpeak &x);
  static void SetUpdateTimeCurrent(IOARpeak *p);
  static void SetCountBool(IOARpeak *p,bool b);
  static bool GetCountBool(const IOARpeak &x);
  static wxString SetupAlleleName(const wxString &_s, bool bAmel);
  static bool Equal(const IOARpeak &x1, const IOARpeak &x2,
    bool bCheckAllele = true, bool bCheckArtifact = true);
  static bool DoubleEqual(double d1, double d2, unsigned int nPlaces = 3);
  static void SetIsCritical(IOARpeak *p, bool bCritical);
  static bool  IsCritical(int nLevel);
  static bool  IsCritical(const IOARpeak &x)
  {
    return IsCritical(x.GetCriticalLevel());
  }

  static wxString FormatBool
		(bool b, const char *psTrue = "1", const char *psFalse = "")
  {
    const char *ps = b ? psTrue : psFalse;
    wxString sRtn(ps);
    return sRtn;
  }

  // static format functions

  static wxString FormatAlleleName(
    const IOARpeak &x,bool bAmel, bool bCheckOffLadder);

  static wxString FormatBPSint(const IOARpeak &x)
  {
    int n = nwxRound::Round(x.GetBPS());
    wxString s = nwxString::FormatNumber(n);
    return s;
  }
  static wxString FormatBPS(const IOARpeak &x,bool bOLgrid = false)
  {
    bool bOL = bOLgrid && x.IsOffLadder();
    const char *fmt = bOLgrid ? "%g" : "%7f";
    wxString s = nwxString::FormatDouble(x.GetBPS(), fmt);
    if(bOL && !s.IsEmpty())
    {
      s.Append(" (OL)");
    }
    return s;
  }
  static const wxString &UnFormatOffLadderString(const wxString &s);
  static const wxString &FormatOffLadderString(const wxString &s);
  static const wxString &FormatOffLadderString(const IOARpeak &x)
  {
    const wxString &s(x.GetOffLadderString());
    const wxString &sRtn(FormatOffLadderString(s));
    return sRtn;
  }

  static wxString FormatOffLadder(const IOARpeak &x)
  {
    return FormatBool(x.IsOffLadder());
  }
  static wxString FormatCount(const IOARpeak &x)
  {
    return FormatBool(x.GetCountBool());
  }
  static wxString FormatRFU(const IOARpeak &x)
  {
    return nwxString::FormatDouble(x.GetRFU());
  }
  static wxString FormatTime(const IOARpeak &x)
  {
    return nwxString::FormatDouble(x.GetTime());
  }
  static wxString FormatPeakArea(const IOARpeak &x)
  {
    return nwxString::FormatDouble(x.GetPeakArea());
  }
  static wxString FormatFit(const IOARpeak &x)
  {
    wxString s;
    if(x.GetFit() >= 0.0)
    {
      s = nwxString::FormatDouble(x.GetFit(),"%.4f");
    }
    return s;
  }
  static wxString FormatIsAllele(const IOARpeak &x)
  {
    bool bOK = x.IsAllele();
    return FormatBool(bOK);
  }
  static wxString FormatIsArtifact(const IOARpeak &x)
  {
    bool bOK = x.IsArtifact();
    return FormatBool(bOK);
  }
  static wxString FormatIsCritical(const IOARpeak &x)
  {
    bool bOK = COARpeak::IsCritical(x);
    return FormatBool(bOK);
  }

  static bool IsOffLadderAlleleName(const wxString &s)
  {
    return s.Contains("OL");
  }
  static void SetupOffLadderAlleleName(wxString *ps, bool b)
  {
    if(b == IsOffLadderAlleleName(*ps)) {;}
    else if(!b)
    {
      ps->Replace("(OL)","",true);
      ps->Replace("OL","",true);
      nwxString::Trim(ps);
    }
    else
    {
      // prepend OL
      wxString s("OL");
      nwxString::Trim(ps);
      s.Append(*ps);
      *ps = s;
    }
  }
  static bool ArtifactNotCritical(int n)
  {
    return (n < 1) || (n >= ARTIFACT_NOT_CRITICAL);
  }
  // non static Format functions
  wxString FormatAlleleName(bool bAmel, bool bCheckOffLadder) const
  {
    return FormatAlleleName(*this,bAmel,bCheckOffLadder);
  }
  wxString FormatOffLadder() const
  {
    return FormatOffLadder(*this);
  }
  const wxString &FormatOffLadderString() const
  {
    return FormatOffLadderString(*this);
  }
  wxString FormatBPS(bool bOLgrid = false) const
  {
    return FormatBPS(*this, bOLgrid);
  }
  wxString FormatBPSint() const
  {
    return FormatBPSint(*this);
  }
  wxString FormatCount() const
  {
    return FormatCount(*this);
  }
  wxString FormatRFU() const
  {
    return FormatRFU(*this);
  }
  wxString FormatTime() const
  {
    return FormatTime(*this);
  }
  wxString FormatPeakArea() const
  {
    return FormatPeakArea(*this);
  }
  wxString FormatFit() const
  {
    return FormatFit(*this);
  }
  wxString FormatIsAllele() const
  {
    return FormatIsAllele(*this);
  }
  wxString FormatIsArtifact() const
  {
    return FormatIsArtifact(*this);
  }
  wxString FormatIsCritical() const
  {
    return FormatIsCritical(*this);
  }

  static const unsigned int FIT_DIGIT_MATCH;
  static const wxString EMPTY_STRING;
  static const wxDateTime ZERO_TIME;
  static COARIOstringOL IO_OL;
};

class COARallele;
class COARartifact;

class COARpeakAny : public COARpeak
{
  // generic peak class that can be converted to
  // (or used to create) an artifact or allele
public:
  virtual ~COARpeakAny();
  COARpeakAny(const IOARpeak &x)
  {
    Set(x);
  }
  void SetupArtifactInfo(const IOARpeak *p);

  virtual int GetID() const;
  virtual int GetAlleleCount() const;
  virtual int GetCriticalLevel() const;

  virtual double GetMeanBPS() const;
  virtual double GetBPS() const;
  virtual double GetRFU() const;
  virtual double GetTime() const;
  virtual double GetPeakArea() const;
  virtual double GetFit() const;

  virtual bool IsArtifact() const;
  virtual bool IsAllele() const;
  virtual bool IsDisabled() const;
  virtual bool IsEditable() const;
  virtual const wxString &GetOffLadderString() const;

  virtual const wxString &GetLocusName() const;
  virtual const wxString &GetAlleleName() const;
  virtual const wxDateTime &GetUpdateTime() const;
  virtual const wxString &GetArtifactLabel() const;
  virtual const wxString &GetArtifactUserDisplay() const;

  // Set functions

  virtual void SetID(int n);
  virtual void SetAlleleCount(int n);
  virtual void SetCriticalLevel(int n);
  virtual void SetMeanBPS(double d);
  virtual void SetBPS(double d);
  virtual void SetRFU(double d);
  virtual void SetTime(double d);
  virtual void SetPeakArea(double d);
  virtual void SetFit(double d);
  virtual void SetIsArtifact(bool b);
  virtual void SetIsAllele(bool b);
  virtual void SetIsEditable(bool);
//  virtual void SetIsDisabled(bool b);
  virtual void SetOffLadderString(const wxString &s);

  virtual void SetAlleleName(const wxString &s, bool bAmel = false);
  virtual void SetLocusName(const wxString &s);
  virtual void SetUpdateTime(const wxDateTime &x);
  virtual void SetArtifactLabel(const wxString &s);
  virtual void SetArtifactUserDisplay(const wxString &s);
  virtual void SetUpdateTimeCurrent();
private:
  void _Init();

  wxString m_sName;
  wxString m_sLocusName;
  wxString m_sArtifactLabel;
  wxString m_sArtifactUserDisplay;
  wxString m_sOffLadder;
  wxDateTime m_dtUpdate;
  double m_dRFU;
  double m_dTime;
  double m_dPeakArea;
  double m_dBPS;
  double m_dMeanBPS;
  double m_dFit;

  int m_nID;
  int m_nAlleleCount;
  int m_nCriticalLevel; // 1 is critical, 20 not so much
  bool m_bArtifact;
  bool m_bAllele;
};

typedef std::set<COARpeakAny *,COARpeakLessTime> setCOARpeakAny;

#endif
