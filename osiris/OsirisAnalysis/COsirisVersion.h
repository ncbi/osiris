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
*  FileName: COsirisVersion.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_OSIRIS_VERSION_H__
#define __C_OSIRIS_VERSION_H__
#include "OsirisVersion.h"
#include <wx/string.h>
#include "nwx/nwxGlobalObject.h"

class COsirisVersion
{
public:
  COsirisVersion()
  {
    _Setup(OSIRIS_VERSION);
  }
  COsirisVersion(const wxString &sVersion)
  {
    _Setup(sVersion);
  }
  COsirisVersion(const COsirisVersion &x)
  {
    (*this) = x;
  }
  COsirisVersion &operator = (const COsirisVersion &x)
  {
    m_sOS = x.m_sOS;
    m_nMajor = x.m_nMajor;
    m_nMinor = x.m_nMinor;
    m_nRelease = x.m_nRelease;
    m_nBeta = x.m_nBeta;
    return (*this);
  }
  bool Equals(const COsirisVersion &x, bool bIgnoreOS = false) const
  {
      bool bRtn =
        (m_nMajor == x.m_nMajor) &&
        (m_nMinor == x.m_nMinor) &&
        (m_nRelease == x.m_nRelease) &&
        (m_nBeta == x.m_nBeta) &&
          (bIgnoreOS || (m_sOS == x.m_sOS));
      return bRtn;
  }
  bool operator < (const COsirisVersion &x) const
  {
    bool bRtn = false;
#define OS_VER_COMPARE(nn) \
    else if(nn < x.nn)  { bRtn = true; } \
    else if(nn != x.nn) { bRtn = false; }

    if(0) {}
    OS_VER_COMPARE(m_nMajor)
    OS_VER_COMPARE(m_nMinor)
    OS_VER_COMPARE(m_nRelease)
    OS_VER_COMPARE(m_nBeta)

    return bRtn;
#undef OS_VER_COMPARE
  }
  bool operator > (const COsirisVersion &x) const
  {
    bool bRtn = (x < *this);
    return bRtn;
  }
#ifdef __WXDEBUG__
  static void UnitTest();
  wxString ToString() const;
#endif
private:
  void _Setup(const wxString &s);
  wxString m_sOS;
  int m_nMajor;
  int m_nMinor;
  int m_nRelease;
  int m_nBeta;

nwxDECLARE_GLOBAL_OBJECT(COsirisVersion)
};

#endif
