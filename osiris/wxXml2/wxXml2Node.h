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
*  FileName: wxXml2Node.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XML2_NODE_H__
#define __WX_XML2_NODE_H__

#include "wxXml2/wxXml2Object.h"

typedef int wxXml2NodeType;
class wxXml2Property;

#define wxXML_ELEMENT_NODE          XML_ELEMENT_NODE
#define wxXML_ATTRIBUTE_NODE        XML_ATTRIBUTE_NODE
#define wxXML_TEXT_NODE             XML_TEXT_NODE
#define wxXML_CDATA_SECTION_NODE    XML_CDATA_SECTION_NODE
#define wxXML_ENTITY_REF_NODE       XML_ENTITY_REF_NODE
#define wxXML_ENTITY_NODE           XML_ENTITY_NODE
#define wxXML_PI_NODE               XML_PI_NODE
#define wxXML_COMMENT_NODE          XML_COMMENT_NODE
#define wxXML_DOCUMENT_NODE         XML_DOCUMENT_NODE
#define wxXML_DOCUMENT_TYPE_NODE    XML_DOCUMENT_TYPE_NODE
#define wxXML_DOCUMENT_FRAG_NODE    XML_DOCUMENT_FRAG_NODE
#define wxXML_NOTATION_NODE         XML_NOTATION_NODE
#define wxXML_HTML_DOCUMENT_NODE    XML_HTML_DOCUMENT_NODE

class WXDLLIMPEXP_BASE wxXml2Node : public wxXml2Object
{
public:
  friend class wxXml2Document;
  friend class wxXml2Property;

//wxXml2Node(
//  wxXml2Node *parent, 
//  wxXml2NodeType type, 
//  const wxString &name, 
//  const wxString &content = wxEmptyString, 
//  wxXml2Property *props = NULL, 
//  wxXml2Node *next = NULL);

//  wxXml2Node(const wxXml2Node &node);

  virtual ~wxXml2Node();
  wxXml2Node(
    wxXml2NodeType _type,
    const wxString& name,
    const wxString& content = wxEmptyString);
  

  void AddChild(wxXml2Node* child);
  void AddProperty(const wxString& name, const wxString& value);
  void AddProperty(wxXml2Property* prop);
  bool DeleteProperty(const wxString& name);
  wxXml2Node* GetChildren() const
  {
    return m_pFirstChild;
  }

  wxString GetContent() const;
  // int GetDepth(wxXml2Node *grandparent = NULL) const;

  //wxString GetNodeContent() const;
  wxString GetName() const;
  wxXml2Node *GetNext() const
  {
    return m_pNext;
  }
  wxXml2Node *GetParent() const
  {
    return m_pParent;
  }
  bool GetPropVal(const wxString& propName, wxString* value) const;
  wxString GetPropVal(const wxString &propName, const wxString &defaultVal) const;
  wxXml2Property *GetProperties() const
  {
    return m_pFirstProperty;
  }
  wxXml2NodeType GetType() const
  {
    return m_pNode->type;
  }
  bool HasProp(const wxString& propName) const;
  bool InsertChild(wxXml2Node* child, wxXml2Node* before_node);
  // bool IsWhitespaceOnly() const;
  bool RemoveChild(wxXml2Node* child);
  void SetChildren(wxXml2Node* child)
  {
    AddChild(child);
  }
  void SetContent(const wxString& con);
  void SetName(const wxString& name);
  void SetNext(wxXml2Node* next);
//  void SetParent(wxXml2Node* parent);
  // void SetProperties(wxXml2Property* prop);
  // void SetType(wxXml2NodeType type);
  // wxXml2Node& operator=(const wxXml2Node& node);

protected:
  wxXml2Property *_RemoveProperty(wxXml2Property *prop);
  wxXml2Property *_FindProperty(const wxString &sName);
  wxXml2Property *_FindProperty(xmlAttrPtr p);
  void _CleanupProperties();
  void _BuildProperties();
  void _RemoveNext();
private:
  // accessible from wxXml2Document
   xmlNodePtr GetNodePtr() const
   {
     return m_pNode;
   }
   wxXml2Node(xmlNodePtr pXmlNode); // used internally
   bool _CheckPropertyList(wxXml2Property *pSave = NULL);

private:
  xmlNodePtr m_pNode;
  wxXml2Node *m_pNext;
  wxXml2Node *m_pParent;
  wxXml2Node *m_pFirstChild;
  wxXml2Property *m_pFirstProperty;
  DECLARE_ABSTRACT_CLASS(wxXml2Node);

};

#endif
