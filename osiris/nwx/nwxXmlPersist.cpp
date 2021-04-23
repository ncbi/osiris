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

*  FileName: nwxXmlPersist.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwxXmlPersist.h"
#include "nwx/nwxString.h"
#include "nwx/stdb.h"
#include <memory>
#include "nwx/stde.h"
#include "nwx/CIncrementer.h"

nwxXmlIOint nwxXmlPersist::g_IOint;
nwxXmlIOint nwxXmlPersist::g_IOintNonZero(true);
nwxXmlIOintPositive nwxXmlPersist::g_IOintPositive(false);
nwxXmlIOuint nwxXmlPersist::g_IOuint;
nwxXmlIOlong nwxXmlPersist::g_IOlong;
nwxXmlIOdouble nwxXmlPersist::g_IOdouble;
nwxXmlIOdouble nwxXmlPersist::g_IOdoubleM1(-1.0);
nwxXmlIOwxString nwxXmlPersist::g_IOwxString;
nwxXmlIOwxString nwxXmlPersist::g_IOwxStringNotEmpty(true);
nwxXmlIObool nwxXmlPersist::g_IObool;
nwxXmlIOboolSkipFalse nwxXmlPersist::g_IOboolSkipFalse;
nwxXmlIOboolDefaultTrue nwxXmlPersist::g_IOboolTrue;
nwxXmlIOboolDefaultTrueSkip nwxXmlPersist::g_IOboolTrueSkip;
nwxXmlIOvectorInt nwxXmlPersist::g_IOvectorInt;
nwxXmlIOvectorUint nwxXmlPersist::g_IOvectorUint;
nwxXmlIOvectorDouble nwxXmlPersist::g_IOvectorDouble;
nwxXmlIOwxColour nwxXmlPersist::g_IOwxColour;
nwxXmlIOwxDateTime nwxXmlPersist::g_IOwxDateTime;
nwxXmlIOwxDateTime nwxXmlPersist::g_IOwxDateTimeNonZero(false,true);
nwxXmlIOwxDateTime nwxXmlPersist::g_IOwxDateTimeXML(true,true);


// ********************************************************
//
//  toString
//

wxString ToString::NodeContents(wxXmlNode *pNode, bool bDeep)
{
  wxString sRtn;
  wxXmlNode *pChild;
  wxXmlNodeType nType;
  for(pChild = pNode->GetChildren();
    pChild != NULL;
    pChild = pChild->GetNext())
  {
    nType = pChild->GetType();
    if(nType == wxXML_TEXT_NODE || nType == wxXML_CDATA_SECTION_NODE)
    {
      sRtn.Append(pChild->GetContent());
    }
    else if(bDeep && (nType == wxXML_ELEMENT_NODE))
    {
      sRtn.Append("\n");
      sRtn.Append(NodeContents(pChild,bDeep));
    }
  }
  return sRtn;
}

// ********************************************************
//
//  nwxXmlIOwxDateTime
//


wxString nwxXmlIOwxDateTime::ToString(void *pObj)
{
  wxString sRtn;
  wxDateTime &x(CAST(pObj));
  if(m_bXML)
  {
    //  YYYY-MM-DDThh:mm:ss
    sRtn = nwxString::FormatDateTimeXML(x);
  }
  else
  {
    //  seconds since 1/1/1970
    sRtn.Printf("%ld",(long)x.GetTicks());
  }
  return sRtn;
}
wxDateTime nwxXmlIOwxDateTime::FromString(const wxString &s)
{
  wxDateTime rtn;
  if(s.Find("T") == wxNOT_FOUND)
  {
    //  seconds since 1/1/1970
    time_t n = (time_t)StringTo::Uint(s);
    rtn.Set(n);
  }
  else
  {
    //  YYYY-MM-DDThh:mm:ss
    rtn = nwxString::UnFormatDateTimeXML(s);
  }
  return rtn;
}

bool nwxXmlIOwxDateTime::Skip(void *pObj)
{
  bool b = false;
  if(m_bSkipIfZero)
  {
    wxDateTime &x(CAST(pObj));
    b = !x.GetTicks();
  }
  return b;
}

