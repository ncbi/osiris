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
*  FileName: nwxXmlPersistCollections.h
*  Author:   Douglas Hoffman
*
*  classes extracted from nwxXmlPersist.h because that file was getting too
*  large.
*/
#ifndef __NWX_XML_PERSIST_COLLECTIONS_H__
#define __NWX_XML_PERSIST_COLLECTIONS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/setptr.h"
#include "nwx/mapptr.h"

//*********************************************** TnwxXmlIOPersistVector

template<class T> class TnwxXmlIOPersistVector : public InwxXmlIO
{
  //
  // this is for persistent std::vector<T *> where T is a subclass of nwxXmlPersist
  //  and has a default constructor.
  //
  //  If m_bManageAll is true, then all occurrences of
  //  std::vector<T *> that are created from loading an XML stream
  //  will be cleaned up.
  //
  //  If a std::vector<T *> is created by another method,
  //  then it will not be cleaned up by this destructor
  //
public:
  TnwxXmlIOPersistVector<T>(bool bManageAll = true)
  {
    m_bManageAll = bManageAll;
  }
  TnwxXmlIOPersistVector<T>(const TnwxXmlIOPersistVector<T> &x)
  {
    (*this) = x;
  }
  TnwxXmlIOPersistVector<T> &operator = (const TnwxXmlIOPersistVector<T> &x)
  {
    m_bManageAll = x.m_bManageAll;
    return *this;
  }
  virtual ~TnwxXmlIOPersistVector<T>()
  {
    Cleanup();
  }
  static void Cleanup(vector<T *> *pv)
  {
    vectorptr<T>::cleanup(pv);
  }
  void Cleanup()
  {
    std::vector<T *> *px;
    for(typename std::set< std::vector<T*> * >::iterator itr = m_setMem.begin();
      itr != m_setMem.end();
      ++itr)
    {
      px = *itr;
      Cleanup(px);
    }
    m_setMem.clear();
  }
  void FinalCleanup()
  {
    Cleanup();
    m_setMem.clear();
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj)
  {
    unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
    if(!Skip(pObj))
    {
      std::vector<T *> &vObj(CAST(pObj));
      size_t nSize(vObj.size());
      apRtn->reserve(nSize << 1);
      for(size_t i = 0; i < nSize; ++i)
      {
        T *px(vObj.at(i));
        unique_ptr<nwxXmlNodeList> apList(px->CreateNodeList(sNodeName,px));
        size_t nListSize = apList->size();
        for(size_t j = 0; j < nListSize; j++)
        {
          apRtn->push_back(apList->at(j));
        }
      }
    }
    return apRtn.release();
  }
  virtual void Init(void *pObj)
  {
    std::vector<T *> &vObj(CAST(pObj));
    // size_t n = ((vector<T *> *)pObj)->size();
    if(m_setMem.find(&vObj) != m_setMem.end())
    {
      Cleanup(&vObj);
    }
    vObj.clear();
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    std::vector<T *> &vObj(CAST(pObj));
    if(m_bManageAll)
    {
      Manage(&vObj);
    }
    T *p = new T;
    vObj.push_back(p);
    return p->LoadFromNode(pNode,p);
  }
  void Manage(vector<T *> *p)
  {
    if(m_setMem.find(p) == m_setMem.end())
    {
      m_setMem.insert(p);
    }
  }
  static std::vector<T *> &CAST(void *pObj)
  {
    return *((vector<T *> *)pObj);
  }
private:
  bool m_bManageAll;
  std::set< std::vector<T*> * > m_setMem;
};

//*********************************************** TnwxXmlPersistVector


template<class T> class TnwxXmlPersistVector : public nwxXmlPersist
{
  // container for a vector of nwxXmlPersist where the array elements
  // have no other XML siblings
  //
  //  the node name, which defaults to "x", is the xml element for each vector item
  //
public:
  typedef typename std::vector<T *>::iterator iterator;
  typedef typename std::vector<T *>::const_iterator const_iterator;
  iterator begin()
  {
    return Get()->begin();
  }
  iterator end()
  {
    return Get()->end();
  }
  const_iterator begin() const
  {
    return Get()->begin();
  }
  const_iterator end() const
  {
    return Get()->end();
  }
  TnwxXmlPersistVector<T>() :
      m_bCreateEmpty(false),
      m_sNode(g_sRootNode)
  {
    RegisterAll(true);
  }
  TnwxXmlPersistVector<T>(
    const wxString &sNodeName, bool bCreateEmpty = false) :
      m_bCreateEmpty(bCreateEmpty),
      m_sNode(sNodeName)
  {
    RegisterAll(true);
  }

  TnwxXmlPersistVector<T>(const TnwxXmlPersistVector<T> &x)
  {
    RegisterAll(true);
    *this = x;
  }
  virtual ~TnwxXmlPersistVector<T>()
  {
    Clear();
  }
  virtual void Init()
  {
    Clear();
  }
  virtual void Init(void *p)
  {
    ((TnwxXmlPersistVector<T> *)p)->Init();
  }
  TnwxXmlPersistVector<T> &operator = (const TnwxXmlPersistVector<T> &x)
  {
    m_bCreateEmpty = x.m_bCreateEmpty;
    m_sNode = x.m_sNode;
    vectorptr<T>::copy(&m_pv,x.m_pv);
    return *this;
  }
  bool IsEqual(const TnwxXmlPersistVector<T> &x) const
  {
    bool bRtn = (m_sNode == x.m_sNode) &&
      vectorptr<T>::IsEqual(m_pv,x.m_pv);
    return bRtn;
  }
#if 0
  //  these two functions are overridden for debug breakpoints
/*
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    return nwxXmlPersist::LoadFromNode(p,pObj);
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    return nwxXmlPersist::LoadFromNode(p);
  }
*/
#endif

  std::vector<T *> *Get()
  {
    return &m_pv;
  }
  const std::vector<T *> *Get() const
  {
    return &m_pv;
  }
  std::vector<T *> *operator ->()
  {
    return &m_pv;
  }
  T *at(size_t n) const
  {
    return m_pv.at(n);
  }
  T *operator[](size_t n) const
  {
    return at(n);
  }
  size_t Size() const
  {
    return m_pv.size();
  }
  bool removeAt(size_t n)
  {
    return vectorptr<T>::removeAt(&m_pv,n);
  }

  void SetCreateEmptyNote(bool b)
  {
    m_bCreateEmpty = b;
  }
  virtual bool CreateEmptyNode() const
  {
    return m_bCreateEmpty;
  }
  bool ListNotEmpty() const
  {
    bool bRtn = false;
    T *p;
    for(typename std::vector<T *>::const_iterator itr = m_pv.begin();
      itr != m_pv.end();
      ++itr)
    {
      p = *itr;
      if(!p->Skip(NULL))
      {
        bRtn = true;
        break;
      }
    }
    return bRtn;
  }
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName)
  {
    nwxXmlNodeList *pRtn(NULL);
    if( CreateEmptyNode() || ListNotEmpty() )
    {
      pRtn = nwxXmlPersist::CreateNodeList(sNodeName);
    }
    else
    {
      pRtn = new nwxXmlNodeList;
    }
    return pRtn;
  }
  virtual bool Skip(void *pObj_)
  {
    TnwxXmlPersistVector<T> *pObj = (TnwxXmlPersistVector<T> *)pObj_;
    bool bRtn = !( pObj->CreateEmptyNode() || pObj->ListNotEmpty() );
    return bRtn;
  }
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName, void *p)
  {
    nwxXmlNodeList *pRtn(NULL);
    TnwxXmlPersistVector<T> *pObj = (TnwxXmlPersistVector<T> *)p;
    if( pObj->CreateEmptyNode() || pObj->ListNotEmpty() )
    {
      pRtn = nwxXmlPersist::CreateNodeList(sNodeName,pObj);
    }
    else
    {
      pRtn = new nwxXmlNodeList;
    }
    return pRtn;
  }

  virtual const wxString &GetNodeName() const
  {
    return m_sNode;
  }
  void SetNodeName(const wxString &sNodeName)
  {
    m_sNode = sNodeName;
    RegisterAll();
  }
  void Clear()
  {
    vectorptr<T>::cleanup(&m_pv);
  }
  InwxXmlIO *GetIO()
  {
    return &m_io;
  }
