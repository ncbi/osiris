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

*  FileName: nwxXmlMRU.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_XML_MRU__
#define __NWX_XML_MRU__

#include <time.h>
#include <wx/datetime.h>
#include <wx/filename.h>
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/nwxString.h"
#include "nwx/nwxProgressCallback.h"

//*********************************************************  nwxMRU

class nwxMRU : public nwxXmlPersist
{
public:
  virtual ~nwxMRU() { _CleanupFn(); }
  nwxMRU(const wxString &sPath);
  nwxMRU() : m_pfn(NULL)
  {
    _Clear();
    RegisterAll(true);
  }
  nwxMRU(const nwxMRU &x) : m_pfn(NULL)
  {
    (*this) = x;
    RegisterAll(true);
  }

  nwxMRU &operator = (const nwxMRU &x)
  {
    m_timeMod = x.m_timeMod;
    m_timeAccess = x.m_timeAccess;
    m_sPath = x.m_sPath;
    if(m_pfn != NULL)
    {
      if(x.m_pfn != NULL)
      {
        *m_pfn = *(x.m_pfn);
      }
      else
      {
        _CleanupFn();
      }
    }
    return *this;
  }
  void SetModTime(time_t t)
  {
    m_timeMod.Set(t);
  }
  void SetAccessTimeNow()
  {
    m_timeAccess.SetToCurrent();
  }
  const wxString &GetFullPath() const
  {
    return m_sPath;
  }
  time_t GetAccessTime() const
  {
    return m_timeAccess.GetTicks();
  }
  time_t GetModTime() const
  {
    return m_timeMod.GetTicks();
  }
  wxString GetAccessTimeFormat(const wxChar *p = nwxString::TIME_FORMAT) const
  {
    return m_timeAccess.Format(p);
  }
  wxString GetModTimeFormat(const wxChar *p = nwxString::TIME_FORMAT) const
  {
    return m_timeMod.Format(p);
  }
  bool FileOK()
  {
    return wxFileName::IsFileReadable(m_sPath);
  }
  bool UpdateModTime();
  int CompareModTime(); 
      // return 1 if file has been modified since being added to XML MRU
      //   -1 if file date/time is older than in XML MRU
      //   0 if they are equal
      //   -2 if the file cannot be found
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("Path",&m_sPath);
    RegisterWxDateTime("ModTime",&m_timeMod);
    RegisterWxDateTime("AccessTime",&m_timeAccess);
  }

  int CompareName(const nwxMRU &x) const;
  int CompareTimeAccess(const nwxMRU &x) const;
  int CompareTimeModified(const nwxMRU &x) const;
  int CompareLocation(const nwxMRU &x) const;

private:

  void _CleanupFn()
  {
    if(m_pfn != NULL)
    {
      delete m_pfn;
      m_pfn = NULL;
    }
  }
  void _SetupFn() const
  {
    if(m_pfn == NULL)
    {
      m_pfn = new wxFileName(GetFullPath());
    }
  }
  void _Clear()
  {
    m_timeMod.Set((time_t)0);
    m_timeAccess = m_timeMod;
    m_sPath.Empty();
  }
  wxDateTime m_timeMod;
  wxDateTime m_timeAccess;
  wxString m_sPath;
  mutable wxFileName *m_pfn;
};

class MRULessByPath
{
public:
  bool operator() (const nwxMRU &x1, const nwxMRU &x2) const
  {
    const wxString &s1(x1.GetFullPath());
    const wxString &s2(x2.GetFullPath());
    int n = s1.CmpNoCase(s2);
    if(!n)
    {
      n = s1.Cmp(s2);
    }
    return n < 0;
  }
  bool operator() (const nwxMRU *p1, const nwxMRU *p2) const
  {
    return (*this)(*p1,*p2);
  }
  bool operator() (nwxMRU *p1, nwxMRU *p2) const
  {
    return (*this)(*p1,*p2);
  }
};


class MRULessByAccess
{
public:
  bool operator() (const nwxMRU &x1, const nwxMRU &x2) const
  {
    time_t t1 = x1.GetAccessTime();
    time_t t2 = x2.GetAccessTime();
    bool b = false;
    if(t1 < t2)
    {
      b = true;
    }
    else if(t1 == t2)
    {
      b = m_lp(x1,x2);
    }
    return b;
  }
  bool operator() (const nwxMRU * const p1, const nwxMRU * const p2) const
  {
    return (*this)(*p1,*p2);
  }
private:
  MRULessByPath m_lp;
};




template <class T, class C1, class C2> class CopySet
{
public:
  virtual ~CopySet<T,C1,C2>() {;}
  CopySet<T,C1,C2>() {;}
  CopySet<T,C1,C2>( set <T,C1> &x1, set <T,C2> &x2)
  {
    (*this)(x1,x2);
  }
  CopySet<T,C1,C2>(set<T,C1> *p1, set<T,C2> *p2)
  {
    (*this)(*p1,*p2);
  }

  set<T,C1> &operator()(set<T,C1> &x1, set<T,C2> &x2)
  {
    x1.clear();
    for(typename set<T,C2>::iterator itr = x2.begin(); itr != x2.end(); ++itr)
    {
      x1.insert(*itr);
    }
    return x1;
  }
  set<T,C1> *operator()(set<T,C1> *p1, set<T,C2> *p2)
  {
    return &((*this)(*p1,*p2));
  }
};


