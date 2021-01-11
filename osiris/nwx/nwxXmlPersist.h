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

*  FileName: nwxXmlPersist.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_XML_PERSIST_H__
#define __NWX_XML_PERSIST_H__

#define USING_WX_EXPAT 0
#define USING_XML2_MODS 1

#include <wx/filename.h>
#include <wx/string.h>
#include <wx/colour.h>
#include <wx/datetime.h>
#include <wx/debug.h>
#if USING_WX_EXPAT
#include <wx/xml/xml.h>
#else
#include "wxXml2/wxXml2.h"
#endif

#include <string.h>
#include "nwx/stdb.h"
#include <map>
#include <vector>
#include <set>
#include <memory>
#include "nwx/stde.h"

// #include "nwx/CIncrementer.h"
#include "nwx/vectorptr.h"
#include "nwx/nsstd.h"
#include "nwx/nwxLockFile.h"

typedef vector<wxXmlNode *> nwxXmlNodeList;

class StringTo
{
public:
  static wxColour Color(const wxString &s)
  {
    return wxColour(s);
  }
  static double Double(const wxString &s)
  {
    return atof(s.utf8_str());
  }
  static int Int(const wxString &s)
  {
    return atoi(s.utf8_str());
  }
  static long Long(const wxString &s)
  {
    return atol(s.utf8_str());
  }
  static unsigned int Uint(const wxString &s)
  {
    int n = atoi(s.utf8_str());
    if(n < 0) { n = 0;}
    return (unsigned int)n;
  }
  static bool Bool(const wxString &s)
  {
    const char *p = s.utf8_str();
    const char *pOK = strchr("tT123456789",*p);
    return (pOK != NULL);
  }
private:
  StringTo() {;} // prevent instantiation
};

class ToString
{
public:
  static wxString Color(wxColour &c)
  {
    return c.GetAsString(wxC2S_CSS_SYNTAX);
  }
  static wxString Double(double d)
  {
    return wxString::Format( "%.12g", d );
  }
  static wxString Int(int n)
  {
    return wxString::Format( "%d", n );
  }
  static wxString Long(long n)
  {
    return wxString::Format( "%ld", n );
  }
  static wxString Uint(unsigned int n)
  {
    return wxString::Format( "%u", n);
  }
  static wxString Bool(bool b)
  {
    const wxChar *p = b ? wxS("true") : wxS("false");
    return wxString(p);
  }

  static wxString NodeContents(wxXmlNode *pNode, bool bDeep = false);
private:
  ToString() {;} // prevent instantiation
};

//
//  InwxXmlIO
//
//   Interface for the IO handler for the XML.  It has two functions,
//    1 - Create an XML node list from an object
//    2 - Put data into an object from an XML node
//

class InwxXmlIO
{
public:
  virtual ~InwxXmlIO() {;}
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj) = 0;
  virtual void Init(void *pObj) = 0;
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj) = 0;
  virtual bool Skip(void *)
  {
    return false;
  }
  virtual bool Validate(void *)
  {
    // this is in its infancy and needs some kind of messaging when an error occurs
    return true;
  }
};

class InwxXmlIOstr : public InwxXmlIO
{
public:
  virtual ~InwxXmlIOstr() {;}
  virtual bool LoadFromString(const wxString &s, void *pObj) = 0;
  virtual bool CreateString(wxString *psRtn, void *pObj) = 0;
};


//***************************************************************
//
//  TnwxXmlIO - template to implement InwxXmlIO for a simple type
//    a simple type is a string, int, double, etc.  i.e. a type
//    whose contents can be converted to / from a simple string

template<class T> class TnwxXmlIO : public InwxXmlIOstr
{
public:
  virtual ~TnwxXmlIO<T>() {;}
  virtual bool CreateString(wxString *psStr,void *pObj)
  {
    bool bRtn = !Skip(pObj);
    if(bRtn)
    {
      (*psStr) = ToString(pObj);
    }
    return bRtn;
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj)
  {
    wxString sValue;
    nwxXmlNodeList *pRtn = new nwxXmlNodeList;
    if(CreateString(&sValue,pObj))
    {
      wxXmlNode *pNode = new wxXmlNode(
        wxXML_ELEMENT_NODE,sNodeName);
      pNode->AddChild(
        new wxXmlNode(
          wxXML_TEXT_NODE,wxEmptyString,sValue
        ));
      pRtn->reserve(1);
      pRtn->push_back(pNode);
    }
    return pRtn;
  }
  virtual bool LoadFromString(const wxString &s, void *pObj)
  {
    T &x(CAST(pObj));
    x = FromString(s);
    return true;
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    wxString s = ToString::NodeContents(pNode);
    return LoadFromString(s,pObj);
  }
  static T &CAST(void *pObj)
  {
    return *((T *)pObj);
  }
  virtual wxString ToString(void *pObj) = 0;
  virtual T FromString(const wxString &s) = 0;
  virtual void SetDefault(const T &x)
  {
    m_DEFAULT = x;
  }
  virtual const T &GetDefault()
  {
    return m_DEFAULT;
  }
  virtual void Init(void *pObj)
  {
    T &x(CAST(pObj));
    x = m_DEFAULT;
  }

protected:
  T m_DEFAULT;
};

//  Implementations for TnwxXmlIO for many simple types

