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
*
*  FileName: nwxWCharBuffer.h
*  Author:   Douglas Hoffman
*    12/15/2019
*
*  A memory management class for a wchar_t * buffer, usually used for ARGV in
*  nwxProcess.  Reason: wxString operator (wchar_t *) return a const and
*  ARGV is not a const
*
*/
#ifndef __NWX_WCHAR_BUFFER_H__
#define __NWX_WCHAR_BUFFER_H__
#include <wx/string.h>

class nwxWCharBuffer
{
public:
  nwxWCharBuffer() : m_pBuffer(NULL), m_nLen(0)
  {}
  nwxWCharBuffer(const nwxWCharBuffer &x) : m_pBuffer(NULL), m_nLen(0)
  {
    Set(x.Get());
  }
  nwxWCharBuffer(const wchar_t *p) : m_pBuffer(NULL), m_nLen(0)
  {
    Set(p);
  }
  nwxWCharBuffer(const char *p) : m_pBuffer(NULL), m_nLen(0)
  {
    Set(p);
  }
  nwxWCharBuffer(const wxString &s) : m_pBuffer(NULL), m_nLen(0)
  {
    Set(s);
  }
  nwxWCharBuffer(size_t n, const wchar_t *p = NULL) : m_pBuffer(NULL), m_nLen(0)
  {
    _alloc(n);
    if (p != NULL)
    {
      Set(p);
    }
  }
  virtual ~nwxWCharBuffer()
  {
    _cleanup();
  }
  void Set(const wchar_t *p)
  {
    size_t nLen = wcslen(p);
    _alloc(nLen);
    wcscpy(m_pBuffer, p);
  }
  void Set(const char *p)
  {
    wxString s = wxString(p);
    Set((const wchar_t *)s);
  }
  void Set(const wxString &s)
  {
    Set((const wchar_t *)s);
  }
  operator wchar_t *()
  {
    return m_pBuffer;
  }
  operator const wchar_t *() const
  {
    return m_pBuffer;
  }
  wchar_t *Get()
  {
    return m_pBuffer;
  }
  const wchar_t *Get() const
  {
    return m_pBuffer;
  }
private:
  void _alloc(size_t nLen)
  {
    if ((m_pBuffer == NULL) || (nLen > m_nLen))
    {
      _cleanup();
      m_pBuffer = new wchar_t[(nLen + 1) * sizeof(wchar_t)];
      m_pBuffer[0] = wchar_t(0);
      m_nLen = nLen;
    }
  }
  void _cleanup()
  {
    if (m_pBuffer != NULL)
    {
      delete[] m_pBuffer;
      m_pBuffer = NULL;
    }
    m_nLen = 0;
  }
  wchar_t *m_pBuffer;
  size_t m_nLen;
};

#endif
