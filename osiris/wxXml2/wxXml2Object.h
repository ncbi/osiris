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
*  FileName: wxXml2Object.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __WX_XML2_OBJECT_H__
#define __WX_XML2_OBJECT_H__

#include <wx/object.h>
#include <wx/string.h>



//  libxml #defines
#ifdef _WINDOWS
#define _REENTRANT
#define HAVE_WIN32_THREADS
#define LIBXML_STATIC
#endif

#ifdef ATTRIBUTE_PRINTF
#undef ATTRIBUTE_PRINTF
#endif

#include <libxml/tree.h>
#include <map>
#include <vector>

class IXml2ErrorReceiver
{
public:
  virtual void ReceiveXml2Error(const wxString &s) = 0;
  virtual ~IXml2ErrorReceiver() {}
protected:
  IXml2ErrorReceiver() {}
};

class WXDLLIMPEXP_BASE wxXml2Object : public wxObject
{
public:
  virtual ~wxXml2Object()
  {
    g_nCount--;
    if(!g_nCount)
    {
      _cleanup();
    }
  }
protected:
  wxXml2Object()
  {
    if(!g_nCount)
    {
      _init();
    }
    g_nCount++;
  }
public:
  static void SendError(const wxString &s);
  static void AddReceiver(IXml2ErrorReceiver *p);
  static void RemoveReceiver(IXml2ErrorReceiver *p);

private:
  void _init();
  void _cleanup();
  static int g_nCount;
  static std::map<IXml2ErrorReceiver *,int> g_mapReceivers;
  DECLARE_ABSTRACT_CLASS(wxXml2Object)
};

class wxXml2ReceiverHolder
{
public:
  wxXml2ReceiverHolder(IXml2ErrorReceiver *p)
  {
    m_p = p;
    wxXml2Object::AddReceiver(m_p);
  }
  virtual ~wxXml2ReceiverHolder()
  {
    wxXml2Object::RemoveReceiver(m_p);
  }
private:
  IXml2ErrorReceiver *m_p;
};

class wxXml2SimpleErrorReceiver : public IXml2ErrorReceiver
{
public:
  wxXml2SimpleErrorReceiver() : m_Hold(this)
  {}
  virtual ~wxXml2SimpleErrorReceiver()
  {}
  virtual void ReceiveXml2Error(const wxString &s)
  {
    m_as.push_back(s);
  }
  const std::vector<wxString> &GetErrors() const
  {
    return m_as;
  }
private:
  std::vector<wxString> m_as;
  wxXml2ReceiverHolder m_Hold;
};



//#ifdef _DEBUG
//#define NO_FREE 0
//#endif


#endif
