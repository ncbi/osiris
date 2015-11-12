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

*  FileName: CDialogAnalysis.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CDialogAnalysis.h"
#include "CDialogVolumes.h"
#include "CFrameAnalysis.h"
#include "CLabSettings.h"
#include "Platform.h"
#include "nwx/nwxTabOrder.h"
#include "nwx/nwxFileUtil.h"
#include <wx/filename.h>

CDialogAnalysis::CDialogAnalysis(
    wxWindow *parent,
    CParmOsiris *pParm,
    bool bSave,
    bool bEnableInputDir,
    bool bUseLabSettings) :
  wxDialog(parent,wxID_ANY,wxString("Analyze Data")),
    m_pVolume(NULL),
    m_pParm(pParm),
    m_pDirInput(NULL),
    m_pDirOutput(NULL),
    m_pTextInput(NULL),
    m_pTextOutput(NULL),
    m_pCheckSubDir(NULL),
    m_pComboVolumeName(NULL),
    m_pComboLsName(NULL),
    m_pGridRFU(NULL),
    m_pRadioRaw(NULL),
    m_pRadioAnalyzed(NULL),
    m_bSave(bSave),
    m_bUseLabSettings(bUseLabSettings),
    m_bLabSettingsOK(true),
    m_bDataAnalyzed(false),
    m_bProblem(false)
{

  if(m_bLabSettingsOK)
  {
    //ConfigDir *pDir = mainApp::GetConfig();
    //wxString sLadderFile = pDir->GetLadderFileName();

    m_pKitList = mainApp::GetKitList();
    if(!m_pKitList->GetCount())
    {
      m_bProblem = true;
      m_sErrorStr = m_pKitList->GetErrorMsg();
      if(m_sErrorStr.IsEmpty())
      {
        m_sErrorStr = "Cannot load kit information";
      }
    }
    else
    {
      const wxString &sMsg(m_pKitList->GetErrorMsg());
      if(!sMsg.IsEmpty())
      {
        mainApp::ShowAlert(sMsg,parent);
      }
    }
  }
  if(!m_bProblem)
  {
    wxSize size(300,-1);

    // input directory

    m_pTextInput = new wxTextCtrl(this,wxID_ANY,".\\TestAnalysis\\Cofiler",
      wxDefaultPosition, size);
    wxButton *pBrowseInput = 
      new wxButton(this,IDbrowseInput,"Browse...");
    wxBoxSizer *pSizerInput = new wxBoxSizer(wxHORIZONTAL);
    pSizerInput->Add(m_pTextInput,1,wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
    pSizerInput->Add(pBrowseInput,0,wxALIGN_CENTER_VERTICAL);
    m_pTextInput->SetEditable(bEnableInputDir);
    pBrowseInput->Enable(bEnableInputDir);

    // output directory

    m_pTextOutput = new wxTextCtrl(this,wxID_ANY,".\\Reports",
      wxDefaultPosition, size);
    wxButton *pBrowseOutput = 
      new wxButton(this,IDbrowseOutput,"Browse...");
    wxBoxSizer *pSizerOutput = new wxBoxSizer(wxHORIZONTAL);
    pSizerOutput->Add(m_pTextOutput,1,wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
    pSizerOutput->Add(pBrowseOutput,0,wxALIGN_CENTER_VERTICAL);

    // checkbox for time stamps subdirectory

    m_pCheckSubDir = new wxCheckBox(this,wxID_ANY,"Create time stamped subdirectory");

    // Operating Procedure Name
#ifdef __WXOSX_COCOA__
#define CB_STYLE  wxCB_READONLY
#else
#define CB_STYLE  wxCB_SORT | wxCB_READONLY
#endif
    const wxArrayString &as(m_volumes.GetVolumeNames());
    m_pComboVolumeName = new wxComboBox(this,IDvolumeName,as[0],
      wxDefaultPosition, wxDefaultSize,
      as, CB_STYLE);
    wxButton *pBrowseVolume = 
      new wxButton(this,IDvolume,"Browse...");
    wxBoxSizer *pSizerVolume = new wxBoxSizer(wxHORIZONTAL);
    pSizerVolume->Add(m_pComboVolumeName,1,wxALIGN_CENTER_VERTICAL | wxRIGHT, ID_BORDER);
    pSizerVolume->Add(pBrowseVolume,0,wxALIGN_CENTER_VERTICAL);

    // lane standard

    m_pComboLsName = new wxComboBox(this,wxID_ANY,wxEmptyString,
      wxDefaultPosition, wxDefaultSize,
      0, NULL,CB_STYLE);

#undef CB_STYLE

    // RFU
    m_pGridRFU = new CGridRFURun(this);
    wxBoxSizer *pSizerRFU = new wxBoxSizer(wxHORIZONTAL);
    pSizerRFU->Add(m_pGridRFU,1,wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, ID_BORDER);
    pSizerRFU->AddSpacer(100);

    wxBoxSizer *pSizerRadio = new wxBoxSizer(wxHORIZONTAL);

    m_pRadioRaw = new wxRadioButton(
      this,
      wxID_ANY, 
      CParmOsiris::LABEL_DATA_RAW,
      wxDefaultPosition, 
      wxDefaultSize, 
      wxRB_GROUP);
    pSizerRadio->Add(m_pRadioRaw,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    m_pRadioAnalyzed = new wxRadioButton(
      this,
      wxID_ANY,
      CParmOsiris::LABEL_DATA_ANALYZED,
      wxDefaultPosition,
      wxDefaultSize);
    pSizerRadio->Add(m_pRadioAnalyzed,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT,ID_BORDER);



    wxBoxSizer *pSizerButtons = new wxBoxSizer(wxHORIZONTAL);
    wxButton *pButtonOK = 
      new wxButton(this,wxID_OK,"&OK");
    wxButton *pButtonCancel =
      new wxButton(this,wxID_CANCEL,"&Cancel");

    pSizerButtons->Add(pButtonOK,0,wxRIGHT,ID_BORDER);
    pSizerButtons->Add(pButtonCancel,0,wxLEFT,ID_BORDER);

    wxFlexGridSizer *pGrid(new wxFlexGridSizer(7,2,ID_BORDER,ID_BORDER));
    int FLAG = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxEXPAND;
    int FLAG_TOP = wxALIGN_TOP | wxALIGN_LEFT | wxTOP;
    pGrid->SetFlexibleDirection(wxBOTH);

    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_INPUT),0,FLAG);
    pGrid->Add(pSizerInput,0,FLAG);
    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_OUTPUT), 0, FLAG);
    pGrid->Add(pSizerOutput, 0, FLAG);
    pGrid->AddSpacer(1);
    pGrid->Add(m_pCheckSubDir,0,FLAG);
    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_VOLUME),0, FLAG);
    pGrid->Add(pSizerVolume, 0, FLAG);
    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_LS),0, FLAG);
    pGrid->Add(m_pComboLsName, 0, FLAG);
    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_RFU), 0, FLAG_TOP,2);
    pGrid->Add(pSizerRFU, 0, FLAG ^ wxEXPAND);
    pGrid->Add(MakeBoxSizerText(CParmOsiris::LABEL_DATA), 0, FLAG);
    pGrid->Add(pSizerRadio,0,FLAG);

    {
      nwxTabOrder tabOrder;
      tabOrder.push_back(m_pTextInput);
      tabOrder.push_back(pBrowseInput);
      tabOrder.push_back(m_pTextOutput);
      tabOrder.push_back(pBrowseOutput);
      tabOrder.push_back(m_pCheckSubDir);
      tabOrder.push_back(m_pComboVolumeName);
      tabOrder.push_back(pBrowseVolume);
      tabOrder.push_back(m_pComboLsName);
      tabOrder.push_back(m_pGridRFU);
      tabOrder.push_back(m_pRadioRaw);
      tabOrder.push_back(m_pRadioAnalyzed);
      tabOrder.push_back(pButtonOK);
      tabOrder.push_back(pButtonCancel);
    }

    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    pSizer->Add(pGrid,1,wxALL | wxALIGN_CENTER, ID_BORDER);
    pSizer->Add(pSizerButtons, 0, wxALL | wxALIGN_CENTER, ID_BORDER);
    pSizer->Fit(this);
    SetSizer(pSizer);
    CentreOnParent();
  } // if(m_bLabSettingsOK)
}