class nwxXmlIOint : public TnwxXmlIO<int>
{
public:
  nwxXmlIOint() : m_bNonZero(false)
  {
    m_DEFAULT = 0;
  }
  nwxXmlIOint(bool bNonZero) : m_bNonZero(bNonZero)
  {
    m_DEFAULT = 0;
  }
  nwxXmlIOint(int nDefault, bool bNonZero = false) : m_bNonZero(bNonZero)
  {
    m_DEFAULT = nDefault;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Int( CAST(pObj) );
  }
  virtual int FromString(const wxString &s)
  {
    return StringTo::Int(s);
  }
  void SetNonZero(bool bNonZero) { m_bNonZero = bNonZero; }
  bool IsNonZero() { return m_bNonZero;}
  virtual bool Skip(void *pObj)
  {
    bool bRtn = false;
    if(m_bNonZero)
    {
      int &x(CAST(pObj));
      bRtn = !x;
    }
    return bRtn;
  };
private:
  bool m_bNonZero;
};

class nwxXmlIOintPositive : public nwxXmlIOint
{
public:
  nwxXmlIOintPositive() : 
      nwxXmlIOint(-1,true) {}
  nwxXmlIOintPositive(bool bNonZero) : 
    nwxXmlIOint(-1,bNonZero) {}
  virtual ~nwxXmlIOintPositive(){}
  virtual bool Skip(void *pObj)
  {
    int &x(CAST(pObj));
    bool bRtn = IsNonZero() ? (x <= 0) : (x < 0);
    return bRtn;
  }
};


class nwxXmlIOlong : public TnwxXmlIO<long>
{
public:
  nwxXmlIOlong()
  {
    m_DEFAULT = 0;
  }
  nwxXmlIOlong(long n)
  {
    m_DEFAULT = n;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Long( CAST(pObj) );
  }
  virtual long FromString(const wxString &s)
  {
    return StringTo::Long(s);
  }
};

class nwxXmlIOuint : public TnwxXmlIO<unsigned int>
{
public:
  nwxXmlIOuint()
  {
    m_DEFAULT = 0;
  }
  nwxXmlIOuint(unsigned int n)
  {
    m_DEFAULT = n;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Uint( CAST(pObj) );
  }
  virtual unsigned int FromString(const wxString &s)
  {
    return StringTo::Uint(s);
  }
};


class nwxXmlIOdouble : public TnwxXmlIO<double>
{
public:
  nwxXmlIOdouble()
  {
    m_DEFAULT = 0.0;
  }
  nwxXmlIOdouble(double d)
  {
    m_DEFAULT = d;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Double( CAST(pObj) );
  }
  virtual double FromString(const wxString &s)
  {
    return StringTo::Double(s);
  }
};

class nwxXmlIOdoublePositive : public nwxXmlIOdouble
{
public:
  nwxXmlIOdoublePositive(double DEFAULT, bool bNonZero) :
    nwxXmlIOdouble(DEFAULT),
    m_bNonZero(bNonZero)
  {}

  nwxXmlIOdoublePositive(bool bNonZero = false) : 
    nwxXmlIOdouble(-1.0),
    m_bNonZero(bNonZero)
  {}

  virtual bool Skip(void *pObj)
  {
    double &x(CAST(pObj));
    bool bRtn = m_bNonZero ? (x <= 0.0) : (x < 0.0);
    return bRtn;
  };
private:
  bool m_bNonZero;
};

class nwxXmlIOdoubleFmt : public nwxXmlIOdouble
{
public:
  nwxXmlIOdoubleFmt() : m_sFormat(".12g")
  {
    ClearRange();
  }
  nwxXmlIOdoubleFmt(const wxChar *psFormat) : m_sFormat(psFormat)
  {
    ClearRange();
  }
  nwxXmlIOdoubleFmt(int nDigits)
  {
    SetDigits(nDigits);
    ClearRange();
  }
  virtual wxString ToString(void *pObj)
  {
    double &x(CAST(pObj));
    return wxString::Format(m_sFormat,x);
  }
  void SetFormat(const wxChar *psFormat = wxS(".12g"))
  {
    m_sFormat = psFormat;
  }
  void SetFormat(const wxString &sFormat)
  {
    m_sFormat = sFormat;
  }
  const wxString &GetFormat() const
  {
    return m_sFormat;
  }
  void SetDigits(int nDigits)
  {
    m_sFormat = "%.";
    m_sFormat.Append(ToString::Int(nDigits));
    m_sFormat.Append("f");
  }
  void SetRangeLow(double d)
  {
    m_dLo = d;
    m_bLo = true;
  }
  void SetRangeHi(double d)
  {
    m_dHi = d;
    m_bHi = true;
  }
  void SetRange(double dLo, double dHi)
  {
    SetRangeLow(dLo);
    SetRangeHi(dHi);
  }
  void ClearRange()
  {
    m_dLo = 0.0;
    m_dHi = 0.0;
    m_bLo = false;
    m_bHi = false;
  }
  virtual bool Validate(void *pObj)
  {
    double &x(CAST(pObj));
    bool bRtn = true;
    if(m_bLo && (x < m_dLo))
    {
      bRtn = false;
    }
    else if(m_bHi && (x > m_dHi))
    {
      bRtn = false;
    }
    return bRtn;
  }
private:

  wxString m_sFormat;
  double m_dLo;
  double m_dHi;
  bool m_bLo;
  bool m_bHi;
};