private:
  std::vector<T *> m_pv;
  TnwxXmlIOPersistVector<T> m_io;
  bool m_bCreateEmpty;
protected:
  virtual void RegisterAll(bool = false)
  {
    ClearRegistration();
    const wxString &sNode(GetNodeName());
    if(!sNode.IsEmpty())
    {
      Register(sNode,&m_io,(void *) &m_pv);
    }
  }
  wxString m_sNode;

};

template<class T> class TnwxXmlPersistVectorSorted : public TnwxXmlPersistVector<T>
{
public:
  TnwxXmlPersistVectorSorted<T>() 
  {}
  TnwxXmlPersistVectorSorted<T>(
    const wxString &sNodeName, bool bCreateEmpty = false) :
      TnwxXmlPersistVector<T>(sNodeName,bCreateEmpty) 
  {}
  TnwxXmlPersistVectorSorted<T>(const TnwxXmlPersistVectorSorted<T> &x) :
      TnwxXmlPersistVector<T>(x) 
  {}
  virtual void Sort() = 0;
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName)
  {
    Sort();
    return TnwxXmlPersistVector<T>::CreateNodeList(sNodeName);
  }
  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName, void *pObj)
  {
    ((TnwxXmlPersistVectorSorted<T> *)pObj)->Sort();
    return TnwxXmlPersistVector<T>::CreateNodeList(sNodeName,pObj);
  }
  virtual bool LoadFromNode(wxXmlNode *p, void *pObj)
  {
    bool b = TnwxXmlPersistVector<T>::LoadFromNode(p,pObj);
    if(b)
    {
      ((TnwxXmlPersistVectorSorted<T> *)pObj)->Sort();
    }
    return b;
  }
  virtual bool LoadFromNode(wxXmlNode *p)
  {
    bool b = TnwxXmlPersistVector<T>::LoadFromNode(p);
    if(b)
    {
      Sort();
    }
    return b;
  }
};


//*********************************************** TnwxXmlIOPersistSet

template<class T, class S = less<T *> > class TnwxXmlIOPersistSet : public InwxXmlIO
{
  //
  // this is for persistent std::set<T *, S> where T is a subclass of nwxXmlPersist
  //  and has a default constructor.
  //
  //  If m_bManageAll is true, then all occurrences of
  //  std::vector<T *> that are created from loading an XML stream
  //  will be cleaned up.
  //
  //  If a std::vector<T *> is created by another method,
  //  then it will not be cleaned up by this destructor
  //
public:
  TnwxXmlIOPersistSet<T,S>(bool bManageAll = true)
  {
    m_bManageAll = bManageAll;
  }
  TnwxXmlIOPersistSet<T,S>(const TnwxXmlIOPersistSet<T,S> &x)
  {
    (*this) = x;
  }
  TnwxXmlIOPersistSet<T,S> &operator =(const TnwxXmlIOPersistSet<T,S> &x)
  {
    m_bManageAll = x.m_bManageAll;
    return *this;
  }
  virtual ~TnwxXmlIOPersistSet<T,S>()
  {
    Cleanup();
  }
  void Cleanup()
  {
    for(typename std::set< std::set<T*,S> * >::iterator itr = m_setMem.begin();
      itr != m_setMem.end();
      ++itr)
    {
      setptr<T,S>::cleanup(*itr);
    }
    m_setMem.clear();
  }
  void FinalCleanup()
  {
    Cleanup();
    m_setMem.clear();
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj)
  {
    unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
    if(!Skip(pObj))
    {
      std::set<T *,S> &vObj(CAST(pObj));
      size_t nSize(vObj.size());
      apRtn->reserve(nSize << 1);
      for(typename std::set<T *,S>::iterator itr = vObj.begin();
        itr != vObj.end();
        ++itr)
      {
        T *px(*itr);
        unique_ptr<nwxXmlNodeList> apList(px->CreateNodeList(sNodeName,px));
        size_t nListSize = apList->size();
        for(size_t j = 0; j < nListSize; j++)
        {
          apRtn->push_back(apList->at(j));
        }
      }
    }
    return apRtn.release();
  }
  virtual void Init(void *pObj)
  {
    std::set<T *,S> &vObj(CAST(pObj));
    if(m_setMem.find(&vObj) != m_setMem.end())
    {
      setptr<T,S>::cleanup(&vObj);
    }
    vObj.clear();
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    std::set<T *,S> &vObj(CAST(pObj));
    if(m_bManageAll)
    {
      Manage(&vObj);
    }
    T *p = new T;
    bool bRtn = p->LoadFromNode(pNode,p);
    if( bRtn && (vObj.find(p) == vObj.end()) )
    {
      vObj.insert(p);
    }
    else
    {
      delete p;
    }
    return bRtn;
  }
  void Manage(set<T *,S> *p)
  {
    if(m_setMem.find(p) == m_setMem.end())
    {
      m_setMem.insert(p);
    }
  }
  static std::set<T *,S> &CAST(void *pObj)
  {
    return *((set<T *,S> *)pObj);
  }
private:
  bool m_bManageAll;
  std::set< std::set<T*,S> * > m_setMem;
};

