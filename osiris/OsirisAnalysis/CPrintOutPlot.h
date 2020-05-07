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
*  FileName: CPrintOutPlot.cpp
*  Author:   Douglas Hoffman
*
*  Implementation for wxPrintout for printing an OSIRIS plot
*
*/
#ifndef __C_PRINTOUT_PLOT__
#define __C_PRINTOUT_PLOT__

#include <wx/print.h>
#include "nwx/PersistentSize.h"

class CFramePlot;
class wxDC;

class CPrintPlotPreview : public wxPreviewFrame
{
public:
  CPrintPlotPreview(wxPrintPreview *pPreview, wxWindow *pParent, 
    const wxString &sTitle, bool bPageButtons = false);
  virtual ~CPrintPlotPreview() {}
  virtual void CreateControlBar();
private:
  bool m_bPageButtons;
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_ABSTRACT_CLASS(CPrintPlotPreview)
  DECLARE_EVENT_TABLE()

};

class CPrintOutPlot : public wxPrintout
{
public:
  CPrintOutPlot(CFramePlot *pFrame, bool bPreview = false) :
    wxPrintout(wxT("OSIRIS Plot")),
    m_pFramePlot(pFrame),
    MAX_PPI(bPreview ? -1 : 600)
  {
  }
  virtual ~CPrintOutPlot();
  virtual bool OnPrintPage(int page);
  virtual bool HasPage(int page);
  virtual void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);

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
  static void DoPrintPreview(CFramePlot *pPlot);
  static void DoPageSetup(CFramePlot *pPlot);
  static void DoPrint(CFramePlot *pPlot);
#ifdef __WXMAC__
  static void DoPageMargins(CFramePlot *pPlot);
#endif
protected:
  static wxPageSetupDialogData *GetPageSetupData();
  static void UpdatePageSetup();
  static wxPrintData *GetPrintData();
  static void UpdatePageSetupData(wxPrintData *pPrintData = NULL, wxPageSetupDialogData *pSetupData = NULL);

  int MAX_PPI;
  static wxPrintData *g_printData;
  static wxPageSetupDialogData* g_pageSetupData;
private:
  CFramePlot *m_pFramePlot;

};

#endif
