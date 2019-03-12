/* ===========================================================================
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

*  FileName: CFrameRunAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_FRAME_RUN_ANALYSIS_H__
#define __C_FRAME_RUN_ANALYSIS_H__

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/gauge.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/tglbtn.h>
#include <wx/listctrl.h>
#include <wx/process.h>
#include "CMDIFrame.h"
#include "CDirList.h"
#include "CParmOsiris.h"
#include "CVolumes.h"
class CProcessAnalysis;
class mainFrame;
class CListProcess;
class CDialogAnalysis;

class CFrameRunAnalysis : public CMDIFrame, public IXml2ErrorReceiver
{
public:
  CFrameRunAnalysis(mainFrame *parent, const wxSize &sz, const wxString &sFileName = wxEmptyString);
  CFrameRunAnalysis(mainFrame *parent, const wxSize &sz, const CVolume &vol);
  CFrameRunAnalysis(mainFrame *parent, const wxSize &sz, const CVolume &vol, const CParmOsiris &parmNew);
  CFrameRunAnalysis(CFrameRunAnalysis *pPrev, const CVolume &vol, const CParmOsiris &parmNew);
  virtual ~CFrameRunAnalysis();
  virtual int GetType();
  virtual wxString GetFileName();
  virtual void OnTimer(wxTimerEvent &);
  virtual void ReceiveXml2Error(const wxString &s);
  void Run();
  void OnButtonCancel(wxCommandEvent &);
  void OnButtonCancelAll(wxCommandEvent &);
  void OnButtonView(wxCommandEvent &);
  void OnButtonSelectAll(wxCommandEvent &);
  void OnButtonClear(wxCommandEvent &);
  void OnButtonReAnalyze(wxCommandEvent &);
  void OnClose(wxCloseEvent &);

  void OnSelectionChanged(wxListEvent &);
  void OnActivate(wxListEvent &);
  bool IsOK() { return m_bOK; }
  void OnDetails(wxCommandEvent &);
  // void OnProcessEnd(wxProcessEvent&);
  void ShowDetails()
  {
    if(!m_pButtonDetails->GetValue())
    {
      wxCommandEvent e;
      m_pButtonDetails->SetValue(true);
      OnDetails(e);
    }
    SetFocus();
    Raise();
  }
  bool IsDone();
  bool CheckIsDone();
  CDirList *GetDirList()
  {
    return &m_DirList;
  }
  size_t GetDirListCount()
  {
    return m_DirList.GetCount();
  }
private:
  void OnEndProcess(wxProcessEvent &e);
  void _PingOnOpen(const char *psType);
  bool _FileNeedsAttention(CDirEntry *pEntry);
  void _RunAutoExport(CDirEntry *pEntry);
  void _BuildWindow(const wxString &sTitle, const wxSize &sz);
  void _Run();
  void DoCancel(bool bCancelAll);
  void Cleanup();
  CDirEntry *NextDirEntry();
  void UpdateOutputText();
  void UpdateButtonState();

  vector<wxString> m_asErrors;
  CVolume m_volume;
  CParmOsiris m_parmOsiris;
  CDirList m_DirList;
  wxButton *m_pButtonSelectAll;
  wxButton *m_pButtonClearAll;
  wxButton *m_pButtonCancel;
  wxButton *m_pButtonCancelAll;
  wxButton *m_pButtonReAnalyze;
  wxButton *m_pButtonView;

  wxGauge *m_pGauge;
  wxStaticText *m_pLabelElapsed;
  wxTextCtrl *m_pTextOutput;
  wxToggleButton *m_pButtonDetails;
  CProcessAnalysis *m_pAnalysis;
  wxBoxSizer *m_pSizer;
  CListProcess *m_pListDir;
//  CDialogAnalysis *m_pDlgAnalysis;
  time_t m_tStart;
  bool m_bOK;
  size_t m_nNext;
  DECLARE_EVENT_TABLE();
};
#endif
