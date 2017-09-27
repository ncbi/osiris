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
*  FileName: nwxZipInput.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwxZipInput.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include "nwx/nwxFileUtil.h"
#include "nwx/mapptr.h"
#include "nwx/nwxFileUtil.h"
#include <memory>

wxString nwxZipBase::__MAKE_PATH(const wxString &sDir, const wxString &sPath)
{
  wxString sRtn(sDir);
  wxString s(sPath);
  if(!sRtn.IsEmpty())
  {
    nwxFileUtil::EndWithSeparator(&sRtn);
    nwxFileUtil::NoStartWithSeparator(&s);
  }
  sRtn.Append(s);
  return sRtn;
}


void nwxZipInput::_Cleanup()
{

  m_sAll.Clear();
  _ClearBase();
  mapptr<wxString,wxZipEntry>::cleanup(&m_pMapEntries);
  m_pCacheEntry = NULL;
#define DELPTR(p) if(p != NULL) { delete p; p = NULL; }
  DELPTR(m_pStream)
  DELPTR(m_pFileStream)
#undef DELPTR
}

bool nwxZipInput::OpenZip(const wxString &sZipFilePath)
{
  bool bRtn = false;
  if(wxFileName::IsFileReadable(sZipFilePath))
  {
    std::auto_ptr<wxFFileInputStream> pffTemp(new wxFFileInputStream(sZipFilePath));
    if(pffTemp->IsOk())
    {
      std::auto_ptr<wxZipInputStream> pffTempZ(new wxZipInputStream(*(pffTemp.get())));
      if(pffTempZ->IsOk())
      {
        _Cleanup();
        m_pStream = pffTempZ.release();
        m_pFileStream = pffTemp.release();
        int nCount = m_pStream->GetTotalEntries();
        int nFound = 0;
        wxZipEntry *pEntry;
        for(pEntry = m_pStream->GetNextEntry();
          pEntry != NULL;
          pEntry = m_pStream->GetNextEntry())
        {
          m_sAll.Add(pEntry->GetName());
          m_pMapEntries.insert(
            std::map<wxString, wxZipEntry *>::value_type(
            pEntry->GetName(),pEntry));
          nFound++;
        }
        if(nFound == nCount)
        {
          bRtn = true;
          m_sAll.Sort();
          _SetOpenZipFilePath(sZipFilePath);
        }
        else
        {
          _Cleanup();
        }
      }
    }
  }
  return bRtn;
}
bool nwxZipInput::Chdir(const wxString &sFileName, bool bFullPath)
{
  bool bRtn = false;
  wxZipEntry *pe;
  if(sFileName.IsEmpty())
  {
    bRtn = true;
    if(bFullPath)
    {
      _ClearDir();
    }
  }
  else if( ((pe = _FindEntry(sFileName,bFullPath)) != NULL) &&
    pe->IsDir() )
  {
    _Chdir(pe->GetName());
    bRtn = true;
  }
  return bRtn;
}

bool nwxZipInput::InCwd(const wxString &sFileName, bool bFullPath)
{
  wxZipEntry *pe = _FindEntry(sFileName,bFullPath);
  bool bRtn = false;
  if(pe != NULL)
  {
    wxString sName = pe->GetName();
    size_t nLen = sName.Len();
    size_t nLenDir = _Dir().Len();
    bRtn = (nLenDir > 0) &&
      (nLenDir <= nLen) &&
      (sName.SubString(0,nLenDir) == _Dir());
  }
  return bRtn;
}

wxZipEntry *nwxZipInput::_FindEntry(const wxString &sFileName, bool bFullPath)
{
  wxString sName;
  wxZipEntry *pE = NULL;
  sName = _BUILD_PATH(sFileName,bFullPath);
  if( (m_pCacheEntry != NULL) &&
      (sName == m_pCacheEntry->GetName())
    )
  {
    pE = m_pCacheEntry;
  }
  else
  {
    std::map<wxString, wxZipEntry *>::const_iterator itr = m_pMapEntries.find(sName);
    pE = (itr == m_pMapEntries.end()) ? NULL : itr->second;
    if(pE != NULL)
    {
      m_pCacheEntry = pE;
    }
  }
  return pE;
}

bool nwxZipInput::IsDir(const wxString &sFileName, bool bFullPath)
{
  wxZipEntry *pE = _FindEntry(sFileName,bFullPath);
  return (pE != NULL) && pE->IsDir();
}
wxDateTime nwxZipInput::GetDateTime(const wxString &sFileName, bool bFullPath)
{
  wxDateTime dtRtn((time_t)0);
  wxZipEntry *pE = _FindEntry(sFileName,bFullPath);
  if(pE != NULL)
  {
    dtRtn = pE->GetDateTime();
  }
  return dtRtn;
}

bool nwxZipInput::IsFile(const wxString &sFileName, bool bFullPath)
{
  wxZipEntry *pE = _FindEntry(sFileName,bFullPath);
  return (pE != NULL) && !pE->IsDir();
}

