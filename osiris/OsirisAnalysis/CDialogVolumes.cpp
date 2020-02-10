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
*  FileName: CDialogVolumes.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "mainFrame.h"
#include "Platform.h"
#include "CDialogVolumes.h"
#include "CMessageDialogCheckCancel.h"
#include "CKitList.h"
#include "wxIDS.h"
#include "CParmOsiris.h"
#include <wx/msgdlg.h>
#include <wx/utils.h>


//  number of milliseconds between 'touching' the lock file
//  for a locked volume

//******************************************************  CDialogVolumeRename
class CDialogVolumeRename : public wxDialog
{
public:
  CDialogVolumeRename(
    CVolumes *pVolumes,
    CVolume *pThisVolume,
    wxWindow *parent);
  virtual ~CDialogVolumeRename();
private:
  CVolumes *m_pVolumes;
  CVolume *m_pThisVolume;
  void OnOK(wxCommandEvent &);
  wxTextCtrl *m_pText;
  DECLARE_EVENT_TABLE();
};

CDialogVolumeRename::~CDialogVolumeRename() {}
CDialogVolumeRename::CDialogVolumeRename(
    CVolumes *pVolumes,
    CVolume *pThisVolume,
    wxWindow *parent) :
  wxDialog(parent,wxID_ANY,"Rename " VOLUME_STRING,
    wxDefaultPosition, wxDefaultSize,
    mainApp::DIALOG_STYLE),
  m_pVolumes(pVolumes),
  m_pThisVolume(pThisVolume)

{
  wxStaticText *pLabelOld = new wxStaticText(this,wxID_ANY,"Current name:");
  wxStaticText *pValueOld = new wxStaticText(this,wxID_ANY,m_pThisVolume->GetVolumeName());
  wxStaticText *pLabelNew = new wxStaticText(this,wxID_ANY,"Enter new name:");
  wxFont fn = pValueOld->GetFont();
  fn.SetPointSize(fn.GetPointSize() + 1);
  fn.SetWeight(wxFONTWEIGHT_BOLD);
  pValueOld->SetFont(fn);
  m_pText = new wxTextCtrl(this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxSize(100,-1),wxTE_LEFT);
  wxBoxSizer *pSizer1 = new wxBoxSizer(wxVERTICAL);
  pSizer1->Add(pLabelOld,0,wxALIGN_LEFT,0);
  pSizer1->Add(pValueOld,0,wxLEFT | wxALIGN_LEFT,ID_BORDER + ID_BORDER);
  pSizer1->Add(pLabelNew,0,wxTOP | wxALIGN_LEFT,ID_BORDER);
  pSizer1->Add(m_pText,0,wxEXPAND, 0);
  pSizer1->AddStretchSpacer(1);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(pSizer1, 1, (wxALL ^ wxBOTTOM) | wxEXPAND, ID_BORDER);
  pSizer->Add(
    CreateButtonSizer(wxOK | wxCANCEL),
    0, wxALIGN_CENTER | wxALL, ID_BORDER);
  SetSizer(pSizer);
  Layout();
  Fit();
  CentreOnParent();
}
void CDialogVolumeRename::OnOK(wxCommandEvent &e)
{
  wxString sValue = m_pText->GetValue();
  if(m_pVolumes->Rename(m_pThisVolume,sValue))
  {
    e.Skip(true);
  }
  else
  {
    mainApp::ShowError(m_pVolumes->GetLastError(),this);
  }
}

BEGIN_EVENT_TABLE(CDialogVolumeRename,wxDialog)
EVT_BUTTON(wxID_OK,CDialogVolumeRename::OnOK)
END_EVENT_TABLE()

//******************************************************  CDialogVolumeAddNew
class CDialogVolumeAddNew : public wxDialog
{
public:
  CDialogVolumeAddNew(
    CVolumes *pVolumes,
    wxWindow *parent);
  virtual ~CDialogVolumeAddNew();
  const wxString &GetVolName()
  {
    return m_sName;
  }
private:
  wxString m_sName;
  CVolumes *m_pVolumes;
  wxTextCtrl *m_pText;
  wxChoice *m_pChoiceKit;

  void OnOK(wxCommandEvent &);
  DECLARE_EVENT_TABLE();
};

