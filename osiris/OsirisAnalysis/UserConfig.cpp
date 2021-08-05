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
*  FileName: UserConfig.cpp
*  Author:   Douglas Hoffman
*
*  Class for copying user configuration files to the Osiris-Files directory
*    and opening the command prompt for using the osiris command line
*    utilities
*
*/
#include "UserConfig.h"
#include "CSitePath.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxString.h"
#include "mainApp.h"
#include "ConfigDir.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include <wx/utils.h> 

const wxString UserConfig::DEMO_FILES(wxT("demofiles"));
const wxString UserConfig::SEARCH_STRING(wxT("{OSIRISFILES}"));
const wxString UserConfig::EXE_STRING(wxT("{OSIRISEXEDIR}"));
#ifndef __WXMSW__
const wxString UserConfig::BACKSLASH(wxT("\\"));
const wxString UserConfig::FWDSLASH(wxT("/"));
#endif

bool UserConfig::UserConfigExists(bool bBuild, wxString *pPath)
{
  wxString sSitePath = CSitePath::GetGlobal()->GetRealSitePath();
  wxString sToolsPath;
  bool bRtn = (sSitePath.Len() > 0) && wxDirExists(sSitePath);
  if (bRtn)
  {
    ConfigDir *pConfig = mainApp::GetConfig();
    sToolsPath = pConfig->GetSiteToolsPath();
    bRtn = wxDirExists(sToolsPath);
    if (bBuild && !bRtn)
    {
      nwxFileUtil::MkDir(sToolsPath, true);
      bRtn = wxDirExists(sToolsPath);
      if (bRtn)
      {
        UserConfig x;
        bRtn = x._copyConfig();
      }
    }
    if ((pPath != NULL) && bRtn)
    {
      *pPath = sToolsPath;
    }
  }
  return bRtn;
}
#ifdef __WXMAC__
wxString UserConfig::_findTerminalApp()
{
  wxString sRtn;
  wxString sApp;
  wxString sMessage(wxT("Cannot find terminal app on macintosh\n"));
  wxString sCheck;
  const char *pListDir[] =
  {
      "/Applications/",
      "/Applications/Utilities/",
      "/System/Applications/",
      "/System/Applications/Utilities/",
      "~/Applications/",
      NULL
  };
  const char *pListApp[] =
  {
    "iTerm", "Terminal", NULL
  };
  const char **pApp;
  const char **pDir;
  for (pApp = pListApp; ((*pApp) != NULL) && sRtn.IsEmpty(); ++pApp)
  {
    for (pDir = pListDir; ((*pDir) != NULL) && sRtn.IsEmpty(); ++pDir)
    {
      sApp = (*pDir);
      sApp.Append(*pApp);
      sApp.Append(wxT(".app"));
      wxFileName fApp(sApp);
      fApp.Normalize(wxPATH_NORM_ABSOLUTE | wxPATH_NORM_TILDE |
        wxPATH_NORM_ENV_VARS | wxPATH_NORM_SHORTCUT);
      sApp = fApp.GetFullPath();
      sCheck = sApp;
      sCheck += "/Contents";
      if (wxFileName::IsDirReadable(sCheck))
      {
        sRtn = sApp;
      }
      else
      {
        sMessage += "\n  ";
        sMessage += sApp;
      }
    }
  }
  if(sRtn.IsEmpty())
  {
    mainApp::LogMessage(sMessage);
  }
  return sRtn;
}
#endif

bool UserConfig::OpenTerminal()
{
  bool bRtn = UserConfigExists();
  if (bRtn)
  {
    wxString sScript = mainApp::GetConfig()->GetExePath();
    nwxFileUtil::EndWithSeparator(&sScript);
#ifdef __WXMSW__
#define SCRIPTFILE wxT("oscmdline.bat")
#endif
#ifdef __WXMAC__
#define SCRIPTFILE wxT("oscmdline.sh")
#endif
    sScript.Append(SCRIPTFILE);
    if (wxFileName::IsFileReadable(sScript))
    {
      wxString sUserScript = mainApp::GetConfig()->GetConfigPath();
      nwxFileUtil::EndWithSeparator(&sUserScript);
      sUserScript.Append(SCRIPTFILE);
      bRtn = wxCopyFile(sScript, sUserScript, true) && _fixFile(sUserScript) &&
        wxFileName::IsFileReadable(sUserScript) && 
        _runScript(sUserScript);
    }
  }
  return bRtn;
}
bool UserConfig::_runScript(const wxString &sFileName)
{
#ifdef __WXMSW__
  return nwxFileUtil::OpenFileFromOS(sFileName);
#endif
#ifdef __WXMAC__
  wxString sAppPath = _findTerminalApp();
  bool bRtn = !sAppPath.IsEmpty();
  if (bRtn)
  {
    wxArrayString as;
    as.Add("-a");
    as.Add(sAppPath);
    as.Add(sFileName);
    bRtn = !nwxFileUtil::OpenCommandFromOS(as);
  }
  return bRtn;
#endif
}

