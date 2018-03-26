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

#ifdef __WXMAC__
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <unistd.h>
//#include <mach-o/dyld.h>
#endif

nwxIMPLEMENT_GLOBAL_OBJECT(CSitePath)

#define OSIRIS_FILES wxS("Osiris-Files")

// methods identical to windows and mac OSX

wxString CSitePath::GetExistingParent()
{
  return nwxFileUtil::GetExistingParent(m_sSiteDir);
}
bool CSitePath::ExistingParentWritable()
{
  return nwxFileUtil::ExistingParentWritable(m_sSiteDir);
}
bool CSitePath::SitePathExists() const
{
  bool bRtn = (!m_sSiteDir.IsEmpty()) &&
    wxFileName::DirExists(m_sSiteDir);
  return bRtn;
}

bool CSitePath::CreateSitePathSimple()
{
  bool bRtn = 
    SitePathExists() 
    ? true
    : wxFileName::Mkdir(
        m_sSiteDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
  if(bRtn)
  {
    _setupRealSitePath();
  }
  return bRtn;
}

void CSitePath::LogTestString() const
{
  wxString s;
  BuildTestString(&s);
  nwxLog::LogMessage(s);
}


void CSitePath::BuildTestString(wxString *ps) const
{
#define BOOL_TO_STR(b) ((b) ? wxS("true") : wxS("false"))
  ps->Empty();
  ps->Append(wxS("\nm_sExeDir: "));
  ps->Append(m_sExeDir);
  ps->Append(wxS("\nm_sSiteDir: "));
  ps->Append(m_sSiteDir);
  ps->Append(wxS("\nm_sRealSiteDir: "));
  ps->Append(m_sRealSiteDir);
  ps->Append(wxS("\nm_bUserPath: "));
  ps->Append(BOOL_TO_STR(m_bUserPath));
  ps->Append(wxS("\nm_bThisUserPath: "));
  ps->Append(BOOL_TO_STR(m_bThisUserPath));
#ifdef __WXMAC__
  ps->Append(wxS("\nm_bXattrProblem: "));
  ps->Append(BOOL_TO_STR(m_bXattrProblem));
#endif
  ps->Append(wxS("\nm_nLastError: "));
  ps->Append(nwxString::FormatNumber(m_nLastError));
#ifdef __WXMSW__
  ps->Append(wxS("\nm_sCScript: "));
  ps->Append(m_sCScript);
#endif
  ps->Append(wxS("\n\n"));
}

const wxString &CSitePath::LastErrorStr()
{
  switch(m_nLastError)
  {
  case 0:
    m_sErrorString.Empty();
    break;
#ifdef __WXMAC__
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
#endif
#ifdef __WXMSW__
  case SCRIPT_NOT_FOUND:
    m_sErrorString = wxS("Cannot find script, CSitePath.vbs");
    break;
  case WINDOWS_SCRIPT_HOST_NOT_FOUND:
    m_sErrorString = wxS("Cannot find Windows scripting host, cscript.exe");
    break;
#endif
  default:
    m_sErrorString = strerror(m_nLastError);
    break;
  }
  return m_sErrorString;
}

// ********************************************************* mac OS X
#ifdef __WXMAC__

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
  const wxString sSiteLib(wxS("/Library/Application Support/Osiris-Files"));
  const wxString sSiteShared(wxS("/Users/Shared/Osiris-Files"));
  const wxString sSharedHome(wxS("/Users/Shared"));
  const wxString sSlash(wxS("/"));
  int nApp;
  if(wxFileName::FileExists(sPath))
  {
    m_sExeDir = nwxFileUtil::GetRealPath(sPath);
    if(m_sExeDir.IsEmtpy())
    {
      m_nLastError = errno;
      m_sExeDir = sPath;
    }
  }
  else
  {
    wxString sMsg(wxS("exe path does not exist: "));
    sMsg.Append(sPath);
    nwxLog::LogMessage(sMsg);
    m_sExeDir = sPath;
  }
  wxString sExeLower(m_sExeDir);
  sExeLower.MakeLower();
  m_bXattrProblem = !sExeLower.Find("/private/var");
  //  if exe file is in /private/var, then OS X 10.12 or later
  //  moved it there upon execution
  //   the fix for this is to run xattr -r -c 'Osiris.app'

//  figure out path for Osiris-Files
//  based on application location

  if(m_bXattrProblem || !m_sExeDir.find(wxS("/Applications/")))
  {
    // OSIRIS is in /Applications or unknown (/private/var)
    // look in /Library/Application Support
    //  and /Users/Shared

    if(wxFileName::DirExists(sSiteLib))
    {
      // compatible with older versions
      m_sSiteDir = sSiteLib;
    }
    else if(wxFileName::DirExists(sSharedHome))
    {
      // preferred location is /Users/Shared/Osiris-Files
      m_sSiteDir = sSiteShared;
    }
    else
    {
      // fallback if /Users/Shared does not exist
      m_sSiteDir = sSiteLib;
    }
  }
  else if( (nApp = sExeLower.Find(wxS(".app"))) > 0 )
  {
    // look for existing Osiris-Files directory in parent
    //  of .app directory

    wxString sApp(m_sExeDir.Mid(0,nApp + 4));
    wxFileName fnApp(sApp);
    wxString sDirSibling = fnApp.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    sDirSibling.Append(OSIRIS_FILES);
    if(wxFileName::DirExists(sDirSibling))
    {
      m_sSiteDir = sDirSibling;
    }
    if ( (!m_sExeDir.Find(wxS("/Users/"))) &&
         (!(m_sExeDir.Find(sSharedHome + wxS("/")) == 0)) &&
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
        m_bThisUserPath = (m_sExeDir.Len() > nLenHome) &&
          (m_sExeDir.Left(nLenHome) == sHome);
        if(m_sSiteDir.IsEmpty())
        {
          m_sSiteDir = sApp.Mid(0,nSlash3); // no trailing slash
          m_sSiteDir.Append(sSiteLib);
        }
      }
    }
    if(m_sSiteDir.IsEmpty())
    {
      // not a user directory nor Applications,
      // possible network install
      m_sSiteDir = sDirSibling;
    }
  }
  _setupRealSitePath();
}

