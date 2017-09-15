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
*  FileName: nwxLog.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_LOG_H__
#define __NWX_LOG_H__

#include <time.h>
#include <stdarg.h>
#include <wx/log.h>
#include <wx/string.h>

class nwxLog
{
private:
  nwxLog() {} // prevent instantiation
public:
  static void LogMessage(const wxString &sMsg)
  {
    time_t t;
    time(&t);
    wxLog::OnLog(wxLOG_Message,sMsg,t);
  }
  static void LogMessageV(const wxChar *psFormat,...)
  {
    va_list ap;
    va_start(ap,psFormat);
    wxString s = wxString::FormatV(psFormat,ap);
    va_end(ap);
    LogMessage(s);
  }
};

#ifdef __WXDEBUG__

#ifdef WIN32
#define __func__ __FUNCTION__
#endif

#define nwxLog_I_WAS_HERE \
  nwxLog::LogMessageV(wxT("I WAS HERE: %ls, line %d, %ls"),wxString(__FILE__).wc_str(),__LINE__,wxString(__func__).wc_str())

#define nwxLog_I_WAS_HERE_MSG(msg) \
  nwxLog::LogMessageV(wxT("I WAS HERE: %ls, line %d, %ls\n    %ls"),wxString(__FILE__).wc_str(),__LINE__,wxString(__func__).wc_str(), wxString(msg).wc_str())



#else

#define nwxLog_I_WAS_HERE
#define nwxLog_I_WAS_HERE_MSG(msg)

#endif

#endif
