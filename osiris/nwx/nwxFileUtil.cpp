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
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/filefn.h>
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxString.h"
#include "nwx/nwxLog.h"

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
#undef __EXE_NAME__
#ifdef __WXMAC__
#define __EXE_NAME__ wxT("open")
#endif
#ifdef __WXMSW__
#define __EXE_NAME__ wxT("explorer.exe")
#endif

bool nwxFileUtil::ShowFileFolder(const wxString &sFileName)
{
  wxFileName fn(sFileName);
  bool bRtn = false;
  if(wxDir::Exists(sFileName))
  {
    bRtn = _ShowFolder(fn.GetFullPath());
  }
  else if(wxFileName::IsFileReadable(sFileName))
  {
    bRtn = true;
    bool bFallback = true;
#ifdef __EXE_NAME__
    wxPathList apList;
    apList.AddEnvList(wxT("PATH"));
    wxString sPathName = apList.FindAbsoluteValidPath(__EXE_NAME__);
    if(DO_NOT_SELECT_FILE) {}
    else if(sPathName.IsEmpty()) {}
    else if(!wxFileName::IsFileExecutable(sPathName)) {}
    else
    {
      bFallback = false;
      wxString sCommand = __EXE_NAME__; 
      wxString sFileArg;
      // windows has a problem in that 
      // wxFileExists("c:\Windows\system32\explorer.exe")
      // returns true even though that is not the correct path
      // as a work around, omit the path
#ifdef __WXMSW__
      sFileArg = wxT("/select,");
#endif
      sFileArg.Append(sFileName);

#ifdef __WXMSW__
//      sCommand.Append(wxT(" "));
//      sCommand.Append(sFileArg);
      // wxExecute seems to have problems with ARGV[1] in windows
//      int nRtn = system(sCommand.utf8_str()) & 255;
//      bFallback = (nRtn > 1);
#endif
      const wchar_t *ARGV[4];
      size_t ndx = 0;
      ARGV[ndx++] = sCommand.wx_str();
#ifdef __WXMAC__
      ARGV[ndx++] = wxT("-R");
#endif
      ARGV[ndx++] = sFileArg.wx_str();
      ARGV[ndx++] = NULL;
      long nRtn = wxExecute((wchar_t **)ARGV,wxEXEC_SYNC);
#ifdef __WXMSW__
      bFallback = (nRtn < 0) && (nRtn > 1);
#else
      bFallback = !!nRtn;
#endif
#if 1
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
  return bRtn;
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

bool nwxFileUtil::MkDir(const wxString &sDir)
{
  bool bRtn = false;
  if(wxDir::Exists(sDir))
  {
    bRtn = true;
  }
  else
  {
    wxString s(sDir);
    UpDir(&s,1);
    if(MkDir(s)) // check parent directory
    {
      // parent directory now exists
      // make directory
      bRtn = ::wxMkdir(sDir);
    }
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


#if 0

// written but not tested, used wxCopyFile instead

int nwxFileUtil::CopyFile(
  const wxString &sFileFrom,
  const wxString &sFileTo,
  bool bOverwrite,
  wxString *psError)
{
  wxFile fin;
  wxFile fout;
  wxULongLong nInputSize;
  wxULongLong nOutputSize(0);
  int nRtn = COPY_OK;
  if(!wxFile::Exists(sFileFrom))
  {
    nRtn = INPUT_FILE_MISSING;
  }
  else if( (nInputSize = wxFileName::GetSize(sFileFrom)) == wxInvalidSize)
  {
    nRtn = INPUT_OPEN_ERROR;
  }
  else if(!fin.Open(sFileFrom.wc_str(),wxFile::read))
  {
    nRtn = INPUT_OPEN_ERROR;
  }
  else if((!bOverwrite) && wxFile::Exists(sFileTo.wc_str()))
  {
    nRtn = OUTPUT_FILE_EXISTS;
  }
  else if(!fout.Create(sFileTo.wc_str(),bOverwrite))
  {
    nRtn = OUTPUT_OPEN_ERROR;
  }
  else
  {
    const size_t BUFFER_SIZE = 4096;
    size_t nRead;
    size_t nWrite;
    size_t nToWrite;
    size_t n;
    wxUint8 buffer[BUFFER_SIZE];
    wxUint8 *pBuffer;
    while((nRtn == COPY_OK) && (nInputSize > nOutputSize))
    {
      pBuffer = &buffer[0];
      nRead = fin.Read((void *) pBuffer,BUFFER_SIZE);
      if(nRead == wxInvalidOffset)
      {
        nRtn = READ_ERROR;
      }
      else if(!nRead)
      {
        nRtn = READ_ERROR;
      }
      nToWrite = nRead;
      nWrite = 0;
      while((nRtn == COPY_OK) && (nToWrite > 0))
      {
        // this should be done in one loop
        n = fout.Write((void *)pBuffer,nToWrite);
        if(!n)
        {
          nRtn = WRITE_ERROR;
        }
        else
        {
          nWrite += n;
          nToWrite -= n;
          if(nToWrite)
          {
            pBuffer += n;
          }
        }
      }
      if(nRtn == COPY_OK)
      {
        nOutputSize += nWrite;
      }
    }
    if(nRtn != COPY_OK) {}
    else if(!fout.Close())
    {
      nRtn = WRITE_ERROR;
    }
  }
  if(psError != NULL)
  {
    *psError = CopyErrorMessage(nRtn,sFileFrom,sFileTo);
  }
  return nRtn;
}
wxString nwxFileUtil::CopyErrorMessage(
  int nRtn,
  const wxString &sFrom,
  const wxString &sTo)
{
  wxString sRtn;
  switch(nRtn)
  {
  case INPUT_FILE_MISSING:
    sRtn = "Cannot find input file, ";
    sRtn.Append(sFrom);
    break;
  case OUTPUT_FILE_EXISTS:
    sRtn = "Output file, ";
    sRtn.Append(sTo);
    sRtn.Append(", exists.");
    break;
  case INPUT_OPEN_ERROR:
    sRtn = "Cannot open input file, ";
    sRtn.Append(sFrom);
    break;
  case OUTPUT_OPEN_ERROR:
    sRtn = "Cannot open output file, ";
    sRtn.Append(sTo);
    break;
  case READ_ERROR:
    sRtn = "Cannot read input file, ";
    sRtn.Append(sFrom);
    break;
  case WRITE_ERROR:
    sRtn = "Cannot write output file, ";
    sRtn.Append(sTo);
  case COPY_OK:
  default:
    break;
  }
  return sRtn;
}

#endif