#ifdef  _DEBUG
void nwxXmlIOwxDateTime::UnitTest()
{
  nwxXmlIOwxDateTime xx;
  wxString sTimeT("1230854460");
  wxString sXML("2009-01-02T00:01:00");
  wxString sTimeT2;
  wxString sXML2;
  wxDateTime dt;
  wxDateTime dtXML;
  dt = xx.FromString(sTimeT);
  dtXML = xx.FromString(sXML);
  sTimeT2 = xx.ToString((void *)&dt);
  xx.SetXML(true);
  sXML2 = xx.ToString((void *)&dtXML);
  if( (sTimeT != sTimeT2) ||
      (sXML != sXML2) ||
      (dt != dtXML) )
  {
    wxString sMsg("nwxXmlIOwxDateTime::UnitTest failed\n");
    sMsg.Append(nwxString::FormatDateTime(dt));
    sMsg.Append("\n");
    sMsg.Append(nwxString::FormatDateTime(dtXML));
    wxASSERT_MSG(0,sMsg);
  }
}
#endif


// ********************************************************
//
//  nwxXmlPersist
//

nwxXmlNodeList *nwxXmlPersist::CreateNodeList(
  const wxString &sNodeName, void *_pObj)
{
  //
  //  this function satisfies the pure virtual function
  //  defined in InwxXmlIO
  //
  nwxXmlPersist *pObj = (nwxXmlPersist *)_pObj;
  return pObj->CreateNodeList(sNodeName);
}

nwxXmlNodeList *nwxXmlPersist::CreateNodeList(const wxString &sNodeName)
{
  unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
  if(!Skip((void *)this))
  {
    unique_ptr<nwxXmlNodeList> apItems(m_xml.CreateNodeList());
    wxXmlNode *pParent = new wxXmlNode(wxXML_ELEMENT_NODE,sNodeName);
    m_xml.SetAttributes(pParent);
    apRtn->push_back(pParent);
    size_t nItems = apItems->size();
    wxXmlNode *pLastChild  = NULL;
    wxXmlNode *p;

    if(!m_sComment.IsEmpty())
    {
      pLastChild = new wxXmlNode(wxXML_COMMENT_NODE,wxT("comment"),m_sComment);      
      pParent->AddChild(pLastChild);
    }
    if(nItems)
    {
      size_t nBegin = 0;
      if(pLastChild == NULL)
      {
        pLastChild = apItems->at(0);
        pParent->AddChild(pLastChild);
        nBegin = 1;
      }
      for(size_t i = nBegin; i < nItems; i++)
      {
        p = apItems->at(i);
        pLastChild->SetNext(p);
        pLastChild = p;
      }
    }
  }
  return apRtn.release();
}

void nwxXmlPersist::SetComment(const wxString &s)
{
  m_sComment = s;
  m_sComment.Replace("-","_",true);
  m_sComment.Replace("&"," ",true);
  m_sComment.Replace("<"," ",true);
  m_sComment.Replace(">"," ",true);
}

bool nwxXmlPersist::LoadFromNode(wxXmlNode *pNode,void *_pObj)
{
  //
  //  this function satisfies the pure virtual function
  //  defined in InwxXmlIO
  //
  nwxXmlPersist *pObj = (nwxXmlPersist *)_pObj;

  return pObj->LoadFromNode(pNode);
}

bool nwxXmlPersist::LoadFromNode(wxXmlNode *pNode)
{
  return m_xml.Load(pNode);
}

void nwxXmlPersist::_SetFileName(const wxString &sFileName)
{
  if(m_sLastFileName != sFileName)
  {
    if((m_pLock != NULL) && !m_sLastFileName.IsEmpty())
    {
      m_pLock->Unlock(m_sLastFileName);
    }
    m_sLastFileName = sFileName;
  }
  if( (!m_sLastFileName.IsEmpty()) && 
      wxFileName::FileExists(m_sLastFileName) )
  {
    wxFileName fn(m_sLastFileName);
    m_dtFileModTime = fn.GetModificationTime();
  }
  else
  {
    m_dtFileModTime.Set((time_t)0);
  }
}
bool nwxXmlPersist::FileExists()
{
  // if file was loaded, check if the file still exists
  // b/c of possibility that file was removed from another process
  const wxString &s(GetLastFileName());
  bool bRtn = s.IsEmpty() ? false : wxFileName::FileExists(s);
  return bRtn;
}
bool nwxXmlPersist::CheckFileModification(bool bReload)
{
  bool bRtn = false;
  if(m_dtFileModTime.GetTicks() > 0)
  {
    wxFileName fn(GetLastFileName());
    bRtn = fn.FileExists() &&
      (fn.GetModificationTime() != m_dtFileModTime);
    if(bRtn && bReload)
    {
      bRtn = ReloadFile();
    }
  }
  return bRtn;
}


