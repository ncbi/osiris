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
*  FileName: wxXml2Object.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/object.h>
#include "wxXml2Object.h"
#ifdef ATTRIBUTE_PRINTF
#undef ATTRIBUTE_PRINTF
#endif
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include "wxXml2/wxXml2Ptr.h"

int wxXml2Object::g_nCount(0);
map<IXml2ErrorReceiver *,int> wxXml2Object::g_mapReceivers;

IMPLEMENT_ABSTRACT_CLASS(wxXml2Object,wxObject)

extern "C"
{
  static void StructuredErrorFunction(void *, xmlErrorPtr error)
  {
    wxString s =  wxString::Format(
      "xml error: %d; line %d; %s",
      error->code,
      error->line,
      error->message);
    wxXml2Object::SendError(s);
  }

  void GenericErrorFunction(void *, const char *msg,...)
  {
    va_list ap;
    va_start(ap,msg);
    const size_t BUFFERSIZE = 4095;
    char sBuffer[BUFFERSIZE + 1];

    // CYA, vsprintf does not seem to work as documented
    memset(sBuffer,0, sizeof(sBuffer)); 

    vsnprintf(sBuffer,BUFFERSIZE,msg,ap);

    // if buffer is too small in VC++, need to clean up 
    // did not work as documented while debugging
    sBuffer[BUFFERSIZE] = 0; 
    va_end(ap);

    size_t nLen = strlen(sBuffer);
    while(nLen > 0 && sBuffer[nLen - 1] <= 32)
    {
      nLen--;
      sBuffer[nLen] = 0;
    }
    if(sBuffer[0]) // not empty
    {
      wxString s(sBuffer);
      wxXml2Object::SendError(s);
    }
  }
};

void wxXml2Object::_init()
{
  xmlInitMemory();
  xmlSetStructuredErrorFunc(NULL,StructuredErrorFunction);
  xmlSetGenericErrorFunc(NULL,GenericErrorFunction);
}
void wxXml2Object::_cleanup()
{
#ifndef NO_FREE
  xmlCleanupParser();
#endif
  return;
}

void wxXml2Object::SendError(const wxString &_s)
{
  wxString s(_s);
  s.Trim(true);
  s.Trim(false);
  if(!s.IsEmpty())
  {
    map<IXml2ErrorReceiver *,int>::iterator itr;
    for(itr = g_mapReceivers.begin(); itr != g_mapReceivers.end(); ++itr)
    {
      itr->first->ReceiveXml2Error(s);
    }
  }
}


void wxXml2Object::AddReceiver(IXml2ErrorReceiver *p)
{
  map<IXml2ErrorReceiver *,int>::iterator itr = g_mapReceivers.find(p);
  if(itr != g_mapReceivers.end())
  {
    itr->second++;
  }
  else
  {
    g_mapReceivers.insert(map<IXml2ErrorReceiver *,int>::value_type(p,1));
  }
}
void wxXml2Object::RemoveReceiver(IXml2ErrorReceiver *p)
{
  map<IXml2ErrorReceiver *,int>::iterator itr = g_mapReceivers.find(p);
  if(itr != g_mapReceivers.end())
  {
    itr->second--;
    if(!itr->second)
    {
      g_mapReceivers.erase(itr);
    }
  }
}


wxXml2Ptr::~wxXml2Ptr()
{
  if(m_p != NULL)
  {
#ifndef NO_FREE
    xmlFree(m_p);
#endif
  }
}
