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
*  FileName: nwxFileUtil.cpp
*  Author:   Douglas Hoffman
*
*/
#include <stdlib.h>
#ifdef __WXMAC__
#include <limits.h>
#endif
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/filefn.h>
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxString.h"
#include "nwx/nwxLog.h"
#include "nwx/CIncrementer.h"

bool nwxFileUtil::DO_NOT_SELECT_FILE = false;

wxFileDialog *nwxFileUtil::CreateSaveDialog(
  wxWindow *parent,
  const wxString &sTitle,
  const wxString &sType,
  const wxString &sCurrentName,
  const wxString &sDefaultName)
{
  wxString sFileName = sCurrentName;
  if(sFileName.IsEmpty() || !wxFileName::DirExists(sFileName))
  {
    sFileName = sDefaultName;
  }
  wxFileName fn(sFileName);
  wxString sPath = fn.GetPath();
  wxString sName = fn.GetFullName();    
  wxFileDialog *pDialogFile = new wxFileDialog(
    parent,sTitle,sPath,sName,
    sType,
    wxFD_SAVE | wxFD_OVERWRITE_PROMPT  );
  return pDialogFile;
}

wxString nwxFileUtil::SetupFileName(
  const wxString &sOriginalFile, const wxString &_sExt)
{
  wxString sFile(sOriginalFile);
  wxString sExt(_sExt);
  size_t nLen = sExt.Len();
  if(nLen && !sExt.StartsWith("."))
  {
    wxString s(".");
    s.Append(sExt);
    sExt = s;
    nLen++;
  }
  nwxString::SetFileExtension(&sFile,sExt);
  if(wxFileName::FileExists(sFile))
  {
    wxString sBaseFile(sFile);
    int i = 1;
    if(nLen)
    {
      sBaseFile.Truncate(sBaseFile.Len() - nLen);
    }
    sBaseFile.Append("_");
    for(bool bNotDone = true;
      bNotDone;
      bNotDone = wxFileName::FileExists(sFile))
    {
      sFile = sBaseFile;
      sFile.Append(nwxString::FormatNumber(i++));
      sFile.Append(sExt);
    }
  }
  return sFile;
}
void nwxFileUtil::EndWithSeparator(wxString *psDir)
{
  wxString sSeparators = wxFileName::GetPathSeparators();
  size_t nLen = psDir->Len();
  if((!nLen) || (sSeparators.Find(psDir->Last()) == wxNOT_FOUND))
  {
    psDir->Append(wxFileName::GetPathSeparator());
  }
}
void nwxFileUtil::NoStartWithSeparator(wxString *psDir)
{
  wxString sSeparators = wxFileName::GetPathSeparators();
  size_t nLen1 = psDir->Len();
  size_t nLen = nLen1;
  const wxChar *ps = psDir->wc_str();
  while((nLen > 0) && (sSeparators.Find(*ps) != wxNOT_FOUND))
  {
    ps++;
    nLen--;
  }
  if(nLen != nLen1)
  {
    (*psDir) = wxString(ps);
  }
}

void nwxFileUtil::NoEndWithSeparator(wxString *psDir)
{
  wxString sSeparators = wxFileName::GetPathSeparators();
  size_t nLen = psDir->Len();
  while((nLen > 0) && (sSeparators.Find(psDir->Last()) != wxNOT_FOUND))
  {
    psDir->RemoveLast();
    nLen--;
  }
}
wxString nwxFileUtil::GetExistingParent(const wxString &sPath)
{
  wxFileName fn(sPath);
  wxString sPrev(sPath);
  wxString sTmp = sPath;
  while(!( wxFileName::DirExists(sTmp) || fn.FileExists() ))
  {
    sTmp = fn.GetPath();
    if(sTmp == sPrev)
    {
      //  probably hit the root, prevent endless loop
      sTmp.Empty();
      break;
    }
    else
    {
      sPrev = sTmp;
      fn.Assign(sTmp);
    }
  }
  wxString sRtn;
  if(sTmp.IsEmpty())
  {}
  else if(wxFileName::DirExists(sTmp))
  {
    sRtn = sTmp;
  }
    // if fn.DirExists() is false, it is likley that 
    // a file exists with the desired path
    // so a directory cannot be created;
  return sRtn;
}

