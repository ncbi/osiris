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
*  FileName: nwxFileUtil.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_FILE_DIALOG_H__
#define __NWX_FILE_DIALOG_H__

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#ifndef __WXMSW__
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef __WXMSW__
#include <wx/volume.h>
#endif
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"

class nwxFileUtil
{
public:

  static wxFileDialog *CreateSaveDialog(
    wxWindow *parent,
    const wxString &sTitle,
    const wxString &sType,
    const wxString &sCurrentName,
    const wxString &sDefaultName);
  static wxString SetupFileName(
    const wxString &sOriginalFile,
    const wxString &sExt);
#ifndef __WXMSW__
  static mode_t GetDirPermission
    (const wxString &sPath, bool bCheckExistingParent = false);
  static bool SetFilePermissionFromDir
    (const wxString &sPath, bool bLogError = true);
#endif
#ifdef __WXMSW__
  static wxString GetMSWDriveLetter(const wxString &sPath);
  static wxFSVolumeKind GetMSWDriveType(const wxString &sPath);
  static bool IsMSWDriveReadOnly(wxFSVolumeKind n)
  {
    return ((n == wxFS_VOL_CDROM) || (n == wxFS_VOL_DVDROM));
  }
  static bool IsMSWDriveNetwork(wxFSVolumeKind n)
  {
    return (n == wxFS_VOL_NETWORK);
  }
  static bool IsMSWDriveFloppy(wxFSVolumeKind n)
  {
    return (n == wxFS_VOL_FLOPPY);
  }

#endif
  static wxString ForwardSlash(const wxString &s);
  static wxString GetRealPath(const wxString &sPath);
  static wxString PathFind(const wxString &sExeFile, bool bCheckSys = true, bool bCheckPath = true);
  static bool ShowFileFolder(const wxString &sFileName, bool bSelectDir = false);
  static int OpenFileFromOS(const wxString &sFileName);
  static wxString BaseName(const wxString &sDir);
  static wxString GetExistingParent(const wxString &s);
  static bool ExistingParentWritable(const wxString &s);
  static void NoEndWithSeparator(wxString *psDir);
  static void NoStartWithSeparator(wxString *psDir);
  static void EndWithSeparator(wxString *psDir);
  static bool UpDir(wxString *psDir, int n = 1);
  static bool MkDir(const wxString &sDir, bool bInheritMode = false);
  static int CopyFiles(const wxString &sFrom, const wxString &sTo, const wxString &sFilter = wxEmptyString, bool bOverwrite = false);
  static size_t GetAllFilesNoCase(const wxString &sDirName, wxArrayString *pasFile, const wxString &sFileSpec, int flags = wxDIR_DEFAULT);
  static void FixURL(wxString *psURL)
  {
    psURL->Replace(wxS("%"), wxS("%25"));
    psURL->Replace(wxS(" "), wxS("%20"));
    psURL->Replace(wxS("+"), wxS("%2b"));
  }
  static bool IsNewer(const wxString &sFileName, time_t t)
  {
    bool bRtn = !t;
    if(!bRtn)
    {
      wxFileName fn(sFileName);
      if(fn.FileExists())
      {
        bRtn = (fn.GetModificationTime().GetTicks() >= t);
      }
    }
    return bRtn;
  }
  static bool IsNewer(const wxString &sFileName, const wxDateTime &dt)
  {
    return IsNewer(sFileName,dt.GetTicks());
  }
  static void SetDoNotSelectFile(bool b = true)
  {
    DO_NOT_SELECT_FILE = b;
  }
  static bool GetDoNotSelectFile()
  {
    return DO_NOT_SELECT_FILE;
  }
  static wxString TextToFileName(const wxString &sName, size_t MAXLEN = wxNOT_FOUND);
  static wxString FindNewDirName(const wxString &sFullPath);
private:
  static std::vector<wxString> g_asSysPath;
  static std::vector<wxString> g_asPath;
  static bool g_PATH_SET;
  static bool DO_NOT_SELECT_FILE;
  static wxString _pathFind(
    const wxString &sExeFile, const std::vector<wxString> &asPath);
  static bool _ShowFolder(const wxString &sFileName);
  static void _setupPaths();
  static wxString _getFileOpenExe();
                 // explorer.exe for Windows, open for Mac OS X
#ifdef __WXMSW__
  static void _initVolumes();
  static bool g_bInitVolumes;
#endif
  nwxFileUtil() {;} // prevent instantiation
};


#endif
