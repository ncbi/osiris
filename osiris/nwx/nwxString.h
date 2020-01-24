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

*  FileName: nwxString.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_STRING_H__
#define __NWX_STRING_H__

#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"

#include <wx/string.h>
#include <wx/datetime.h>
#include "nsstd.h"

class nwxString
{
  // string utilities
public:
  static size_t Split(const char *ps, vector<wxString> *pvs, const char *psSep);
  static size_t Split(const char *ps, set<wxString> *pvs, const char *psSep);
  static void Join(const vector<wxString> &vs, wxString *ps, const char *psSep);
  static void Join(const set<wxString> &vs, wxString *ps, const char *psSep);
  
  static void FixEOLU(wxString *ps, bool bClearEmpty = false);

  static size_t Split(const char *ps, vector<wxString> *pvs, const char csep = '\t')
  {
    char x[2] = {csep,0};
    return Split(ps,pvs,x);
  }
  static size_t Split(const char *ps, set<wxString> *pss, const char csep = '\t')
  {
    char x[2] = {csep,0};
    return Split(ps,pss,x);
  }
  static void Join(const vector<wxString> &vs, wxString *ps, const char csep = '\t')
  {
    char x[2] = {csep,0};
    Join(vs,ps,x);
  }
  static void Join(const set<wxString> &ss, wxString *ps, const char csep = '\t')
  {
    char x[2] = {csep,0};
    Join(ss,ps,x);
  }
  static void Append(wxString *ps1, const wxString &s2, const wxChar *psSep = wxT("\n"))
  {
    if(!s2.IsEmpty())
    {
      if(!ps1->IsEmpty())
      {
        ps1->Append(psSep);
      }
      ps1->Append(s2);
    }
  }
  static size_t SplitLines(const char *ps, vector<wxString> *pvs, bool bClearEmpty = false, bool bTrim = false)
  {
    wxString s(ps);
    FixEOLU(&s,bClearEmpty);
    size_t nRtn = Split(s.utf8_str(),pvs,"\n");
    if(bTrim && (nRtn > 0))
    {
      Trim(pvs);
      nRtn = pvs->size();
    }
    return nRtn;
  }
  static size_t SplitLines(const char *ps, set<wxString> *pss, bool bClearEmpty = false, bool bTrim = false)
  {
    wxString s(ps);
    FixEOLU(&s,bClearEmpty);
    size_t nRtn = Split(s.utf8_str(),pss,"\n");
    if(bTrim && (nRtn > 0))
    {
      Trim(pss);
      nRtn = pss->size();
    }
    return nRtn;
  }

  static bool All8bit(const wxChar *p)
  {
    bool bRtn = true;
    for (; *p; ++p)
    {
      if (*p > 255)
      {
        bRtn = false;
        break;
      }
    }
    return bRtn;
  }
  static int FindNoCase(
    const wxString &str,
    const wxString &subStr);
  static bool ContainsNoCase(
    const wxString &str, const wxString &subStr)
  {
    return (FindNoCase(str,subStr) != wxNOT_FOUND);
  }
  static void SetFileExtension(wxString *ps, const wxString &sExt);
  static wxString FormatDateTime(time_t t);
  static wxString FormatDateTime(const wxDateTime &dt,const wxString &sDefault = nwxString::TIME_0);
  static wxString FormatDateTimeXML(const wxDateTime &dt);
  static wxDateTime UnFormatDateTimeXML(const wxString &s);
  static wxString FormatNumber(unsigned int n)
  {
    return wxString::Format("%u",n);
  }
  static wxString FormatNumber(int n)
  {
    return wxString::Format("%d",n);
  }
  static wxString FormatNumber(double d)
  {
    return wxString::Format("%g",d);
  }
  static wxString FormatDouble
		(double d,const char *psFmt = "%g", bool bZeroBlank = true)
  {
    wxString s;
    if( (!bZeroBlank) || (d != 0.0) )
    {
      s.Printf(psFmt,d);
    }
    return s;
  }
  static void Trim(wxString *ps)
  {
    ps->Trim(true);
    ps->Trim(false);
  }
  static void Trim(vector<wxString> *pvs,bool bRemoveEmpty = true);
  static void Trim(set<wxString> *pss,bool bRemoveEmpty = true);
  static void TrimLower(wxString *ps)
  {
    Trim(ps);
    ps->MakeLower();
  }
  static wxChar MakeUpperChar(wxChar x)
  {
    wxChar rtn = IsLower(x) ? (x - TO_LOWER) : x;
    return rtn;
  }
  static wxChar MakeLowerChar(wxChar x)
  {
    wxChar rtn = IsUpper(x) ? (x + TO_LOWER) : x;
    return rtn;
  }
  static bool IsUpper(wxChar x)
  {
    bool bRtn = (ALPHA_UPPER.Find(x) != wxNOT_FOUND);
    return bRtn;
  }
  static bool IsLower(wxChar x)
  {
    bool bRtn = (ALPHA_lower.Find(x) != wxNOT_FOUND);
    return bRtn;
  }  
  static bool IsNumeric(wxChar x)
  {
    bool bRtn = (NUMERIC.Find(x) != wxNOT_FOUND);
    return bRtn;
  }
  static bool IsAlpha(wxChar x)
  {
    bool bRtn = IsLower(x) || IsUpper(x);
    return bRtn;
  }
  static bool IsAlphaNumeric(wxChar x)
  {
    bool bRtn = IsNumeric(x) || IsAlpha(x);
    return bRtn;
  }
  static bool IsSign(wxChar x)
  {
    bool bRtn = (x == '+') || (x == '-');
    return bRtn;
  }
  static size_t FindNonInt(const wxString &_s)
  {
    size_t nLen = _s.Len();
    size_t n;
    for (n = 0; (n < nLen) && nwxString::IsNumeric(_s.GetChar(n)); ++n) {}
    return n;
  }
  static size_t FindInt(const wxString &_s)
  {
    size_t nLen = _s.Len();
    size_t n;
    for (n = 0; (n < nLen) && !nwxString::IsNumeric(_s.GetChar(n)); ++n) {}
    return n;
  }