//*********************************************** TnwxXmlPersistSet

template< class T, class S = less<T *> > class TnwxXmlPersistSet : public nwxXmlPersist
{
  // container for a vector of nwxXmlPersist where the array elements
  // have no other XML siblings
  //
  //  the node name, which defaults to "x", is the xml element for each vector item
  //
  // this class is similar to TnwxXmlPersistVector and uses the same XML structure,
  // except the searching (using the comparison class, S) requires each element
  // to be unique by some criteria with the default being a C++ pointer value
public:
  typedef typename std::set<T *,S>::iterator iterator;
  typedef typename std::set<T *,S>::const_iterator const_iterator;
  iterator begin()
  {
    return Get()->begin();
  }
  iterator end()
  {
    return Get()->end();
  }
  const_iterator begin() const
  {
    return Get()->begin();
  }
  const_iterator end() const
  {
    return Get()->end();
  }
  TnwxXmlPersistSet<T,S>() :
      m_bCreateEmpty(false),
      m_sNode(g_sRootNode)
  {
    RegisterAll(true);
  }
  TnwxXmlPersistSet<T,S>(const wxString &s, bool bCreateEmpty = false) :
      m_bCreateEmpty(bCreateEmpty),
      m_sNode(s)
  {
    RegisterAll(true);
  }

  TnwxXmlPersistSet<T,S>(const TnwxXmlPersistSet<T,S> &x)
  {
    RegisterAll(true);
    *this = x;
  }
  virtual ~TnwxXmlPersistSet<T,S>()
  {
    Clear();
  }
  virtual void Init()
  {
    Clear();
  }
  virtual void Init(void *p)
  {
    ((TnwxXmlPersistSet<T,S> *)p)->Init();
  }
  TnwxXmlPersistSet<T,S> &operator = (const TnwxXmlPersistSet<T,S> &x)
  {
    m_sNode = x.m_sNode;
    m_bCreateEmpty = x.m_bCreateEmpty;
    setptr<T,S>::copy(&m_pv,x.m_pv);
    return *this;
  }
  std::set<T *,S> *Get()
  {
    return &m_pv;
  }
  const std::set<T *,S> *Get() const
  {
    return &m_pv;
  }
  std::set<T *,S> *operator ->()
  {
    return &m_pv;
  }
  size_t Size() const
  {
    return m_pv.size();
  }
  bool IsEqual(const TnwxXmlPersistSet<T,S> &x) const
  {
    bool bRtn = true;
    if(m_sNode != x.m_sNode)
    {
      bRtn = false;
    }
    else if(setptr<T,S>::IsEqual(m_pv,x.m_pv))
    {
      // done
      // bRtn = true;
    }
    else
    {
      typename std::set<T*,S>::const_iterator itr;
      typename std::set<T*,S>::const_iterator itrx;
      const std::set<T*,S> *pThis = Get();
      const std::set<T*,S> *px = x.Get();
      T* pObj;
      T* pObjx;
      for(itr = pThis->begin();
        bRtn && (itr != pThis->end());
        ++itr)
      {
        pObj = *itr;
        itrx = px->find(*itr);
        if(itrx != px->end())
        {
          pObjx = *itrx;
          if(!( (*pObj) == (*pObjx) ))
          {
            bRtn = false;
          }
        }
        else if(!pObj->Skip(pObj))
        {
          // does not exist in x and not being skipped
          // therefore not equal
          bRtn = false;
        }
      }
      for(itrx = px->begin();
        bRtn && (itrx != px->end());
        ++itrx)
      {
        pObjx = *itrx;
        itr = pThis->find(*itrx);
        if(itr == pThis->end())
        {
          if(!pObjx->Skip(pObjx))
          {
            bRtn = false;
          }
        }
        else
        {
          // do nothing, comparison was made in previous loop
#ifdef __WXDEBUG__
          pObj = *itr;
          if(!( (*pObj) == (*pObjx) ))
          {
            bRtn = false;
            wxASSERT_MSG(0,"unexpected inequality");
          }
#endif
        }
      }
    }
    return bRtn;
  }
  bool operator == (const TnwxXmlPersistSet<T,S> &x) const
  {
    return IsEqual(x);
  }

  void SetCreateEmptyNote(bool b)
  {
    m_bCreateEmpty = b;
  }
  virtual bool CreateEmptyNode() const
  {
    return m_bCreateEmpty;
  }

  bool ListNotEmpty()
  {
    bool bRtn = false;
    T *p;
    for(typename std::set<T *,S>::iterator itr = m_pv.begin();
      itr != m_pv.end();
      ++itr)
    {
      p = *itr;
      if(!p->Skip(NULL))
      {
        bRtn = true;
        break;
      }
    }
    return bRtn;
  }
  virtual bool Skip(void *pObj_)
  {
    TnwxXmlPersistSet<T,S> *pObj = (TnwxXmlPersistSet<T,S> *)pObj_;
    bool bRtn = !( pObj->CreateEmptyNode() || pObj->ListNotEmpty() );
    return bRtn;
  }

  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName)
  {
    nwxXmlNodeList *pRtn(NULL);
    if( CreateEmptyNode() || ListNotEmpty() )
    {
      pRtn = nwxXmlPersist::CreateNodeList(sNodeName);
    }
    else
    {
      pRtn = new nwxXmlNodeList;
    }
    return pRtn;
  }

  virtual const wxString &GetNodeName() const
  {
    return m_sNode;
  }
  void SetNodeName(const wxString &x)
  {
    m_sNode = x;
    RegisterAll();
  }
  void Clear()
  {
    setptr<T,S>::cleanup(&m_pv);
  }
  InwxXmlIO *GetIO()
  {
    return &m_io;
  }
