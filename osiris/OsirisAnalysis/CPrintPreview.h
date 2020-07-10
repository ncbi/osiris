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
*  FileName: CPrintPreview.h
*  Author:   Douglas Hoffman
*
*  CPrintPreview implements wxPrintPreview
*  CPrintPreviewFrame implement wxPrintPreviewFrame
*/
#ifndef __C_PRINT_PREVIEW_H__
#define __C_PRINT_PREVIEW_H__

#include <wx/print.h>
#include "nwx/PersistentSize.h"

class CDialogPrintSettings;
class CFrameAnalysis;
class CFramePlot;
class CPrintPreviewFrame;
class CPrintPreview;
class wxArrayString;
class wxTextCtrl;
class wxStaticText;
class wxButton;
class wxChoice;

class CPreviewControlBar : public wxPreviewControlBar
{
  // display and control print preview
  // this handles page button events for pages
  // and updated the page number but other events must be handled by the 
  // containing CPrintPreviewFrame
  //
  //  events are
  //   TYPE         ID                action
  //   ------------------------------------------------------------------------
  //   wxButton     IDprintPrint      print to printer
  //   wxTextCtrl   IDprintPageText   change in page number
  //   wxChoice     IDprintZoom       change in zoom
  //   wxButton     IDprintSettings   print settings in CDialogPrintSettings
public:
  CPreviewControlBar(CPrintPreviewFrame *parent, CPrintPreview *preview);
  virtual ~CPreviewControlBar();
  virtual void CreateButtons();
  void SetPageCount(int n);
  void SetPage(int n);
  void SetZoom(int n);
  int GetPageCount()
  {
    return m_nPageCount;
  }
  int GetPage();
  int GetZoom();

  bool IncrementPage()
  {
    int n = GetPage();
    bool bRtn = false;
    if (n < GetPageCount())
    {
      SetPage(n + 1);
      bRtn = true;
    }
    return bRtn;
  }
  bool DecrementPage()
  {
    int n = GetPage();
    bool bRtn = false;
    if (n > 1)
    {
      SetPage(n - 1);
      bRtn = true;
    }
    return bRtn;
  }
private:
  wxButton *_CreateButton(int nID, const wxString &sLabel, const wxString &sToolTip, const wxFont &f);
  void _UpdatePageButtons(int n = -1);
  void _OnButtonFirst(wxCommandEvent &);
  void _OnButtonPrev(wxCommandEvent &);
  void _OnButtonNext(wxCommandEvent &);
  void _OnButtonLast(wxCommandEvent &);
  void _OnTextCtrlPageChange(wxCommandEvent &);

  static void _SetupZoomChoices();
  static int _GetZoomIndex(int nPct);

  static wxArrayString g_asZoomChoices;
  static const int g_anZoomChoices[5];
  CPrintPreview *m_pPrintPreview;
  wxTextCtrl *m_pTextPage;
  wxStaticText *m_pTextPageCount;
  wxChoice *m_pListZoom;
  wxButton *m_pButtonFirst;
  wxButton *m_pButtonPrev;
  wxButton *m_pButtonNext;
  wxButton *m_pButtonLast;
  int m_nPageCount;
  DECLARE_ABSTRACT_CLASS(CPreviewControlBar)
  DECLARE_EVENT_TABLE()
};

class CPrintPreviewFrame : public wxPreviewFrame
{
public:
  CPrintPreviewFrame(CPrintPreview *pPreview, wxFrame *pParent,
    const wxString &sTitle, bool bPageButtons = false);
  virtual ~CPrintPreviewFrame() {}
  virtual void CreateControlBar();
  void UpdateSettings();
private:
  CPreviewControlBar *_GetControlBar()
  {
    return wxDynamicCast(m_controlBar, CPreviewControlBar);
  }
  void _OnSettings(wxCommandEvent &);
  void _OnColors(wxCommandEvent &);
  void _OnZoom(wxCommandEvent &);
  void _OnPageChange(wxCommandEvent &);
  void _OnPrint(wxCommandEvent &);
  CPrintPreview *m_pPreview;
  wxDialog *m_pDialogSettings;
  CFrameAnalysis *m_pFrameAnalysis;
  CFramePlot *m_pFramePlot;
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
    m_sPrintType(sPrintType)
  {
    _SetDefaultZoom();
  }
  CPrintPreview(const wxString &sPrintType, wxPrintout *printout, wxPrintout *printoutForPrinting, wxPrintData *data) :
    wxPrintPreview(printout, printoutForPrinting, data),
    m_sPrintType(sPrintType)
  {
    _SetDefaultZoom();
  }
  virtual ~CPrintPreview() {}
  virtual bool Print(bool interactive);
  virtual void SetZoom(int n);
  virtual int GetMaxPage();

private:
  const wxString m_sPrintType;
  void _SetDefaultZoom();
};




#endif