  static bool IsNumber(const wxString &s,bool bAllowExp = true);
  static bool IsInteger(const wxString &s, bool bAllowSign = true);


  static bool ContainsAlphaNumeric(const wxString &s);
  static int Compare(const wxString &_s1, const wxString &_s2)
  {
    wxString s1(_s1);
    wxString s2(_s2);
    Trim(&s1);
    Trim(&s2);
    return s1.CmpNoCase(s2);
  }
  static int CompareSort(const wxString &_s1, const wxString &_s2)
  {
    // case sensitive, but for sorting non case sensitive
    //
    //  A < a
    //  a < AB
    //
    wxString s1(_s1);
    wxString s2(_s2);
    Trim(&s1);
    Trim(&s2);
    int nRtn = s1.CmpNoCase(s2);
    if(!nRtn)
    {
      nRtn = s1.Cmp(s2);
    }
    return nRtn;
  }
  static bool FileNameStringOK(const wxString &s);
  static bool FileNameStringEqual(const wxString &a, const wxString &b)
  {
    // used for comparing file names
#ifdef __WXMSW__
    bool bRtn = !a.CmpNoCase(b);
#else
    bool bRtn = !a.Cmp(b);
#endif
    return bRtn;
  }

  static const wxChar * const TIME_FORMAT_XML;
  static const wxChar * const TIME_FORMAT;
  static const wxChar * const EOL;
  static wxString TIME_0;
  static const wxString ALPHA_lower;
  static const wxString ALPHA_UPPER;
  static const wxString NUMERIC;
  static const wxString EMPTY;
  static const wxChar TO_LOWER;

#ifdef _DEBUG
  static void UnitTest_FileExt();
  static void UnitTest_DateTime();
  static void UnitTest_Trim();
  static void UnitTest_FindNoCase();
#endif

private:
  nwxString(){;} // prevent instantiation
};

class nwxStringLessFileName
{
public:
  nwxStringLessFileName() {;}
  bool operator()(const wxString &x1, const wxString &x2) const
  {
#ifdef __WXMSW__
    int n = x1.CmpNoCase(x2);
#else
    int n = x1.Cmp(x2);
#endif
    return (n < 0);
  }
  bool operator()(const wxString *px1, const wxString *px2) const
  {
    return (*this)(*px1,*px2);
  }
};

class nwxStringLessNoCase
{
public:
  nwxStringLessNoCase() {;}
  bool operator()(const wxString &x1, const wxString &x2) const
  {
    int n = x1.CmpNoCase(x2);
    return (n < 0);
  }
  bool operator()(const wxString *px1, const wxString *px2) const
  {
    return (*this)(*px1,*px2);
  }
};
class nwxStringLessNoCaseSort
{
  // similar to nwxStringLessNoCase, but if the strings
  // match with case insensitive comparison,
  // then check for case sensitive comparison.
  //
  //  this creates the following scenario
  //
  //  'ABC' < 'abc' < 'ABCD'
  //  and 'ABC' != 'abc'
  //
  //  This is useful for case sensitive sets with
  //  case insensitive sorting
public:
  nwxStringLessNoCaseSort() {;}
  bool operator()(const wxString &x1, const wxString &x2) const
  {
    int n = x1.CmpNoCase(x2);
    if(!n)
    {
      n = x1.Cmp(x2);
    }
    return (n < 0);
  }
  bool operator()(const wxString *px1, const wxString *px2) const
  {
    return (*this)(*px1,*px2);
  }
};

#endif
