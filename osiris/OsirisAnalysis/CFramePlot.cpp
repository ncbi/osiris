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
*
*  FileName: CFramePlot.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include "mainApp.h"
#include "CFramePlot.h"
#include "CFrameAnalysis.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "nwx/nwxHoldScrollPos.h"
#include "OsirisFileTypes.h"
#include <wx/wupdlock.h>
#include <wx/scrolwin.h>
#include <wx/file.h>
#include <wx/settings.h>
#include <wx/filename.h>
#include <wx/dcmemory.h>
#include "nwx/CPointerHold.h"
#include "nwx/CIncrementer.h"
#include "nwx/nwxString.h"
#include "nwx/nwxBatch.h"
#include "CDialogExportPlot.h"
#include "COsirisIcon.h"
#include "CKitColors.h"
#include "CPlotData.h"
#include "COARfile.h"
#include "CMenuBarGraph.h"
#include "CMenuFileGraph.h"
#include "CMenuEdit.h"
#include "CParmOsiris.h"
#include "CPanelHistoryMenu.h"

#if FP_SCROLL_EVENT
DEFINE_EVENT_TYPE(wxEVT_SCROLL_PLOT)
#endif
DEFINE_EVENT_TYPE(wxEVT_SIZE_DELAY_PLOT)

#define SCROLL_UNITS 4

// round DOWN to the nearest multiple of 4, if n >= 0
#define PIXEL_TO_SCROLL_UNITS(n)  ((n) >> 2)
#define SCROLL_UNITS_TO_PIXELS(n) ((n) << 2)

//**************************************************** CFramePlotMenu
class CFramePlotMenu : public wxMenu
{
public:
  CFramePlotMenu();
  virtual ~CFramePlotMenu();
  CFramePlotMenu *Clone();
  void RemoveAllPlots();
  void InsertPlot(wxMenuItem *pItem, int nOffset);
  void UpdateLabels();
  void SetToolbarLabel(bool bShow = true);
  CMenuPlot *FindMenuPlot(int nID);
  int PlotCount()
  {
    int n = (int)GetMenuItemCount() - (int)m_nBaseSize;
    if(n < 0) { n = 0; }
    return n;
  }
  void AddPlot(wxMenuItem *pItem)
  {
    InsertPlot(pItem,PlotCount());
  }
  void BeginBatch()
  {
    m_nBatchCount++;
  }
  void EndBatch()
  {
    m_nBatchCount--;
    _CheckUpdate();
  }
  void CopySettings(CFramePlotMenu &menuFrom, bool bDeep = false);
  void CopyPlotSettings(CFramePlotMenu &menuFrom);
  void SetHistorySubMenu(wxMenu *p);
  CMenuHistory *GetHistorySubMenu()
  {
    CMenuHistory *pRtn = 
      (m_pMenuItemHistory == NULL)
      ? NULL
      : wxDynamicCast(m_pMenuItemHistory->GetSubMenu(),CMenuHistory);
    return pRtn;
  }
private:
  void _CheckUpdate()
  {
    if(m_bUpdate && !m_nBatchCount)
    {
      UpdateLabels();
    }
  }
  int m_nBatchCount;
  size_t m_nBaseSize;
  bool m_bUpdate;
  bool m_bClone;
  wxMenuItem *m_pMenuItemHistory;
};

typedef TnwxBatch<CFramePlotMenu> CBatchPlotMenu;


CFramePlotMenu::CFramePlotMenu()
  : wxMenu(0L),
    m_nBatchCount(0),
    m_bUpdate(false),
    m_bClone(false),
    m_pMenuItemHistory(NULL)
{
  AppendSeparator();
  Append(IDmenuParameters,"Parameters...",
    "Show parameters used to create this data");
  Append(
    IDExportGraphic,
    CMenuFileGraph::EXPORT_GRAPH,
    CMenuFileGraph::EXPORT_GRAPH_HELP);
  Append(IDmenuShowHideToolbar,CMDIFrame::HIDE_TOOLBARS);
  Append(IDmenuShowHidePlotScrollbars,"Hide Plot Scrollbars");
  AppendCheckItem(IDmenuShowHideWindowScrollbar,"Resizable Plots");
  Append(IDmenuTable,"Show T&able");
  SetHistorySubMenu(NULL);
  Append(IDMaxLadderLabels,
    CMenuPlot::LABEL_MAX_PEAK_LABELS,
    CMenuPlot::STATUS_MAX_PEAK_LABELS);

  m_nBaseSize = GetMenuItemCount();
};
CFramePlotMenu::~CFramePlotMenu()
{
  // plot menu items must be removed because
  // they are destroyed by their corresponding
  // CPanelPlot instances
  if(!m_bClone)
  {
    RemoveAllPlots();
  }
}

CFramePlotMenu *CFramePlotMenu::Clone()
{
  CFramePlotMenu *pRtn = new CFramePlotMenu();
  pRtn->m_bClone = true;
  pRtn->CopySettings(*this,true);
  return pRtn;
}
void CFramePlotMenu::CopyPlotSettings(CFramePlotMenu &menuFrom)
{
  size_t nPlotCount = menuFrom.PlotCount();
  CBatchPlotMenu xxx(this);
  if(m_bClone)
  {
    RemoveAllPlots();
    wxMenuItemList &list(menuFrom.GetMenuItems());
    wxMenuItemList::iterator itr = list.begin();
    wxMenuItem *pItem;
    wxMenuItem *pNewItem;
    CMenuPlot *pMenuPlot;
    size_t nPlotCurrent;
    for(nPlotCurrent = 0; nPlotCurrent < nPlotCount; nPlotCurrent++)
    {
      pItem = *itr;
      itr++;
      pMenuPlot = wxDynamicCast(pItem->GetSubMenu(),CMenuPlot);
      if(pMenuPlot == NULL)
      {
        wxASSERT_MSG(0,
          "CFramePlotMenu::CopySettings - CMenuPlot expected");
      }
      else
      {
        pNewItem = new wxMenuItem(
          this,
          pItem->GetId(),
          pItem->GetItemLabel(),
          pItem->GetHelp(),
          wxITEM_NORMAL,
          pMenuPlot->Clone()
          );
        InsertPlot(pNewItem,(int)nPlotCurrent);
      }
    }
  }
  else if(PlotCount() == (int)nPlotCount)
  {
    wxMenuItemList::iterator itrFrom = menuFrom.GetMenuItems().begin();
    wxMenuItemList::iterator itrTo = GetMenuItems().begin();
    CMenuPlot *pMenuPlotFrom;
    CMenuPlot *pMenuPlotTo;
    size_t i;
    for(i = 0; i < nPlotCount; i++)
    {
      pMenuPlotFrom = wxDynamicCast((*itrFrom)->GetSubMenu(),CMenuPlot);
      pMenuPlotTo = wxDynamicCast((*itrTo)->GetSubMenu(),CMenuPlot);
      if(pMenuPlotFrom == NULL || pMenuPlotTo == NULL)
      {
        wxString s;
        wxASSERT_MSG(0,"CFramePlotMenu::CopySettings() "
            "problem with CMenuPlot submenu");
      }
      else
      {
        pMenuPlotTo->CopySettings(*pMenuPlotFrom);
      }
      ++itrFrom;
      ++itrTo;
    }
  }
  else
  {
    wxASSERT_MSG(0,"CFramePlotMenu::CopyPlotSettings() "
          "plot count does not match");
  }
}

