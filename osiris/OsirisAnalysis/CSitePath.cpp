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
*  FileName: CSitePath.cpp
*  Author:   Douglas Hoffman
*  Date:     2018/02/07
*
*  full (I mean less empty) description is in CSitePath.h
*
*/
#include "CSitePath.h"

#ifdef __WXMAC__
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach-o/dyld.h>
#include <sys/stat.h>
#include <wx/utils.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include "nwx/nwxString.h"
#include "nwx/nwxLog.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxUserInfo.h"
#include "mainApp.h"
#include "ConfigDir.h"

nwxIMPLEMENT_GLOBAL_OBJECT(CSitePath)

const wxString CSitePath::g_sAppleScript(wxS("/usr/bin/osascript"));
wxString CSitePath::g_sScriptPath;

CSitePath::CSitePath() :
  m_nLastError(0),
  m_bUserPath(false),
  m_bThisUserPath(false),
  m_bXattrProblem(false)
{
  // constructor.  Get path for executable, figure out where
  //  site folder (Osiris-Files) should be
  //  not sure what will happen with sym links
  
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxFileName fn(sp.GetExecutablePath());
  char buffer[PATH_MAX + 1];
  wxString sPath = fn.GetFullPath(wxPATH_NATIVE);
  const wxString sSiteLib(wxT("/Library/Application Support/Osiris-Files"));
  const wxString sSiteShared(wxT("/Users/Shared/Osiris-Files"));
  const wxString sSharedHome(wxT("/Users/Shared"));
  const wxString sSlash(wxT("/"));
  int nApp;
  if(realpath(sPath.utf8_str(),buffer) == NULL)
  {
    m_nLastError = errno;
    m_sExePath = sPath;
  }
  else
  {
    m_sExePath  = wxString::FromUTF8(buffer);
  }
  wxString sExeLower(m_sExePath);
  sExeLower.MakeLower();
  m_bXattrProblem = !sExeLower.Find("/private/var");
  //  if exe file is in /private/var, then OS X 10.12 or later
  //  moved it there upon execution
  //   the fix for this is to run xattr -r -c 'Osiris.app'

//  figure out path for Osiris-Files
//  based on application location

  if(m_bXattrProblem || !m_sExePath.find(wxT("/Applications/")))
  {
    // OSIRIS is in /Applications or unknown (/private/var)
    // look in /Library/Application Support
    //  and /Users/Shared

    if(wxFileName::DirExists(sSiteLib))
    {
      // compatible with older versions
      m_sSitePath = sSiteLib;
    }
    else if(wxFileName::DirExists(sSharedHome))
    {
      // preferred location is /Users/Shared/Osiris-Files
      m_sSitePath = sSiteShared;
    }
    else
    {
      // fallback if /Users/Shared does not exist
      m_sSitePath = sSiteLib;
    }
  }
  else if( (nApp = sExeLower.Find(wxT(".app"))) > 0 )
  {
    // look for existing Osiris-Files directory in parent
    //  of .app directory

    wxString sApp(m_sExePath.Mid(0,nApp + 4));
    wxFileName fnApp(sApp);
    wxString sDirSibling = fnApp.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    sDirSibling.Append("Osiris-Files");
    if(wxFileName::DirExists(sDirSibling))
    {
      m_sSitePath = sDirSibling;
    }
    if ( (!m_sExePath.Find(wxT("/Users/"))) &&
         (!(m_sExePath.Find(sSharedHome + wxT("/")) == 0)) &&
         (sApp.Replace(sSlash,sSlash,true) >= 3) )
    {
      // find user directory, not necessarily current user
      int nSlash2 = sApp.find(sSlash,1);
      int nSlash3 = (nSlash2 > 0) ? sApp.find(sSlash,nSlash2 + 1) : wxNOT_FOUND;
      if(nSlash3 != wxNOT_FOUND)
      {
        m_bUserPath = true;
        wxString sHome = wxFileName::GetHomeDir();
        nwxFileUtil::EndWithSeparator(&sHome);
        size_t nLenHome = sHome.Len();
        m_bThisUserPath = (m_sExePath.Len() > nLenHome) &&
          (m_sExePath.Left(nLenHome) == sHome);
        if(m_sSitePath.IsEmpty())
        {
          m_sSitePath = sApp.Mid(0,nSlash3); // no trailing slash
          m_sSitePath.Append(sSiteLib);
        }
      }
    }
    if(m_sSitePath.IsEmpty())
    {
      // not a user directory nor Applications,
      // possible network install
      m_sSitePath = sDirSibling;
    }
  }
  _setupRealSitePath();
}

