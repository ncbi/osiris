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
*  FileName: wxXslObject.cpp
*  Author:   Douglas Hoffman
*
*/
#include "wxXslObject.h"
#include "wxXml2/wxXml2Document.h"
#include <libxml/xpathInternals.h>
#include <libxslt/xsltconfig.h>
#include <libxslt/xsltutils.h>
#ifdef XSLT_LOCALE_WINAPI
#include <libxml/threads.h>
#endif
#include <libxslt/extra.h>
#include <libxslt/extensions.h>
#include <libexslt/exslt.h>
#include <wx/string.h>
#include <wx/mstream.h>
#include <list>
#include "nwx/nsstd.h"
#include "paramsXsl.h"


extern "C"
{
extern void GenericErrorFunction(void *, const char *msg,...);

static void _extStrReplace(
    xmlXPathParserContextPtr ctxt, int nargs)
{
  /* libxslt does not provide str:replace,
     so it is implemented here */

  list<wxString> vs;
  wxString s;
  wxString sFrom;
  wxString sTo;
  xmlXPathObjectPtr pObj;
  int i;

  if(nargs < 0)
  {
    nargs = 0;
  }

  switch(nargs)
  {
  case 0:
    valuePush(ctxt,xmlXPathNewCString(""));
    break;
  case 1:
    pObj = valuePop(ctxt);
    valuePush(ctxt,pObj);
    if(pObj->type != XPATH_STRING)
    {
      xmlXPathStringFunction(ctxt,1);
    }
    break;
  default:
    for(i = 0; i < nargs; i++)
    {
      pObj = valuePop(ctxt);
      if(pObj->type != XPATH_STRING)
      {
        valuePush(ctxt, pObj);
        xmlXPathStringFunction(ctxt,1);
        pObj = valuePop(ctxt);
      }
      s = (wxChar *)(pObj->stringval);
      vs.push_front(s);
      xmlXPathFreeObject(pObj);
    }
    for(i = nargs; i < 3; i++)
    {
      vs.push_back(wxEmptyString);
    }
    list<wxString>::iterator itr = vs.begin();
    s = (*itr);
    itr++;
    sFrom = (*itr);
    itr++;
    sTo = (*itr);
    s.Replace(sFrom,sTo,true);
    pObj = xmlXPathNewCString(s.utf8_str());
    valuePush(ctxt,pObj);
  }
}

#ifdef XSLT_LOCALE_WINAPI
// hack to fix a memory leak in _cleanupXsl() below
extern xmlRMutexPtr xsltLocaleMutex;
#endif

};  // extern C

int wxXslObject::g_nXslClass = 0;
xsltStylesheetPtr wxXslObject::g_pSheetParam = NULL;

#define NAME_REPLACE (const xmlChar *)"replace"
#define URI_REPLACE  (const xmlChar *) "http://exslt.org/strings"


void wxXslObject::_cleanupXsl()
{
  if(g_pSheetParam != NULL)
  {
    xsltFreeStylesheet(g_pSheetParam);
    g_pSheetParam = NULL;
  }
	xsltUnregisterExtModuleFunction(NAME_REPLACE,URI_REPLACE);
  xsltCleanupGlobals();

#ifdef XSLT_LOCALE_WINAPI
  // hack to fix a memory leak
  if(xsltLocaleMutex != NULL)
  {
    xmlFreeRMutex(xsltLocaleMutex);
    xsltLocaleMutex = NULL;
  }
#endif
}
void wxXslObject::_initXsl()
{
//	xsltRegisterAllExtras();
	exsltRegisterAll();
  xsltRegisterExtModuleFunction(
    NAME_REPLACE,URI_REPLACE,_extStrReplace);
  xsltSetGenericErrorFunc(NULL,GenericErrorFunction); // retrieve <xsl:message>
}
xsltStylesheetPtr wxXslObject::GetSheetParam()
{
  if(g_pSheetParam == NULL)
  {
    wxMemoryInputStream stream(
      (const char *)paramsXsl,
      sizeof(paramsXsl) / sizeof(paramsXsl[0]));
    wxXml2Document xDoc(stream);
    xmlDocPtr pDoc = xDoc.AcquireDocPtr();
    if(pDoc != NULL)
    {
      g_pSheetParam = xsltParseStylesheetDoc(pDoc);
    }
  }
  return g_pSheetParam;
}
//IMPLEMENT_ABSTRACT_CLASS(wxXslObject, wxXml2Object);