const wxString nwxXmlPersist::g_sRootNode("x");

const wxString nwxXmlPersist::g_sXMLNS("xmlns");
const wxString nwxXmlPersist::g_sXMLNSxsi("xmlns:xsi");
const wxString nwxXmlPersist::g_sXMLNSsql("xmlns:sql");
const wxString nwxXmlPersist::g_sXSIschemaLocation(
  "xsi:schemaLocation");

const wxString nwxXmlPersist::g_sXSIvalue(
  "http://www.w3.org/2001/XMLSchema-instance");
const wxString nwxXmlPersist::g_sSQLvalue(
  "urn:schemas-microsoft-com:mapping-schema");

// ********************************************************
//
//  nwxXmlContainer
//


void nwxXmlContainer::Init()
{
  if( (!m_nInitRecursion) && (!m_bNoInit) )
  {
    CIncrementer incr(m_nInitRecursion);
    PERSIST *pP;
    PERSISTstr *pPS;
    for(vector<PERSIST *>::iterator itr = m_list.begin();
      itr != m_list.end();
      ++itr)
    {
      pP = *itr;
      pP->pPersist->Init(pP->pObj);
    }
    for(vector<PERSISTstr *>::iterator itrs = m_listAttr.begin();
      itrs != m_listAttr.end();
      ++itrs)
    {
      pPS = *itrs;
      pPS->pPersist->Init(pPS->pObj);
    }
    if(m_pPersistText != NULL)
    {
      m_pPersistText->pPersist->Init(m_pPersistText->pObj);
    }
  }
}

bool nwxXmlContainer::Load(wxXmlNode *pNode)
{
  wxString sName;
  wxString sValue;
  wxString sText;
  wxXmlNode *pChild;
  wxXmlProperty *pAttr;
  mapPERSIST::iterator itr;
  mapPERSIST::iterator itrEnd = m_map.end();
  mapPERSISTstr::iterator itrs;
  mapPERSISTstr::iterator itrsEnd = m_mapAttr.end();
  bool bRtn = true;
  wxXmlNodeType nType;
  Init();
  CIncrementer incr(m_nInitRecursion);

  // Nodes

  for(pChild = pNode->GetChildren();
      bRtn && (pChild != NULL);
      pChild = pChild->GetNext())
  {
    nType = pChild->GetType();
    if(nType == wxXML_ELEMENT_NODE)
    {
      sName = pChild->GetName();
      itr = m_map.find(sName);
      if(itr != itrEnd)
      {
        PERSIST *pp(itr->second);
        bRtn = pp->pPersist->LoadFromNode(pChild,pp->pObj);
      }
    }
    else if(nType == wxXML_TEXT_NODE || nType == wxXML_CDATA_SECTION_NODE)
    {
      sText.Append(pChild->GetContent());
    }
  }

  itrsEnd = m_mapAttr.end();
  for(pAttr = pNode->GetProperties();
    bRtn && (pAttr != NULL);
    pAttr = pAttr->GetNext())
  {
    sName = pAttr->GetName();
    itrs = m_mapAttr.find(sName);
    if(itrs != itrsEnd)
    {
      sValue = pAttr->GetValue();
      PERSISTstr *pp(itrs->second);
      bRtn = pp->pPersist->LoadFromString(sValue,pp->pObj);
    }
  }

  if( (m_pPersistText != NULL) && (!sText.IsEmpty()) )
  {
    m_pPersistText->pPersist->LoadFromString(sText,m_pPersistText->pObj);
  }
  return bRtn;
}




bool nwxXmlContainer::Register(
      const wxString &sName, InwxXmlIO *pPersist, void *pObj)
{
  return TregisterNode<InwxXmlIO>::Register(sName, pPersist, pObj, &m_map, &m_list);
}

