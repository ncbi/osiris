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
*  FileName: wxXml2Document.cpp
*  Author:   Douglas Hoffman
*
*/

#include <wx/filename.h>
#include <wx/wfstream.h>
#include "wxXml2Document.h"
#include "wxXml2Node.h"
#include <libxml/parser.h>
#ifdef _WINDOWS
#include <memory.h>
#else
#include <string.h>
#endif
#ifdef ATTRIBUTE_PRINTF
#undef ATTRIBUTE_PRINTF
#endif
////// libxml2 callback functions

extern "C"
{

//  libmxl2 xmlInputReadCallback
//  see:  http://xmlsoft.org/html/libxml-xmlIO.html#xmlInputReadCallback
static int _fXmlInputReadCallback(void *context, char *buffer, int len)
{
  int nRtn = 0;
  wxInputStream *pStream((wxInputStream *)context);
  if(!pStream->Eof())
  {
    pStream->Read((void *)buffer, (size_t)len);
    nRtn = (int) pStream->LastRead();
  }
  return nRtn;
}

//  libxml2 xmlInputCloseCallback
//  see: http://xmlsoft.org/html/libxml-xmlIO.html#xmlInputCloseCallback
static int _fXmlInputCloseCallback(void *)
{
  return 0;
}

//  libxml2 xmlOutpuWriteCallback
//  see: http://xmlsoft.org/html/libxml-xmlIO.html#xmlOutputWriteCallback
static int _fXmlOutputWriteCallback(void *context, const char *buffer, int len)
{
  int nRtn = 0;
  wxOutputStream *pStream( (wxOutputStream *) context);
  pStream->Write((void *) buffer, (size_t)len);
  nRtn = (int) pStream->LastWrite();
  return nRtn;
}

//  libxml2 xmlOutpuCloseCallback
//  see: http://xmlsoft.org/html/libxml-xmlIO.html#xmlOutputCloseCallback
static int _fXmlOutputCloseCallback(void *context)
{
  wxOutputStream *pStream( (wxOutputStream *) context);
  bool b = pStream->Close();
  int nRtn = b ? 0 : -1;
  return nRtn;
}

} // extern "C"

////// END libxml2 callback functions
#if 0
class CxmlBuffer
{
public:
  CxmlBuffer()
  {
    m_pBuf = xmlBufferCreate();
  }
  ~CxmlBuffer()
  {
    xmlBufferFree(m_pBuf);
  }
  xmlBufferPtr Get()
  {
    return m_pBuf;
  }
private:
  xmlBufferPtr m_pBuf;
};
#endif

wxXml2Document::wxXml2Document() : 
  m_pXmlDoc(NULL), m_pNodeRoot(NULL)
{}

wxXml2Document::wxXml2Document(
  const wxString &filename,
  const wxString &encoding,
  int flags) : m_pXmlDoc(NULL), m_pNodeRoot(NULL)
{
  Load(filename,encoding,flags);
}

wxXml2Document::wxXml2Document(
  wxInputStream &stream,
  const wxString &encoding,
  int flags) : m_pXmlDoc(NULL), m_pNodeRoot(NULL)
{
  Load(stream,encoding,flags);
}

wxXml2Document::~wxXml2Document()
{
  _CleanupDoc();
}

wxXml2Node *wxXml2Document::GetRoot() const
{
  if( (m_pNodeRoot == NULL) && (m_pXmlDoc != NULL) )
  {
    m_pNodeRoot = new wxXml2Node(m_pXmlDoc->children);
  }
  return m_pNodeRoot;
}


bool wxXml2Document::Load(
  const wxString &filename, const wxString &encoding, int flags)
{
#if 1
  _CleanupDoc();
  int nOptions = 
    (flags == wxXMLDOC_KEEP_WHITESPACE_NODES) 
    ? 0
    : XML_PARSE_NOBLANKS;
  if(wxFileName::IsFileReadable(filename))
  {
#ifdef __WXMSW__
    wxString sFileName(filename);
    sFileName.Replace("\\","/",true);
    if(sFileName.StartsWith("//"))
    {
      sFileName.Prepend("file:///");
    }
    m_pXmlDoc = xmlReadFile(sFileName.utf8_str(),encoding.utf8_str(),nOptions);
#else
    m_pXmlDoc = xmlReadFile(filename.utf8_str(),encoding.utf8_str(),nOptions);
#endif
  }
  bool bRtn = m_pXmlDoc != NULL;
  if(bRtn)
  {
    _BuildTree();
  }

  return bRtn;
#else
  wxFileInputStream fs(filename);
  bool bOK = fs.IsOk();
  if(bOK)
  {
    bOK = Load(fs,encoding,flags);
  }
  return bOK;
#endif
}

