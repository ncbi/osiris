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

*  FileName: mainFrame.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include <wx/log.h>
#include "nwx/stdb.h"
#include <map>
#include <set>
#include <list>
#include "nwx/stde.h"

#include "nwx/nsstd.h"
#include <wx/mdi.h>
#include <wx/timer.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include "nwx/nwxString.h"
#include "CDialogAnalysis.h"
#include "CParmOsiris.h"
#include "CDialogMRU.h"
#include "CKitColors.h"
#include "CAllLoci.h"
#include "nwx/nwxDialogLog.h"
#include "CMDIfileManager.h"
#include "Platform.h"
#include "CVolumes.h"
#include "CDialogMaxLadderLabel.h"
#include "CDialogPromptNewerFile.h"
#include "CMenuBar.h"
#include "wxXsl/wxXslObject.h"

DECLARE_EVENT_TYPE(CEventKillWindow,-1)
DECLARE_EVENT_TYPE(CEventDragDropDelay,-1)

class CFileDropTarget;
class COARfile;
class CFrameAnalysis;
class CFrameRunAnalysis;
class CFramePlot;
class CDialogEditGridColours;
class wxColourDialog;
class wxMenu;
class CDialogVolumes;

typedef enum
{
  TYPE_REPORT = 0, TYPE_PLOT = 1, TYPE_BATCH = 2
} OSIRIS_FILE_TYPE;

