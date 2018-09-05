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
*  FileName: CPanelPlot.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_PANEL_PLOT_H__
#define __C_PANEL_PLOT_H__

#include <wx/sashwin.h>
#include <wx/sizer.h>
#include "nwx/stdb.h"
#include "nwx/CIncrementer.h"
#include <map>
#include <vector>
#include <set>
#include <memory>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxPlotCtrl.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxShiftSizer.h"

#include "CPanelPlotToolbar.h"
#include "CMenuPlot.h"
#include "nwx/nwxMenuItem.h"
#include "Platform.h"
#include "wxIDS.h"
class CFrameAnalysis;
class COARfile;
class COARsample;
class CPlotData;
class CKitColors;
class CPanelPlot;
class CMDIFrame;
class CFramePlot;
class wxTimer;
class IOARpeak;
class CSamplePeak;
class COARpeakAny;

typedef map<int,wxPlotData *> mapSamplePlots;

typedef vector<wxPlotData *> vectorILSlines;
typedef map<double,wxPlotData *> mapMinRfu;

class CPlotCtrl : public nwxPlotCtrl
{
public:
  CPlotCtrl(wxWindow *parent, CPanelPlot *pPlot, wxWindowID id = wxID_ANY);
  virtual void OnClickXLabel(const nwxPointLabel &x, const wxPoint &pt);
  virtual void OnClickLabel(const nwxPointLabel &x, const wxPoint &pt);
  bool SetViewRect(const wxRect2DDouble &view, bool send_event=false);
private:
  CPanelPlot *m_pPlot;
};


class CPanelPlot : public wxSashWindow, public InwxShiftReceiver
{
private:
  class CLadderPeakSet
  {
  public:
    CLadderPeakSet(
      const wxColour &colour, 
      const wxString &sChannelName, 
      unsigned int nChannel) :
      m_colour(colour),
      m_sChannelName(sChannelName),
      m_nChannel(nChannel)
    { }
    CLadderPeakSet(const CLadderPeakSet &x)
    {
      (*this) = x;
    }
    CLadderPeakSet &operator = (const CLadderPeakSet &x)
    {
      m_colour = x.m_colour;
      m_sChannelName = x.m_sChannelName;
      m_nChannel = x.m_nChannel;
      m_vpPeaks = x.m_vpPeaks; // copy pointers
      return *this;
    }
    void Reserve(size_t n)
    {
      m_vpPeaks.reserve(n);
    }
    void AddPeak(CSamplePeak *p)
    {
      m_vpPeaks.push_back(p);
    }
    const vector<CSamplePeak *> *GetPeaks() const
    {
      return &m_vpPeaks;
    }
    unsigned int GetChannel() const
    {
      return m_nChannel;
    }
    const wxColour &GetColour() const
    {
      return m_colour;
    }
    const wxString &GetChannelName() const
    {
      return m_sChannelName;
    }
    size_t GetPeakCount() const
    {
      return m_vpPeaks.size();
    }
    bool HasPeaks() const
    {
      return (GetPeakCount() > 0);
    }

  private:
    vector<CSamplePeak *> m_vpPeaks;
    wxColour m_colour;
    wxString m_sChannelName;
    unsigned int m_nChannel;
  };
  class DelayedViewRect
  {
    // sometimes the viewport is not set as specified on a new window
    // this class is used to delay setting the viewport by 'm_nDelay'
    // timer events
    //
    // also, the view rectangle is stored here so that when it is 
    // retrieved, the last settings is returned as opposed to
    // the 'wrong' setting when the above situation occurs
    // This is useful when syncing multiple plots or making other
    // changes where the view rectangle is saved and restored
    //
  public:
    DelayedViewRect() :
        m_rect(0.0,0.0,1.0,1.0),
        m_pPlot(NULL),
        m_nDelay(0),
        m_bSendEvent(false)
    {}
    virtual ~DelayedViewRect()
    {}