void CFramePlotMenu::CopySettings(CFramePlotMenu &menuFrom, bool bDeep)
{
  if(m_bClone || bDeep)
  {
    CopyPlotSettings(menuFrom);
  }
  if(bDeep)
  {
    // set up history menu

    CMenuHistory *pMenuHistFrom = menuFrom.GetHistorySubMenu();
    CMenuHistory *pMenuHistTo = GetHistorySubMenu();
    if(pMenuHistTo != NULL)
    {
      if(pMenuHistFrom != NULL)
      {
        pMenuHistFrom->CopyState(pMenuHistTo);
      }
      else
      {
        SetHistorySubMenu(NULL);
      }
    }
    else if(pMenuHistFrom != NULL)
    {
      if(m_bClone)
      {
        SetHistorySubMenu(pMenuHistFrom->Clone());
      }
      else
      {
        wxASSERT_MSG(0,"CFramePlotMenu::CopySettings() "
          "inconsistent history menus");
      }
    }
  }
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHideToolbar);
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHidePlotScrollbars);
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHideWindowScrollbar);
}
CMenuPlot *CFramePlotMenu::FindMenuPlot(int nID)
{
  size_t n = PlotCount();
  size_t i;
  wxMenuItemList::iterator itr = GetMenuItems().begin();
  CMenuPlot *pRtn = NULL;
  CMenuPlot *p;
  for(i = 0; i < n; i++)
  {
    p = wxDynamicCast((*itr)->GetSubMenu(),CMenuPlot);
    if((p != NULL) && p->HasID(nID))
    {
      pRtn = p;
      i = n; // loop exit
    }
    ++itr;
  }
  return pRtn;
}
void CFramePlotMenu::SetHistorySubMenu(wxMenu *p)
{
  wxString sLabel("&History");
  if(m_pMenuItemHistory != NULL)
  {
    sLabel = m_pMenuItemHistory->GetItemLabel();
    Destroy(m_pMenuItemHistory);
  }
  if(p != NULL)
  {
    m_pMenuItemHistory = AppendSubMenu(p,sLabel);
  }
  else
  {
    m_pMenuItemHistory = Append(IDmenuHistoryTop,sLabel);
  }
}
void CFramePlotMenu::RemoveAllPlots()
{
  wxMenuItem *pItem;
  while(GetMenuItemCount() > m_nBaseSize)
  {
    wxMenuItemList &lst(GetMenuItems());
    pItem = *lst.begin();
    if(m_bClone)
    {
      Destroy(pItem);
    }
    else
    {
      Remove(pItem);
    }
  }
}
void CFramePlotMenu::InsertPlot(wxMenuItem *pItem, int nOffset)
{
  if(nOffset < 0)
  {
    nOffset = 0;
  }
  else if(nOffset)
  {
    int nMax = PlotCount();
    if(nOffset > nMax) { nOffset = nMax;}
  }
  Insert((size_t)nOffset,pItem);
  UpdateLabels();
}

void CFramePlotMenu::UpdateLabels()
{
  if(m_nBatchCount)
  {
    m_bUpdate = true;
  }
  else
  {
    wxMenuItemList &lst(GetMenuItems());
    wxMenuItemList::iterator itr = lst.begin();
    int nCount = PlotCount();
    int i;
    if(!nCount) {;}
    else if(nCount == 1)
    {
      (*itr)->SetItemLabel("Plot");
    }
    else
    {
      wxString s;
      (*itr)->SetItemLabel("Top Plot");
      ++itr;
      for(i = 2; i < nCount; i++)
      {
        s.Printf("Plot %d",i);
        (*itr)->SetItemLabel(s);
        ++itr;
      }
      (*itr)->SetItemLabel("Bottom Plot");
    }
  }
}

//**************************************************** CFramePlot

typedef TnwxBatch<CFramePlot> CBatchPlot;

void CFramePlot::_SetupTitle()
{
  wxString sFileName(m_pData->GetLastFileName());
  if(sFileName.Len())
  {
    wxFileName fn(sFileName);
    sFileName = fn.GetFullName();
    if(m_pOARfile != NULL)
    {
      wxString sOAR = m_pOARfile->GetLastFileName();
      wxFileName fno(sOAR);
      sOAR = fno.GetFullName();
      if(!sOAR.IsEmpty())
      {
        sFileName.Append("; ");
        sFileName.Append(sOAR);
      }
    }
  }
  else
  {
    sFileName = "error...";
  }
  SetTitle(mainApp::FormatWindowTitle(
    sFileName,false,&m_pData->GetParameters(), GetSelectedTime()));
}

CFramePlot::~CFramePlot()
{
  _CleanupMenuHistoryPopup();
  _CleanupMenuPopup();
  delete m_pData;
////  delete m_pMenu;
  _CleanupExportDialog();
}

CFramePlot::CFramePlot(
  mainFrame *pOwner,
  const wxSize &size,
  CPlotData *pData,
  COARfile *pFile,
  CKitColors *pColors,
  bool bSingle,
  unsigned int nChannel,
  bool bUseExternalTimer
  ) :
    CMDIFrame(pOwner, IDframePlot, "",
      wxDefaultPosition,
      size
      ),
    m_pPanel(NULL),
    m_pDialogPlot(NULL),
    m_pData(pData),
    m_pOARfile(NULL),
    m_pColors(pColors),
    m_pMenu(new CFramePlotMenu),
    m_pMenuHistory(NULL),
    m_pMenuPopup(NULL),
    m_pMenuHistoryPopup(NULL),
    m_TimeLastRebuild(NULL),
    m_nState(FP_NO_STATE),
    m_pPlotSyncTo(NULL),
#if DELAY_PLOT_AREA_SYNC
    m_pPlotSyncThisTo(NULL),
#endif
    m_nInSync(0),
    m_nMinHeight(-1),
    m_nMenuUp(0),
    m_nMenuBatchCount(0),
    m_nScrollOnTimer(-1),
    m_nScrollOnTimerOffset(0),
#if DELAY_PLOT_AREA_SYNC
    m_nSyncThisTimer(0),
#endif
    m_bUpdateMenu(false),
    m_bUseExternalTimer(bUseExternalTimer),
    m_bFixed(true)
{ 
  m_pPanel = new wxScrolledWindow(this,wxID_ANY,wxDefaultPosition, wxDefaultSize,wxVSCROLL);
  {
    CParmOsirisGlobal parm;
    m_bShowToolbar = !parm->GetHideGraphicToolbar();
    m_bShowScrollbars = !parm->GetHideGraphicScrollbar();
    m_bFixed = !parm->GetPlotResizable();
    m_nMinHeight = m_bFixed ? -1 : parm->GetPlotMinHeight();
  }
  _UpdateToolbarMenuLabel();
  _UpdateScrollbarMenuLabel();
  SetWindowScrollbarMenuLabel();

  SetOARfile(pFile);
  COsirisIcon x;
  SetIcon(x);
  m_pSizer = new wxBoxSizer(wxVERTICAL);
  m_pPanel->SetSizer(m_pSizer);

  CPanelPlot *p = GetPanelPlot(false);
//  p->ResetDefaults();
  m_pSizer->Add(p,1,wxEXPAND);
  if(bSingle)
  {
    p->EnableDelete(false);
    if(nChannel)
    {
      p->ShowOneChannel(nChannel);
    }
    else
    {
      p->ShowAllChannels();
    }
    p->RebuildCurves();
    _UpdateMenu();
  }
  else
  {
    SetMultiple(p);
  }
  SetMenuBar(new CMenuBarGraph(m_pMenu));
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(m_pPanel,1,wxEXPAND);
  SetSizer(pSizerAll);
}