private:
  std::set<T *,S> m_pv;
  TnwxXmlIOPersistSet<T,S> m_io;
  bool m_bCreateEmpty;
protected:
  virtual void RegisterAll(bool = false)
  {
    ClearRegistration();
    const wxString &sNode(GetNodeName());
    if(!sNode.IsEmpty())
    {
      Register(sNode,&m_io,(void *) &m_pv);
    }
  }
  wxString m_sNode;

};



// begin Map

//  the map classes, TnwxXmlIOPersistMap and TnwxXmlPersistMap,
//  were developed on 12/23/10 and have not been tested at all

//*********************************************** TnwxXmlIOPersistMap

template<class K, class T, class S = less<K> > class TnwxXmlIOPersistMap : public InwxXmlIO
{
  //
  // this is for persistent std::map<K, T *, S> where T is a subclass of nwxXmlPersist
  //  and has a default constructor.
  //
  //  If m_bManageAll is true, then all occurrences of
  //  std::map<K,T *,S> that are created from loading an XML stream
  //  will be cleaned up.
  //
  //  If a std::map<K,T *,S> is created by another method,
  //  then it will not be cleaned up by this destructor
  //
  //   Important Note:  class T must contain a method 
  //      GetKey() that returns a value of type K or const K&
  //      and it must contain a method 
  //            SetKey(K), SetKey(K&), or SetKey(const K&)
  //
public:
  typedef typename std::map<K, T *,S>::value_type VALUE_TYPE;
  TnwxXmlIOPersistMap<K,T,S>(bool bManageAll = true)
  {
    m_bManageAll = bManageAll;
  }
  TnwxXmlIOPersistMap<K,T,S>(const TnwxXmlIOPersistMap<T,S> &x)
  {
    (*this) = x;
  }
  TnwxXmlIOPersistMap<K,T,S> &operator =(const TnwxXmlIOPersistMap<T,S> &x)
  {
    m_bManageAll = x.m_bManageAll;
    return *this;
  }
  virtual ~TnwxXmlIOPersistMap<K,T,S>()
  {
    Cleanup();
  }
  void Cleanup()
  {
    for(typename std::set< std::map<K, T*,S> * >::iterator itr = m_setMem.begin();
      itr != m_setMem.end();
      ++itr)
    {
      mapptr<K,T,S>::cleanup(*itr);
    }
    m_setMem.clear();
  }
  void FinalCleanup()
  {
    Cleanup();
    m_setMem.clear();
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pObj)
  {
    unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
    if(!Skip(pObj))
    {
      std::map<K, T *,S> &vObj(CAST(pObj));
      size_t nSize(vObj.size());
      apRtn->reserve(nSize << 1);
      for(typename std::map<K, T *,S>::iterator itr = vObj.begin();
        itr != vObj.end();
        ++itr)
      {
        T *px(itr->second);
        unique_ptr<nwxXmlNodeList> apList(px->CreateNodeList(sNodeName,px));
        size_t nListSize = apList->size();
        for(size_t j = 0; j < nListSize; j++)
        {
          apRtn->push_back(apList->at(j));
        }
      }
    }
    return apRtn.release();
  }
  virtual void Init(void *pObj)
  {
    typename std::map<K, T *,S> &vObj(CAST(pObj));
    if(m_setMem.find(&vObj) != m_setMem.end())
    {
      mapptr<K,T,S>::cleanup(&vObj);
    }
    vObj.clear();
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    typename std::map<K, T *,S> &vObj(CAST(pObj));
    if(m_bManageAll)
    {
      Manage(&vObj);
    }
    T *p = new T;
    bool bRtn = p->LoadFromNode(pNode,p);
    if( bRtn && (vObj.find(p->GetKey()) == vObj.end()) )
    {
      vObj.insert(VALUE_TYPE(p->GetKey(),p));
    }
    else
    {
      delete p;
    }
    return bRtn;
  }
  void Manage(map<K, T *,S> *p)
  {
    if(m_setMem.find(p) == m_setMem.end())
    {
      m_setMem.insert(p);
    }
  }
  static std::map<K, T *,S> &CAST(void *pObj)
  {
    return *((map<K, T *,S> *)pObj);
  }
private:
  bool m_bManageAll;
  std::set< std::map<K,T*,S> * > m_setMem;
};

//*********************************************** TnwxXmlPersistMap

