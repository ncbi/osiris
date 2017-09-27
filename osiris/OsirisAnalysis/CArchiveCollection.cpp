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
*  FileName: CArchiveCollection.h
*  Author:   Douglas Hoffman
*  Purpose:  Collect all info needed for creating an OSIRIS archive
*
*/
#include "CArchiveCollection.h"
#include "COARfile.h"
#include "CDirList.h"
#include "nwxZip/nwxZipOutput.h"

void CArchiveCollection::_Init()
{
  m_pFile = NULL;
  m_bMissingMessageBook = false;
  m_bMissingLadderInfo = false;
  m_bMissingOAR = false;

  m_asInput.clear();
  m_asInputMissing.clear();
  m_asOutput.clear();
  m_asOutputMissing.clear();
}

bool CArchiveCollection::SetFile(const COARfile *pFile)
{
  _Init();
  m_pFile = pFile;
  bool bRtn = false;
  if(m_pFile != NULL)
  {
    wxString sFile;
    size_t nSampleCount = m_pFile->GetSampleCount();
    size_t i;
    unsigned int nDel = m_pFile->GetDeletedSampleCount();
    const COARsample *pSample;
    m_asOutput.insert(m_pFile->GetFileName());
    for (i = 0; i < nSampleCount; ++i)
    {
      pSample = m_pFile->GetSample(i);
      sFile = m_pFile->FindPlotFile(pSample);
      if(sFile.IsEmpty())
      {
        m_asOutputMissing.insert(pSample->GetName());
      }
      else
      {
        m_asOutput.insert(sFile);
        bRtn = true;
      }
      sFile = m_pFile->FindInputFile(pSample);
      if(sFile.IsEmpty())
      {
        m_asInputMissing.insert(pSample->GetName());
      }
      else
      {
        m_asInput.insert(sFile);
      }
    }
    if(bRtn)
    {
      sFile = m_pFile->FindMessageBookFile();
      if(sFile.IsEmpty())
      {
        m_bMissingMessageBook = true;
      }
      else
      {
        m_asOutput.insert(sFile);
      }
      sFile = m_pFile->FindFileByName("ladderInfo.xml");
      if(sFile.IsEmpty())
      {
        m_bMissingLadderInfo = true;
      }
      else
      {
        m_asOutput.insert(sFile);
      }
      if( (!nDel) && !m_pFile->IsOAR())
      {
        sFile = m_pFile->FindFileByName(GetOARfileName());
        if(sFile.IsEmpty())
        {
          m_bMissingOAR = true;
        }
        else
        {
          m_asOutput.insert(sFile);
        }
      }
    }
    else
    {
      _Init();
    }
  }
  return bRtn;
}
wxString CArchiveCollection::GetOARfileName()
{
  wxFileName fn(m_pFile->GetHeadingFileName());
  wxString s = fn.GetFullName();
  return s;
}
const wxString &CArchiveCollection::GetInputFileType()
{
  return m_pFile->GetInputFileType();
}
size_t CArchiveCollection::_Missing(wxArrayString *pas, const SET_STR &ss)
{
  size_t nRtn = ss.size();
  if(nRtn && (pas != NULL))
  {
    for(SET_STR::const_iterator itr = ss.begin();
      itr != ss.end();
      ++itr)
    {
      pas->Add(*itr);
    }
  }
  return nRtn;
}
bool CArchiveCollection::WriteArchive(const wxString &sPath, bool bIncludeInput)
{
  wxFileName fn(sPath);
  wxDateTime dt;
  dt.SetToCurrent();
  wxString sTopDir = fn.GetName();
  sTopDir.Replace(wxT(" "),wxT("_"),true);
  sTopDir.Append(dt.Format("_%Y%m%d-%H%M%S"));
  nwxZipOutput zip(fn.GetFullPath());
  bool bRtn = false;
  if(zip.IsZipOpen())
  {
    SET_STR::iterator itr;
    bRtn = true;
    if(bIncludeInput && !m_asOutput.empty())
    {
      zip.Chdir(sTopDir,true);
      zip.Chdir(wxT("input"),false);
      for(itr = m_asInput.begin();
        itr != m_asInput.end();
        ++itr)
      {
        if(!zip.WriteFileToCwd(*itr))
        {
          bRtn = false;
          break;
        }
      }
    }
    if(bRtn)
    {
      zip.Chdir(sTopDir,true);
      zip.Chdir(wxT("output"),false);
      for(itr = m_asOutput.begin();
        itr != m_asOutput.end();
        ++itr)
      {
        if(!zip.WriteFileToCwd(*itr))
        {
          bRtn = false;
          break;
        }
      }
    }
    if(bRtn)
    {
      bRtn = zip.CloseZip();
    }
    else
    {
      zip.Discard();
    }
  }
  return bRtn;
}