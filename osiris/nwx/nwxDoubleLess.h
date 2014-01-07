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
*  FileName: nwxDoubleLess.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_DOUBLE_LESS__
#define __NWX_DOUBLE_LESS__

class nwxDoubleLess
{
  // less than function object for fixed point numbers
  //  this funtion rounds the number to a number of (default 3)
  //  decimal places before comparing
  //  for example:
  //     (3.123 < 3.123111) == false
  //     (3.123111 < 3.123) == false
  //     (3.0 < 4.0) == true
  //
private:
  double _ROUND(double d) const
  {
    // may have undesired results if d < 0
    // ROUND(double d) takes care of this issue
    return floor(d * m_dMult + m_dRound);
  }
public:
  nwxDoubleLess() 
  {
    m_dMult = 1000.0;
    m_dRound = 0.5;
  }
  nwxDoubleLess(double dMult, double dRound = 0.5) : 
    m_dMult(dMult), m_dRound(dRound)
  {;}
  virtual ~nwxDoubleLess() {;}
  double ROUND(double d) const
  {
    double dRtn;
    if(d < 0)
    {
      dRtn = -_ROUND(-d);
    }
    else
    {
      dRtn = _ROUND(d);
    }
    return dRtn;
  }
  bool operator()(double d1, double d2) const
  {
    double d1x = ROUND(d1);
    double d2x = ROUND(d2);
    return d1x < d2x;
  }
  int cmp(double d1, double d2) const
  {
    double d1x = ROUND(d1);
    double d2x = ROUND(d2);
    int nRtn = 0;
    if(d1x < d2x)
    {
      nRtn = -1;
    }
    else if(d1x > d2x)
    {
      nRtn = 1;
    }
    return nRtn;
  }
protected:
  double m_dMult;
  double m_dRound;
};

#endif