void CFramePlot::ReInitialize(const wxString &sLocus, bool bSingle)
{
  TnwxBatchColl<CPanelPlot> batch(m_setPlots);
  CPanelPlot *pPlot = *m_setPlots.begin();
  pPlot->ResetDefaults();
  unsigned int nChannel = 0;
  int nScroll = 0;
  if(bSingle)
  {
    RemoveAllPlotsExcept(pPlot);
    if(!sLocus.IsEmpty())
    {
      nChannel = m_pData->GetChannelFromLocus(sLocus);
    }
    if(nChannel > 0)
    {
      pPlot->ShowOneChannel(nChannel);
    }
    else
    {
      pPlot->ShowAllChannels();
    }
    pPlot->RebuildCurves(true);
    pPlot->ZoomToLocus(sLocus,0);
  }
  else
  {
    SetMultiple(pPlot);
    ZoomToLocus(sLocus,0);
    if( (m_nState == FP_VARIABLE_MANY_PLOTS) && (!sLocus.IsEmpty()) )
    {
      nScroll = m_pData->GetChannelFromLocus(sLocus) - 1;
    }
  }
  _SendSizeAction();
  SetFocusPlot(nScroll);
#if FP_SCROLL_EVENT
//  SendScrollPlotEvent(nScroll);
#endif

}
wxString CFramePlot::GetFileName()
{
  wxString s(m_pData->GetLastFileName());
  return s;
}
int CFramePlot::GetType()
{
  return FRAME_PLOT;
}
wxMenu *CFramePlot::GetMenu()
{
  return m_pMenu;
}
wxMenu *CFramePlot::GetGraphMenu()
{
  return GetMenu();
}
void CFramePlot::OnTimer(wxTimerEvent &e)
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->OnTimer(e);
  }
  if(m_nScrollOnTimer >= 0)
  {
    _ScrollPlot();
  }
#if DELAY_PLOT_AREA_SYNC
  if(m_nSyncThisTimer > 0)
  {
    m_nSyncThisTimer--;
    if((!m_nSyncThisTimer) && (m_pPlotSyncThisTo != NULL))
    {
      this->_SyncTo(m_pPlotSyncThisTo);
      m_pPlotSyncThisTo = NULL;
    }
  }
#endif
}
void CFramePlot::UpdateOARfile(const wxString &sSampleFileName)
{
  // call this when the OAR file has been edited
  if( (m_pOARfile != NULL) &&
      nwxString::FileNameStringEqual(
      m_pData->GetFilename(),sSampleFileName) &&
      (GetSelectedTime() == NULL)
      )
  {
    _RebuildLabels(true);
  }
/*
  _SetupTitle();
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->RebuildLabels();
  }
*/
}
void CFramePlot::RebuildAll()
{
  CBatchPlot BATCH(this);
  m_pPanel->Show(false);
  _UpdateMenu();
  map<unsigned int,CPanelPlot *> mapNrPlot;
  set<CPanelPlot *>::iterator itr;
  map<unsigned int,CPanelPlot *>::iterator itrm;
  bool bEnableAppend = false;
  bool bEnableDelete = false;
  unsigned int nChannelCount = m_pData->GetChannelCount();
  unsigned int nMapSize;

  for(itr = m_setPlots.begin(); itr != m_setPlots.end(); ++itr)
  {
    if(mapNrPlot.size() < nChannelCount)
    {
      mapNrPlot.insert(
        map<unsigned int,CPanelPlot *>::value_type(
        (*itr)->GetPlotNumber(),*itr) );
    }
    else
    {
      m_setPlotsHidden.insert(*itr);
    }
    m_pSizer->Detach(*itr);
    (*itr)->Show(false);
  }

  // kill all hidden plots

  for(itr = m_setPlotsHidden.begin();
    itr != m_setPlotsHidden.end();
    ++itr)
  {
    (*itr)->Destroy();
  }
  m_setPlotsHidden.clear();
  m_vpPlotsByMenuNumber.clear();
  m_setPlots.clear();

  nMapSize = (unsigned int)mapNrPlot.size();
  bEnableAppend = nMapSize < nChannelCount;
  bEnableDelete = nMapSize > 1;

  for(itrm = mapNrPlot.begin();
    itrm != mapNrPlot.end();
    ++itrm)
  {
    CPanelPlot *pplot = GetPanelPlot(false,itrm->first);
    CPanelPlot *pold = itrm->second;
    pplot->CopySettings(*pold);
    pplot->EnableAppend(bEnableAppend);
    pplot->EnableDelete(bEnableDelete);
    pold->Destroy();
    m_pSizer->Add(pplot,1,wxEXPAND);
  }
  _SetupTitle();
  _UpdateViewState(true);
}
void CFramePlot::SetOARfile(COARfile *pFile)
{
  if(pFile != m_pOARfile)
  {
    m_pOARfile = pFile;
    _SetupHistoryMenu();
    m_pParent->InsertWindow(this,m_pOARfile);
    _SetupTitle(); // change oar file in title
    set<CPanelPlot *>::iterator itr;
    set<CPanelPlot *> *pSet;
    set<CPanelPlot *> *pAllPanels[] =
    {
      &m_setPlots, &m_setPlotsHidden
    };
    size_t COUNT = sizeof(pAllPanels) / sizeof(pAllPanels[0]);
    for(size_t i = 0; i < COUNT; ++i)
    {
      pSet = pAllPanels[i];
      for(itr = pSet->begin(); itr != pSet->end(); ++itr)
      {
        (*itr)->SetOARfile(m_pOARfile);
      }
    }
    _RebuildLabels(true);
    //  need to send a resize event because
    //  the combo box does not resize when
    //  peak area is added
    Refresh();
  }
  else if(pFile == NULL)
  {
    _SetupTitle();
  }
}
void CFramePlot::_SetupHistoryMenu()
{
  if(m_pOARfile != NULL)
  {
    set<CPanelPlot *>::iterator itr;
    set<CPanelPlot *> *psets[] = { &m_setPlots, &m_setPlotsHidden };

    m_pMenuHistory = new CMenuHistory(m_pOARfile,false);
    _CleanupMenuHistoryPopup();
    CMenuHistory *pMenuPopup = _GetMenuHistoryPopup();
    m_pMenu->SetHistorySubMenu(m_pMenuHistory);

    for(size_t ndx = 0; ndx < 2; ++ndx)
    {
      set<CPanelPlot *> *pSet = psets[ndx];
      for(itr = pSet->begin(); itr != pSet->end(); ++itr)
      {
        (*itr)->SetHistoryMenu(pMenuPopup);
      }
    }
  }
}
void CFramePlot::_RebuildLabels(bool bForce)
{
  const wxDateTime *pTime = m_pMenuHistory->GetSelected();
  if( bForce || (!m_TimeLastRebuild.IsEqualTo(pTime)) )
  {
    m_TimeLastRebuild = pTime;
    set<CPanelPlot *>::iterator itr;
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
//      (*itr)->RebuildLabels(true);
      (*itr)->LabelTypeChanged();
      (*itr)->UpdateHistoryButtons();
    }
    _SetupTitle();
  }
}
bool CFramePlot::SetScrollbarMenuLabel(bool bShow)
{
  wxMenu *pMenu = GetMenu();
  bool bRtn = false;
  if(pMenu != NULL)
  {
    wxMenuItem *pItem = pMenu->FindItem(IDmenuShowHidePlotScrollbars);
    if(pItem != NULL)
    {
      const char *psLabel = bShow ? "Show Plot Scrollbars" : "Hide Plot Scrollbars";
      pItem->SetItemLabel(psLabel);
      bRtn = true;
    }
  }
  return bRtn;
}

bool CFramePlot::SetWindowScrollbarMenuLabel()
{
  wxMenu *pMenu = GetMenu();
  bool bRtn = false;
  if(pMenu != NULL)
  {
    wxMenuItem *pItem = pMenu->FindItem(IDmenuShowHideWindowScrollbar);
    if(pItem != NULL)
    {
      pItem->Check(!m_bFixed);
      bRtn = true;
    }
  }
  return bRtn;
}


