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
*  FileName: CPanelPlotToolbar.cpp
*  Author:   Douglas Hoffman
*
*  Provide a toolbar for each plot using the CPanelPlot class
*/
#include "mainApp.h"
#include <wx/arrstr.h>
#include <wx/colour.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "CPanelPlotToolbar.h"
#include "CPlotData.h"
#include "CKitColors.h"
#include "COARmisc.h"
#include "CPanelHistoryMenu.h"
#include "Platform.h"
//**********************************************  CComboArtifact

CComboArtifact::CComboArtifact(wxWindow *parent, wxWindowID id) :
  wxComboBox()
{
  wxArrayString as;
  as.Alloc(CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT);
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    as.Add(_T(CArtifactDisplayList::apsDisplay[i]));
  }
  Create(parent,id,as.Item(CArtifactDisplayList::DEFAULT_NDX),
    wxDefaultPosition, wxDefaultSize,
    as,wxCB_DROPDOWN | wxCB_READONLY);
  SetToolTip(_T("Select artifacts to display." PLOT_TOOLBAR_SHIFT_ALL));
}

void CComboArtifact::SetIntValue(int n)
{
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    if(CArtifactDisplayList::anValue[i] == n)
    {
      SetSelection(i);
      i = 32000; // loop exit
    }
  }
}
int CComboArtifact::GetIntValue()
{
  int n = GetSelection();
  int nRtn = CArtifactDisplayList::anValue[n];
  return nRtn;
}

void CComboArtifact::SetDefault()
{
  SetSelection(CArtifactDisplayList::DEFAULT_NDX);
}


//**********************************************  CPanelPlotToolbar


