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
*  FileName: CKitLadderInfo.h
*  Author:   Douglas Hoffman
*
*/
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxXmlPersistCollections.h"
#include "nwx/setptr.h"
#include "nwx/nsstd.h"
#include <wx/arrstr.h>

class CKitLadderLocus : public nwxXmlPersist
{
public:
  CKitLadderLocus()
  {
    RegisterAll(true);
  }
  CKitLadderLocus(const CKitLadderLocus &x)
  {
    RegisterAll(false);
    (*this) = x;
  }
  virtual ~CKitLadderLocus() 
  {;}
  
  CKitLadderLocus &operator = (const CKitLadderLocus &x)
  {
    m_nChannel = x.m_nChannel;
    m_nMinBP = x.m_nMinBP;
    m_nMaxBP = x.m_nMaxBP;
    m_sName = x.m_sName;
    return (*this);
  }
  bool operator < (const CKitLadderLocus &b) const
  {
    const CKitLadderLocus &a(*this);
    int na = a.GetChannel();
    int nb = b.GetChannel();
    bool bRtn = false;
    if(na == nb)
    {
      na = a.GetMinBP();
      nb = b.GetMinBP();
    }
    bRtn = (na < nb);
    return bRtn;
  }
  int GetChannel() const
  {
    return m_nChannel;
  }
  int GetMinBP() const
  {
    return m_nMinBP;
  }
  int GetMaxBP() const
  {
    return m_nMaxBP;
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
protected:
  virtual void RegisterAll(bool bInConstructor = false)
  {
    RegisterWxString(_T("Name"),&m_sName);
    RegisterInt(_T("Channel"),&m_nChannel);
    RegisterInt(_T("MinBP"),&m_nMinBP);
    RegisterInt(_T("MaxBP"),&m_nMaxBP);
    if(bInConstructor)
    {
      Init();
    }
  }
private:
  wxString m_sName;  // locus name
  int m_nChannel;
  int m_nMinBP;
  int m_nMaxBP;
};

class CKitLadderLocusLess
{
public:
  CKitLadderLocusLess() {;}
  virtual ~CKitLadderLocusLess() {;}
  bool operator ()(const CKitLadderLocus &a, const CKitLadderLocus &b) const
  {
    bool bRtn = (a < b);
    return bRtn;
  } 
  bool operator ()(const CKitLadderLocus *pa, const CKitLadderLocus *pb) const
  {
    bool bRtn = (*pa) < (*pb);
    return bRtn;
  }
};



class CKitLadderInfo : public nwxXmlPersist
{
public:
  CKitLadderInfo();
  CKitLadderInfo(const CKitLadderInfo &x);
  virtual ~CKitLadderInfo();
  CKitLadderInfo &operator =(const CKitLadderInfo &x);
  virtual void Init();
  virtual void Init(void *p);
protected:
  virtual void RegisterAll(bool = false);
private:
  void _Cleanup();

  wxString m_sName;
  nwxXmlPersistSetWxString m_setLS;
  TnwxXmlIOPersistSet<CKitLadderLocus, CKitLadderLocusLess> m_setIO;
  set<CKitLadderLocus *, CKitLadderLocusLess> m_setLocus;
};