    const wxRect2DDouble &GetViewRect()
    {
      return m_rect;
    }
    void Check()
    {
      switch(m_nDelay)
      {
      case 0:
        break;
      case 1:
        _DoIt();
        break;
      default:
        m_nDelay--;
        break;
      }
    }
    void SetPlotCtrl(CPlotCtrl *p)
    {
      m_pPlot = p;
    }
    void Setup(const wxRect2DDouble &rect, bool bSendEvent = false, unsigned int nDelay = 1)
    {
      m_rect = rect;
      m_bSendEvent = bSendEvent;
      if(nDelay >= m_nDelay)
      {
        m_nDelay = nDelay;
      }
      if(!m_nDelay)
      {
        _DoIt();
      }
    }
    void Update()
    {
      if( (!m_nDelay) && (m_pPlot != NULL) )
      {
        m_rect = m_pPlot->GetViewRect();
      }
    }
    unsigned int GetDelay()
    {
      return m_nDelay;
    }
  private:
    void _DoIt()
    {
      if(m_pPlot != NULL)
      {
        m_pPlot->SetViewRect(m_rect, m_bSendEvent);
        m_nDelay = 0;
      }
    }
    wxRect2DDouble m_rect;
    CPlotCtrl *m_pPlot;
    unsigned int m_nDelay;
    bool m_bSendEvent;
  };

public:
  // constructor for CFramePlot - MDI window with plots
  static const int ALLELE_SORT;
  static const int ARTIFACT_SORT;
  CPanelPlot(
    CFramePlot *parent, 
    CPlotData *pData,
    COARfile *pFile,
    CMenuHistory *pMenuHistory,
    CKitColors *pColors,
    int nMenuNumber,
    bool bDraw = true, 
    int nPlotNumber = 0,
    bool bExternalTimer = false);

  // constructor for CFrameAnalysis - MDI with grid and plot preview
  CPanelPlot(
    wxWindow *parent,
    CFrameAnalysis *pFrame, 
    CPlotData *pData,
    COARfile *pFile,
    CKitColors *pColors,
    bool bExternalTimer = false);
  virtual ~CPanelPlot();

  bool IsPreview()
  {
    return (m_pFrameAnalysis != NULL);
  }
  bool HasToolbar()
  {
    return (m_pButtonPanel != NULL);
  }
  COARsample *GetSample();
  bool CanShowPeakArea();
  void SetSashAndMinHeight(bool bShowSash, int nHeight);
  void SetLabelType(LABEL_PLOT_TYPE n,LABEL_PLOT_TYPE nDefault = LABEL_NONE);
  void SetArtifactLabelNone()
  {
    _SetArtifactLabel(CArtifactDisplayList::nArtifactLabelNone);
  }
  void SetArtifactLabelCritical()
  {
    _SetArtifactLabel(CArtifactDisplayList::nArtifactLabelCritical);
  }
  void SetArtifactLabelAll()
  {
    _SetArtifactLabel(CArtifactDisplayList::nArtifactLabelAll);
  }
#if 0
  int GetLabelType()
  {
    return (int)m_pMenu->LabelType();
  }
#endif
  size_t GetLabelTypes(vector<unsigned int> *pan)
  {
    return m_pMenu->GetLabelTypes(pan);
  }
  bool MenuEvent(wxCommandEvent &e);

  wxMenuItem *GetMenuItem(wxMenu *parent);
  wxMenu *GetMenu()
  {
    return m_pMenu;
  }
  CMenuPlot *GetMenuPlot()
  {
    return m_pMenu;
  }
  CPanelPlotToolbar *GetToolbar()
  {
    return m_pButtonPanel;
  }
  void SyncToolbar(CPanelPlot *p)
  {
    if( (p != this) && HasToolbar() )
    {
      m_pButtonPanel->SyncTo(p->m_pButtonPanel);
      m_pShiftSizer->UpdateSize();
    }
  }
  void SyncState(CPanelPlot *p, int nID);
  void RebuildCurves(bool bIgnoreViewRect = false);
  wxRect2DDouble GetZoomOutRect(bool bAll = false);
  void ZoomToLocus(const wxString &sLocus, unsigned int nDelay = 0);
  void EditPeak(COARpeakAny *pPeak);
  wxRect2DDouble GetZoomLocus(const wxString &sLocus);

  void ShowToolbar(bool bShow);
  CPlotData *GetPlotData()
  {
    return m_pData;
  }
  void ShowScrollbars(bool bShow)
  {
    m_pPlotCtrl->ShowScrollbars(bShow);
  }
  bool AreScrollbarsShown()
  {
    return m_pPlotCtrl->AreScrollbarsShown();
  }
  bool IsToolbarShown()
  {
    return 
      HasToolbar()
      ? m_pButtonPanel->IsShown()
      : false;
  }

