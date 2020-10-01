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
*  FileName: CPanelPlotPreview.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_PLOT_PREVIEW_H__
#define __C_PANEL_PLOT_PREVIEW_H__

#include <wx/panel.h>
#include <wx/string.h>
#include <wx/timer.h>
#include <wx/event.h>
#include "nwx/stdb.h"
#include <list>
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "LABEL_TYPES.h"
class CPlotData;
class CPanelPlot;
class wxBoxSizer;
class wxStaticText;
class CKitColors;
class COARfile;
class CFrameAnalysis;
class wxMenu;
class CMenuPlot;
class CMenuHistory;

class CPanelPlotPreview : public wxPanel
{
public:
  CPanelPlotPreview(
    CFrameAnalysis *pFrame,
    wxWindow *parent, 
    COARfile *pFile,
    CKitColors *pColors,
    CMenuHistory *pMenuHistory = NULL,
    size_t nMaxCount = 6
    );
  virtual ~CPanelPlotPreview();

  CPanelPlot *SetFileLocus(
    const wxString &sFileName,
    const wxString &sLocus,
    bool bReload = false);

  CPanelPlot *SetFileChannels(
    const wxString &sFileName,
    const vector<unsigned int> &nChannels,
    bool bReload = false);
  CPanelPlot *SetFileAllChannels(
    const wxString &sFileName,
    bool bReload = false
    );

  CPanelPlot *SetFileILS(
    const wxString &sFileName,
    bool bReload = false);

  void SetFileMissing();
  bool MenuEvent(wxCommandEvent &e);
  void SetMaxCount(size_t nMaxCount);
  //void OnTimer(wxTimerEvent &e); // EXT TIMER
  CMenuPlot *GetMenuPlot();
  wxMenu *GetMenu();
  void SetPeakLabelType(int n);
  void UpdateLadderLabels();
  bool IsToolbarShown();
  bool AreScrollbarsShown();
  void ShowToolbar(bool bShow);
  void ShowScrollbars(bool bShow);
  void ToggleToolbar()
  {
    ShowToolbar(!IsToolbarShown());
  }
  void ToggleScrollbars()
  {
    ShowScrollbars(!AreScrollbarsShown());
  }

private:
  void _Cleanup(size_t n = 0);
  bool _RemoveFile(const wxString &sFileName);
  CPanelPlot *_FindCurrent();
  CPanelPlot *_Setup(const wxString &sFileName, bool bReload);
  CPanelPlot *_Create(const wxString &sFileName);
  void _HideCurrent();
  wxStaticText *_SetupMessage();

  list<CPanelPlot *> m_listPlots;
  size_t m_nMaxCount;
  wxStaticText *m_pTextMessage;
  wxBoxSizer *m_pSizer;
  CFrameAnalysis *m_pFrameAnalysis;
  COARfile *m_pOARfile;
  CKitColors *m_pColors;
  CMenuHistory *m_pMenuHistory;
  //bool m_bExternalTimer;// EXT TIMER
  bool m_bLogged; // set to true if there is a problem with a timer event
};

#endif
