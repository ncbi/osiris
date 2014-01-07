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
*  FileName: CAlertViewStatus.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_ALERT_VIEW_STATUS_H__
#define __C_ALERT_VIEW_STATUS_H__

#include "nwx/nwxXmlPersist.h"

class CAlertViewStatus
{
public:
  CAlertViewStatus()
  {
    m_nStatus = ALL;
  }
  CAlertViewStatus(int nStatus)
  {
    m_nStatus = nStatus;
  }
  CAlertViewStatus(const CAlertViewStatus &x)
  {
    m_nStatus = x.m_nStatus;
  }
  CAlertViewStatus &operator = (const CAlertViewStatus &x)
  {
    m_nStatus = x.m_nStatus;
    return *this;
  }
  CAlertViewStatus &operator = (int n)
  {
    m_nStatus = n;
    return *this;
  }
  operator int() const
  {
    return m_nStatus;
  }
  bool operator == (int n)
  {
    return m_nStatus == n;
  }
  bool operator != (int n)
  {
    return !((*this) == n);
  }
  bool operator == (const CAlertViewStatus &x)
  {
    return m_nStatus == x.m_nStatus;
  }
  bool operator != (const CAlertViewStatus &x)
  {
    return !((*this) == x);
  }

  typedef enum
  {
    DIRECTORY =        1,
    SAMPLE =           2,
    CHANNEL =          4,
    ILS =              8,
    SAMPLE_LOCUS =    16,
    DIRECTORY_LOCUS = 32,

    _BIGPLUS1,
    ALL = _BIGPLUS1 + _BIGPLUS1 - 3
  } TYPE;


  int GetPlus() const
  {
    // returns status with all future items set
    return m_nStatus | (~ALL);
  }
  int Get() const
  {
    return m_nStatus;
  }
  void SetAll()
  {
    m_nStatus = ALL;
  }
  bool IsAll() const
  {
    return _Get(ALL);
  }
  bool IsNone() const
  {
    return (m_nStatus & ALL) == 0;
  }
  void Set(int n)
  {
    m_nStatus = (n & ALL);
  }
  void Clear()
  {
    Set(0);
  }

  bool GetDirectory() const
  {
    return _Get(DIRECTORY);
  }
  bool GetSample() const
  {
    return _Get(SAMPLE);
  }
  bool GetChannel() const
  {
    return _Get(CHANNEL);
  }
  bool GetILS() const
  {
    return _Get(ILS);
  }
  bool GetSampleLocus() const
  {
    return _Get(SAMPLE_LOCUS);
  }
  bool GetDirectoryLocus() const
  {
    return _Get(DIRECTORY_LOCUS);
  }
  bool IsValueSet(int n) const
  {
    return _Get(n);
  }

  void SetValue(int nValue, bool b = true)
  {
    _Set(b,nValue);
  }

  void SetDirectory(bool b = true)
  {
    _Set(b,DIRECTORY);
  }
  void SetSample(bool b = true)
  {
    _Set(b,SAMPLE);
  }
  void SetChannel(bool b = true)
  {
    _Set(b,CHANNEL);
  }
  void SetILS(bool b = true)
  {
    _Set(b,ILS);
  }
  void SetSampleLocus(bool b = true)
  {
    _Set(b,SAMPLE_LOCUS);
  }
  void SetDirectoryLocus(bool b  = true)
  {
    _Set(b,DIRECTORY_LOCUS);
  }
private:
  void _Set(int n)
  {
    m_nStatus |= n;
  }
  void _Clear(int n)
  {
    m_nStatus &= (~n);
  }
  void _Set(bool bSet, int n)
  {
    if(bSet) { _Set(n); }
    else     { _Clear(n); }
  }
  bool _Get(int n) const
  {
    bool b = ((m_nStatus & n) == n);
    return b;
  }

  int m_nStatus;
};

#endif