class mainFrame : public wxMDIParentFrame
{
public:
  mainFrame();
  virtual ~mainFrame();
  virtual bool ShowWindow(bool bHasArgs= false);
#ifdef __WXMAC__
  virtual bool Show(bool show = false); // need to intercept on macintosh
#endif
  void OpenFileCheckNewer(const wxString &sFileName);
  void OpenFile(
    const wxString &sFileName, 
    const wxString &sLocus = wxEmptyString, 
    COARfile *pFile = NULL);
  bool ShowColourDialog(wxColour *pColour);
  void OnRecentFiles(wxCommandEvent &);
  void OnQuit(wxCommandEvent &);
  void OnDoClose(wxCommandEvent &);
  void OnOpen(wxCommandEvent &);
  void OnOpenPlot(wxCommandEvent &);
  void OnOpenBatch(wxCommandEvent &);
  void OnShowLog(wxCommandEvent &);
  void OnAnalyze(wxCommandEvent &);
  void OnClose(wxCloseEvent &e);
  void OnLabSettings(wxCommandEvent &);
  void OnExportSettings(wxCommandEvent &);
  void OnEditGridColours(wxCommandEvent &);
  void OnTimer(wxTimerEvent &);
  void OnAbout(wxCommandEvent &);
  void OnCheckForUpdates(wxCommandEvent &);
  void OnHelp(wxCommandEvent &);
  void OnContactUs(wxCommandEvent &);
  void OnMaxLadderLabels(wxCommandEvent &);
  void OnSave(wxCommandEvent &);
  void OnMenu(wxCommandEvent &e);
  void OnMenuOpen(wxMenuEvent &);
  void OnMenuClose(wxMenuEvent &e);
  void OnKillWindow(wxCommandEvent &e);
  void OnDropFiles(wxCommandEvent &e);
  void KillWindow(wxWindow *pFrame);
  void DropFiles();
  void OpenFiles(const wxArrayString &filenames);
  static bool ContainsEnd(const wxString &sFileName);
  bool FileInProgress(
    const wxString &sFileName, bool bMessage = false);
#ifdef __WXMAC__
  wxPoint SelectPosition();
#endif
  void ErrorMessage(const wxString &s);
  void FileErrorMessage(const wxString &sFileName);
  void FileEmptyMessage(const wxString &sFileName);
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
  void RemoveWindow(CMDIFrame *p)
  {
    // called from CMDIFrame::Destroy()

    m_MDImgr.RemoveWindow(p);
    if(p == m_pLastActive)
    {
      m_pLastActive = NULL;
    }
    if(!m_MDImgr.Size())
    {
      ClearStatusText();
    }
  }
  void ClearStatusText()
  {
#ifndef __WXMAC__
    SetStatusText(_T(""));
#endif
  }
  void KillOARfile(COARfile *pFile)
  {
    // a problem occurred with an OAR file
    // remove its use
    m_MDImgr.KillOARfile(pFile);
  }
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
  void UpdateSamplePlot(COARfile *pFile, const wxString &sSampleName)
  {
    m_MDImgr.UpdateSamplePlot(pFile,sSampleName);
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
  CKitColors *GetKitColors()
  {
    return &m_kitColors;
  }
  wxMenu *GetLastMenuShown()
  {
    return m_pLastMenuShown;
  }
  void SetLastMenuShown(wxMenu *p)
  {
    m_pLastMenuShown = p;
  }
  bool ReAnalyze(
    CFrameRunAnalysis *pPrev,
    const CVolume &vol,
    const CParmOsiris &parmNew);
  bool ReAnalyzeSamples(
    const CVolume &vol,
    const CParmOsiris &parmNew);
  static bool FileExtensionOK(const wxString &s);
  void CheckSaveStatus();
  void AddToMRU(const wxString &sFileName);
  void RemoveFromMRU(const wxString &sFileName);
  void CheckActiveFrame();

//  use only if desperate
//  CMenuBar *GetMenuBar()
//  {
//    return m_pMenuBar;
//  }

  CXSLExportFileType *GetFileTypeByID(int nID)
  {
    return m_pMenuBar->GetFileTypeByID(nID);
  }

  void DisableMenus();
  void SetupMenus();
  bool CheckMaxFrames(bool bShowError = true);
  static const wxSize &Size90();
  static const wxSize &Size80();
private:
  int _PromptNewerFile(const wxString &sFile, const wxString &sNewerFile)
  {
    CDialogPromptNewerFile fn(this,sFile,sNewerFile);
    int n = fn.ShowModal();
    return n;
  }

  // SET_FILE_NAMES is typedef'd in CDialogPromptNewerFile.h

  int _PromptNewerFiles(const SET_FILE_NAMES &setNames, const wxString &sNewest)
  {
    CDialogPromptNewerFile fn(this,setNames,sNewest);
    int n = fn.ShowModal();
    return n;
  }

  bool _CheckForNewerFiles(
    const wxArrayString &filenames,
    wxArrayString *psNewFilenames);
  bool _VerifyClose();
  void OpenAnalysisFile(const wxString &sFileName);
  void OpenAnalysisFile(COARfile *pFile);
  void _CheckAnalysisFile(CFrameAnalysis *pWin);
  CFramePlot *OpenGraphicFile(
    const wxString &sFileName,
    const wxString &sLocus = wxEmptyString,
    COARfile *pFile = NULL);
  bool OpenBatchFile(const wxString &sFileName);
  void OpenFileDialog(OSIRIS_FILE_TYPE);
  void _CheckDragDropQueue();
  wxSize GetChildSize(int nPct = 90);
  static const wxPoint &Point5();
  static void SetupSize();
  static wxSize g_size80;
  static wxSize g_size90;
  static wxPoint g_point5;
  static const char * const NOFIND; 
  static const size_t MAX_FRAMES;
  static const int MRU_NO_WARNING;

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
      m_pTimer = new wxTimer(this,IDtimer);
      m_pTimer->Start(250,false);
    }
  }
  CFrameAnalysis *GetAnalysisFrame();
  wxXslObject XXX; 
    // presence of this object 
    // causes initialization and cleanup of libxml2
    // and libxslt to happen only once
  CMDIfileManager m_MDImgr;
  CKitColors m_kitColors;
  wxFileDialog *m_pDialogOpen;
  CDialogMRU *m_pDialogMRU;
//  CDialogVolumes *m_pVolumes;
//  CDialogAnalysis *m_pDlgAnalysis;
  nwxDialogLog *m_pDialogErrorLog;
  wxTimer *m_pTimer;
  wxMenu *m_pLastMenuShown;
  CMenuBar *m_pMenuBar;
  CDialogEditGridColours *m_pColourEditDialog;
#if HAS_CUSTOM_COLORS
  wxColourDialog *m_pDialogColour;
#endif
  CFileDropTarget *m_pDropTarget;
  CAllLoci *m_pAllLoci;
  int m_nTimerCount;
  CMDIFrame *m_pLastActive;
  list<wxString> m_lsDragDropQueue;

  static CMDIFrame *INIT_LAST_ACTIVE;
  DECLARE_EVENT_TABLE()
};


#endif
