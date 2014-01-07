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
*  FileName: nwxRound.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_ROUND_H__
#define __NWX_ROUND_H__

#include <math.h>

#ifdef _DEBUG
#include <wx/debug.h>
#endif

class nwxRound
{
public:
  nwxRound(double x)
  {
    m_n = Round(x);
  }
  nwxRound(const nwxRound &x)
  {
    m_n = x.Get();
  }
  static int Round(double d)
  {
    int n;
    if(d < 0.0)
    {
      n = -( (int) floor( 0.5 - d) );
    }
    else
    {
      n = (int) floor(0.5 + d);
    }
    return n;
  }
  operator int() const
  {
    return m_n;
  }
  int Get() const
  {
    return m_n;
  }
  nwxRound &operator = (double x)
  {
    m_n = Round(x);
    return *this;
  }
  nwxRound &operator = (const nwxRound &x)
  {
    m_n = x.Get();
    return *this;
  }
#ifdef _DEBUG
  static bool UnitTest()
  {
    nwxRound x1(-3.5);
    nwxRound x2(3.5);
    nwxRound x3(100.4999999);
    nwxRound x4(12345678.012345678);
    int nx[4] = { (int)x1,(int)x2,(int)x3,(int)x4 };
    int nv[4] = { -4,4,100,12345678};
    bool bRtn = true;
    for(int i = 0; i < 4; i++)
    {
      if(nx[i] != nv[i])
      {
        bRtn = false; // failed
        i = 4; // loop exit
      }
    }
    wxASSERT_MSG(bRtn,_T("nwxRound::UnitTest Failed"));
    return bRtn;
  }
#endif
private:
  int m_n;
};

#endif
