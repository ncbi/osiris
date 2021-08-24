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
*  FileName: wxXml2Document.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XML2_DOCUMENT_H__
#define __WX_XML2_DOCUMENT_H__

#include <wx/stream.h>
#include "wxXml2/wxXml2Object.h"

#define wxXMLDOC_KEEP_WHITESPACE_NODES 1 
#define wxXMLDOC_NONE 0

class wxXml2Node;

class WXDLLIMPEXP_BASE wxXml2Document : public wxXml2Object
{
public:
  friend class wxXslObject;
  friend class wxXslSheet;
  wxXml2Document();
  wxXml2Document(
    const wxString& filename,
    const wxString& encoding = wxT("UTF-8"),
    int flags = wxXMLDOC_NONE
    );
  // Loads the given filename using the given encoding. See Load.

  wxXml2Document(
    wxInputStream& stream, 
    const wxString& encoding = wxT("UTF-8"),
    int flags = wxXMLDOC_NONE
    );
  // Loads the XML document from given stream using the given encoding. See Load.

  // wxXml2Document(const wxXml2Document& doc);   NOT IMPLEMENTED
  // Copy constructor. Deep copies all the XML tree of the given document.

  virtual ~wxXml2Document();

  // wxXml2Node* DetachRoot();
  // wxString GetEncoding() const;
  // wxString GetFileEncoding() const;

  wxXml2Node* GetRoot() const;
  //wxString GetVersion() const;
  bool IsOk() const
  {
    return(m_pXmlDoc != NULL);
  }

  bool Load(
    const wxString& filename, 
    const wxString& encoding = wxT("UTF-8"), 
    int flags = wxXMLDOC_NONE);

  bool Load(
    wxInputStream& stream, 
    const wxString& encoding = wxT("UTF-8"), 
    int flags = wxXMLDOC_NONE);

  bool Save(const wxString& filename, int indentstep = 1) const;
  bool Save(wxOutputStream& stream, int indentstep = 1) const;
  bool CloseAfterSave(bool b)
  {
    bool rtn = m_bCloseAfterSave;
    m_bCloseAfterSave = b;
    return rtn;
  }
  bool CloseAfterSave() const
  {
    return m_bCloseAfterSave;
  }

  // void SetEncoding(const wxString& enc);

  void SetRoot(wxXml2Node* node);
  //void SetVersion(const wxString& version);
  //wxXml2Document& operator operator=(const wxXml2Document& doc);
protected:
  void SetDocPtr(xmlDocPtr p)
  {
    if(p != NULL)
    {
      _CleanupDoc();
      m_pXmlDoc = p;
      _BuildTree();
    }
  }
  xmlDocPtr GetDocPtr()
  {
    return m_pXmlDoc;
  }
  xmlDocPtr AcquireDocPtr()
  {
    xmlDocPtr pRtn = m_pXmlDoc;
    m_pXmlDoc = NULL;
    _CleanupRoot();
    return pRtn;
  }
private:
  void _CleanupDoc();
  void _CleanupRoot();
  void _BuildTree();
  static void _BuildTree(xmlNodePtr pXmlNode, wxXml2Node *pNode);
  xmlDocPtr m_pXmlDoc;
  mutable wxXml2Node *m_pNodeRoot;
  bool m_bCloseAfterSave;
  DECLARE_ABSTRACT_CLASS(wxXml2Document)
};
/*
-- not needed

class wxXml2CloseAfterSave
{
public:
  wxXml2CloseAfterSave(wxXml2Document *pDoc, bool bClose):
    m_pDoc(pDoc)

  {
    m_bState = m_pDoc->CloseAfterSave(bClose);
  }
  virtual ~wxXml2CloseAfterSave()
  {
    m_pDoc->CloseAfterSave(m_bState);
  }
private:
  wxXml2Document *m_pDoc;
  bool m_bState;
};
*/

#endif


