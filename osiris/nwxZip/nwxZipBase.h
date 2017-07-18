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
*  FileName: nwxZipBase.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_ZIP_BASE_H__
#define __NWX_ZIP_BASE_H__

#include <wx/string.h>

class nwxZipBase
{
public:
  nwxZipBase() {}
  virtual ~nwxZipBase() {}
  wxString Getdir()
  {
    return m_sCWD;
  }
  const wxString &GetOpenZipFilePath()
  {
    return m_sZipPath;
  }

protected:
  friend class DIR_HOLD;
  class DIR_HOLD
  {
  public:
    DIR_HOLD(nwxZipBase *p) :
        m_pZip(p),
        m_sCWD(p->_Dir())
    {}
    virtual ~DIR_HOLD()
    {
      m_pZip->_Chdir(m_sCWD);
    }
  private:
    wxString m_sCWD;
    nwxZipBase *m_pZip;
  };

  wxString _BUILD_PATH(
    const wxString &sFileName, 
    bool bFullPath)
  {
    wxString sRtn;
    if(bFullPath)
    {
      sRtn = sFileName;
    }
    else
    {
      sRtn = __MAKE_PATH(_Dir(),sFileName);
    }
    return sRtn;
  }
  void _Chdir(const wxString &s)
  {
    m_sCWD = s;
  }
  void _ClearDir()
  {
    m_sCWD.Empty();
  }
  void _ClearBase()
  {
    _ClearDir();
    m_sZipPath.Empty();
  }
  const wxString &_Dir() const
  {
    return m_sCWD;
  }
  void _SetOpenZipFilePath(const wxString &s)
  {
    m_sZipPath = s;
  }
  static wxString __MAKE_PATH(const wxString &sDir, const wxString &sPath);
  // __MAKE_PATH is implemented in nwxZipInput.cpp
private:
  wxString m_sCWD;
  wxString m_sZipPath;

};


#endif