bool nwxFileUtil::ExistingParentWritable(const wxString &sPath)
{
  wxString s = GetExistingParent(sPath);
  bool bRtn = !s.IsEmpty()
    && wxFileName::IsDirWritable(s)
    && wxFileName::IsDirReadable(s);
  return bRtn;

}

wxString nwxFileUtil::BaseName(const wxString &_sDir)
{
  wxString sDir(_sDir);
  NoEndWithSeparator(&sDir);
  wxString sRtn;
  size_t nLen = sDir.Len();
  if(nLen)
  {
    wxFileName fn(sDir);
    sRtn = fn.GetFullName();
  }
  return sRtn;
}

bool nwxFileUtil::_ShowFolder(const wxString &sFolderName)
{
  wxString sURL("file:///");
  sURL.Append(sFolderName);
  wxBusyCursor x;
  sURL.Replace(wxS("%"),wxS("%25"));
  sURL.Replace(wxS(" "),wxS("%20"));
  sURL.Replace(wxS("+"),wxS("%2b"));
  bool bRtn = wxLaunchDefaultBrowser(sURL);
  return bRtn;
}

#ifndef __WXMSW__

// OS-662

mode_t nwxFileUtil::GetDirPermission(const wxString &sPath, bool bCheckExistingParent)
{
  mode_t nRtn = 0;
  if(sPath.IsEmpty()) {}
  else if(wxFileName::DirExists(sPath))
  {
    struct stat statBuf;
    if(!stat(sPath.utf8_str(),&statBuf))
    {
      nRtn = statBuf.st_mode;
    }
  }
  else if(wxFileName::FileExists(sPath))
  {
    wxFileName fn(sPath);
    nRtn = GetDirPermission(fn.GetPath(),false);
  }
  else if(bCheckExistingParent)
  {
    wxString sPathExist =
      GetExistingParent(sPath);
    nRtn = GetDirPermission(sPathExist,false);
  }
  return nRtn;
}


bool nwxFileUtil::SetFilePermissionFromDir(
  const wxString &sPath, bool bLogError)
{
  //
  //  set group and others write permission on
  //  file or directory specified by sPath using those permissions
  //  in the containing directory
  //  This adds permissions if applicable and does not remove any permissions

  bool bRtn = false;
  bool bDirExists = wxFileName::DirExists(sPath);
  wxString sError;
  if(bDirExists || wxFileName::FileExists(sPath))
  {
    wxFileName fn(sPath);
    struct stat statBuffer;
    mode_t modeDir = 0;
    mode_t mask  = bDirExists ? 0777 : 0666;
    if(stat(fn.GetPath().utf8_str(),&statBuffer))
    {
      sError.Append(wxS("stat() failed for: "));
      sError.Append(fn.GetPath());
      sError.Append(wxS("\n"));
    } // error
    else if( (modeDir = statBuffer.st_mode & mask) == 0 ) {} // done
    else if(!stat(sPath.utf8_str(),&statBuffer))
    {
      mode_t modeFile = statBuffer.st_mode;
      mode_t modeDesired = modeFile | modeDir;
      bRtn = (modeDesired != modeFile)
        ? !chmod(sPath.utf8_str(),modeDesired)
        : true;
      if(!bRtn)
      {
        sError = wxString::Format
          (wxS("chmod() failed, mode = %d\n"),
           (int) modeDesired);
      }
    }
    else
    {
      sError.Append(wxS("stat() failed for: "));
      sError.Append(sPath);
      sError.Append(wxS("\n"));
    }
  }
  if((!bRtn) && bLogError)
  {
    nwxLog::LogMessage(
      wxString::Format(
        wxS("Cannot set file permission for %s\n%s"),
        sPath, sError));
  }
  return bRtn;
}

// OS-662 end

#endif

#undef __EXE_NAME__
#ifdef __WXMAC__
#define __EXE_NAME__ wxT("open")
#endif
#ifdef __WXMSW__
#define __EXE_NAME__ wxT("explorer.exe")
#endif

