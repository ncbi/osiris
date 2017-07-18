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
*  FileName: nwxZipInput.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_ZIP_INPUT_H__
#define __NWX_ZIP_INPUT_H__

#include <wx/arrstr.h>
#include <wx/string.h>
#include <wx/stream.h>
#include <wx/datetime.h>
#include <wx/dir.h>
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "nwxZip/nwxZipBase.h"

class wxZipInputStream;
class wxZipEntry;
class wxFFileInputStream;

class nwxZipInput : public nwxZipBase
{
public:
  friend class nwxZipOutput;
  nwxZipInput():
    m_pCacheEntry(NULL),
    m_pStream(NULL),
    m_pFileStream(NULL)
  {}

  nwxZipInput(const wxString &sZipFilePath) : 
    m_pCacheEntry(NULL),
    m_pStream(NULL),
    m_pFileStream(NULL)
  {
    OpenZip(sZipFilePath);
  }
  virtual ~nwxZipInput()
  {
    _Cleanup();
  }
  bool OpenZip(const wxString &sZipFilePath);
  void CloseZip()
  {
    _Cleanup();
  }
  bool IsZipOpen()
  {
    return (m_pStream != NULL);
  }

  wxInputStream *OpenFile(const wxString &sFileName,bool bFullPath = false);
  size_t FilesExist(const wxString &sOutputPath, const wxString &sFileName, bool bFullPath = false);
  size_t FilesExist(const wxString &sOutputPath)
  {
    return FilesExist(sOutputPath,wxEmptyString,true);
  }
  bool WriteFile(const wxString &sOutputFilePath, const wxString &sFileName, bool bFullPath = false);
  bool WriteDir(const wxString &sOutputPath, const wxString &sDir, bool bFullPath = false);
  bool WriteAll(const wxString &sOutputPath)
  {
    return WriteDir(sOutputPath,wxEmptyString,true);
  }

  bool Chdir(const wxString &sFileName, bool bFullPath = false);
  bool IsFile(const wxString &sFileName, bool bFullPath = false);
  bool IsDir(const wxString &sFileName, bool bFullPath = false);
  bool InCwd(const wxString &sFileName, bool bFullPath = false);
  wxDateTime GetDateTime(const wxString &sFileName, bool bFullPath = false);
  size_t GetFiles(wxArrayString *pas, bool bCwd = false, bool bDirs = true, const wxString &sPattern = wxEmptyString);
  size_t GetOutputFilePaths(wxArrayString *pasOut, const wxString &sOutputPath, const wxArrayString &as);
  size_t GetDirs(wxArrayString *pas, bool bCwd = false);
  size_t GetSize(const wxString &sFileName, bool bFullPath = false);
private:
  void _Cleanup();
  wxZipEntry *_FindEntry(const wxString &sFileName, bool bFullPath);
  wxArrayString m_sAll;
  std::map<wxString, wxZipEntry *> m_pMapEntries;
  wxZipEntry *m_pCacheEntry;
  wxZipInputStream *m_pStream;
  wxFFileInputStream *m_pFileStream;
};


#endif
