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
*  FileName: mainFrame.h
*  Author:   Douglas Hoffman
*
*    For Windows this is the MDI Parent window
*    For Mac OSX, this is an event handler that keeps track
*      of the other windows
*    If GTK is implemented, it will be a window with a menu that
*      appears when there are no other windows.
*
*/
#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include "Platform.h"

#include <wx/log.h>
#include "nwx/stdb.h"
#include <map>
#include <set>
#include <list>
#include "nwx/stde.h"

#include "nwx/nsstd.h"

#if !defined( __NO_MDI__)
#include <wx/mdi.h>
#else
#include <wx/frame.h>
#endif

#include <wx/event.h>
//#include <wx/timer.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include "nwx/nwxString.h"
#include "CDialogAnalysis.h"
#include "CParmOsiris.h"
#include "CDialogMRU.h"
#include "CAllLoci.h"
#include "nwx/nwxDialogLog.h"
#include "CMDIfileManager.h"
#include "CVolumes.h"
#include "CDialogMaxLadderLabel.h"
#include "CDialogPromptNewerFile.h"
#include "CMenuBar.h"
#include "wxIDS.h"
#include "wxXsl/wxXslObject.h"
#if mainFrameIsWindow
#include "nwx/PersistentSize.h"
#endif

class COARfile;
class CFrameAnalysis;
class CFrameRunAnalysis;
class CFramePlot;
class CDialogEditGridColours;
class wxColourDialog;
class wxMenu;
class CDialogVolumes;




#if DRAG_DROP_FILES
DECLARE_EVENT_TYPE(CEventDragDropDelay,-1)
class CFileDropTarget;
#endif

typedef enum
{
  TYPE_REPORT = 0, TYPE_PLOT = 1, TYPE_BATCH = 2, TYPE_ARCHIVE = 3
} OSIRIS_FILE_TYPE;

class mainFrame : public mainFrameSuper
{
public:
#if mainFrameIsWindow
  DECLARE_ABSTRACT_CLASS(mainFrame)
#endif
  static int GetCount()
  {
    return g_mainFrameCount;
  }
  mainFrame();
  virtual ~mainFrame();
  virtual bool Startup(bool bHasArgs= false);
  void OpenFileCheckNewer(const wxString &sFileName);
  void OpenFile(
    const wxString &sFileName, 
    const wxString &sLocus = wxEmptyString, 
    COARfile *pFile = NULL,
    bool bNoChange = false);
  bool ShowColourDialog(wxColour *pColour);
  bool DoClose();

  void OnMenuOpen(wxMenuEvent &e);
  void OnMenuClose(wxMenuEvent &);

#if mainFrameIsWindow
  void OnClose(wxCloseEvent &e);
#endif
  void OnRecentFiles(wxCommandEvent &);
  void OnOpen(wxCommandEvent &);
  void OnOpenPlot(wxCommandEvent &);
  void OnOpenBatch(wxCommandEvent &);
  void OnOpenArchive(wxCommandEvent &);
  void OnShowLog(wxCommandEvent &);
  void OnAnalyze(wxCommandEvent &);
  void OnLabSettings(wxCommandEvent &);
  void OnAccessSiteSettings(wxCommandEvent &);
  void OnShowSiteSettings(wxCommandEvent &);
  bool SetupSiteSettings();
  void OnArtifactLabels(wxCommandEvent &);
  void OnPinger(wxCommandEvent &);
  void OnExportSettings(wxCommandEvent &);
  void OnEditGridColours(wxCommandEvent &);
  void OnTimer(wxTimerEvent &);
  void OnAbout(wxCommandEvent &);
  void OnCheckForUpdates(wxCommandEvent &);
  void OnHelp(wxCommandEvent &);
  void OnContactUs(wxCommandEvent &);
  void OnMaxLadderLabels(wxCommandEvent &);
  void OnMenu(wxCommandEvent &e);
#ifdef __WINDOW_LIST__
  void OnWindowMenu(wxCommandEvent &e);
#endif
#if DRAG_DROP_FILES
  void OnDropFiles(wxCommandEvent &e);
  void DropFiles();
#endif
  void OpenFiles(const wxArrayString &filenames);
  static bool ContainsEnd(const wxString &sFileName);
  bool FileInProgress(
    const wxString &sFileName, bool bMessage = false);
  void ErrorMessage(const wxString &s);
  void FileErrorMessage(const wxString &sFileName);
  void FileEmptyMessage(const wxString &sFileName);
  wxWindow *DialogParent()
  {
#if mainFrameIsWindow
    return this;
#else
    return NULL;
#endif
  }
  bool FindWindow(CMDIFrame *p)
  {
    bool b = m_MDImgr.FindWindow(p);
    return b;
  }
  void InsertWindow(CMDIFrame *p, COARfile *pFile = NULL)
  {
    // called from CMDIFrame constructor
    m_MDImgr.InsertWindow(p,pFile);
  }
  void RemoveWindow(CMDIFrame *p);
  void ClearStatusText()
  {
#ifndef __WXMAC__
    SetStatusText("");
#endif
  }
  void KillOARfile(COARfile *pFile)
  {
    // a problem occurred with an OAR file
    // remove its use
    m_MDImgr.KillOARfile(pFile);
  }

