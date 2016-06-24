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
*  FileName: COARpeak.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COARpeak.h"
#include "wxIDS.h"

const wxString IOARpeak::OL_TRUE("true");
const wxString IOARpeak::OL_FALSE("false");
const wxString IOARpeak::OL_ACCEPTED("accepted");
const wxString IOARpeak::OL_YES("yes");
const wxString IOARpeak::OL_NO(" ");
const wxString IOARpeak::OL_NO_DISPLAYED("no");

const unsigned int COARpeak::FIT_DIGIT_MATCH(8);
const wxString COARpeak::EMPTY_STRING;
const wxDateTime COARpeak::ZERO_TIME((time_t)0);
COARIOstringOL COARpeak::IO_OL;

IOARpeak::~IOARpeak()
{;}

COARpeak::~COARpeak()
{;}

void COARpeak::SetCountBool(bool b)
{
  SetCountBool(this,b);
}
void COARpeak::SetCountBool(IOARpeak *p, bool b)
{
  p->SetAlleleCount(b ? 2 : 0);
}

bool COARpeak::GetCountBool() const
{
  return GetCountBool(*this);
}
bool COARpeak::GetCountBool(const IOARpeak &x)
{
  int n = x.GetAlleleCount();
  bool b = n > 1;
  return b;
}


void COARpeak::SetIsCritical(IOARpeak *p, bool bCritical)
{
  p->SetCriticalLevel(bCritical ? 1 : ARTIFACT_NOT_CRITICAL);
}
bool COARpeak::IsCritical(int nLevel)
{
  return (nLevel > 0) && (nLevel <= MIN_CRITICAL_ARTIFACT);  // in wxIDS.h
}

void COARpeak::Set(const IOARpeak &x)
{
  Copy(this,x);
}
void COARpeak::Copy(IOARpeak *pTo, const IOARpeak &x)
{
  pTo->SetID(x.GetID());
  pTo->SetAlleleCount(x.GetAlleleCount());
  pTo->SetCriticalLevel(x.GetCriticalLevel());
  pTo->SetBPS(x.GetBPS());
  pTo->SetMeanBPS(x.GetMeanBPS());
  pTo->SetRFU(x.GetRFU());
  pTo->SetTime(x.GetTime());
  pTo->SetPeakArea(x.GetPeakArea());
  pTo->SetFit(x.GetFit());
  pTo->SetIsArtifact(x.IsArtifact());
  pTo->SetIsAllele(x.IsAllele());
//  pTo->SetIsDisabled(x.IsDisabled());
  pTo->SetIsEditable(x.IsEditable());
  pTo->SetOffLadderString(x.GetOffLadderString());
  pTo->SetAlleleName(x.GetAlleleName());
  pTo->SetLocusName(x.GetLocusName());
  pTo->SetArtifactLabel(x.GetArtifactLabel());
  pTo->SetArtifactUserDisplay(x.GetArtifactUserDisplay());
  pTo->SetUpdateTime(x.GetUpdateTime());
}

void COARpeak::SetUpdateTimeCurrent(IOARpeak *p)
{
  wxDateTime dt;
  dt.SetToCurrent();
  p->SetUpdateTime(dt);
}

wxString COARpeak::SetupAlleleName(const wxString &_s,bool bAmel)
{
  wxString sRtn(_s);
  int nDot;
  sRtn.MakeUpper();
  sRtn.Trim(true);
  sRtn.Trim(false);
  if(bAmel)
  {
    if(sRtn.Contains("X"))
    {
      sRtn = "1";
    }
    else if(sRtn.Contains("Y"))
    {
      sRtn = "2";
    }
  }
  else if ( (nDot = sRtn.Find(".")) != wxNOT_FOUND )
  {
    int nLen = (int)sRtn.Len();
    int nDot2 = nDot + 2;
    if(nLen > nDot2)
    {
      nLen = nDot2;  // make nLen accurate for next if()
      sRtn.Truncate(nLen);
    }
    if( sRtn.EndsWith(".0") || (nLen == (nDot + 1)) )
    {
      // ends with "." or ends with ".0"
      sRtn.Truncate(nDot);
    }
  }
  return sRtn;
}

bool COARpeak::DoubleEqual(double d1,double d2, unsigned int nPlaces)
{
  // check if two doubles are almost equal
  // this is ok for numbers with an absolute value > 1
  // and up to 3 decimal places
  static double DIFF[] =
  {
    1,              // 0
    0.1,            // 1
    0.01,           // 2
    0.001,          // 3
    0.0001,         // 4
    0.00001,        // 5
    0.000001,       // 6
    0.0000001,      // 7
    0.00000001      // 8
  };
  static unsigned int SIZE_DIFF = (sizeof(DIFF) / sizeof(DIFF[0])) - 1;

  if(nPlaces > SIZE_DIFF) { nPlaces = SIZE_DIFF; }
  bool b = (d1 == d2);
  if(!b)
  {
    double dDiff = (d1 - d2);
    if(dDiff < 0) { dDiff = -dDiff; }
    b = (dDiff < DIFF[nPlaces]);
  }
  return b;
}