template<class K, class T, class S = less<K> > class TnwxXmlPersistMap : public nwxXmlPersist
{
  // container for a map of nwxXmlPersist where the array elements
  // have no other XML siblings
  //
  //  the node name, which defaults to "x", is the xml element for each vector item
  //
  // this class is similar to TnwxXmlPersistVector and uses the same XML structure,
  // except the searching (using the comparison class, S) requires each element
  // to be unique by some criteria with the default being a C++ pointer value
  // The key, classK, is obtained by calling GetKey() from the object of class T
public:
  typedef typename std::map<K, T *,S>::value_type VALUE_TYPE;
  typedef typename std::map<K, T *,S>::iterator iterator;
  typedef typename std::map<K, T *,S>::const_iterator const_iterator;
  iterator begin()
  {
    return Get()->begin();
  }
  iterator end()
  {
    return Get()->end();
  }
  const_iterator begin() const
  {
    return Get()->begin();
  }
  const_iterator end() const
  {
    return Get()->end();
  }
  TnwxXmlPersistMap<K, T,S>() :
      m_bCreateEmpty(false),
      m_sNode(g_sRootNode)
  {
    RegisterAll(true);
  }
  TnwxXmlPersistMap<K,T,S>(const wxString &s, bool bCreateEmpty = false) :
      m_bCreateEmpty(bCreateEmpty),
      m_sNode(s)
  {
    RegisterAll(true);
  }

  TnwxXmlPersistMap<K,T,S>(const TnwxXmlPersistMap<K,T,S> &x)
  {
    *this = x;
    RegisterAll(true);
  }
  virtual ~TnwxXmlPersistMap<K,T,S>()
  {
    Clear();
  }
  virtual void Init()
  {
    Clear();
  }
  virtual void Init(void *p)
  {
    ((TnwxXmlPersistMap<K,T,S>*)p)->Init();
  }
  TnwxXmlPersistMap<K,T,S> &Copy(const TnwxXmlPersistMap<K,T,S> &x)
  {
    m_sNode = x.m_sNode;
    m_bCreateEmpty = x.m_bCreateEmpty;
    mapptr<K,T,S>::copy(&m_pv,x.m_pv);
    return *this;
  }
  TnwxXmlPersistMap<K,T,S> &operator = (const TnwxXmlPersistMap<K,T,S> &x)
  {
    return Copy(x);
  }
  void InsertOrReplace(const T &x)
  {
    iterator itr = m_pv.find(x.GetKey());
    if(itr == m_pv.end())
    {
      T *p = new T(x);
      m_pv.insert(
        VALUE_TYPE(
          p->GetKey(),p));
    }
    else
    {
      T *p = itr->second;
      (*p) = x;
    }
  }
  bool Insert(T *p)
  {
    iterator itr = m_pv.find(p->GetKey());
    bool bRtn = (itr == m_pv.end());
    if(bRtn)
    {
      m_pv.insert(
        VALUE_TYPE(p->GetKey(),p));
    }
    return bRtn;
  }
  std::map<K, T *,S> *Get()
  {
    return &m_pv;
  }
  const std::map<K, T *,S> *Get() const
  {
    return &m_pv;
  }
  std::map<K, T *,S> *operator ->()
  {
    return &m_pv;
  }
  size_t Size() const
  {
    return m_pv.size();
  }
  bool IsEqual(const TnwxXmlPersistMap<K,T,S> &x) const
  {
    bool bRtn = true;
    if(m_sNode != x.m_sNode)
    {
      bRtn = false;
    }
    else if(mapptr<K,T,S>::IsEqual(m_pv,x.m_pv))
    {
      // done
      // bRtn = true;
    }
    else
    {
      const_iterator itr;
      const_iterator itrx;
      const std::map<K,T*,S> *pThis = Get();
      const std::map<K,T*,S> *px = x.Get();
      T* pObj;
      T* pObjx;
      for(itr = pThis->begin();
        bRtn && (itr != pThis->end());
        ++itr)
      {
        pObj = itr->second;
        itrx = px->find(itr->first);
        if(itrx != px->end())
        {
          pObjx = itrx->second;
          if(!( (*pObj) == (*pObjx) ))
          {
            bRtn = false;
          }
        }
        else if(!pObj->Skip(pObj))
        {
          // does not exist in x and not being skipped
          // therefore not equal
          bRtn = false;
        }
      }
      for(itrx = px->begin();
        bRtn && (itrx != px->end());
        ++itrx)
      {
        pObjx = itrx->second;
        itr = pThis->find(itrx->first);
        if(itr == pThis->end())
        {
          if(!pObjx->Skip(pObjx))
          {
            bRtn = false;
          }
        }
        else
        {
          // do nothing, comparison was made in previous loop
#ifdef __WXDEBUG__
          pObj = itr->second;
          if(!( (*pObj) == (*pObjx) ))
          {
            bRtn = false;
            wxASSERT_MSG(0,"unexpected inequality");
          }
#endif
        }
      }
    }
    return bRtn;
  }
  bool operator == (const TnwxXmlPersistMap<K,T,S> &x) const
  {
    return IsEqual(x);
  }
  bool operator != (const TnwxXmlPersistMap<K,T,S> &x) const
  {
    return !IsEqual(x);
  }
  bool HasKey(K x) const
  {
    bool b = (m_pv.find(x) != m_pv.end());
    return b;
  }
  bool HasKeyp(const K &x) const
  {
    bool b = (m_pv.find(x) != m_pv.end());
    return b;
  }
  T *Find(const K &x)
  {
    iterator itr = m_pv.find(x);
    T *pRtn = (itr == m_pv.end()) ? NULL : itr->second;
    return pRtn;
  }
  const T *Find(const K &x) const
  {
    const_iterator itr = m_pv.find(x);
    const T *pRtn = (itr == m_pv.end()) ? NULL : itr->second;
    return pRtn;
  }

  bool Delete(const K &x)
  {
    iterator itr = m_pv.find(x);
    bool bRtn = (itr != m_pv.end());
    if(bRtn)
    {
      delete itr->second;
      m_pv.erase(itr);
    }
    return bRtn;
  }
  bool ChangeKey(T *obj, const K &sNew)
  {
    iterator itr = m_pv.find(sNew);
    bool bRtn = false;
    if(itr != m_pv.end())
    {
      bRtn = (itr->second == obj);
      // if same name, no problem
      // otherwise key already used
    }
    else
    {
      itr = m_pv.find(obj->GetKey());
      if(itr == end())
      {
        wxASSERT_MSG(0,
          "TnwxXmlPersistMap<K,T,S>::RenameKey could not find object");
      }
      else
      {
        m_pv.erase(itr);
        m_pv.insert(VALUE_TYPE(
          sNew,obj));
        obj->SetKey(sNew);
        bRtn = true;
      }
    }
    return bRtn;
  }
  bool FixKeys(size_t *nFix = NULL)
  {
    // this function is unused and untested
    std::set<K,S> tempSet;
    std::map<K,T*,S> killMap;
    iterator itr;
    iterator itrb;
    T *obj;
    K key;
    bool bRtn = true;
    for(itr = m_pv.begin(); bRtn && (itr != m_pv.end()); ++itr)
    {
      obj = itr->second;
      key = obj->GetKey();
      itrb = m_pv.find(key);
      if(itrb == m_pv.end())
      {
        // this object has been renamed

        if(tempSet.find(key) != tempSet.end())
        {
          // duplicate found with another object being renamed
          bRtn = false;
        }
        else
        {
          tempSet.insert(key);
          killMap.insert(VALUE_TYPE(itr->first,obj));
        }
      }
      else if(itrb->second != itr->second)
      {
        // duplicate found
        bRtn = false;
      }
    }
    size_t nSize = bRtn ? tempSet.size() : 0;
    size_t nCount = 0;
    if(!nSize)
    {
      // we are done
    }
    else if(nSize != killMap.size())
    {
      // we have a bug
      wxASSERT_MSG(0,
        "TnwxXmlPersistMap<K,T,S>::FixKeys has a size problem");
      bRtn = false;
    }
    else
    {
      for(itr = killMap.begin(); itr != killMap.end(); ++itr)
      {
        itrb = m_pv.find(itr->first);
        obj = itr->second;
        if(itrb == m_pv.end())
        {
          wxASSERT_MSG(0,
            "TnwxXmlPersistMap<K,T,S>::FixKeys "
            "cannot remove previously found item");
        }
        else if(m_pv.find(obj->GetKey()) != m_pv.end())
        {
          wxASSERT_MSG(0,
            "TnwxXmlPersistMap<K,T,S>::FixKeys "
            "found item that was previously not found");
        }
        else
        {
          m_pv.erase(itrb);
          m_pv.insert(
            VALUE_TYPE(
              obj->GetKey(),
              obj));
          nCount++;
        }
      }
    }
    if(nFix != NULL)
    {
      *nFix = nCount;
    }
    return bRtn;
  }
  void SetCreateEmptyNote(bool b)
  {
    m_bCreateEmpty = b;
  }
  virtual bool CreateEmptyNode() const
  {
    return m_bCreateEmpty;
  }

  bool ListNotEmpty()
  {
    bool bRtn = false;
    T *p;
    for(typename std::map<K, T *,S>::iterator itr = m_pv.begin();
      itr != m_pv.end();
      ++itr)
    {
      p = itr->second;
      if(!p->Skip(NULL))
      {
        bRtn = true;
        break;
      }
    }
    return bRtn;
  }
  virtual bool Skip(void *pObj_)
  {
    TnwxXmlPersistMap<K,T,S> *pObj = (TnwxXmlPersistMap<K,T,S> *)pObj_;
    bool bRtn = !( pObj->CreateEmptyNode() || pObj->ListNotEmpty() );
    return bRtn;
  }

  virtual nwxXmlNodeList *CreateNodeList(const wxString &sNodeName)
  {
    nwxXmlNodeList *pRtn(NULL);
    if( CreateEmptyNode() || ListNotEmpty() )
    {
      pRtn = nwxXmlPersist::CreateNodeList(sNodeName);
    }
    else
    {
      pRtn = new nwxXmlNodeList;
    }
    return pRtn;
  }

  virtual const wxString &GetNodeName() const
  {
    return m_sNode;
  }
  void SetNodeName(const wxString &x)
  {
    m_sNode = x;
    RegisterAll();
  }
  void Clear()
  {
    mapptr<K,T,S>::cleanup(&m_pv);
  }
  InwxXmlIO *GetIO()
  {
    return &m_io;
  }
private:
  std::map<K, T *,S> m_pv;
  TnwxXmlIOPersistMap<K,T,S> m_io;
  bool m_bCreateEmpty;
protected:
  virtual void RegisterAll(bool = false)
  {
    ClearRegistration();
    const wxString &sNode(GetNodeName());
    if(!sNode.IsEmpty())
    {
      Register(sNode,&m_io,(void *) &m_pv);
    }
  }
  wxString m_sNode;

};