CDialogVolumeAddNew::~CDialogVolumeAddNew() {}

CDialogVolumeAddNew::CDialogVolumeAddNew(
    CVolumes *pVolumes,
    wxWindow *parent) :
  wxDialog(parent,wxID_ANY,"Create New " VOLUME_STRING,
    wxDefaultPosition, wxDefaultSize,
    mainApp::DIALOG_STYLE
    ),
  m_pVolumes(pVolumes)
{
  
  const wxArrayString &as(pVolumes->GetVolumeNames());
  wxBoxSizer *pSizer1 = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);

  m_pChoiceKit = new wxChoice(
    this,wxID_ANY,wxDefaultPosition,wxDefaultSize,
    as);

  m_pText = new wxTextCtrl(this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxSize(100,-1),wxTE_LEFT);
  pSizer1->Add(
    new wxStaticText(this,wxID_ANY,"Select " aVolume_string " to copy:"),
    0,wxALIGN_LEFT,0);
  pSizer1->Add(m_pChoiceKit,0,wxEXPAND | wxBOTTOM,ID_BORDER);
  pSizer1->Add(
    new wxStaticText(this,wxID_ANY,"Enter " Volume_string " name:"),
    0,wxALIGN_LEFT,0);
  pSizer1->Add(m_pText,0,wxEXPAND,0);
  pSizer1->AddStretchSpacer(1);
  pSizer->Add(pSizer1, 1, (wxALL ^ wxBOTTOM) | wxEXPAND, ID_BORDER);
  pSizer->Add(
    CreateButtonSizer(wxOK | wxCANCEL),
    0, wxALIGN_CENTER | wxALL, ID_BORDER);
  SetMinSize(wxSize(250,10));
  SetSizer(pSizer);
  Layout();
  Fit();
  CentreOnParent();
}
void CDialogVolumeAddNew::OnOK(wxCommandEvent &e)
{
  wxString sCopyFrom = m_pChoiceKit->GetStringSelection();
  m_sName = m_pText->GetValue();
  bool bError = true;
  {
    wxBusyCursor xxx;
    if(m_pVolumes->Create(sCopyFrom,m_sName))
    {
      bError = false;
      e.Skip();
    }
  }

  if(bError) // we want wxBusyCursor out of scope here
  {
    m_sName.Empty();
    mainApp::ShowError(m_pVolumes->GetLastError(),this);
  }
}
BEGIN_EVENT_TABLE(CDialogVolumeAddNew,wxDialog)
EVT_BUTTON(wxID_OK,CDialogVolumeAddNew::OnOK)
END_EVENT_TABLE()


//******************************************************  CDialogVolumes

int CDialogVolumes::FindString(const wxString &s, const wxArrayString &as)
{
  int nRtn = -1;
  size_t nSize = as.GetCount();
  for(size_t i = 0; i < nSize; i++)
  {
    if(as.Item(i) == s)
    {
      nRtn = (int) i;
      i = nSize; // loop exit
    }
  }
  return nRtn;
}
CDialogVolumes::~CDialogVolumes() {}