bool COARpeak::Equal(
  const IOARpeak &x1, const IOARpeak &x2,
  bool bCheckAllele, bool bCheckArtifact)
{
  bool bRtn = true;
  if(bCheckArtifact)
  {
    bool bArtifact = x1.IsArtifact();
    if(bArtifact != x2.IsArtifact())
    {
      bRtn = false;
    }
    else if(bArtifact)
    {
      int n1 = x1.GetCriticalLevel();
      int n2 = x2.GetCriticalLevel();
      bRtn = (n1 == n2) &&
        (x1.IsEditable() == x2.IsEditable());
    }
  }
  if(!bRtn) {;}
  else if(x1.GetID() != x2.GetID())
  { bRtn = false; }
  else if(x1.GetAlleleCount() != x2.GetAlleleCount())
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetBPS(),x2.GetBPS()))
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetMeanBPS(),x2.GetMeanBPS()))
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetRFU(),x2.GetRFU()))
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetTime(),x2.GetTime()))
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetPeakArea(),x2.GetPeakArea()))
  { bRtn = false; }
  else if(!DoubleEqual(x1.GetFit(),x2.GetFit(),FIT_DIGIT_MATCH))
  { bRtn = false; }
//  else if( bCheckArtifact && (x1.IsArtifact() != x2.IsArtifact()) )
//  { bRtn = false; }
  else if( bCheckAllele && (x1.IsAllele() != x2.IsAllele()) )
  { bRtn = false; }
  else if(x1.IsDisabled() != x2.IsDisabled())
  { bRtn = false; }
  else if(x1.GetOffLadderString() != x2.GetOffLadderString())
  { bRtn = false; }
  else if(x1.GetAlleleName() != x2.GetAlleleName())
  { bRtn = false; }
  else if(x1.GetLocusName() != x2.GetLocusName())
  { bRtn = false; }
  else if(bCheckArtifact && (x1.GetArtifactLabel() != x2.GetArtifactLabel()))
  { bRtn = false; }
  else if(bCheckArtifact && (x1.GetArtifactUserDisplay() != x2.GetArtifactUserDisplay()))
  { bRtn = false; }

  return bRtn;
}


//******** Format functions

wxString COARpeak::FormatAlleleName(const IOARpeak &x, bool bAmel, bool bCheckOffLadder)
{
  bool bOL = bCheckOffLadder ? x.IsOffLadder() : false;
  wxString s(x.GetAlleleName());
  if(bAmel)
  {
    s.Replace("1","X");
    s.Replace("2","Y");
  }
  if(bOL && !s.IsEmpty())
  {
    SetupOffLadderAlleleName(&s,bOL);
  }
  return s;
}

const wxString &COARpeak::UnFormatOffLadderString(const wxString &s)
{
  const wxString *psRtn;
  if(!s.CmpNoCase(IOARpeak::OL_ACCEPTED))
  {
    psRtn = &IOARpeak::OL_ACCEPTED;
  }
  else if( (!s.CmpNoCase(IOARpeak::OL_YES)) ||
            IOARpeak::IsOffLadderString(s) )
  {
    psRtn = &IOARpeak::OL_TRUE;
  }
  else
  {
    psRtn = &IOARpeak::OL_FALSE;
  }
  return *psRtn;
}
const wxString &COARpeak::FormatOffLadderString(const wxString &s)
{
  const wxString *psRtn;
  if(!s.CmpNoCase(IOARpeak::OL_ACCEPTED))
  {
    psRtn = &IOARpeak::OL_ACCEPTED;
  }
  else if(IOARpeak::IsOffLadderString(s))
  {
    psRtn = &IOARpeak::OL_YES;
  }
  else
  {
    psRtn = &IOARpeak::OL_NO;
  }
  return *psRtn;
}

//************************************************* COARpeakAny

COARpeakAny::~COARpeakAny()
{
;
}
void COARpeakAny::_Init()
{
  m_sName.Empty();
  m_sLocusName.Empty();
  m_sArtifactLabel.Empty();
  m_sArtifactUserDisplay.Empty();
  m_dtUpdate.Set((time_t)0);
  m_dRFU = 0.0;
  m_dTime = 0.0;
  m_dPeakArea = 0.0;
  m_dFit = 0.0;
  m_dBPS = 0.0;

  m_nID = 0;
  m_nAlleleCount = 0;
  m_nCriticalLevel = ARTIFACT_NOT_CRITICAL;  // not critical

  m_bArtifact = false;
  m_bAllele = false;
  m_bEditable = true;
  m_sOffLadder = COARpeak::OL_FALSE;
};