class nwxXmlIOwxString : public TnwxXmlIO<wxString>
{
public:
  nwxXmlIOwxString() : m_bNotEmpty(false)
  {
    InitLen();
  }
  nwxXmlIOwxString(bool bNotEmpty) : m_bNotEmpty(bNotEmpty)
  {
    InitLen();
  }
  virtual wxString ToString(void *pObj)
  {
    return CAST(pObj);
  }
  virtual wxString FromString(const wxString &s)
  {
    return s;
  }
  virtual bool Skip(void *pObj)
  {
    bool bRtn = false;
    if(m_bNotEmpty)
    {
      wxString &x(CAST(pObj));
      bRtn = x.IsEmpty();
    }
    return bRtn;
  }
  bool IsNotEmpty() const
  {
    return m_bNotEmpty;
  }
  void SetNotEmpty(bool b)
  {
    m_bNotEmpty = b;
  }
  virtual bool Validate(void *pObj)
  {
    wxString &x(CAST(pObj));
    bool bRtn = true;
    size_t nLen = x.Len();
    if(m_bNotEmpty && !nLen)
    {} // empty and OK
    else if(nLen < m_nMinLen)
    {
      bRtn = false;
    }
    else if(nLen > m_nMaxLen)
    {
      bRtn = false;
    }
    return bRtn;
  }
  void SetMinLength(size_t n)
  {
    m_nMinLen = n;
  }
  void SetMaxLength(size_t n)
  {
    m_nMaxLen = n;
  }
  void SetLength(size_t nLo, size_t nHi)
  {
    m_nMinLen = nLo;
    m_nMaxLen = nHi;
  }
  size_t GetMinLength() const
  {
    return m_nMinLen;
  }
  size_t GetMaxLength() const
  {
    return m_nMaxLen;
  }
private:
  void InitLen()
  {
    m_nMinLen = 0;
    m_nMaxLen = 0x7fffffff;
  }
  wxString m_sDefault;
  bool m_bNotEmpty;
  size_t m_nMinLen;
  size_t m_nMaxLen;
};



class nwxXmlIOwxDateTime : public TnwxXmlIO<wxDateTime>
{
public:
  nwxXmlIOwxDateTime() :
      m_bXML(false), m_bSkipIfZero(false)
  {
    m_DEFAULT = (time_t) 0;
  }
  nwxXmlIOwxDateTime(bool bXMLstyle, bool bSkipIfZero = false) :
      m_bXML(bXMLstyle), m_bSkipIfZero(bSkipIfZero)
  {
    m_DEFAULT = (time_t) 0;
  }

  virtual wxString ToString(void *pObj);
  virtual wxDateTime FromString(const wxString &s);
  bool IsXML()
  {
    return m_bXML;
  }
  void SetXML(bool b)
  {
    m_bXML = b;
  }
  bool SkipIfZero()
  {
    return m_bSkipIfZero;
  }
  void SetSkipIfZero(bool b)
  {
    m_bSkipIfZero = b;
  }
  virtual bool Skip(void *pObj);
#ifdef _DEBUG
  static void UnitTest();
#endif

private:
  bool m_bXML;
  bool m_bSkipIfZero;
};



class nwxXmlIOwxColour : public TnwxXmlIO<wxColour>
{
public:
  nwxXmlIOwxColour() : m_bSkipDefault(false)
  {
    _InitDefault();
  }
  nwxXmlIOwxColour(bool bSkipDefault) : m_bSkipDefault(bSkipDefault)
  {
    _InitDefault();
  }
  nwxXmlIOwxColour(const wxColour &cDEF, bool bSkipDefault) : m_bSkipDefault(bSkipDefault)
  {
    m_DEFAULT = cDEF;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Color(CAST(pObj));
  }
  virtual wxColour FromString(const wxString &s)
  {
    wxColour c(s);
    return c;
  }
  void SetSkipDefault(bool b = true)
  {
    m_bSkipDefault = b;
  }
  bool GetSkipDefault() const 
  {
    return m_bSkipDefault;
  }
  virtual bool Skip(void *pOBJ)
  {
    bool bRtn = GetSkipDefault() && (CAST(pOBJ) == m_DEFAULT);
    return bRtn;
  }
protected:
  void _InitDefault()
  {
    m_DEFAULT.Set(0,0,0);
  }
  bool m_bSkipDefault;
};

class nwxXmlIObool : public TnwxXmlIO<bool>
{
public:
  nwxXmlIObool()
  {
    m_DEFAULT = false;
  }
  virtual wxString ToString(void *pObj)
  {
    return ToString::Bool(CAST(pObj));
  }
  virtual bool FromString(const wxString &s)
  {
    return StringTo::Bool(s);
  }
};

class nwxXmlIOboolSkipFalse : public nwxXmlIObool
{
public:
  virtual bool Skip(void *pObj)
  {
    bool &x(CAST(pObj));
    return !x;
  }
};

class nwxXmlIOboolDefaultTrue : public nwxXmlIObool
{
public:
  nwxXmlIOboolDefaultTrue()
  {
    m_DEFAULT = true;
  }
};
class nwxXmlIOboolDefaultTrueSkip : public nwxXmlIObool
{
public:
  nwxXmlIOboolDefaultTrueSkip()
  {
    m_DEFAULT = true;
  }
  virtual bool Skip(void *pObj)
  {
    bool x(CAST(pObj));
    return x;
  }
};

//  TnwxXmlIOvector - used for a list of objects of a simple type
//    where the list can be represented by a simple string separated by
//    a whitespace or comma
//
//   The simple type cannot be a string if the string can contain
//       the actual separator.  This is used primarily for a vector of
//       numbers

