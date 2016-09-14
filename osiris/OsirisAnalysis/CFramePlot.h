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
*  FileName: CFramePlot.h
*  Author:   Douglas Hoffman
*
*  NOTE: This code has way too many hacked patches
*     to combat unwanted behavior and should be cleaned up
*     mainly by use of CBatchPlot(this) to delay any redrawing
*/
#ifndef __C_FRAME_PLOT_H__
#define __C_FRAME_PLOT_H__

#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"
#include <wx/sizer.h>
#include <wx/panel.h>
#include "nwx/nsstd.h"
#include "nwx/PersistentSize.h"
#include "CMDIFrame.h"
#include "CPlotData.h"
#include "wxIDS.h"
#include "CPanelPlot.h"
#include "CHistoryTime.h"
#include "CMenuHistory.h"
#include "CDialogPlotMessage.h"
#include "Platform.h"
//typedef vector<wxPlotData *> vectorMinRFU;

#define FP_SCROLL_EVENT 0

#if FP_SCROLL_EVENT
DECLARE_EVENT_TYPE(wxEVT_SCROLL_PLOT,-1)
#endif
DECLARE_EVENT_TYPE(wxEVT_SIZE_DELAY_PLOT,-1)

class CKitColors;
class CFramePlot;
class mainFrame;
class CFramePlotMenu;
class CDialogExportPlot;
class wxScrolledWindow;
class COARpeakAny;
class COARsample;

typedef enum
{
  FP_NO_STATE,
  FP_FIXED,
  FP_VARIABLE_1PLOT,
  FP_VARIABLE_MANY_PLOTS
} CFramePlotState;

class CFramePlot : public CMDIFrame
{
public:
  CFramePlot(
    mainFrame *parent, 
    const wxSize &size, 
    CPlotData *pData, 
    COARfile *pFile,
    CKitColors *pColors,
    bool bSingle = false,
    unsigned int nChannel = 0,
    bool bUseExternalTimer = false
    );
  virtual ~CFramePlot();
  virtual int GetType();
  virtual bool MenuEvent(wxCommandEvent &e);
  virtual wxMenu *GetMenu();
  virtual wxMenu *GetGraphMenu();
  virtual wxString GetFileName();
  virtual void OnTimer(wxTimerEvent &e);
  void SetOARfile(COARfile *pFile);
  void UpdateOARfile(const wxString &sSampleFileName);
  void EditPeak(COARpeakAny *,COARsample *);

  virtual bool Show(bool show = true);
//  void ShowScrollbars(bool bShow);
//  void ShowToolbars(bool bShow);