void COARpeakAny::SetupArtifactInfo(const IOARpeak *p)
{
  // this is used for setting artifact info in
  // a peak that was created as an allele and
  // is now being set to be an artifact also
  if(p->IsArtifact())
  {
    SetIsArtifact(true);
    SetArtifactLabel(p->GetArtifactLabel());
    SetArtifactUserDisplay(p->GetArtifactUserDisplay());
    SetCriticalLevel(p->GetCriticalLevel());
    SetIsEditable(p->IsEditable());
    const wxDateTime &dt(p->GetUpdateTime());
    if(dt > GetUpdateTime())
    {
      SetUpdateTime(dt);
    }
  }
}

int COARpeakAny::GetID() const
{
  return m_nID;
}
int COARpeakAny::GetAlleleCount() const
{
  return m_nAlleleCount;
}
int COARpeakAny::GetCriticalLevel() const
{
  return m_nCriticalLevel;
}

double COARpeakAny::GetMeanBPS() const
{
  return m_dMeanBPS;
}
double COARpeakAny::GetBPS() const
{
  return m_dBPS;
}
double COARpeakAny::GetRFU() const
{
  return m_dRFU;
}
double COARpeakAny::GetTime() const
{
  return m_dTime;
}
double COARpeakAny::GetPeakArea() const
{
  return m_dPeakArea;
}
double COARpeakAny::GetFit() const
{
  return m_dFit;
}

bool COARpeakAny::IsArtifact() const
{
  return m_bArtifact;
}
bool COARpeakAny::IsAllele() const
{
  return m_bAllele;
}
bool COARpeakAny::IsDisabled() const
{
  return !(m_bAllele || m_bArtifact);
}
bool COARpeakAny::IsEditable() const
{
  return m_bEditable;
}
const wxString &COARpeakAny::GetOffLadderString() const
{
  return m_sOffLadder;
}

const wxString &COARpeakAny::GetAlleleName() const
{
  return m_sName;
}
const wxString &COARpeakAny::GetLocusName() const
{
  return m_sLocusName;
}
const wxString &COARpeakAny::GetArtifactLabel() const
{
  return m_sArtifactLabel;
}
const wxString &COARpeakAny::GetArtifactUserDisplay() const
{
  return m_sArtifactUserDisplay;
}
const wxDateTime &COARpeakAny::GetUpdateTime() const
{
  return m_dtUpdate;
}

  // Set functions

void COARpeakAny::SetID(int n)
{
  m_nID = n;
}
void COARpeakAny::SetAlleleCount(int n)
{
  m_nAlleleCount = n;
}
void COARpeakAny::SetCriticalLevel(int n)
{
  m_nCriticalLevel = n;
}
void COARpeakAny::SetBPS(double d)
{
  m_dBPS = d;
}
void COARpeakAny::SetMeanBPS(double d)
{
  m_dMeanBPS = d;
}
void COARpeakAny::SetRFU(double d)
{
  m_dRFU = d;
}
void COARpeakAny::SetTime(double d)
{
  m_dTime = d;
}

void COARpeakAny::SetPeakArea(double d)
{
  m_dPeakArea = d;
}
void COARpeakAny::SetFit(double d)
{
  m_dFit = d;
}
void COARpeakAny::SetIsArtifact(bool b)
{
  m_bArtifact = b;
}
void COARpeakAny::SetIsAllele(bool b)
{
  m_bAllele = b;
}
void COARpeakAny::SetIsEditable(bool b)
{
  m_bEditable = b;
}
/*
//void COARpeakAny::SetIsDisabled(bool b)
{
  if(b)
  {
    m_bAllele = false;
    m_bArtifact = false;
  }
}
*/
void COARpeakAny::SetOffLadderString(const wxString &s)
{
  m_sOffLadder = s;
}


void COARpeakAny::SetAlleleName(const wxString &_s, bool bAmel)
{
  m_sName = COARpeak::SetupAlleleName(_s,bAmel);
}
void COARpeakAny::SetLocusName(const wxString &s)
{
  m_sLocusName = s;
}
void COARpeakAny::SetArtifactLabel(const wxString &s)
{
  m_sArtifactLabel = s;
}
void COARpeakAny::SetArtifactUserDisplay(const wxString &s)
{
  m_sArtifactUserDisplay = s;
}
void COARpeakAny::SetUpdateTime(const wxDateTime &x)
{
  m_dtUpdate = x;
}
void COARpeakAny::SetUpdateTimeCurrent()
{
  m_dtUpdate.SetToCurrent();
}