bool nwxFileUtil::ShowFileFolder(const wxString &sFileName, bool bCheckDir)
{
  wxFileName fn(sFileName);
  bool bRtn = false;
  if(bCheckDir && wxDir::Exists(sFileName))
  {
    bRtn = _ShowFolder(fn.GetFullPath());
  }
  else if(wxFileName::IsFileReadable(sFileName)
          || wxFileName::IsDirReadable(sFileName))
  {
    bRtn = true;
    bool bFallback = true;
    const wchar_t *ARGV[4];
#ifdef __EXE_NAME__
  wxString sPathName = PathFind(__EXE_NAME__,true,false);
    if(DO_NOT_SELECT_FILE) {}
    else if(sPathName.IsEmpty()) {}
    else if(!wxFileName::IsFileExecutable(sPathName)) {}
    else
    {
      bFallback = false;
//      wxString sCommand = __EXE_NAME__; 
      // windows has a problem in that 
      // wxFileExists("c:\Windows\system32\explorer.exe")
      // returns true even though that is not the correct path
      // as a work around, omit the path
      size_t ndx = 0;
      ARGV[ndx++] = sPathName.wx_str();
#ifdef __WXMAC__
      ARGV[ndx++] = wxT("-R");
#endif
#ifdef __WXMSW__
      // OS-746, setting /select, and the filename as two paramters
      // will work if there is a space in the file name
      // if there is no space, concatenation of the two parameters
      // works fine.
      ARGV[ndx++] = wxT("/select,");
#endif
      ARGV[ndx++] = sFileName.wx_str();
      ARGV[ndx++] = NULL;
      long nRtn = wxExecute((wchar_t **)ARGV,wxEXEC_SYNC);
#ifdef __WXMSW__
      bFallback = (nRtn < 0) || (nRtn > 1);
#else
      bFallback = !!nRtn;
#endif
#ifdef __WXDEBUG__
      wxString sLog = wxT("Show file: ");
      sLog += sFileName;
      sLog += "; return code = ";
      sLog = nwxString::FormatNumber((int) nRtn);
      nwxLog::LogMessage(sLog);
#endif
    }
#endif // defined(__EXE_NAME__)
    if(bFallback)
    {
      bRtn = _ShowFolder(fn.GetPath());
    }
  }
  else
  {
    wxString sTmp("Cannot show file: ");
    sTmp.Append(sFileName);
    nwxLog::LogMessage(sFileName);
  }
  return bRtn;
}
wxString nwxFileUtil::_getFileOpenExe()
{
  wxString sPathName = PathFind(__EXE_NAME__,true,false);
  if(sPathName.IsEmpty()) {}
  else if(!wxFileName::IsFileExecutable(sPathName))
  {
    sPathName.Empty();
  }
  return sPathName;
}
int nwxFileUtil::OpenFileFromOS(const wxString &sFileName)
{
  // STOP HERE
  int nRtn = 0;
  wxString sEXE = _getFileOpenExe();
  bool b = sEXE.IsEmpty() ? false :
    ( wxFileName::IsFileReadable(sFileName) 
      || wxFileName::IsDirReadable(sFileName)
    );
  if(!b)
  {
    nRtn = -2;
  }
  else
  {
    const wchar_t *ARGV[3];
    ARGV[0] = sEXE.wx_str();
    ARGV[1] = sFileName.wx_str();
    ARGV[2] = NULL;
    nRtn = (int) wxExecute((wchar_t **)ARGV,wxEXEC_SYNC);
    if(nRtn)
    {
      wxString sMsg = "Exec: '";
      sMsg.Append(sEXE);
      sMsg.Append(" ");
      sMsg.Append(sFileName);
      sMsg.Append("' -- returned: " );
      sMsg.Append(nwxString::FormatNumber((int) nRtn));
      nwxLog::LogMessage(sMsg);
    }
  }
  return nRtn;
}

wxString nwxFileUtil::ForwardSlash(const wxString &sPath)
{
  wxString sRtn(sPath);
  if(sRtn.Find(L'\\') != wxNOT_FOUND)
  {
    sRtn.Replace(wxS(":"),wxS(""));
    sRtn.Replace(L'\\',L'/');
    if(sRtn.at(0) != L'/')
    {
      wxString s = L'/';
      s.Append(sRtn);
      sRtn = s;
    }
    while( sRtn.Replace(wxS("//"),wxS("/")) ) {}
  }
#ifdef TMP_DEBUG
  if(sPath != sRtn)
  {
    wxString s(wxS("nwxFileUtil::ForwardSlash("));
    s.Append(sPath);
    s.Append(wxS(") = "));
    s.Append(sRtn);
    nwxLog::LogMessage(s);
  }
#endif
  return sRtn;
}


