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
*  FileName: wxXslSheet.cpp
*  Author:   Douglas Hoffman
*
*/
#include "wxXslSheet.h"
#include "wxXml2/wxXml2Document.h"
#include "nwx/nwxString.h"
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <stdlib.h>


wxXslSheet::wxXslSheet() : 
  m_pSheet(NULL), m_bParmSheetOK(false), m_bFileURL(false)
{
}

wxXslSheet::wxXslSheet(const wxChar *psURL) :
  m_pSheet(NULL), m_bParmSheetOK(false), m_bFileURL(false)
{
  Load(psURL);
}

wxXslSheet::wxXslSheet(wxInputStream &str) : 
  m_pSheet(NULL), m_bParmSheetOK(false), m_bFileURL(false)
{
  Load(str);
}

wxXslSheet::~wxXslSheet()
{
  _CleanupSheet();
}

void wxXslSheet::_CleanupSheet()
{
  ClearSheetParms();
  if(m_pSheet != NULL)
  {
    xsltFreeStylesheet(m_pSheet);
    m_pSheet = NULL;
  }
}
bool wxXslSheet::Load()
{
  _CleanupSheet();
  ClearParms();

  // memory leak in xsltParseStylesheet[doc|file]

  m_bFileURL = false;
  m_dtFile.Set((time_t)0);
  if(!m_sURL.IsEmpty())
  {
    wxXml2Document docSheet(m_sURL);
    xmlDocPtr pDocSheet = docSheet.AcquireDocPtr();
    wxFileName fn(m_sURL);
    if(fn.FileExists())
    {
      m_dtFile = fn.GetModificationTime();
      m_bFileURL = true;
    }
    if(pDocSheet != NULL)
    {
      m_pSheet = xsltParseStylesheetDoc(pDocSheet);
      if(m_pSheet == NULL)
      {
        xmlFreeDoc(pDocSheet);
      }
      else
      {
        _LoadSheetParms();
      }
    }
  }
  return IsOK();
}

bool wxXslSheet::Load(wxInputStream &str)
{
  _CleanupSheet();
  ClearParms();

  // memory leak in xsltParseStylesheet[doc|file]

  m_bFileURL = false;
  m_dtFile.Set((time_t)0);
  wxXml2Document docSheet(str);
  if(docSheet.IsOk())
  {
    xmlDocPtr pDocSheet = docSheet.AcquireDocPtr();
    if(pDocSheet != NULL)
    {
      m_pSheet = xsltParseStylesheetDoc(pDocSheet);
      if(m_pSheet == NULL)
      {
        xmlFreeDoc(pDocSheet);
      }
      else
      {
        _LoadSheetParms();
      }
    }
  }
  return IsOK();
}

bool wxXslSheet::CheckReload()
{
  bool bRtn = false;
  if(m_sURL.IsEmpty()) 
  {}
  else if(m_bFileURL || !IsOK())
  {
    wxFileName fn(m_sURL);
    wxDateTime dt((time_t)0);
    if(fn.FileExists())
    {
      dt = fn.GetModificationTime();
    }
    if(dt != m_dtFile)
    {
      wxString s(m_sURL);
      Load(s);
      bRtn = true;
    }
  }
  return bRtn;
}
void wxXslSheet::_LoadSheetParms()
{
  xsltStylesheetPtr pSheet = GetSheetParam(); // from wxXslObject
  ClearSheetParms();
  if(pSheet != NULL && m_pSheet != NULL)
  {
    const char *px[1] = { NULL };
    // extract <xsl:param> info from stylesheet
    xmlDocPtr pDocOut = xsltApplyStylesheet(
      pSheet,m_pSheet->doc,px);
    if(pDocOut != NULL)
    {
      wxXml2Document doc;
      doc.SetDocPtr(pDocOut);
      m_bParmSheetOK = m_parmsSheet.LoadFromNode(doc.GetRoot());
    }
  }
}

xmlDocPtr wxXslSheet::_Transform(wxXml2Document *pXml2)
{
  xmlDocPtr pDocRtn = 
    (m_pSheet == NULL)
    ? NULL
    : xsltApplyStylesheet(
        m_pSheet,
        pXml2->GetDocPtr(),
        m_parmsUser.GetParms());
  return pDocRtn;
}
wxXml2Document *wxXslSheet::TransformToDOM(wxXml2Document *pXml2)
{
  wxXml2Document *pRtn = NULL;
  xmlDocPtr pDocRtn = _Transform(pXml2);
  if(pDocRtn != NULL)
  {
    pRtn = new wxXml2Document;
    pRtn->SetDocPtr(pDocRtn);
  }
  return pRtn;
}
bool wxXslSheet::TransformToFile(
  const wxString &sFileName, wxXml2Document *pXml2)
{
  bool bRtn = false;
  xmlDocPtr pDoc = _Transform(pXml2);
  if(pDoc == NULL)
  {}
  else if(wxFileExists(sFileName) && (!wxRemoveFile(sFileName)))
  {
    wxString s(_T("Could not delete existing file: "));
    s.Append(sFileName);
    wxXml2Object::SendError(s);
    bRtn = false;
  }
  else
  {
    int n = xsltSaveResultToFilename(
      sFileName.utf8_str(),
      pDoc,
      m_pSheet,
      0);
    bRtn = (n > -1);
    xmlFreeDoc(pDoc);
  }
  return bRtn;
}