CPanelPlotToolbar::CPanelPlotToolbar(
  wxWindow *parent,
  CPlotData *pData,
  CKitColors *pColors,
  CMenuHistory *pMenu,
  bool bFirst)
    : wxScrolledWindow(parent,wxID_ANY,wxDefaultPosition, wxDefaultSize,0)
{
  wxSize MARGIN(2,0);
  wxButton *pBtn;
  wxStaticText *pStatText;
  wxBoxSizer *pSizer = new wxBoxSizer(wxHORIZONTAL);
#if __USING_NATIVE_TOGGLE
  wxPanel *pPanelChannelButton;
  wxBoxSizer *pSizerChannelButton;
#else
  wxSize DYEMARGIN(8,0);
#endif
  int nSizerFlags =
    wxTOP | wxBOTTOM | wxRIGHT |
    wxALIGN_CENTER_VERTICAL;

  unsigned int nChannelCount = pData->GetChannelCount();
  unsigned int i;
  int SPACER = ID_BORDER << 2;

  m_vShiftWindows.reserve(8);
  EnableScrolling(false,false);

  if(nChannelCount > CHANNEL_MAX)
  {
    nChannelCount = CHANNEL_MAX;
  }
  m_nChannelCount = nChannelCount;
  m_pPanel = new wxPanel(this);
  pStatText = new wxStaticText(m_pPanel,wxID_ANY,_T("Data:"));
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);

  // analyzed data button
  m_pButtonAnalyzed = new wxToggleButton(
    m_pPanel,IDgraphAnalyzed,_T("A"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonAnalyzed->SetValue(bFirst);
  m_pButtonAnalyzed->SetToolTip(_T("View or hide analyzed data." PLOT_TOOLBAR_SHIFT_ALL));
  wxFont fnt = m_pButtonAnalyzed->GetFont();
  fnt.SetWeight(wxFONTWEIGHT_BOLD);
  m_pButtonAnalyzed->SetFont(fnt);
  pSizer->Add(m_pButtonAnalyzed,0,nSizerFlags,ID_BORDER);

  // raw data button
  m_pButtonRaw = new wxToggleButton(m_pPanel,IDgraphRaw,_T("R"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonRaw->SetToolTip(_T("View or hide raw data." PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonRaw->SetFont(fnt);
  m_pButtonRaw->SetValue(false);
  pSizer->Add(m_pButtonRaw,0,nSizerFlags,ID_BORDER);

  // ladder data button
  m_pButtonLadder = new wxToggleButton(m_pPanel,IDgraphLadder,_T("L"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonLadder->SetToolTip(_T("View or hide ladder data." PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonLadder->SetFont(fnt);
  m_pButtonLadder->SetValue(false);
  pSizer->Add(m_pButtonLadder,0,nSizerFlags,ID_BORDER);

  // baseline data button
  m_pButtonBaseline = new wxToggleButton(m_pPanel,IDgraphBaseline,_T("B"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonBaseline->SetToolTip(_T("View or hide baseline data." PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonBaseline->SetFont(fnt);
  m_pButtonBaseline->SetValue(false);
  _EnableBaseline(pData->HasBaseline());
  pSizer->Add(m_pButtonBaseline,0,nSizerFlags,ID_BORDER);



  // view table
  wxButton *pButtonTable =
    new wxButton(m_pPanel,IDgraphTable,_T("Table"),
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
  pButtonTable->SetToolTip(_T("View a table containing the analysis data."));
  pSizer->Add(pButtonTable,0,nSizerFlags,ID_BORDER);


  pSizer->AddSpacer(SPACER);
  wxString sLabel;
  int nID = IDgraphCH1;
  char label[2] = {0,0};
  const CChannelColors *pCC;
  wxColour WHITE(255,255,255);
  pStatText = new wxStaticText(m_pPanel,wxID_ANY,_T("Channel:"));
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);

  for(i = 1; i <= nChannelCount; i++)
  {
    pCC = pColors->GetColorChannel(pData->GetKitName(),i);
    if(pCC != NULL)
    {
      sLabel = pCC->m_sDyeName;
    }
    else
    {
      label[0] = '0' + i;
      sLabel = label;
    }
    const wxColour &cl(pColors->GetColor(
      pData->GetKitName(),ANALYZED_DATA,i));
#if __USING_NATIVE_TOGGLE
    pPanelChannelButton = new wxPanel(m_pPanel,wxID_ANY);
    pSizerChannelButton = new wxBoxSizer(wxHORIZONTAL);
    pPanelChannelButton->SetBackgroundColour(cl);
    wxToggleButton *pTgl = new wxToggleButton(pPanelChannelButton,nID,sLabel,
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT
      );
    pSizerChannelButton->Add(pTgl,0,wxALL,2);
    pPanelChannelButton->SetSizer(pSizerChannelButton);
    pSizer->Add(pPanelChannelButton,0,nSizerFlags,0);
#else
    wxCustomButton *pTgl = new wxCustomButton(m_pPanel,nID,sLabel,
      wxDefaultPosition, wxDefaultSize, wxCUSTBUT_TOGGLE);
    pTgl->SetForegroundColour(WHITE);
    pTgl->SetMargins(DYEMARGIN,true);
    pSizer->Add(pTgl,0,nSizerFlags,ID_BORDER);
#endif
    m_pButtonChannel[i] = pTgl;
    pTgl->SetValue(bFirst);
    pTgl->SetToolTip(wxString::Format(
      _T("View or hide data for channel %d\n" 
         "Hold down the shift key to view only channel %d "),i,i));

    pTgl->SetBackgroundColour(cl);
  }
  for(i = nChannelCount + 1; i <= CHANNEL_MAX; i++)
  {
    m_pButtonChannel[i] = NULL;
  }
  m_pButtonChannel[0] = NULL;
  pSizer->AddSpacer(SPACER);

  // sync button
  m_pButtonSync = new wxToggleButton(m_pPanel,IDgraphSyncAxes,_T("Sync"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonSync->SetToolTip(
    _T("Syncrhonize axes on all plots when zooming.\n"
      "Hold down the shift key to set all plots."));
  m_pButtonSync->SetValue(bFirst);
  pSizer->Add(m_pButtonSync,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(m_pButtonSync);

  // ILS button

  m_pButtonILS = new wxToggleButton(m_pPanel,
    IDgraphILS, _T("ILS"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonILS->SetToolTip(_T("Show or hide vertical lines for the ILS." PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonILS->SetValue(false);
  pSizer->Add(m_pButtonILS,0,nSizerFlags,ID_BORDER);


  // rfu button
  m_pButtonRfu = new wxToggleButton(m_pPanel,IDgraphRFU,_T("RFU"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonRfu->SetToolTip(_T("View or hide minimum RFU." PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonRfu->SetValue(false);
  pSizer->Add(m_pButtonRfu,0,nSizerFlags,ID_BORDER);

  // labels label
  pStatText = new wxStaticText(m_pPanel,wxID_ANY,_T("Labels:"));
  pSizer->AddSpacer(SPACER);
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);


  // labels pulldown
  //
  //
  m_pComboLabels = new CComboLabels(m_pPanel,
    IDgraphLabelsCombo, true);
  BOX_COMBO(m_pComboLabels,pSizer,nSizerFlags); // #defined in CComboLabels.h

  m_pButtonLadderLabels = new wxToggleButton(
    m_pPanel, IDgraphLadderLabels,_T("Ladder"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pButtonLadderLabels->SetToolTip(
    _T("View or hide ladder labels\nwhen ladder data are shown."
    PLOT_TOOLBAR_SHIFT_ALL));
  m_pButtonLadderLabels->SetValue(false);
  pSizer->Add(m_pButtonLadderLabels,0,nSizerFlags,ID_BORDER);

  pStatText = new wxStaticText(m_pPanel,wxID_ANY,_T("Artifacts:"));
  pSizer->AddSpacer(ID_BORDER);
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);

  m_pComboArtifact = new CComboArtifact(m_pPanel,IDgraphArtifactCombo);
  BOX_COMBO(m_pComboArtifact,pSizer,nSizerFlags); // #defined in CComboLabels.h
  m_pPanelHistory = new CPanelHistoryMenu(
    GetParent()->GetParent(),
    m_pPanel,IDhistoryButton,_T("H"));
  pSizer->Add(m_pPanelHistory,0,wxALIGN_CENTRE_VERTICAL,0);
  m_vShiftWindows.push_back(m_pPanelHistory);
  if(pMenu != NULL)
  {
    m_pPanelHistory->SetMenu(pMenu);
  }

  pSizer->AddSpacer(SPACER + ID_BORDER);

  // zoom out button
  pBtn = new wxButton(m_pPanel, IDgraphZoomOut,_T("Reset Axes"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip(
    _T("Reset axes to view all alleleic data\n"
      "Hold down the shift key to include primer peaks"));
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(pBtn);

  // multiple button
  pBtn = new wxButton(m_pPanel, IDgraphMultiple,_T("Multiple"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip(_T(
    "Create a separate plot for each channel\n"
    "or hold down the shift key to remove\n"
    "all plots except this"
    ));
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);

  // append button
  m_pButtonAppend = new wxButton(m_pPanel, IDgraphAppend,_T("Append"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonAppend->SetToolTip(_T("Add a new plot for a this sample"));
  pSizer->Add(m_pButtonAppend,0,nSizerFlags,ID_BORDER);

  // big spacer
  pSizer->AddStretchSpacer(1);

  //  param button

  pBtn = new wxButton(m_pPanel,IDbuttonDetails,_T("Parameters..."),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip(_T("Display analysis parameters"));
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(pBtn);
  //  delete button

  m_pButtonDelete = new wxButton(m_pPanel,IDgraphRemove,_T("X"),
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonDelete->SetToolTip(_T(
    "Remove this plot or hold down the\n"
    "shift key to remove all plots\n"
    "except this"));
  m_pButtonDelete->SetFont(fnt);
  m_pButtonDelete->Enable(!bFirst);
  pSizer->Add(m_pButtonDelete,0,nSizerFlags ^ wxRIGHT,ID_BORDER);

  m_pPanel->SetSizer(pSizer);
  pSizer = new wxBoxSizer(wxHORIZONTAL);
  pSizer->Add(m_pPanel,1,wxEXPAND,0);
  SetSizer(pSizer);
  Layout();
  SetAutoLayout(true);
}



CPanelPlotToolbar::~CPanelPlotToolbar() {;}


// data
void CPanelPlotToolbar::SyncTo(CPanelPlotToolbar *p)
{
  if(p != this)
  {
    int x1;
    int x2;
    int y;
    m_pPanel->GetPosition(&x1,&y);
    p->m_pPanel->GetPosition(&x2,&y);
    if(x1 != x2)
    {
      m_pPanel->Move(x2,y);
    }
  }
}

void CPanelPlotToolbar::SetHistoryMenu(CMenuHistory *pMenu)
{
  m_pPanelHistory->SetMenu(pMenu,false);
}
void CPanelPlotToolbar::UpdateHistoryButtons()
{
  m_pPanelHistory->EnablePrevNextButtons();
}
bool CPanelPlotToolbar::AnalyzedValue()
{
  bool bRtn = m_pButtonAnalyzed->GetValue();
  return bRtn;
}
bool CPanelPlotToolbar::RawValue()
{
  return m_pButtonRaw->GetValue();
}
bool CPanelPlotToolbar::LadderValue()
{
  return m_pButtonLadder->GetValue();
}
bool CPanelPlotToolbar::BaselineValue()
{
  return m_pButtonBaseline->GetValue();
}
bool CPanelPlotToolbar::BaselineEnabled()
{
  return m_pButtonBaseline->IsEnabled();
}

void CPanelPlotToolbar::ShowAnalyzed(bool b)
{
  m_pButtonAnalyzed->SetValue(b);
}
void CPanelPlotToolbar::ShowRaw(bool b)
{
  m_pButtonRaw->SetValue(b);
}
void CPanelPlotToolbar::ShowLadder(bool b)
{
  m_pButtonLadder->SetValue(b);
}
void CPanelPlotToolbar::ShowBaseline(bool b)
{
  m_pButtonBaseline->SetValue(b);
}
void CPanelPlotToolbar::EnableBaseline(bool b)
{
  _EnableBaseline(b);
}


// channels

bool CPanelPlotToolbar::ChannelValue(unsigned int n)
{
  CHANNEL_BUTTON *p = GetChannelButton(n);
  bool bRtn = (p == NULL) ? false : p->GetValue();
  return bRtn;
}
void CPanelPlotToolbar::ShowChannel(unsigned int n,bool b)
{
  CHANNEL_BUTTON *p = GetChannelButton(n);
  if(p != NULL)
  {
    p->SetValue(b);
  }
}
int CPanelPlotToolbar::ChannelCount()
{
  return m_nChannelCount;
}

//  Sync, ILS, RFU

bool CPanelPlotToolbar::SyncValue()
{
  return m_pButtonSync->GetValue();
}
bool CPanelPlotToolbar::MinRfuValue()
{
  return m_pButtonRfu->GetValue();
}
bool CPanelPlotToolbar::LadderLabels()
{
  return m_pButtonLadderLabels->GetValue();
}
bool CPanelPlotToolbar::ILSValue()
{
  return m_pButtonILS->GetValue();
}
void CPanelPlotToolbar::SetSync(bool b)
{
  m_pButtonSync->SetValue(b);
}
void CPanelPlotToolbar::ShowILS(bool b)
{
  m_pButtonILS->SetValue(b);
}
void CPanelPlotToolbar::ShowMinRfu(bool b)
{
  m_pButtonRfu->SetValue(b);
}
void CPanelPlotToolbar::ShowLadderLabels(bool b)
{
  m_pButtonLadderLabels->SetValue(b);
}


// labels, artifacts

LABEL_PLOT_TYPE CPanelPlotToolbar::LabelType()
{
  return (LABEL_PLOT_TYPE) m_pComboLabels->GetSelection();
}
void CPanelPlotToolbar::SetLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault)
{
  n = CheckLabelType(n,nDefault);
  if(n != LabelType())
  {
    m_pComboLabels->Select((int)n);
  }
}
int CPanelPlotToolbar::ArtifactValue()
{
  return m_pComboArtifact->GetIntValue();
}
void CPanelPlotToolbar::SetArtifactValue(int nLevel)
{
  if(nLevel != ArtifactValue())
  {
    m_pComboArtifact->SetIntValue(nLevel);
  }
}

// enable/disable Label menu
void CPanelPlotToolbar::EnableLabelMenu(bool b)
{
  m_pComboLabels->Enable(b);
}
bool CPanelPlotToolbar::IsLabelMenuEnabled()
{
  return m_pComboLabels->IsEnabled();
}


//  enable, disable append, delete buttons

void CPanelPlotToolbar::EnableAppend(bool b)
{
  m_pButtonAppend->Enable(b);
}
bool CPanelPlotToolbar::IsAppendEnabled()
{
  return m_pButtonAppend->IsEnabled();
}
void CPanelPlotToolbar::EnableDelete(bool b)
{
  m_pButtonDelete->Enable(b);
}
bool CPanelPlotToolbar::IsDeleteEnabled()
{
  return m_pButtonDelete->IsEnabled();
}

void CPanelPlotToolbar::EnablePeakAreaLabel(bool b)
{
  if(b != PeakAreaLabelEnabled())
  {
    m_pComboLabels->EnablePeakAreaLabel(b);
    m_pPanel->Layout(); // combo box is a different size
    Layout();
  }
}
bool CPanelPlotToolbar::PeakAreaLabelEnabled()
{
  return m_pComboLabels->PeakAreaLabelEnabled();
}


// IShiftReceiver functions

void CPanelPlotToolbar::ShiftLeft(bool bShiftKey)
{
  if(bShiftKey)
  {
    m_pPanel->Move(0,0);
  }
  else
  {
    int xView;
    int y;
    m_pPanel->GetPosition(&xView,&y);
    xView = -xView;
    if(xView > 0)
    {
      // find last window whose right side is not shown
      vector<wxWindow *>::iterator itr;
      int xLast = 0;
      int xPos = 0;
      int xSz;
//      bool bFound = false;
      bool bFirstLoop = true;

      for(itr = m_vShiftWindows.begin();
        itr != m_vShiftWindows.end();
        ++itr)
      {
        if(bFirstLoop)
        {
          xPos = 0;
          bFirstLoop = false;
        }
        else
        {
          (*itr)->GetPosition(&xPos,&y);
        }
        (*itr)->GetSize(&xSz,&y);
        if((xPos + xSz) < xView)
        {
          xLast = xPos;
        }
        else
        {
          break;
        }
      }
      m_pPanel->Move(-xLast,0);
    }
  }
}
void CPanelPlotToolbar::ShiftRight(bool bShiftKey)
{
  int xSize;
  int xVirt;
  int xPos;
  int y;
  GetSize(&xSize,&y);
  GetVirtualSize(&xVirt,&y);
  if(bShiftKey)
  {
    m_pPanel->Move(xSize - xVirt,0);
  }
  else
  {
    m_pPanel->GetPosition(&xPos,&y);
    int xRight = xSize - xPos;
    if(xRight < xVirt)
    {
      // find leftmost window whose left side is not shown
      // start at the right and go left
      int xLast = xVirt;
      vector<wxWindow *>::reverse_iterator itr;
      for(itr = m_vShiftWindows.rbegin();
        itr != m_vShiftWindows.rend();
        ++itr)
      {
        (*itr)->GetPosition(&xPos,&y);
        if(xPos > xRight)
        {
          xLast = xPos;
        }
        else
        {
          break; // we are done
        }
      }
      xLast -= xSize;
      if(xLast < 0)
      {
        xLast = 0;
      }
      m_pPanel->Move(-xLast,0);
    }
  }
}

bool CPanelPlotToolbar::CanShiftLeft()
{
  int x;
  int y;
  m_pPanel->GetPosition(&x,&y);
  return (x < 0);
}
bool CPanelPlotToolbar::CanShiftRight()
{
  int xPos;
  int xVirt;
  int xSize;
  int y;
  bool bRtn = false;
  GetSize(&xSize,&y);
  GetVirtualSize(&xVirt,&y);
  if(xVirt > xSize)
  {
    m_pPanel->GetPosition(&xPos,&y);
    bRtn = (xSize - xPos) < xVirt;
  }
  return bRtn;
}
