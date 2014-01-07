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
*  FileName: wxXml2Node.cpp
*  Author:   Douglas Hoffman
*
*/

#include <wx/string.h>
#include <wx/debug.h>
#include <wx/log.h>
#include <vector>
#include "wxXml2Node.h" 
#include "wxXml2Property.h"
#ifdef ATTRIBUTE_PRINTF
#undef ATTRIBUTE_PRINTF
#endif

#include <libxml/xmlmemory.h>
#include <libxml/entities.h>
#include "WXCAST.h"
#include "wxXml2Ptr.h"

inline const xmlChar *XML_CAST(const wxString &s)
{
  return (const xmlChar *)(s.c_str());
}
using namespace std;

wxXml2Node::~wxXml2Node()
{
  xmlNodePtr pNode = m_pNode;
  m_pNode = NULL;

  if( (m_pParent != NULL) && (m_pParent->m_pFirstChild != NULL) )
  {
    // this should have been done already, but
    // is not required
    m_pParent->RemoveChild(this);
  }
#ifndef NO_FREE
  if( (pNode != NULL) && (pNode->parent == NULL) )
  {
    xmlFreeNode(pNode);
  }
#endif
  if(m_pFirstChild != NULL)
  {
    wxXml2Node *pChild = m_pFirstChild;
    m_pFirstChild = NULL; 
    pChild->m_pParent = NULL;
    // set m_pFirstChild to NULL in order to
    // prevent problem in pChild->m_pParent->RemoveChild();
    // this would be VERY SLOW if RemoveChild was called 
    // for entire node tree

    if(pNode == NULL)
    {
      // if m_pNode was null, then
      // set to null for child, etc.
      pChild->m_pNode = NULL;
    }
    delete pChild;
  }
  // remove siblings if not done already
  // doing this recursively can have a stack overflow
  wxXml2Node *pNext;
  wxXml2Node *pThis;
  for(pThis = m_pNext; 
    pThis != NULL;
    pThis = pNext)
  {
    pNext = pThis->m_pNext;
    pThis->m_pNext = NULL; // prevent multiple 'delete'
    if(pNode == NULL)
    {
      // if m_pNode for this node was NULL, then
      // set it to null for sibs
      pThis->m_pNode = NULL;
    }
    delete pThis;
  }
  if(m_pFirstProperty != NULL)
  {
    delete m_pFirstProperty;
  }
}

wxXml2Node::wxXml2Node(xmlNodePtr pNode) :
  m_pNode(pNode),
  m_pNext(NULL),
  m_pParent(NULL),
  m_pFirstChild(NULL),
  m_pFirstProperty(NULL)
{
  _BuildProperties();
}

wxXml2Node::wxXml2Node(
  wxXml2NodeType _type,
  const wxString& name,
  const wxString& content) :
    m_pNode(NULL),
    m_pNext(NULL),
    m_pParent(NULL),
    m_pFirstChild(NULL),
    m_pFirstProperty(NULL)
{
  switch(_type)
  {
    case wxXML_ELEMENT_NODE:
      if(content.IsEmpty())
      {
        m_pNode = xmlNewNode(NULL, XML_CAST(name));
      }
      else
      {
        m_pNode = xmlNewChild(NULL,NULL,
          XML_CAST(name),
          XML_CAST(content)
          );
      }
      break;
    case wxXML_TEXT_NODE:
      m_pNode = xmlNewText(XML_CAST(content));
      break;
    case wxXML_COMMENT_NODE:
      m_pNode = xmlNewComment(XML_CAST(content));
      break;
    default:
      {
        wxString s = wxString::Format(
          _T("wxXml2Node constructor, unsupported type %d"),
          (int)_type);
        wxASSERT_MSG(0,s);
        wxLog::OnLog(wxLOG_Message,(const wxChar *)s,time(NULL));
      }
      break;
  }
}