template<class T> class TnwxXmlIOvector : public InwxXmlIOstr
{
public:
  virtual ~TnwxXmlIOvector<T>(){;}
  virtual wxString ToStringElem(void *pObj) = 0;
  virtual T FromStringElem(const wxString &s) = 0;

  virtual bool LoadFromString(const wxString &s, void *pObj)
  {
    vector<T> &vList(VCAST(pObj));
    wxString sNode(s);
    wxString str;
    //bool bRtn = false; //OS-566, always return true

    //
    //  change all separators into spaces
    //
    sNode.Replace(","," ");
    sNode.Replace("\r"," ");
    sNode.Replace("\n"," ");
    sNode.Replace("\t"," ");
    sNode.Trim(true);
    sNode.Trim(false);

    //  change all multiple spaces into single spaces

    while(sNode.Replace("  "," ")) {;}

    //  count spaces and add 1

    size_t nCount = sNode.Freq(wxChar(' '));
    nCount++;
    Init(pObj);
    const char *ps = sNode.utf8_str();
    if(*ps)
    {
      vList.reserve(nCount);
      const char *ps1;
      const char *ps0 = ps;
      for(ps1 = strchr(ps0,' ');
        ps1 != NULL;
        ps1 = strchr(ps0,' '))
      {
        str = sNode.Mid(ps0 - ps,ps1 - ps0);
        vList.push_back(FromStringElem(str));
        ps0 = ps1;
        ps0++;
        // bRtn = true; //OS-566, always return true
      }
      if(*ps0)
      {
        str = sNode.Mid(ps0 - ps);
        vList.push_back(FromStringElem(str));
        //bRtn = true; //OS-566, always return true
      }
    }
    //return bRtn;
    return true; //OS-566, always return true
  }
  virtual bool CreateString(wxString *psRtn, void *pObj)
  {
    bool bRtn = !Skip(pObj);
    if(bRtn)
    {
      wxString sRtn;
      vector<T> &vList(VCAST(pObj));
      size_t n = vList.size();
      sRtn.Alloc(n << 3);
      for(size_t i = 0; i < n; i++)
      {
        if(sRtn.Len())
        {
          sRtn.Append(" ");
        }
        T &x(vList.at(i));
        sRtn.Append(ToStringElem((void *)&x));
      }
      *psRtn = sRtn;
    }
    return bRtn;
  }

  // InwxXmlIO methods

  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj)
  {
    wxString sValue;
    nwxXmlNodeList *pRtn = new nwxXmlNodeList;
    if(CreateString(&sValue,pObj))
    {
      wxXmlNode *pNode = new wxXmlNode(
        wxXML_ELEMENT_NODE,sNodeName);
      pNode->AddChild(
        new wxXmlNode(
          wxXML_TEXT_NODE,wxEmptyString,sValue
        ));
      pRtn->reserve(1);
      pRtn->push_back(pNode);
    }
    return pRtn;
  }
  virtual void Init(void *pObj)
  {
    VCAST(pObj).clear();
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    wxString s = ToString::NodeContents(pNode);
    return LoadFromString(s,pObj);
  }

  static T &CAST(void *pObj)
  {
    return *((T *)pObj);
  }
  static vector<T> &VCAST(void *pObj)
  {
    return *((vector<T> *)pObj);
  }
};

//   Implementations for TnwxXmlIOvector for numbers

class nwxXmlIOvectorInt : public TnwxXmlIOvector<int>
{
public:
  virtual int FromStringElem(const wxString &s)
  {
    return StringTo::Int(s);
  }
  virtual wxString ToStringElem(void *pObj)
  {
    return ToString::Int(CAST(pObj));
  }

};

class nwxXmlIOvectorUint: public TnwxXmlIOvector<unsigned int>
{
public:
  virtual unsigned int FromStringElem(const wxString &s)
  {
    return StringTo::Uint(s);
  }
  virtual wxString ToStringElem(void *pObj)
  {
    return ToString::Uint(CAST(pObj));
  }

};

class nwxXmlIOvectorDouble : public TnwxXmlIOvector<double>
{
public:
  virtual double FromStringElem(const wxString &s)
  {
    return StringTo::Double(s);
  }
  virtual wxString ToStringElem(void *pObj)
  {
    return ToString::Double(CAST(pObj));
  }

};

//***************************************************** nwxXmlContainer
//
//    nwxXmlContainer - used for a hash table to map the name
//      of an XML element to its InwxXmlIO handler and a pointer
//      to the object itself.  This is used by primarily by
//      "class nwxXmlPersist" defined below

class nwxXmlContainer
{
private:

  template<class IO> class T_PERSIST
  {
    // PERSIST, structure containing a tag name, IO handler, and a pointer to an object
  public:
    T_PERSIST()
    {
      Setup(NULL,NULL);
    }
    T_PERSIST(IO *_pPersist, void *_pObj)
    {
      Setup(_pPersist,_pObj);
    }
    T_PERSIST(const wxString &_sName, IO *_pPersist, void *_pObj) :
        sName(_sName)
    {
      Setup(_pPersist,_pObj);
    }
    void Setup(IO *_pPersist, void *_pObj)
    {
      pPersist = _pPersist;
      pObj = _pObj;
    }
    void Init()
    {
      pPersist->Init(pObj);
    }
    const wxString sName;
    IO *pPersist;
    void *pObj;
  };
  typedef T_PERSIST<InwxXmlIO> PERSIST;
  typedef T_PERSIST<InwxXmlIOstr> PERSISTstr;

  typedef map<const wxString, PERSIST *> mapPERSIST;
  typedef map<const wxString, PERSISTstr *> mapPERSISTstr;