bool nwxXmlContainer::RegisterAttribute(
      const wxString &sName, InwxXmlIOstr *pPersist, void *pObj)
{
  return TregisterNode<InwxXmlIOstr>::Register(sName, pPersist, pObj, &m_mapAttr, &m_listAttr);
}

nwxXmlNodeList *nwxXmlContainer::CreateNodeList()
{
  unique_ptr<nwxXmlNodeList> apRtn(new nwxXmlNodeList);
  size_t nReserve(m_list.size() << 1);
  size_t nCount = 0;
  if(m_pPersistText != NULL)
  {
    nReserve++;
  }
  if(nReserve)
  {
    apRtn->reserve(nReserve);    
    if(m_pPersistText != NULL)
    {
      wxString sText;
      if( m_pPersistText->pPersist->CreateString(
              &sText,m_pPersistText->pObj) &&
          (!sText.IsEmpty())
        )
      {
        apRtn->push_back(new wxXmlNode(
          wxXML_TEXT_NODE,wxEmptyString,sText));
      }
    }
    for(vector<PERSIST *>::iterator itr = m_list.begin();
      itr != m_list.end();
      ++itr)
    {
      unique_ptr<nwxXmlNodeList> apList(
      (*itr)->pPersist->CreateNodeList((*itr)->sName,(*itr)->pObj));
      for(nwxXmlNodeList::iterator itrn = apList->begin();
        itrn != apList->end();
        ++itrn)
      {
        apRtn->push_back(*itrn);
        nCount++;
        if(nCount >= nReserve)
        {
          nReserve <<= 1;
          apRtn->reserve(nReserve);
        }
      }
    }
  }
  return apRtn.release();
}



#if USING_XML2_MODS
int nwxXmlContainer::SetupProperties(wxXmlNode *pNode)
{
  wxString sValue;
  PERSISTstr *pPERSIST;
  int nCount = 0;
  for(mapPERSISTstr::iterator itr = m_mapAttr.begin();
      itr != m_mapAttr.end();
      ++itr)
  {
    pPERSIST = itr->second;
    if(pPERSIST->pPersist->CreateString(&sValue,pPERSIST->pObj))
    {
      pNode->AddProperty(pPERSIST->sName,sValue);
      nCount++;
    }
  }
  return nCount;
}

void nwxXmlContainer::ClearAttributes(wxXmlNode *pNode)
{
  wxString sName;
  wxXmlProperty *pProp;
  for(pProp = pNode->GetProperties();
    pProp != NULL;
    pProp = pNode->GetProperties())
  {
    sName = pProp->GetName();
    pNode->DeleteProperty(sName);
  }
}
#else
void nwxXmlContainer::ClearAttributes(wxXmlNode *pNode)
{
  ClearProperties(pNode->GetProperties());
  pNode->SetProperties(NULL);
}
void nwxXmlContainer::ClearProperties(wxXmlProperty *pProp)
{
  if(pProp != NULL)
  {
    ClearProperties(pProp->GetNext());
    pProp->SetNext(NULL);
    delete pProp;
  }
}
wxXmlProperty *nwxXmlContainer::CreateProperties(int *pnCount)
{
  int nCount = 0;
  wxString sValue;
  wxXmlProperty *pFirst(NULL);
  wxXmlProperty *pLast(NULL);
  wxXmlProperty *pThis(NULL);
  PERSISTstr *pPERSIST;

  for(mapPERSISTstr::iterator itr = m_mapAttr.begin();
      itr != m_mapAttr.end();
      ++itr)
  {
    pPERSIST = itr->second;
    if(pPERSIST->pPersist->CreateString(&sValue,pPERSIST->pObj))
    {
      pThis = new wxXmlProperty(pPERSIST->sName,sValue);
      ++nCount;
      if(pLast == NULL)
      {
        // begin list
        pFirst = pThis;
        pLast = pThis;
      }
      else
      {
        // append list
        pLast->SetNext(pThis);
        pLast = pThis;
      }
    }
  }
  if(pnCount != NULL)
  {
    *pnCount = nCount;
  }
  return pFirst;
}
#endif


// UNIT TEST
#define NWX_XML_UNIT_TEST_CODE
#include "nwxXmlUnitTest.h"