size_t nwxZipInput::GetFiles(wxArrayString *pas, bool bCwd, bool bDirs,const wxString &sPattern)
{
  size_t nCount = m_sAll.GetCount();
  bool bNoPattern = sPattern.IsEmpty();
  pas->Empty();
  pas->Alloc(nCount);
  if(_Dir().IsEmpty()) { bCwd = false; }
  for(size_t i = 0; i < nCount; ++i)
  {
    const wxString &s(m_sAll.Item(i));
    if(!bDirs && IsDir(s))
    {} // skip
    if(bCwd && !InCwd(s,true))
    {
      if(pas->GetCount()) 
      {
        break; // past directory, done
      }
    } // skip
    else
    {
      if(bNoPattern || s.Matches(sPattern))
      {
        pas->Add(s);
      }
    }
  }
  return  pas->GetCount();
}
size_t nwxZipInput::GetDirs(wxArrayString *pas, bool bCwd)
{
  size_t nCount = m_sAll.GetCount();
  pas->Empty();
  if(!( bCwd && _Dir().IsEmpty() ))
  {
    for(size_t i = 0; i < nCount; ++i)
    {
      const wxString &s(m_sAll.Item(i));
      if(!IsDir(s,true))
      {}
      else if(bCwd && !InCwd(s,true))
      {
        if(pas->GetCount()) 
        {
          break; // past directory, done
        }
      } // skip
      else
      {
        pas->Add(s);
      }
    }
  }
  return  pas->GetCount();
}

wxInputStream *nwxZipInput::OpenFile(const wxString &sFileName,bool bFullPath)
{
  wxZipEntry *pE = _FindEntry(sFileName,bFullPath);
  wxInputStream *pRtn =
    ((pE != NULL) && m_pStream->OpenEntry(*pE) && m_pStream->IsOk()) ? m_pStream : NULL;
  return pRtn;
}
size_t nwxZipInput::GetSize(const wxString &sFileName,bool bFullPath)
{
  wxZipEntry *pE = _FindEntry(sFileName,bFullPath);
  size_t nSize = (pE != NULL) ? (size_t)pE->GetSize() : 0;
  return nSize;
}

bool nwxZipInput::WriteFile(const wxString &sOutputFilePath, const wxString &sFileName, bool bFullPath)
{
  bool bRtn = false;
  wxFileName fname(sOutputFilePath);
  wxInputStream *pS = NULL;
  if( fname.Normalize() && 
    ((pS = OpenFile(sFileName,bFullPath)) != NULL) &&
      fname.Mkdir(wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL)
    )
  {
    wxTempFileOutputStream fout(fname.GetFullPath());
    if(fout.IsOk())
    {
      wxDateTime dt = GetDateTime(sFileName,bFullPath);
      size_t nSize = GetSize(sFileName,bFullPath);
      fout.Write(*pS);
      size_t nWrite = nSize ? fout.TellO() : 0;
      if( fout.IsOk() && 
          pS->Eof() && 
          (nWrite == nSize)
        )
      {
        if(fout.Close())
        {
          bRtn = fname.SetTimes(&dt,&dt,NULL);
        }
      }
      else
      {
        fout.Discard();
      }
    }
  }
  return bRtn;
}

size_t nwxZipInput::GetOutputFilePaths(wxArrayString *pasOut, const wxString &sOutputPath, const wxArrayString &as)
{
  size_t nCount = as.GetCount();
  size_t i;
  size_t nRtn = 0;
  for(i = 0; i < nCount; i++)
  {
    const wxString &sFileName = as.Item(i);
    wxFileName fname(__MAKE_PATH(sOutputPath,sFileName));
    pasOut->Add(fname.GetFullPath());
    nRtn++;
  }
  return nRtn;
}
bool nwxZipInput::WriteDir(const wxString &sOutputPath, const wxString &sDir, bool bFullPath)
{
  bool bRtn = false;
  DIR_HOLD xxx(this);
  if(Chdir(sDir,bFullPath))
  {
    wxArrayString as;
    wxArrayString asOut;
    size_t i;
    size_t nLast = 0;
    bool bDir;
    bRtn = true;
    GetFiles(&as,true,true);
    GetOutputFilePaths(&asOut,sOutputPath,as);
    size_t nCount = as.GetCount();
    for(i = 0; bRtn && (i < nCount); i++)
    {
      nLast = i;
      const wxString &sFileName = as.Item(i);
      bDir = IsDir(sFileName);
      wxFileName fname(__MAKE_PATH(sOutputPath,sFileName));
      if(bDir)
      {
        bRtn = wxFileName::Mkdir(asOut.Item(i),wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
      }
      else
      {
        bRtn = WriteFile(asOut.Item(i),sFileName,true);
      }
    }
    if(!bRtn)
    {
      for(i = 0; i < nLast; ++i)
      {
        wxFileName fname(asOut.Item(nLast - i - 1));
        if(!fname.Exists())
        {}
        else if(fname.IsDir())
        {
          fname.Rmdir(); // only if empty
        }
        else
        {
          wxRemoveFile(fname.GetFullPath());
        }
      }
    }
  }
  return bRtn;
}
size_t nwxZipInput::FilesExist(const wxString &sOutputPath, const wxString &sDir, bool bFullPath)
{
  size_t nRtn = 0;
  DIR_HOLD xxx(this);
  if(Chdir(sDir,bFullPath))
  {
    wxArrayString as;
    wxArrayString asOut;
    GetFiles(&as,true,true);
    size_t n = GetOutputFilePaths(&asOut,sOutputPath,as);
    size_t i;
    for(i = 0; i < n; ++i)
    {
      if(wxFileName::FileExists(asOut.Item(i))) { nRtn++;}      
    }
  }
  return nRtn;
}