  int GetPlotTypes()
  {
    return m_pMenu->GetPlotTypes();
  }
  void CopySettings(CPanelPlot &w);
  void SetPlotNumber(unsigned int i)
  {
    m_nPlotNr = i;
  }
  unsigned int GetPlotNumber()
  {
    return m_nPlotNr;
  }
  bool SyncValue()
  {
    return m_pMenu->SyncValue();
  }
  bool XBPSValue()
  {
    return m_pMenu->XBPSValue();
  }
  void SetXBPS(bool b = true, bool bUpdate = false)
  {
    if(b != XBPSValue())
    {
      m_pMenu->SetXBPS(b);
      _SyncControllers(m_pMenu);
      if(bUpdate)
      {
        RebuildCurves();
      }
    }
  }
  void EnableLabelMenu(bool b = true)
  {
    if(b != m_pMenu->IsLabelMenuEnabled())
    {
      m_pMenu->EnableLabelMenu(b);
      _SyncControllers(m_pMenu);
    }
  }
  void EnableDelete(bool b = true)
  {
//    if(HasToolbar())
//    {
//      m_pButtonPanel->EnableDelete(b);
//    }
    if(m_pMenu->IsDeleteEnabled() != b)
    {
      m_pMenu->EnableDelete(b);
      _SyncControllers(m_pMenu);
    }
  }
  void EnableAppend(bool b = true)
  {
//    if(HasToolbar())
//    {
//      m_pButtonPanel->EnableAppend(b);
//    }
    if(m_pMenu->IsAppendEnabled() != b)
    {
      m_pMenu->EnableAppend(b);
      _SyncControllers(m_pMenu);
    }
  }
  void ShowChannel(unsigned int n, bool b = true)
  {
    if(m_pMenu->ChannelValue(n) != b)
    {
      m_pMenu->ShowChannel(n,b);
      _SyncControllers(m_pMenu);
    }
  }
  void ShowOneChannel(unsigned int n)
  {
    m_pMenu->ShowOneChannel(n);
    _SyncControllers(m_pMenu);
  }

  void ShowAllChannels(bool bShow = true);

  const wxRect2DDouble &GetViewRect()
  {
    const wxRect2DDouble &r = m_viewRect.GetViewRect();
    if(r.m_width < 1.0001)
    {
      return m_pPlotCtrl->GetViewRect();
    }
    return r;
  }
  void SetViewRect(const wxRect2DDouble &r, bool bSendEvent = false, unsigned int nDelay = 0)
  {
    m_viewRect.Setup(r,bSendEvent, nDelay);
  }

  void ZoomOut(bool bAll = false, unsigned int nDelay = 0)
  {
    SetViewRect(GetZoomOutRect(bAll),false,nDelay);
  }
  void BeginBatch()
  {
    m_pPlotCtrl->BeginBatch();
    m_nBatchCount++;
  }
  void EndBatch()
  {
    m_pPlotCtrl->EndBatch();
    m_nBatchCount--;
  }
  void MakeCurveVisible(int n = -1)
  {
    m_pPlotCtrl->MakeCurveVisible(n);
  }
  nwxPlotCtrl *GetPlotCtrl()
  {
    return m_pPlotCtrl;
  }
  void SetFocusPlot()
  {
    m_pPlotCtrl->SetFocus();
  }
  const wxDateTime *GetSelectedTime();
  void SetExternalTimer(bool b = true);
  void OnTimer(wxTimerEvent &e);
  void SetOARfile(COARfile *pFile);
  void RebuildLabels(bool bRedraw = false);
  void UpdateLadderLabels();
  void SetHistoryMenu(CMenuHistory *pMenu)
  {
    if(HasToolbar())
    {
      m_pButtonPanel->SetHistoryMenu(pMenu);
    }
  }
  void UpdateHistoryButtons()
  {
    if(HasToolbar())
    {
      m_pButtonPanel->UpdateHistoryButtons();
    }
  }

  // IShiftReceiver functions
  virtual void ShiftLeft(bool bShiftKey);
  virtual void ShiftRight(bool bShiftKey);
  virtual bool CanShiftLeft();
  virtual bool CanShiftRight();

  void ResetDefaults()
  {
    if(IsPreview())
    {
      SetPreviewSettings();
    }
    else
    {
      SetPlotSettings();
    }

//    if(HasToolbar())
//    {
//      m_pButtonPanel->ResetDefaults();
//    }
//    m_pMenu->ResetDefaults();
  }
private:
  static wxColour COLOUR_RFU;

  void SetupLabelMenus();
  void SetPlotSettings();
  void SetPreviewSettings();
  void _SyncControllers(CPlotController *pSyncTo);
  CMenuPlot *_GetLastMenu();
  CMDIFrame *_GetFrame();
  void _SetArtifactLabel(int nLabel);
  void _ConvertRectToBPS(wxRect2DDouble *pRect);

