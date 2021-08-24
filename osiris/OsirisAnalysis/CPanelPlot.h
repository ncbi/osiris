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
*  OS-1435 7/7/20 add m_bPrinting and GetColor()
*     to accommodate the dark print option
*/

#ifndef __C_PANEL_PLOT_H__
#define __C_PANEL_PLOT_H__

#if 0

#define PANEL_PLOT_TYPE wxSashLayoutWindow
#include <wx/laywin.h>

#else
#define PANEL_PLOT_TYPE wxSashWindow
#include <wx/sashwin.h>

#endif

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
#include "nwx/nwxTimerReceiver.h"

#include "CPlotData.h"
#include "CPanelPlotToolbar.h"
#include "CMenuPlot.h"
#include "nwx/nwxMenuItem.h"
#include "Platform.h"
#include "wxIDS.h"

DECLARE_EVENT_TYPE(CEventCannotShowBPS,-1)

class CFrameAnalysis;
class COARfile;
class COARsample;
class CKitColors;
class CPanelPlot;
class CMDIFrame;
class CFramePlot;
class wxTimer;
class IOARpeak;
class CSamplePeak;
class COARpeakAny;
class wxBitmap;

typedef std::map<int,wxPlotData *> mapSamplePlots;

typedef std::vector<wxPlotData *> vectorILSlines;
typedef std::map<double,wxPlotData *> mapMinRfu;
typedef std::map<int, wxColour> mapColor;

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

class CPanelPlot :
  public PANEL_PLOT_TYPE,
  public InwxShiftReceiver,
  public nwxTimerReceiver
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
    const std::vector<CSamplePeak *> *GetPeaks() const
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
    std::vector<CSamplePeak *> m_vpPeaks;
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
      m_pBatch(NULL),
      m_rect(0.0, 0.0, 1.0, 1.0),
      m_pPlot(NULL),
      m_nDelay(0),
      m_bSendEvent(false)
    {}
    virtual ~DelayedViewRect()
    {
      _CleanupBatch();
    }
    const wxRect2DDouble &GetViewRect()
    {
      return m_rect;
    }
    void Check(bool bForce = false)
    {
      if (bForce)
      {
        m_nDelay = 1;
      }
      switch (m_nDelay)
      {
      case 0:
        break;
      case 1:
        _DoIt();
        m_nDelay = 0;
        break;
      default:
        m_nDelay--;
        break;
      }
    }
    void SetPlotCtrl(CPlotCtrl *p)
    {
      _CleanupBatch();
      m_pPlot = p;
      m_rect = p->GetViewRect();
    }
    void Setup(const wxRect2DDouble &rect, bool bSendEvent = false, unsigned int nDelay = 1)
    {
      m_rect = rect;
      m_bSendEvent = bSendEvent;
      if (nDelay >= m_nDelay)
      {
        m_nDelay = nDelay;
        _SetupBatch();
      }
      if (!m_nDelay)
      {
        _DoIt();
      }
    }
    void Update()
    {
      if ((!m_nDelay) && (m_pPlot != NULL))
      {
        m_rect = m_pPlot->GetViewRect();
      }
    }
    unsigned int GetDelay()
    {
      return m_nDelay;
    }
  private:
    void _CleanupBatch()
    {
      if (m_pBatch != NULL)
      {
        delete m_pBatch;
        m_pBatch = NULL;
      }
    }
    void _SetupBatch(bool bCheckDelay = true)
    {
      if (bCheckDelay && !m_nDelay)
      {
      }
      else if (m_pBatch == NULL && m_pPlot != NULL)
      {
        m_pBatch = new TnwxBatch<CPlotCtrl>(m_pPlot);
      }
    }
    void _DoIt()
    {
      if (m_pPlot != NULL)
      {
        m_pPlot->SetViewRect(m_rect, m_bSendEvent);
        m_nDelay = 0;
        _CleanupBatch();
      }
    }
    TnwxBatch<CPlotCtrl> *m_pBatch;
    wxRect2DDouble m_rect;
    CPlotCtrl *m_pPlot;
    unsigned int m_nDelay;
    bool m_bSendEvent;
  };