bool nwxFileUtil::UpDir(wxString *psDir, int n)
{
  bool bRtn = true;
  wxString s(*psDir);
  NoEndWithSeparator(&s);
#ifdef __WXMSW__
  bool bDriveLetter = (s.Len() == 2) && (s.Last() == ':');
  bool bUNC = bDriveLetter ? false : !psDir->Cmp("\\\\");
  bool bDone = (n < 1) ||
      (s.Len() < 2) ||
      bDriveLetter || bUNC;
#else
    bool bDone = (n < 1) || (s.Len() < 2);
#endif
  if(bDone)
  {
    bRtn = (n < 1);  // shouldn't be here anyway
#ifdef __WXMSW__
    if(bDriveLetter)
    {
      EndWithSeparator(psDir);
    }
    else if(!bUNC)
    {
      NoEndWithSeparator(psDir);
    }
#else
    if(psDir->Len() > 1)
    {
      NoEndWithSeparator(psDir);
    }
#endif
  }
  else
  {
    wxFileName fn(s);
    s = fn.GetPath();
    NoEndWithSeparator(&s);
    *psDir = s;
    if(n == 1)
    {
      bRtn = true;
    }
    else
    {
      bRtn = UpDir(psDir,n-1);
    }
  }
  return bRtn;
}

bool nwxFileUtil::MkDir(
    const wxString &sDir, 
    bool
#ifndef __WXMSW__
    bInheritMode
#endif
    )
{
  bool bRtn = false;
  if(wxDir::Exists(sDir))
  {
    bRtn = true;
  }
  else
  {
#ifndef __WXMSW__
// OS-662
    mode_t modeParent = bInheritMode
      ? int(GetDirPermission(sDir,true) & 0777)
      : 0;
    int nPerm = wxS_DIR_DEFAULT | modeParent;
#else
    int nPerm = wxS_DIR_DEFAULT;
#endif
    bRtn = wxFileName::Mkdir
      (sDir, nPerm, wxPATH_MKDIR_FULL);
#ifndef __WXMSW__
#ifdef TMP_DEBUG
    int nCalledChmod = 0;
#endif
    if(bRtn && modeParent)
    {
      wxString sDirCheck(sDir);
      bool bDone = false;
      while(bRtn && !bDone)
      {
        NoEndWithSeparator(&sDirCheck);
        mode_t modeNew = GetDirPermission(sDirCheck,false);
        if(modeNew && ((modeNew | modeParent) != modeNew))
        {
          bRtn = !chmod(sDirCheck.utf8_str(), modeNew | modeParent);
#ifdef TMP_DEBUG
          nCalledChmod++;
          {
            wxString sMsg =
              wxString::Format(
                wxS("-------- chmod(\"%s\",0%o) = %s"),
                sDirCheck,
                modeNew | modeParent,
                bRtn ? "true" : "false");
            nwxLog::LogMessage(sMsg);
          }
#endif
          wxFileName fn(sDirCheck);
          sDirCheck = fn.GetPath();
        }
        else
        {
          bDone = true; // loop exit
        }
      }
    }

#ifdef TMP_DEBUG
    {
#define BOOL_TO_STR(b) ((b) ? wxS("true") : wxS("false"))
      wxString strMsg(wxS("nwxFileUtil::MkDir(\""));
      strMsg.Append(sDir);
      strMsg.Append(wxS("\", "));
      strMsg.Append(BOOL_TO_STR(bInheritMode));
      strMsg.Append(
        wxString::Format(
          wxS("): modeParent = %o, bRtn = %s, nCalledChmod = %d"),
          modeParent,BOOL_TO_STR(bRtn),nCalledChmod
          ));
      nwxLog::LogMessage(strMsg);
#undef BOOL_TO_STR
    }
#endif
#endif
//
//  removed 2/26/2018 discard 8/1/2018
//    wxString s(sDir);
//    UpDir(&s,1);
//    if(MkDir(s)) // check parent directory
//    {
//      // parent directory now exists
//      // make directory
//      bRtn = ::wxMkdir(sDir);
//    }
  }
  return bRtn;
}