//********************************************* wxXslParams

wxXslParams::~wxXslParams()
{
  if(m_ppParms != NULL)
  {
    free(m_ppParms);
  }
}

bool wxXslParams::SetParm(
  const wxString &sName, const wxString &sValue)
{
  map<wxString, wxString>::iterator itr = m_mapParms.find(sName);
  bool bRtn = false;
  wxString s;
  if(nwxString::IsNumber(sValue))
  {
    s = sValue;
  }
  else
  {
    // string must be quoted inside '' or ""
    // if both types of quotes are used, then 
    // change single quotes ' to backquotes `

    const wxChar *QUOT = _T("'");
    const wxChar *QUOT2 = _T("\"");
    const wxChar *BACKQUOT = _T("`");
    const wxChar *ENCLOSE = QUOT;
    wxString sTmp = sValue;

    if(!sTmp.Contains(QUOT)) 
    {} // no problem
    else if(sTmp.Contains(QUOT2))
    {
      // contains both types of quotes,
      // so change the single quotes to backquotes
      //
      sTmp.Replace(QUOT,BACKQUOT,true);
    }
    else
    {
      // enclose in double quotes
      ENCLOSE = QUOT2;
    }
    s = ENCLOSE;
    s.Append(sTmp);
    s.Append(ENCLOSE);
  }
  if(itr == m_mapParms.end())
  {
    bRtn = true;
  }
  else if(itr->second != s)
  {
    bRtn = true;
    m_mapParms.erase(itr);
  }
  if(bRtn)
  {
    m_bParmsModified = true;
    m_mapParms.insert(
      map<wxString, wxString>::value_type(sName,s));
  }
  return bRtn;
}
bool wxXslParams::ClearParm(const wxString &sName)
{
  map<wxString, wxString>::iterator itr = m_mapParms.find(sName);
  bool bRtn = (itr != m_mapParms.end());
  if(bRtn)
  {
    m_mapParms.erase(itr);
    m_bParmsModified = true;
  }
  return bRtn;
}
const wxChar *wxXslParams::GetParm(const wxString &sName) const
{
  map<wxString, wxString>::const_iterator itr =
      m_mapParms.find(sName);
  const wxChar *psRtn = NULL;
  if(itr != m_mapParms.end())
  {
    psRtn = itr->second;
  }
  return psRtn;
}

void wxXslParams::Clear()
{
  m_bParmsModified = true;
  m_mapParms.clear();
}

const char **wxXslParams::GetParms()
{
  const char **ppPtr = m_ppParms;
  if( (m_ppParms == NULL) || m_bParmsModified )
  {
    // round up (size * 2 + 1) to the nearest multiple of 8
    // size is the number of name value pairs
    // size * 2 is the number of names and values
    //  add 1 for the NULL terminator
    //
    size_t nCount =
      ((((m_mapParms.size() << 1) + 1) + 7) & (~7));
    size_t nSize = nCount * sizeof(const char *);
    if(m_ppParms == NULL)
    {
      m_ppParms = (const char **)malloc(nSize);
      m_nPtrAlloc = 
        (m_ppParms == NULL) ? 0 : nSize;
      ppPtr = m_ppParms;
    }
    else if(nSize > m_nPtrAlloc)
    {
      ppPtr = (const char **)realloc((void *)m_ppParms,nSize);
      if(ppPtr != NULL)
      {
        m_ppParms = ppPtr;
        m_nPtrAlloc = nSize;
      }
    }
    if(ppPtr != NULL)
    {
      map<wxString, wxString>::iterator itr;
      for(itr = m_mapParms.begin(); itr != m_mapParms.end(); ++itr)
      {
        *ppPtr++ = itr->first.utf8_str();
        *ppPtr++ = itr->second.utf8_str();
      }
      *ppPtr = NULL;
    }
    wxASSERT_MSG((ppPtr - m_ppParms) <= (int)nCount, "Arrary overrun");
  }
  return m_ppParms;
}

void wxXslParams::Init()
{
  Clear();
}
void wxXslParams::RegisterAll(bool)
{
  Register(_T("param"),&m_param);
}

void wxXslParams::wxXslSingleParam::RegisterAll(bool)
{
  RegisterWxStringAttribute(_T("name"),&m_sName);
  RegisterWxStringText(&m_sValue);
}

bool wxXslParams::wxXslSingleParam::LoadFromNode(
  wxXmlNode *pNode, void *pObj)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(pNode,pObj);
  if(bRtn)
  {
    _UpdateParent();
  }
  return bRtn;
}

bool wxXslParams::wxXslSingleParam::LoadFromNode(
  wxXmlNode *pNode)
{
  bool bRtn = nwxXmlPersist::LoadFromNode(pNode);
  if(bRtn)
  {
    _UpdateParent();
  }
  return bRtn;
}