  template<class IO> class TregisterNode
  {
  private:
    TregisterNode<IO>() {;} // private, prevent instantiation
  public:
    typedef map<const wxString, T_PERSIST<IO> * > TMAP_PERSIST;
    typedef vector< T_PERSIST<IO> * > TV_PERSIST;
    static bool Register(
      const wxString &sName,
      IO *pPersist,
      void *pObj,
      TMAP_PERSIST *pMap,
      TV_PERSIST *pList)
    {
      typename TMAP_PERSIST::iterator itr = pMap->find(sName);
      if(itr == pMap->end())
      {
        T_PERSIST<IO> *per = new T_PERSIST<IO>(sName,pPersist,pObj);
        pList->push_back(per);
        pMap->insert(typename TMAP_PERSIST::value_type(sName,per));
      }
      else
      {
        itr->second->Setup(pPersist,pObj);
      }
      return true;
    }
  };
public:
  nwxXmlContainer() : m_pPersistText(NULL)
  {
    m_nInitRecursion = 0;
    m_bNoInit = false;
  }
  nwxXmlContainer(const nwxXmlContainer &x)  : m_pPersistText(NULL)
  {
    m_nInitRecursion = 0;
    m_bNoInit = false;
    (*this) = x; // operator doesn't copy anything
  }
  virtual ~nwxXmlContainer()
  {
    wxASSERT_MSG(!m_nInitRecursion,"m_nInitRecursion is not 0");
    Clear();
  }
  /*
  CIncrementer *CreateInitIncrementer()
  {
    return new CIncrementer(m_nInitRecursion);
  }
  */
  nwxXmlContainer &operator = (const nwxXmlContainer &)
  {
    // don't copy anything
    // otherwise you will crash in the destructor
    // because pointers will be copied
    return *this;
  }
  bool Register(
        const wxString &sName, InwxXmlIO *pPersist, void *pObj);
  bool RegisterAttribute(
        const wxString &sName, InwxXmlIOstr *pPersist, void *pObj);

  bool Register(const wxString &sName, InwxXmlIO *pPersist)
  {
    return Register(sName, pPersist, (void *)pPersist);
  }
  bool RegisterAttribute(const wxString &sName, InwxXmlIOstr *pPersist)
  {
    return RegisterAttribute(sName, pPersist, (void *)pPersist);
  }
  bool RegisterText(InwxXmlIOstr *pIO, void *pObj)
  {
    if(m_pPersistText == NULL)
    {
      m_pPersistText = new PERSISTstr;
    }
    m_pPersistText->Setup(pIO,pObj);
    return true;
  }
  bool RegisterText(InwxXmlIOstr *pPersist)
  {
    return RegisterText(pPersist, (void *) pPersist);
  }

  void ClearText()
  {
    if(m_pPersistText != NULL)
    {
      delete m_pPersistText;
      m_pPersistText = NULL;
    }
  }
  void Clear()
  {
    vectorptr<PERSIST>::cleanup(&m_list);
    vectorptr<PERSISTstr>::cleanup(&m_listAttr);
    m_map.clear();
    m_mapAttr.clear();
    ClearText();
  }

  virtual bool Load(wxXmlNode *pNode);
  virtual nwxXmlNodeList *CreateNodeList();
  virtual void Init();
#if USING_XML2_MODS
  int SetupProperties(wxXmlNode *pNode);
#else
  wxXmlProperty *CreateProperties(int *pnCount = NULL);
  static void ClearProperties(wxXmlProperty *pProp);
#endif
  static void ClearAttributes(wxXmlNode *pNode);
  virtual int SetAttributes(wxXmlNode *pNode)
  {
    int nCount;
    ClearAttributes(pNode);
#if USING_XML2_MODS
    nCount = SetupProperties(pNode);
#else
    wxXmlProperty *pProp = CreateProperties(&nCount);
    if(nCount)
    {
      pNode->SetProperties(pProp);
    }
#endif
    return nCount;
  }
  void SetNoInit(bool b = true)
  {
    m_bNoInit = b;
  }
  bool GetNoInit()
  {
    return m_bNoInit;
  }
private:
  int m_nInitRecursion;
  bool m_bNoInit;
  mapPERSIST m_map;
  mapPERSISTstr m_mapAttr;
  vector<PERSIST *> m_list;
  vector<PERSISTstr *> m_listAttr;
  PERSISTstr *m_pPersistText;
};
//***************************************************** nwxXmlPersist
//
//
//   nwxXmlPersist - InwxXmlIO for a object heirarchy that can be represented
//     by an XML tree where each node can contain one or more child nodes
//     where each node is an element or a string
//
//   12/30/08 attribute support is being implemented
//   12/31/08 Top level node will support string data but string nodes
//    containing only whitespace will be ignored.  All string data
//    with the same parent will be concatenated before it is processed.
//    as of 12/31/08 this is being designed but not implemented
//
//   This class is ususally inherited by another class that will
//   contain data and can therefore manage its own XML persistence
//
//   Usage:  create a subclass to inherit nwxXmlPersist,
//     override virtual RegisterAll(bool)
//     to register the persistent data in the class.  That's it.
//
//    Valid example, no string data at the top level
//
//    <top>
//     <level_1>
//         <age>3</age>
//         <name>Fred</name>
//     </level_1>
//     <level_2>Boy</level_2>
//   </top>
//
//   Example with string data at the top level, but not recommended
//
//    <top>
//     <level_1>
//         <age>3</age>
//         <name>Fred</name>
//     </level_1>
//     Boy
//   </top>
//