void CFramePlot::_CleanupExportDialog()
{
  if(m_pDialogPlot != NULL)
  {
    delete m_pDialogPlot;
    m_pDialogPlot = NULL;
  }
}
CDialogExportPlot *CFramePlot::_GetExportDialog()
{
  if(m_pDialogPlot == NULL)
  {
    const wxString &sFile(m_pData->GetLastFileName());
    m_pDialogPlot = new CDialogExportPlot(this,wxID_ANY,sFile);
  }
  return m_pDialogPlot;
}

bool CFramePlot::MenuEvent(wxCommandEvent &e)
{
  int nID = e.GetId();
  bool bRtn = true;
/*
  long nLong = e.GetExtraLong();
  if( (nLong < 6) && (m_nMenuUp > 0) )
  {
    wxCommandEvent ee(e);
    ee.SetExtraLong(++nLong);
    GetEventHandler()->AddPendingEvent(ee);
  }
  else
*/
  if(ID_IS_HISTORY(nID))
  {
    m_pMenuHistory->Select(nID);
    if(m_pMenuHistoryPopup != NULL)
    {
      m_pMenuHistoryPopup->Select(nID);
    }
    _RebuildLabels(true);
  }
  else if(nID == IDmenuHistoryTop)
  {
    if(m_pOARfile != NULL)
    {
      wxASSERT_MSG(
        0,"Received IDhistoryTop menu when OAR file is available");
    }
    else
    {
      _FindOARfile();
    }

  }
  else if(nID == IDmenuTable)
  {
    OnTableButton(e);
  }
  else if(nID == IDmenuParameters)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    if(itr != m_setPlots.end())
    {
      (*itr)->OnBtnDetails(e);
      bRtn = true;
    }
  }
  else if(nID == IDmenuShowHideToolbar)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    bool bWasShown = (*itr)->IsToolbarShown();
    for( ;itr != m_setPlots.end(); ++itr)
    {
      (*itr)->ShowToolbar(!bWasShown);
    }
    m_bShowToolbar = !bWasShown;
    _UpdateToolbarMenuLabel();
    CParmOsirisGlobal parm;
    parm->SetHideGraphicToolbar(bWasShown);
  }
  else if(nID == IDmenuShowHideWindowScrollbar)
  {
    _ToggleFixed();
  }
  else if(nID == IDmenuShowHidePlotScrollbars)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    bool bWasShown = (*itr)->AreScrollbarsShown();
    for( ;itr != m_setPlots.end(); ++itr)
    {
      (*itr)->ShowScrollbars(!bWasShown);
    }
    m_bShowScrollbars = !bWasShown;
    _UpdateScrollbarMenuLabel();
    CParmOsirisGlobal parm;
    parm->SetHideGraphicScrollbar(bWasShown);
  }
  else if(nID == IDExportGraphic)
  {
    CDialogExportPlot *pDlg = _GetExportDialog();
    int n = pDlg->ShowModal();
    if(n == wxID_OK)
    {
      _CleanupExportDialog();
    }
  }
  else
  {
    int nPlot = ID_GET_PLOT_NUMBER(nID);
    if(nPlot >= 0 && nPlot < (int)m_vpPlotsByMenuNumber.size())
    {
      CPanelPlot *pPanel = m_vpPlotsByMenuNumber.at(nPlot);
      if( (m_pMenuPopup != NULL) &&
          ((wxMenu *)m_pMenuPopup == GetLastMenuShown()) )
      {
        CMenuPlot *pMenuPlot = pPanel->GetMenuPlot();
        CMenuPlot *pMenuPlotFrom = m_pMenuPopup->FindMenuPlot(nID);
        if(pMenuPlot == NULL || pMenuPlotFrom == NULL)
        {
          wxASSERT_MSG(0,
            "CFramePlot::MenuEvent() "
            "cannot find plot menu from popup");
        }
        else
        {
          pMenuPlot->CopySettings(*pMenuPlotFrom);
        }
      }
      bRtn = pPanel->MenuEvent(e);
    }
    else
    {
      bRtn = false;
    }
  }
  return bRtn;
}

