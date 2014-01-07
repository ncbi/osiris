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

*  FileName: CAllLoci.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_ALL_LOCI_H__
#define __C_ALL_LOCI_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/CIncrementer.h"

class CAllLoci : public nwxXmlPersist
{
public:
  CAllLoci() : m_setLocus(_T("Name"))
  {
    RegisterAll(true);    
  }
  virtual void Init(void *p)
  {
    m_setLocus.Init(p);
  }
  virtual void Init()
  {
    m_setLocus.Init();
    m_setKits.clear();
  }
  std::set<wxString> &GetSet()
  {
    return this->m_setLocus.GetSet();
  }
  std::set<wxString> *GetSetPtr()
  {
    return this->m_setLocus.GetSetPtr();
  }
  std::set<wxString> &GetSetKits()
  {
    return m_setKits;
  }
  std::set<wxString> *GetSetPtrKits()
  {
    return &m_setKits;
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    int &nInit(m_setLocus.GetInitCount());
    if(!nInit)
    {
      m_setLocus.Init();
      m_setKits.clear();
    }
    CIncrementer incr(m_setLocus.GetInitCount());
    CIncrementer incr2(m_xio.GetInitCount());
    bool bRtn = nwxXmlPersist::LoadFromNode(pNode,pObj);
    return bRtn;
  }
  virtual bool LoadFromNode(wxXmlNode *pNode)
  {
    int &nInit(m_setLocus.GetInitCount());
    if(!nInit)
    {
      m_setLocus.Init();
      m_setKits.clear();
    }
    CIncrementer incr(m_setLocus.GetInitCount());
    bool bRtn = nwxXmlPersist::LoadFromNode(pNode);
    return bRtn;
  }

protected:
  virtual void RegisterAll(bool = false)
  {
    Register(_T("Kits"),this);
    Register(_T("Set"),this);
    Register(_T("Name"),m_setLocus.GetIO(), &m_setKits);
    Register(_T("Locus"),&m_setLocus);
  }

private:
  set<wxString> m_setKits;
  nwxXmlIOPersistSetWxString m_xio;
  nwxXmlPersistSetWxString m_setLocus;
};

#endif