class nwxXmlPersist : public InwxXmlIO
{
public:
  nwxXmlPersist() : 
      m_dtFileModTime((time_t) 0), 
      m_pLock(NULL), 
      m_bReadOnly(false)
  {;}
  nwxXmlPersist(bool bReadOnly) : 
      m_dtFileModTime((time_t) 0), 
      m_pLock(NULL),
      m_bReadOnly(bReadOnly)
  {;}
  virtual ~nwxXmlPersist() 
  {
    if(m_pLock != NULL)
    {
      delete m_pLock;
      m_pLock = NULL;
    }
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj);
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName);
  virtual bool LoadFromNode(wxXmlNode *, void *pObj);
  virtual bool LoadFromNode(wxXmlNode *);
  void SetNoInit(bool b = true)
  {
    m_xml.SetNoInit(b);
  }
  bool GetNoInit()
  {
    return m_xml.GetNoInit();
  }
  void SetReadOnly(bool bReadOnly)
  {
    m_bReadOnly = bReadOnly;
  }
  bool IsReadOnly() const
  {
    return m_bReadOnly;
  }
  virtual void Init()
  {
    m_xml.Init();
  }
  virtual void Init(void *p)
  {
    if(p == (void *)this)
    {
//      this->Init();  // could be a problem
      m_xml.Init();
    }
  }
  virtual bool LoadStream(wxInputStream &stream)
  {
    auto_ptr<wxXmlDocument> apDoc(new wxXmlDocument);
    bool bRtn = apDoc->Load(stream);
    if(bRtn)
    {
      bRtn = LoadFromNode(apDoc->GetRoot());
    }
    _SetFileName(wxEmptyString);
    return bRtn;
  }
  virtual bool LoadFile(const wxString &sFileName, bool bLock, int nLockWait = 0)
  {
    //  discard this and following lines on 6/5/2018
    // nwxLockFile Lock;
    auto_ptr<wxXmlDocument> apDoc(new wxXmlDocument);
    if(m_pLock == NULL)
    {
      m_pLock = new nwxLockFile();
    }
    bool bExist = wxFileName::FileExists(sFileName);
    bool bRtn = 
      (bLock ? m_pLock->LockWait(sFileName,nLockWait) : true) &&
      // OS-357, no need to wait for lock, will check reload later.
      //  discard this and following lines on 6/5/2018
      // (m_pLock->HasLock(sFileName) || m_pLock->WaitUntilUnlocked(sFileName,3)) &&
      (bExist ? apDoc->Load(sFileName) : true);
    m_dtFileModTime.Set((time_t)0);
    if(!bRtn) {}
    else if(bExist)
    {
      bRtn = LoadFromNode(apDoc->GetRoot());
    }
    else
    {
      Init();
    }
    if(bRtn)
    {
      _SetFileName(sFileName);
    }
    else
    {
      _SetFileName(wxEmptyString);
    }

    return bRtn;
  }
  virtual bool LoadFile(const wxString &sFileName)
  {
    return LoadFile(sFileName,false);
  }
  virtual bool ReloadFile()
  {
    bool bRtn = false;
    if(!m_sLastFileName.IsEmpty())
    {
      Init();
      bRtn = LoadFile(m_sLastFileName);
    }
    return bRtn;
  }
  virtual bool CheckFileModification(bool bReload = false);
  virtual bool FileExists();
  bool UpdateLock()
  {
    bool bRtn = false;
    if(m_pLock != NULL)
    {
      m_pLock->UpdateAll();
      bRtn = m_pLock->HasLock(m_sLastFileName);
    }
    return bRtn;
  }
  bool Lock(int nWait = 0)
  {
    bool bRtn = false;
    if(!m_sLastFileName.IsEmpty())
    {
      if(m_pLock == NULL)
      {
        m_pLock = new nwxLockFile();
      }
      bRtn = m_pLock->LockWait(m_sLastFileName,nWait);
    }
    return bRtn;
  }
  bool IsLocked()
  {
    bool bRtn = (m_pLock != NULL)
      ?  m_pLock->IsLocked(m_sLastFileName)
    : nwxLockFile::LockFileExists(m_sLastFileName);
  // if m_pLock locked the file, then the file time will be updated
  //  to reset the lock timeout
    return bRtn;
  }
  bool Unlock()
  {
    bool bRtn = true;
    if( (!m_sLastFileName.IsEmpty()) &&
        (m_pLock != NULL) && 
        m_pLock->HasLock(m_sLastFileName) )
    {
      bRtn = m_pLock->Unlock(m_sLastFileName);
    }
    return bRtn;
  }
  virtual bool SaveFile(const wxString &sFileName)
  {
    bool bRtn = false;
    if(!m_bReadOnly)
    {
      auto_ptr<nwxXmlNodeList> apNode(
        CreateNodeList(RootNode(),(void *)this) );
      if(apNode->size() == 1)
      {
        bool bKeepLock = false;
        if(m_pLock == NULL)
        {
          m_pLock = new nwxLockFile();
        }
        else
        {
          bKeepLock = m_pLock->HasLock(sFileName);
        }
        auto_ptr<wxXmlDocument> apDoc(new wxXmlDocument);
        apDoc->SetRoot(apNode->at(0));
        bRtn = 
          (bKeepLock || m_pLock->LockWait(sFileName,3)) &&
          apDoc->Save(sFileName,1);
        if(!bKeepLock)
        {
          m_pLock->Unlock(sFileName);
        }
      }
      else
      {
        // FOR DEBUGGING
        bRtn = false;
        vectorptr<wxXmlNode>::cleanup(apNode.get());
      }
      if(bRtn)
      {
        _SetFileName(sFileName);
      }
      else
      {
        _SetFileName(wxEmptyString);
      }
    }
    return bRtn;
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sRootNode;
  }
  bool LastFileExists() const
  {
    bool b = m_sLastFileName.IsEmpty() ? false
      : wxFileExists(m_sLastFileName);
    return b;
  }

  const wxString &GetLastFileName() const
  {
    return m_sLastFileName;
  }
  const wxString &GetComment() const
  {
    return m_sComment;
  }
  void SetComment(const wxString &s);
  const wxDateTime &GetFileModTime() const
  {
    return m_dtFileModTime;
  }