void wxXml2Node::AddChild(wxXml2Node *child)
{
  while(m_pFirstChild != NULL)
  {
    wxXml2Node *pFC = m_pFirstChild;
    RemoveChild(pFC);
    delete pFC;
  }
  m_pFirstChild = child;
  child->m_pParent = this;
  if(child->m_pNode->parent != m_pNode)
  {
    xmlUnlinkNode(child->m_pNode);
    xmlNodePtr pNode = xmlAddChild(m_pNode,child->m_pNode);
    if(pNode != child->m_pNode)
    {
      m_pNode = pNode;
    }
  }
}

void wxXml2Node::AddProperty(const wxString &name, const wxString &value)
{
  wxXml2Property *pProp = new wxXml2Property(name,value); 
  AddProperty(pProp);
}
void wxXml2Node::AddProperty(wxXml2Property *prop)
{
  if(prop->m_pNode == this) {} // do nothing
  else if( prop->m_pNode != NULL )
  {
    wxString s = _T("wxXml2Node::AddProperty(wxXml2Property *) - " 
      "property used on another node - "
      "creating new property with name/value");
    wxLog::OnLog(wxLOG_Message,(const wxChar *)s,time(NULL));
    wxASSERT_MSG(0,s);
    AddProperty(prop->GetName(),prop->GetValue());
  }
  else
  {
    wxString sName = prop->GetName();
    wxString sValue = prop->GetValue();
    wxString sOldValue;
    bool bDone = false;

    if( GetPropVal(sName,&sOldValue) )
    {
      if(sOldValue == sValue)
      {
        bDone = true;
      }
      else
      {
        DeleteProperty(sName);
      }
    }
    if(!bDone)
    {
      xmlAttrPtr ptr = xmlSetProp(m_pNode,XML_CAST(sName),XML_CAST(sValue));
      xmlAttrPtr ptrPrev = ptr->prev;
      xmlAttrPtr ptrNext = ptr->next;
#ifdef _DEBUG
      wxXml2Property *pOldProperty = _FindProperty(ptr); // should be null
      wxASSERT_MSG(pOldProperty == NULL,
      "wxXml2Node::AddProperty _FindProperty(ptr) returned non null");
#endif
      wxXml2Property *pPrev = _FindProperty(ptrPrev);
      wxXml2Property *pNext = _FindProperty(ptrNext);
      wxXml2Property *pSave = NULL;
      if(pPrev != NULL)
      {
        if(pPrev->m_pNext != pNext)
        {
          pSave = pNext;
        }
        pPrev->m_pNext = prop;
      }
      else
      {
        if(this->m_pFirstProperty != pNext)
        {
          pSave = m_pFirstProperty;
        }
        this->m_pFirstProperty = prop;
      }
      prop->m_pNext = pNext;
      prop->m_pNode = this;
      if(prop->m_pAttr != ptr)
      {
        // swap property
        if( (prop->m_pAttr != NULL) &&
            (prop->m_pAttr->parent == NULL) )
        {
#ifndef NO_FREE
          xmlFreeProp(prop->m_pAttr);  // COULD BE TROUBLE
#endif
        }
        prop->m_pAttr = ptr;
      }
      _CheckPropertyList(pSave);
    }
  }
}

