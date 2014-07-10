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
*  FileName: nwxString.cpp
*  Author:   Douglas Hoffman
*
*/

#include <string.h>
#include <time.h>
#include "nwxString.h"

const wxChar * const nwxString::TIME_FORMAT_XML(wxS("%Y-%m-%dT%H:%M:%S"));

const wxChar * const nwxString::TIME_FORMAT(wxS("%Y-%m-%d %I:%M:%S %p"));
#ifdef __WXMSW__
#define __EOL wxS("\r\n")
#else
#define __EOL wxS("\n")
#endif
const wxChar * const nwxString::EOL(__EOL);
#undef __EOL

wxString nwxString::TIME_0("Original");

const wxString nwxString::ALPHA_UPPER("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const wxString nwxString::ALPHA_lower("abcdefghijklmnopqrstuvwxyz");
const wxString nwxString::NUMERIC("0123456789");
const wxString nwxString::EMPTY(wxEmptyString);
const wxChar nwxString::TO_LOWER(wxChar('a') - wxChar('A'));

bool nwxString::IsNumber(const wxString &s, bool bAllowExp)
{
  const wxChar *p = s.wc_str();
  bool bFoundDigit = false;
  bool bRtn = true;
  if(IsSign(*p))
  {
    // optional sign
    p++;
  }
  while(IsNumeric(*p))
  {
    // digits
    bFoundDigit = true;
    p++;
  }
  if((*p) == '.')
  {
    // decimal, optional
    p++;
  }
  while(IsNumeric(*p))
  {
    // digits
    bFoundDigit = true;
    p++;
  }
  if(bAllowExp && bFoundDigit)
  {
    // exp OK if at least one digit found so far
    
    bool bFoundExpDigit = false;
    if((*p) == 'e' || (*p) == 'E')
    {
      p++;
      if(IsSign(*p))
      {
        // optional sign
        p++;
      }
      while(IsNumeric(*p))
      {
        // required numeric
        p++;
        bFoundExpDigit = true;
      }
      if(!bFoundExpDigit)
      {
        // E or e found w/o digits, return false
        bRtn = false;
      }
    }
  }
  if( (*p) || !bFoundDigit )
  {
    // either no digits found 
    // or not at end of string
    bRtn = false;
  }
  return bRtn;
}

bool nwxString::IsInteger(const wxString &s, bool bAllowSign)
{
  const wxChar *p = s.wc_str();
  bool bFoundDigit = false;

  if(bAllowSign && IsSign(*p))
  {
    p++;
  }
  while(IsNumeric(*p))
  {
    p++;
    bFoundDigit = true;
  }
  bool bRtn = bFoundDigit && (!(*p));
  return bRtn;
}

bool nwxString::ContainsAlphaNumeric(const wxString &s)
{
  size_t nLen = s.Len();
  size_t i;
  wxChar x;
  bool bRtn = false;
  for(i = 0; i < nLen; i++)
  {
    x = s.GetChar(i);
    if(IsAlphaNumeric(x))
    {
      bRtn = true;
      i = nLen; // loop exit
    }
  }
  return bRtn;
}

bool nwxString::FileNameStringOK(const wxString &s)
{
  const wxString BAD("<>:\"/\\|?*'`");
  size_t nLen = s.Len();
  size_t i;
  wxChar x;
  bool bRtn = true;
  for(i = 0; i < nLen; i++)
  {
    x = s.GetChar(i);
    if( (x & 0x80) || (x < wxChar(' ')) || (BAD.find(x) != (size_t)wxNOT_FOUND) )
    {
      bRtn = false;
      i = nLen;
    }
  }
  return bRtn;
}

void nwxString::Trim(vector<wxString> *pvs, bool bRemoveEmpty)
{
  wxString s;
  vector<wxString>::iterator itr;
  vector<wxString>::iterator itrTmp;
  bool bDec = false;

  for(itr = pvs->begin();
    itr != pvs->end();
    ++itr)
  {
	  if(bDec)
	  {
		  --itr;
	  }
    s = *itr;
    Trim(&s);
	  bDec = false;
    if(bRemoveEmpty && s.IsEmpty())
    {
      itr = pvs->erase(itr);
      if(!pvs->size())
      {
        break;
      }
      else if(itr != pvs->begin())
      {
        --itr;
      }
      else
      {
	      bDec = true;
      }
    }
    else if(s != *itr)
    {
      itr = pvs->erase(itr);
      itr = pvs->insert(itr,s);
    }
  }
}

void nwxString::Trim(set<wxString> *pss, bool bRemoveEmpty)
{
  wxString s;
  set<wxString>::iterator itr;
  vector<wxString>::iterator itrv;
  vector<wxString> vs;
  vs.reserve(pss->size());

  for(itr = pss->begin();
    itr != pss->end();
    ++itr)
  {
    vs.push_back(*itr);
  }
  Trim(&vs,bRemoveEmpty);
  pss->clear();
  for(itrv = vs.begin(); itrv != vs.end(); ++itrv)
  {
    pss->insert(*itrv);
  }
}

int nwxString::FindNoCase(
  const wxString &str,
  const wxString &substr)
{
  wxString ss(str);
  wxString ssub(substr);
  ss.MakeLower();
  ssub.MakeLower();
  int nRtn = ss.Find((const wxChar *)ssub);
  return nRtn;
}

void nwxString::SetFileExtension(wxString *ps, const wxString &sExt)
{
  // STOP HERE
  wxChar dot('.');
  int nFs = ps->Find(wxChar('/'),true);
#ifdef __WXMSW__
  int nFb = ps->Find(wxChar('\\'),true);
#endif
  int nP  = ps->Find(dot,true);
  bool bUseDot =
    ( (nP != wxNOT_FOUND) // dot found
      && ((nFs == wxNOT_FOUND) || (nFs < nP)) // dot after '/'
#ifdef __WXMSW__
      && ((nFb == wxNOT_FOUND) || (nFb < nP)) // dot after '\'
#endif
    );
  if(bUseDot)
  {
    ps->Truncate(nP);
  }
  if( sExt.Len() && (sExt.GetChar(0) != dot) )
  {
    ps->Append(dot);
  }
  ps->Append(sExt);
}

wxString nwxString::FormatDateTime(time_t t)
{
	wxDateTime dt(t);
	wxString sRtn = nwxString::FormatDateTime(dt);
  return sRtn;
}
wxString nwxString::FormatDateTime(const wxDateTime &dt, const wxString &sDefault)
{
  wxString sRtn;
  if(dt.GetTicks() == 0)
  {
    sRtn = sDefault;
  }
  else
  {
    sRtn = dt.Format(nwxString::TIME_FORMAT);
  }
  return sRtn;
}
wxString nwxString::FormatDateTimeXML(const wxDateTime &dt)
{
  // works in UTC only
  return dt.Format(TIME_FORMAT_XML,wxDateTime::GMT0);
}
wxDateTime nwxString::UnFormatDateTimeXML(const wxString &s)
{
  // works in UTC only
  wxDateTime dtRtn((time_t)0);
  wxDateTime dt((time_t)0);
  const wxChar *ps = dt.ParseFormat(
    s,TIME_FORMAT_XML);
  if(ps != NULL)
  {
    dtRtn = dt.FromTimezone(wxDateTime::GMT0);
  }
  return dtRtn;
}

size_t nwxString::Split(const char *ps, vector<wxString> *pvs, const char *psSearch)
{
  size_t nRtn = 0;
  size_t nReserve = 4;
  size_t nSepLen = strlen(psSearch);
  pvs->clear();
  pvs->reserve(nReserve);
  const char *psSubString = ps;
  const char *psSep;
  if(nSepLen > 0)
  {
    for(psSep = strstr(psSubString,psSearch);
      psSep != NULL;
      psSep = strstr(psSubString,psSearch) )
    {
      if(nRtn >= nReserve)
      {
        nReserve <<= 1;
        pvs->reserve(nReserve);
      }
      if(psSep != psSubString)
      {
        pvs->push_back(wxString(psSubString,psSep - psSubString));
      }
      else
      {
        pvs->push_back(wxEmptyString);
      }
      psSep += nSepLen;
      psSubString = psSep;
      nRtn++;
    }
  }
  if(nRtn || *psSubString)
  {
    pvs->push_back(wxString(psSubString));
    nRtn++;
  }
  return nRtn;
}
size_t nwxString::Split(const char *ps, set<wxString> *pss, const char *psSearch)
{
  vector<wxString> vs;
  vector<wxString>::iterator itr;
  Split(ps,&vs,psSearch);
  pss->clear();
  for(itr = vs.begin(); itr != vs.end(); ++itr)
  {
    pss->insert(*itr);
  }
  return pss->size();
}
void nwxString::Join(const vector<wxString> &vs, wxString *ps, const char *psSep)
{
  wxString sSep(wxEmptyString);
  size_t nReserve = 256;
  size_t nLen = 0;
  size_t nSepLen = 0;
  bool bFirst = true;
  ps->Empty();
  ps->Alloc(nReserve);
  for(vector<wxString>::const_iterator itr = vs.begin();
    itr != vs.end();
    ++itr)
  {
    nLen += (*itr).Len();
    nLen += nSepLen;
    if(nLen >= nReserve)
    {
      while(nLen >= nReserve)
      {
        nReserve <<= 1;
      }
      ps->Alloc(nReserve);
    }
    (*ps) += sSep;
    (*ps) += (*itr);
    if(bFirst)
    {
      bFirst = false;
      if(psSep != NULL)
      {
        sSep = psSep;
        nSepLen = sSep.Len();
      }
    }
  }
}

void nwxString::Join(const set<wxString> &ss, wxString *ps, const char *psSep)
{
  wxString sSep(wxEmptyString);
  size_t nReserve = 256;
  size_t nLen = 0;
  size_t nSepLen = 0;
  bool bFirst = true;
  ps->Empty();
  ps->Alloc(nReserve);
  for(set<wxString>::const_iterator itr = ss.begin();
    itr != ss.end();
    ++itr)
  {
    nLen += (*itr).Len();
    nLen += nSepLen;
    if(nLen >= nReserve)
    {
      while(nLen >= nReserve)
      {
        nReserve <<= 1;
      }
      ps->Alloc(nReserve);
    }
    (*ps) += sSep;
    (*ps) += (*itr);
    if(bFirst)
    {
      bFirst = false;
      if(psSep != NULL)
      {
        sSep = psSep;
        nSepLen = sSep.Len();
      }
    }
  }
}

void nwxString::FixEOLU(wxString *ps, bool bClearEmpty)
{
  // set all end-of-line strings to unix style, '\n'
  ps->Replace("\r\n","\n",true);
  ps->Replace("\r","\n",true);
  if(bClearEmpty)
  {
    while(ps->Replace("\n\n","\n",true)) {;}
    if(ps->StartsWith("\n"))
    {
      ps->Remove(0);
    }
    if(ps->EndsWith("\n"))
    {
      ps->Remove(ps->Len() - 1);
    }
  }
}

#ifdef _DEBUG

void nwxString::UnitTest_FindNoCase()
{
  wxString s("One of These Nights");
  wxString sub("nIGH");
  int nV = 13;
  int n = FindNoCase(s,sub);
  wxASSERT_MSG(n == nV,
    "nwxString::UnitTest_FindNoCase() failed");
}

void nwxString::UnitTest_Trim()
{
  vector<wxString> vs;
  wxString s;
  const char *ps[] =
  {
    " \n\n \r \t ",
    "  This ",
    " is ",
    "",
    "    a",
    "test  "
  };
  size_t n = sizeof(ps) / sizeof(ps[0]);
  size_t i;
  vs.reserve(n);
  for(i = 0; i < n; i++)
  {
    s = ps[i];
    vs.push_back(s);
  }
  Trim(&vs);
  Join(vs,&s,'_');
  bool bOK = (vs.size() == 4) &&
    (s == "This_is_a_test");
  wxASSERT_MSG(bOK,"nwxString::UnitTest_Trim() failed");
  vs.clear();
  vs.push_back("   x   ");
  Trim(&vs);
  vs.clear();
  vs.push_back("      ");
  Trim(&vs);
}

void nwxString::UnitTest_DateTime()
{
  wxDateTime dt;
  wxDateTime dt2;
  wxString s;
  wxString s2;
  wxString sMsg;
  dt.SetToCurrent();
  s = nwxString::FormatDateTimeXML(dt);
  dt2 = nwxString::UnFormatDateTimeXML(s);
  s2 = nwxString::FormatDateTimeXML(dt2);
  if((dt != dt2) || (s != s2))
  {
    sMsg = "nwxString::UnitTextDateTime failed\n";
    sMsg.Append(s);
    sMsg.Append("\n");
    sMsg.Append(s2);
    wxASSERT_MSG(0,sMsg);
  }
}

void nwxString::UnitTest_FileExt()
{
  wxString sf1("C:/dir1/dir2/foo.dll");
  wxString sf2("C:\\dir3\\dir4\\bar.dll");
#ifdef __WXMSW__
  wxString sf3("C:\\dir5\\dir.6\\xxx");
#else
  wxString sf3("/home/whatever/dir5/dir.6/xxx");
#endif
  nwxString::SetFileExtension(&sf1,"txt");
  nwxString::SetFileExtension(&sf2,".txt");
  nwxString::SetFileExtension(&sf3,".zip");
  bool b1 = (sf1 == "C:/dir1/dir2/foo.txt");
  bool b2 = (sf2 == "C:\\dir3\\dir4\\bar.txt");
#ifdef __WXMSW__
  bool b3 = (sf3 == "C:\\dir5\\dir.6\\xxx.zip");
#else
  bool b3 = (sf3 == "/home/whatever/dir5/dir.6/xxx.zip");
#endif
  if(!(b1 && b2 && b3))
  {
    wxString sfTxt;
    sfTxt.Alloc(256);
    sfTxt = " Problem with File Ext:\n";
    sfTxt.Append(sf1);
    sfTxt.Append("\n");
    sfTxt.Append(sf2);
    sfTxt.Append("\n");
    sfTxt.Append(sf3);
    wxASSERT_MSG(0,sfTxt);
  }
}

#endif