bool wxXml2Document::Load(
  wxInputStream &stream, const wxString &encoding, int flags)
{
  _CleanupDoc();
  int nOptions = 
    (flags == wxXMLDOC_KEEP_WHITESPACE_NODES) 
    ? 0
    : XML_PARSE_NOBLANKS;

  m_pXmlDoc = xmlReadIO(
    _fXmlInputReadCallback,
    _fXmlInputCloseCallback,
    (void *)&stream,
    ".",
    (const char *) encoding.utf8_str(),
    nOptions);
  bool bRtn = m_pXmlDoc != NULL;
  if(bRtn)
  {
    _BuildTree();
  }
  return bRtn;
}

bool wxXml2Document::Save(
  const wxString &filename, int indentstep) const
{
  bool bOK = m_pXmlDoc != NULL;
  if(bOK)
  {
#if 0
    int nHold = xmlKeepBlanksDefault(0);
    int nBytes = xmlSaveFormatFileEnc(filename.utf8_str(),m_pXmlDoc,"UTF-8",!!indentstep);
    bOK = (nBytes > 0);
    if(nHold)
    {
      xmlKeepBlanksDefault(nHold);
    }
#else
    wxTempFileOutputStream fs(filename);
    bOK = fs.IsOk();
    if(bOK)
    {
      bOK = Save(fs,indentstep);
    }
#endif
  }
  return bOK;
}
bool wxXml2Document::Save(
  wxOutputStream &stream, int indentstep) const
{
  bool bOK = m_pXmlDoc != NULL;
  if(bOK)
  {
    int nRtn = -1;
    bOK = stream.IsOk();
    if(bOK)
    {
      int nHoldIndent = xmlIndentTreeOutput;
      xmlIndentTreeOutput = 1;
      int nHold = xmlKeepBlanksDefault(0);
//      xmlCharEncodingHandlerPtr pEnc =
//        xmlGetCharEncodingHandler(XML_CHAR_ENCODING_UTF8);
      xmlOutputBufferPtr pXmlBuf = 
        xmlOutputBufferCreateIO(
        _fXmlOutputWriteCallback,
        _fXmlOutputCloseCallback,
        (void *) &stream,
        NULL);
      nRtn = xmlSaveFormatFileTo(
        pXmlBuf,
        m_pXmlDoc,
        "UTF-8",
        !!indentstep);
      bOK = nRtn > 0;

      if(nHold)
      {
        xmlKeepBlanksDefault(nHold);
      }

      xmlIndentTreeOutput = nHoldIndent;
    }
  }
  return bOK;
}
void wxXml2Document::SetRoot(wxXml2Node *pNode)
{
  _CleanupDoc();
  m_pXmlDoc = xmlNewDoc((const xmlChar *)"1.0");
  if(m_pXmlDoc != NULL)
  {
    m_pNodeRoot = pNode;
    xmlDocSetRootElement(m_pXmlDoc,pNode->GetNodePtr());
  }
}
void wxXml2Document::_CleanupRoot()
{
  if(m_pNodeRoot != NULL)
  {
    m_pNodeRoot->m_pNode = NULL; 
    // do not delete node
    // let xmlFreeDoc() take care of it
    delete m_pNodeRoot;
    m_pNodeRoot = NULL;
  }
}
void wxXml2Document::_CleanupDoc()
{
  _CleanupRoot();
  if(m_pXmlDoc != NULL)
  {
#ifndef NO_FREE
    xmlFreeDoc(m_pXmlDoc);
#endif
    m_pXmlDoc = NULL;
  }
}
void wxXml2Document::_BuildTree(xmlNodePtr pXmlNode, wxXml2Node *pNodeParent)
{
  xmlNodePtr pChild;
  wxXml2Node *pNode;
  wxXml2Node *pNodePrev(NULL);
  for(pChild = pXmlNode->children; pChild != NULL; pChild = pChild->next)
  {
    pNode = new wxXml2Node(pChild);
    if(pNodePrev == NULL)
    {
      pNodeParent->SetChildren(pNode);
    }
    else
    {
      pNode->m_pParent = pNodePrev->m_pParent;
      pNodePrev->SetNext(pNode);
    }
    pNodePrev = pNode;
    if(pChild->type == XML_ELEMENT_NODE)
    {
      _BuildTree(pChild,pNode);
    }
  }
}

void wxXml2Document::_BuildTree()
{
  if( (m_pXmlDoc != NULL) && (m_pNodeRoot == NULL) )
  {
    xmlNodePtr pNode;
    for(pNode = m_pXmlDoc->children; 
        (pNode != NULL) && (pNode->type != XML_ELEMENT_NODE);
        pNode = pNode->next)
    {;}
    if(pNode != NULL)
    {
      m_pNodeRoot = new wxXml2Node(pNode);
      _BuildTree(pNode,m_pNodeRoot);
    }
  }
}

IMPLEMENT_ABSTRACT_CLASS(wxXml2Document,wxXml2Object)