CDialogAnalysis::~CDialogAnalysis()
{
  if(m_pDirInput != NULL)
  {
    delete m_pDirInput;
  }
  if(m_pDirOutput != NULL)
  {
    delete m_pDirOutput;
  }
}
#define LOCK_READ_MS 5000 // every 5 seconds

void CDialogAnalysis::OnTimer(wxTimerEvent &e)
{
  if(m_pVolume != NULL)
  {
    m_pVolume->SetInUseOnTimer(e.GetInterval());
  }
}
void CDialogAnalysis::OnBrowseInput(wxCommandEvent &)
{
  wxDirDialog *pDir = GetInputDialog();
  if(pDir->ShowModal() == wxID_OK)
  {
    m_pTextInput->SetValue(pDir->GetPath());
  }
}
void CDialogAnalysis::OnBrowseOutput(wxCommandEvent &)
{
  wxDirDialog *pDir = GetOutputDialog();
  if(pDir->ShowModal() == wxID_OK)
  {
    m_pTextOutput->SetValue(pDir->GetPath());
  }
}
void CDialogAnalysis::OnBrowseVolume(wxCommandEvent &)
{
  wxString sSelect = m_pComboVolumeName->GetStringSelection();
  if(!sSelect.IsEmpty())
  {
    auto_ptr<wxBusyCursor> cursor(new wxBusyCursor);
    CDialogVolumes dlg(this,sSelect,true);
    cursor = auto_ptr<wxBusyCursor>(NULL);
    int n = dlg.ShowModal();
    if(n == wxID_OK)
    {
      wxString sSelectNew = dlg.GetSelection();
      if( (!sSelectNew.IsEmpty()) &&
        (sSelectNew != sSelect) )
      {
        SelectVolumeByName(sSelectNew);
      }
    }
  }
}