void CSitePath::_setupRealSitePath()
{
  char buffer[PATH_MAX + 1];
  if(!m_sRealSitePath.IsEmpty()) {}
  else if(m_sSitePath.IsEmpty()) {}
  else if(!wxFileName::DirExists(m_sSitePath)) {}
  else if(realpath(m_sSitePath.utf8_str(),buffer) == NULL)
  {
    m_nLastError = errno;
  }
  else
  {
    m_sRealSitePath  = wxString::FromUTF8(buffer);
  }
}

bool CSitePath::SitePathExists() const
{
  bool bRtn = (!m_sSitePath.IsEmpty()) &&
    wxFileName::DirExists(m_sSitePath);
  return bRtn;
}
/*
bool CSitePath::_appendArg(wxString *pStr, const wxString &sArg)
{
  bool bRtn = (sArg.Find(wxT("\"")) == wxNOT_FOUND);
  // cannot process a directory name with a double quote "

  //   (sArg.Find(wxT("'")) == wxNOT_FOUND) );

  if(bRtn)
  {
    pStr->Append(wxT(" "));
    if(sArg.Find(wxT(" ")) != wxNOT_FOUND)
    {
      pStr->Append(wxT("'"));
      pStr->Append(sArg);
      pStr->Append(wxT("'"));
    }
    else
    {
      pStr->Append(sArg);
    }
  }
  return bRtn;
}
*/

const wxString &CSitePath::_getTmpShellScript()
{
  // get user config directory
  wxString sDir = mainApp::GetConfig()->GetConfigPath();
  wxString sFormat(wxS("tmp-%d.sh"));
  int n = 0;
  nwxFileUtil::EndWithSeparator(&sDir);
  _unlinkTmpShellScript();
  do
  {
    m_sTmpShellScript = sDir;
    m_sTmpShellScript.Append(wxString::Format(sFormat,n));
    n++;
  }
  while(wxFile::Exists(m_sTmpShellScript));

  return m_sTmpShellScript;
}
bool CSitePath::_unlinkTmpShellScript()
{
  bool bRtn = true;
  if(!m_sTmpShellScript.IsEmpty())
  {
    bRtn = !unlink(m_sTmpShellScript.utf8_str());
    m_sTmpShellScript.Empty();
  }
  return bRtn;
}

const wxString &CSitePath::_prepareShellParm(const wxString &s, wxString *pBuffer)
{
  const wxUniChar cs(' ');
  const wxUniChar cq1('\'');
  const wxUniChar cq2('"');
  const wxUniChar cBack('\\');
  size_t nq2 = s.Find(cq2);
  size_t nq1 = s.Find(cq1);
  bool bSpace = s.Find(cs) != wxNOT_FOUND;
  bool bQuote1 = nq1 != (size_t)wxNOT_FOUND;
  bool bQuote2 = nq2 != (size_t)wxNOT_FOUND;
  bool bBack = s.Find(cBack) != wxNOT_FOUND;
  if(!(bSpace || bQuote1 || bQuote2 || bBack))
  {
    *pBuffer = s;
  }
  else if(!bQuote1)
  {
    *pBuffer = cq1;
    pBuffer->Append(s);
    pBuffer->Append(cq1);
  }
  else if(!bQuote2)
  {
    *pBuffer = cq2;
    pBuffer->Append(s);
    pBuffer->Append(cq2);
  }
  else
  {
    wxString sTmpBuffer;
    size_t n = (nq1 < nq2) ? nq1 : nq2;
    n++;
    *pBuffer = _prepareShellParm(s.Left(n),&sTmpBuffer);
    pBuffer->Append(_prepareShellParm(s.Mid(n),&sTmpBuffer));
  }
  return *pBuffer;
}

