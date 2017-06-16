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

*  FileName: CFrameAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_FRAME_ANALYSIS_H__
#define __C_FRAME_ANALYSIS_H__

#include <wx/grid.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#include <wx/tglbtn.h>
#include <wx/string.h>
#include <wx/menu.h>
#include "nwx/nsstd.h"
#include "nwx/nwxColorUtil.h"
#include "nwx/PersistentSize.h"
#include "CMDIFrame.h"
#include "CKitColors.h"
#include "CParmOsiris.h"
#include "CComboLabels.h"
#include "COARsampleSort.h"
#include "CPanelHistoryMenu.h"
#include "CPanelSampleAlertNotebook.h"
#include "CGridAnalysis.h"
#include "CMenuEdit.h"
#include "CMenuAnalysis.h"
#include "CMenuBarAnalysis.h"
#include "LABEL_TYPES.h"
#include "Platform.h"


DECLARE_EVENT_TYPE(CEventRepaint,-1)
DECLARE_EVENT_TYPE(CEventRestoreScroll,-1)
// 6/8/15 CEventRestoreScroll - when repainting the grid
//   the scrolling position needs to be saved and restored
//   the restore needs to be in a queued event, otherwise
//   the grid will be scrolled to top/left

class mainFrame;
class CMenuSort;
class CDialogCMF;
class CPanelPlotPreview;
class CGridLocusPeaks;
class CPanelStatus;
class COARfile;
class COARsample;
class CDialogAnalysis;
class CPanelAlerts;
class CXSLExportFileType;
class COARpeakAny;
class CFrameSample;
/*
typedef enum
{
  OAR_ERROR,
  OAR_VERSION_1_0,
  OAR_VERSION_1_1
} OAR_VERSION;
*/

#define EDIT_MENU 1

//**************************************************** CFrameAnalysis


class CFrameAnalysis : public CMDIFrame
{
public:
  static const int FIRST_LOCUS_COLUMN;
  static const int ILS_COLUMN;
  static const int CHANNEL_ALERT_COLUMN;
  static const int STATUS_COLUMN;
  CFrameAnalysis(mainFrame *parent, 
    wxSize sz, const wxString &sFileName);
  CFrameAnalysis(mainFrame *parent, 
    wxSize sz, COARfile *pOARfile);
  virtual ~CFrameAnalysis();
  virtual bool TransferDataToWindow();

  virtual wxMenu *GetMenu();
  virtual wxMenu *GetTableMenu();
  virtual wxMenu *GetGraphMenu();
  virtual int GetType();
  virtual bool SetToolbarMenuLabel(bool bShow, bool bPlural = false);
  virtual bool MenuEvent(wxCommandEvent &e);
  virtual void OnTimer(wxTimerEvent &);
  virtual wxString GetFileName();
  void SetLabelType(int n);
  LABEL_PLOT_TYPE GetPlotLabelType();
  wxWindow *GetInfoPanel();
  CPanelPlotPreview *GetGraphPanel()
  {
    return m_pPanelPlotPreview;
  }