void CFramePlot::_FindOARfile(int nType)
{
  CDialogPlotMessageFind dlg(this,nType);
  int n = dlg.ShowModal();
  if(n == wxID_YES)
  {
    wxString sError;
    wxString sTypes = FILE_TYPE_REPORT;
    wxString sFile = m_pData->FindAnalysisFile();
    wxString sDir(sFile);
    bool bNotFound = sFile.IsEmpty();
    bool bDone = false;
    bool bNew = false;
    COARfile *pFile(NULL);
    if(bNotFound)
    {
      sDir = m_pData->GetLastFileName();
    }
    wxFileName fn(sDir);
    sDir = fn.GetPath();
    if(!bNotFound)
    {
      sFile = fn.GetFullName();
    }
    wxFileDialog fileDlg(
        this,"Find Analysis File",
        sDir,sFile,sTypes,
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    while(!bDone)
    {
      bNew = false;
      if(fileDlg.ShowModal() != wxID_OK)
      {
        bDone = true;
        pFile = NULL;
      }
      else
      {
        sFile = fileDlg.GetPath();
        pFile = m_pParent->FindOARfile(sFile);
        if(pFile != NULL)
        {
          if(_CheckAnalysisFile(pFile))
          {
            bDone = true;
          }
          else
          {
            pFile = NULL;
          }
        }
        else if(m_pParent->FileInProgress(sFile,true))
        {;}
        else
        {
          const char *psBadFile =
              "This is not a valid OSIRIS Analysis Report,\n"
              "possibly from an older version of OSIRIS";
          bool bShowError = false;
          if(_CheckIsXML(sFile))
          {
            pFile = new COARfile();
            if(!pFile->LoadFile(sFile))
            {
              bShowError = true;
            }
            else
            {
              bDone = _CheckAnalysisFile(pFile);
            }
            if(!bDone)
            {
              delete pFile;
              pFile = NULL;
            }
          }
          else
          {
            bShowError = true;
          }
          if(bShowError)
          {
            mainApp::ShowError(psBadFile,this);
          }
        }
      }
    }
    if(pFile != NULL)
    {
      SetOARfile(pFile);
    }
  }
}
bool CFramePlot::_CheckAnalysisFile(COARfile *pFile)
{
  wxString sError;
  if(pFile->GetKitName() != m_pData->GetKitName())
  {
    sError = "This analysis file does not use\n"
                "the same kit as the plot data.";
  }
  else if(pFile->GetSampleByName(m_pData->GetFilename()) == NULL)
  {
    sError = "This sample, ";
    sError.Append(m_pData->GetFilename());
    sError.Append(", is not found in this analysis file");
  }
  bool bRtn = sError.IsEmpty();
  if(!bRtn)
  {
    mainApp::ShowError(sError,this);
  }
  return bRtn;
}
bool CFramePlot::_CheckIsXML(const wxString &sFileName)
{
  wxFile file(sFileName);
  const size_t BUFSIZE = 12;
  char sBuf[BUFSIZE + 1];
  size_t nRead = 0;
  bool bRtn = false;
  if(file.IsOpened())
  {
    nRead = file.Read((void *)sBuf,BUFSIZE);
    file.Close();
    bRtn = (nRead == BUFSIZE) && (sBuf[0] == '<');
  }
  return bRtn;
}

void CFramePlot::OnClose(wxCloseEvent &)
{
  Destroy();
}
void CFramePlot::OnHistoryUpdate(wxCommandEvent &e)
{
  CPanelHistoryMenu *pPanel = (CPanelHistoryMenu *)e.GetEventObject();
  if(pPanel != NULL)
  {
    m_pMenuHistory->SelectTime(pPanel->GetSelectedTime());
  }
  _RebuildLabels(false);
}
void CFramePlot::UpdateHistory()
{
  //  call this when a file is saved and therefore
  //  the history list has a new entry and call _SetupTitle
  //  because the file name may have changed
  //
  //  also call this if a file was modified, the analysis window
  //  was closed and changes were discarded, therefore the
  //  OAR file was reloaded.
  if(m_pMenuHistory != NULL)
  {
    _SetupHistoryMenu(); // rebuild history menu
    _SetupTitle();
    _RebuildLabels(true);
  }
}


void CFramePlot::CheckFileModification()
{
  if(!DialogIsShowingOrNoFocus())
  {
    wxString sOARfile;
    wxString sPLTfile;
    vector<wxString> vsFiles;
    bool bReload = false;
    bool bError = false;
    bool bReloadPlot = false;
    bool bCancel = false;
    size_t nSize;
    CFrameAnalysis *pFrameAnalysis(NULL);
    vsFiles.reserve(3);
    if( (m_pOARfile != NULL) &&
      (!m_pParent->FileInProgress(m_pOARfile->GetLastFileName())) &&
      m_pOARfile->CheckFileModification() )
    {
      sOARfile = m_pOARfile->GetLastFileName();
      vsFiles.push_back(sOARfile);
      if(m_pOARfile->IsModified())
      {
        pFrameAnalysis = m_pParent->FindAnalysisFrame(m_pOARfile);
      }
    }
    if(  (!m_pParent->FileInProgress(m_pData->GetLastFileName())) &&
      m_pData->CheckFileModification() )
    {
      sPLTfile = m_pData->GetLastFileName();
      vsFiles.push_back(sPLTfile);
    }
    nSize = vsFiles.size();
    if(nSize)
    {
      if(nSize == 1)
      {
        wxString sEmpty;
        vsFiles.push_back(sEmpty);
      }
      if(PromptReload(vsFiles.at(0),vsFiles.at(1)))
      {
        if(pFrameAnalysis != NULL)
        {
          if(pFrameAnalysis->CheckSaveOnCloseFile())
          {
            // user was prompted to save changes
            // and did not cancel
            bReload = m_pOARfile->LoadFile(sOARfile);
            bError = !bReload;
          }
          else
          {
            bCancel = true;
          }
        }
        else if(!sOARfile.IsEmpty())
        {
          bReload = m_pOARfile->ReloadFile();
          bError = !bReload;
        }
        if(bError)
        {
          mainApp::ShowError(
            "A problem occurred when reading an OAR file\n",this);
          m_pParent->KillOARfile(m_pOARfile);
        }
        else if(!bCancel)
        {
          if(!sPLTfile.IsEmpty())
          {
            auto_ptr<CPlotData> pPlotDataHole(m_pData);
            m_pData = new CPlotData();
            bReloadPlot = m_pData->LoadFile(sPLTfile);
            if(bReloadPlot)
            {
              RebuildAll();
            }
            else
            {
  //            m_pParent->RemoveWindow(this);
              _Kill();
            }
          }
          if(bReload)
          {
            m_pParent->UpdateOARfile(m_pOARfile);
          }
        }
      }
    }
  }
}
void CFramePlot::UpdateLadderLabels()
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->UpdateLadderLabels();
  }
}

void CFramePlot::SyncToolbars(CPanelPlot *p)
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->SyncToolbar(p);
  }
}

void CFramePlot::AddPlot(CPanelPlot *pPreceed, bool bUpdateView)
{
  CBatchPlot BATCH(this);
  CPanelPlot *p;
  {
    CPanelPlotToolbarSaveState xxx(bUpdateView ? pPreceed : NULL);
    wxRect2DDouble rect = pPreceed->GetViewRect();
    unsigned int nr = pPreceed->GetPlotNumber() + 1;
    size_t nSize = m_setPlots.size();
    if(nSize == 1)
    {
      pPreceed->EnableDelete(true);
    }
    p = GetPanelPlot(false,nr);
    if(nr == (unsigned int)nSize)
    {
      m_pSizer->Add(p,1,wxEXPAND);
    }
    else
    {
      m_pSizer->Insert(nr,p,1,wxEXPAND);
      UpdatePlotNumbers();
    }
    pPreceed->SetViewRect(rect);
    p->CopySettings(*pPreceed);
    if(bUpdateView)
    {
      _UpdateViewState();
    }
  } // calls destructor to xxx() above
  p->SyncToolbar(pPreceed);
  _UpdateMenu();
}
void CFramePlot::_RebuildMenu()
{
  CBatchPlotMenu BATCH(m_pMenu);
  wxSizerItem *pItem;
  CPanelPlot *pCheck;
  size_t nSize = m_setPlots.size();
  m_pMenu->RemoveAllPlots();
  for(size_t i = 0; i < nSize; i++)
  {
    pItem = m_pSizer->GetItem(i);
    pCheck = (pItem == NULL)
      ? NULL : (CPanelPlot *)(pItem->GetWindow());
    if(pCheck != NULL)
    {
      wxMenuItem *pItem = pCheck->GetMenuItem(m_pMenu);
      m_pMenu->AddPlot(pItem);
    }
  }
}
void CFramePlot::UpdatePlotNumbers()
{
  wxSizerItem *pItem;
  CPanelPlot *pCheck;
  size_t nSize = m_setPlots.size();
  bool bAppend = (nSize < (size_t)(m_pData->GetChannelCount()));
  for(size_t i = 0; i < nSize; i++)
  {
    pItem = m_pSizer->GetItem(i);
    pCheck = (pItem == NULL)
      ? NULL : (CPanelPlot *)(pItem->GetWindow());
    if(pCheck != NULL)
    {
      pCheck->SetPlotNumber((unsigned int)i);
      pCheck->EnableAppend(bAppend);
    }
  }
}
void CFramePlot::RemovePlot(CPanelPlot *pRemove, bool bRefresh)
{
  CBatchPlot BATCH(this);
  size_t nSize = m_setPlots.size();
  if(nSize > 1)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.find(pRemove);
    if(itr != m_setPlots.end())
    {
      nwxHoldScrollPos xx(m_pPanel);
      m_setPlots.erase(itr);
      m_setPlotsHidden.insert(pRemove);
      m_pSizer->Detach(pRemove);
      pRemove->Show(false);
      if(nSize == 2)
      {
        // we are now down to one plot
        itr = m_setPlots.begin();
        (*itr)->EnableDelete(false);
      }
      if(bRefresh)
      {
        UpdatePlotNumbers();
        _UpdateMenu();
        _UpdateViewState();
      }
    }
  }
}

void CFramePlot::RemoveAllPlotsExcept(CPanelPlot *pRemove)
{
  CBatchPlot BATCH(this);
  set<CPanelPlot *>::iterator itr;
  CPanelPlot *pPanel;
  for(size_t nSize = m_setPlots.size();
      nSize > 1;
      nSize = m_setPlots.size())
  {
    itr = m_setPlots.begin();
    pPanel = *itr;
    if(pPanel == pRemove)
    {
      ++itr;
      pPanel = *itr;
    }
    RemovePlot(pPanel, nSize == 2);
  }
}

