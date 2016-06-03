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

*  FileName: ConfigDir.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __CONFIG_DIR_H__
#define __CONFIG_DIR_H__

#include <wx/string.h>
#include <wx/filename.h>
#include "nwx/nwxFileUtil.h"

class ConfigDir
{
public:
  ConfigDir(
    const char *psConfigSubdir = NULL,
    const char *psFileSubdir = NULL);
  virtual ~ConfigDir();
  void SetConfigSubdir(const char *psSubdir);
  void SetFileSubdir(const char *psSubdir);


  const wxString &GetExePath() const
  {
    return m_sExePath;
  }
  const wxString &GetExeConfigPath() const
  {
    return m_sExeConfigPath;
  }
  const wxString &GetExeVolumePath() const
  {
    return m_sExeVolumePath;
  }
  const wxString &GetConfigPath(const char *psSubdir)
  {
    if(psSubdir != NULL)
    {
      SetConfigSubdir(psSubdir);
    }
    return m_sConfigPath;
  }
  const wxString &GetSitePath() const
  {
    return m_sSitePath;
  }
  const wxString &GetConfigPath() const
  {
    return m_sConfigPath;
  }
  const wxString &GetFilePath(const char *psFileSubdir)
  {
    if(psFileSubdir != NULL)
    {
      SetFileSubdir(psFileSubdir);
    }
    return m_sFilePath;
  }
  const wxString &GetFilePath() const
  {
    return m_sFilePath;
  }
  bool IsOK() const
  {
    return !m_bError;
  }
  wxString GetLadderFileName() const
  {
    // Osiris v 1.x
    wxString sRtn = GetExeConfigPath();
    nwxFileUtil::EndWithSeparator(&sRtn);
    sRtn.Append("LadderData.xml");
    return sRtn;
  }
  wxString GetILSLadderFilePath() const
  {
    // Osiris v 2.1+
    wxString sRtn = GetExeConfigPath();
    sRtn.Append("LadderSpecifications");
    nwxFileUtil::EndWithSeparator(&sRtn);
    return sRtn;
  }
  wxString GetILSLadderFileName(const wxString &s) const 
  {
    // Osiris v 2.x
    wxString sRtn = GetILSLadderFilePath();
    sRtn.Append(s);
    return sRtn;
  }
  wxString GetILSLadderFileName() const
  {
    // Osiris v 2.x
    return GetILSLadderFileName("ILSAndLadderInfo.xml");
  }
  wxString GetMessageBookFileName() const
  {
    wxString sRtn = GetILSLadderFilePath();
    sRtn.Append("MessageBook.xml");
    return sRtn;
  }
#define ARTIFACT_FILE_NAME wxT("ArtifactLabels.xml")
  wxString LocalArtifactLabelsFileName() const
  {
    wxString sRtn;
    _BuildFileName(GetSitePath(),ARTIFACT_FILE_NAME,&sRtn);
    return sRtn;
  }
  wxString FindArtifactLabelsFileName() const
  {
    wxString sRtn;
    if(!_FindFileName(ARTIFACT_FILE_NAME,&sRtn))
    {
      sRtn.Clear();
    }
    return sRtn;
  }
#undef ARTIFACT_FILE_NAME
#ifdef __WXDEBUG__
  void Log();
#endif

private:

  wxString m_sExePath;
    // full path of argv[0] w/o program name
  wxString m_sExeConfigPath;
    // m_sExePath + "Config"
  wxString m_sExeVolumePath;
    // m_sExeConfigPath + "Volumes"
  wxString m_sConfigPath;
    // full path for config files m_sTopConfigPath + m_sSubdir
  wxString m_sFilePath;
    // full path for data files m_sTopFilePath + m_sFileSubdir
  wxString m_sTopConfigPath; 
    // top level directory for config files (${HOME}/Application Data/.osiris)
  wxString m_sTopFilePath;
    // top level directory for data files (${HOME}/My Documents/Osiris)
  wxString m_sSubdir;
    // Subdir for config files (.osiris)
  wxString m_sFileSubdir;
    // Subdir for data files (Osiris)
  wxString m_sSitePath;
    // Subdir for user config files, operating procedures (volumes), etc.
  bool m_bError;
    // true is an error occurred when creating a directory
  void _SetupSitePath();

  static bool _BuildFileName(const wxString &sPath, const wxChar *psFileName, wxString *psRtn)
  {
    // load the file name in psRtn whether or not it exists
    wxString sRtn = sPath;
    nwxFileUtil::EndWithSeparator(&sRtn);
    sRtn.Append(psFileName);
    *psRtn = sRtn;
    bool bRtn = wxFileName::IsFileReadable(sRtn);
    return bRtn;
  }
  bool _FindFileName(const wxChar *psFileName, wxString *psRtn) const
  {
    // load the file name only if it exists
    wxString sRtn;
    bool bRtn = _BuildFileName(GetSitePath(),psFileName,&sRtn);
    if(!bRtn)
    {
      bRtn = _BuildFileName(GetILSLadderFilePath(),psFileName,&sRtn);
    }
    if(bRtn)
    {
      *psRtn = sRtn;
    }
    return bRtn;
  }

};

#endif