  void UpdateSettingsPlot();
  void UpdateSettingsPreview();
  void UpdateSettings()
  {
    if(IsPreview())
    {
      UpdateSettingsPreview();
    }
    else
    {
      UpdateSettingsPlot();
    }
  }
  void UpdateGridLabels(int nLabelType = -1);
  wxColour GetMinRfuColour();
  static void ExpandRect(wxRect2DDouble *p, double dBy = 0.05); 
  // expand height and width by  (1 + dBy + dBy)  
  // for 5% in each direction or 10% total,  dBy = 0.05
  void ExtendLabelHeight(wxRect2DDouble *p);  // expand height to accommodate 8px labels
  int GetLabelHeightPixels()
  {
    return 16;
  }
  void _OnTimer(wxTimerEvent &);
  void _SendSizeAction()
  {
    wxCommandEvent ee(wxEVT_COMMAND_ENTER,wxID_ANY);
    ee.SetEventObject(this);
    GetEventHandler()->AddPendingEvent(ee);
  }
  static double *Copy2Points(double *pd)
  {
    size_t n = sizeof(double) << 1;
    double *pRtn = (double *)malloc(n);
    memcpy((void *) pRtn, (void *)pd,n);
    return pRtn;
  }
  bool _IsXAxisBPS()
  {
    return ((m_pMenu != NULL) && m_pMenu->XBPSValue());
  }
  const char *_GetXAxisLabel()
  {
    const char *psRtn =
      _IsXAxisBPS()
      ? "BPS (ILS ref.)"
      : "Time (seconds)";
    return psRtn;
  }
  vectorILSlines *_GetILSlines()
  {
    vectorILSlines *pRtn = _IsXAxisBPS() ? &m_vILS_XBPS : &m_vILS;
    if(pRtn->empty())
    {
      BuildILSlines();
    }
    return pRtn;
  }
  mapMinRfu *_GetMinRFUlines()
  {
    mapMinRfu *pRtn = _IsXAxisBPS() ? &m_mapMinRfuAll_XBPS : &m_mapMinRfuAll;
    return pRtn;
  }

  void _BuildPanel(
    int nMenuNumber,bool bFirst, 
    CMenuHistory *pMenuHistory);
  void BuildILSlines();


  void ShowILSlines();
  void BuildMinRfuLines();
  void CleanupMinRfuLines();
  void BuildRfuSet(set<double> *psetD);
  void ShowMinRfuLines();

  //  labels

  void _AppendLadderPeaks(
    unsigned int nChannel,
    const wxString &sChannelName);

  void _BuildOARlabels();
  void _BuildPeakLabels(
    const vector<CSamplePeak *> *pp,
    const wxColour &colour,
    const wxString &sChannelName,
    unsigned int nChannel,
    vector<unsigned int> &anLabel);
  void _BuildLadderPeakLabels(
    vector<unsigned int> &anLabel);

  void _BuildPLTlabels(bool bArtifactOnly = false, unsigned int nChannel = 0);
  wxString _AlleleLabel(const IOARpeak *pPeak, vector<unsigned int> &anLabel);
  wxString _ArtifactToolTip(
    const IOARpeak *pPeak, const wxString &sChannelName);
  wxString _AlleleToolTip(
    const IOARpeak *pPeak, int nChannel, const wxString &sChannelName);

  // delayed view rect update

  void _CheckViewRect();
  static int _SamplePlotKey(DATA_TYPE nType, unsigned int nChannel, bool bNoise, bool bXBPS)
  {
    // create a key for mapSamplePlot
    int nRtn = ( (nChannel & 255) << 8 ) | (nType << 2) | (bNoise ? 2 : 0) | (bXBPS ? 1 : 0);
    return nRtn;
  }


  wxPlotData *_FindData(
    DATA_TYPE nType, 
    unsigned int nChannel, 
    bool bNoise, 
    bool bXBPS);
  void _CleanupLadderPeakSet();
  void _CleanupPeakAny();
  int _GetLadderPeakCount();
  //void _CleanupMenu(); // remove m_pMenuItem from its menu and delete
  void _BuildMenu(int nPlotNr);

  vector<CLadderPeakSet *> m_vpLadderPeakSet;
  DelayedViewRect m_viewRect;