  const wxDateTime *GetSelectedTime()
  {    
    const wxDateTime *pRtn = 
      (m_pButtonHistory == NULL)
      ? NULL
      : m_pButtonHistory->GetSelectedTime();
    return pRtn;
  }
  bool IsToolbarShown()
  {
    return m_pPanelToolbar->IsShown();
  }
  void ShowToolbar(bool bShow)
  {
    if(bShow != IsToolbarShown())
    {
      m_pPanelToolbar->Show(bShow);
      Layout();
      SetToolbarMenuLabel(!bShow);
    }
  }
  void ToggleToolbar();
  bool CanSaveAs() const
  {
    return _XmlFile();
  }
  bool CanSave() const
  {
    bool bRtn = false;
    if(CanSaveAs())
    {
      bRtn = m_pOARfile->CanSave();
    }
    return bRtn;
  }
  bool FileError()
  {
    return m_bFileError;
  }
  void RepaintData()
  {
    //
    //  need to defer repaint because it can destroy
    //  a panel whose event is currently being processed
    //
    wxCommandEvent ee(CEventRepaint,GetId());
    ee.SetEventObject(this);
    GetEventHandler()->AddPendingEvent(ee);
  }
  void RepaintAllData(const COARsample *p);
  bool FileEmpty();
  bool SaveFile();
  bool SaveFileAs();
  bool PromptSaveFileNow(const wxString &sMessage);
  bool FileNeedsAttention(bool bCMF = true, bool bShowMessage = true); // future
  virtual void UpdateFileMenu();
  bool ExportCMF();
  void SetupTitle();
  virtual void UpdateHistory();
  virtual void UpdateLadderLabels();
  virtual void CheckFileModification();
  virtual bool IsEdited();
  bool CheckSaveOnCloseFile();
  bool SelectSample(const wxString &sSample);
  bool SelectSampleType(const wxString &sSample, int nType);
  bool SelectSampleLocus(const wxString &sSample, const wxString &sLocus);
  size_t GetSampleRow(const wxString &sSample);
#if HAS_STATUS_BAR
  virtual void UpdateStatusBar();
#endif
  void DoReviewSample(int nReviewType,COARsample *pSample);
  void DoAcceptSample(int nReviewType,COARsample *pSample);
  void DoReviewLocus(COARsample *pSample, COARlocus *pLocus);
  void DoAcceptLocus(COARsample *pSample, COARlocus *pLocus);
#if 0
  void DoEditLocus(COARsample *pSample, 
    COARlocus *pLocus, 
    wxWindow *pParent = NULL);
#endif
  bool CheckIfHistoryOK();
  void CheckSaveStatus();
  void EditPeak(COARpeakAny *, COARsample *, CMDIFrame * = NULL);
  void ShowSampleFrame(COARsample *pSample, const wxString &sLocus, int nAlertType, int nEventID = -1);
private:
  CXSLExportFileType *GetFileTypeByID(int nID);

  bool _IsPreviewShowing()
  {
    return m_pSplitterTop->IsSplit();
  }
  bool _CheckIfTampered(COARfile *pOAR = NULL);
    // return true if tampered and rejected by user
  void _SetPreviewMenu(wxMenu *p)
  {
    if(m_pMenuBar != NULL) {m_pMenuBar->SetPlotMenu(p);}
  }
  void _EnablePreview();
  void _DisablePreview();
  void _HidePreview();
  void _ShowPreview();
  wxString _GetGraphicFileName(int nRow = -1,bool bMessage = false);
  wxString _GetLocusFromColumn(int nCol = -1);
  bool _SaveOERFile(const wxString &sFileName);
  void _Build();
  bool _IsCellEdited(int nRow,int nCol);
  void _SetupLocusColumn
    (wxCommandEvent &e, COARsample **ppSample, COARlocus **ppLocus);
  void _OnAcceptLocus(wxCommandEvent &e);
  void _OnReviewLocus(wxCommandEvent &e);
  void _OnRepaint(wxCommandEvent &);
  void _OnRestoreScroll(wxCommandEvent &);
  void _OnReviewSample(int nReviewType);
  void _OnAcceptSample(int nReviewType);
  void _OnEnableMultiple();
  void _OnEnableSample();
  void _OnReAnalyze();
  void _OnDeleteDisabled();
#if 0
  void _OnEditLocus(COARsample *pSample, int nCol);
  void _OnEditAlerts(COARsample *pSample, int nCol);
#endif
  int _GetPreviewColumn();
  void _UpdatePreview();
  void _SetupLocusPanel(COARsample *pSample, int nChannel, const wxString &sLocusName);
  void _LayoutInfo(wxWindow *pChild);
  void _SetupStatusPanel(COARsample *pSample);
  void _DestroyStatusPanel();
  void _HideStatusPanel();
  void _DestroyLocusPanel();
  bool _CheckStatusPanel(COARsample *pSample,bool bDestroyIfFalse = false);
  bool _RowNeedsAttention(int nRow);
  void ShowFileSaveError(const wxString &s);
  void _LayoutAll();
  bool _XmlFile() const
  {
    return (m_pOARfile != NULL);
  }
  void CheckSelection(bool bForce = false)
  {
#if 1
    if(_XmlFile())
    {
      CheckSelectionXML(bForce);
    }
#else
    if(_TextFile())
    {
      CheckSelectionText(bForce);
    }
    else if(_XmlFile())
    {
      CheckSelectionXML(bForce);
    }
#endif
  }
  bool _IsControlColumn(int nCol)
  {
    int nLast = m_pGrid->GetNumberCols() - 1;
    return (nCol >= nLast);
  }
  bool _IsControlColumn()
  {
    return _IsControlColumn(m_nLastColSelect);
  }
  bool _IsLocusColumn(int nCol)
  {
    return (nCol >= FIRST_LOCUS_COLUMN) && (!_IsControlColumn(nCol));
  }
  bool _IsLocusColumn()
  {
    return _IsLocusColumn(m_nLastColSelect);
  }
  bool _IsChannelColumn(int nCol)
  {
    return nCol == 0 || nCol == 1;
  }
  bool _IsChannelColumn()
  {
    return _IsChannelColumn(m_nLastColSelect);
  }
  void SetFileNameLabel(const wxString &sFileName)
  {
    m_pLabelFile->SetValue(sFileName);
    m_pLabelFile->SetInsertionPointEnd();
  }
  static int _ColToType(int nCol)
  {
    int nType = -1;
    if(nCol == ILS_COLUMN)
    {
      nType = SA_NDX_ILS;
    }
    else if(nCol == CHANNEL_ALERT_COLUMN)
    {
      nType = SA_NDX_CHANNEL;
    }
    else
    {
      nType = SA_NDX_SAMPLE;
    }
    return nType;
  }
  static int _TypeToCol(int nType)
  {
    int nCol = -1;
    if(nType == SA_NDX_ILS)
    {
      nCol = ILS_COLUMN;
    }
    else if(nType == SA_NDX_CHANNEL)
    {
      nCol = CHANNEL_ALERT_COLUMN;
    }
    return nCol;
  }
#if 0
  void _CleanupMenus()
  {
    if(m_pMenu != NULL)
    {
      delete m_pMenu;
      m_pMenu = NULL;
    }
  }
#endif
  void _UpdateMenu();
  void _UpdateHistoryMenu(int nRow, int nCol,bool bEnabled = true);
  void _UpdateHistoryMenu(bool bEnabled)
  {
    _UpdateHistoryMenu(m_nLastRowSelect, m_nLastColSelect,bEnabled);
  }
  void _CleanupCMF();
  void _CleanupMessageBook();

