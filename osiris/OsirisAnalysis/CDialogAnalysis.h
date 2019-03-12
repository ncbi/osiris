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

*  FileName: CDialogAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_ANALYSIS_H__
#define __C_DIALOG_ANALYSIS_H__

#include "mainApp.h"
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/dirdlg.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/radiobut.h>
//#include <wx/timer.h>

#include "wxIDS.h"
#include "nwx/nwxTabOrder.h"
#include "CParmOsiris.h"
#include "CKitList.h"
#include "CLabSettings.h"
#include "CVolumes.h"
//#include "nwx/nwxTimerReceiver.h"
#include "CGridRFURun.h"

class CDialogAnalysis : public wxDialog //, public nwxTimerReceiver
{
public:
  CDialogAnalysis(
    wxWindow *parent,
    CParmOsiris *pOsiris = NULL, 
    bool bSave = true, 
    bool bEnableInputDir = true,
    bool bUseLabSettings = true);
  virtual ~CDialogAnalysis();
  wxString &GetInputDirectory()
  {
    return m_sInputDirectory;
  }
  wxString &GetOutputDirectory()
  {
    return m_sOutputDirectory;
  }
  const CVolume &GetVolume() const
  {
    return *m_pVolume;
  }
  wxString GetKitName()
  {
    wxString sRtn;
    if(m_pVolume != NULL)
    {
      sRtn = m_pVolume->GetKitName();
    }
    return sRtn;
  }
  bool SetVolumeName(const wxString &sName)
  {
    m_sVolumeName = sName;
    bool bRtn = SelectVolumeByName(sName);
    return bRtn;
  }
  wxString &GetVolumeName()
  {
    return m_sVolumeName;
  }
//  BEGIN CGridRFURun Get/Set
  int GetMinRFU()
  {
    return m_nMinRFU;
  }
  int GetMinRFU_Interlocus()
  {
    return m_nMinRFU_Interlocus;
  }

  int GetMinRFU_ILS()
  {
    return m_nMinRFU_ILS;
  }
  int GetMinRFU_Ladder()
  {
    return m_nMinRFU_Ladder;
  }

  int GetMinRFU_LadderInterlocus()
  {
    return m_nMinRFU_LadderInterlocus;
  }
  int GetMinRFU_SampleDetection()
  {
    return m_nMinRFU_SampleDetection;
  }
  const vector<int> &GetChannelRFU() const
  {
      return m_anChannelRFU;
  }
  const vector<int> &GetChannelDetection() const
  {
    return m_anChannelDetection;
  }

  // set

  void SetMinRFU(int n)
  {
    m_nMinRFU = n;
  }
  void SetMinRFU_Interlocus(int n)
  {
    m_nMinRFU_Interlocus = n;
  }

  void SetMinRFU_ILS(int n)
  {
    m_nMinRFU_ILS = n;
  }
  void SetMinRFU_Ladder(int n)
  {
    m_nMinRFU_Ladder = n;
  }

  void SetMinRFU_LadderInterlocus(int n)
  {
    m_nMinRFU_LadderInterlocus = n;
  }
  void SetMinRFU_SampleDetection(int n)
  {
    m_nMinRFU_SampleDetection = n;
  }
  void SetChannelRFU(const vector<int> &an)
  {
      m_anChannelRFU = an;
  }
  void SetChannelDetection(const vector<int> &an)
  {
    
    m_anChannelDetection = an;
  }
  
  bool LabSettingsOK()
  {
    return m_bLabSettingsOK;
  }
  bool AnalyzedData()
  {
    return m_bDataAnalyzed;
  }
  bool IsOK()
  {
    return !m_bProblem;
  }
  const wxString &GetErrorMsg()
  {
    return m_sErrorStr;
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void OnBrowseInput(wxCommandEvent &);
  void OnBrowseOutput(wxCommandEvent &);
  void OnBrowseVolume(wxCommandEvent &);
  void OnVolumeChange(wxCommandEvent &);
  //virtual void OnTimer(wxTimerEvent &);
  void DoVolumeChange();
  virtual bool Show(bool show = true);
  bool SelectVolumeByName(const wxString &s);
  const wxString &GetOverride()
  {
    return m_sOverride;
  }
private:
  static void FindPath(wxString *ps, const wxString &sDefault);
  void AppendOverride(const wxChar *ps)
  {
    if(!m_sOverride.IsEmpty())
    {
      m_sOverride.Append(", ");
    }
    else
    {
      m_sOverride.Alloc(256);
    }
    m_sOverride.Append(ps);
  }
  void SetupOverride();

  wxBoxSizer *MakeBoxSizerText(const wxString &sText)
  {
    wxBoxSizer *pRtn(new wxBoxSizer(wxHORIZONTAL));
    wxStaticText *p(new wxStaticText(this,wxID_ANY,sText));
    pRtn->Add(p,0,
      wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxTOP,
      ID_BORDER);
    return pRtn;
  }
  wxDirDialog *GetInputDialog()
  {
    if(m_pDirInput == NULL)
    {
      wxString s = m_pTextInput->GetValue();
      wxString sDefault = mainApp::GetConfig()->GetExePath();
      sDefault.Append("TestAnalysis");
      FindPath(&s,sDefault);
      m_pDirInput = new wxDirDialog(this,
        "Input Directory",s,
        wxDD_DIR_MUST_EXIST | wxDD_DEFAULT_STYLE | wxRESIZE_BORDER);
    }
    return m_pDirInput;
  }
  wxDirDialog *GetOutputDialog()
  {
    if(m_pDirOutput == NULL)
    {
      wxString s = m_pTextOutput->GetValue();
      FindPath(&s,mainApp::GetConfig()->GetFilePath());
      m_pDirOutput = new wxDirDialog(this,
        "Output Directory",s);
    }
    return m_pDirOutput;
  }

  wxString m_sErrorStr;
  CVolumes m_volumes;
  CPersistKitList *m_pKitList;
  CVolume *m_pVolume;
  CParmOsiris *m_pParm;

  wxDirDialog *m_pDirInput;
  wxDirDialog *m_pDirOutput;
  wxTextCtrl *m_pTextInput;
  wxTextCtrl *m_pTextOutput;
  wxCheckBox *m_pCheckSubDir;
  wxComboBox *m_pComboVolumeName;
  wxComboBox *m_pComboLsName;

  CGridRFURun *m_pGridRFU;
  wxRadioButton *m_pRadioRaw;
  wxRadioButton *m_pRadioAnalyzed;

  vector<int> m_anChannelRFU;
  vector<int> m_anChannelDetection;
  wxString m_sInputDirectory;
  wxString m_sOutputDirectory;
  wxString m_sVolumeName;
  wxString m_sLsName;
  wxString m_sOverride;
  int m_nMinRFU;
  int m_nMinRFU_ILS;
  int m_nMinRFU_Ladder;
  int m_nMinRFU_Interlocus;
  int m_nMinRFU_LadderInterlocus;
  int m_nMinRFU_SampleDetection;
  bool m_bSave;
  bool m_bUseLabSettings;
  bool m_bLabSettingsOK;
  bool m_bDataAnalyzed;
  bool m_bProblem;
  bool m_bParmsFromConstructor;
  DECLARE_EVENT_TABLE()
};

#endif