size_t nwxFileUtil::GetAllFilesNoCase(
  const wxString &sDirName, wxArrayString *pasFile, 
  const wxString &sFileSpec, int flags)
{
  size_t nRtn = 0;
  if(wxDir::Exists(sDirName))
  {
    wxArrayString as;
    wxString s;
    wxString sSpec = sFileSpec;
    sSpec.MakeLower();
    size_t n = wxDir::GetAllFiles(sDirName,&as,wxEmptyString,flags);
    size_t i;
    for(i = 0; i < n; ++i)
    {
      const wxString &sItem = as.Item(i);
      s = sItem;
      s.MakeLower();
      if(sFileSpec.IsEmpty() || s.Matches(sSpec))
      {
        pasFile->Add(sItem);
        nRtn++;
      }
    }
  }
  return nRtn;
}

std::vector<wxString> nwxFileUtil::g_asSysPath;
std::vector<wxString> nwxFileUtil::g_asPath;
bool nwxFileUtil::g_PATH_SET = false;

#ifdef __WXMSW__
#define ENV_PATH_SEPARATOR ';'
#else
#define ENV_PATH_SEPARATOR ':'
#endif

void nwxFileUtil::_setupPaths()
{
  if(!g_PATH_SET)
  {
    g_PATH_SET = true;
#define DIR_NOT_EXIST(s) \
    if(!wxFileName::DirExists(s)) \
    { \
      wxString sTmp(wxS("Cannot find directory: ")); \
      sTmp.Append(s); \
      nwxLog::LogMessage(sTmp); \
    }

#ifdef __WXMSW__
    const char *ps = getenv("SystemRoot");
    if(ps == NULL)
    {
      ps = getenv("windir");
    }
    if(ps == NULL)
    {
      nwxLog::LogMessage(wxS("Cannot find window SystemRoot in environment"));
    }
    else
    {
      wxString sPath(ps);
      DIR_NOT_EXIST(sPath)
      else
      {
        EndWithSeparator(&sPath);
        wxString sSysPath(sPath);
        sSysPath.Append("System32\\"); 
        g_asSysPath.push_back(sPath);
        DIR_NOT_EXIST(sSysPath)
        else
        {
          g_asSysPath.push_back(sSysPath);
        }
      }
    }
#else
    wxString sPath;
    const char *apsCheck[] =
      { "/usr/bin/","/bin/",NULL };
    const char **psCheck;
    for(psCheck = &apsCheck[0]; *psCheck != NULL; ++psCheck)
    {
      sPath = *psCheck;
      DIR_NOT_EXIST(sPath)
      else
      {
        g_asSysPath.push_back(sPath);
      }
    }
#endif
    char *psPath = getenv("PATH");
    if(psPath != NULL)
    {
      nwxString::Split(psPath,&g_asPath,ENV_PATH_SEPARATOR);
    }
    else
    {
      nwxLog::LogMessage("Cannot find PATH in environment");
    }
#ifdef TMP_DEBUG
    {
      wxString sTmp;
      wxString sMsg(wxS("PATH:\n  "));
      nwxString::Join(g_asSysPath,&sTmp,"\n  ");
      sMsg.Append(sTmp);
      sMsg.Append(wxS("\n\nSystem path:\n  "));
      nwxString::Join(g_asPath,&sTmp,"\n  ");
      sMsg.Append(sTmp);
      nwxLog::LogMessage(sTmp);
    }
#endif
  }
}
wxString nwxFileUtil::_pathFind(const wxString &sExeFile, const std::vector<wxString> &asPath)
{
  wxString sRtn;
  wxString sFullPath;
  std::vector<wxString>::const_iterator itr;
  for(itr = asPath.begin(); itr != asPath.end(); ++itr)
  {
    sFullPath = *itr;
    EndWithSeparator(&sFullPath);
    sFullPath.Append(sExeFile);
    if(wxFileName::IsFileExecutable(sFullPath))
    {
      sRtn = sFullPath;
      break;
    }
  }
  return sRtn;
}
wxString nwxFileUtil::PathFind(const wxString &sExeFile, bool bCheckSys, bool bCheckPath)
{
  wxString sRtn;
  if(!g_PATH_SET)
  {
    _setupPaths();
  }
  if(bCheckSys)
  {
    sRtn = _pathFind(sExeFile, g_asSysPath);
  }
  if(sRtn.IsEmpty() && bCheckPath)
  {
    sRtn = _pathFind(sExeFile, g_asPath);
  }
  return sRtn;
}

