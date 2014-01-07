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
*  FileName: nwxColorUtil.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __NWX_COLOR_UTIL_H__
#define __NWX_COLOR_UTIL_H__

#include <wx/window.h>
#include <wx/colour.h>

class nwxColorUtil
{
private:
  nwxColorUtil() {} // disable instantiation
  static unsigned char _Brighten(unsigned char x)
  {
    // compute the average of x and 256 rounded down
    // (x + 256) / 2 = x/2 + 256/2 = x/2 | 256
    x >>= 1;
    x |= 128;
    return x;
  }
public:
  static wxColour Average(const wxColour &c1, const wxColour &c2)
  {
#define AVG_INT_CH(n1,n2) (unsigned char) \
  ((( (unsigned int)(n1) + (unsigned int)(n2) ) >> 1) & 255)

    wxColour cRtn(
      AVG_INT_CH(c1.Red(),c2.Red()),
      AVG_INT_CH(c1.Green(),c2.Green()),
      AVG_INT_CH(c1.Blue(),c2.Blue()));
    return cRtn;
#undef AVG_INT_CH
  }
  static void Brighten(wxColour *pc)
  {
    unsigned char r = _Brighten(pc->Red());
    unsigned char g = _Brighten(pc->Green());
    unsigned char b = _Brighten(pc->Blue());
    unsigned char a = _Brighten(pc->Alpha());
    pc->Set(r,g,b,a);
  }
  static wxColour Brighten(const wxColour &c)
  {
    wxColour cRtn(c);
    Brighten(&cRtn);
    return cRtn;
  }
  static wxColour BrightenBackground(wxWindow *pw)
  {
    wxColour c(pw->GetBackgroundColour());
    Brighten(&c);
    return c;
  }
  static void BackgroundParent(wxWindow *pw, bool bBrighten = false)
  {
    wxWindow *pParent = pw->GetParent();
    if(pParent != NULL)
    {
      wxColour c = pParent->GetBackgroundColour();
      if(bBrighten) { Brighten(&c); }
      pw->SetBackgroundColour(c);
    }
  }
};

#endif
