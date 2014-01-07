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
*  FileName: nwxKillDir.cpp
*  Author:   Douglas Hoffman -- needs testing
*
*/

#include "nwx/nwxKillDir.h"
#include "nwx/nwxFileUtil.h"
#include <wx/filefn.h>
#ifdef _DEBUG
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#endif

wxDirTraverseResult nwxKillDir::OnDir(const wxString &dirname)
{
  if(m_bRecursive)
  {
    _Kill(dirname);
  }
  return wxDIR_IGNORE;
}
wxDirTraverseResult nwxKillDir::OnFile(const wxString &filename)
{
  // there was a problem deleting the file here, possibly because
  // the directory was modified when traversing
  if(nwxFileUtil::IsNewer(filename,m_t))
  {
    m_asFiles.push_back(filename);
  }
  return wxDIR_CONTINUE;
}

void nwxKillDir::_Kill(const wxString &sDir)
{
  m_asDirs.push_back(sDir);
  {
    wxDir dir(sDir);
    if(dir.IsOpened())
    {
      list<wxString>::reverse_iterator itr;
      size_t nCount = m_asFiles.size();
      dir.Traverse(*this);
      for(size_t n = m_asFiles.size();
        n > nCount;
        --n)
      {
        itr = m_asFiles.rbegin();
        ::wxRemoveFile(*itr);
        m_asFiles.pop_back();
      }
    }
  }
}

bool nwxKillDir::Kill(
  const wxString &sDir,
  bool bRecursive,
  time_t t)
{
  bool bRtn = true;
  m_bRecursive = bRecursive;
  m_t = t;
  list<wxString>::reverse_iterator itr;
  size_t nCount = m_asDirs.size();
  _Kill(sDir);
  for(size_t n = m_asDirs.size();
    (n > nCount);
    --n)
  {
    itr = m_asDirs.rbegin();
    if(!::wxRmdir(*itr))
    {
      bRtn = false;
    }
    m_asDirs.pop_back();
  }
  return bRtn;
}


#ifdef _DEBUG
void nwxKillDir::UnitTest()
{
  wxStandardPaths sp;
  wxString sDocs = sp.GetDocumentsDir();
  wxString sDir;
  wxString sFile;
  wxString sBase;
  wxString sError;
  wxString HELLO(_T("hello world\r\n"));
  wxFile fh;
  int i = 0;
  int j;
  wxChar cSep = wxFileName::GetPathSeparator();
  bool bOK = true;
  if(sDocs.Last() != cSep)
  {
    sDocs.Append(cSep);
  }
  bool bExists = true;
  while(bExists)
  {
    i++;
    sBase = wxString::Format(_T("xx%06d"),i);
    sDir = sDocs + sBase;
    bExists = (wxFile::Exists(sDir) || wxDir::Exists(sDir));
  }
  for(i = 0; bOK && (i < 4); i++)
  {
    if(::wxMkdir(sDir))
    {
      sDir.Append(cSep);
      for(j = 1; bOK && (j < 4); j++)
      {
        sFile = sDir;
        sFile += wxString::Format(_T("hello%d.txt"),j);
        if(!( fh.Open(sFile,wxFile::write) &&
          fh.Write(HELLO) &&
          fh.Flush() ))
        {
          bOK = false;
          sError = _T("Could not create file: ");
          sError.Append(sFile);
        }
        if(fh.IsOpened()) { fh.Close(); }
      }
      sDir.Append(sBase);
    }
    else
    {
      sError = _T("Could not make dir: ");
      sError.Append(sDir);
      bOK = false;
    }
  }
  if(bOK)
  {
    // at this point files were made
    sDir = sDocs + sBase;
    if(!wxDir::Exists(sDir))
    {
      sError = _T("Directory: ");
      sError.Append(sDir);
      sError.Append(_T("\ndoes not exist -- not created"));
      bOK = false;
    }
    else if(!nwxKillDir::KillStatic(sDir))
    {
      sError = _T("Could not kill dir: ");
      sError.Append(sDir);
      bOK = false;
    }
    else if(wxDir::Exists(sDir))
    {
      sError = _T("Directory: ");
      sError.Append(sDir);
      sError.Append(_T("\nstill exists -- nwxKillDir failed!"));
      bOK = false;
    }
  }
  if(!bOK)
  {
    wxASSERT_MSG(0,sError);
  }
}
#endif