CDialogVolumes::CDialogVolumes(
      wxWindow *parent, 
      const wxString &_sSelect, 
      bool bReadOnly) :
  wxDialog(parent,wxID_ANY,"OSIRIS Lab Settings",
    GET_PERSISTENT_POSITION(CDialogVolumes),
    GET_PERSISTENT_SIZE_DEFAULT(CDialogVolumes,CPanelLabSettings::g_SIZE_FRAME),
    mainApp::DIALOG_STYLE
#ifdef __WXMAC__
    & ~wxCLOSE_BOX 
#endif
    ),
  m_pVolumeCurrent(NULL),
  m_pChoice(NULL),
  m_pPanelLab(NULL),
  m_pGridObject(NULL),
  m_bGridEditorShown(false),
  m_bReadOnly(bReadOnly)
{
  const wxArrayString &as = m_volumes.GetVolumeNames();
  size_t nSize = as.GetCount();
#ifndef __NO_MDI__
  SetMaxSize(mainFrame::Size80());
#endif

  if(nSize)
  {
    wxString sSelect;
    int nSelect = -1;
    if(!_sSelect.IsEmpty())
    {
      nSelect = FindString(_sSelect,as);
    }
    if(nSelect < 0)
    {
      CParmOsirisGlobal parms;
      sSelect = parms->GetVolumeName();
      if(!sSelect.IsEmpty())
      {
        nSelect = FindString(sSelect,as);
      }
      if(nSelect < 0)
      {
        nSelect = 0;
      }
    }
    m_pChoice = new wxChoice(
      this,IDvolume,wxDefaultPosition,wxDefaultSize,as);
    m_sSelection = as.Item(nSelect);
    m_pChoice->Select(nSelect);
    m_pPanelLab = new CPanelLabSettings(this,m_bReadOnly);
    wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);
    pSizerTop->Add(
      new wxStaticText(this,wxID_ANY,"Select " VOLUME_STRING ":"),
      0,wxALIGN_CENTER_VERTICAL | wxRIGHT,ID_BORDER);
    pSizerTop->Add(m_pChoice,0,wxALIGN_CENTER_VERTICAL,0);
    if(!m_bReadOnly)
    {
      m_pButtonAdd = nwxChoiceButtons::CreateAddButton(this,VOLUME_STRING,false,pSizerTop);
      m_pButtonRemove = nwxChoiceButtons::CreateRemoveButton(this,VOLUME_STRING,false,pSizerTop);
      m_pButtonRename = nwxChoiceButtons::CreateRenameButton(this,VOLUME_STRING,false,pSizerTop);
      m_pButtonFolder = nwxChoiceButtons::CreateFolderButton(this, false, pSizerTop);
    }
    else
    {
      m_pButtonAdd = NULL;
      m_pButtonRemove = NULL;
      m_pButtonRename = NULL;
      m_pButtonFolder = NULL;
    }

    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    pSizer->Add(pSizerTop,0,(wxALL ^ wxBOTTOM) | wxALIGN_CENTER,ID_BORDER);
    pSizer->Add(m_pPanelLab,1,wxEXPAND,0);
    SetSizer(pSizer);
    _SetVolume();
//    CentreOnParent();
    SetMaxSize(wxDefaultSize);
    SetEscapeId(wxID_NONE);
  }
  else
  {
    wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *pText = new wxStaticText(this,wxID_ANY,"ERROR: Cannot find standard " Volume_string "s");
    pSizer->Add(pText,0,wxALL | wxALIGN_LEFT,ID_BORDER);
    pSizer->Add(CreateButtonSizer(wxOK),0,wxALL | wxALIGN_CENTER,ID_BORDER);
    SetSizer(pSizer);
    Layout();
  }
}
bool CDialogVolumes::_ResetVolume(CVolume *pHold)
{
  bool bRtn = true;
  wxString sError = "A problem occurred when setting " Volume_string " to ";
  sError.Append(m_sSelection);
  mainApp::LogMessage(sError);
  mainApp::ShowError(sError,this);
  if(pHold != NULL)
  {
    m_pVolumeCurrent = pHold;
    const wxString &s(pHold->GetVolumeName());
    if(m_pChoice->SetStringSelection(s))
    {
      m_sSelection = s;
    }
    bRtn = false;
  }
  else
  {
    m_pPanelLab->SetReadOnly(true);
  }
  return bRtn;
}
bool CDialogVolumes::_SetVolume(/*bool bFit*/)
{
  bool bRtn = true;
  bool bReadOnly = false;
  CVolume *pHold = m_pVolumeCurrent;
  if(m_sSelection.IsEmpty())
  {
    m_sSelection = m_pChoice->GetStringSelection();
  }
  m_pVolumeCurrent = m_volumes.Find(m_sSelection);
  if(m_pVolumeCurrent == NULL)
  {
    bRtn = _ResetVolume(pHold);
  }
  else if(m_pVolumeCurrent == pHold)
  {} // do nothing
  else
  {
    CLabSettings *pLab(m_pVolumeCurrent->GetLabSettings());
    m_labCopy = *pLab;
    bReadOnly = 
      m_bReadOnly || !m_pVolumeCurrent->Lock();
    bool bOK = _SetupAll();
    if(!bOK)
    {
      bRtn = _ResetVolume(pHold);
    }
    else
    {
      if(!m_bReadOnly && (pHold != NULL) && !pHold->IsNewVolume())
      {
        // do not release lock on new volumes until window is closed

        pHold->ReleaseLock();
      }
      _EnableRenameRemove(!bReadOnly);
      m_pPanelLab->Layout();
      bool bIsShown = IsShown();
      wxSize szSave;
      if(bIsShown)
      {
        SetMinSize(wxDefaultSize);
        SetMaxSize(wxDefaultSize);
        // scrollbars were not showing in tables, so the following was added
        // the code above is probably not needed.
        nwxGrid::ForceRefreshAll(this);
      }
      if(bReadOnly && !m_bReadOnly && 
        (m_pVolumeCurrent->IsLocked() || m_pVolumeCurrent->CreatedByNewerVersion()) 
          && !m_pVolumeCurrent->IsReadOnly()
          )
      {
        // could not lock volume, try again and show dialog
        // if it fails
        wxCommandEvent ee(wxEVT_COMMAND_BUTTON_CLICKED,IDlock);
        GetEventHandler()->AddPendingEvent(ee);
      }
    }
  }
  RE_RENDER;
  return bRtn;
}
bool CDialogVolumes::_CurrentVolumeModified()
{
  bool bRtn = false;
  {
    CMessageSuppressor xxx;
    bRtn = !TransferDataFromWindow();
  }
  if(!bRtn)
  {
      CLabSettings *pLab(m_pVolumeCurrent->GetLabSettings());
      bRtn = (*pLab) != m_labCopy;
  } 
  return bRtn;
}