  void ReInitialize(const wxString &sLocus, bool bSingle);
  void AddPlot(CPanelPlot *pPreceed, bool bUpdate = true);
  void RemovePlot(CPanelPlot *pRemove, bool bRefresh = true);
  void RemoveAllPlotsExcept(CPanelPlot *pKeep);
  void SetMultiple(CPanelPlot *p, bool bCurrentView = false);
  void SyncThis(CPanelPlot *p);
#if DELAY_PLOT_AREA_SYNC
  bool SyncTo(CPanelPlot *p)
  {
    m_nSyncThisTimer = 1;
    m_pPlotSyncThisTo = p;
    return _SyncTo(p);
  }
#else
  bool SyncTo(CPanelPlot *p)
  {
    return _SyncTo(p);
  }
#endif
  void SyncState(CPanelPlot *p, int nID);
  void SetUseExternalTimer(bool b = true);
  bool SetScrollbarMenuLabel(bool bShow);
  bool SetWindowScrollbarMenuLabel();
  void RebuildAll();
  bool InSyncUpdateTo(CPanelPlot *p)
  {
    return m_pPlotSyncTo == p;
  }
  bool InSyncUpdate() const
  {
    return (m_nInSync > 0);
  }
  size_t GetPlotCount() const
  {
    return m_setPlots.size();
  }
  wxScrolledWindow *GetPanel()
  {
    return m_pPanel;
  }
  void ZoomOut(bool bAll = false,unsigned int nDelay = 0);
  void ZoomToLocus(const wxString &sLocus,unsigned int nDelay = 0);
  void ZoomAll(const wxRect2DDouble &r,unsigned int nDelay = 0);
  wxRect2DDouble GetZoomOutRect(bool bAll = false);
  void BeginBatch()
  {
    m_nBatchCount++;
    if(m_pPanel != NULL)
    {
      m_pPanel->Show(false);
    }
  }
  void EndBatch()
  {
    m_nBatchCount--;
    if(!m_nBatchCount)
    {
      _CheckRebuildMenu();
      if(!m_pPanel->IsShown())
      {
        m_pPanel->Show(true);
      }
      Layout();
      _SendSizeAction();
      if(m_nDelayViewState)
      {
        _UpdateViewState();
      }
    }
  }
  bool IsInBatch()
  {
    return (m_nBatchCount > 0);
  }
  bool NotInBatch()
  {
    return !IsInBatch();
  }
  wxBitmap *CreateBitmap(
    int nWidth, int nHeight, int nDPI, 
    const wxString &sTitle = wxEmptyString);
  const wxDateTime *GetSelectedTime()
  {
    const wxDateTime *pRtn = 
      (m_pMenuHistory == NULL)
      ? NULL
      : m_pMenuHistory->GetSelected();
    return pRtn;
  }
  virtual void UpdateHistory();
  virtual void UpdateLadderLabels();
  virtual void CheckFileModification();
  void SyncToolbars(CPanelPlot *p);
  void SetFocusPlot(int n);
#if FP_SCROLL_EVENT
  void SendScrollPlotEvent(int nPlot = 0, int nDelay = 2)
  {
    wxCommandEvent ee(wxEVT_SCROLL_PLOT,GetId());
    int n = _MakeScrollEventInt(nPlot, nDelay);
    ee.SetEventObject(this);
    ee.SetInt(n);
    GetEventHandler()->AddPendingEvent(ee);
  }
#endif
#if HAS_STATUS_BAR
  virtual void UpdateStatusBar();
#endif
private:

  // following two functions are for setting and getting info from the value of
  // wxCommand::GetInt()/SetInt(); for the scroll event wxEVT_SCROLL_PLOT
#if FP_SCROLL_EVENT
  static int _MakeScrollEventInt(int nPlot, int nDelay)
  {
    return ((nDelay << 8) | (nPlot & 255));
  }
  static void _ExtractScrollEventInt(int n,int *pnChannel, int *pnDelay)
  {
    *pnDelay = n >> 8;
    *pnChannel = n & 255;
  }
#endif
  static bool _CheckIsXML(const wxString &s);
  void _ToggleFixed()
  {
    _SetFixed(!m_bFixed);
  }
  void _FindOARfile(
    int nType = CDialogPlotMessageFind::MSG_TYPE_HISTORY);
  void _RebuildLabels(bool bForce = true);
  void _SetupHistoryMenu();
  void _SetupTitle();
  void _RebuildMenu();
  void _ScrollPlot();

  void _CheckRebuildMenu()
  {
    if(m_bUpdateMenu && !IsInBatch())
    {
      _RebuildMenu();
    }
  }
  void _UpdateMenu()
  {
    m_bUpdateMenu = true;
    _CheckRebuildMenu();
  }
  void _UpdateToolbarMenuLabel()
  {
    SetToolbarMenuLabel(!m_bShowToolbar,true);
  }
  void _UpdateScrollbarMenuLabel()
  {
    SetScrollbarMenuLabel(!m_bShowScrollbars);
  }
  void _SendSizeAction(int n = 0)
  {
    if(!IsInBatch())
    {
      wxCommandEvent ee(wxEVT_SIZE_DELAY_PLOT,GetId());
      ee.SetEventObject(this);
      ee.SetInt(n);
      GetEventHandler()->AddPendingEvent(ee);
    }
  }