public:
  // constructor for CFramePlot - MDI window with plots
  static const int ALLELE_SORT;
  static const int ARTIFACT_SORT;
  static const int MENU_NUMBER_ANALYSIS_PRINT;
  enum
  {
    ZOOM_PRIMER_PEAK_NONE = 0,
    ZOOM_PRIMER_PEAK_X = 1,
    ZOOM_PRIMER_PEAK_XY = 3
  };
  CPanelPlot(
    CFramePlot *parent,
    CPlotData *pData,
    COARfile *pFile,
    CMenuHistory *pMenuHistory,
    CKitColors *pColors,
    int nMenuNumber,
    bool bDraw = true,
    int nPlotNumber = 0,
    bool bPrinting = false
   );

  // constructor for CFrameAnalysis - MDI with grid and plot preview
  CPanelPlot(
    wxWindow *parent,
    CFrameAnalysis *pFrame,
    CPlotData *pData,
    COARfile *pFile,
    CKitColors *pColors
    //,bool bExternalTimer = false   // EXT TIMER
    );

  // constructor for Analysis Printout, called from CPrintOutAnalysis
  CPanelPlot(
    CFrameAnalysis *pFrame,
    CPlotData *pData,
    COARfile *pFile
  );
  virtual ~CPanelPlot();

  void InvalidateColors()
  {
    m_mapColors.clear();
    CleanupBins();
  }
  void SendPlotSizeEvent();
  bool IsPreview()
  {
    // NOTE: this returns true for an Analysis printout
    return (m_pFrameAnalysis != NULL) && !m_bPrintAnalysis;
  }
  bool IsPrintAnalysis()
  {
    return m_bPrintAnalysis;
  }
  bool IsPlotFrame()
  {
    return m_pFramePlot != NULL;
  }
  bool HasToolbar()
  {
    return (m_pButtonPanel != NULL);
  }
  int DrawPlotTitleToDC(
    wxDC *pDC, const wxString &sTitle,
    const wxString &sTitleDetails,
    int nWidth, int nHeight, double dDPI,
    bool bForcePrintFont);

  int DrawXAxisLabelToDC(wxDC *pDC, const wxRect &rectBitmap, double dDPI, bool bForcePrintFont)
  {
    return GetPlotCtrl()->DrawXAxisLabel(pDC, rectBitmap, dDPI, bForcePrintFont, true);
  }
  int GetXAxisLabelHeight(wxDC *pDC, const wxRect &rectBitmap, double dDPI, bool bForcePrintFont)
  {
    return GetPlotCtrl()->GetXAxisLabelSize(pDC, rectBitmap, dDPI, bForcePrintFont).GetHeight();
  }
  void SetRenderingToWindow(bool b)
  {
    GetPlotCtrl()->SetRenderingToWindow(b);
    if (b && !m_bIgnoreTimer)
    {
      m_viewRect.Check(true);
    }
    if (!b)
    {
      Show(false);
    }
    m_bIgnoreTimer = !b;
  }
  bool GetRenderingToWindow()
  {
    return GetPlotCtrl()->RenderingToWindow();
  }

  wxBitmap *CreateMultiChannelBitmap(
    int nWidth, int nHeight, int nDPI,
    int nStartChannel,
    int nPageNr,
    bool bForcePrintFont = false);

  void DrawPlotToDC(
    wxDC *pDC,
    const wxRect &rect,
    double dDPI,
    bool bShowXAxis = true,
    bool bForcePrintFont = false);
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
  size_t GetLabelTypes(std::vector<unsigned int> *pan)
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
  wxRect2DDouble GetZoomOutRect(
    int nPrimerPeaks, int nLabelHeight = 0,
    double dMinRFU = -1.0, bool bIncludeRightEnd = false);
  wxRect2DDouble GetZoomOutRect(bool bAll = false, int nLabelHeight = 0)
  {
    return GetZoomOutRect(
      (bAll ? ZOOM_PRIMER_PEAK_XY : ZOOM_PRIMER_PEAK_NONE), nLabelHeight, -1.0, bAll);
  }
  void ZoomToLocus(const wxString &sLocus, unsigned int nDelay = 0);
  void EditPeak(COARpeakAny *pPeak);
  wxRect2DDouble GetZoomLocus(const wxString &sLocus);
  CPlotData *GetPlotData()
  {
    return m_pData;
  }

  void ShowToolbar(bool bShow);
  void ShowScrollbars(bool bShow);
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
  void CopySettings(CPanelPlot &w, int nDelay = 1);
  void CopySettings(CPanelPlot &w, int nDelay, bool bPrinting)
  {
    CopySettings(w, nDelay);
    SetPrinting(bPrinting);
  }
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
  void SetSync(bool b)
  {
    m_pMenu->SetSync(b);
  }
  bool XBPSValue();
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
    if(m_bIgnoreTimer || r.m_width < 1.0001 || !m_pPlotCtrl->RenderingToWindow())
    {
      return m_pPlotCtrl->GetViewRect();
    }
    return r;
  }
  void SetViewRect(const wxRect2DDouble &r, bool bSendEvent = false, unsigned int nDelay = 0)
  {
    m_viewRect.Setup(r, bSendEvent, m_bIgnoreTimer ? 0 : nDelay);
    if (m_bIgnoreTimer && m_viewRect.GetDelay())
    {
      m_viewRect.Check(true);
    }
  }

  void ZoomOut(bool bAll = false, unsigned int nDelay = 0, int nLabelHeight = 0)
  {
    SetViewRect(GetZoomOutRect(bAll, nLabelHeight),false,nDelay);
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
  bool InBatch()
  {
    return (m_nBatchCount > 0);
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
  //void SetExternalTimer(bool b = true);
  void OnTimer(wxTimerEvent &e);
  void SetOARfile(COARfile *pFile);
  void RebuildBins();
  void RebuildLabels(bool bRedraw = false);
  void UpdateLadderLabels();
  void SetPrintSettings();
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
  double GetLabelHeightExtension(int nLabelHeight = 0);
  void AdjustLabelHeightExtension(double dCurrentExtension, const wxRect &rect, int nLabelHeight = 0);

  void SetPrinting(bool b = true)
  {
    if (b != m_bPrinting)
    {
      m_bPrinting = b;
      InvalidateColors();
    }
  }
  bool IsPrinting()
  {
    return m_bPrinting;
  }
  void ResetDefaults()
  {
    if(IsPlotFrame())
    {
      SetPlotSettings();
    }
    else
    {
      SetPreviewSettings();
    }

//    if(HasToolbar())
//    {
//      m_pButtonPanel->ResetDefaults();
//    }
//    m_pMenu->ResetDefaults();
  }
  static void CheckRange(int *pn1, int *pn2, int nMinRange = 100)
  {
    // check axis range and correct if needed
    int &n1(*pn1); // readability
    int &n2(*pn2);
    if (n1 > n2)
    {
      int t = n1;
      n1 = n2;
      n2 = t;
    }
    int nDiff = n2 - n1;
    if (nDiff < nMinRange)
    {
      n1 -= ((nMinRange - nDiff) >> 1);
      n2 = n1 + nMinRange;
    }
  }
  const wxString &SampleTitle(wxString *ps);
  const wxString &TitleStrings(wxString *ps, int nPage);
  void CleanupBins();
private:
  const wxColour &_GetColour(DATA_TYPE n, unsigned int nChannel);
  static void _SetYUserRange(wxRect2DDouble *pRect);  // Y
  static void _SetXUserRange(wxRect2DDouble *pRect, bool bXBPS);  // X
  static wxColour COLOUR_RFU;

  bool _BitmapZoomSample(int nXlabelHeight, wxRect2DDouble *pRectZoom);
  void _BitmapZoomPlot(int nXlabelHeight, unsigned int nChannel);
  int _BitmapPrimerZoom(int nXScale, bool bNegCtrl);
  int _GetPlotsPerPage();
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
    if(IsPlotFrame())
    {
      UpdateSettingsPlot();
    }
    else if(IsPreview())
    {
      UpdateSettingsPreview();
    }
  }
  void UpdateGridLabels(int nLabelType = -1);
  wxColour GetMinRfuColour();
  static void ExpandRect(wxRect2DDouble *p, double dBy = 0.05); 
  // expand height and width by  (1 + dBy + dBy)  
  // for 5% in each direction or 10% total,  dBy = 0.05
  void ExtendLabelHeight(wxRect2DDouble *p, int nLabelHeight = 0);  
      // expand height to accommodate labels
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
  void _CheckBPSEvent(bool b = true)
  {
    bool bSendEvent = b && 
      m_pData->CannotSetBPS() &&
      !_HasFileBeenPrompted();
    if(bSendEvent)
    {
      _SetFileHasBeenPrompted();
      wxCommandEvent ee(CEventCannotShowBPS,GetId());
      ee.SetEventObject(this);
      GetEventHandler()->AddPendingEvent(ee);
    }
  }
  bool _CanSetBPS()
  {
    return m_pData->CanSetBPS();
  }
  static double *Copy2Points(double *pd)
  {
    size_t n = sizeof(double) << 1;
    double *pRtn = (double *)malloc(n);
    memcpy((void *) pRtn, (void *)pd,n);
    return pRtn;
  }
  const char *_GetXAxisLabel()
  {
    const char *psRtn =
      XBPSValue()
      ? "BPS (ILS ref.)"
      : "Time (seconds)";
    return psRtn;
  }
  vectorILSlines *_GetILSlines()
  {
    vectorILSlines *pRtn = XBPSValue() ? &m_vILS_XBPS : &m_vILS;
    if(pRtn->empty())
    {
      BuildILSlines();
    }
    return pRtn;
  }
  mapMinRfu *_GetMinRFUlines()
  {
    mapMinRfu *pRtn = XBPSValue() ? &m_mapMinRfuAll_XBPS : &m_mapMinRfuAll;
    return pRtn;
  }

  void _BuildPanel(
    int nMenuNumber,bool bFirst, 
    CMenuHistory *pMenuHistory);
  void BuildILSlines();


  void ShowILSlines();
  void BuildMinRfuLines();
  void CleanupMinRfuLines();
  void BuildRfuSet(std::set<double> *psetD);
  void ShowMinRfuLines();

  //  labels

  void _AppendLadderPeaks(
    unsigned int nChannel,
    const wxString &sChannelName);

  void _BuildOARlabels();
  void _BuildPeakLabels(
    const std::vector<CSamplePeak *> *pp,
    const wxColour &colour,
    const wxString &sChannelName,
    unsigned int nChannel,
    std::vector<unsigned int> &anLabel);
  void _BuildLadderPeakLabels(
    std::vector<unsigned int> &anLabel);
  static wxColour &_SETUP_PRINT_COLOR(const wxColour &color, int nPct, wxColour *pColor);
  void _SetupGridColor();
  void _BuildPLTlabels(bool bArtifactOnly = false, unsigned int nChannel = 0);
  wxString _AlleleLabel(const IOARpeak *pPeak, std::vector<unsigned int> &anLabel, bool bILS);
  wxString _ArtifactToolTip(
    const IOARpeak *pPeak, const wxString &sChannelName);
  wxString _AlleleToolTip(
    const IOARpeak *pPeak, int nChannel, const wxString &sChannelName);

  // delayed view rect update

  // void _CheckViewRect();
  static int _SamplePlotKey(DATA_TYPE nType, unsigned int nChannel, bool bNoise, bool bXBPS, bool bPrinting = false)
  {
    // create a key for mapSamplePlot
    int nRtn = ( (nChannel & 255) << 8 ) | (nType << 3) | (bPrinting ? 4 : 0) | (bNoise ? 2 : 0) | (bXBPS ? 1 : 0);
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
  nwxPlotBinSet *_GetBinsByChannel(unsigned int n);
  //void _CleanupMenu(); // remove m_pMenuItem from its menu and delete
  void _BuildMenu(int nPlotNr);

  std::vector<CLadderPeakSet *> m_vpLadderPeakSet;
  DelayedViewRect m_viewRect;

  std::set<double> m_setMinRfu;
  mapMinRfu m_mapMinRfuAll;
  mapMinRfu m_mapMinRfuAll_XBPS;

  mapSamplePlots m_mapPlotData;
  std::vector<COARpeakAny *> m_vPeakAny;
  std::vector<nwxPlotBinSet *> m_vpBinsByChannel;
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
  //wxTimer *m_pTimer;  // EXT TIMER
  unique_ptr<nwxMenuItem> m_pMenuItem;

  unsigned int m_nPlotNr;
  unsigned int m_nBatchCount;
  unsigned int m_nILScurveOffset;
  unsigned int m_nNoiseCurves;
  int m_nMenuOffset;
  //bool m_bExternalTimer;  // EXT TIMER
  bool m_bIgnoreTimer;
  bool m_bDoTimer;
  bool m_bXBPS; // used to check if event changed x axis
  bool m_bPrintAnalysis;
  bool m_bPrinting;

  // BEGIN - keep track of channels with no noise
  std::set<unsigned int> m_setNoNoiseChannel;
  mapColor m_mapColors;
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

  static std::set<wxString> g_setNoBpsPrompt;
  static bool _HasFileBeenPrompted(CPlotData *p);
  static void _SetFileHasBeenPrompted(CPlotData *p);
  static wxString _GetFileName(CPlotData *p);
  bool _HasFileBeenPrompted()
  {
    return _HasFileBeenPrompted(m_pData);
  }
  void  _SetFileHasBeenPrompted()
  {
    _SetFileHasBeenPrompted(m_pData);
  }
  void _RebuildCurves(wxCommandEvent &w, bool bShift);

public:
//  commented out 7/14/08 kill in 60 days
//  void OnChannelToggle(wxCommandEvent &);
//  void OnDataToggle(wxCommandEvent &);
//  void OnBtnILS(wxCommandEvent &);
//  void RemoveILSlines();
//  void UpdateMinRfuLines();
//  void HideCurve(DATA_TYPE nType, unsigned int nChannel);

  void OnRebuildCurves(wxCommandEvent &);
  void OnRebuildWithOAR(wxCommandEvent &);

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
  void OnViewChanging(wxPlotCtrlEvent &);
  void OnViewChanged(wxPlotCtrlEvent &);
  void OnPointSelected(wxPlotCtrlEvent &);
  void OnNoBPSPrompt(wxCommandEvent &);
  void OnLabelTypeChanged(wxCommandEvent &);
  //void OnTimerEvent(wxTimerEvent &); // EXT TIMER
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