bool CSitePath::_runScript(const wxArrayString &pas, bool bAsAdmin)
{
  wxFileName fnExe(m_sExePath);
  wxString sCmd;
  wxString sArg;
  wxString sBack(wxS("\\"));
  wxString sBack2(wxS("\\\\"));
  bool bRtn = false;

  if(g_sScriptPath.IsEmpty())
  {
    g_sScriptPath = fnExe.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    g_sScriptPath.Append(wxT("CSitePath.sh"));
  }
  if(!wxFileName::FileExists(g_sScriptPath))
  {
    m_nLastError = SCRIPT_NOT_FOUND;
  }
  else if(!wxFileName::IsFileExecutable(g_sScriptPath))
  {
    m_nLastError = SCRIPT_NOT_EXECUTABLE;
  }
  else
  {
    bRtn = true;
    const wchar_t *ARGV[12];
    size_t nSize = pas.GetCount();
    if(!bAsAdmin)
    {
      size_t i;
      size_t n;
      ARGV[0] = g_sScriptPath.wx_str();
      for (i = 0, n = 1; i < nSize; ++i,++n)
      {
        ARGV[n] = pas.Item(i).wx_str();
      }
      ARGV[n] = NULL;
    }
    else
    {
      // build oascript command
      const wxString &sTmp = _getTmpShellScript();
      wxFile fh;
      if(!fh.Open(sTmp,wxFile::write,0755))
      {
        bRtn = false;
        m_nLastError = EPERM;
      }
      else
      {
        sCmd = wxS("do shell script \"");
        if(sTmp.Find(' ') == wxNOT_FOUND)
        {
          sCmd.Append(sTmp);
        }
        else
        {
          wxString sTmp2(sTmp);
          sTmp2.Replace(wxS(" "),wxS("?"));
          sCmd.Append(sTmp2);
        }
        sCmd.Append(wxS("\" with administrator privileges"));
        wxString sShellScript(wxS("#!/bin/bash\n"));
        sShellScript.Append(_prepareShellParm(g_sScriptPath,&sArg));
        for(size_t i = 0; i < nSize; ++i)
        {
          _prepareShellParm(pas.Item(i),&sArg);
          sShellScript.Append(wxS(" "));
          sShellScript.Append(sArg);
        }
        sShellScript.Append(wxS("\n"));
        bRtn = fh.Write(sShellScript) && fh.Close();
        if(!bRtn)
        {
          m_nLastError = EPERM;
        }
#ifdef TMP_DEBUG
        {
          wxString sTmp(wxS("Shell script:\n"));
          sTmp.Append(sShellScript);
          sTmp.Append("written?: ");
          sTmp.Append(bRtn ? "yes" : "no");
          nwxLog::LogMessage(sTmp);
        }
#endif // TMP_DEBUG
      }
      if(!bRtn)
      {}
      else if(!wxFile::Exists(g_sAppleScript))
      {
        bRtn = false;
        m_nLastError = APPLE_SCRIPT_NOT_FOUND;
      }
      else
      {
        ARGV[0] = g_sAppleScript.wc_str();
        ARGV[1] = wxT("-e");
        ARGV[2] = sCmd.wc_str();
        ARGV[3] = NULL;
      }
    }
    if(bRtn)
    {
#ifdef TMP_DEBUG
      {
        wxString sTmp = "CSitePath::_runScript:";
        size_t i;
        for(i = 0; ARGV[i] != NULL; ++i)
        {
          sTmp.Append(wxS(" "));
          sTmp.Append(ARGV[i]);
        }
        nwxLog::LogMessage(sTmp);
      }
#endif // TMP_DEBUG
      long nRtn = wxExecute((wchar_t **)ARGV,wxEXEC_SYNC);
      if(!nRtn)
      {
        bRtn = true;
        m_nLastError = 0;
      }
      else if(nRtn > 0)
      {
        m_nLastError = EACCES;
      }
      else
      {
        m_nLastError = ENOENT;
      }
    }
  }
  return bRtn;
}

