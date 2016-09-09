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
*  FileName: CFrameSample.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_FRAME_SAMPLE_H__
#define __C_FRAME_SAMPLE_H__

#include "CMDIFrame.h"
#include "CHistoryTime.h"
#include "nwx/PersistentSize.h"
#include "CMenuSample.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
class CNotebookEditSample;
class COARsample;
class COARfile;
class CHistoryTime;
class CFrameAnalysis;

class CFrameSample : public CMDIFrame
{
public:
  CFrameSample(
    CFrameAnalysis *pCreator,
    mainFrame *parent,
    const wxSize &sz, 
    COARfile *pFile,
    COARsample *pSample,
    int nSelectAlerts = -1,
    const wxString &sSelectLocus = wxEmptyString);
  virtual ~CFrameSample();
  //virtual bool Show(bool show = true);
  void SelectLocus(const wxString &sLocus);
  void Select(int n);
  wxString GetDisplayedSampleName();
  void SelectAlerts(int nAlertType);
  virtual int GetType();
  virtual bool TransferDataToWindow();
  virtual void OnTimer(wxTimerEvent &);
  virtual bool MenuEvent(wxCommandEvent &e);
  virtual wxMenu *GetMenu();
  virtual bool Destroy();
  bool IsModified(int *pnFirstPage = NULL);

  void OnClose(wxCloseEvent &e);
  void UpdateMenu();
  void SetupTitle(bool bForce = false);
  void InitiateRepaintData();
  void RepaintData();
  const wxString &GetUserID();
  void SaveUserID();
  bool CanOverrideUserID();
  void UpdateSizeHack(bool bForce = true);
  void SetupMenuItems();
  static void FormatCloseWarning(const std::vector<wxString> &vsSamples, wxString *ps);
  bool CheckApplyNoNotes(int nPanel = -1);
  DECLARE_PERSISTENT_SIZE
private:
  void _EnableItem(int nID, bool bEnable)
  {
    m_pToolbar->EnableItem(nID,bEnable);
    m_pMenuBar->EnableItem(nID,bEnable);
  }
  void _SetSampleEnabled(bool bSampleEnabled)
  {
    m_pToolbar->SetSampleEnabled(bSampleEnabled);
    m_pMenuBar->SetSampleEnabled(bSampleEnabled);
  }
  typedef CMDIFrame SUPER;
  void _OpenGraphic();
  void _History();
  void _ApplyAll();
  void _Apply();
  void _Accept();
  void _Approve();
  void _ToggleDisabled();
  void _ShowTable();
  //void _ShowToolbar(bool bShow);

  wxString m_sUserID;
  wxSize m_sz;
  CHistoryTime m_Hist;
  CNotebookEditSample *m_pNoteBook;
  CFrameAnalysis *m_pCreator;
  mainFrame *m_pParent;
  COARfile *m_pOARfile;
  COARsample *m_pSample;
  CToolbarSample *m_pToolbar;
  CMenuBarSample *m_pMenuBar;
  bool m_bTitleMod;
  //bool m_bFirstShow;
public:
  DECLARE_EVENT_TABLE()
  DECLARE_ABSTRACT_CLASS(CFrameSample)
};

#endif