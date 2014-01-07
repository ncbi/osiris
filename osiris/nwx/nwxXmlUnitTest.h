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
*  FileName: nwxXmlUnitTest.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_XML_UNIT_TEST_H__
#define __NWX_XML_UNIT_TEST_H__

#ifdef _DEBUG

#include <wx/mstream.h>
#include <wx/wfstream.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"

class nwxXmlUnitClass2 : public nwxXmlPersist
{
public:
  nwxXmlUnitClass2()
  {
    RegisterAll(true);
  }
  virtual ~nwxXmlUnitClass2() {}
  int GetNr() const
  {
    return m_nr;
  }
  bool IsEqual(const nwxXmlUnitClass2 &x) const;
  bool operator == (const nwxXmlUnitClass2 &x) const
  {
    return IsEqual(x);
  }
  bool operator != (const nwxXmlUnitClass2 &x) const
  {
    return !IsEqual(x);
  }
protected:
  virtual void RegisterAll(bool = false);

private:
  wxString m_sa;
  wxString m_sc;
  wxString m_se;
  wxString m_sType;
  wxString m_sc2s;
  double m_dc2d;
  int m_nr;
  int m_nc2n;
  bool m_bc2b;
};

class C2Less
{
public:
  C2Less(){;}
  virtual ~C2Less(){;}
  bool operator ()(const nwxXmlUnitClass2 *p1,const nwxXmlUnitClass2 *p2) const
  {
    int n1 = p1->GetNr();
    int n2 = p2->GetNr();
    return (n1 < n2);
  }
};

class nwxXmlUnitClass3 : public nwxXmlPersist
{
public:
  nwxXmlUnitClass3()
  {
    RegisterAll(true);
  }
  virtual ~nwxXmlUnitClass3()
  {}
  bool IsEqual(const nwxXmlUnitClass3 x) const;
  bool operator == (const nwxXmlUnitClass3 x) const
  {
    return IsEqual(x);
  }
  bool operator != (const nwxXmlUnitClass3 x) const
  {
    return !IsEqual(x);
  }
protected:
  virtual void RegisterAll(bool = false);
private:
  wxString m_sType; // attr type
  wxString m_sName; //attr name
  wxString m_sx;
  int m_na;
};

class nwxXmlNameNumber : public nwxXmlPersist
{
public:
  nwxXmlNameNumber(wxString *psName, double *pdNumber) :
      m_psName(psName),
      m_pdNumber(pdNumber)
  {
    RegisterAll(true);
  }
  virtual ~nwxXmlNameNumber()
  {}

protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxStringNotEmptyAttribute(_T("name"),m_psName);
    RegisterDoubleText(m_pdNumber);
  }
private:
  wxString *m_psName;
  double *m_pdNumber;
};
class nwxXmlNameString : public nwxXmlPersist
{
public:
  nwxXmlNameString(wxString *psName, wxString *psText) :
      m_psName(psName),
      m_psText(psText)
  {
    RegisterAll(true);
  }
  virtual ~nwxXmlNameString()
  {}

protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxStringNotEmptyAttribute(_T("name"),m_psName);
    RegisterWxStringText(m_psText);
  }
private:
  wxString *m_psName;
  wxString *m_psText;
};

class nwxXmlTimeZone : public nwxXmlPersist
{
public:
  nwxXmlTimeZone(wxDateTime *pt, wxString *psZone) :
      m_pt(pt),
      m_psZone(psZone)
  {
    RegisterAll(true);
  }
  virtual ~nwxXmlTimeZone()
  {}

protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxStringNotEmptyAttribute(_T("zone"),m_psZone);
    RegisterWxDateTimeXMLText(m_pt);
  }
private:
  wxDateTime *m_pt;
  wxString *m_psZone;
};