  bool _SyncTo(CPanelPlot *p);
  void _SyncSize();
  void _SetFixed(bool bFixed);
  void _UpdateVirtualWidth();
  void _UpdateMinHeight();
  void _UpdateViewState(bool bForce = false);
  void _UpdateMinHeight(int nHeight)
  {
    m_nMinHeight = nHeight;
    _UpdateMinHeight();
  }
  bool _ShowSash()
  {
    bool bRtn = (m_nState == FP_VARIABLE_MANY_PLOTS);
    return bRtn;
  }
  void _SetPlotScroll(int nPlot, int nOffset)
  {
    m_nScrollOnTimer = nPlot; // plot number to scroll to top
    m_nScrollOnTimerOffset = nOffset; // offset of scrolling in pixels
  }
  void _ResetPlotScroll()
  {
    _SetPlotScroll(-1,0);
  }
  CFramePlotMenu *_GetMenuPopup();
  CMenuHistory *_GetMenuHistoryPopup();
  void _CleanupMenuPopup();
  void _CleanupMenuHistoryPopup()
  {
    if(m_pMenuHistoryPopup != NULL)
    {
      delete m_pMenuHistoryPopup;
      m_pMenuHistoryPopup = NULL;
    }
  }

  CDialogExportPlot *_GetExportDialog();
  bool _CheckAnalysisFile(COARfile *pFile);
  void _CleanupExportDialog();
  void ShowToolbars(bool bShow = true);
  void UpdatePlotNumbers();
  CPanelPlot *GetPanelPlot(bool bDraw = true, unsigned int nr = 0);
  wxScrolledWindow *m_pPanel;
  wxBoxSizer *m_pSizer;
  CDialogExportPlot *m_pDialogPlot;
  set<CPanelPlot *> m_setPlots;
  set<CPanelPlot *> m_setPlotsHidden;
  vector<CPanelPlot *> m_vpPlotsByMenuNumber;
  CPlotData *m_pData;
  COARfile *m_pOARfile;
  CKitColors *m_pColors;
  CFramePlotMenu *m_pMenu;
  CMenuHistory *m_pMenuHistory;
  CFramePlotMenu *m_pMenuPopup;
  CMenuHistory *m_pMenuHistoryPopup;
  CHistoryTime m_TimeLastRebuild;
  CFramePlotState m_nState;
  CPanelPlot *m_pPlotSyncTo;
#if DELAY_PLOT_AREA_SYNC
  CPanelPlot *m_pPlotSyncThisTo;
#endif

  int m_nInSync;
  int m_nMinHeight;
  int m_nMenuUp;
  int m_nBatchCount;
  int m_nScrollOnTimer; // plot number to scroll to top
  int m_nScrollOnTimerOffset; // offset of scrolling in pixels
  int m_nDelayViewState;
#if DELAY_PLOT_AREA_SYNC
  int m_nSyncThisTimer;
#endif
  bool m_bUpdateMenu;
  bool m_bUseExternalTimer;
  bool m_bShowToolbar;
  bool m_bShowScrollbars;
  bool m_bFixed;
public:
  void OnHistoryUpdate(wxCommandEvent &e);
  void OnExport(wxCommandEvent &e);
  void OnClose(wxCloseEvent &e);
  void OnContextMenu(wxContextMenuEvent &e);
  void OnHistoryButton(wxCommandEvent &e);
  void OnTableButton(wxCommandEvent &e);
  void OnSashDragged(wxSashEvent &e);
  void OnSize(wxSizeEvent &e);
  void OnSizeAction(wxCommandEvent &e);
#if FP_SCROLL_EVENT
  void OnScrollPlot(wxCommandEvent &e);
#endif
  DECLARE_PERSISTENT_SIZE
  DECLARE_ABSTRACT_CLASS(CFramePlot)
  DECLARE_EVENT_TABLE()
};



#endif