  void TileTwoWindows(CMDIFrame *pLeft, CMDIFrame *pRight);
  CMDIFrame *FindWindowByName(const wxString &sPath, bool bRaise = false)
  {
    CMDIFrame *pRtn = m_MDImgr.FindWindowByName(sPath,bRaise);
    return pRtn;
  }
  void UpdateHistory(COARfile *pFile)
  {
    m_MDImgr.UpdateHistory(pFile);
  }
  COARfile *FindOARfile(const wxString &sName)
  {
    return m_MDImgr.FindOARfile(sName);
  }
  void UpdateSamplePlot(COARfile *pFile, const wxString &sSampleFileName)
  {
    m_MDImgr.UpdateSamplePlot(pFile,sSampleFileName);
  }
  void DiscardChanges(COARfile *pFile)
  {
    m_MDImgr.DiscardChanges(pFile);
  }
  void UpdatePlots(COARfile *pFile)
  {
    m_MDImgr.UpdatePlots(pFile);
  }
  void UpdateOARfile(COARfile *pFile)
  {
    m_MDImgr.UpdateOARfile(pFile,true);
  }
  CFrameAnalysis *FindAnalysisFrame(COARfile *pFile)
  {
    return m_MDImgr.FindAnalysisFrame(pFile);
  }
  bool ReAnalyze(
    CFrameRunAnalysis *pPrev,
    const CVolume &vol,
    const CParmOsiris &parmNew);
  bool ReAnalyzeSamples(
    const CVolume &vol,
    const CParmOsiris &parmNew);
  static bool FileExtensionOK(const wxString &s);
  void AddToMRU(const wxString &sFileName);
  void RemoveFromMRU(const wxString &sFileName);
  void CheckActiveFrame()
#ifdef __NO_MDI__
  {}
#else
  ;
#endif
#ifdef __WINDOW_LIST__
  void ActivateFromWindowMenu(int nID)
  {
    m_MDImgr.ActivateFromWindowMenu(nID);
  }
  void CheckUpdateWindowMenu()
  {
    if(m_pLastActive != NULL)
    {
      m_MDImgr.CheckUpdateWindowMenu(m_pLastActive);
    }
  }
  void InvalidateWindowMenu()
  {
    m_MDImgr.InvalidateWindowMenu();
  }
#endif
  CMDIFrame *GetActiveFrame()
  {
    return m_pLastActive;
  }
  void SetActiveFrame(CMDIFrame *p)
  {
    m_pLastActive = p;
#ifdef __WINDOW_LIST__
#ifdef __WXDEBUG__
    mainApp::LogMessage(wxS("MoveToTop"));
#endif
    CheckUpdateWindowMenu();
    m_MDImgr.MoveToTop(m_pLastActive);
#endif
#ifdef __WXDEBUG__
    _LogActiveFrame();
#endif
  }
  void KillActiveFrame(CMDIFrame *p)
  {
    if(p == m_pLastActive)
    {
      SetActiveFrame(NULL);
    }
  }