class nwxXmlUnitTest : public nwxXmlPersist
{
public:
  static bool RunUnitTest();
  nwxXmlUnitTest() : m_sRoot(_T("unit"))
  {
    pnn = new nwxXmlNameNumber(&m_saNumberName,&m_dNumber);
    pns1 = new nwxXmlNameString(&m_saString1Name, &m_sString1);
    pns2 = new nwxXmlNameString(&m_saString2Name, &m_sString2);
    ptz = new nwxXmlTimeZone(&m_tTime,&m_saTimeZone);
    m_vec.SetNodeName(_T("class3"));
    m_set.SetNodeName(_T("class2"));
    RegisterAll(true);
  }
  virtual ~nwxXmlUnitTest()
  {
    delete pnn;
    delete pns1;
    delete pns2;
    delete ptz;
  }
  virtual const wxString &RootNode(void) const
  {
    return m_sRoot;
  }
  bool IsEqual(const nwxXmlUnitTest &x) const;
  bool operator == (const nwxXmlUnitTest &x) const
  {
    return IsEqual(x);
  }
  bool operator != (const nwxXmlUnitTest &x) const
  {
    return !IsEqual(x);
  }


protected:
  virtual void RegisterAll(bool bInConstructor = false);

private:
  wxDateTime m_tTime;
  TnwxXmlPersistSet<nwxXmlUnitClass2,C2Less> m_set;
  TnwxXmlPersistVector<nwxXmlUnitClass3> m_vec;

  wxString m_sRoot;
  wxString m_sString1;
  wxString m_saString1Name;
  wxString m_sString2;
  wxString m_saString2Name;
  wxString m_saNumberName;
  wxString m_saTimeZone;

  double m_dNumber;

  nwxXmlNameNumber *pnn;
  nwxXmlNameString *pns1;
  nwxXmlNameString *pns2;
  nwxXmlTimeZone *ptz;

  int m_nAttr1;
  int m_nAttr2;

  bool m_bBool;

  
};

#ifdef NWX_XML_UNIT_TEST_CODE

#define XUCMP(z) else if(z != x.z) { bRtn = false; }

bool nwxXmlUnitClass2::IsEqual(const nwxXmlUnitClass2 &x) const
{
  bool bRtn = true;
  if(0) {}
  XUCMP(m_sa)
  XUCMP(m_sc)
  XUCMP(m_se)
  XUCMP(m_sType)
  XUCMP(m_sc2s)
  XUCMP(m_dc2d)
  XUCMP(m_nr)
  XUCMP(m_nc2n)
  XUCMP(m_bc2b)
  return bRtn;
}
void nwxXmlUnitClass2::RegisterAll(bool)
{
  RegisterInt(_T("c2n"),&m_nc2n);
  RegisterWxString(_T("c2s"),&m_sc2s);
  RegisterBool(_T("c2b"),&m_bc2b);
  RegisterDouble(_T("c2d"),&m_dc2d);
  RegisterIntAttribute(_T("nr"),&m_nr);
  RegisterWxStringNotEmptyAttribute(_T("a"),&m_sa);
  RegisterWxStringNotEmptyAttribute(_T("c"),&m_sc);
  RegisterWxStringNotEmptyAttribute(_T("e"),&m_se);
  RegisterWxStringNotEmptyAttribute(_T("type"),&m_sType);
}

bool nwxXmlUnitClass3::IsEqual(const nwxXmlUnitClass3 x) const
{
  bool bRtn = true;
  if(0) {;}
  XUCMP(m_sType)
  XUCMP(m_sName)
  XUCMP(m_sx)
  XUCMP(m_na)
  return bRtn;
}
void nwxXmlUnitClass3::RegisterAll(bool)
{
  RegisterWxString(_T("x"),&m_sx);
  RegisterInt(_T("a"),&m_na);
  RegisterWxStringNotEmptyAttribute(_T("type"),&m_sType);
  RegisterWxStringNotEmptyAttribute(_T("name"),&m_sName);
}

void nwxXmlUnitTest::RegisterAll(bool)
{
  Register(_T("string1"),pns1);
  Register(_T("string2"),pns2);
  Register(_T("number"),pnn);
  RegisterBool(_T("bool"),&m_bBool);
  Register(_T("time"),ptz);
  Register(_T("set"),&m_set);
  Register(_T("vector"),&m_vec);
  RegisterIntAttribute(_T("attr1"),&m_nAttr1);
  RegisterIntAttribute(_T("attr2"),&m_nAttr2);
}

