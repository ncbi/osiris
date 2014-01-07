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

*  FileName: nwxIntValidator.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_INT_VALIDATOR_H__
#define __NWX_INT_VALIDATOR_H__

#include <limits.h>
#include <wx/valtext.h>
#include <wx/arrstr.h>
#include "nwx/nwxTypeValidator.h"

//  text validator used for floating point numbers
//  of the form
//
//   [\-+]?\d*

class nwxIntValidator : public tnwxTypeValidator<int>
{
public:
  nwxIntValidator(
    int *pn = NULL, 
    int nLo = INT_MIN,
    int nHi = INT_MAX,
    int nDefault = 0,
    bool bAllowEmpty = true) :
    tnwxTypeValidator<int>(pn,nLo,nHi,nDefault,bAllowEmpty,true)
  {
    SetStyle(wxFILTER_NUMERIC);
  }
  nwxIntValidator(const nwxIntValidator &x) :  tnwxTypeValidator<int>(x)
  {
    SetStyle(wxFILTER_NUMERIC);  
  }
  virtual wxValidator *Clone() const
  {
    return new nwxIntValidator(*this);
  }
  virtual wxString ToString(int x);
  virtual int FromString(const wxString &s);
  virtual bool ValidateString(const wxString &s);

protected:
};


class nwxUintValidator : public tnwxTypeValidator<unsigned int>
{
public:
  nwxUintValidator(
    unsigned int *pn = NULL, 
    unsigned int nLo = 0,
    unsigned int nHi = UINT_MAX,
    unsigned int nDefault = 0,
    bool bAllowEmpty = true) :
    tnwxTypeValidator<unsigned int>(pn,nLo,nHi,nDefault,bAllowEmpty,true)
  {
    SetStyle(wxFILTER_NUMERIC);
  }

  nwxUintValidator(const nwxUintValidator &x) :  tnwxTypeValidator<unsigned int>(x)
  {
    SetStyle(wxFILTER_NUMERIC);
  }

  virtual wxValidator *Clone() const
  {
    return new nwxUintValidator(*this);
  }
  virtual wxString ToString(unsigned int x);
  virtual unsigned int FromString(const wxString &s);
  virtual bool ValidateString(const wxString &s);
};

#endif
