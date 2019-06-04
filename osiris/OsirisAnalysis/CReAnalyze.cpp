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
*  FileName: CReAnalyze.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include "CReAnalyze.h"
#include "COARfile.h"
#include "COARsample.h"
#include "CParmOsiris.h"
#include "nwx/nwxFileUtil.h"
#include <wx/datetime.h>
#include <wx/filefn.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dir.h>

CReAnalyze::CReAnalyze(COARfile *pFile) :
  m_pParms(new CParmOsiris(pFile->GetParameters())),
  m_bOK(true)
{
  m_bOK = 
  _FindSamples(pFile) && 
    _Mkdir(pFile) &&
    _CopySamples();

  if(!m_sErrors.IsEmpty())
  {
    mainApp::LogMessage(m_sErrors);
  }
}
CReAnalyze::~CReAnalyze()
{
  _Cleanup();
}
void CReAnalyze::_Cleanup() 
{
  if(m_pParms != NULL)
  {
    delete m_pParms;
    m_pParms = NULL;
  }
}


bool CReAnalyze::CleanupNewFiles()
{
  //  this wasn't successful, remove files
  wxString sError;
  bool bRemoveFail = false;
  vector<wxString>::iterator itr;
  for(itr = m_vsNewFiles.begin();
    itr != m_vsNewFiles.end();
    ++itr)
  {
    if(wxFile::Exists(*itr) && !wxRemoveFile(*itr))
    {
      bRemoveFail = true; // failure, do not remove directory
      sError = "Cannot delete file, ";
      sError.Append(*itr);
      mainApp::LogMessage(sError);
      sError.Empty();
    }
  }
  m_vsNewFiles.clear();

  if(bRemoveFail)
  {}
  else if(!wxRmdir(m_sNewInputDir))
  {
    sError = "Cannot remove directory, ";
    sError.Append(m_sNewInputDir);
    mainApp::LogMessage(sError);
    bRemoveFail = true;
  }
  return !bRemoveFail;
}
bool CReAnalyze::_CopySamples()
{
  wxString sNewName;
  wxString sOldName;
  wxString sError;
  vector<wxString> vsCopy;
  bool bRtn = true;

  vector<wxString>::iterator itr;
  m_vsNewFiles.reserve(m_vsSamples.size());
  for(itr = m_vsSamples.begin();
    bRtn && (itr != m_vsSamples.end());
    ++itr)
  {
    sNewName = m_sNewInputDir;
    sOldName = m_sInputDir;
    sNewName.Append(*itr);
    sOldName.Append(*itr);
    if(!wxCopyFile(sOldName,sNewName,false))
    {
      sError = "Could not copy file, ";
      sError.Append(*itr);
      sError.Append("\n");
      m_sErrors.Append(sError);
      bRtn = false;
    }
    m_vsNewFiles.push_back(sNewName);
  }
  if(bRtn)
  {
    m_pParms->SetInputDirectory(m_sNewInputDir);
  }
  else
  {
    CleanupNewFiles();
  }
  return bRtn;
}
bool CReAnalyze::_FindSamples(COARfile *pFile)
{
  vector<const COARsample *> vpSamples;
  wxString sError;
  wxString sName;
  wxString sInputDir = pFile->GetInputDirectory();
  bool bRtn = false;
  FIX_FILE_NAME(sInputDir);
  nwxFileUtil::NoEndWithSeparator(&sInputDir);
  if(!wxDir::Exists(sInputDir))
  {
    sError = "Input directory, ";
    sError += sInputDir;
    sError += (", does not exist.\n");
    m_sErrors.Append(sError);
  }
  else if(!pFile->GetDisabledSamples(&vpSamples,true))
  {
    m_sErrors.Append("No disabled samples found.\n");
  }
  else
  {
    wxString sEXT(pFile->GetFileExt(false));
    wxString sNotFound;
    sInputDir = m_pParms->GetInputDirectory();
    vector<const COARsample *>::iterator itr;
    int nMissing = 0;
    
    nwxFileUtil::EndWithSeparator(&sInputDir);
    m_vsSamples.reserve(vpSamples.size());


    for(itr = vpSamples.begin();
      itr != vpSamples.end();
      ++itr)
    {
      sName = (*itr)->GetName();
      wxFileName fn(sInputDir,sName,sEXT);
      if(!fn.FileExists())
      {
        sNotFound.Append("   ");
        sNotFound.Append(sName);
        sNotFound.Append("\n");
        nMissing++;
      }
      else
      {
        // full file name with no path
        m_vsSamples.push_back(fn.GetFullName());
      }
    }
    if(!sNotFound.IsEmpty())
    {
      sError.Printf("Cannot find the following file%s:\n",
        (nMissing > 1) ? "s" : "");
      m_sErrors.Append(sError);
      m_sErrors.Append(sNotFound);
    }
    bRtn = m_sErrors.IsEmpty() && !m_vsSamples.empty();
    if(bRtn)
    {
      m_sInputDir = sInputDir;
    }
  }
  return bRtn;
}


bool CReAnalyze::_Mkdir(COARfile *)
{
  wxString sInputDir = m_pParms->GetInputDirectory();
  wxString sNewDir;
  wxString sDate;
  wxString snr;
  int i = 1;
  const int MAX_TRY = 500;
  bool bRtn = false;
  const wxChar *FMT = wxS("Could not create subdirectory,\n");
  wxDateTime dt;
  dt.SetToCurrent();
  sDate = dt.Format("retry_%y_%m_%d_%H%M");
  FIX_FILE_NAME(sInputDir);
  nwxFileUtil::EndWithSeparator(&sInputDir);
  sNewDir = sInputDir + sDate;
  while((i <= MAX_TRY) && (wxFileName::FileExists(sNewDir)))
  {
    snr.Printf(".%d",i++);
    sNewDir = sInputDir + sDate + snr;
  }
  if(i > MAX_TRY)
  {
    m_sErrors.Append(FMT);
    m_sErrors.Append(sInputDir);
    m_sErrors.Append(sDate);
    m_sErrors.Append(wxString::Format(
      ",\nafter %d attempts.\n",MAX_TRY));
  }
  else if(!wxMkdir(sNewDir))
  {
    m_sErrors.Append(FMT);
    m_sErrors.Append(sNewDir);
    m_sErrors.Append("\n");
  }
  else
  {
    nwxFileUtil::EndWithSeparator(&sNewDir);
    m_sNewInputDir = sNewDir;
    bRtn = true;
  }
  return bRtn;
}