bool nwxXmlUnitTest::IsEqual(const nwxXmlUnitTest &x) const
{
  bool bRtn = true;
  const set<nwxXmlUnitClass2 *,C2Less> *pSet1 = m_set.Get();
  const set<nwxXmlUnitClass2 *,C2Less> *pSet2 = x.m_set.Get();
  const vector<nwxXmlUnitClass3 *> *pVec1 = m_vec.Get();
  const vector<nwxXmlUnitClass3 *> *pVec2 = x.m_vec.Get();

  if(pVec1->size() != pVec2->size())
  { bRtn = false; }
  else if(pSet1->size() != pSet2->size())
  { bRtn = false; }
  XUCMP(m_sRoot)
  XUCMP(m_sString1)
  XUCMP(m_saString1Name)
  XUCMP(m_sString2)
  XUCMP(m_saString2Name)
  XUCMP(m_saNumberName)
  XUCMP(m_saTimeZone)
  XUCMP(m_dNumber)
  XUCMP(m_nAttr1)
  XUCMP(m_nAttr2)
  XUCMP(m_bBool)
  else
  {
    const nwxXmlUnitClass2 *p2a;
    const nwxXmlUnitClass2 *p2b;
    set<nwxXmlUnitClass2 *,C2Less>::const_iterator itrs;
    set<nwxXmlUnitClass2 *,C2Less>::const_iterator itrsx =
      pSet2->begin();
    for(itrs = pSet1->begin();
      itrs != pSet1->end();
      ++itrs)
    {
      p2a = *itrs;
      p2b = *itrsx;
      if((*p2a) != (*p2b))
      {
        bRtn = false;
        break;
      }
      else
      {
        ++itrsx;
      }
    }
    if(bRtn)
    {
      nwxXmlUnitClass3 *p3a;
      nwxXmlUnitClass3 *p3b;
      vector<nwxXmlUnitClass3 *>::const_iterator itrv;
      vector<nwxXmlUnitClass3 *>::const_iterator itrvx =
        pVec2->begin();
      for(itrv = pVec1->begin();
        itrv != pVec1->end();
        ++itrv)
      {
        p3a = *itrv;
        p3b = *itrvx;
        if((*p3a) != (*p3b))
        {
          bRtn = false;
          break;
        }
        else
        {
          ++itrvx;
        }
      }
    }
  }
  return bRtn;
}

// xml generated from file with xml2cstr.pl
//
//use strict 'vars';
//
//my $line;
//my $nr = 0;
//my $sNr;
//
//while ($line = <STDIN>)
//{
//  ++$nr;
//  $line =~ s/[\r\n]+$//;
//  $line =~ s/"/\\"/g;
//  $sNr = substr("     " . $nr,-5);
//  print("/* $sNr */  \"${line}\\n\"\n");
//}
//0;


static const char sXML[] =
/*     1 */  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
/*     2 */  "<unit attr1=\"1\" attr2=\"2\">\n"
/*     3 */  "  <string1 name=\"string1\">s1</string1>\n"
/*     4 */  "  <string2 name=\"string2.0\">s2 second</string2>\n"
/*     5 */  "  <number name=\"pi\">3.1415926536</number>\n"
/*     6 */  "  <bool>true</bool>\n"
/*     7 */  "  <time zone=\"EST\">2009-06-23T15:29:21</time>\n"
/*     8 */  "  <set>\n"
/*     9 */  "    <class2 a=\"b\" c=\"D\" e=\"F\" nr=\"1\" type=\"set\">\n"
/*    10 */  "      <c2n>2</c2n>\n"
/*    11 */  "      <c2s>bee</c2s>\n"
/*    12 */  "      <c2b>true</c2b>\n"
/*    13 */  "      <c2d>3.141593</c2d>\n"
/*    14 */  "    </class2>\n"
/*    15 */  "    <class2 a=\"z\" e=\"e\" nr=\"2\" type=\"set\" >\n"
/*    16 */  "      <c2n>4</c2n>\n"
/*    17 */  "      <c2s>cat</c2s>\n"
/*    18 */  "      <c2b>false</c2b>\n"
/*    19 */  "      <c2d>2.718282</c2d>\n"
/*    20 */  "    </class2>\n"
/*    21 */  "    <class2 a=\"c\" c=\"X\" e=\"ZZZZ\" nr=\"3\" type=\"set\">\n"
/*    22 */  "      <c2n>6</c2n>\n"
/*    23 */  "      <c2s>life.1</c2s>\n"
/*    24 */  "      <c2b>true</c2b>\n"
/*    25 */  "      <c2d>42.1</c2d>\n"
/*    26 */  "    </class2>\n"
/*    27 */  "  </set>\n"
/*    28 */  "  <vector>\n"
/*    29 */  "    <class3 name=\"class31\" type=\"vector\">\n"
/*    30 */  "      <x>y</x>\n"
/*    31 */  "      <a>1</a>\n"
/*    32 */  "    </class3>\n"
/*    33 */  "    <class3 name=\"class32\" type=\"vector\">\n"
/*    34 */  "      <x>x</x>\n"
/*    35 */  "      <a>3</a>\n"
/*    36 */  "    </class3>\n"
/*    37 */  "    <class3 name=\"class33\" type=\"vector\">\n"
/*    38 */  "      <x>w</x>\n"
/*    39 */  "      <a>33</a>\n"
/*    40 */  "    </class3>\n"
/*    41 */  "    <class3 name=\"class34\" type=\"vector\">\n"
/*    42 */  "      <x>h</x>\n"
/*    43 */  "      <a>0</a>\n"
/*    44 */  "    </class3>\n"
/*    45 */  "    <class3 name=\"class35\" type=\"vector\">\n"
/*    46 */  "      <x>g</x>\n"
/*    47 */  "      <a>8</a>\n"
/*    48 */  "    </class3>\n"
/*    49 */  "    <class3 name=\"class36\" type=\"vector\">\n"
/*    50 */  "      <x>xxx</x>\n"
/*    51 */  "      <a>1234</a>\n"
/*    52 */  "    </class3>\n"
/*    53 */  "  </vector>\n"
/*    54 */  "</unit>\n"
;
static const size_t XML_SIZE = (sizeof(sXML) / sizeof(sXML[0])) - 1;