bool CDialogAnalysis::SelectVolumeByName(const wxString &s)
{
  int nSelect = m_pComboVolumeName->GetSelection();
  int nSelect2 = -1;
  bool bRtn = false;
  if(m_pComboVolumeName->SetStringSelection(s))
  {
    DoVolumeChange();
    bRtn = !m_bProblem;
  }
  else
  {
    m_bProblem = true;
  }
  if(m_bProblem)
  {
    nSelect2 = m_pComboVolumeName->GetSelection();
    m_pComboVolumeName->Select(nSelect);
    DoVolumeChange();
    if(m_bProblem)
    {
      int i;
      int nCount = (int) m_pComboVolumeName->GetCount();
      for(i = 0; i < nCount; i++)
      {
        if( (i != nSelect) && (i != nSelect2) )
        {
          m_pComboVolumeName->Select(i);
          DoVolumeChange();
          if(!m_bProblem)
          {
            i = nCount; // loop exit
          }
        }
      }
      if(m_bProblem)
      {
        // if there is still a problem, then we are soooo f***ed
        mainApp::ShowError(
          "There is a problem with all " Volume_string "s.\n"
          "Contact the OSIRIS developers at\n"
          "forensics@ncbi.nlm.nih.gov for further\n"
          "assistance",this);
        EndModal(wxID_CANCEL);
      }
    }
  }
  return bRtn;
}

bool CDialogAnalysis::Show(bool show)
{
  bool bRtn = wxDialog::Show(show);
  if(show)
  {
    m_pTextInput->SetFocus();
  }
  return bRtn;
}


