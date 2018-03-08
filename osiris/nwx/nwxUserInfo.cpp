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
*  FileName: nwxUserInfo.cpp
*  Author:   Douglas Hoffman
*  Date:     2018/02/11
*
*  obtains user and group info using the UNIX API, currently Mac OSX
*
*/

#ifdef __WXMAC__

#include "nwx/nwxUserInfo.h"
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <string.h>
#include <errno.h>

nwxIMPLEMENT_GLOBAL_OBJECT(nwxUserInfo)

nwxUserInfo::nwxUserInfo() : m_nGID(0), m_nUID(0), m_nLastError(0)
{
#define MAX_GID 128  
  int anList[MAX_GID];
  int nCount = MAX_GID;
  struct passwd *pPwEntry;

  memset(anList,0,sizeof(anList));
  m_nUID = getuid();
  pPwEntry = (m_nUID > 0) ? getpwuid(m_nUID) : NULL;
  errno = 0;
  if(pPwEntry == NULL)
  {
    m_nLastError = errno;
  }
  else
  {
    m_nGID = pPwEntry->pw_gid;
    m_sUser = wxString::FromUTF8(pPwEntry->pw_name);
    int n = getgrouplist(m_sUser.utf8_str(), m_nGID, anList, &nCount);
    if( (n == 0 || n == -1) && (nCount > 0) )
    {
      wxString ss;
      char *psGroup;
      int nID;
      int n;
      m_asGroupNames.Alloc(nCount);
      m_anGroupIDs.reserve(nCount);
      for(n = 0; n < nCount; ++n)
      {
        nID = anList[n];
        psGroup = group_from_gid(nID,0);
        if(psGroup != NULL)
        {
          m_asGroupNames.Add(wxString::FromUTF8(psGroup));
          m_anGroupIDs.push_back(nID);
          m_mapGIDtoName.insert(
            MAP_NS::value_type(
              nID,m_asGroupNames.Item(n)));
          m_mapNameToGID.insert(
            MAP_SN::value_type(
              m_asGroupNames.Item(n),nID));
        }  
      }
    }
    else
    {
      m_nLastError = errno;
    }
  }
#undef MAX_GID
}


const wxString nwxUserInfo::g_EMPTY(wxEmptyString);
#endif