bool nwxXmlUnitTest::RunUnitTest()
{
  wxString s("\n\nnwxXmlUnitTest::RunUnitTest() ");
  size_t nErrorLen = s.Len();
  bool bRtn = true;
  try
  {
    nwxXmlUnitTest x1;
    nwxXmlUnitTest x2;
    wxMemoryInputStream instr(sXML,XML_SIZE);
    wxString sFileOut("testout.xml");
    wxString sFileMod("testmod.xml");

    if(!x1.LoadStream(instr))
    {
      s.Append(_T("Cannot load xml stream"));
    }
    else if(!x1.SaveFile(sFileOut))
    {
      s.Append( _T("Cannot save file "));
      s.Append(sFileOut);
    }
    else if(!x2.LoadFile(sFileOut))
    {
      s.Append(_T("Cannot reload file "));
      s.Append(sFileOut);
    }
    else if(x1 != x2)
    {
      s.Append("compare test failed");
    }
    else
    {
      wxXmlDocument xmlDoc(sFileOut);
      if(!xmlDoc.IsOk())
      {
        s.Append(_T("Cannot reload file "));
        s.Append(sFileOut);
      }
      else
      {
        wxXmlNode *pNodeRoot = xmlDoc.GetRoot();
        wxXmlNode *pNode = pNodeRoot->GetChildren(); // string1
        wxXmlNode *pNode2;
        wxXmlNode *pJunk;
        wxXmlNode *pText;
        pNode = pNode->GetNext(); // string2 
        pNode = pNode->GetNext(); // number
        pNode->AddProperty(_T("name"),_T("exp"));
        pText = pNode->GetChildren();
        pText->SetContent(_T("2.7182818"));
        pNode = pNode->GetNext(); // bool
        pNode = pNode->GetNext(); // time
        pNode = pNode->GetNext(); // set
        pNode2 = pNode->GetChildren(); // class2[0]
        pNode2 = pNode2->GetNext(); // class2[1]
        pJunk = pNode2;
        pNode2 = pNode2->GetNext(); // class2[2];
        pNode->RemoveChild(pJunk);
        delete pJunk;
        pNode2->AddProperty(_T("oc"),_T("md"));
        pNode2->DeleteProperty(_T("e"));
#if 1
        // end of mods
        wxFileOutputStream fout(sFileMod);
        if(!fout.IsOk())
        {
          s.Append(_T("Cannot open write modified file "));
          s.Append(sFileMod);
        }
        else if(!xmlDoc.Save(fout,2))
#else
        if(!xmlDoc.Save(sFileMod))
#endif
        {
          s.Append(_T("Cannot save file "));
          s.Append(sFileMod);
        }
      }
    }
  }
  catch(...)
  {
    s.Append(_T(" Exception caught"));
  }
  if(s.Len() != nErrorLen)
  {
    wxASSERT_MSG(0,s);
    bRtn = false;
  }
  return bRtn;
}

#endif
#endif
#endif
