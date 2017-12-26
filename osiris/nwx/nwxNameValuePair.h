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
*  FileName: nwxNameValuePair.h
*  Author:   Douglas Hoffman
*  Date:     2017-12-22
*
*   Class for xml type of 
*      <elem>
*        <name>knock knock</name>
*        <value>who is there</value>
*      <elem>
*/

#ifndef __NWX_NAME_VALUE_PAIR_H__
#define __NWX_NAME_VALUE_PAIR_H__

#include "nwx/nwxXmlPersist.h"

class nwxNameValuePair : public nwxXmlPersist
{
public:
  nwxNameValuePair()
  {
    RegisterAll(true);
  }
  virtual ~nwxNameValuePair()
  {}
  virtual void RegisterAll(bool)
  {
    RegisterWxString(wxS("Name"),&m_sName);
    RegisterWxString(wxS("Value"),&m_sValue);
  }
  const wxString &GetName() const
  {
    return m_sName;
  }
  const wxString &GetValue() const
  {
    return m_sValue;
  }
  void SetName(const wxString &s)
  {
    m_sName = s;
  }
  void SetValue(const wxString &s)
  {
    m_sValue = s;
  }
private:
  wxString m_sName;
  wxString m_sValue;

};

#endif