bool CDialogVolumes::TransferDataFromWindow()
{
  bool bRtn = true;
  if((m_pVolumeCurrent != NULL) &&
    (m_pPanelLab != NULL) &&
    (!m_bReadOnly) &&
    (!m_pPanelLab->IsReadOnly()) )
  {
    bRtn = m_pPanelLab->TransferDataFromWindow();
  }
  return bRtn;
}
bool CDialogVolumes::_SaveCurrent(bool bWarnUser)
{
  bool bRtn = false;
  if(m_bReadOnly)
  {
    bRtn = true;
  }
  else if(!m_pVolumeCurrent->Lock())
  {
    // should already be locked
    if(bWarnUser)
    {
      mainApp::ShowError(
        "Cannot save settings because lock cannot be obtained.",
        this);
    }
  }
  else if(!m_pVolumeCurrent->Save())
  {
    if(bWarnUser)
    {
      mainApp::ShowError(
        "An error occurred when saving settings",this);
    }
  }
  else
  {
    m_labCopy = *m_pVolumeCurrent->GetLabSettings();
    m_pPanelLab->UpdateAfterSave();
    bRtn = true;
  }
  return bRtn;
}
bool CDialogVolumes::_Apply(bool bWarnUser)
{
  bool bRtn = false;
  if(TransferDataFromWindow())
  {
    bRtn = _CurrentVolumeModified()
    ? _SaveCurrent(bWarnUser) : true;
  }
  return bRtn;
}


void CDialogVolumes::OnOK(wxCommandEvent &)
{
  if(m_pPanelLab == NULL)
  {
    EndModal(wxID_OK);
  }
  else if(!TransferDataFromWindow())
  {}
  else if((m_pPanelLab == NULL) || _Apply())
  {
    EndModal(wxID_OK);
  }
}
void CDialogVolumes::OnCancel(wxCommandEvent &)
{
  bool bEnd = true;
  if(_IsGridEditorShown())
  {
    bEnd = false;
  }
  else if(m_pPanelLab != NULL) // && _CurrentVolumeModified())
  {
    bEnd = _ChangeVolumeWarningOK();
  }
  if(bEnd)
  {
    EndModal(wxID_CANCEL);
  }
}
void CDialogVolumes::OnApply(wxCommandEvent &)
{
  _Apply();
}