void CDialogAnalysis::OnVolumeChange(wxCommandEvent &)
{
  wxString sCurrent;
  if((m_pVolume != NULL) && !m_bProblem)
  {
    sCurrent = m_pVolume->GetVolumeName();
  }
  DoVolumeChange();
  if(m_bProblem)
  {
    mainApp::ShowError(
      "There is a problem with the selected " Volume_string ".",this);
    
    if( (!sCurrent.IsEmpty()) && (
        (m_pVolume == NULL) ||
        (sCurrent != m_pVolume->GetVolumeName())
      ) )
    {
      // change it back
      SelectVolumeByName(sCurrent);
    }
  }
}
void CDialogAnalysis::DoVolumeChange()
{
  wxString sVolume = m_pComboVolumeName->GetStringSelection();
  m_pVolume = m_volumes.Find(sVolume);
  const wxArrayString *pa(NULL);
  m_pComboLsName->Clear();
  m_bProblem  = false;
  if(m_pVolume == NULL)
  {
    m_bProblem = true;
  }
  else
  {
    wxString sKitName = m_pVolume->GetKitName();
    pa = sKitName.IsEmpty() ? NULL : m_pKitList->GetLsArray(sKitName);
    if(pa == NULL)
    {
      m_bProblem = true;
    }
    else
    {
      CLabSettings *pLab = m_pVolume->GetLabSettings();
      m_pComboLsName->Append(*pa);
      CLabMarkerSetCollection *pMarker = 
        pLab->GetMarkerCollection();
      bool bEnableRFU = pLab->CanOverrideMinRfu();
      bool bEnableILS = pLab->GetAllowUserOverrideILS();
      bool bAnalyzed = pLab->GetAnalyzedData();
      if(pMarker != NULL)
      {
        m_pComboLsName->SetStringSelection(pMarker->GetILSName());
      }
      else
      {
        m_bProblem = true;
        m_pComboLsName->Select(0);
      }
      if(m_bUseLabSettings || !bEnableRFU)
      {
        pLab->GetMinRFU(
          &m_nMinRFU, &m_nMinRFU_ILS, &m_nMinRFU_Ladder,
          &m_nMinRFU_Interlocus, &m_nMinRFU_LadderInterlocus, 
          &m_nMinRFU_SampleDetection,
          &m_anChannelRFU,
          &m_anChannelDetection);
      }
      else
      {
        m_nMinRFU = m_pParm->GetMinRFU_Sample();
        m_nMinRFU_ILS = m_pParm->GetMinRFU_ILS();
        m_nMinRFU_Ladder = m_pParm->GetMinRFU_Ladder();
        m_nMinRFU_Interlocus = m_pParm->GetMinRFU_Interlocus();
        m_anChannelRFU = m_pParm->GetChannelRFU();
        m_anChannelDetection = m_pParm->GetChannelDetection();
        m_nMinRFU_LadderInterlocus = m_pParm->GetMinLadderInterlocusRFU();
        m_nMinRFU_SampleDetection = m_pParm->GetSampleDetectionThreshold();
      }
      if(bEnableRFU)
      {
#define FIX_RFU(Sample,Interlocus,Ladder,LadderInterlocus,SampleDetection) \
if(Interlocus < Sample) \
{  Interlocus = Sample; } \
if(LadderInterlocus < Ladder) \
{  LadderInterlocus = Ladder;} \
if( (SampleDetection > Sample) || (SampleDetection <= 0) ) \
{  SampleDetection = Sample; }

        FIX_RFU(
          m_nMinRFU,m_nMinRFU_Interlocus,
          m_nMinRFU_Ladder,m_nMinRFU_LadderInterlocus,
          m_nMinRFU_SampleDetection);
      }
      m_pGridRFU->SetupKit(sKitName);
      m_pGridRFU->TransferDataToWindow();
      m_pGridRFU->SetAllReadOnly(!bEnableRFU);

      m_pRadioRaw->SetValue(!bAnalyzed);
      m_pRadioAnalyzed->SetValue(bAnalyzed);

      m_pRadioRaw->Enable(bEnableILS);
      m_pRadioAnalyzed->Enable(bEnableILS);
      m_pComboLsName->Enable(bEnableILS);
      Layout();
      Fit();
    }
  }
}

bool CDialogAnalysis::TransferDataToWindow()
{
  if(IsOK())
  {
    wxString sDir;
    if(m_pParm == NULL)
    {
      m_pParm = CParmOsiris::GetGlobal();
    }
    sDir = m_pParm->GetInputDirectory();
    FIX_FILE_NAME(sDir);
    m_pTextInput->SetValue(sDir);
    sDir = m_pParm->GetOutputDirectory();
    FIX_FILE_NAME(sDir);
    m_pTextOutput->SetValue(sDir);
    m_pCheckSubDir->SetValue(m_pParm->GetTimeStampSubDir());
    if(m_sVolumeName.IsEmpty())
    {
      m_sVolumeName = m_pParm->GetVolumeOrKit();
    }
    SelectVolumeByName(m_sVolumeName);
  }
  return true;
}

