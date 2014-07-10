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

*  FileName: nwxIntValidator.cpp
*  Author:   Douglas Hoffman
*
*/
#include <stdlib.h>
#include <ctype.h>
#include "nwx/nwxIntValidator.h"
#include <wx/textctrl.h>
#include <wx/msgdlg.h>


bool nwxIntValidator::ValidateString(const wxString &sIn)
{
  const char *ps(sIn.utf8_str());

  while(isspace(*ps)) { ps++; }  // leading spaces are OK
  if(strchr("+-",*ps)) { ps++; } // signed, optional
  while(isdigit(*ps))  { ps++; } // digits
  while(isspace(*ps)) { ps++; }  // trailing spaces are OK
  return !(*ps);                 // if end of string, return true
}

int nwxIntValidator::FromString(const wxString &s)
{
  const char *ps(s.utf8_str());
  while(isspace(*ps)) { ps++; }
  int n = atoi(ps);
  return n;
}

wxString nwxIntValidator::ToString(int x)
{
  wxString s;
  s.Printf("%d",x);
  return s;
}

bool nwxUintValidator::ValidateString(const wxString &sIn)
{
  const char *ps(sIn.utf8_str());

  while(isspace(*ps)) { ps++; }  // leading spaces are OK
  while(isdigit(*ps))  { ps++; } // digits
  while(isspace(*ps)) { ps++; }  // trailing spaces are OK
  return !(*ps);                 // if end of string, return true
}

unsigned int nwxUintValidator::FromString(const wxString &s)
{
  const char *ps(s.utf8_str());
  while(isspace(*ps)) { ps++; }
  unsigned int n = (unsigned int) atoi(ps);
  return n;
}

wxString nwxUintValidator::ToString(unsigned int x)
{
  wxString s;
  s.Printf("%u",x);
  return s;
}

