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
*  FileName: nwxNumberValidator.h
*  Author:   Douglas Hoffman
*
*  Subclass of nwxDoubleValidator which can be switched to 
*
*/
#ifndef __NWX_NUMBER_VALIDATOR_H__
#define __NWX_NUMBER_VALIDATOR_H__

#include "nwx/nwxDoubleValidator.h"

class nwxNumberValidator : public nwxDoubleValidator
{
public:
  nwxNumberValidator(
    double *pd = NULL, 
    double dLo = -nwxDoubleValidator::BIG_NUMBER,
    double dHi = nwxDoubleValidator::BIG_NUMBER,
    double dDefault = 0.0,
    bool bAllowEmpty = true,
    bool bTypeInt = false) : nwxDoubleValidator
      (pd,dLo,dHi,dDefault,bAllowEmpty),
      m_bTypeInt(bTypeInt)
  {};

  nwxNumberValidator(const nwxNumberValidator &x)
    : nwxDoubleValidator(x)
  {
    m_bTypeInt = x.m_bTypeInt;
  }
  virtual wxValidator *Clone() const
  {
    return new nwxNumberValidator(*this);
  }
  void SetTypeInt(bool bTypeInt)
  {
    m_bTypeInt = bTypeInt;
  }
  bool GetTypeInt()
  {
    return m_bTypeInt;
  }

  virtual bool ValidateString(const wxString &s)
  {
    bool bRtn = nwxDoubleValidator::ValidateString(s);
    if(bRtn && m_bTypeInt)
    {
      bRtn = !s.Contains(".");
    }
    return bRtn;
  }

private:
  bool m_bTypeInt;
};


#endif
