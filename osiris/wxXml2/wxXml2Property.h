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
*  FileName: wxXml2Property.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XML2_PROPERTY_H__
#define __WX_XML2_PROPERTY_H__

#include "wxXml2/wxXml2Object.h"
class wxXml2Node;
class WXDLLIMPEXP_BASE wxXml2Property : public wxXml2Object
{
public:
  friend class wxXml2Node;
  wxXml2Property();
  wxXml2Property(const wxString& name, const wxString& value, wxXml2Node *pNode = NULL);
  virtual ~wxXml2Property();
  wxString GetName() const;
  wxXml2Property* GetNext() const
  {
    return m_pNext;
  }
  wxString GetValue() const;
  void SetName(const wxString& name);
  //void SetNext(wxXml2Property *next);
  void SetValue(const wxString &value);
protected:
  wxXml2Property(wxXml2Node *pNode, xmlAttrPtr pAttr);
  void _RemoveNext();
  xmlAttrPtr _GetAttrPtr()
  {
    return m_pAttr;
  }
  xmlAttrPtr _CreateAttrPtr(wxXml2Node *pNode);
private:
  // m_sName and m_sValue are ignored if m_pAttr is not NULL
  mutable wxString m_sName;
  mutable wxString m_sValue;
  xmlAttrPtr m_pAttr;
  wxXml2Node *m_pNode;
  wxXml2Property *m_pNext;
  DECLARE_DYNAMIC_CLASS(wxXml2Property)
};


#endif