bool wxXml2Node::DeleteProperty(const wxString &name)
{
  wxXml2Property *pProp = _FindProperty(name);
  bool bRtn = (pProp != NULL);
  if(bRtn)
  {
    bool bProblem = false;
    xmlAttrPtr pxmlPrev = pProp->m_pAttr->prev;
    xmlAttrPtr pxmlNext = pProp->m_pAttr->next;
    wxXml2Property *pPrev = _FindProperty(pxmlPrev);
    wxXml2Property *pNext = _FindProperty(pxmlNext);

    if( (pxmlPrev == NULL) != (pPrev == NULL) )
    {
      bProblem = true;
    }
    if( (pxmlNext == NULL) != (pNext == NULL) )
    {
      bProblem = true;
    }
    if(pNext != pProp->m_pNext)
    {
      bProblem = true;
    }
    xmlRemoveProp(pProp->m_pAttr);
    if(pxmlPrev == NULL)
    {
      if(m_pFirstProperty != pProp)
      {
        bProblem = true;
      }
      else
      {
        m_pFirstProperty = pNext;
      }
    }
    else if(pxmlPrev->next != pxmlNext)
    {
      bProblem = true;
    }
    else if(pPrev->m_pNext != pProp)
    {
      bProblem = true;
    }
    else
    {
      pPrev->m_pNext = pNext;
    }
    if(bProblem)
    {
      _CheckPropertyList(pProp);
    }
    else
    {
      pProp->m_pNext = NULL; // destructor will otherwise
                             // remove following attributes
      delete pProp;
    }
  }
  return bRtn;
}

wxString wxXml2Node::GetContent() const
{
  wxString sRtn;
  wxXml2NodeType nType = GetType();
  if( (nType == wxXML_TEXT_NODE) || 
      (nType == wxXML_CDATA_SECTION_NODE) )
  {
    const char *pc = (const char *)(m_pNode->content);
    sRtn = wxT(pc);
  }
  return sRtn;
}
wxString wxXml2Node::GetName() const
{
  wxString sRtn;
  if(GetType() == wxXML_ELEMENT_NODE)
  {
    const char *p = (const char *)m_pNode->name;
    sRtn = wxT(p);
  }
  return sRtn;
}

bool wxXml2Node::GetPropVal(
  const wxString &propName, wxString *value) const
{
  wxXml2Ptr pValue(xmlGetProp(m_pNode,XML_CAST(propName)));
  bool bRtn = pValue.Get() != NULL;
  if(bRtn)
  {
    *value = WX_CAST(pValue);
  }
  return bRtn;
}
wxString wxXml2Node::GetPropVal(
  const wxString &propName,const wxString &defaultVal) const
{
  wxString sRtn;
  bool bFound = GetPropVal(propName,&sRtn);
  if(!bFound)
  {
    sRtn = defaultVal;
  }
  return sRtn;
}

bool wxXml2Node::HasProp(const wxString &propName) const
{
  xmlAttrPtr ptr = xmlHasProp(m_pNode,XML_CAST(propName));
  return(ptr != NULL);
}
bool wxXml2Node::InsertChild(wxXml2Node *child, wxXml2Node *before)
{
  // if 'child' is merged with an adjacent cell 
  // (because they are both text) then 'child' will be
  // 'delete'd
  bool bRtn = true;
  if(child->m_pParent != NULL)
  {
    child->m_pParent->RemoveChild(child);
  }
  else if(child->m_pNode->doc != NULL)
  {
    wxASSERT_MSG(0,
      "wxXml2Node::InsertChild -- attempting to insert\n"
      "a top level node from another document");
    bRtn = false;
  }

  if(bRtn)
  {
    xmlNodePtr ptr = NULL;
    // find before node
    if(before == NULL)
    {
      // this will be the first node
      if(m_pFirstChild == NULL)
      {
        // this will be the first and only child
        xmlAddChild(m_pNode, child->m_pNode);
        m_pFirstChild = child;
      }
      else
      {
        ptr = xmlAddPrevSibling(
          m_pFirstChild->m_pNode,
          child->m_pNode
          );
        if(ptr == NULL)
        {
          wxASSERT_MSG(0,
            _T("wxXml2Node::InsertChild() "
            "failed in xmlAddPrevSibling()"));
          bRtn = false;
        }
        else if(ptr == m_pFirstChild->m_pNode)
        {
          // we merged two text cells, kill child
          child->m_pNode = NULL;
          delete child;
          child = NULL;
        }
        else
        {
          // place child at the beginning of the list
          child->m_pNext = m_pFirstChild;
          m_pFirstChild = child;
        }
      }
    }
    else if(before->m_pParent != this)
    {
      wxASSERT_MSG(
        0, "wxXml2Node::InsertChild -- 'before'\n"
           "is not a child of current node");
    }
    else
    {
      wxXml2Node *pNext = before->m_pNext;
      xmlNodePtr ptr = xmlAddNextSibling(
        before->m_pNode,child->m_pNode);
      if (ptr == NULL)
      {
          wxASSERT_MSG(0,
            _T("wxXml2Node::InsertChild() "
            "failed in xmlAddNextSibling()"));
          bRtn = false;
      }
      else if(ptr == child->m_pNode)
      {
        // insert child into list
        child->m_pNext = pNext;
        before->m_pNext = child;
      }
      else
      {
        // we merged two text cells,
        // kill child
        child->m_pNode = NULL;
        delete child;
        child = NULL;
      }
    }
    if(bRtn && (child != NULL))
    {
      child->m_pParent = this;
    }
  }  
  return bRtn;
}


