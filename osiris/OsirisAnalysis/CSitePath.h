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
*  Date:     2018/02/05
*
*  Performs operations needed for the site path (Osiris-Files folder)
*  in the Macintosh version of OSIRIS. - see windows version below
*
*  The location of the Osiris-Files folder depends on the installation
*  location of Osiris.
*
*  Older versions of OSIRIS:
*    If Osiris is installed in the /Applications directory,
*    then Osiris-Files is created in /Library/Application Support
*    otherwise it is in the same directory as Osiris*.app
*
*  Future:
*    First look for Osiris-Files in the locations described above
*    If found, leave it alone.
*    If it doesn't exist, then use the following logic:
*
*    If Osiris is installed in /Users/username/ then
*     use /Users/username/Library/Application Support
*    If Osiris is installed in /Applications
*     use /Library/Application Support (no change)
*     Other possibilities:
*       /Users/Shared (if writable)
*       /Users/Shared/Library/Application Support
*    If Osiris is installed elsewhere (eg on a network), use
*     the same directory as that containing Osiris*.app
*
*    Whenever the user does not have needed permissions for an operation,
*    the operation will be done from a shell script run as follows:
*
*    osascript -e "do shell script 'script with args' with administrator privileges"
*
*/
#ifndef __C_SITE_PATH_H__
#define __C_SITE_PATH_H__

#include <wx/string.h>
#include <wx/arrstr.h>
#include "nwx/nwxGlobalObject.h"
#include <errno.h>

#ifdef __WXMAC__


class CSitePath
{
private:
  CSitePath();
public:
  enum SITE_PATH_ERROR
  {
    BEGIN = ELAST, // highest number in errno.h
    SCRIPT_NOT_FOUND,
    SCRIPT_NOT_EXECUTABLE,
    APPLE_SCRIPT_NOT_FOUND
  };
  virtual ~CSitePath()
  {
    _unlinkTmpShellScript();
  }
  wxString GetExistingParent();
  // get the closest 'ancestor' to site path that exists
  //  eg. if site path is /Users/joe/Library/Application Support/Osiris-Files
  //    and /Users/joe/Library doesn't exist, return /Users/joe
  //  if 'site path' exists, return it

  bool ExistingParentWritable();

  wxString GetSitePathGroup();
  const wxString &GetSitePath() const
  {
    return m_sSiteDir;
  }
  const wxString &GetRealSitePath()
  {
    _setupRealSitePath();
    return m_sRealSiteDir;
  }
  bool XattrProblem() const
  {
    return m_bXattrProblem;
  }
  bool SitePathExists() const;
  bool CreateSitePath(const wxString &sGroup, const wxString &sUser);
  bool CreateSitePathSimple();
  bool CurrentUserOwnsAll();
  bool UpdateSitePath(const wxString &sGroup, const wxString &sUser, bool bNeedAdmin = false);
  //  bool UpdateFile(const wxString &sTempPath, const wxString &sRealPath);
  //  bool ShowPath() const; // show Osiris-Files in Finder
  int LastError() const
  {
    return m_nLastError;
  }
  const wxString &LastErrorStr();
  void BuildTestString(wxString *ps) const;
  void LogTestString() const;
private:
  const wxString &_getTmpShellScript();
  bool _unlinkTmpShellScript();
  //  static bool _appendArg(wxString *pStr, const wxString &sArg);
  static const wxString &_prepareShellParm
    (const wxString &s, wxString *psBuffer);
  bool _runScript(const wxArrayString &pas, bool bAsAdmin);
  void _setupRealSitePath();
  wxString m_sExeDir;
  wxString m_sSiteDir;
  wxString m_sRealSiteDir;
  wxString m_sTmpShellScript;
  wxString m_sErrorString;
  int m_nLastError;
  bool m_bUserPath;
  bool m_bThisUserPath;
  bool m_bXattrProblem;

  static const wxString g_sAppleScript;
  static wxString g_sScriptPath;
  nwxDECLARE_GLOBAL_OBJECT(CSitePath);
};

#endif


#ifdef __WXMSW__

#include <wx/volume.h>

class CSitePath
{
private:
  CSitePath();
public:
  enum SITE_PATH_ERROR
  {
    BEGIN = 1000, // highest number in errno.h
    SCRIPT_NOT_FOUND,
    WINDOWS_SCRIPT_HOST_NOT_FOUND,
    WINDOWS_EXE_IS_MISSING
  };
  virtual ~CSitePath() {}
  const wxString &GetSitePath() const
  {
    return m_sSiteDir;
  }
  const wxString &GetRealSitePath()
  {
    _setupRealSitePath();
    return m_sRealSiteDir;
  }
  int LastError() const
  {
    return m_nLastError;
  }
  wxString GetExistingParent();
  bool ExistingParentWritable();
  bool SitePathExists() const;
  bool CreateSitePath(const wxString &sGroup, const wxString &sUser);
  bool CreateSitePathSimple();
  bool CurrentUserOwnsAll();
  bool UpdateSitePath(const wxString &sGroup, const wxString &sUser, bool bNeedAdmin = false);
  const wxString &LastErrorStr();
  void BuildTestString(wxString *ps) const;
  void LogTestString() const;
  bool IsNTFS();
  bool IsOK() const
  {
    return !m_bExeMissing;
  }
  bool IsExeMissing() const
  {
    return m_bExeMissing;
  }
private:
  bool _checkLegacyPath();
  bool _checkUserPath();
  bool _checkProgramData();
  void _setupUserPath();
  void _getPeerPath(wxString *ps);
  void _setupRealSitePath();
  bool _runScript(const wxArrayString &pas, bool bAsAdmin);
  wxString m_sExeDir;   // directory of exe (argv[0]) with trailing space
  wxString m_sSiteDir;
  wxString m_sRealSiteDir; // UNC if network drive
  wxString m_sErrorString;
  wxString m_sCScript;
  wxString m_sVBscript;
  int m_nLastError;
  bool m_bUserPath;
  bool m_bThisUserPath;
  bool m_bExeMissing;
  static const wxString g_sCScript; // path of cscript.exe
  static wxString g_sScriptPath;

  nwxDECLARE_GLOBAL_OBJECT(CSitePath);
};


#endif

#endif
