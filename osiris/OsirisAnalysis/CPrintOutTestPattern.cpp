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
*  FileName: CPrintOutTestPattern.cpp
*  Author:   Douglas Hoffman
*
*  Implementation for wxPrintout for printing an OSIRIS test pattern
*  used for adjusting colors on a printout
*
*/
#include <wx/printdlg.h>
#include <wx/mstream.h>
#include "nwx/nwxStaticBitmap.h"
#include "nwx/nwxRound.h"
#include "CPrintOutTestPattern.h"
#include "PrintTestPattern.h" // png image converted to const unsigned char[]
#include "mainApp.h"

class TempPageMargins
{
public:
  TempPageMargins(
    wxPageSetupDialogData *pSetup, 
    const wxPoint &ptTopLeft = m_pt0,
    const wxPoint &ptBottomRight = m_pt0) :
    m_pSetup(pSetup)
  {
    m_ptTL = m_pSetup->GetMarginTopLeft();
    m_ptBR = m_pSetup->GetMarginBottomRight();
    m_pSetup->SetMarginTopLeft(ptTopLeft);
    m_pSetup->SetMarginBottomRight(ptBottomRight);
  }
  virtual ~TempPageMargins()
  {
    m_pSetup->SetMarginTopLeft(m_ptTL);
    m_pSetup->SetMarginBottomRight(m_ptBR);
  }
private:
  static const wxPoint m_pt0;
  wxPageSetupDialogData *m_pSetup;
  wxPoint m_ptTL;
  wxPoint m_ptBR;
};

const wxPoint TempPageMargins::m_pt0(wxPoint(0, 0));

bool CPrintOutTestPattern::OnPrintPage(int)
{
  wxPageSetupDialogData *pSetup = GetPageSetupData();
  TempPageMargins x(pSetup);
  wxDC *pdc = GetDC();
  _setupPageBitmap(pdc);
  wxRect r = GetLogicalPageMarginsRect(*GetPageSetupData());
  int nLogicalWidth = r.GetWidth();
  int nLogicalHeight = r.GetHeight();
  _SetupBitmap(nLogicalWidth <= nLogicalHeight);
  if (m_pBitmap != NULL)
  {
    double dPageAspect = double(nLogicalWidth) / double(nLogicalHeight);

    int nBitmapWidth = m_pBitmap->GetWidth();
    int nBitmapHeight = m_pBitmap->GetHeight();
    double dBitmapAspect = double(nBitmapWidth) / double(nBitmapHeight);
    double dRatio = dBitmapAspect / dPageAspect;
    if (dRatio < 1.03 && dRatio > 0.97)
    {
      // close enough to 1.0
    }
    else if (dRatio < 1.0)
    {
      // bitmap is narrower than page, increase horizontal logical coordinates
      nBitmapWidth = nwxRound::Round(double(nBitmapWidth) / dRatio);
    }
    else
    {
      // bitmap is shorter than page, increase vertical logical coordinates
      nBitmapHeight = nwxRound::Round(double(nBitmapHeight) * dRatio);
    }
    FitThisSizeToPageMargins(wxSize(nBitmapWidth, nBitmapHeight), *GetPageSetupData());
    r = GetLogicalPageMarginsRect(*GetPageSetupData());
    pdc->DrawBitmap(*m_pBitmap, r.GetLeftTop());
  }
  return(m_pBitmap != NULL);
}

wxBitmap *CPrintOutTestPattern::_SetupBitmap(bool bPortrait)
{
  if ((m_pBitmap == NULL) || (bPortrait != m_bPortrait))
  {
    _CleanupBitmap();
    wxMemoryInputStream str((const char *)PrintTestPattern, sizeof(PrintTestPattern));
    wxImage img(str, wxBITMAP_TYPE_PNG);
    if (img.IsOk() && !bPortrait)
    {
      img = img.Rotate90();
    }
    if (img.IsOk())
    {
      m_pBitmap = new wxBitmap(img);
      m_bPortrait = bPortrait;
      if (!m_pBitmap->IsOk())
      {
        _CleanupBitmap();
      }
    }
  }
  return m_pBitmap;
}