void CDialogAnalysis::SetupOverride()
{
  int nMinRFU;
  int nMinRFU_ILS;
  int nMinRFU_Ladder;
  int nMinRFU_Interlocus;
  int nMinRFU_LadderInterlocus;
  int nMinRFU_SampleDetection;
  vector<int> anChannelRFU;
  vector<int> anChannelDetection;

  const CLabSettings *pLab = m_pVolume->GetLabSettings();

  pLab->GetMinRFU(
      &nMinRFU, &nMinRFU_ILS, &nMinRFU_Ladder,
      &nMinRFU_Interlocus, &nMinRFU_LadderInterlocus, 
      &nMinRFU_SampleDetection,
      &anChannelRFU,
      &anChannelDetection);
  FIX_RFU(
    nMinRFU,nMinRFU_Interlocus,
    nMinRFU_Ladder,nMinRFU_LadderInterlocus,
    nMinRFU_SampleDetection);

  typedef struct
  {
    int n1;
    int n2;
    const wxChar *ps;
  } RFU_STRUCT;

  RFU_STRUCT COMPARE_RFU[] =
  {
    {nMinRFU,m_nMinRFU,wxS("Sample RFU")},
    {nMinRFU_ILS,m_nMinRFU_ILS,wxS("ILS RFU")},
    {nMinRFU_Ladder,m_nMinRFU_Ladder,wxS("Ladder RFU")},
    {nMinRFU_Interlocus,m_nMinRFU_Interlocus,wxS("Interlocus RFU")},
    {nMinRFU_LadderInterlocus,m_nMinRFU_LadderInterlocus,wxS("Interlocus Ladder RFU")},
    {nMinRFU_SampleDetection,m_nMinRFU_SampleDetection,wxS("Sample Detection RFU")}
  };
  const size_t SIZE(sizeof(COMPARE_RFU)/sizeof(COMPARE_RFU[0]));
  RFU_STRUCT *pRFU = &COMPARE_RFU[0];

  m_sOverride.Empty();
  for(size_t i = 0; i < SIZE; i++)
  {
    if(pRFU->n1 != pRFU->n2)
    {
      AppendOverride(pRFU->ps);
    }
    pRFU++;
  }

  const CLabMarkerSetCollection *pMarker = pLab->GetMarkerCollection();
  if(pMarker->GetILSName() != m_sLsName)
  {
    AppendOverride(wxS("Internal Lane Standard"));
  }
  if(m_bDataAnalyzed == pLab->GetAnalyzedData())
  {}
  else if(m_bDataAnalyzed)
  {
    AppendOverride(wxS("Analyzed data"));
  }
  else
  {
    AppendOverride(wxS("Raw data"));
  }
}
bool CDialogAnalysis::TransferDataFromWindow()
{
  bool bRtn = false;
  if(IsOK() && m_pVolume != NULL)
  {
    wxString sError;
    bool bOverrideRFU = 
      m_pVolume->GetLabSettings()->CanOverrideMinRfu();
    m_sInputDirectory = m_pTextInput->GetValue();
    m_sOutputDirectory = m_pTextOutput->GetValue();
    m_bDataAnalyzed = m_pRadioAnalyzed->GetValue();
    m_sOverride.Empty();
    m_sVolumeName = m_pComboVolumeName->GetValue();
    m_sLsName = m_pComboLsName->GetValue();
    if(bOverrideRFU)
    {
      m_pGridRFU->TransferDataFromWindow();

      SetupOverride();

      if(m_nMinRFU_Interlocus < m_nMinRFU)
      {
        sError += CParmOsiris::ERROR_MSG_INTERLOCUS;
        sError += "\n";
        // m_pSpinRFU_Interlocus->SetFocus();
      }
      else if(m_nMinRFU_LadderInterlocus < m_nMinRFU_Ladder)
      {
        sError += CParmOsiris::ERROR_MSG_INTERLOCUS;
        sError += "\n";
        // m_pSpinRFU_LadderInterlocus->SetFocus();
      }
    }

    if(m_pVolume->IsLocked())
    {
      wxString sLockUser = m_pVolume->GetLockUser();
      if(sLockUser.IsEmpty())
      {
        sLockUser = "another user";
      }
      sError += "This " Volume_string ", ";
      sError += m_pVolume->GetVolumeName();
      sError += ", is locked by ";
      sError += sLockUser;
      sError += ".\nPlease try again later.\n";
    }
    else if(!m_pVolume->SetInUse())
    {
      sError += "This " Volume_string ", ";
      sError += m_pVolume->GetVolumeName();
      sError += ", cannot be accessed at this time.\n";
    }
    if(!wxDirExists(m_sInputDirectory))
    {
      sError += "Input directory does not exist\n";
    }
    if( (!wxDirExists(m_sOutputDirectory)) && 
        (!wxMkdir(m_sOutputDirectory,0755)) )
    {
      sError += "Cannot create output directory";
    }
    bRtn = sError.IsEmpty();
    if(!bRtn)
    {
      mainApp::ShowError(sError,this);
    }
    else
    {
      if(m_pParm == NULL)
      {
        m_pParm = CParmOsiris::GetGlobal();
      }
      m_pParm->SetDataAnalyzed(m_bDataAnalyzed);
      m_pParm->SetInputDirectory(m_sInputDirectory);
      m_pParm->SetOutputDirectory(m_sOutputDirectory);
      m_pParm->SetVolumeName(m_sVolumeName);
      m_pParm->SetKitName(GetKitName());
      m_pParm->SetLsName(m_sLsName);
      m_pParm->SetMinRFU_ILS(m_nMinRFU_ILS);
      m_pParm->SetMinRFU_Ladder(m_nMinRFU_Ladder);
      m_pParm->SetMinRFU_Sample(m_nMinRFU);
      m_pParm->SetMinRFU_Interlocus(m_nMinRFU_Interlocus);
      m_pParm->SetMinLadderInterlocusRFU(m_nMinRFU_LadderInterlocus);
      m_pParm->SetSampleDetectionThreshold(m_nMinRFU_SampleDetection);
      m_pParm->SetAnalysisOverride(m_sOverride);
      m_pParm->SetTimeStampSubDir(m_pCheckSubDir->GetValue());
      m_pParm->SetChannelDetection(m_anChannelDetection);
      m_pParm->SetChannelRFU(m_anChannelRFU);

      if(!( m_bSave && m_pParm->IsModified() )) {}
      else if(!m_pParm->Save())
      {
        sError =
          "Cannot save parameters to confiuration file\n"
          "Analysis will continue anyway.";
        mainApp::ShowError(sError,this);
      }
    }
  }
  return bRtn;
}

void CDialogAnalysis::FindPath(wxString *ps, const wxString &sDefault)
{
  if(!ps->IsEmpty())
  {
    wxString s1;
    nwxFileUtil::NoEndWithSeparator(ps);
    bool bDone = ps->IsEmpty();
    while( !bDone)
    {
      if(wxFileName::DirExists(*ps))
      {
        bDone = true;
      }
      else
      {
        wxFileName fn(*ps);
        s1 = fn.GetPath();
        if(s1 == *ps)
        {
          ps->Empty();
          bDone = true;
        }
        else
        {
          *ps = s1;
        }
      }
    }
  }
  if(ps->IsEmpty())
  {
    *ps = sDefault;
  }
}

BEGIN_EVENT_TABLE(CDialogAnalysis,wxDialog)
EVT_BUTTON(IDbrowseInput,CDialogAnalysis::OnBrowseInput)
EVT_BUTTON(IDbrowseOutput,CDialogAnalysis::OnBrowseOutput)
EVT_BUTTON(IDvolume,CDialogAnalysis::OnBrowseVolume)

EVT_COMBOBOX(IDvolumeName,CDialogAnalysis::OnVolumeChange)
END_EVENT_TABLE()

