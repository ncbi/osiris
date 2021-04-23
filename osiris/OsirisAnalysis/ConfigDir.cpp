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

*  FileName: ConfigDir.cpp
*  Author:   Douglas Hoffman
*
*/
#include "ConfigDir.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include "CSitePath.h"
#include "nwx/nwxFileUtil.h"
#ifdef __WXDEBUG__
#include "mainApp.h"
void ConfigDir::Log()
{
  struct
  {
    wxString m_sName;
    wxString m_sValue;
  } LIST[] =
  {
    {wxS("m_sExeVolumePath = "), m_sExeVolumePath},
    {wxS("m_sConfigPath = "), m_sConfigPath},
    {wxS("m_sFilePath = "), m_sFilePath},
    {wxS("m_sTopConfigPath = "), m_sTopConfigPath},
    {wxS("m_sTopFilePath = "), m_sTopFilePath},
    {wxS("m_sSubdir = "), m_sSubdir},
    {wxS("m_sFileSubdir = "), m_sFileSubdir},
    {wxS("m_sSitePath = "), GetSitePath()}
  };
  size_t SLIST = sizeof(LIST) / sizeof(LIST[0]);
  size_t i;
  wxString s;
  time_t t; time(&t);
  for(i = 0; i < SLIST; ++i)
  {
    s = LIST[i].m_sName;
    s += LIST[i].m_sValue;
    mainApp::LogMessage(s);
  }
  s = wxS("m_bError = ");
  s += m_bError ? wxS("true") : wxS("false");
  mainApp::LogMessage(s);
}
#endif
ConfigDir::ConfigDir(
  const char *psSubdir, 
  const char *psFileSubdir)
{
  wxStandardPathsBase &sp(wxStandardPaths::Get());
  wxFileName fn(sp.GetExecutablePath());
  m_sExePath = fn.GetPath(
    wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR, 
    wxPATH_NATIVE);
  nwxFileUtil::EndWithSeparator(&m_sExePath);
  m_sExeConfigPath = m_sExePath;
  m_sExeConfigPath += "Config";
  nwxFileUtil::EndWithSeparator(&m_sExeConfigPath);
  m_sExeVolumePath = m_sExeConfigPath;
  m_sExeVolumePath += "Volumes";
  nwxFileUtil::EndWithSeparator(&m_sExeVolumePath);

  m_sTopConfigPath = sp.GetUserConfigDir();
  nwxFileUtil::EndWithSeparator(&m_sTopConfigPath);
  m_sTopFilePath = sp.GetDocumentsDir();
  nwxFileUtil::EndWithSeparator(&m_sTopFilePath);
  SetConfigSubdir(psSubdir);
  SetFileSubdir(psFileSubdir);
}

ConfigDir::~ConfigDir() {;}


void ConfigDir::SetConfigSubdir(const char *psSubdir)
{
  m_sConfigPath = m_sTopConfigPath;
  if( (psSubdir != NULL) && *psSubdir )
  {    
    m_sSubdir = psSubdir;
    nwxFileUtil::NoStartWithSeparator(&m_sSubdir);
    nwxFileUtil::EndWithSeparator(&m_sSubdir);
    m_sConfigPath += m_sSubdir;
  }
  m_bError = !wxFileName::Mkdir(m_sConfigPath,0755,wxPATH_MKDIR_FULL);
}


void ConfigDir::SetFileSubdir(const char *psSubdir)
{
  m_sFilePath = m_sTopFilePath;
  if( (psSubdir != NULL) && *psSubdir )
  {   
    m_sFileSubdir = psSubdir;
    nwxFileUtil::NoStartWithSeparator(&m_sFileSubdir);
    nwxFileUtil::EndWithSeparator(&m_sFileSubdir);
    m_sFilePath += m_sFileSubdir;
  }
}

const wxString &ConfigDir::GetSitePath() const
{
  return CSitePath::GetGlobal()->GetSitePath();
}

size_t ConfigDir::GetSiteILSFileNames(std::set<wxString> *pSetFiles) const
{
  wxString sRtn;
  wxString sPath = GetSiteILSLadderFilePath();
  wxString sFullName;
  wxDir  dir(sPath);
  pSetFiles->clear();
  if (dir.IsOpened())
  {
    wxString sFileName;
    bool b;
    const int DIR_FLAGS = wxDIR_FILES | wxDIR_HIDDEN | wxDIR_NO_FOLLOW;
    for (b = dir.GetFirst(&sFileName, wxT("*_ILSInfo.xml"), DIR_FLAGS);
      b; b = dir.GetNext(&sFileName))
    {
      sFullName = sPath + sFileName;
      if (wxFileName::IsFileReadable(sFullName) && !nwxFileUtil::IsMacAttrFile(sFullName))
      {
        pSetFiles->insert(sFullName);
      }
    }
  }
  return pSetFiles->size();
}

