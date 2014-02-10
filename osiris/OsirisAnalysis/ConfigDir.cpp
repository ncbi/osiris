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
#ifdef __WXMAC__
#include <wx/dir.h>
#endif

ConfigDir::ConfigDir(
  const char *psSubdir, 
  const char *psFileSubdir)
{
  wxStandardPaths &sp(wxStandardPaths::Get());
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
  _SetupSitePath();
}

ConfigDir::~ConfigDir() {;}


void ConfigDir::SetConfigSubdir(const char *psSubdir)
{
  m_sConfigPath = m_sTopConfigPath;
  if( (psSubdir != NULL) && *psSubdir )
  {    
    m_sSubdir = _T(psSubdir);
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
    m_sFileSubdir = _T(psSubdir);
    nwxFileUtil::NoStartWithSeparator(&m_sFileSubdir);
    nwxFileUtil::EndWithSeparator(&m_sFileSubdir);
    m_sFilePath += m_sFileSubdir;
  }
}

void ConfigDir::_SetupSitePath()
{

#ifdef __WXMSW__
  m_sSitePath = GetExePath();
  nwxFileUtil::EndWithSeparator(&m_sSitePath);
  m_sSitePath.Append(_T("site"));
#endif

#ifdef __WXMAC__
  wxString sEXE = GetExePath();
  const wxChar *psLib = _T("/Library/Application Support");
  const wxChar *psSubDir = _T("/Osiris-Files");
  if( sEXE.StartsWith(_T("/Applications/")) &&
      wxDir::Exists(psLib) )
  {
    m_sSitePath = psLib;
    m_sSitePath.Append(psSubDir);
  }
  else
  {
    wxString sUp3 = GetExePath();
    nwxFileUtil::UpDir(&sUp3,3);
    nwxFileUtil::NoEndWithSeparator(&sUp3);
    sUp3.Append(psSubDir);
    m_sSitePath = sUp3;
  }
#endif
}