void CFramePlot::SetMultiple(CPanelPlot *p, bool bCurrentView)
{
  CBatchPlot BATCH(this);
  CPanelPlotToolbarSaveState tbState(p);
  wxWindowUpdateLocker x(this);
  set<CPanelPlot *>::iterator itr;
  wxRect2DDouble rect = p->GetViewRect();
  size_t nSize = m_setPlots.size();
  size_t nNeeded = m_pData->GetChannelCount() - nSize;
  CPanelPlot *pCheck;
  CIncrementer incr(m_nInSync);
  if(nSize > 1)
  {
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pCheck = *itr;
      if(pCheck != p)
      {
        pCheck->CopySettings(*p);
      }
    }
  }
  for(size_t n = 0; n < nNeeded; n++)
  {
    AddPlot(p,false);
  }
  for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
  {
    pCheck = *itr;
    pCheck->ShowOneChannel(pCheck->GetPlotNumber() + 1);
    pCheck->RebuildCurves(true);
  }
  if(bCurrentView)
  {
    ZoomAll(rect);
  }
  _UpdateViewState();
  tbState.RestoreState(true);
  SyncToolbars(p);
}

wxRect2DDouble CFramePlot::GetZoomOutRect(bool bAll)
{
  // begin with a unit rectangle,
  // then expand to include all plots where sync is on
  wxRect2DDouble rtn(0.0,0.0,1.0,1.0);
  if(!bAll)
  {
    rtn.m_x = (double) m_pData->GetBegin();
  }
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    CPanelPlot *pPlot = *itr;
    if(pPlot->SyncValue())
    {
      rtn.Union(pPlot->GetZoomOutRect(bAll));
    }
  }
  return rtn;
}
CMenuHistory *CFramePlot::_GetMenuHistoryPopup()
{
  if(m_pMenuHistory != NULL && m_pMenuHistoryPopup == NULL)
  {
    m_pMenuHistoryPopup = m_pMenuHistory->Clone();
  }
  return m_pMenuHistoryPopup;
}
CFramePlotMenu *CFramePlot::_GetMenuPopup()
{
  _CleanupMenuPopup();
  m_pMenuPopup = (m_pMenu == NULL) ? NULL : m_pMenu->Clone();
  return m_pMenuPopup;
}
CPanelPlot *CFramePlot::GetPanelPlot(bool bFirst, unsigned int nr)
{
  CPanelPlot *pRtn(NULL);
  set<CPanelPlot *>::iterator itr = m_setPlotsHidden.begin();
  if(itr == m_setPlotsHidden.end())
  {
    int nMenu = (int)m_vpPlotsByMenuNumber.size();
    CMenuHistory *pMenuHist = _GetMenuHistoryPopup();
    pRtn = new CPanelPlot(this,m_pData,m_pOARfile,pMenuHist,m_pColors,nMenu,bFirst,nr,true);
    m_vpPlotsByMenuNumber.push_back(pRtn);
    if(bFirst)
    {
      TnwxBatch<CPanelPlot> x(pRtn);
      pRtn->RebuildCurves(true);
      pRtn->ZoomOut();
    }
  }
  else
  {
    pRtn = *itr;
    m_setPlotsHidden.erase(itr);
    pRtn->SetPlotNumber(nr);
    pRtn->Show(true);
  }
  m_setPlots.insert(pRtn);
  pRtn->ShowScrollbars(m_bShowScrollbars);
  pRtn->ShowToolbar(m_bShowToolbar);
  return pRtn;
}

void CFramePlot::SyncState(CPanelPlot *p, int nID)
{
  CPanelPlot *pPlot;
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    pPlot = *itr;
    if(pPlot != p)
    {
      pPlot->SyncState(p,nID);
    }
  }
}
void CFramePlot::SyncThis(CPanelPlot *p)
{
  if( p->SyncValue() && (m_setPlots.size() > 1) )
  {
    CPanelPlot *pPlot;
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if( (pPlot != p) && pPlot->SyncValue() )
      {
        p->SetViewRect(pPlot->GetViewRect());
        break;
      }
    }
    Refresh();
  }
}
bool CFramePlot::_SyncTo(CPanelPlot *p)
{
  bool bRtn = false;
  if( p->SyncValue() && (m_setPlots.size() > 1) && !m_nInSync)
  {
    TnwxBatchColl<CPanelPlot> batch(m_setPlots);
    CPointerHold<CPanelPlot> xxx(m_pPlotSyncTo,p);
    CIncrementer incr(m_nInSync);
    wxRect2DDouble r = p->GetViewRect();
    wxRect2DDouble rect;
    CPanelPlot *pPlot;
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if( (pPlot != p) && pPlot->SyncValue() && (pPlot->GetViewRect() != r) )
      {
        pPlot->SetViewRect(r);
        bRtn = true;
      }
    }
    Refresh();
  }
  return bRtn;
}

void CFramePlot::ZoomOut(bool bAll,unsigned int nDelay )
{
  wxRect2DDouble r = GetZoomOutRect(bAll);
  ZoomAll(r,nDelay);
}

void CFramePlot::ZoomToLocus(const wxString &sLocus,unsigned int nDelay )
{
  if(!m_setPlots.size()) {} // do nothing
  else if(sLocus.IsEmpty())
  {
    // no locus specified
    ZoomOut(false,0);
  }
  else
  {
    // locus specified
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    wxRect2DDouble rect = (*itr)->GetZoomLocus(sLocus);
    ZoomAll(rect,nDelay);
  }
}

void CFramePlot::ZoomAll(const wxRect2DDouble &rect,unsigned int nDelay )
{
  CPanelPlot *pFirstSync = NULL;
  {
    TnwxBatchColl< CPanelPlot > BATCH(m_setPlots);
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
        itr != m_setPlots.end();
        ++itr)
    {
      (*itr)->SetViewRect(rect,false,0);
      if(!(*itr)->SyncValue()) 
      {
        // not sync'd
        if(nDelay)
        {
          (*itr)->SetViewRect(rect,false,nDelay);
        }
      }
      else if(pFirstSync == NULL)
      {
        pFirstSync = *itr;
      }
    }

    if(pFirstSync != NULL)
    {
      SyncTo(pFirstSync);
      if(nDelay)
      {
        pFirstSync->SetViewRect(rect,true,nDelay);
      }
    }
  }
}
void CFramePlot::_CleanupMenuPopup()
{
  if(m_pMenuPopup != NULL)
  {
    delete m_pMenuPopup;
    m_pMenuPopup = NULL;
  }
}
void CFramePlot::OnContextMenu(wxContextMenuEvent &)
{
  if((!m_nMenuUp) && (m_pMenu != NULL))
  {
    // there have been problem with this event
    // being triggered when the menu is already up.
    // This is being prevented by making sure
    // that m_nMenuUp is zero

    CIncrementer x(m_nMenuUp);
    CFramePlotMenu *pMenu = _GetMenuPopup();
    PopupMenu_(pMenu);
    _CleanupMenuPopup();
  }
}

void CFramePlot::EditPeak(COARpeakAny *pPeak, COARsample *pSample)
{
  if(m_pOARfile != NULL)
  {
    CFrameAnalysis *pFrame = m_pParent->FindAnalysisFrame(m_pOARfile);
    if(pFrame != NULL)
    {
      pFrame->EditPeak(pPeak,pSample,this);
    }
  }
}
void CFramePlot::OnHistoryButton(wxCommandEvent &e)
{
  if(m_pOARfile != NULL)
  {
    e.Skip();
  }
  else
  {
    CPanelPlot *p = *m_setPlots.begin();
    CPanelPlotToolbarSaveState xxx(p);
    _FindOARfile();
    xxx.RestoreState(true);
    SyncToolbars(p);
  }
}
void CFramePlot::OnTableButton(wxCommandEvent &e)
{
  if(m_pOARfile == NULL)
  {
    _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_TABLE);
    if(m_pOARfile != NULL)
    {
      // if this is a menu event,change to button
      // because processing is faster
      wxCommandEvent ee(e);
      ee.SetEventType(wxEVT_COMMAND_BUTTON_CLICKED);
      ee.SetEventObject(this);
      GetEventHandler()->AddPendingEvent(e);
    }
  }
  else
  {
    m_pParent->OpenFile(m_pOARfile->GetLastFileName(),wxEmptyString,m_pOARfile);
    CFrameAnalysis *pFrame = m_pParent->FindAnalysisFrame(m_pOARfile);
    if(pFrame != NULL)
    {
      pFrame->SelectSample(m_pData->GetFilename());
    }
  }
}

