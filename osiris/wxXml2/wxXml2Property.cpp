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
*  FileName: wxXml2Property.cpp
*  Author:   Douglas Hoffman
*
*/

#include "wxXml2Property.h"
#include "wxXml2Node.h"
#include "WXCAST.h"
#include "wxXml2Ptr.h"

wxXml2Property::wxXml2Property() 
  : m_pAttr(NULL), m_pNode(NULL), m_pNext(NULL)
{}

wxXml2Property::wxXml2Property(
  const wxString &name, const wxString &value, wxXml2Node *pNode) :
    m_sName(name),
    m_sValue(value),
    m_pAttr(NULL),
    m_pNode(pNode),
    m_pNext(NULL)
{
  if(pNode != NULL)
  {
    pNode->DeleteProperty(name);
    pNode->AddProperty(this);
  }
}
wxXml2Property::wxXml2Property(wxXml2Node *pNode, xmlAttrPtr pAttr) :
  m_pAttr(pAttr),
  m_pNode(pNode),
  m_pNext(NULL)
{}

wxXml2Property::~wxXml2Property()
{
  if(m_pNext != NULL)
  {
    delete m_pNext;
    m_pNext = NULL;
  }
}

wxString wxXml2Property::GetName() const
{
  if(m_pAttr != NULL)
  {
    m_sName = WX_CAST(m_pAttr->name);
  }
  return m_sName;
}
wxString wxXml2Property::GetValue() const
{
  if(m_pAttr != NULL)
  {
    wxXml2Ptr pValue(xmlGetProp(m_pAttr->parent,m_pAttr->name));
    // the preceeding isn't particularly efficient
    // but is cleaner than digging into the 'struct'ure
    if(pValue.IsNull())
    {
      m_sValue.Empty();
    }
    else
    {
      m_sValue = WX_CAST(pValue);
    }
  }
  return m_sValue;
}

void wxXml2Property::SetName(const wxString &name)
{
  if(m_pNode == NULL)
  {
    m_sName = name;
  }
  else 
  {
    GetValue();
    m_pNode->_RemoveProperty(this);
    m_sName = name;
    m_pNode->AddProperty(this);
  }
}
void wxXml2Property::SetValue(const wxString &value)
{
  if(m_pNode == NULL)
  {
    m_sValue = value;
  }
  else
  {
    GetName();
    m_pNode->_RemoveProperty(this);
    m_sValue = value;
    m_pNode->AddProperty(this);
  }
}

IMPLEMENT_ABSTRACT_CLASS(wxXml2Property,wxXml2Object)