bool CSitePath::CreateSitePathSimple()
{
  bool bRtn = wxFileName::Mkdir(
    m_sSitePath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
  return bRtn;
}

bool CSitePath::CreateSitePath(const wxString &sGroup, const wxString &sUser)
{
  bool bRtn = true;
  m_nLastError = 0;
  if(SitePathExists())
  {
    bRtn = false;
    m_nLastError = EEXIST;
  }
  else
  {
    bRtn = CreateSitePathSimple();
    if(bRtn) {}
    else if(!m_bUserPath || m_bThisUserPath)
    {
      wxArrayString as;
      as.Add(wxT("MKDIR"));
      as.Add(m_sSitePath);
      as.Add(sGroup);
      if(!sUser.IsEmpty())
      {
        as.Add(sUser);
      }
      bRtn = _runScript(as, !bRtn);
    }
    else
    {
      bRtn = false;
      m_nLastError = EACCES;
    }
  }
#ifdef TMP_DEBUG
  wxString sMsg(wxS("CSitePath::CreateSitePath(\""));
  sMsg.Append(sGroup);
  sMsg.Append(wxS("\", \""));
  sMsg.Append(sUser);
  sMsg.Append(
    wxString::Format(
      wxS("\"), rtn = %s, m_nLastError = %d"),
      bRtn ? "true" : "false", m_nLastError));
  nwxLog::LogMessage(sMsg);
#endif
  return bRtn;
}

bool CSitePath::UpdateSitePath(const wxString &sGroup, const wxString &sUser)
{
  bool bRtn = true;
  bool bCreated = false;
  if(!SitePathExists())
  {
    bRtn = CreateSitePath(sGroup,sUser);
    if(bRtn)
    {
      _setupRealSitePath();
      bCreated = true;
    }
  }
  if(bRtn && !sGroup.IsEmpty() && !m_sRealSitePath.IsEmpty())
  {
    wxArrayString as;
    as.Add(wxS("CHGRP"));
    as.Add(m_sRealSitePath);
    as.Add(sGroup);
    bRtn = _runScript(as,!CurrentUserOwnsAll());
  }
  if(bRtn && !bCreated && sGroup.IsEmpty() && !sUser.IsEmpty())
  {
    wxArrayString as;
    as.Add(wxS("MKDIR"));
    as.Add(m_sRealSitePath);
    as.Add(wxEmptyString);
    as.Add(sUser);
    bRtn = _runScript(as,!CurrentUserOwnsAll());
  }
  return bRtn;
}


//  CSitePath::CurrentUserOwnsAll
//    traverse site path for one or more
//    files not owned by current user
//
//   returns true if no errors found and all files
//    are owned by the current user
//    otherwise it returns false
//  code is below MyTraverser class

class MyTraverser : public wxDirTraverser
{
public:
  MyTraverser() : m_nLastError(0), m_nUID(getuid()), m_bOwner(true)
    {}
  virtual ~MyTraverser() {}
  virtual wxDirTraverseResult OnFile	(	const wxString & 	filename	)	
  {
    wxDirTraverseResult rtn = wxDIR_CONTINUE;
    m_asList.Add(filename);
    if(stat(filename.utf8_str(),&m_statBuffer))
    {
      m_nLastError = errno;
      m_bOwner = false;
      rtn = wxDIR_STOP;
    }
    else if(m_statBuffer.st_uid != m_nUID)
    {
      m_bOwner = false;
      rtn = wxDIR_STOP;
    }
    return rtn;
  }
  virtual wxDirTraverseResult OnDir	(	const wxString & 	dirname	)
  {
    return OnFile(dirname);
  }
  virtual wxDirTraverseResult OnOpenError	(	const wxString & )
  {
    m_bOwner = false;
    m_nLastError = EACCES;
    return wxDIR_STOP;
  }
  bool IsOwner() const
  {
    return m_bOwner;
  }
  int LastError() const
  {
    return m_nLastError;
  }
  void Reset()
  {
    m_bOwner = true;
    m_nLastError = 0;
    m_asList.Empty();
  }
  const wxArrayString &DirList()
  {
    return m_asList;
  }
  
private:
  struct stat m_statBuffer;
  wxArrayString m_asList;
  int m_nLastError;
  uid_t m_nUID;
  bool m_bOwner;

};

bool CSitePath::CurrentUserOwnsAll()
{
  int bRtn = false;
  if(m_sRealSitePath.IsEmpty())
  {
    m_nLastError = EPERM;
  }
  else if(!SitePathExists())
  {
    m_nLastError = EEXIST;
  }
  else
  {
    wxDir oDir(m_sRealSitePath);
    if(!oDir.IsOpened())
    {
      m_nLastError = EPERM;
    }
    else
    {
      MyTraverser traverser;
      traverser.OnDir(m_sRealSitePath);
      const size_t n1 = (size_t)-1;
      size_t nCount = oDir.Traverse(traverser,wxEmptyString,
                    wxDIR_FILES |wxDIR_DIRS | wxDIR_HIDDEN);
      if(nCount == n1)
      {
        m_nLastError = EPERM;
      }
      else
      {
        bRtn = traverser.IsOwner();
        if(traverser.LastError())
        {
          m_nLastError = traverser.LastError();
        }
      }
#ifdef TMP_DEBUG
      wxString sTmp(wxS("CurrentUserOwnsAll()\ndirectory count = "));
      const wxArrayString &asList(traverser.DirList());
      sTmp.Append(wxString::Format(wxS("%d"),(int)nCount));
      sTmp.Append(wxS("\nreturn = "));
      sTmp.Append(bRtn ? wxS("true") : wxS("false"));
      sTmp.Append(
        wxString::Format(
          wxS("\ndir list(%d):\n"), int(asList.GetCount())
          )
        );
      for(size_t n = 0; n < asList.GetCount(); ++n)
      {
        sTmp.Append(wxString::Format
                    (wxS("\n  %d %s"),
                     (int)n, asList.Item(n).utf8_str()));
      }
      nwxLog::LogMessage(sTmp);
#endif // TMP_DEBUG

    }
  }
  return bRtn;
}

wxString CSitePath::GetSitePathGroup()
{
  const wxString &sRealPath = GetRealSitePath();
  wxString sRtn;
  struct stat statBuffer;
  if(sRealPath.IsEmpty())
  {}
  else if(stat(sRealPath.utf8_str(),&statBuffer))
  {
    m_nLastError = errno;
  }
  else
  {
    nwxUserInfo *pInfo = nwxUserInfo::GetGlobal();
    sRtn = pInfo->GetGroupNameFromID(statBuffer.st_gid);
  }
  return sRtn;
}
wxString CSitePath::GetExistingParent()
{
  return nwxFileUtil::GetExistingParent(m_sSitePath);
}

bool CSitePath::ExistingParentWritable()
{
  return nwxFileUtil::ExistingParentWritable(m_sSitePath);
}

void CSitePath::BuildTestString(wxString *ps) const
{
#define BOOL_TO_STR(b) ((b) ? wxT("true") : wxT("false"))
  ps->Empty();
  ps->Append(wxT("\nm_sExePath: "));
  ps->Append(m_sExePath);
  ps->Append(wxT("\nm_sSitePath: "));
  ps->Append(m_sSitePath);
  ps->Append(wxT("\nm_sRealSitePath: "));
  ps->Append(m_sRealSitePath);
  ps->Append(wxT("\nm_bUserPath: "));
  ps->Append(BOOL_TO_STR(m_bUserPath));
  ps->Append(wxT("\nm_bThisUserPath: "));
  ps->Append(BOOL_TO_STR(m_bThisUserPath));
  ps->Append(wxT("\nm_bXattrProblem: "));
  ps->Append(BOOL_TO_STR(m_bXattrProblem));
  ps->Append(wxT("\nm_nLastError: "));
  ps->Append(nwxString::FormatNumber(m_nLastError));
  ps->Append(wxT("\n\n"));
}

void CSitePath::LogTestString() const
{
  wxString s;
  BuildTestString(&s);
  nwxLog::LogMessage(s);
}

const wxString &CSitePath::LastErrorStr()
{
  switch(m_nLastError)
  {
  case 0:
    m_sErrorString.Empty();
    break;
  case SCRIPT_NOT_FOUND:
    m_sErrorString = wxS("The shell script:\n");
    m_sErrorString.Append(g_sScriptPath);
    m_sErrorString.Append(wxS("\nis missing"));
    break;
  case SCRIPT_NOT_EXECUTABLE:
    m_sErrorString = wxS("The shell script:\n");
    m_sErrorString.Append(g_sScriptPath);
    m_sErrorString.Append(wxS("\nis not executable"));
    break;
  case APPLE_SCRIPT_NOT_FOUND:
    m_sErrorString = wxS("Cannot find AppleScript: ");
    m_sErrorString.Append(g_sAppleScript);
    break;
  default:
    m_sErrorString = strerror(m_nLastError);
    break;
  }
  return m_sErrorString;
}

#endif