// end Map


//********************************* class TnwxXmlIOPersistVectorSimple

template<class T, class NWXIO> class TnwxXmlIOPersistVectorSimple
            : public InwxXmlIO
{
  //
  // this is for persistent std::vector<T> where T and NWXIO have
  // default constructors.  NWXIO is a subclass of InwxXmlIO
  // Used for simple types like
  // int, uint, wxString, double, etc.
  //
public:
  TnwxXmlIOPersistVectorSimple<T,NWXIO>()    { m_nInit = 0;}
  TnwxXmlIOPersistVectorSimple<T,NWXIO>(const TnwxXmlIOPersistVectorSimple<T,NWXIO> &)
  {
    m_nInit = 0;
  }
  TnwxXmlIOPersistVectorSimple<T,NWXIO> &operator = (const TnwxXmlIOPersistVectorSimple<T,NWXIO> &)
  {
    return *this;
  }
  virtual ~TnwxXmlIOPersistVectorSimple<T,NWXIO>()
  {
    wxASSERT_MSG(!m_nInit,"m_nInit is not 0");
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pvObj)
  {
    unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
    if(!Skip(pvObj))
    {
      std::vector<T> &vObj(CAST(pvObj));
      size_t nSize(vObj.size());
      apRtn->reserve(nSize << 1);
      for(size_t i = 0; i < nSize; ++i)
      {
        const T &x(vObj.at(i));
        unique_ptr<nwxXmlNodeList> apList(m_io.CreateNodeList(sNodeName,(void *)&x));
        size_t nListSize = apList->size();
        for(size_t j = 0; j < nListSize; j++)
        {
          apRtn->push_back(apList->at(j));
        }
      }
    }
    return apRtn.release();
  }
  virtual void Init(void *pObj)
  {
    if(!m_nInit)
    {
      std::vector<T> &vObj(CAST(pObj));
      vObj.clear();
    }
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    std::vector<T> &vObj(CAST(pObj));
    T x;
    bool b = m_io.LoadFromNode(pNode,&x);
    if(b)
    {
      vObj.push_back(x);
    }
    return b;
  }
  static std::vector<T> &CAST(void *pObj)
  {
    return *((vector<T> *)pObj);
  }
  int &GetInitCount()
  {
    return m_nInit;
  }
private:
  NWXIO m_io;
  int m_nInit;
};