//*********************************************************  nwxXmlMRU

class nwxXmlMRU : public nwxXmlPersist
{
public:
  static const size_t DEFAULT_MAX_MRU_SIZE;
  nwxXmlMRU()
  {
    _Clear();
  }
  nwxXmlMRU(size_t nMaxSize)
  {
    _Clear();
    m_nMaxSize = nMaxSize;
  }
  nwxXmlMRU(const nwxXmlMRU &x)
  {
    _Clear();
    (*this) = x;

  }
  nwxXmlMRU(
    const wxString &sFileName, 
    size_t nMaxSize = nwxXmlMRU::DEFAULT_MAX_MRU_SIZE) 
  {
    _Clear();
    m_nMaxSize = nMaxSize;
    SetFileName(sFileName);
  }
  virtual ~nwxXmlMRU() 
  {
    _Clear(false);
  }
  nwxXmlMRU &operator = (const nwxXmlMRU &x)
  {
    SetFileName(x.GetLastFileName());
    m_setMRU = x.m_setMRU;
    m_nMaxSize = x.m_nMaxSize;
    m_vpMRU.clear();
    m_nBatch = 0;
    m_bMod = false;
    return *this;
  }

  virtual void RegisterAll(bool = false);

  int GetCount() const
  {
    int n = (int)m_setMRU.Get()->size();
    return n;
  }
  void UpdateModTimeAll(
    InwxProgressCallback *pCB = NULL, 
    bool bRemoveMissing = false);
  bool RemoveFile(const wxString &sPath);
  bool UpdateModTime(const wxString &sPath, bool bRemoveMissing = false);

  bool UpdateFileAccess(const wxString &sPath);
      // sPath, name of an MRU file, update access time,
      // do not update mod time unless it didn't previous appear in MRU list

  bool Save(); // save XML file with MRU list only if modified
  bool SetFileName(const wxString &sFileName);
      // name of XML file containing the MRU list
  bool BeginBatch(int nWait = 5);
  void EndBatch();

  virtual void Init()
  {
    nwxXmlPersist::Init();
    _Clear(false);
  }
  virtual void Init(void *p)
  {
    if(p == (void *)this)
    {
      nwxXmlPersist::Init(p);
      _Clear();
    }
  }

  bool Exists()
  {
    return wxFileName::IsFileReadable(GetLastFileName());
  }
  size_t Size()
  {
    return m_setMRU.Size();
  }
  const vector<nwxMRU *> *GetVector(bool bRebuild = false)
  {
    if( bRebuild || (Size() != m_vpMRU.size()) )
    {
      _BuildVector();
    }
    return &m_vpMRU;
  }
  virtual const wxString &RootNode(void) const
  {
    return g_sMRU;
  }
private:
  void _BuildVector();
  void _RemoveExcess();

  void _Clear(bool bConstructor = true)
  {
    m_setMRU.Clear();
    m_vpMRU.clear();
    m_nBatch = 0;
    m_bMod = false;
    if(bConstructor)
    {
      m_nMaxSize = DEFAULT_MAX_MRU_SIZE;
      RegisterAll(true);
      m_setMRU.SetNodeName("File");
    }
    else
    {
      Unlock();
    }
  }
  nwxMRU *Find(const wxString &sPath, bool bCreate = false);
  TnwxXmlPersistSet<nwxMRU,MRULessByPath> m_setMRU;
  vector<nwxMRU *> m_vpMRU;
  size_t m_nMaxSize;
  size_t m_nBatch;
  bool m_bMod;
  static const wxString g_sMRU;
};

class nwxMRUBatch
{
public:
  nwxMRUBatch(nwxXmlMRU *p, int nWait = 5)
  {
    m_p = p;
    m_bOK = p->BeginBatch(nWait);
  }
  virtual ~nwxMRUBatch()
  {
    if(m_bOK)
    {
      m_p->EndBatch();
    }
  }
  bool IsOK()
  {
    return m_bOK;
  }
private:
  nwxXmlMRU *m_p;
  bool m_bOK;
};

class SortMRUbyAccess : public CopySet<nwxMRU *,MRULessByAccess, MRULessByPath>
{
public:
  virtual ~SortMRUbyAccess()
  {}
  SortMRUbyAccess()
  {}
  SortMRUbyAccess(
    set<nwxMRU *,MRULessByAccess> &x1,
    set<nwxMRU *,MRULessByPath> &x2) :
      CopySet<nwxMRU *,MRULessByAccess, MRULessByPath>(x1,x2)
  {}
  SortMRUbyAccess(
    set<nwxMRU *,MRULessByAccess> *p1,
    set<nwxMRU *,MRULessByPath> *p2) :
      CopySet<nwxMRU *,MRULessByAccess, MRULessByPath>(p1,p2)
  {}
};



#endif
