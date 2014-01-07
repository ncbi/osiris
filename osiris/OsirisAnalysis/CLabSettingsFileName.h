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
*  FileName: CLabSettingsFileName.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_LAB_SETTINGS_FILE_NAME_H__
#define __C_LAB_SETTINGS_FILE_NAME_H__

#include <wx/filename.h>

class CLabSettingsFileName
{
public:
  CLabSettingsFileName() : m_bSetup(false)
  {;}
  
  virtual ~CLabSettingsFileName() 
  {;}

  void Reset()
  {
    m_bSetup = false;
  }
  const wxString &GetFileName()
  {
    _CheckSetup();
    return m_sFileName;
  }
  bool IsReadable()
  {
    _CheckSetup();
    return m_fn.IsFileReadable();
  }
  bool IsWritable()
  {
    _CheckSetup();
    return m_bOriginalFile ? false : m_fn.IsFileWritable();
  }
  wxDateTime GetModTime()
  {
    _CheckSetup();
    return m_fn.GetModificationTime();
  }
  operator const wxString &()
  {
    return GetFileName();
  }

private:
  void _CheckSetup()
  {
    if(!m_bSetup) { _Setup(); }
  }
  void _Setup();
  wxFileName m_fn;
  wxString m_sFileName;
  bool m_bOriginalFile;
  bool m_bSetup;
};

#endif