  void SetFileNameLabel(COARsample *pSample);
  void SelectRowCol(int nRow, int nCol);
  void SelectRow(int nRow);
  void ShowGraphicByRow(int nRow = -1);
  void ShowStatusText(bool bIncludeStatusPanel = true );
  wxString m_sPath;
  COARsampleSort m_SampleSort;

  typedef std::map<const COARsample *, CFrameSample *> MapSampleFrame;
  typedef std::pair<const COARsample *, CFrameSample *> PairSampleFrame;
  MapSampleFrame m_mapSamples;
  
  CFrameSample *_FindSampleFrame(const COARsample *pSample)
  {
    MapSampleFrame::iterator itr =
      m_mapSamples.find(pSample);
    return (itr == m_mapSamples.end()) ? NULL : itr->second;
  }
  CFrameSample *_FindSampleFrameByRow(int nRow = -1)
  {
    COARsample *pSample = _FindSampleByRow(nRow);
    return _FindSampleFrame(pSample);
  }
  COARsample *_FindSampleByRow(int nRow = -1)
  {
    if(nRow < 0) { nRow = m_pGrid->GetGridCursorRow(); }
    COARsample *pSample = m_SampleSort.GetSample((size_t) nRow);
    return pSample;
  }
  void _AddSample(const COARsample *ps,CFrameSample *pf)
  {
    m_mapSamples.insert(
      PairSampleFrame(ps,pf));
  }
  bool _DestroySamples();
  bool _CheckSamples();
public:
  void RemoveSample(COARsample *pSample,CFrameSample *pf)
  {
    MapSampleFrame::iterator itr = m_mapSamples.find(pSample);
    if(itr != m_mapSamples.end() &&
       itr->first == pSample && 
       itr->second == pf)
    {
      m_mapSamples.erase(itr);
    }
#ifdef __WXDEBUG__
    else
    {
      wxASSERT_MSG(0,wxT("Problem in CFrameAnalysis::RemoveSample"));
    }
#endif
  }
private:

  COARfile *m_pOARfile;
  CMenuAnalysis *m_pMenu;
  CMenuBarAnalysis *m_pMenuBar;