#ifdef __WXMSW__
wxString nwxFileUtil::GetMSWDriveLetter(const wxString &sPath)
{
  wxString sRtn;
  size_t nLen = sPath.Len();
  if(nLen < 2) {}
  else if (sPath.Mid(1,1) == wxS(":"))
  {
    sRtn = sPath.Mid(0,2);
    sRtn.Append("\\");
    sRtn.MakeUpper();
  }
  return sRtn;
}
bool nwxFileUtil::g_bInitVolumes(false);

void nwxFileUtil::_initVolumes()
{
  if(!g_bInitVolumes)
  {
    wxArrayString as = wxFSVolume::GetVolumes();
    g_bInitVolumes = true;
  }
}
wxFSVolumeKind nwxFileUtil::GetMSWDriveType(const wxString &sPath)
{
  size_t nLen = sPath.Len();
  wxFSVolumeKind nRtn = wxFS_VOL_MAX;
  if(nLen < 2) {}
  else if (sPath.Left(2) == wxS("\\\\"))
  {
    nRtn = wxFS_VOL_NETWORK;
  }
  else
  {
    wxString s = GetMSWDriveLetter(sPath);
    if(!s.IsEmpty())
    {
      //   not getting fs type on F Drive
      if(!g_bInitVolumes)
      {
        _initVolumes();
      }
      wxFSVolume v(s);
      if(v.IsOk())
      {
        nRtn = v.GetKind();
      }
    }
  }
  return nRtn;
}

wxString nwxFileUtil::GetRealPath(const wxString &sPath)
{
  // for MS Windows, replace drive letter with
  //  UNC, if this is a network drive
  wxString sDrive;
  wxString sRtn;
  bool bError = false;
  if(wxFileName::FileExists(sPath) || wxFileName::DirExists(sPath))
  {
    sDrive = GetMSWDriveLetter(sPath);
  }
  else
  {
    bError = true;
  }
  if(!sDrive.IsEmpty())
  {
    if(!g_bInitVolumes)
    {
      _initVolumes();
    }
    wxFSVolume v(sDrive);
    if(v.IsOk() && IsMSWDriveNetwork(v.GetKind()))
    {
      wxString s = v.GetDisplayName();
      // eg. "subdir (\\node\home\username\) (X:)"
      size_t nBegin = s.Find(wxS("(\\\\"));
      if(nBegin > 0)
      {
        wxString sSubDir = s.Left(nBegin);
        wxString sNetPath = s.Mid(nBegin + 1);
        size_t nEnd = sNetPath.Find(wxS(")"));
        if(nEnd > 0)
        {
          wxString sPathNoDrive = sPath.Mid(2);
          NoStartWithSeparator(&sPathNoDrive);
          nwxString::Trim(&sPathNoDrive);

          sNetPath = sNetPath.Left(nEnd);
          nwxString::Trim(&sNetPath);
          EndWithSeparator(&sNetPath);
          nwxString::Trim(&sSubDir);
          sNetPath.Append(sSubDir);
          EndWithSeparator(&sNetPath);
          sNetPath.Append(sPathNoDrive);
          sRtn = sNetPath;  // done
        }
      }
    }
  }
  if(sRtn.IsEmpty() && !bError)
  {
    sRtn = sPath;
  }
  return sRtn;
}
#endif

#ifdef __WXMAC__

wxString nwxFileUtil::GetRealPath(const wxString &sPath)
{
  wxString sRtn;
  char buffer[PATH_MAX + 1];
  bool bError = false;
  if(sPath.IsEmpty())
  {
  }
  else if(realpath(sPath.utf8_str(),buffer) == NULL)
  {
    bError = true;
  }
  else
  {
    sRtn = wxString::FromUTF8(buffer);
  }
#ifdef TMP_DEBUG
  {
    wxString sMsg(wxS("nwxFileUtil::GetRealPath(\""));
    sMsg.Append(sPath);
    sMsg.Append(wxS("\") = "));
    sMsg.Append(sRtn);
    nwxLog::LogMessage(sMsg);
  }
#endif
  return sRtn;
}
#endif