void CSitePath::_setupRealSitePath()
{
  char buffer[PATH_MAX + 1];
  if(!m_sRealSiteDir.IsEmpty()) {}
  else if(m_sSiteDir.IsEmpty()) {}
  else if(!wxFileName::DirExists(m_sSiteDir)) {}
  else
  {
    m_sRealSiteDir = nwxFileUtil::GetRealPath(m_sSiteDir);
    if(m_sRealSiteDir.IsEmpty())
    {
      m_nLastError = errno;
    }
  }
}

/*
bool CSitePath::_appendArg(wxString *pStr, const wxString &sArg)
{
  bool bRtn = (sArg.Find(wxS("\"")) == wxNOT_FOUND);
  // cannot process a directory name with a double quote "

  //   (sArg.Find(wxS("'")) == wxNOT_FOUND) );

  if(bRtn)
  {
    pStr->Append(wxS(" "));
    if(sArg.Find(wxS(" ")) != wxNOT_FOUND)
    {
      pStr->Append(wxS("'"));
      pStr->Append(sArg);
      pStr->Append(wxS("'"));
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
  wxFileName fnExe(m_sExeDir);
  wxString sCmd;
  wxString sArg;
  wxString sBack(wxS("\\"));
  wxString sBack2(wxS("\\\\"));
  bool bRtn = false;

  if(g_sScriptPath.IsEmpty())
  {
    g_sScriptPath = fnExe.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    g_sScriptPath.Append(wxS("CSitePath.sh"));
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
        ARGV[1] = wxS("-e");
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


bool CSitePath::CreateSitePath(const wxString &sGroup, const wxString &sUser)
{
  bool bRtn = true;
  m_nLastError = 0;
  if(SitePathExists())
  {
    bRtn = false;
    m_nLastError = EEXIST;
  }
  else if(m_bUserPath && !m_bThisUserPath)
  {
    bRtn = false;
    m_nLastError = EACESS;
  }
  else
  {
    bRtn = CreateSitePathSimple();
    if(!m_bUserPath || m_bThisUserPath)
    {
      wxArrayString as;
      as.Add(bRtn ? wxS("CHGRP") : wxS("MKDIR"));
      as.Add(m_sSiteDir);
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

bool CSitePath::UpdateSitePath(const wxString &sGroup, const wxString &sUser, bool bNeedAdmin)
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
  if(bRtn && !sGroup.IsEmpty() && !m_sRealSiteDir.IsEmpty())
  {
    wxArrayString as;
    as.Add(wxS("CHGRP"));
    as.Add(m_sRealSiteDir);
    as.Add(sGroup);
    bRtn = _runScript(as,bNeedAdmin);
  }
  if(bRtn && !bCreated && sGroup.IsEmpty() && !sUser.IsEmpty())
  {
    wxArrayString as;
    as.Add(wxS("MKDIR"));
    as.Add(m_sRealSiteDir);
    as.Add(wxEmptyString);
    as.Add(sUser);
    bRtn = _runScript(as,bNeedAdmin);
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
  if(m_sRealSiteDir.IsEmpty())
  {
    m_nLastError = EPERM;
  }
  else if(!SitePathExists())
  {
    m_nLastError = EEXIST;
  }
  else
  {
    wxDir oDir(m_sRealSiteDir);
    if(!oDir.IsOpened())
    {
      m_nLastError = EPERM;
    }
    else
    {
      MyTraverser traverser;
      traverser.OnDir(m_sRealSiteDir);
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





#endif

// ********************************************************* Windows

#ifdef __WXMSW__

void CSitePath::_getPeerPath(wxString *ps)
{
  // retrieve full path of Osiris-File
  // with same parent as the installation
  // directory
  *ps = m_sExeDir;
  nwxFileUtil::UpDir(ps);
  nwxFileUtil::EndWithSeparator(ps);
  ps->Append(OSIRIS_FILES);
}

bool CSitePath::_checkLegacyPath()
{
  // if legacy site dir exists, use it
  //   "site" subdirectory in Osiris installation directory

  wxString sSiteLegacy(m_sExeDir);
  bool bRtn = false;
  sSiteLegacy.Append(wxS("site"));
  if(wxFileName::DirExists(sSiteLegacy))
  {
    bRtn = true;
    m_sSiteDir = sSiteLegacy;
  }
  else
  {
    _getPeerPath(&sSiteLegacy);
    if(wxFileName::DirExists(sSiteLegacy))
    {
      bRtn = true;
      m_sSiteDir = sSiteLegacy;
    }
  }
  return bRtn;
}
void CSitePath::_setupUserPath()
{
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  m_sSiteDir = sp.GetUserConfigDir();
  nwxFileUtil::EndWithSeparator(&m_sSiteDir);
  m_sSiteDir.Append(OSIRIS_FILES);
  m_bUserPath = true;
  m_bThisUserPath = true;
}
bool CSitePath::_checkUserPath()
{
  // if OSIRIS is in the user's home path
  //  put 'Osiris-Files' in AppData\roaming
  // first check current user
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxString sHomeDir = wxGetHomeDir();
  nwxFileUtil::EndWithSeparator(&sHomeDir);
  wxString sUserConfig = sp.GetUserConfigDir();
  size_t nLenHome = sHomeDir.Len();
  size_t nLenConfig = sUserConfig.Len();
  size_t nLenExe = m_sExeDir.Len();
  bool bRtn = false;
  if(!(
    (nLenHome > 0) &&
    (nLenHome < nLenConfig) &&
    nwxString::FileNameStringEqual(sUserConfig.Left(nLenHome),sHomeDir)
    ))
  {
    // if config dir is not in home dir, forget it
  }
  else if(
      (nLenExe > nLenHome) &&
      nwxString::FileNameStringEqual(sHomeDir,m_sExeDir.Left(nLenHome))
    )
  {
    // osiris is installed in THIS user's path
    m_sSiteDir = sUserConfig;
    nwxFileUtil::EndWithSeparator(&m_sSiteDir);
    m_sSiteDir.Append(OSIRIS_FILES);
    m_bUserPath = true;
    m_bThisUserPath = true;
    bRtn = true;
  }
  else
  {
    // check other user's directory
    // first guess at home directory
    wxString sUsers = sHomeDir;
    nwxFileUtil::UpDir(&sUsers);
    nwxFileUtil::EndWithSeparator(&sUsers); // users parent dir
    size_t nUsersLen = sUsers.Len();
    if(  (nUsersLen < nLenExe) &&
         nwxString::FileNameStringEqual(sUsers,m_sExeDir.Left(nUsersLen)) 
      )
    {
      // looks like other user's dir
      // get owner home and append current user config subdir
      size_t nSep = m_sExeDir.Mid(nUsersLen).Find(sUsers.Right(1));
      if(nSep != wxNOT_FOUND)
      {
        wxString sConfigSubDir = sUserConfig.Mid(nLenHome); // user config subdir
        wxString sOwnerConfig = m_sExeDir.Left(nUsersLen + nSep + 1); // owner home
        sOwnerConfig.Append(sConfigSubDir); // owner config dir
#ifdef TMP_DEBUG
        {
          wxString s;
          s.Append(wxS("\nuser config: "));
          s.Append(sUserConfig);
          s.Append(wxS("\nowner config: "));
          s.Append(sOwnerConfig);
          nwxLog::LogMessage(s);
        }
#endif
        if(wxFileName::DirExists(sOwnerConfig))
        {
          m_sSiteDir = sOwnerConfig;
          nwxFileUtil::EndWithSeparator(&m_sSiteDir);
          m_sSiteDir.Append(OSIRIS_FILES);
          m_bUserPath = true;
          m_bThisUserPath = false;
          bRtn = true;
        }
      }
    }
  }
  return bRtn;
}

bool CSitePath::_checkProgramData()
{
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxString sConfig = sp.GetConfigDir();
  nwxFileUtil::NoEndWithSeparator(&sConfig);
  if(sConfig.find('\\') != sConfig.rfind('\\'))
  {
    nwxFileUtil::UpDir(&sConfig);
  }
  bool bRtn = wxFileName::DirExists(sConfig);
  if(bRtn)
  {
    nwxFileUtil::EndWithSeparator(&sConfig);
    sConfig.Append(OSIRIS_FILES);
    m_sSiteDir = sConfig;
  }
  return bRtn;
}

void CSitePath::_setupRealSitePath()
{
  if(!m_sRealSiteDir.IsEmpty()) {}
  else if(m_sSiteDir.IsEmpty()) {}
  else if(!wxFileName::DirExists(m_sSiteDir)) {}
  else 
  {
    m_sRealSiteDir = nwxFileUtil::GetRealPath(m_sSiteDir);
  }
}

CSitePath::CSitePath() :
  m_nLastError(0),
  m_bUserPath(false),
  m_bThisUserPath(false)
{
  // constructor.  Get path for executable, figure out where
  //  site folder (Osiris-Files) should be
  //  not sure what will happen with sym links
  
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxFileName fn(sp.GetExecutablePath());
  wxString sPath = fn.GetFullPath(wxPATH_NATIVE);
  m_sExeDir = fn.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME);
  m_sCScript = nwxFileUtil::PathFind(wxS("cscript.exe"),true,false);

  if(wxFileName::FileExists(m_sCScript))
  {
    m_sVBscript = m_sExeDir;
    nwxFileUtil::EndWithSeparator(&m_sVBscript);
    m_sVBscript.Append(wxS("CSitePath.vbs"));
    if(!wxFileName::FileExists(m_sVBscript))
    {
      wxString sMsg(wxS("Cannot find script: "));
      sMsg.Append(m_sVBscript);
      nwxLog::LogMessage(sMsg);
      m_sVBscript.Clear();
    }
  }
  else
  {
    nwxLog::LogMessage(wxS("Cannot find cscript.exe"));
  }

  // check for legacy location

  if(!_checkLegacyPath())
  {
    wxFSVolumeKind nDriveKind = nwxFileUtil::GetMSWDriveType(m_sExeDir);
    if(nwxFileUtil::IsMSWDriveReadOnly(nDriveKind))
    {
      // optical drives are assumed to be read only
      // use c:\Users\username\AppData\Roaming
      _setupUserPath();
    }
    else if(nwxFileUtil::IsMSWDriveNetwork(nDriveKind))
    {
      _getPeerPath(&m_sSiteDir);
    }
    else if(_checkUserPath())
    {}
    else if(_checkProgramData())
    {}
    else
    {
      _getPeerPath(&m_sSiteDir);
    }
  }
  _setupRealSitePath();
}

bool CSitePath::CurrentUserOwnsAll()
{
  bool bRtn = false;
  _setupRealSitePath();
  if(!m_sRealSiteDir.IsEmpty())
  {
    wxArrayString as;
    as.Alloc(2);
    as.Add(wxS("ISOWNER"));
    as.Add(m_sRealSiteDir.wx_str());
    bRtn = _runScript(as,false);
  }
  return bRtn;
}

bool CSitePath::_runScript(const wxArrayString &as, bool bElevate)
{
  bool bRtn = false;
  if(m_sCScript.IsEmpty())
  {
    m_nLastError = WINDOWS_SCRIPT_HOST_NOT_FOUND;
  }
  else if(m_sVBscript.IsEmpty())
  {
    m_nLastError = SCRIPT_NOT_FOUND;
  }
  else
  {
    const wchar_t *ARGV[24];
    wxString sTmp;
    size_t ndx = 3;
    size_t nSize = as.GetCount();
    ARGV[0] = m_sCScript.wx_str();
    ARGV[1] = wxS("/NOLOGO");
    ARGV[2] = m_sVBscript.wx_str();
    if(bElevate)
    {
      const wxString &sTmp = mainApp::GetConfig()->GetConfigPath();
      ARGV[3] = wxS("-r");
      ARGV[4] = sTmp.wx_str();
      ndx = 5;
    }
    for(size_t i = 0; i < nSize; ++i)
    {
      ARGV[ndx++] = as.Item(i).wx_str();
    }
    size_t nRtn = wxExecute((wchar_t **)ARGV,wxEXEC_SYNC);
    bRtn = !nRtn;
    if(!bRtn)
    {
      wxString sMsg;
      wxString sArg;
      sMsg.Printf(wxS("Shell failed, errorlevel %d:\n"),(int) nRtn);
      for(const wchar_t **pArg = ARGV; (*pArg) != NULL; ++pArg)
      {
        sArg = *pArg;
        if(sArg.IsEmpty())
        {
          sMsg.Append(wxS("\"\" "));
        }
        else if(sArg.Find(wxS(" ")) != wxNOT_FOUND)
        {
          sMsg.Append(wxS("\""));
          sMsg.Append(sArg);
          sMsg.Append(wxS("\" "));
        }
        else
        {
          sMsg.Append(sArg);
          sMsg.Append(wxS(" "));
        }
      }
      nwxLog::LogMessage(sMsg);
    }
  }
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
    if(CreateSitePathSimple())
    {
      bRtn = true;
    }
    else if(!m_bUserPath || m_bThisUserPath)
    {
      wxArrayString as;
      as.Alloc(4);
      as.Add(wxS("MKDIR"));
      as.Add(m_sSiteDir);
      as.Add(sGroup);
      if(!sUser.IsEmpty())
      {
        as.Add(sUser);
      }
      bRtn = _runScript(as, true);
    }
    else
    {
      bRtn = false;
      m_nLastError = EACCES;
    }
  }
  if(bRtn)
  {
    _setupRealSitePath();
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

bool CSitePath::UpdateSitePath(const wxString &sGroup, const wxString &sUser, bool bElevate)
{
  bool bRtn = true;
  if(!SitePathExists())
  {
    bRtn = CreateSitePath(sGroup,sUser);
  }
  else if(sGroup.IsEmpty() && sUser.IsEmpty())
  {
    bRtn = false;
  }
  else
  {
    wxArrayString as;
    as.Alloc(4);
    as.Add(wxS("CHGRP"));
    as.Add(m_sSiteDir);
    as.Add(sGroup);
    if(!sUser.IsEmpty())
    {
      as.Add(sUser);
    }
    bRtn = _runScript(as, bElevate);
  }
  return bRtn;
}
bool CSitePath::IsNTFS()
{
  bool bRtn = false;
  wxString sPath;
  if(SitePathExists())
  {
    _setupRealSitePath();
    sPath = m_sRealSiteDir;
  }
  if(sPath.IsEmpty())
  {
    sPath = GetExistingParent();
  }
  if(!sPath.IsEmpty())
  {
    wxArrayString as;
    as.Alloc(2);
    as.Add(wxS("GETOWNER"));
    as.Add(m_sRealSiteDir);
    bRtn = _runScript(as,false);
  }
  return bRtn;
}

#endif
