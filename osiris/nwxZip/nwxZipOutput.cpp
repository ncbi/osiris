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
*  FileName: nwxZipOutput.cpp
*  Author:   Douglas Hoffman
*
*/
#include "nwxZip/nwxZipOutput.h"
#include "nwxZip/nwxZipInput.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <memory>
#include "nwx/nwxFileUtil.h"


bool nwxZipOutput::OpenZip(const wxString &sFileName)
{
  wxFileName fname(sFileName);
  bool bRtn = false;
  if(fname.Normalize())
  {
    wxString sFullPath = fname.GetFullPath();
    std::auto_ptr<wxTempFileOutputStream> pStream(
      new wxTempFileOutputStream(sFullPath));
    if(pStream.get()->IsOk())
    {
      std::auto_ptr<wxZipOutputStream> pZip(
        new wxZipOutputStream( *(pStream.get()) ));
      bRtn = pZip.get()->IsOk();
      if(bRtn)
      {
        _Cleanup();
        m_pStream = pZip.release();
        m_pFileStream = pStream.release();
        m_sZipPath = sFullPath;
        bRtn = true;
      }
    }
  }
  return bRtn;
}

bool nwxZipOutput::CloseZip()
{
  bool bRtn = false;
  if(m_pStream == NULL) {}
  else if(!m_pStream->IsOk()) {}
  else if(!m_pStream->Close()) {}
  else if(!m_pFileStream->IsOk()) {}
  else if(!m_pFileStream->Close()) {}
  else
  {
    bRtn = true;
  }
  _Cleanup();
  return bRtn;
}

void nwxZipOutput::_Cleanup()
{
  _ClearBase();
  m_sZipPath.Empty();
#define DELPTR(p) if(p != NULL) { delete p; p = NULL; }
  DELPTR(m_pStream)
  DELPTR(m_pFileStream)
#undef DELPTR
  m_setDone.clear();
}

wxOutputStream *nwxZipOutput::OpenFile(const wxString &sFileName, bool bFullPath, const wxDateTime &dt)
{
  wxString sName = _BUILD_PATH(sFileName,bFullPath);
  wxOutputStream *pRtn = NULL;
  if(IsZipOpen() && !_BeenDone(sName) && m_pStream->IsOk())
  {
    _SetDone(sName);
    if(m_pStream->PutNextEntry(sName,dt))
    {
      pRtn = m_pStream;
    }
  }
  return pRtn;
}

bool nwxZipOutput::CloseFile()
{
  bool bRtn = false;
  if(IsZipOpen() && m_pStream->IsOk())
  {
    bRtn = m_pStream->CloseEntry();
  }
  return bRtn;
}

bool nwxZipOutput::Chdir(const wxString &sFileName, bool bFullPath)
{
  wxString sName = _BUILD_PATH(sFileName,bFullPath);
  nwxFileUtil::EndWithSeparator(&sName);
  _Chdir(sName);
  return true;
}
bool nwxZipOutput::WriteFileFromZipInput(nwxZipInput *pZipIn, const wxString &sFileNameIn)
{
  wxZipEntry *pE = pZipIn->_FindEntry(sFileNameIn,true);
  bool bRtn = false;
  if(pE != NULL)
  {
    bRtn = m_pStream->CopyEntry(
      new wxZipEntry(*pE),*pZipIn->m_pStream);
  }
  return bRtn;
}

bool nwxZipOutput::WriteFile(const wxString &sInputFilePath, const wxString &sFileName, bool bFullPath)
{
  wxString sZipFileName = _BUILD_PATH(sFileName,bFullPath);
  wxFileName fname(sInputFilePath);
  
  bool bRtn = false;
  wxOutputStream *pOUT;
  if( fname.IsFileReadable() )
  {
    std::auto_ptr<wxFFileInputStream> pIN(new wxFFileInputStream(fname.GetFullPath()));
    if( pIN.get()->IsOk() &&
        ( (pOUT = OpenFile(sZipFileName,true,fname.GetModificationTime())) != NULL )
      )
    {
      pOUT->Write(*(pIN.get()));
      wxULongLong nIn = fname.GetSize();
      wxFileOffset nOut = pOUT->TellO();
      if( pOUT->IsOk() &&
          pIN.get()->Eof() &&
          ( (nIn == nOut) || (nOut == 0) )
        )
      {
        bRtn = true;
      }
      CloseFile();
    }
  }
  return bRtn;
}

bool nwxZipOutput::WriteFileToCwd(const wxString &sInputFilePath)
{
  wxFileName fname(sInputFilePath);
  bool bRtn = false;
  if(fname.IsFileReadable() && fname.Normalize())
  {
    wxString sName = fname.GetFullName();
    bRtn = WriteFile(fname.GetFullPath(),sName,false);
  }
  return bRtn;
}

bool nwxZipOutput::WriteDir(const wxString &sInputPath) // may want to use relative to cwd
{
  bool bRtn = false;
  wxFileName fname(sInputPath);
  if(fname.IsDirReadable() && fname.Normalize())
  {
    wxString sDirIn = fname.GetFullPath();
    wxString sDir;
    wxArrayString asFiles;
    size_t nCount = wxDir::GetAllFiles(sDirIn,&asFiles);
    asFiles.Sort();
    bRtn = true;

    for(size_t i = 0; i < nCount; ++i)
    {
      const wxString &sFileName = asFiles.Item(i);
      wxFileName fn(sFileName);
      if(fn.IsDir()) 
      {
      }
      else if(fn.IsFileReadable() && fn.MakeRelativeTo(sDirIn))
      {
        sDir = fn.GetPath(true);
        Chdir(sDir,true);
        if(!WriteFileToCwd(sFileName))
        {
          bRtn = false;
          break;
        }
      }
    }
  }
  return bRtn;
}
