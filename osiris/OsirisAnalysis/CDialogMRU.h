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

*  FileName: CDialogMRU.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_MRU_H__
#define __C_DIALOG_MRU_H__

#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/string.h>
#include <wx/progdlg.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxProgressCallback.h"

class nwxXmlMRU;
class mainFrame;
class nwxListMRU;
class wxTextCtrl;

class CProgressDialogMRU : 
  public wxProgressDialog, 
  public InwxProgressCallback
{
public:
  CProgressDialogMRU(wxWindow *parent, int nCount) : 
      wxProgressDialog(
        _T("Cleaning up list"),
        _T("Updating modification times and\n"
          "removing missing files from list."),
        nCount,
        parent,
        wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT |
        wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME)
      {;}
  virtual bool UpdateProgress(int n)
  {
    return Update(n);
  }
};

#define MRU_STYLE_CLEANUP       1
#define MRU_STYLE_OPEN_STARTUP  2

class CDialogMRU : public wxDialog
{
public:
  CDialogMRU(
    nwxXmlMRU *pMRU, mainFrame *parent,
    int nStyle = 0
    );
  virtual ~CDialogMRU();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual bool Validate();
  void CheckStartup(bool b = true)
  {
    if(m_pCBstartup != NULL)
    {
      m_pCBstartup->SetValue(b);
    }
  }
  bool StartupChecked()
  {
    bool bRtn = false;
    if(m_pCBstartup != NULL)
    {
      bRtn = m_pCBstartup->GetValue();
    }
    return bRtn;
  }
  bool LoadFiles();
  void ClearSelection();
  void OnActivate(wxListEvent &e);
  void OnSelect(wxListEvent &e);
  // void OnColumnClick(wxListEvent &e);
  void SetMaxFilesToOpen(size_t n)
  {
    m_nMAX = n;
  }
  size_t GetMaxFileToOpen()
  {
    return m_nMAX;
  }
  void OnCleanup(wxCommandEvent &);
  void OnClearSearch(wxCommandEvent &);
  void OnSearch(wxCommandEvent &);
  void UpdateStartup();
private:
  void _SetupImageList();
  wxTextCtrl *m_pTextSearch;
  wxCheckBox *m_pCBstartup;
  wxButton *m_pButtonCleanup;
  nwxListMRU *m_pListMRU;
  nwxXmlMRU *m_pXmlMRU;
  mainFrame *m_pParent;
  size_t m_nMAX;
  time_t m_tMessage;
  vector<wxString> m_vsFiles;
  DECLARE_EVENT_TABLE();
};


#endif