void CFramePlot::OnSashDragged(wxSashEvent &e)
{
  CPanelPlot *pw = (CPanelPlot *) e.GetEventObject();
  wxRect r = e.GetDragRect();
  wxPoint pt = pw->GetPosition();
  wxSize szC = m_pPanel->GetClientSize();
  wxSize szV = m_pPanel->GetVirtualSize();
  int nH = r.GetHeight();
  int nHC = szC.GetHeight();
  if(nH > nHC)
  {
    nH = nHC;
  }
  _UpdateMinHeight(nH);
  _SendSizeAction();
  pw->SetFocusPlot();
  _SetPlotScroll(pw->GetPlotNumber(),pt.y);
}
void CFramePlot::_UpdateViewState(bool bForce)
{
  //
  // this is called when the number of plots
  // has changed, including when the window
  // is created

  CFramePlotState nState = 
    m_bFixed 
    ? FP_FIXED
    : ( 
        (m_setPlots.size() > 1)
        ? FP_VARIABLE_MANY_PLOTS
        : FP_VARIABLE_1PLOT
      );
  if(bForce || (nState != m_nState))
  {
    m_nState = nState;
    if(m_nState != FP_VARIABLE_MANY_PLOTS)
    {
      m_nMinHeight = -1;
      m_pPanel->SetScrollbars(0,0,0,0,0,0,true);
    }
    else
    {
      m_pPanel->SetScrollRate(0,SCROLL_UNITS);
    }
  }
  if(m_nState == FP_VARIABLE_MANY_PLOTS)
  {
    _UpdateMinHeight();
    _UpdateVirtualWidth();
  }
  m_pSizer->Layout();
}
void CFramePlot::_UpdateVirtualWidth()
{
  wxSize szC = m_pPanel->GetClientSize();
  wxSize szV = m_pPanel->GetVirtualSize();
  if(szC.GetWidth() != szV.GetWidth())
  {
    szV.SetWidth(szC.GetWidth());
    m_pPanel->SetVirtualSize(szV);
  }
}
void CFramePlot::_UpdateMinHeight()
{
  if(m_bFixed || (m_nMinHeight > 0))
  {
    CParmOsirisGlobal parm;
    parm->SetPlotMinHeight(m_nMinHeight);
  }
  bool bShowSash = _ShowSash();
  int nMinHeight = bShowSash ? m_nMinHeight : -1;
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->SetSashAndMinHeight(bShowSash,nMinHeight);
  }
  bool bSync = true;
  if(nMinHeight > 0)
  {
    int nTotalHeight = 0;
    int nClientHeight = 0;
    wxSize szC = m_pPanel->GetClientSize();
#ifdef _DEBUG
      wxSize szV = m_pPanel->GetVirtualSize();
#endif
    nTotalHeight = 
      m_nMinHeight * (int)m_setPlots.size();
    nClientHeight = szC.GetHeight();
    if(nTotalHeight >= nClientHeight)
    {
      szC.SetHeight(nTotalHeight);
      m_pPanel->SetVirtualSize(szC);
      bSync = false;
    }
  }
  if(bSync)
  {
    _SyncSize();
  }
}
void CFramePlot::_SyncSize()
{
  wxSize szC = m_pPanel->GetClientSize();
  wxSize szV = m_pPanel->GetVirtualSize();
  if(szV != szC)
  {
    m_pPanel->SetVirtualSize(szC);
#ifdef _DEBUG
    szV = m_pPanel->GetVirtualSize();
#endif
  }
}
void CFramePlot::_SetFixed(bool bFixed)
{
  if(bFixed != m_bFixed)
  {
    m_bFixed = bFixed;
    {
      CParmOsirisGlobal parm;
      parm->SetPlotFixed(m_bFixed);
    }
    RebuildAll();
  }
  else
  {
    _SendSizeAction();
  }
  SetWindowScrollbarMenuLabel();
}
void CFramePlot::SetFocusPlot(int nPlot)
{
  // nPlot is the plot number (0 based)
  if( (nPlot >= (int)m_setPlots.size()) || (nPlot < 0) )
  {
    nPlot = 0;
  }
  wxSizerItem *pItem = m_pSizer->GetItem(nPlot);
  CPanelPlot *pWin = (pItem != NULL) ? (CPanelPlot *)pItem->GetWindow() : NULL;
  if(pWin != NULL)
  {
    pWin->SetFocusPlot();
  }
#if FP_SCROLL_EVENT
  SendScrollPlotEvent(nPlot,0);
#else
  _SetPlotScroll(nPlot,0);
#endif
}

void CFramePlot::_ScrollPlot()
{
  if( (m_nState == FP_VARIABLE_MANY_PLOTS) && IsShown())
  {
    int nScrollY = 0;
    bool bCancel = false;
    wxSize szC = m_pPanel->GetClientSize();
    if(szC.GetHeight() < 100)
    {
      bCancel = true; // window not sized and layed out yet
    }
    else if(m_nScrollOnTimer < 1) 
    {
      // nScrollY = 0; // scrolling to the top, already set to 0
    }
    else if(m_nScrollOnTimer >= (int)(m_setPlots.size() - 1))
    {
      // last plot, scrolling to the bottom
      nScrollY = 0x7fffffff; // need a number > max scroll units
    }
#if 1
    else
    {
      wxSize sz;
      wxSizerItem *pItem;
      for(int i = 0; i < m_nScrollOnTimer; i++)
      {
        pItem = m_pSizer->GetItem(i);
        if(pItem != NULL)
        {
          sz = pItem->GetSize();
          nScrollY += sz.GetHeight();
        }
      }
    }
#else
    else
    {
      // scroll such that plot following this one
      // starts at the bottom of the window
      // n is 1 based and m_pSizerItem->GetItem() is zero based,
      // so get the position of Item(n) and scroll it to the bottom
      // which is the position - client height
      //
      int nX;
      int nY;
      wxSizerItem *pItem = m_pSizer->GetItem(m_nScrollOnTimer);
      m_pPanel->GetViewStart(&nX,&nY);
      wxPoint posStart(SCROLL_UNITS_TO_PIXELS(nX),SCROLL_UNITS_TO_PIXELS(nY));
      wxPoint pos = pItem->GetPosition();
      wxSize szC = m_pPanel->GetClientSize();
      if(szC.GetHeight() < 100)
      {
        bCancel = true; // window not sized and layed out yet
      }
      else if(pos.y < posStart.y)
      {
        // need to scroll up
        nScrollY = pos.y;
      }
      else
      {
        wxSize szItem = pItem->GetSize();
        int nYWindowBottom = posStart.y + szC.GetHeight();
        int nYPlotBottom = pos.y + szItem.GetHeight();
        if(nYPlotBottom <= nYWindowBottom)
        {
          bCancel = true; // no scrolling needed
        }
        else if(szC.GetHeight() < (szItem.GetHeight() + SCROLL_UNITS))
        {
          nScrollY = pos.y;
        }
        else
        {
          // scroll bottom of window into view
          // add (SCROLL_UNITS - 1) to round scroll units up
          nScrollY = nYPlotBottom - szC.GetHeight() + SCROLL_UNITS - 1;
        }
      }
    }
#endif
    if(!bCancel)
    {
      nScrollY -= m_nScrollOnTimerOffset;
      if(nScrollY > 0)
      {
        nScrollY = PIXEL_TO_SCROLL_UNITS(nScrollY);
      }
      else
      {
        nScrollY = 0;
      }
      m_pPanel->Scroll(0,nScrollY);
      _ResetPlotScroll();
    }
  }
}
#if FP_SCROLL_EVENT