template<class T, class NWXIO> class TnwxXmlIOPersistSetSimple
            : public InwxXmlIO
{
  //
  // this is for persistent std::set<T> where T and NWXIO have
  // default constructors.  NWXIO is a subclass of InwxXmlIO
  // Used for simple types like
  // int, uint, wxString, double, etc.
  //
public:
  TnwxXmlIOPersistSetSimple<T,NWXIO>()    { m_nInit = 0;}
  TnwxXmlIOPersistSetSimple<T,NWXIO>(const TnwxXmlIOPersistSetSimple<T,NWXIO> &)
  { m_nInit = 0;}
  virtual ~TnwxXmlIOPersistSetSimple<T,NWXIO>()
  {
    wxASSERT_MSG(!m_nInit,"m_nInit is not 0");
  }
  TnwxXmlIOPersistSetSimple<T,NWXIO> &operator =(const TnwxXmlIOPersistSetSimple<T,NWXIO> &)
  {
    return *this;
  }
  virtual nwxXmlNodeList *CreateNodeList(
              const wxString &sNodeName, void *pvObj)
  {
    unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
    if(!Skip(pvObj))
    {
      std::set<T> &vObj(CAST(pvObj));
      for(typename std::set<T>::iterator itr = vObj.begin();
        itr != vObj.end();
        ++itr)
      {
        const T &x(*itr);
        unique_ptr<nwxXmlNodeList> apList(m_io.CreateNodeList(sNodeName,(void *)&x));
        size_t nListSize = apList->size();
        for(size_t j = 0; j < nListSize; j++)
        {
          apRtn->push_back(apList->at(j));
        }
      }
    }
    return apRtn.release();
  }
  virtual void Init(void *pObj)
  {
    if(!m_nInit)
    {
      std::set<T> &vObj(CAST(pObj));
      vObj.clear();
    }
  }
  virtual bool LoadFromNode(wxXmlNode *pNode, void *pObj)
  {
    std::set<T> &vObj(CAST(pObj));
    T x;
    bool b = m_io.LoadFromNode(pNode,&x);
    if(b)
    {
      vObj.insert(x);
    }
    return b;
  }
  static std::set<T> &CAST(void *pObj)
  {
    return *((set<T> *)pObj);
  }
  int &GetInitCount()
  {
    return m_nInit;
  }
private:
  NWXIO m_io;
  int m_nInit;
};




template <class T, class NWXIO> class TnwxXmlPersistVectorSimple
                    : public nwxXmlPersist
{
  // this is used for a vector inside a parent node.
  // where the child is a simple type or a type whose contents can
  //      be converted to/from a single string,
  //     i.e. string, int, double, etc.
  // for example:
  //  <parent>
  //    <child>1</child>
  //    <child>2</child>
  //    <child>3</child>
  //    <child>4</child>
  //  </parent>
  //
public:
  virtual ~TnwxXmlPersistVectorSimple<T,NWXIO>()
  {
  }

  // BEGIN CONSTRUCTORS

  TnwxXmlPersistVectorSimple<T,NWXIO>()
    : m_bSkipIfEmpty(false)
  {
    m_sChildName = g_sRootNode;
    RegisterAll(true);
  }
  TnwxXmlPersistVectorSimple<T,NWXIO>(
    const wxString &sChildName, bool bSkipIfEmpty = false)
    : m_bSkipIfEmpty(bSkipIfEmpty)
  {
    m_sChildName = sChildName;
    RegisterAll(true);
  }
  TnwxXmlPersistVectorSimple<T,NWXIO>(const TnwxXmlPersistVectorSimple<T,NWXIO> &x)
  {
    (*this) = x;
  }

  // END CONSTRUCTORS

  TnwxXmlPersistVectorSimple<T,NWXIO> &operator = (const std::vector<T> &x)
  {
    m_vec = x;
    return *this;
  }
  TnwxXmlPersistVectorSimple<T,NWXIO> &operator = (const TnwxXmlPersistVectorSimple<T,NWXIO> &x)
  {
    m_sChildName = x.m_sChildName;
    m_bSkipIfEmpty = x.m_bSkipIfEmpty;
    m_vec = x.m_vec;
    RegisterAll();
    return *this;
  }

  bool operator == (const TnwxXmlPersistVectorSimple<T,NWXIO> &x) const
  {
    return (m_sChildName == x.m_sChildName) &&
      (m_vec == x.m_vec);
  }
  bool operator != (const TnwxXmlPersistVectorSimple<T,NWXIO> &x) const
  {
    return !((*this) == x);
  }
  void SetChildName(const wxString &x)
  {
    m_sChildName = x;
    RegisterAll();
  }
  virtual void Init(void *p)
  {
    if(p == (void *) this)
    {
      Init();
    }
    else if(p == (void *)&m_vec)
    {
      Init();
    }
    else
    {
      ((TnwxXmlPersistVectorSimple<T,NWXIO> *)p)->Init();
    }
  }
  virtual void Init()
  {
    if(!GetInitCount())
    {
      m_vec.clear();
      nwxXmlPersist::Init();
    }
  }
  operator std::vector<T> &()
  {
    return m_vec;
  }
  operator std::vector<T> *()
  {
    return &m_vec;
  }
  const std::vector<T> &GetVector() const
  {
    return m_vec;
  }
  std::vector<T> &GetVector()
  {
    return m_vec;
  }
  std::vector<T> *GetVectorPtr()
  {
    return &m_vec;
  }
  std::vector<T> * operator ->()
  {
    return &m_vec;
  }
  virtual bool Skip()
  {
    bool bRtn = m_bSkipIfEmpty;
    if(!bRtn) {}
    else if(m_vec.empty()) {}
    else
    {
      bRtn = true;
      typename std::vector<T>::iterator itr;
      for(itr = m_vec.begin();
        (itr != m_vec.end()) && bRtn;
        ++itr)
      {
        const T &x = *itr;
        if(!m_xio.Skip((void *)&x))
        {
          bRtn = false;
        }
      }
    }
    return bRtn;
  }
  virtual bool Skip(void *p)
  {
    TnwxXmlPersistVectorSimple<T,NWXIO> *pObj = 
      (TnwxXmlPersistVectorSimple<T,NWXIO> *)p;
    return pObj->Skip();
  }
  void SetSkipIfEmpty(bool b)
  {
    m_bSkipIfEmpty = b;
  }
protected:
  virtual void RegisterAll(bool bConstructor = false)
  {
    if(!bConstructor)
    {
      ClearRegistration();
    }
    Register(m_sChildName,&m_xio,(void *)&m_vec);
  }
  int &GetInitCount()
  {
    return m_xio.GetInitCount();
  }
  TnwxXmlIOPersistVectorSimple<T,NWXIO> *GetIO()
  {
    return &m_xio;
  }
private:
  std::vector<T> m_vec;
  TnwxXmlIOPersistVectorSimple<T,NWXIO> m_xio;
  wxString m_sChildName;
  bool m_bSkipIfEmpty;

};


