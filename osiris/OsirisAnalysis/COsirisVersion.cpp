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
*  FileName: COsirisVersion.cpp
*  Author:   Douglas Hoffman
*
*/
#include "COsirisVersion.h"
#include "nwx/nwxString.h"
#include <stdlib.h>
#ifdef __WXDEBUG__
#include "nwx/nwxLog.h"
#endif
#define NO_BETA 32767


nwxIMPLEMENT_GLOBAL_OBJECT(COsirisVersion)

void COsirisVersion::_Setup(const wxString &_s)
{

  size_t n;

  // cleanup
  const wxUniChar DOT(L'.');
  const wxString sFIND_OS(OSIRIS_VERS_SEARCH_OS);
  const wxString BETA(" beta ");
  m_nMajor = 0;
  m_nMinor = 0;
  m_nRelease = 0;
  m_nBeta = NO_BETA; // if not beta, use a high number
  n = _s.Find(sFIND_OS);
  wxString s;
  if(n > 0)
  {
    m_sOS = _s.Mid(n + sFIND_OS.Len());
    s = _s.Left(n);
  }
  else
  {
    s = _s;
    m_sOS.Clear();
  }
  n = nwxString::FindInt(s);
  if(n > 0)
  {
    s = s.Mid(n);
  }
  s.MakeLower();
  if(s.Len())
  {
    // s[0] is an int, get major version
    n = nwxString::FindNonInt(s);
    m_nMajor = atoi(s.Left(n).utf8_str());
    if((n < s.Len()) && (s.GetChar(n) == DOT))
    {
      // found a dot, get minor version
      s = s.Mid(n+1);
      n = nwxString::FindNonInt(s);
      if(n > 0)
      {
        m_nMinor = atoi(s.Left(n).utf8_str());
        if( (n < s.Len()) && (s.GetChar(n) == DOT) )
        {
          // found another dot, get release
          s = s.Mid(n+1);
          n = nwxString::FindNonInt(s);
          if(n > 0)
          {
            m_nRelease = atoi(s.Left(n).utf8_str());
            s = s.Mid(n);
          }
        }
      }
    }
    n = s.Find(BETA);
    if(n != wxNOT_FOUND)
    {
      s = s.Mid(n + BETA.Len());
      n = nwxString::FindNonInt(s);
      if(n > 0)
      {
        m_nBeta = atoi(s.Left(n).utf8_str());
      }
    }
  }
}

#ifdef __WXDEBUG__

void COsirisVersion::UnitTest()
{
#define FUNCTION_NAME "COsirisVersion::UnitTest() "
  COsirisVersion xCurrent(OSIRIS_FULLNAME);
  wxString s = xCurrent.ToString();
  wxString sError;
  int nErrors = 0;
  wxString sOSIRIS_VERSION(OSIRIS_VERSION);
  if(s != sOSIRIS_VERSION)
  {
    sError = wxS(FUNCTION_NAME);
    sError.Append(wxS("ERROR: ToString = "));
    sError.Append(s);
    sError.Append(wxS("; original = "));
    sError.Append(OSIRIS_FULLNAME);
    nwxLog::LogMessage(sError);
    nErrors++;
  }
  s = OSIRIS_FULLNAME;
  size_t n = s.Find(wxS("Beta"));
  if(n == wxNOT_FOUND) { n = s.Find("beta"); }
  if(n != wxNOT_FOUND)
  {
    size_t n2 = s.Find(OSIRIS_VERS_SEARCH_OS);
    if((n2 > n) && (n2 != wxNOT_FOUND))
    {
      wxString s2 = s.Left(n) + s.Mid(n2);
      COsirisVersion xNext(s2);
      if(!(xNext > xCurrent))
      {
        sError = wxS(FUNCTION_NAME);
        sError.Append("Next is not > current\ncurrent: ");
        sError.Append(xCurrent.ToString());
        sError.Append("\nnext: ");
        sError.Append(xNext.ToString());
        nwxLog::LogMessage(sError);
        nErrors++;
      }
    }
  }
  // another test
  COsirisVersion xPC("Osiris 2.2.1.4 for Windows");
  COsirisVersion xLN("Osiris 2.2.1.9 for Linux");
  if(!xPC.Equals(xLN,true))
  {
    sError = wxS(FUNCTION_NAME);
    sError.Append("false unequal WITHOUT name between\n");
    sError.Append(xPC.ToString());
    sError.Append("\nand\n");
    sError.Append(xLN.ToString());
    nErrors++;
    nwxLog::LogMessage(sError);
  }
  if(xPC.Equals(xLN,false))
  {
    sError = wxS(FUNCTION_NAME);
    sError.Append("false equal with name between\n");
    sError.Append(xPC.ToString());
    sError.Append("\nand\n");
    sError.Append(xLN.ToString());
    nErrors++;
    nwxLog::LogMessage(sError);
  }
  if(!nErrors)
  {
    sError = wxS(FUNCTION_NAME);
    sError.Append(" tests passed!");
    nwxLog::LogMessage(sError);
  }
}

wxString COsirisVersion::ToString() const
{
  wxChar DOT = L'.';
  wxString sRtn = nwxString::FormatNumber(m_nMajor);
  sRtn.Append(DOT);
  sRtn.Append(nwxString::FormatNumber(m_nMinor));
  if(m_nRelease > 0)
  {
    sRtn.Append(DOT);
    sRtn.Append(nwxString::FormatNumber(m_nRelease));
  }
  if(m_nBeta != NO_BETA)
  {
    sRtn.Append(wxS(" Beta "));
    sRtn.Append(nwxString::FormatNumber(m_nBeta));
  }
  if(!m_sOS.IsEmpty())
  {
    sRtn.Append(wxS(OSIRIS_VERS_SEARCH_OS));
    sRtn.Append(m_sOS);
  }
  return sRtn;
}
#endif