bool wxXml2Node::RemoveChild(wxXml2Node *child)
{
  bool bRtn = false;
  if(child->m_pParent == this)
  {
    wxXml2Node *prev = NULL;
    wxXml2Node *find;
    for(find = m_pFirstChild;
      (find != NULL) && (find != child);
      find = find->m_pNext)
    {
      prev = find;
    }
    if(find != NULL)
    {
      if(prev != NULL)
      {
        prev->m_pNext = child->m_pNext;
      }
      else
      {
        m_pFirstChild = child->m_pNext;
      }
      child->m_pParent = NULL;
      child->m_pNext = NULL;
      xmlUnlinkNode(child->m_pNode);
      bRtn = true;
    }
  }
  return bRtn;
}

void wxXml2Node::SetContent(const wxString &content)
{
  int nType = GetType();
  if( (nType == wxXML_TEXT_NODE) ||
      (nType == wxXML_CDATA_SECTION_NODE) )
  {
    wxXml2Ptr pStr(xmlEncodeEntitiesReentrant(
      m_pNode->doc,XML_CAST(content)));
    wxXml2Ptr pStr2(xmlEncodeSpecialChars(
      m_pNode->doc,pStr));
    xmlNodeSetContent(m_pNode,pStr2);
  }
}

void wxXml2Node::SetName(const wxString &name)
{
  xmlNodeSetName(m_pNode,XML_CAST(name));
}

void wxXml2Node::_RemoveNext()
{
  if(m_pNext != NULL)
  {
    m_pNext->_RemoveNext();
    xmlUnlinkNode(m_pNext->m_pNode);
  }
}
void wxXml2Node::SetNext(wxXml2Node *next)
{
  xmlNodePtr ptr(NULL);
  _RemoveNext();
  m_pNext = NULL;
  if(m_pNode->next != next->m_pNode)
  {
    ptr = xmlAddNextSibling(m_pNode,next->m_pNode);
  }
  else
  {
    ptr = next->m_pNode;
  }
  if(ptr == m_pNode)
  {
    // two text cells merged
    next->m_pNode = NULL;
    delete next;
    m_pNext = NULL;
  }
  else if(ptr == next->m_pNode)
  {
    m_pNext = next;
  }
  else
  {
    wxString s(_T("wxXml2Node::SetNext() failed - " 
      "xmlAddNextSibling returned "));
    if(ptr == NULL)
    {
      s.Append(_T("NULL pointer"));
    }
    else
    {
      s.Append(_T("unknown pointer"));
    }
    wxASSERT_MSG(0,s);
    m_pNext = NULL;
  }
}