bool UserConfig::_copyConfig()
{
  ConfigDir *pConfig = mainApp::GetConfig();
  m_sSrcDir = pConfig->GetExeToolsPath();
  m_sDestDir = pConfig->GetSiteToolsPath();
  nwxFileUtil::NoEndWithSeparator(&m_sSrcDir);
  nwxFileUtil::NoEndWithSeparator(&m_sDestDir);
  wxDir dir(m_sSrcDir);
  dir.Traverse(*this);
  if ((!m_bStatus) && wxDirExists(m_sDestDir))
  {
    // if the config tools directory was created and not successfully copied
    // then rename it
    int n = 0;
    wxString sRename = m_sDestDir + wxT(".") + nwxString::FormatNumber(n);
    while (wxFileExists(sRename) || wxDirExists(sRename))
    {
      ++n;
      sRename = m_sDestDir + wxT(".") + nwxString::FormatNumber(n);
    }
    wxRename(m_sDestDir, sRename);
  }
  return m_bStatus;
}
bool UserConfig::_fixDemoFile(const wxString &sFileName)
{
  bool bRtn = true;
  if (nwxString::ContainsNoCase(sFileName, DEMO_FILES))
  {
    bRtn = _fixFile(sFileName);
  }
  return bRtn;
}
bool UserConfig::_fixFile(const wxString &sFileName)
{
  wxFile f;
  wxString sContents;
  bool bRtn = true;
  if (!f.Open(sFileName, wxFile::read))
  {
    bRtn = false;
  }
  else if (!( f.ReadAll(&sContents) && f.Close() ))
  {
    bRtn = false;
  }
  else
  {
    bool bChange = false;
    if (sContents.Contains(SEARCH_STRING))
    {
      wxString sSitePath = CSitePath::GetGlobal()->GetRealSitePath();
      sContents.Replace(SEARCH_STRING, sSitePath, true);
      bChange = true;

    }
    if (sContents.Contains(EXE_STRING))
    {
      wxString s = mainApp::GetConfig()->GetExePath();
      sContents.Replace(EXE_STRING, s, true);
      bChange = true;
    }
#ifndef __WXMSW__
    if (sFileName.EndsWith(wxT(".sh")))
    {
      // do not replace backslash in a shell script
    }
    else if (sContents.Contains(BACKSLASH))
    {
      bChange = true;
      sContents.Replace(BACKSLASH, FWDSLASH, true);
    }
#endif
    if (!bChange)
    {
      // done
    }
    else if (f.Open(sFileName, wxFile::write))
    {
      const char *p = sContents.utf8_str();
      size_t nLen = strlen(p);
      bRtn = f.Write((void *)p, nLen) && f.Close();
    }
    else
    {
      bRtn = false;
    }
  }
  return bRtn;
}

#define RETURN return m_bStatus ? wxDIR_CONTINUE : wxDIR_STOP

wxDirTraverseResult UserConfig::OnDir(const wxString &dirname)
{
  wxString sNewDir(dirname);
  sNewDir.Replace(m_sSrcDir, m_sDestDir);
  if (dirname == sNewDir)
  {
    m_bStatus = false;
  }
  else if (!( wxDirExists(sNewDir) || nwxFileUtil::MkDir(sNewDir, true) ))
  {
    m_bStatus = false;
  }
  RETURN;
}
wxDirTraverseResult UserConfig::OnFile(const wxString& filename)
{
  wxString sNewFile(filename);
  sNewFile.Replace(m_sSrcDir, m_sDestDir);
  if (sNewFile == filename)
  {
    m_bStatus = false;
  }
  else if (!( wxFileExists(sNewFile) || wxCopyFile(filename, sNewFile, false) ))
  {
    m_bStatus = false;
  }
  else if (!_fixDemoFile(sNewFile))
  {
    m_bStatus = false;
  }
  RETURN;
}
wxDirTraverseResult UserConfig::OnOpenError(const wxString &)
{
  m_bStatus = false;
  RETURN;
}

