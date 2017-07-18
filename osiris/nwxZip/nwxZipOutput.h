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
*  FileName: nwxZipOutput.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_ZIP_OUTPUT_H__
#define __NWX_ZIP_OUTPUT_H__

#include <wx/string.h>
#include <wx/stream.h>
#include <wx/datetime.h>
#include "nwxZip/nwxZipBase.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

class wxZipOutputStream;
class wxZipEntry;
class wxTempFileOutputStream;
class nwxZipInput;

class nwxZipOutput : public nwxZipBase
{
public:
  nwxZipOutput() :
    m_pStream(NULL),
    m_pFileStream(NULL)
    {}

  nwxZipOutput(const wxString sZipFilePath) :
    m_pStream(NULL),
    m_pFileStream(NULL)
    {
      OpenZip(sZipFilePath);
    }
  virtual ~nwxZipOutput()
  {
    CloseZip();
  }
  bool OpenZip(const wxString &sZipFilePath);
  bool CloseZip();
  void Discard()
  {
    _Cleanup();
  }
  bool IsZipOpen()
  {
    return (m_pStream != NULL);
  }
  wxOutputStream *OpenFile(const wxString &sFileName,bool bFullPath = false, const wxDateTime &dt = wxDateTime::Now() );
  bool CloseFile();

  bool WriteFileFromZipInput(nwxZipInput *pZipIn, const wxString &sFileNameIn);
  bool WriteFileToCwd(const wxString &sInputFilePath);
  bool WriteFile(const wxString &sInputFilePath, const wxString &sFileName, bool bFullPath = false);
  bool WriteDir(const wxString &sInputPath);
  bool Chdir(const wxString &sFileName, bool bFullPath = false);
private:
  bool _BeenDoneRaw(const wxString &s)
  {
    std::set<wxString>::iterator itr = m_setDone.find(s);
    bool bRtn = (itr != m_setDone.end());
    return bRtn;
  }
  void _SetDoneRaw(const wxString &s)
  {
    m_setDone.insert(s);
  }
#ifdef __WXMSW__
  bool _BeenDone(const wxString &s)
  {
    wxString sl(s);
    sl.MakeLower();
    return _BeenDoneRaw(sl);
  }
  void _SetDone(const wxString &s)
  {
    wxString sl(s);
    sl.MakeLower();
    _SetDoneRaw(sl);
  }
#else
  bool _BeenDone(const wxString &s)
  {
    return _BeenDoneRaw(s);
  }
  void _SetDone(const wxString &s);
  {
    _SetDoneRaw(s);
  }
#endif


  void _Cleanup();
  std::set<wxString> m_setDone;
  wxString m_sZipPath;
  wxZipOutputStream *m_pStream;
  wxTempFileOutputStream *m_pFileStream;
};





#endif