wxXml2Property *wxXml2Node::_RemoveProperty(wxXml2Property *prop)
{
  wxXml2Property *pRtn = NULL;
  wxXml2Property *pPrev = NULL;
  for(pRtn = m_pFirstProperty;
    (pRtn != NULL) && (pRtn != prop);
    pRtn = pRtn->m_pNext)
  {
    pPrev = pRtn;
  }
  if(pRtn != NULL)
  {
    // remove property
    xmlRemoveProp(pRtn->m_pAttr);
    pRtn->m_pAttr = NULL;
    pRtn->m_pNode = NULL;
    // remove from linked list
    if(pPrev == NULL)
    {
      m_pFirstProperty = pRtn->m_pNext;
    }
    else
    {
      pPrev->m_pNext = pRtn->m_pNext;
    }
    pRtn->m_pNext = NULL;
    _CheckPropertyList();
  }
  return pRtn;
}
wxXml2Property *wxXml2Node::_FindProperty(xmlAttrPtr p)
{
  wxXml2Property *pRtn = NULL;
  if(p != NULL)
  {
    for(pRtn = m_pFirstProperty;
      (pRtn != NULL) && (pRtn->m_pAttr != p);
      pRtn = pRtn->m_pNext)
    { }
  }
  return pRtn;
}

wxXml2Property *wxXml2Node::_FindProperty(const wxString &sName)
{
  wxXml2Property *pRtn = NULL;
  for(pRtn = m_pFirstProperty;
    (pRtn != NULL) && (pRtn->GetName() != sName);
    pRtn = pRtn->m_pNext)
  { }
  return pRtn;
}

void wxXml2Node::_CleanupProperties()
{
  if(m_pFirstProperty != NULL)
  {
    delete m_pFirstProperty;
    m_pFirstProperty = NULL;
  }
}

void wxXml2Node::_BuildProperties()
{
  wxXml2Property *pPrev = NULL;
  wxXml2Property *pProp;
  xmlAttrPtr pxmlProp;

  _CleanupProperties();
  for(pxmlProp = m_pNode->properties;
    pxmlProp != NULL;
    pxmlProp = pxmlProp->next)
  {
    pProp = new wxXml2Property(this,pxmlProp);
    if(pPrev == NULL)
    {
      m_pFirstProperty = pProp;
    }
    else
    {
      pPrev->m_pNext = pProp;
    }
    pPrev = pProp;
  }
}

bool wxXml2Node::_CheckPropertyList(wxXml2Property *pSave)
{
  bool bFound = (pSave == NULL);
  bool bFoundXML = false;
  bool bOK = true;
  wxXml2Property *pProp;
  wxXml2Property *pPrev(NULL);
  for(pProp = m_pFirstProperty;
    pProp != NULL;
    pProp = pProp->m_pNext)
  {
    if(bFound)
    {}
    else if(pProp == pSave)
    {
      bFound = true;
    }
    else if(pProp->m_pAttr == pSave->m_pAttr)
    {
      bFoundXML = true;
    }
    if(pPrev == NULL)
    {
      if(m_pNode->properties != pProp->m_pAttr)
      {
        bOK = false;
      }
    }
    else
    {
      if(pPrev->m_pAttr->next != pProp->m_pAttr)
      {
        bOK = false;
      }
    }
    pPrev = pProp;
  }
  if(!bFound)
  {
    wxString s(_T(
      "wxXml2Node::_CheckPropertyList(pSave) "
      "did not find property pSave"));
    
    if(bFoundXML)
    {
      pSave->m_pAttr = NULL;
    }
    else
    {
      s.Append(_T(", xmlAttrPtr not found either"));
    }
    pSave->m_pNext = NULL;
    delete pSave;
    wxLog::OnLog(wxLOG_Message,(const wxChar *)s,time(NULL));
    wxASSERT_MSG(0,s);
  }
  if(!bOK)
  {
    wxString s(_T("wxXml2Node::_CheckPropertyList bOK == false"));
    wxASSERT_MSG(0,s);
    wxLog::OnLog(wxLOG_Message,(const wxChar *)s,time(NULL));
    _BuildProperties();
  }
  return bOK;
}
IMPLEMENT_ABSTRACT_CLASS(wxXml2Node,wxXml2Object)
