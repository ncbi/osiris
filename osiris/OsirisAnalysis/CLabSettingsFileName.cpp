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
*  FileName: CLabSettingsFileName.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CLabSettingsFileName.h"
#include "mainApp.h"
#include "ConfigDir.h"

#ifndef __WXMSW__
#include <sys/stat.h>
#include <copyfile.h>
static bool CopyFile(const char *psOrig, const char *psNew, bool bFailIfExists)
{
  struct stat buf;
  bool bRtn = false;
  if(bFailIfExists && !stat(psNew,&buf))
  {;  // dest exists
  }
  else if(stat(psOrig,&buf))
  {
    ; // original file does not exist
  }
  else if(copyfile(psOrig, psNew,NULL,COPYFILE_ALL) == 0)
  {
    bRtn = true;
  }
  return bRtn;
}

#endif

void CLabSettingsFileName::_Setup()
{
  // this should be called ONLY by CLabSettingsFileName::_CheckSetup()

  ConfigDir *pDir = mainApp::GetConfig();
  wxString sFile = pDir->GetExeConfigPath();
  wxString sOriginal = sFile;
  sFile += "GenericLabSettings.xml";
  sOriginal += "defaultlab.xml";
  wxFileName fn(sFile);
  m_bOriginalFile = false;

  if(!fn.FileExists())
  {
    wxFileName fno(sOriginal);
    m_bOriginalFile = true;
    if(!fno.IsFileReadable())
    {
      m_fn.Clear();
    }
    else if(!CopyFile(sOriginal.utf8_str(),sFile.utf8_str(),true))
    {
      m_fn = fno;
    }
    else
    {
      wxFileName fn2(sFile); // not sure if fn is valid
              // because the file did not exist when
              // the class was instantiated.
              // Create a new wxFileName with sFile
              // and assign it to m_fn
      m_fn = wxFileName(sFile);
      m_bOriginalFile = false;
    }
  }
  else if(!fn.IsFileReadable())
  {
    // file exists but is not readable,
    // use defaultlab.xml if possible

    wxFileName fno(sOriginal);
    m_fn = fno;
    m_bOriginalFile = true;
  }
  else
  {
    m_fn = fn;
  }
  if(m_fn.IsFileReadable())
  {
    m_sFileName = m_fn.GetFullPath();
  }
  else
  {
    m_sFileName.Empty();
  }
  m_bSetup = true;
}
