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
*  FileName: nwxUserInfo.h
*  Author:   Douglas Hoffman
*  Date:     2018/02/11
*
*  obtains user and group info using the UNIX API, currently Mac OSX
*
*/
#ifndef __NWX_USER_INFO_H__
#define __NWX_USER_INFO_H__
#ifdef __WXMAC__

#include "nwx/stdb.h"
#include <map>
#include <vector>
#include "nwx/stde.h"
#include <sys/types.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include "nwx/nwxGlobalObject.h"

class nwxUserInfo
{
private:
  nwxUserInfo();
public:
  typedef std::map<int,const wxString &> MAP_NS;
  typedef std::map<const wxString &,int> MAP_SN;
  virtual ~nwxUserInfo() {}
  const wxString &GetUserName() const
  {
    return m_sUser;
  }
  uid_t GetUID() const
  {
    return m_nUID;
  }
  int GetDefaultGID() const
  {
    return m_nGID;
  }
  int GetLastError() const
  {
    return m_nLastError;
  }
  const wxString &GetDefaultGroupName() const
  {
    return GetGroupNameFromID(m_nGID);
  }
  const std::vector<int> &GetGroupIDs() const
  {
    return m_anGroupIDs;
  }
  const wxArrayString &GetGroupNames() const
  {
    return m_asGroupNames;
  }
  const wxString &GetGroupNameFromID(int nID) const
  {
    MAP_NS::const_iterator itr = m_mapGIDtoName.find(nID);
    return (itr == m_mapGIDtoName.end()) ? g_EMPTY : itr->second;
  }
  int GetGroupIDfromName(const wxString &sName) const
  {
    MAP_SN::const_iterator itr = m_mapNameToGID.find(sName);
    return (itr == m_mapNameToGID.end()) ? -1 : itr->second;
  }
private:
  static const wxString g_EMPTY;
  wxArrayString m_asGroupNames;
  std::vector<int> m_anGroupIDs;
  MAP_NS m_mapGIDtoName;
  MAP_SN m_mapNameToGID;
  wxString m_sUser;
  gid_t m_nGID;
  uid_t m_nUID;
  int m_nLastError;
  nwxDECLARE_GLOBAL_OBJECT(nwxUserInfo);
};



#endif
#endif
