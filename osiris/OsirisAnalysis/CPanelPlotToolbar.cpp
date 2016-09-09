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
#include <wx/arrstr.h>
#include <wx/colour.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "CPanelPlotToolbar.h"
#include "CPlotData.h"
#include "CKitColors.h"
#include "COARmisc.h"
#include "CPanelHistoryMenu.h"
#include "CMenuLabels.h"
#include "CButtonSample.h"
#include "Platform.h"
//**********************************************  CComboArtifact

CComboArtifact::CComboArtifact(wxWindow *parent, wxWindowID id) :
  wxComboBox()
{
  wxArrayString as;
  as.Alloc(CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT);
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    as.Add(CArtifactDisplayList::apsDisplay[i]);
  }
  Create(parent,id,as.Item(CArtifactDisplayList::DEFAULT_NDX),
    wxDefaultPosition, wxDefaultSize,
    as,wxCB_DROPDOWN | wxCB_READONLY);
  SetToolTip("Select artifacts to display." PLOT_TOOLBAR_SHIFT_ALL);
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
  int nMenuNumber,
  bool bFirst)
    : wxScrolledWindow(parent,wxID_ANY,wxDefaultPosition, wxDefaultSize,wxHSCROLL)
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
  EnableScrolling(true,false);
  SetScrollRate(1,0);
  ShowScrollbars(wxSHOW_SB_NEVER,  wxSHOW_SB_NEVER);

  if(nChannelCount > CHANNEL_MAX)
  {
    nChannelCount = CHANNEL_MAX;
  }
  m_nChannelCount = nChannelCount;
  m_pPanel = new wxPanel(this);
  pStatText = new wxStaticText(m_pPanel,wxID_ANY,"Data:");
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);

  // analyzed data button
  m_pButtonAnalyzed = new wxToggleButton(
    m_pPanel,IDgraphAnalyzed,"A",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonAnalyzed->SetValue(bFirst);
  m_pButtonAnalyzed->SetToolTip("View or hide analyzed data." PLOT_TOOLBAR_SHIFT_ALL);
  wxFont fnt = m_pButtonAnalyzed->GetFont();
  fnt.SetWeight(wxFONTWEIGHT_BOLD);
  m_pButtonAnalyzed->SetFont(fnt);
  pSizer->Add(m_pButtonAnalyzed,0,nSizerFlags,ID_BORDER);

  // raw data button
  m_pButtonRaw = new wxToggleButton(m_pPanel,IDgraphRaw,"R",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonRaw->SetToolTip("View or hide raw data." PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonRaw->SetFont(fnt);
  m_pButtonRaw->SetValue(false);
  pSizer->Add(m_pButtonRaw,0,nSizerFlags,ID_BORDER);

  // ladder data button
  m_pButtonLadder = new wxToggleButton(m_pPanel,IDgraphLadder,"L",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonLadder->SetToolTip("View or hide ladder data." PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonLadder->SetFont(fnt);
  m_pButtonLadder->SetValue(false);
  pSizer->Add(m_pButtonLadder,0,nSizerFlags,ID_BORDER);

  // baseline data button
  m_pButtonBaseline = new wxToggleButton(m_pPanel,IDgraphBaseline,"B",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonBaseline->SetToolTip("View or hide baseline data." PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonBaseline->SetFont(fnt);
  m_pButtonBaseline->SetValue(false);
  _EnableBaseline(pData->HasBaseline());
  pSizer->Add(m_pButtonBaseline,0,nSizerFlags,ID_BORDER);



  // view table
  wxButton *pButtonSample = new CButtonSample(m_pPanel);
  wxButton *pButtonTable =
    new wxButton(m_pPanel,IDgraphTable,"Table",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
  pButtonTable->SetToolTip("View a table containing the analysis data.");
  pSizer->Add(pButtonSample,0,nSizerFlags,ID_BORDER);
  pSizer->Add(pButtonTable,0,nSizerFlags,ID_BORDER);


  pSizer->AddSpacer(SPACER);
  wxString sLabel;
  int nID = IDgraphCH1;
  const CChannelColors *pCC = NULL;
  const CSingleKitColors *pKitColors = pColors->GetKitColors(pData->GetKitName());
  const wxColour *pColor;
  pStatText = new wxStaticText(m_pPanel,wxID_ANY,"Channel:");
  unsigned int nChannelILS = pData->GetILSChannel();

  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);
  for(i = 1; i <= nChannelCount; i++)
  {
    if(pKitColors != NULL)
    {
      pCC = (i == nChannelILS)
        ? pKitColors->GetColorChannelFromLS(pData->GetParameters().GetLsName())
        : pKitColors->GetColorChannel(i);
    }
    if(pCC != NULL)
    {
      pColor = pCC->GetColorAnalyzedPtr();
      sLabel = pCC->GetDyeName();
    }
    else
    {
      pColor = &CKitColors::g_BLACK;
      sLabel = wxString::Format(wxT("%u"), i);
    }
#if __USING_NATIVE_TOGGLE
    pPanelChannelButton = new wxPanel(m_pPanel,wxID_ANY);
    pSizerChannelButton = new wxBoxSizer(wxHORIZONTAL);
    pPanelChannelButton->SetBackgroundColour(*pColor);
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
    pTgl->SetForegroundColour(*wxWHITE);
    pTgl->SetMargins(DYEMARGIN,true);
    pSizer->Add(pTgl,0,nSizerFlags,ID_BORDER);
#endif
    m_pButtonChannel[i] = pTgl;
    pTgl->SetValue(bFirst);
    pTgl->SetToolTip(wxString::Format(
      "View or hide data for channel %d\n" 
         "Hold down the shift key to view only channel %d ",i,i));

    pTgl->SetBackgroundColour(*pColor);
  }
  for(i = nChannelCount + 1; i <= CHANNEL_MAX; i++)
  {
    m_pButtonChannel[i] = NULL;
  }
  m_pButtonChannel[0] = NULL;
  pSizer->AddSpacer(SPACER);

  // sync button
  m_pButtonSync = new wxToggleButton(m_pPanel,IDgraphSyncAxes,"Sync",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonSync->SetToolTip(
    "Syncrhonize axes on all plots when zooming.\n"
      "Hold down the shift key to set all plots.");
  m_pButtonSync->SetValue(bFirst);
  pSizer->Add(m_pButtonSync,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(m_pButtonSync);

  // ILS button

  m_pButtonILS = new wxToggleButton(m_pPanel,
    IDgraphILS, "ILS",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonILS->SetToolTip("Show or hide vertical lines for the ILS." PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonILS->SetValue(false);
  pSizer->Add(m_pButtonILS,0,nSizerFlags,ID_BORDER);


  // rfu button
  m_pButtonRfu = new wxToggleButton(m_pPanel,IDgraphRFU,"RFU",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonRfu->SetToolTip("View or hide minimum RFU." PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonRfu->SetValue(false);
  pSizer->Add(m_pButtonRfu,0,nSizerFlags,ID_BORDER);

  m_pButtonLadderLabels = new wxToggleButton(
    m_pPanel, IDgraphLadderLabels,"Ladder",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pButtonLadderLabels->SetToolTip(
    "View or hide ladder labels\nwhen ladder data are shown."
    PLOT_TOOLBAR_SHIFT_ALL);
  m_pButtonLadderLabels->SetValue(false);
  pSizer->Add(m_pButtonLadderLabels,0,nSizerFlags,ID_BORDER);

  // labels pulldown
  //
  //
  m_pMenuLabels = new CMenuLabels(CMenuLabels::MENU_TYPE_PLOT,nMenuNumber);
  m_pButtonLabels = new nwxButtonMenu(m_pMenuLabels,m_pPanel,wxID_ANY,wxS("Peak Labels"));
  pSizer->Add(m_pButtonLabels,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(m_pButtonLabels);

  pStatText = new wxStaticText(m_pPanel,wxID_ANY,"Artifacts:");
  pSizer->AddSpacer(ID_BORDER);
  pSizer->Add(pStatText, 0, nSizerFlags, ID_BORDER);
  m_vShiftWindows.push_back(pStatText);

  m_pComboArtifact = new CComboArtifact(m_pPanel,IDgraphArtifactCombo);
  BOX_COMBO(m_pComboArtifact,pSizer,nSizerFlags); // #defined in CComboLabels.h
  m_pPanelHistory = new CPanelHistoryMenu(
    GetParent()->GetParent(),
    m_pPanel,IDhistoryButton,"H");
  pSizer->Add(m_pPanelHistory,0,wxALIGN_CENTRE_VERTICAL,0);
  m_vShiftWindows.push_back(m_pPanelHistory);
  if(pMenu != NULL)
  {
    m_pPanelHistory->SetMenu(pMenu);
  }

  pSizer->AddSpacer(SPACER + ID_BORDER);

  // zoom out button
  pBtn = new wxButton(m_pPanel, IDgraphZoomOut,"Reset Axes",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip(
    "Reset axes to view all alleleic data\n"
      "Hold down the shift key to include primer peaks");
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(pBtn);

  // multiple button
  pBtn = new wxButton(m_pPanel, IDgraphMultiple,"Multiple",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip(
    "Create a separate plot for each channel\n"
    "or hold down the shift key to remove\n"
    "all plots except this"
    );
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);

  // append button
  m_pButtonAppend = new wxButton(m_pPanel, IDgraphAppend,"Append",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonAppend->SetToolTip("Add a new plot for a this sample");
  pSizer->Add(m_pButtonAppend,0,nSizerFlags,ID_BORDER);

  // big spacer
  pSizer->AddStretchSpacer(1);

  //  param button

  pBtn = new wxButton(m_pPanel,IDbuttonDetails,"Parameters...",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  pBtn->SetToolTip("Display analysis parameters");
  pSizer->Add(pBtn,0,nSizerFlags,ID_BORDER);
  m_vShiftWindows.push_back(pBtn);
  //  delete button

  m_pButtonDelete = new wxButton(m_pPanel,IDgraphRemove,"X",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT
    );
  m_pButtonDelete->SetToolTip(
    "Remove this plot or hold down the\n"
    "shift key to remove all plots\n"
    "except this");
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
    GetViewStart(&x1,&y);
    p->GetViewStart(&x2,&y);
    if(x1 != x2)
    {
      Scroll(x2,y);
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


void CPanelPlotToolbar::SetLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault)
{
  LABEL_PLOT_TYPE nType = CheckLabelType(n,nDefault);
  m_pMenuLabels->SelectByType(nType,true);
}
size_t CPanelPlotToolbar::GetLabelTypes(vector<unsigned int> *pan)
{
  size_t n = m_pMenuLabels->GetSelectionTypes(pan);
  return n;
}
void CPanelPlotToolbar::SetLabelTypes(const vector<unsigned int> &an)
{
  m_pMenuLabels->SetSelectionTypes(an);
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
  m_pButtonLabels->Enable(b);
}
bool CPanelPlotToolbar::IsLabelMenuEnabled()
{
  return m_pButtonLabels->IsEnabled();
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
  m_pMenuLabels->EnablePeakAreaLabel(b);
}
bool CPanelPlotToolbar::PeakAreaLabelEnabled()
{
  return m_pMenuLabels->PeakAreaLabelEnabled();
}


// IShiftReceiver functions

void CPanelPlotToolbar::ShiftLeft(bool bShiftKey)
{
  if(bShiftKey)
  {
    Scroll(0,0);
  }
  else
  {
    int xView;
    int y;
    GetViewStart(&xView,&y);
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
      Scroll(xLast,0);
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
    Scroll(xVirt - xSize,0);
  }
  else
  {
    GetViewStart(&xPos,&y);
    int xRight = xSize + xPos;
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
      Scroll(xLast,0);
    }
  }
}

bool CPanelPlotToolbar::CanShiftLeft()
{
  int x;
  int y;
  GetViewStart(&x,&y);
  return (x > 0);
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
    GetViewStart(&xPos,&y);
    bRtn = (xSize + xPos) < xVirt;
  }
  return bRtn;
}