void CFramePlot::OnScrollPlot(wxCommandEvent &e)
{
  int nInt = e.GetInt();
  int nDelay;
  int nPlot;
  _ExtractScrollEventInt(nInt, &nPlot,&nDelay);
  if(nDelay > 1)
  {
    SendScrollPlotEvent(nPlot,nDelay - 1);
  }
  else
  {
    ScrollPlot(nPlot);
  }
}
#endif
void CFramePlot::OnSizeAction(wxCommandEvent &e)
{
  int nInt = e.GetInt();
  if(!IsShown())
  {} // do nothing
  else if(nInt > 1)
  {
    _SendSizeAction(nInt - 1);
  }
  else if(m_nState ==  FP_VARIABLE_MANY_PLOTS)
  {
    wxSize szC = m_pPanel->GetClientSize();
    wxSize szV = m_pPanel->GetVirtualSize();
    int nHC = szC.GetHeight();
    if(nHC < m_nMinHeight)
    {
      // size of window is smaller than one 
      // plot, so make the min plot height smaller
      _UpdateMinHeight(nHC);
    }
    else
    {
      // when scrollbar appears, client width
      // becomes smaller and virtual width
      // needs to be updated 
      _UpdateVirtualWidth();
    }
  }
  else
  {
    _SyncSize();
  }
}
bool CFramePlot::Show(bool show)
{
  bool bRtn = CMDIFrame::Show(show);
  if(bRtn && show)
  {
    TnwxBatchColl<CPanelPlot> batch(m_setPlots);
    _UpdateViewState(true);
    m_pPanel->Scroll(0,0);
  }
  return bRtn;
}
void CFramePlot::OnSize(wxSizeEvent &e)
{
  _SendSizeAction();
  if(&e != NULL)
  {
    e.Skip(true);
  }
}

wxBitmap *CFramePlot::CreateBitmap(
  int nWidth, int nHeight, int nDPI, const wxString &sTitle)
{
  wxBitmap *pBitmap = new wxBitmap(nWidth,nHeight,32);
  wxMemoryDC dc(*pBitmap);
  int nTitleOffset = 0;
  int nPlotHeight;


  // initialize bitmap to white -- probably not necessary

  dc.SetBackground(*wxWHITE_BRUSH);
  dc.SetBackgroundMode(wxSOLID);
  dc.Clear();

  // set up title

  if( (!sTitle.IsEmpty()) && (!m_setPlots.empty()) )
  {
    wxSize szTitle;
    wxFont fn = (*m_setPlots.begin())->GetPlotCtrl()->GetAxisFont();
    double dSize = double(fn.GetPointSize() * nDPI) * (1.0/36.0);
    // scale font by multplying by DPi and dividing by 72, then double
    int nMaxX = (nWidth * 9) / 10;
    int nMaxY = nHeight / 10;
    double dResize = 1.0;
    bool bResize = true;
    fn.SetPointSize(nwxRound::Round(dSize));
    //fn.SetWeight(wxFONTWEIGHT_BOLD);  // comment out, kinda ugly

    dc.SetFont(fn);
    dc.SetTextForeground(*wxBLACK);
    dc.SetTextBackground(*wxWHITE);
    szTitle = dc.GetTextExtent(sTitle);
    if(szTitle.GetWidth() > nMaxX)
    {
      dResize = (double)nMaxX / (double)szTitle.GetWidth();
      bResize = true;
    }
    if(szTitle.GetHeight() > nMaxY)
    {
      double d = (double) nMaxY / (double) szTitle.GetHeight();
      if(d < dResize)
      {
        dResize = d;
      }
      bResize = true;
    }
    if(bResize)
    {
      int nPt = (int)floor(dSize * dResize);
      if(nPt < 4) { nPt = 4;}
      fn.SetPointSize(nPt);
      dc.SetFont(fn);
      szTitle = dc.GetTextExtent(sTitle);
    }
    nTitleOffset = szTitle.GetHeight();
    int nTitleHalf = nTitleOffset >> 1;
    nTitleOffset += nTitleHalf;
    int nXoffset = nWidth - szTitle.GetWidth();
    if(nXoffset < 0)
    {
      nXoffset = 0;
    }
    nXoffset >>= 1;
    dc.DrawText(sTitle,nXoffset,nTitleHalf);
  }

  nPlotHeight = (nHeight - nTitleOffset) / (int) m_setPlots.size();
  if(nPlotHeight >= 20)
  {
    set<CPanelPlot *>::iterator itr;
    wxRect rect(0,0,nWidth,nPlotHeight);
    double dDPI = (double) nDPI;
    CPanelPlot *pPlot;
    nwxPlotCtrl *pPlotCtrl;
    int nY;

    // there is a axis sync bug, so here is the work around
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if(pPlot->SyncValue())
      {
        SyncTo(pPlot);
        break;
      }
    }
#ifdef __WXMAC__
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      // create separate bitmap/dc for each plot and do a Blit to copy to main bitmap/dc
      wxBitmap BitmapTmp(nWidth,nPlotHeight,32);
      wxMemoryDC dcTmp(BitmapTmp);
      dcTmp.SetBackground(*wxWHITE_BRUSH);
      dcTmp.Clear();
      pPlot = *itr;
      nY = (int)pPlot->GetPlotNumber();
      pPlotCtrl = pPlot->GetPlotCtrl();
      pPlotCtrl->DrawEntirePlot(&dcTmp,rect,dDPI);
      int nYdest = (nY * nPlotHeight)  + nTitleOffset;
      dc.Blit(0,nYdest,nWidth,nPlotHeight,&dcTmp,0,0);
    }
#else
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      nY = (int)pPlot->GetPlotNumber();
      rect.SetY((nY * nPlotHeight) + nTitleOffset);
      pPlotCtrl = pPlot->GetPlotCtrl();
      pPlotCtrl->DrawEntirePlot(&dc,rect,dDPI);
    }
#endif
  }
  return pBitmap;
}
#if HAS_STATUS_BAR
void CFramePlot::UpdateStatusBar()
{
  m_pParent->SetStatusText(m_pData->GetFilename());
}
#endif

BEGIN_EVENT_TABLE(CFramePlot,CMDIFrame)
EVT_COMMAND(IDhistoryButton,CEventHistory,CFramePlot::OnHistoryUpdate)
EVT_CLOSE(CFramePlot::OnClose)
EVT_CONTEXT_MENU(CFramePlot::OnContextMenu)
EVT_BUTTON(IDhistoryButton,CFramePlot::OnHistoryButton)
EVT_BUTTON(IDgraphTable, CFramePlot::OnTableButton)
EVT_SASH_DRAGGED(wxID_ANY,CFramePlot::OnSashDragged)
EVT_SIZE(CFramePlot::OnSize)
EVT_COMMAND(IDframePlot,wxEVT_SIZE_DELAY_PLOT,CFramePlot::OnSizeAction)
#if FP_SCROLL_EVENT
EVT_COMMAND(IDframePlot,wxEVT_SCROLL_PLOT,CFramePlot::OnScrollPlot)
#endif
END_EVENT_TABLE()