protected:
  virtual void ClearRegistration()
  {
    m_xml.Clear();
  }
  virtual void RegisterAll(bool = false) {;}
  // parameter, true if in constructor, false otherwise

  bool Register(nwxXmlPersist *p)
  {
    return m_xml.Register(p->RootNode(),p);
  }

  bool Register(const wxString &sName, InwxXmlIO *pPersist, void *pObj)
  {
    return m_xml.Register(sName,pPersist,pObj);
  }
  bool Register(const wxString &sName, InwxXmlIO *pPersist)
  {
    return m_xml.Register(sName, pPersist);
  }
  bool RegisterUint(const wxString &sName, unsigned int *pn)
  {
    return Register(sName,&g_IOuint,(void *)pn);
  }
  bool RegisterInt(const wxString &sName, int *pn)
  {
    return Register(sName,&g_IOint,(void *)pn);
  }
  bool RegisterIntPositive(const wxString &sName, int *pn)
  {
    return Register(sName,&g_IOintPositive,(void *)pn);
  }
  bool RegisterIntNonZero(const wxString &sName, int *pn)
  {
    return Register(sName,&g_IOintNonZero,(void *)pn);
  }
  bool RegisterLong(const wxString &sName, long *pn)
  {
    return Register(sName,&g_IOlong,(void *)pn);
  }
  bool RegisterDouble(const wxString &sName, double *pd)
  {
    return Register(sName,&g_IOdouble,(void *)pd);
  }
  bool RegisterDoubleM1(const wxString &sName, double *pd)
  {
    return Register(sName,&g_IOdoubleM1,(void *)pd);
  }
  bool RegisterWxColour(const wxString &sName, wxColour *p)
  {
    return Register(sName,&g_IOwxColour,(void *)p);
  }
  bool RegisterWxDateTime(const wxString &sName, wxDateTime *p)
  {
    return Register(sName,&g_IOwxDateTime,(void *)p);
  }
  bool RegisterWxDateTimeNonZero(const wxString &sName, wxDateTime *p)
  {
    return Register(sName,&g_IOwxDateTimeNonZero,(void *)p);
  }
  bool RegisterWxDateTimeXML(const wxString &sName, wxDateTime *p)
  {
    return Register(sName,&g_IOwxDateTimeXML,(void *)p);
  }
  bool RegisterBool(const wxString &sName, bool *pb)
  {
    return Register(sName,&g_IObool,(void *)pb);
  }
  bool RegisterBoolSkipFalse(const wxString &sName, bool *pb)
  {
    return Register(sName,&g_IOboolSkipFalse,(void *)pb);
  }
  bool RegisterBoolTrue(const wxString &sName, bool *pb)
  {
    return Register(sName,&g_IOboolTrue,(void *)pb);
  }
  bool RegisterBoolTrueSkip(const wxString &sName, bool *pb)
  {
    return Register(sName,&g_IOboolTrueSkip,(void *)pb);
  }
  bool RegisterWxString(const wxString &sName, wxString *ps)
  {
    return Register(sName,&g_IOwxString,(void *)ps);
  }
  bool RegisterWxStringNotEmpty(const wxString &sName, wxString *ps)
  {
    return Register(sName,&g_IOwxStringNotEmpty,(void *)ps);
  }
  bool RegisterUintVector(const wxString &sName, vector<unsigned int> *pn)
  {
    return Register(sName,&g_IOvectorUint,(void *)pn);
  }
  bool RegisterIntVector(const wxString &sName, vector<int> *pn)
  {
    return Register(sName,&g_IOvectorInt,(void *)pn);
  }
  bool RegisterDoubleVector(const wxString &sName, vector<double> *pd)
  {
    return Register(sName,&g_IOvectorDouble,(void *)pd);
  }


  // Attributes

  bool RegisterAttribute(const wxString &sName, InwxXmlIOstr *pPersist, void *pObj)
  {
    return m_xml.RegisterAttribute(sName,pPersist,pObj);
  }
  bool RegisterAttribute(const wxString &sName, InwxXmlIOstr *pPersist)
  {
    return m_xml.RegisterAttribute(sName, pPersist);
  }
  bool RegisterUintAttribute(const wxString &sName, unsigned int *pn)
  {
    return RegisterAttribute(sName,&g_IOuint,(void *)pn);
  }
  bool RegisterIntAttribute(const wxString &sName, int *pn)
  {
    return RegisterAttribute(sName,&g_IOint,(void *)pn);
  }
  bool RegisterIntNonZeroAttribute(const wxString &sName, int *pn)
  {
    return RegisterAttribute(sName,&g_IOintNonZero,(void *)pn);
  }
  bool RegisterLongAttribute(const wxString &sName, long *pn)
  {
    return RegisterAttribute(sName,&g_IOlong,(void *)pn);
  }
  bool RegisterDoubleAttribute(const wxString &sName, double *pd)
  {
    return RegisterAttribute(sName,&g_IOdouble,(void *)pd);
  }
  bool RegisterWxColourAttribute(const wxString &sName, wxColour *p)
  {
    return RegisterAttribute(sName,&g_IOwxColour,(void *)p);
  }
  bool RegisterWxDateTimeAttribute(const wxString &sName, wxDateTime *p)
  {
    return RegisterAttribute(sName,&g_IOwxDateTime,(void *)p);
  }
  bool RegisterWxDateTimeAttributeNonZero(const wxString &sName, wxDateTime *p)
  {
    return RegisterAttribute(sName,&g_IOwxDateTimeNonZero,(void *)p);
  }
  bool RegisterWxDateTimeXMLAttribute(const wxString &sName, wxDateTime *p)
  {
    return RegisterAttribute(sName,&g_IOwxDateTimeXML,(void *)p);
  }
  bool RegisterBoolAttribute(const wxString &sName, bool *pb)
  {
    return RegisterAttribute(sName,&g_IObool,(void *)pb);
  }
  bool RegisterBoolSkipFalseAttribute(const wxString &sName, bool *pb)
  {
    return RegisterAttribute(sName,&g_IOboolSkipFalse,(void *)pb);
  }
  bool RegisterBoolTrueAttribute(const wxString &sName, bool *pb)
  {
    return RegisterAttribute(sName,&g_IOboolTrue,(void *)pb);
  }
  bool RegisterWxStringAttribute(const wxString &sName, wxString *ps)
  {
    return RegisterAttribute(sName,&g_IOwxString,(void *)ps);
  }
  bool RegisterWxStringNotEmptyAttribute(const wxString &sName, wxString *ps)
  {
    return RegisterAttribute(sName,&g_IOwxStringNotEmpty,(void *)ps);
  }

  // Text


  bool RegisterText(InwxXmlIOstr *pPersist, void *pObj)
  {
    return m_xml.RegisterText(pPersist,pObj);
  }
  bool RegisterText(InwxXmlIOstr *pPersist)
  {
    return m_xml.RegisterText(pPersist);
  }
  bool RegisterUintText(unsigned int *pn)
  {
    return RegisterText(&g_IOuint,(void *)pn);
  }
  bool RegisterIntText(int *pn)
  {
    return RegisterText(&g_IOint,(void *)pn);
  }
  bool RegisterIntNonZeroText(int *pn)
  {
    return RegisterText(&g_IOintNonZero,(void *)pn);
  }
  bool RegisterLongText(long *pn)
  {
    return RegisterText(&g_IOlong,(void *)pn);
  }
  bool RegisterDoubleText(double *pd)
  {
    return RegisterText(&g_IOdouble,(void *)pd);
  }
  bool RegisterWxColourText(wxColour *p)
  {
    return RegisterText(&g_IOwxColour,(void *)p);
  }
  bool RegisterWxDateTimeText(wxDateTime *p)
  {
    return RegisterText(&g_IOwxDateTime,(void *)p);
  }
  bool RegisterWxDateTimeXMLText(wxDateTime *p)
  {
    return RegisterText(&g_IOwxDateTimeXML,(void *)p);
  }
  bool RegisterBoolText(bool *pb)
  {
    return RegisterText(&g_IObool,(void *)pb);
  }
  bool RegisterBoolSkipFalseText(bool *pb)
  {
    return RegisterText(&g_IOboolSkipFalse,(void *)pb);
  }
  bool RegisterBoolTrueText(bool *pb)
  {
    return RegisterText(&g_IOboolTrue,(void *)pb);
  }
  bool RegisterWxStringText(wxString *ps)
  {
    return RegisterText(&g_IOwxString,(void *)ps);
  }
  bool RegisterWxStringNotEmptyText(wxString *ps)
  {
    return RegisterText(&g_IOwxStringNotEmpty,(void *)ps);
  }

  wxString m_sComment;
  wxString m_sLastFileName;
  nwxXmlContainer m_xml;
  static nwxXmlIOint g_IOint;
  static nwxXmlIOint g_IOintNonZero;
  static nwxXmlIOintPositive g_IOintPositive;
  static nwxXmlIOlong g_IOlong;
  static nwxXmlIOuint g_IOuint;
  static nwxXmlIOdouble g_IOdouble;
  static nwxXmlIOdouble g_IOdoubleM1;
  static nwxXmlIOwxString g_IOwxString;
  static nwxXmlIOwxString g_IOwxStringNotEmpty;
  static nwxXmlIOwxColour g_IOwxColour;
  static nwxXmlIOwxDateTime g_IOwxDateTime;
  static nwxXmlIOwxDateTime g_IOwxDateTimeXML;
  static nwxXmlIOwxDateTime g_IOwxDateTimeNonZero;
  static nwxXmlIObool g_IObool;
  static nwxXmlIOboolSkipFalse g_IOboolSkipFalse;
  static nwxXmlIOboolDefaultTrue g_IOboolTrue;
  static nwxXmlIOboolDefaultTrueSkip g_IOboolTrueSkip;
  static nwxXmlIOvectorInt g_IOvectorInt;
  static nwxXmlIOvectorUint g_IOvectorUint;
  static nwxXmlIOvectorDouble g_IOvectorDouble;
protected:
  static const wxString g_sRootNode;

  static const wxString g_sXMLNS;
  static const wxString g_sXMLNSxsi;
  static const wxString g_sXMLNSsql;
  static const wxString g_sXSIschemaLocation;

  static const wxString g_sXSIvalue;
  static const wxString g_sSQLvalue;
  void _SetFileName(const wxString &sFileName);
  wxDateTime m_dtFileModTime;
  nwxLockFile *m_pLock;
private:
  bool m_bReadOnly;
};


#define XML_INIT(T) \
virtual void Init(void *p) \
{  if ((p != NULL) && (p != this))  { ((T *)p)->Init(); } \
  else  { Init();  } \
}

#endif