  set<double> m_setMinRfu;
  mapMinRfu m_mapMinRfuAll;
  mapMinRfu m_mapMinRfuAll_XBPS;

  mapSamplePlots m_mapPlotData;
  vector<COARpeakAny *> m_vPeakAny;
  vectorILSlines m_vILS;
  vectorILSlines m_vILS_XBPS;
  CPlotData *m_pData;
  COARfile *m_pOARfile;
  CKitColors *m_pColors;
  CPlotCtrl *m_pPlotCtrl;
  wxBoxSizer *m_pSizer;
  wxPanel *m_pPanel;

  // m_pButtonPanel and m_pMenu need to be kept in sync
  CPanelPlotToolbar *m_pButtonPanel;
  CMenuPlot  *m_pMenu;
  CMenuPlot *m_pMenuPopup;
  nwxShiftSizer *m_pShiftSizer;

  CFramePlot *m_pFramePlot;
  CFrameAnalysis *m_pFrameAnalysis;
  wxTimer *m_pTimer;
  auto_ptr<nwxMenuItem> m_pMenuItem;

  unsigned int m_nPlotNr;
  unsigned int m_nBatchCount;
  unsigned int m_nILScurveOffset;
  unsigned int m_nNoiseCurves;
  int m_nMenuOffset;
  bool m_bExternalTimer;
  bool m_bDoTimer;
  bool m_bXBPS; // used to check if event changed x axis

  // BEGIN - keep track of channels with no noise
  std::set<unsigned int> m_setNoNoiseChannel;
  bool _IsNoNoiseChannel(unsigned int nChannel)
  {
    bool bRtn = (m_setNoNoiseChannel.find(nChannel) != m_setNoNoiseChannel.end());
    return bRtn;
  }
  void _SetNoNoiseChannel(unsigned int nChannel)
  {
    if(!_IsNoNoiseChannel(nChannel))
    {
      m_setNoNoiseChannel.insert(nChannel);
    }
  }
  // END - keep track of channels with no noise

public:
//  commented out 7/14/08 kill in 60 days
//  void OnChannelToggle(wxCommandEvent &);
//  void OnDataToggle(wxCommandEvent &);
//  void OnBtnILS(wxCommandEvent &);
//  void RemoveILSlines();
//  void UpdateMinRfuLines();
//  void HideCurve(DATA_TYPE nType, unsigned int nChannel);

  void OnRebuildCurves(wxCommandEvent &);

  void OnZoomOut(wxCommandEvent &);
//  void OnBtnLabel(wxCommandEvent &);
//  void OnBtnAnalyze(wxCommandEvent &);

//  void OnBtnRaw(wxCommandEvent &);
//  void OnBtnLadder(wxCommandEvent &);
//  void OnBtnSyncAxes(wxCommandEvent &);
  void OnBtnMultiple(wxCommandEvent &);
  void OnBtnAppend(wxCommandEvent &);
  void OnBtnRemove(wxCommandEvent &);
  void OnBtnDetails(wxCommandEvent &);
  void OnSync(wxCommandEvent &);
  void OnViewChanged(wxPlotCtrlEvent &);
  void OnPointSelected(wxPlotCtrlEvent &);
  void OnLabelTypeChanged(wxCommandEvent &);
  void OnTimerEvent(wxTimerEvent &);
  void OnContextMenu(wxContextMenuEvent &e);
  void OnSize(wxSizeEvent &e);
  void OnSizeAction(wxCommandEvent &e);
  void OnCommandEnter(wxCommandEvent &e);

  void LabelTypeChanged();
  DECLARE_EVENT_TABLE()
};

class CPanelPlotToolbarSaveState
{
public:
  CPanelPlotToolbarSaveState()
    : m_pToolbar(NULL), m_pWin(NULL)
  {}
  CPanelPlotToolbarSaveState(CPanelPlot *pWin)
    : m_pToolbar(NULL), m_pWin(NULL)
  {
    Setup(pWin,false);
  }
  virtual ~CPanelPlotToolbarSaveState();
  CPanelPlot *GetPanel()
  {
    return m_pWin;
  }
  CPanelPlot *RestoreState(bool bRemove = true);
  void Setup(CPanelPlot *pWin,bool bRestoreCurrent = true);
  void Clear()
  {
    Setup(NULL,false);
  }
private:

  CPanelPlotToolbar *m_pToolbar;
  CPanelPlot *m_pWin;
  int m_nX;
  bool m_bCanShiftLeft;
  bool m_bCanShiftRight;
};

#endif