template <class T, class NWXIO> class TnwxXmlPersistSetSimple
                    : public nwxXmlPersist
{
  // this is used for a set inside a parent node.
  // where the child is a simple type or a type whose contents can
  //      be converted to/from a single string
  //     , i.e. string, int, double, etc.
  //    and there is no duplicates
  // for example:
  //  <parent>
  //    <child>1</child>
  //    <child>2</child>
  //    <child>3</child>
  //    <child>4</child>
  //  </parent>
  //
public:
  virtual ~TnwxXmlPersistSetSimple<T,NWXIO>()
  {
  }

  // BEGIN CONSTRUCTORS

  TnwxXmlPersistSetSimple<T,NWXIO>()
    : m_bSkipIfEmpty(false)
  {
    m_sChildName = g_sRootNode;
    RegisterAll(true);
  }
  TnwxXmlPersistSetSimple<T,NWXIO>(
    const wxString &sChildName, bool bSkipIfEmpty = false)
    : m_bSkipIfEmpty(bSkipIfEmpty)
  {
    m_sChildName = sChildName;
    RegisterAll(true);
  }
  TnwxXmlPersistSetSimple<T,NWXIO>(const TnwxXmlPersistSetSimple<T,NWXIO> &x)
  {
    (*this) = x;
  }

  // END CONSTRUCTORS

  TnwxXmlPersistSetSimple<T,NWXIO> &operator = (const std::set<T> &x)
  {
    m_set = x;
    return *this;
  }
  TnwxXmlPersistSetSimple<T,NWXIO> &operator = (const TnwxXmlPersistSetSimple<T,NWXIO> &x)
  {
    m_sChildName = x.m_sChildName;
    m_set = x.m_set;
    m_bSkipIfEmpty = x.m_bSkipIfEmpty;
    RegisterAll();
    return *this;
  }
  void SetChildName(const wxString &x)
  {
    m_sChildName = x;
    RegisterAll();
  }
  virtual void Init(void *p)
  {
    if(p == (void *) this)
    {
      Init();
    }
    else if(p == (void *)&m_set)
    {
      Init();
    }
    else
    {
      ((TnwxXmlPersistSetSimple<T,NWXIO> *)p)->Init();
    }
  }
  virtual void Init()
  {
    if(!GetInitCount())
    {
      m_set.clear();
      nwxXmlPersist::Init();
    }
  }
  operator std::set<T> &()
  {
    return m_set;
  }
  operator std::set<T> *()
  {
    return &m_set;
  }
  std::set<T> &GetSet() 
  {
    return m_set;
  }
  std::set<T> *GetSetPtr()
  {
    return &m_set;
  }
  const std::set<T> *GetSetPtr() const
  {
    return &m_set;
  }
  std::set<T> * operator ->()
  {
    return &m_set;
  }
  int &GetInitCount()
  {
    return m_xio.GetInitCount();
  }
  TnwxXmlIOPersistSetSimple<T,NWXIO> *GetIO()
  {
    return &m_xio;
  }

#ifdef __WXDEBUG__
  static void UnitTest();
#endif

  virtual bool Skip()
  {
    bool bRtn = m_bSkipIfEmpty;
    if(!bRtn) {}
    else if(m_set.empty()) {}
    else
    {
      bRtn = true;
      typename std::set<T>::iterator itr;
      for(itr = m_set.begin();
        (itr != m_set.end()) && bRtn;
        ++itr)
      {
        const T &x = *itr;
        if(!m_xio.Skip((void *)&x))
        {
          bRtn = false;
        }
      }
    }
    return bRtn;
  }
  virtual bool Skip(void *p)
  {
    TnwxXmlPersistVectorSimple<T,NWXIO> *pObj = 
      (TnwxXmlPersistVectorSimple<T,NWXIO> *)p;
    return pObj->Skip();
  }
  void SetSkipIfEmpty(bool b)
  {
    m_bSkipIfEmpty = b;
  }

protected:
  virtual void RegisterAll(bool bConstructor = false)
  {
    if(!bConstructor)
    {
      ClearRegistration();
    }
    Register(m_sChildName,&m_xio,(void *)&m_set);
  }

private:
  std::set<T> m_set;
  TnwxXmlIOPersistSetSimple<T,NWXIO> m_xio;
  wxString m_sChildName;
  bool m_bSkipIfEmpty;
};

typedef TnwxXmlIOPersistVectorSimple<wxString,nwxXmlIOwxString> nwxXmlIOPersistVectorWxString;
typedef TnwxXmlIOPersistSetSimple<wxString,nwxXmlIOwxString> nwxXmlIOPersistSetWxString;

typedef TnwxXmlIOPersistVectorSimple<wxDateTime,nwxXmlIOwxDateTime> nwxXmlIOPersistVectorWxDateTime;
typedef TnwxXmlIOPersistSetSimple<wxDateTime,nwxXmlIOwxDateTime> nwxXmlIOPersistSetWxDateTime;

typedef TnwxXmlPersistVectorSimple<wxString,nwxXmlIOwxString> nwxXmlPersistVectorWxString;
typedef TnwxXmlPersistSetSimple<wxString,nwxXmlIOwxString> nwxXmlPersistSetWxString;

typedef TnwxXmlPersistVectorSimple<wxDateTime,nwxXmlIOwxDateTime> nwxXmlPersistVectorWxDateTime;
typedef TnwxXmlPersistSetSimple<wxDateTime,nwxXmlIOwxDateTime> nwxXmlPersistSetWxDateTime;




#endif
