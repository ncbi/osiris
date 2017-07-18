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
*    Info:  Are there any deleted samples?
*           are all files available, oar/oer, plot, message book, ladder info
*           are input (fsa/hid) files available
*
*/
#ifndef __CARCHIVE_COLLECTION_H__
#define __CARCHIVE_COLLECTION_H__

#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"
#include <wx/string.h>

class COARfile;
class wxArrayString;

class CArchiveCollection
{
public:
  CArchiveCollection() : m_pFile(NULL)
  {}
  CArchiveCollection(const COARfile *pFile) : m_pFile(NULL)
  {
    SetFile(pFile);
  }
  CArchiveCollection(const COARfile &oarFile) : m_pFile(NULL)
  {
    SetFile(oarFile);
  }
  virtual ~CArchiveCollection() {}

  bool SetFile(const COARfile *pFile);
  bool SetFile(const COARfile &oarFile)
  {
    return SetFile(&oarFile);
  }
  bool HasInputFiles()
  {
    return (m_asInput.size() > 0);
  }
  bool MissingMessageBook()
  {
    return m_bMissingMessageBook;
  }
  bool MissingLadderInfo()
  {
    return m_bMissingLadderInfo;
  }
  bool MissingOAR()
  {
    return m_bMissingOAR;
  }
  bool HasPlotFiles()
  {
    return (m_asOutput.size() > 0);
  }
  const wxString &GetInputFileType();
  size_t SamplesMissingInputFiles(wxArrayString *pas = NULL)
  {
    return _Missing(pas,m_asInputMissing);
  }
  size_t SamplesMissingPlotFiles(wxArrayString *pas = NULL)
  {
    return _Missing(pas,m_asOutputMissing);
  }
  wxString GetOARfileName();
  bool WriteArchive(const wxString &sPath, bool bIncludeInput);
private:
  typedef std::set<wxString> SET_STR;
  static size_t _Missing(wxArrayString *pas, const SET_STR &ss);
  void _Init();

  const COARfile *m_pFile; // usually oer
  bool m_bMissingMessageBook;
  bool m_bMissingLadderInfo;
  bool m_bMissingOAR;

  SET_STR m_asInput;    // list of input files (fsa or hid)
  SET_STR m_asInputMissing; // list of missing input files

  SET_STR m_asOutput;   // list of output file (oar,oer,plt,xml)
  SET_STR m_asOutputMissing; // list of missing output files
};




#endif