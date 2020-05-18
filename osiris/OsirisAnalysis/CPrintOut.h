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
*  FileName: CPrintOut.cpp
*  Author:   Douglas Hoffman
*
*  Implementation for wxPrintout for printing an OSIRIS plot
*  This is inherited from CPrintOutPlot for printing from a plot window
*  and CPrintOutAnalysis for printing an entire analysis from an Analysis table
*
*/
#ifndef __C_PRINTOUT_H__
#define __C_PRINTOUT_H__

#include <wx/print.h>
#include "nwx/PersistentSize.h"

class CPrintPreviewFrame : public wxPreviewFrame
{
public:
  CPrintPreviewFrame(wxPrintPreview *pPreview, wxFrame *pParent,
    const wxString &sTitle, bool bPageButtons = false);
  virtual ~CPrintPreviewFrame() {}
  virtual void CreateControlBar();
private:
  bool m_bPageButtons;
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_ABSTRACT_CLASS(CPrintPreviewFramePlot)
  DECLARE_EVENT_TABLE()
};


class CPrintPreview : public wxPrintPreview
{
public:
  CPrintPreview(const wxString &sPrintType, wxPrintout *printout, wxPrintout *printoutForPrinting = NULL, wxPrintDialogData *data = NULL) :
    wxPrintPreview(printout, printoutForPrinting, data),
    m_sPrintType(sPrintType),
    m_nZoom(0)
  {
    _SetDefaultZoom();
  }
  CPrintPreview(const wxString &sPrintType, wxPrintout *printout, wxPrintout *printoutForPrinting, wxPrintData *data) :
    wxPrintPreview(printout, printoutForPrinting, data),
    m_sPrintType(sPrintType),
    m_nZoom(0)
  {
    _SetDefaultZoom();
  }
  virtual ~CPrintPreview() {}
  virtual bool Print(bool interactive);
  virtual void SetZoom(int n);
  int GetZoom()
  {
    return m_nZoom;
  }

private:
  const wxString m_sPrintType;
  void _SetDefaultZoom();
  int m_nZoom;
};


class wxDC;

class CPrintOut : public wxPrintout
{
public:
  CPrintOut(bool bPreview = false) :
    wxPrintout(wxT("OSIRIS Plot")),
#ifdef TMP_DEBUG
    m_nSetupPageCount(0),
#endif
    m_bPreview(bPreview)
  {}

  virtual ~CPrintOut();
  virtual wxFrame *GetParent() = 0;
  bool IsPrintPreview()
  {
    // should check wxPrinout::IsPreview
    return m_bPreview;
  }
  static void StaticCleanup()
  {
    if (g_pageSetupData != NULL)
    {
      delete g_pageSetupData;
      g_pageSetupData = NULL;
    }
    if (g_printData != NULL)
    {
      delete g_printData;
      g_printData = NULL;
    }
  }
  static void DoPageSetup(wxFrame *pParent);
#ifdef __WXMAC__
  static void DoPageMargins(wxFrame *parent);
#endif
protected:
  static void _DoPrint(CPrintOut *pPrintout, const wxString &sPingType);
  static void _DoPrintPreview(
    CPrintOut *pPreview, CPrintOut *pPrint, 
    const wxString &sPingPreview, const wxString &sPingPrint,
    bool bPageButtons = false);
  static wxPageSetupDialogData *GetPageSetupData();
  static void UpdatePageSetup();
  static wxPrintData *GetPrintData();
  static void UpdatePageSetupData(
    wxPrintData *pPrintData = NULL,
    wxPageSetupDialogData *pSetupData = NULL);

  static const int MAX_PPI;
  static wxPrintData *g_printData;
  static wxPageSetupDialogData* g_pageSetupData;


  void _setupPageBitmap(wxDC *pdc);
private:
  //  _resInput input parametes used to compute DPI and size of bitmap
  //  _resOutput size and DPI of bitmap to be created
  //  purpose: when creating a multipage printout, this is needed 
  //    for each page and if the input doesn't change, the output
  //    does not need to be recomputed
  class _resInput
  {
  public:
    _resInput() :
      m_logicalPage(0, 0, 0, 0), m_ppi(-1, -1)
    {}
    _resInput(const wxSize &ppi, const wxRect &page) :
      m_logicalPage(page), m_ppi(ppi)
    {}
    virtual ~_resInput() {}
    bool operator ==(const _resInput &x)
    {
      return (m_ppi == x.m_ppi) && (m_logicalPage == x.m_logicalPage);
    }
    bool operator !=(const _resInput &x)
    {
      return !(*this == x);
    }
    _resInput &operator = (_resInput &x)
    {
      m_ppi = x.m_ppi;
      m_logicalPage = x.m_logicalPage;
      return *this;
    }
    wxRect m_logicalPage;
    wxSize m_ppi;
  };

  _resInput m_resInput;

protected:
  class _resOutput
  {
  public:
    _resOutput() : m_nWidth(-1), m_nHeight(-1), m_DPI(-1), m_bFit(false) {}
    virtual ~_resOutput() {}
    int m_nWidth;
    int m_nHeight;
    int m_DPI;
    bool m_bFit;
  };

  _resOutput m_resOutput;
#ifdef TMP_DEBUG
  int m_nSetupPageCount;
#endif
  bool m_bPreview;
};

#endif