  CGridAnalysis *m_pGrid;
  CGridLocusPeaks *m_pGridLocus;
  CPanelStatus *m_pPanelStatus;
  CPanelAlerts *m_pAlerts;
  wxTextCtrl *m_pText;
  wxTextCtrl *m_pLabelFile;
  wxSplitterWindow *m_pSplitter;
  wxSplitterWindow *m_pSplitterTop;
  wxSplitterWindow *m_pSplitterLocusNotices;
  wxPanel *m_pPanelToolbar;
  wxPanel *m_pPanelText;
  wxPanel *m_pPanelGrid;
  wxPanel *m_pPanelInfo;
  CPanelPlotPreview *m_pPanelPlotPreview;
  wxButton *m_pButtonGraphic;
  wxButton *m_pButtonSample;
  wxButton *m_pButtonParms;
#if EDIT_MENU
  nwxButtonMenu *m_pButtonEdit;
#else
  wxButton *m_pButtonEdit;
#endif
  wxToggleButton *m_pTogglePreview;
  wxBoxSizer *m_pSizerInfo;
  CComboLabels *m_pComboCellType;
  CComboLabelsName *m_pComboName;
  CPanelHistoryMenu *m_pButtonHistory;
  CDialogAnalysis *m_pDlgAnalysis;

  CDialogCMF *m_pCMF;

  int m_nLastColSelect;
  int m_nLastRowSelect;
  int m_nEntireRowSelected;
  int m_nNoTimer;
  bool m_bFileError;


/*
  void SetCellStyle(int nRow, int nCol,
    bool bBold, bool bItal = false, bool bHasAlerts = false)
  {
    m_pGrid->SetCellStyle(nRow,nCol,bBold,bItal,bHasAlerts);
  }
  void SetCellStyle(int nRow, int nCol,GRID_FLAG nFlag)
  {
    m_pGrid->SetCellStyle(nRow,nCol,nFlag);
  }
*/
 
  void ResetSelection()
  {
    if(m_nEntireRowSelected >= 0)
    {
      SelectRow(m_nEntireRowSelected);
    }
    else
    {
      m_nLastColSelect = -1;
      CheckSelection();
    }
  }
  const CParmOsiris &GetFileParameters() const
  {
    if(_XmlFile())
    {
      return m_pOARfile->GetParameters();
    }
#if 0
    else if(_TextFile())
    {
      return m_pFile->GetParameters();
    }
#endif
    else
    {
      const CParmOsiris *p(NULL);
      return *p;
    }
  }
  void CheckSelectionXML(bool bForceUpdate = false);
  bool LoadFile(const wxString &sFileName);
  bool DisplayFile();
  void RepaintGridXML();
  void _UpdatePreviewLabelType(int n);

public:
  void OnContextMenu(wxContextMenuEvent &e);
  void OnClose(wxCloseEvent &e);
  void OnSave(wxCommandEvent &);
  void OnLabelNameChangedMenu(wxCommandEvent &);
  void OnLabelNameChanged(wxCommandEvent &);
  void OnLabelTypeChanged(wxCommandEvent &);
  void OnLabelTypeChangedMenu(wxCommandEvent &);
  void OnClickCell(wxGridEvent &);
  void OnRightClickCell(wxGridEvent &);

  void OnShowGraphic(wxCommandEvent &);
  void OnShowSample(wxCommandEvent &);
  void OnShowSampleAndGraphic(wxCommandEvent &);
  void OnShowDetails(wxCommandEvent &);
  void OnGridGraph(wxGridEvent &);
  void OnGridCellGraph(wxGridEvent &);
  void OnLabelClick(wxGridEvent &);
  void OnEditMenu(wxGridEvent &);
  void OnEditFromGrid(wxGridEvent &);
  void OnTogglePreview(wxCommandEvent &);
  void OnHistoryUpdate(wxCommandEvent &);
  void OnChangeAlertView(wxCommandEvent &);
  void OnCheckSplitter(wxCommandEvent &);
  void OnHistoryView(wxCommandEvent &);
  void OnSortGrid(wxCommandEvent &);
  void OnExportCMF(wxCommandEvent &);
  void OnUserExport(wxCommandEvent &);
  DECLARE_PERSISTENT_SIZE  
  DECLARE_EVENT_TABLE()
  DECLARE_ABSTRACT_CLASS(CFrameAnalysis)

};

#endif
