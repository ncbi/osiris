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
*  FileName: ltoa.cpp
*
*/
#include <stdio.h>

static inline char CHAR(int nDigit)
{
  char cRtn;
  if(nDigit > 9)
  {
    cRtn = ('A' + nDigit - 10);
  }
  else
  {
    cRtn = ('0' + nDigit);
  }
  return cRtn;
}

char *_ultoa(unsigned long value, char *buffer, int radix)
{
  if(value == 0)
  {
    *buffer = '0';
    buffer[1] = 0;
  }
  else if(radix == 10)
  {
    sprintf(buffer,"%lu",value);
  }
  else if(radix == 16)
  {
    sprintf(buffer,"%lX",value);
  }
  else if(radix > 1 && radix <= 36)
  {
    int nDigit;
    char xx[64];
    char *pxx = xx;
    char *pBuffer = buffer;
    *pxx = 0;
    // set up digits in reverse order
    while(value > radix)
    {
      nDigit = value % radix;
      value /= radix;
      *pxx++ = CHAR(nDigit);
      *pxx = 0;
    }
    if(value > 0)
    {
      *pxx = CHAR(value);
    }
    else
    {
      // make pxx point to last digit
      pxx--;
    }
    while(pxx >= xx)
    {
      *pBuffer = *pxx;
      pBuffer++;
      pxx--;
    }
    *pBuffer = (char)0;
  }
  else
  {
    *buffer = 0;
  }
  return buffer;
}
char *_ltoa(long value, char *buffer, int radix)
{
  char *rtn = buffer;
  if(value < 0)
  {
    *buffer = '-';
    buffer++;
    value = -value;
  }
  _ultoa((unsigned long) value, buffer,radix);
  return rtn;
}
char *itoa(int value, char *buffer, int radix)
{
  return _ltoa((long)value,buffer,radix);
}