bool CDialogVolumes::_ChangeVolumeWarningOK()
{
  bool bOK = true;
  
  if(m_pVolumeCurrent->HasLock() && _CurrentVolumeModified())
  {
    wxMessageDialog dlg(
      this,
      "Warning: The current " Volume_string " has been modified.\n\n"
         "Do you wish to save your modifications.",
      "Warning",
      wxYES_NO | wxYES_DEFAULT | wxCANCEL | wxICON_EXCLAMATION );
    int n = dlg.ShowModal();
    if(n == wxID_OK || n == wxID_YES)
    {
      if(!TransferDataFromWindow())
      {
        bOK = false;
      }
      else if(!_SaveCurrent())
      {
        bOK = false;
      }
      if(!bOK)
      {
        m_pChoice->SetStringSelection(m_sSelection);
      }
    }
    else if(n == wxID_CANCEL)
    {
      bOK = false;
    }
    else if(n == wxID_NO)
    {
      Restore();
    }
  }
  return bOK;
}
void CDialogVolumes::OnChangeVolume(wxCommandEvent &)
{
  wxString s = m_pChoice->GetStringSelection();
  if(s != m_sSelection)
  {
    bool bOK = _ChangeVolumeWarningOK();
    if(bOK)
    {
      m_sSelection = s;
      _SetVolume();
    }
    else
    {
      m_pChoice->SetStringSelection(m_sSelection);
    }
  }
}
void CDialogVolumes::OnLock(wxCommandEvent &)
{
  if(m_pVolumeCurrent == NULL) 
  {
     // shouldn't happen
    wxASSERT_MSG(0,"Attempted to lock w/ no volume");
  }
  else if(m_bReadOnly) {} // don't lock
  else if(m_pVolumeCurrent->Lock())
  {
    _SetupAll();
  }
  else
  {
    wxString sMessage = m_pVolumeCurrent->GetLastError();
    mainApp::ShowAlert(sMessage,this);
  }
}

bool CDialogVolumes::_SetupAll()
{
  bool bRtn = m_pPanelLab->SetupAll(m_pVolumeCurrent);
  if(bRtn)
  {
     m_labCopy = *m_pVolumeCurrent->GetLabSettings();
  }
  return bRtn;
}

