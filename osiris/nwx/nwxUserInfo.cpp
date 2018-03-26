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

#include "nwx/nwxUserInfo.h"
nwxIMPLEMENT_GLOBAL_OBJECT(nwxUserInfo)

#ifdef __WXMAC__

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <string.h>
#include <errno.h>

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
          ss = wxString::FromUTF8(psGroup);
          m_mapGIDtoName.insert(
            MAP_NS::value_type(
              nID,ss));
          m_mapNameToGID.insert(
            MAP_SN::value_type(
              ss,nID));
        }
        for(MAP_SN::iterator itr = m_mapNameToGID.begin();
          itr != m_mapNameToGID.end();
          ++itr)
        {
          m_asGroupNames.Add(itr->first);
          m_anGroupIDs.push_back(itr->second);
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

#ifdef __WXMSW__
#include <wx/utils.h> 
#include <wx/stdpaths.h>
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxProcess.h"

nwxUserInfo::nwxUserInfo() : m_bGroupsSetup(false), m_nGroupStatus(0)
{
  m_sUser = wxGetUserId();
}

class CProcessGroups : public nwxProcess
{
public:
  CProcessGroups(wxArrayString *pasGroups):
    nwxProcess(NULL),
    m_sGroupName(wxS("Group Name:")),
    m_sType(wxS("Type:")),
    m_pasGroups(pasGroups)
  {
    m_nLenGroupName = m_sGroupName.Len();
    m_nLenType = m_sType.Len();
  }
  virtual void ProcessLine(const char *p, size_t nLen, bool bErrStream);
  const wxString &GetStdErr() const
  {
    return m_sStdErr;
  }
private:
  wxString m_sGroupName;
  wxString m_sType;
  wxString m_sStdErr;
  wxString m_sLastGroup;
  wxArrayString *m_pasGroups;
  size_t m_nLenGroupName;
  size_t m_nLenType;
};

void CProcessGroups::ProcessLine(const char *p, size_t , bool bErrStream)
{
  wxString sLine = wxString::FromUTF8(p);
  if(bErrStream)
  {
    m_sStdErr.Append(sLine);
  }
  else if(sLine.Left(m_nLenGroupName) == m_sGroupName)
  {
    wxString s = sLine.Mid(m_nLenGroupName);
    nwxString::Trim(&s);
    m_sLastGroup = s;
  }
  else if(sLine.Left(m_nLenType) == m_sType)
  {
    wxString s = sLine.Mid(m_nLenType);
    s.MakeLower();
    if(s.Find(wxS("group")) == wxNOT_FOUND)
    {}
    else if(!m_sLastGroup.IsEmpty())
    {
      m_pasGroups->Add(m_sLastGroup);
      m_sLastGroup.Empty();
    }
  }
}

void nwxUserInfo::_setupGroupNames()
{
  if(!m_bGroupsSetup)
  {
    m_bGroupsSetup = true;
    wxString sWhoAmI = nwxFileUtil::PathFind("whoami.exe",true);
    if(sWhoAmI.IsEmpty())
    {
      nwxLog::LogMessage(wxS("Cannot find whoami.exe, therefore cannot find group names"));
    }
    else
    {
      CProcessGroups proc(&m_asGroupNames);
      const char *psWhoAmI;
      char *argv[5];
      psWhoAmI = sWhoAmI.utf8_str();
      argv[0] = (char*) psWhoAmI;
      argv[1] = "/GROUPS";
      argv[2] = "/FO";
      argv[3] = "LIST";
      argv[4] = NULL;
      proc.Run(argv, wxEXEC_SYNC);
      proc.ProcessIO();
      m_nGroupStatus = proc.GetExitStatus();
      const wxString &sErr(proc.GetStdErr());
      if(!sErr.IsEmpty())
      {
        wxString s(wxS("setup group names: error text\n"));
        s.Append(sErr);
        nwxLog::LogMessage(s);
      }
    }
  }
}
#endif

