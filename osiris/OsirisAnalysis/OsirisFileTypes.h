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
*  FileName: OsirisFileTypes.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __OSIRIS_FILE_TYPES_H__
#define __OSIRIS_FILE_TYPES_H__

#define EXT_GRAPHIC ".plt"
#define EXT_REPORT ".oar"
#define EXT_REPORT_EDITED ".oer"
#define EXT_BATCH ".obr"
#define EXT_CMF ".cmf"


#define EXT_PNG ".png"

#define DESCRIPTION_PNG "Portable Network Graphics"

#define DESCRIPTION_GRAPHIC "Plot"
#define DESCRIPTION_REPORT "Report"
#define DESCRIPTION_REPORT_EDITED "Edited Report"
#define DESCRIPTION_BATCH "Batch"

#define FILE_TYPE(S,X) "OSIRIS " S " (*" X ")|*" X
#define FILE_TYPE2(S,X,X2) "OSIRIS " S " (*" X ";*" X2 ")|*" X ";*" X2

#define FILE_TYPE_REPORT_SAVE_AS FILE_TYPE(DESCRIPTION_REPORT_EDITED,EXT_REPORT_EDITED)
#define FILE_TYPE_REPORT FILE_TYPE2(DESCRIPTION_REPORT,  EXT_REPORT, EXT_REPORT_EDITED)
#define FILE_TYPE_GRAPHIC FILE_TYPE(DESCRIPTION_GRAPHIC, EXT_GRAPHIC)
#define FILE_TYPE_BATCH   FILE_TYPE(DESCRIPTION_BATCH,   EXT_BATCH)
#define FILE_TYPE_PNG     FILE_TYPE(DESCRIPTION_PNG,     EXT_PNG)
#define FILE_TYPE_CMF     "CODIS CMF 3.2 File (*" EXT_CMF  ")|*" EXT_CMF

inline bool IS_FILE_TYPE(const wxString &s, const char *psType)
{
  size_t nLen = strlen(psType);
  size_t nsLen = s.Len();
  bool bRtn = false;
  if(nsLen > nLen)
  {
    wxString sEnd(s.Mid(nsLen - nLen));
    bRtn = !sEnd.CmpNoCase(_T(psType));
  }
  return bRtn;
}
inline bool IS_ANALYSIS_FILE(const wxString &s)
{
  wxString sLower = s;
  bool b = sLower.EndsWith(EXT_REPORT) || sLower.EndsWith(EXT_REPORT_EDITED);
  return b;
}

#endif
