#ifndef __C_PRINT_OUT_TEST_PATTERN_H__
#define __C_PRINT_OUT_TEST_PATTERN_H__

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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*
*  FileName: CPrintOutTestPattern.h
*  Author:   Douglas Hoffman
*
*  Implementation for wxPrintout for printing an OSIRIS test pattern
*  used for adjusting colors on a printout
*
*/

#include "CPrintOut.h"

class CPrintOutTestPattern : public CPrintOut
{
public:
  static void Print(wxWindow *parent)
  {
    CPrintOutTestPattern printout(parent, false);
    _DoPrint(&printout, wxT("PrintTestPattern"));
  }
  virtual bool HasPage(int page)
  {
    return (page == 1);
  }
  virtual const wxChar *GetPrintoutType()
  {
    return wxT("TestPattern");
  }
  virtual int GetMaxPage() { return 1; }
  virtual wxWindow *GetParent()
  {
    return m_pParent;
  }

  virtual bool OnPrintPage(int page);
  virtual void GetPageInfo(
    int *minPage, int *maxPage, int *selPageFrom, int *selPageTo)
  {
    *minPage = 1;
    *maxPage = 1;
    *selPageFrom = 1;
    *selPageTo = 1;
  }
private:
  void _CleanupBitmap()
  {
    if (m_pBitmap != NULL)
    {
      delete m_pBitmap;
      m_pBitmap = NULL;
    }
  }
  bool _IsBitmapReady(bool bPortrait)
  {
    return((m_pBitmap != NULL) && (m_bPortrait == bPortrait));
  }
  // constructor accessed from static void PrintTestPattern()
  CPrintOutTestPattern(wxWindow *parent, bool bPreview = false) :
    CPrintOut(bPreview, wxT("OSIRIS Print Test Pattern")),
    m_pParent(parent),
    m_pBitmap(NULL),
    m_bPortrait(true)
  {}
  virtual ~CPrintOutTestPattern()
  {
    _CleanupBitmap();
  }
  wxBitmap *_SetupBitmap(bool bPortrait);
  wxWindow *m_pParent;
  wxBitmap *m_pBitmap;
  bool m_bPortrait;
};

#endif
