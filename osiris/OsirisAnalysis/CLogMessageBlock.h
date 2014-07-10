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
*  FileName: CLogMessageBlock.h
*  Author:   Douglas Hoffman
*    Logs a message in the constructor, and logs an identical message 
*    with " END" appended in the destructor
*
*
*/
#ifndef __C_LOG_MESSAGE_BLOCK_H__
#define __C_LOG_MESSAGE_BLOCK_H__

#include <wx/string.h>
#include "mainApp.h"
#include "Platform.h"

class CLogMessageBlock
{
public:
  CLogMessageBlock(const wxString &s, int nLevel = 0) : m_nLevel(nLevel)
  {
    wxString sTmp("** BEGIN ** ");
    m_s =         "** END   ** ";
    sTmp.Append(s);
    m_s.Append(s);
    mainApp::LogMessage(sTmp,nLevel);
  }
  virtual ~CLogMessageBlock()
  {
    mainApp::LogMessage(m_s,m_nLevel);
  }
private:
  wxString m_s;
  int m_nLevel;
};

class CLogMessageBlockV
{
public:
  CLogMessageBlockV(wxChar *psFormat,...)
  {
    va_list ap;
    va_start(ap,psFormat);
    wxString s = wxString::FormatV(psFormat,ap);
    va_end(ap);
    m_pBlock = new CLogMessageBlock(s);
  }
  virtual ~CLogMessageBlockV()
  {
    delete m_pBlock;
  }
private:
  wxString m_s;
  CLogMessageBlock *m_pBlock;
};

#if LOG_DEBUG
#define LOG_MESSAGE_BLOCK(name,str) CLogMessageBlock name(str);
#define LOG_MESSAGE_TEXT(str) mainApp::LogMessage(str);
#else
#define LOG_MESSAGE_BLOCK(name,str)
#define LOG_MESSAGE_TEXT(str)
#endif

#endif