  bool CheckMaxFrames(bool bShowError = true);
#ifdef MANUALLY_PLACE_FRAMES
  void PlaceFrame(CMDIFrame *pWin);
#endif

#if mainFrameIsWindow
  static const wxSize &Size90();
  static const wxSize &Size80();
  static const wxSize &mainFrameSize();
  static const wxPoint &mainFramePos();
#endif

private:
#ifdef __WXDEBUG__
  void _LogActiveFrame();
#endif
  void _SetupCommonMenuBar();

  int _PromptNewerFile(const wxString &sFile, const wxString &sNewerFile)
  {
    CDialogPromptNewerFile fn(DialogParent(),sFile,sNewerFile);
    int n = fn.ShowModal();
    return n;
  }

  // SET_FILE_NAMES is typedef'd in CDialogPromptNewerFile.h

  int _PromptNewerFiles(const SET_FILE_NAMES &setNames, const wxString &sNewest)
  {
    CDialogPromptNewerFile fn(DialogParent(),setNames,sNewest);
    int n = fn.ShowModal();
    return n;
  }

  bool _CheckForNewerFiles(
    const wxArrayString &filenames,
    wxArrayString *psNewFilenames);
  bool _VerifyClose();
  void OpenArchiveFile(const wxString &sFileName);
  void OpenAnalysisFile(const wxString &sFileName);
  void OpenAnalysisFile(COARfile *pFile);
  void _CheckAnalysisFile(CFrameAnalysis *pWin);
  CFramePlot *OpenGraphicFile(
    const wxString &sFileName,
    const wxString &sLocus = wxEmptyString,
    COARfile *pFile = NULL);
  bool OpenBatchFile(const wxString &sFileName);
  void OpenFileDialog(OSIRIS_FILE_TYPE);
#if DRAG_DROP_FILES
  void _CheckDragDropQueue();
#endif

  wxSize GetChildSize(int nPct = 90);
#if mainFrameIsWindow
  static const wxPoint &GetStartPosition();
  static void SetupSize();
  static wxSize g_size80;
  static wxSize g_size90;
  static wxPoint g_point5;
  static wxPoint g_point50;
  static wxPoint g_point100;
#endif


  static const char * const NOFIND; 
  static const size_t MAX_FRAMES;
  static const int MRU_AT_STARTUP;
  static int g_mainFrameCount;

  // message if lab settings file not found
  
  CAllLoci *GetAllLoci()
  {
    if(m_pAllLoci == NULL)
    {
      m_pAllLoci = new CAllLoci();
      wxString sLadderFile(mainApp::GetConfig()->GetLadderFileName());
      m_pAllLoci->LoadFile(sLadderFile);
    }
    return m_pAllLoci;
  }
  void SetupTimer()
  {
    if(m_pTimer == NULL)
    {
      m_pTimer = new wxTimer(this,(int)IDtimer);
      m_pTimer->Start(250,false);
    }
  }
  CFrameAnalysis *GetAnalysisFrame();
  wxXslObject XXX;
    // presence of this object 
    // causes initialization and cleanup of libxml2
    // and libxslt to happen only once
  CMDIfileManager m_MDImgr;
  wxFileDialog *m_pDialogOpen;
  CDialogMRU *m_pDialogMRU;
//  CDialogVolumes *m_pVolumes;
//  CDialogAnalysis *m_pDlgAnalysis;
  nwxDialogLog *m_pDialogErrorLog;
  wxTimer *m_pTimer;
  CDialogEditGridColours *m_pColourEditDialog;
#if HAS_CUSTOM_COLORS
  wxColourDialog *m_pDialogColour;
#endif
#if DRAG_DROP_FILES
  CFileDropTarget *m_pDropTarget;
#endif
  CAllLoci *m_pAllLoci;
  int m_nTimerCount;
  CMDIFrame *m_pLastActive; // should be set in SetActiveFrame() only so a breakpoint can be set
#if DRAG_DROP_FILES
  list<wxString> m_lsDragDropQueue;
#endif
#ifdef MANUALLY_PLACE_FRAMES
  int m_nFrameSpace;
#endif
  bool m_bDoCloseCalled;
  static CMDIFrame *INIT_LAST_ACTIVE;
#if mainFrameIsWindow
  DECLARE_PERSISTENT_SIZE_POSITION
#endif
  DECLARE_EVENT_TABLE()
};


#endif
