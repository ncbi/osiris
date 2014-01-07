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

*  FileName: nwxDoubleValidator.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_DOUBLE_VALIDATOR_H__
#define __NWX_DOUBLE_VALIDATOR_H__

#include <wx/valtext.h>
#include <wx/arrstr.h>
#include "nwx/nwxTypeValidator.h"

//  text validator used for floating point numbers
//  of the form
//
//   [\-+]?\d*(\.\d*){0,1}?

class nwxDoubleValidator : public tnwxTypeValidator<double>
{
public:
  nwxDoubleValidator(
    double *pd = NULL, 
    double dLo = -nwxDoubleValidator::BIG_NUMBER,
    double dHi = nwxDoubleValidator::BIG_NUMBER,
    double dDefault = 0.0,
    bool bAllowEmpty = true) :
    tnwxTypeValidator<double>(pd,dLo,dHi,dDefault,bAllowEmpty,true)
  {
    SetStyle(wxFILTER_NUMERIC);
  }
  nwxDoubleValidator(const nwxDoubleValidator &x)
    : tnwxTypeValidator<double>(x)
  {
    SetStyle(wxFILTER_NUMERIC);
  }
  virtual wxValidator *Clone() const
  {
    return new nwxDoubleValidator(*this);
  }
  static const double BIG_NUMBER;

  virtual wxString ToString(double x);
  virtual double FromString(const wxString &s);
  virtual bool ValidateString(const wxString &s);

};


#endif