void CDialogVolumes::OnAdd(wxCommandEvent &e)
{
  if(mainApp::SetupSiteSettings())
  {
    bool bButton = (e.GetEventObject() == (wxObject *)m_pButtonAdd);
    wxASSERT_MSG(bButton,"CDialogVolumes::OnAdd, wrong button");
    if(bButton && _ChangeVolumeWarningOK())
    {
      CDialogVolumeAddNew x(&m_volumes,this);
      if(x.ShowModal() == wxID_OK)
      {
        m_sSelection = x.GetVolName();
        _UpdateVolumeNames();
      }
    }
  }
}
void CDialogVolumes::OnRemove(wxCommandEvent &e)
{
  bool bButton = (e.GetEventObject() == (wxObject *)m_pButtonRemove);
  bool bNotReadOnly = 
    (!m_bReadOnly) && m_pVolumeCurrent->HasLock();
  wxASSERT_MSG(bButton,"CDialogVolumes::OnRemove, wrong button");
  wxASSERT_MSG(
    bNotReadOnly,
    "CDialogVolumes::OnRemove, attempt to remove a readonly or unlocked " Volume_string);
  if(bButton && bNotReadOnly)
  {
    wxString sMsg = "Do you wish to remove this " Volume_string ",\n";
    sMsg.Append(m_sSelection);
    wxMessageDialog dlg(this,sMsg,"Remove " VOLUME_STRING,
      wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dlg.CentreOnParent();
    if(dlg.ShowModal() == wxID_YES)
    {
      if(m_volumes.Remove(m_pVolumeCurrent))
      {
        m_pVolumeCurrent = NULL;
        m_sSelection.Empty();
        _UpdateVolumeNames();
      }
      else
      {
        mainApp::ShowError(m_volumes.GetLastError(),this);
      }
    }    
  }
}
void CDialogVolumes::OnShowFolder(wxCommandEvent &)
{
  wxFileName fn(m_pVolumeCurrent->GetLabSettingsFileName());
  if (fn.Exists())
  {
    nwxFileUtil::ShowFileFolder(fn.GetPath(), true);
  }
  else
  {
    wxString s(wxS("Cannot find folder for this selection"));
    mainApp::ShowAlert(s, this);
    s.Append("\n");
    s.Append(fn.GetFullPath());
    mainApp::LogMessage(s);
  }
}
void CDialogVolumes::OnRename(wxCommandEvent &e)
{
  bool bButton = (e.GetEventObject() == (wxObject *)m_pButtonRename);
  bool bNotReadOnly = 
    (!m_bReadOnly) &&  m_pVolumeCurrent->HasLock();
  wxASSERT_MSG(bButton,"CDialogVolumes::OnRename, wrong button");
  wxASSERT_MSG(
    bNotReadOnly,
    "CDialogVolumes::OnRename, attempt to remove a readonly or unlocked " Volume_string);
  if(bButton && bNotReadOnly)
  {
    CDialogVolumeRename x(&m_volumes,m_pVolumeCurrent,this);
    if(x.ShowModal() == wxID_OK)
    {
      const wxString &sName = m_pVolumeCurrent->GetVolumeName();
      if(sName != m_sSelection)
      {
        m_sSelection = sName;
        m_pVolumeCurrent = NULL;
        _UpdateVolumeNames();
      }
    }
  }
}

bool CDialogVolumes::_UpdateVolumeNames()
{
  const wxArrayString &as = m_volumes.GetVolumeNames();
  bool bSet0 = m_sSelection.IsEmpty();
  m_pChoice->Clear();
  m_pChoice->Append(as);
  bool bRtn = !as.IsEmpty();

  if(bSet0) {} // do not update selection
  else if(!m_pChoice->SetStringSelection(m_sSelection))
  {
    wxASSERT_MSG(0,m_sSelection);
    bSet0 = true;
    bRtn = false;
  }
  if(bSet0)
  {
    m_sSelection = m_pChoice->GetString(0);
    m_pChoice->Select(0);
  }
  _SetVolume();
  return bRtn;
}
void CDialogVolumes::OnTimer(wxTimerEvent &e) // called from mainFrame
{
  m_volumes.RefreshLocksOnTimer(e.GetInterval());
}
void CDialogVolumes::OnGridEditStart(wxGridEvent &e)
{
  m_bGridEditorShown = true;
  wxObject *pObj = e.GetEventObject(); 
  m_pGridObject = wxDynamicCast(pObj,wxGrid);
#ifdef __WXDEBUG__
  if(m_pGridObject != NULL)
  {
    nwxGrid::ComputeSize(m_pGridObject);
  }
#endif
  e.Skip();
}
void CDialogVolumes::OnGridEditEnd(wxGridEvent &e)
{
  m_bGridEditorShown = false;
  e.Skip();
}
bool CDialogVolumes::_IsGridEditorShown()
{
  bool bRtn = m_bGridEditorShown;
  if(bRtn && (m_pGridObject != NULL))
  {
    bRtn = m_pGridObject->IsCellEditControlEnabled();
    if(bRtn != m_bGridEditorShown)
    {
      m_bGridEditorShown = bRtn;
    }
  }
  return bRtn;
}

IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogVolumes)

BEGIN_EVENT_TABLE(CDialogVolumes,wxDialog)
EVT_BUTTON(wxID_OK,CDialogVolumes::OnOK)
EVT_BUTTON(IDlock,CDialogVolumes::OnLock)
EVT_BUTTON(wxID_CANCEL,CDialogVolumes::OnCancel)
EVT_BUTTON(wxID_APPLY,CDialogVolumes::OnApply)
EVT_BUTTON(IDadd,CDialogVolumes::OnAdd)
EVT_BUTTON(IDremove,CDialogVolumes::OnRemove)
EVT_BUTTON(IDrename,CDialogVolumes::OnRename)
EVT_BUTTON(IDfolder, CDialogVolumes::OnShowFolder)
EVT_CHOICE(IDvolume,CDialogVolumes::OnChangeVolume)
EVT_GRID_CMD_EDITOR_SHOWN(wxID_ANY, CDialogVolumes::OnGridEditStart) 
EVT_GRID_CMD_EDITOR_HIDDEN(wxID_ANY, CDialogVolumes::OnGridEditEnd) 
EVT_PERSISTENT_SIZE_POSITION(CDialogVolumes)
END_EVENT_TABLE()
