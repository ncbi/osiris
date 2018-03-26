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

*  FileName: CProcessAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PROCESS_ANALYSIS_H__
#define __C_PROCESS_ANALYSIS_H__

#include <stdlib.h>
#include "nwx/nwxProcess.h"

class CDirEntry;
class CVolume;

class CProcessAnalysis : public nwxProcess
{
public:
  CProcessAnalysis(
    CDirEntry *pDirEntry, 
    const CVolume *pVolume,
    wxEvtHandler *parent, 
    int nID = wxID_ANY);
  virtual ~CProcessAnalysis();
  virtual void ProcessLine(
    const char *p, size_t nLen, bool bErrStream);
  double GetProgress()
  {
    return m_dProgress;
  }
  bool IsOutputModified();
  CDirEntry *GetDirEntry()
  {
    return m_pDirEntry;
  }
  wxString GetOutput();

private:
  char *m_psExe;
  double m_dProgress;
  CDirEntry *m_pDirEntry;
};


#endif
