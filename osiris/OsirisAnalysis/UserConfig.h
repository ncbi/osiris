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
*  FileName: UserConfig.h
*  Author:   Douglas Hoffman
*
*  Class for copying user configuration files to the Osiris-Files directory
*    and opening the command prompt for using the osiris command line
*    utilities
*
*/
#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
#include <wx/dir.h>

class UserConfig : public wxDirTraverser
{
private:
  UserConfig() : m_bStatus(true) {} // allow instantiation in static functions
  bool _copyConfig();
  static bool _fixDemoFile(const wxString &sFileName);
  static bool _fixFile(const wxString &sFileName);
#ifdef __WXMAC__
  static wxString _findTerminalApp();
#endif
  wxString m_sSrcDir;
  wxString m_sDestDir;
  bool m_bStatus;
#ifndef __WXMSW__
  static bool g_bEnvSetup;
#endif
  static const wxString DIR_NAME;
  static const wxString SRC_DIR_NAME;
  static const wxString DEMO_FILES;
  static const wxString SEARCH_STRING;
  static const wxString EXE_STRING;
  static const wxString BACKSLASH;
  static const wxString FWDSLASH;
public:
  virtual ~UserConfig() {}
  virtual wxDirTraverseResult OnDir(const wxString& dirname);
  virtual wxDirTraverseResult OnFile(const wxString& filename);
  virtual wxDirTraverseResult OnOpenError(const wxString &openerrorname);
  static bool UserConfigExists(bool bBuild = false, wxString *pPath = NULL);
  static bool OpenTerminal();
};

#endif
