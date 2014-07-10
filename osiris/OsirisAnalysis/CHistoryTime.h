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
*  FileName: CHistoryTime.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_HISTORY_TIME_H__
#define __C_HISTORY_TIME_H__

#include <wx/datetime.h>

class CHistoryTime
{
public:
  CHistoryTime(const wxDateTime *pTime = NULL) : m_dtHistory((time_t) 0)
  {
    SetDateTime(pTime);
  }
  CHistoryTime(const CHistoryTime &x) : m_dtHistory((time_t) 0)
  {
    (*this) = x;
  }
  CHistoryTime &operator = (const CHistoryTime &x)
  {
    m_bCurrent = x.m_bCurrent;
    m_dtHistory = x.m_dtHistory;
    return *this;
  }
  CHistoryTime &operator = (const wxDateTime &dt)
  {
    SetDateTime(&dt);
    return *this;
  }
  CHistoryTime &operator = (const wxDateTime *pdt)
  {
    SetDateTime(pdt);
    return *this;
  }
  virtual ~CHistoryTime();
  bool SetDateTime(const wxDateTime *pTime);
  bool SetCurrentTime()
  {
    return SetDateTime(NULL);
  }
  bool SetOriginalTime()
  {
    wxDateTime dt((time_t)0);
    return SetDateTime(&dt);
  }
  const wxDateTime *GetDateTime() const
  {
    return m_bCurrent ? NULL : &m_dtHistory;
  }
  bool IsCurrent() const
  {
    return m_bCurrent;
  }
  bool IsOriginal() const
  {
    return (!m_bCurrent) && (m_dtHistory.GetTicks() == 0);
  }
  operator const wxDateTime *() const
  {
    return GetDateTime();
  }
  bool IsEqualTo(const wxDateTime *pTime) const
  {
    bool bRtn = false;
    if(pTime == NULL)
    {
      bRtn = m_bCurrent;
    }
    else if(!m_bCurrent)
    {
      bRtn = (m_dtHistory == *pTime);
    }
    return bRtn;
  }
  bool operator == (const CHistoryTime &x)
  {
    return IsEqualTo(x.GetDateTime());
  }

private:
  wxDateTime m_dtHistory;
  bool m_bCurrent;
public:
  static const wxString CURRENT; 

};



#endif
