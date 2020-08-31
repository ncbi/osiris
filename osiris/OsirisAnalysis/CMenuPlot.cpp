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
*  FileName: CMenuPlot.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CMenuPlot.h"
#include "wxIDS.h"
#include "CKitColors.h"
#include "CPlotData.h"
#include "Platform.h"
#include "CGridLocusColumns.h"
#include "nwx/nwxString.h"
#include "nwx/nwxMenuItem.h"

//**********************************************  CMenuArtifact


CMenuArtifact::CMenuArtifact(int nOffset) 
  : wxMenu("",0), m_nOffset(nOffset)
{
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    AppendRadioItem(
      _Ndx2MenuID(i),CArtifactDisplayList::apsDisplay[i]);
  }
  SetDefault();
}
int CMenuArtifact::GetIntValue()
{
  int nRtn = 0;
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    if(IsChecked(_Ndx2MenuID(i)))
    {
      nRtn = CArtifactDisplayList::anValue[i];
      i = 32000; // loop exit
    }
  }
  return nRtn;
}

void CMenuArtifact::SetIntValue(int n)
{
  for(int i = 0; i < CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT; i++)
  {
    if(CArtifactDisplayList::anValue[i] == n)
    {
      Check(_Ndx2MenuID(i),true);
      i = 32000; // loop exit
    }
  }
}
int CMenuArtifact::_Ndx2MenuID(int ndx)
{
  return _ID(CArtifactDisplayList::anMenuID[ndx]);
}
void CMenuArtifact::SetDefault()
{
  Check(_Ndx2MenuID(CArtifactDisplayList::DEFAULT_NDX),true);
}


//**********************************************  CMenuPlot

IMPLEMENT_ABSTRACT_CLASS(CMenuPlot,wxMenu)

CMenuPlot::~CMenuPlot() {;}
CMenuPlot::CMenuPlot(
  CPlotData *pData, CKitColors *pColors)
    : wxMenu("",0),
    m_nOffset(0),
    m_nChannelCount(0),
    m_pKitColors(pColors),
    m_pData(pData),
    m_pMenuData(NULL),
    m_pMenuChannels(NULL),
    m_pMenuArtifact(NULL),
    m_pMenuLabels(NULL),
#if 0
    m_nLastType(LABEL_NONE),
#endif
    m_bXBPS(false),
    m_bPreview(true)
#if 0
    ,m_bUsedDefault(false)
#endif
{
  m_nOffset = ID_GET_PLOT_BASE(0);
  _Build(pData,pColors);
}

CMenuPlot::CMenuPlot(
  int nOffset, CPlotData *pData, CKitColors *pColors)
  : wxMenu("", 0),
  m_nOffset(nOffset),
  m_nChannelCount(0),
  m_pKitColors(pColors),
  m_pData(pData),
  m_pMenuData(NULL),
  m_pMenuChannels(NULL),
  m_pMenuArtifact(NULL),
  m_pMenuLabels(NULL),
#if 0
  m_nLastType(LABEL_NONE),
#endif
  m_bXBPS(false),
  m_bPreview(false)
#if 0
    ,m_bUsedDefault(false)
#endif
{
  _Build(pData,pColors);
}
CMenuPlot *CMenuPlot::Clone()
{
  CMenuPlot *pRtn = NULL;
  if(m_bPreview)
  {
    pRtn = new CMenuPlot(m_pData,m_pKitColors);
  }
  else
  {
    pRtn = new CMenuPlot(m_nOffset,m_pData,m_pKitColors);
  }
  pRtn->CopySettings(*this);
  return pRtn;
}
void CMenuPlot::_Build(CPlotData *pData, CKitColors *pColors) 
{
  wxString s;
  const CSingleKitColors *pKitColors =
    pColors->GetKitColors(pData->GetKitName());
  const CChannelColors *pChannelColors;
  wxMenuItem *pItem;
  int nID;
  unsigned int iu;

  m_nChannelCount = pData->GetChannelCount();
  unsigned int nChannelILS = pData->GetILSChannel();

  // build data menu

  m_pMenuData = new wxMenu;
  m_pMenuData->AppendCheckItem(
    _ID(IDmenuPlotDataAnalyzed),
    "Analyzed");
  m_pMenuData->AppendCheckItem(
    _ID(IDmenuPlotDataRaw),
    "Raw");
  m_pMenuData->AppendCheckItem(
    _ID(IDmenuPlotDataLadder),
    "Ladder");
  m_pMenuData->AppendCheckItem(
    _ID(IDmenuPlotDataBaseline),
    "Baseline");
  EnableBaseline(pData->HasBaseline());
  if(!m_bPreview)
  {
    m_pMenuData->Append(
      IDmenuDisplaySample,
      "Edit Sample");
    m_pMenuData->Append(
      IDmenuTable,
      "Show Table");
  }


  // channel menu

  m_pMenuChannels = new wxMenu;
  if(pKitColors == NULL)
  {
    for(iu = 1; iu <= m_nChannelCount; iu++)
    {
      nID = _ID( ID_GET_CHANNEL_ID_FROM_NR(iu) );
      s.Printf("Channel %u",iu);
      m_pMenuChannels->AppendCheckItem(nID,s);
    }
  }
  else
  {
    for(iu = 1; iu <= m_nChannelCount; iu++)
    {
      pChannelColors = (nChannelILS == iu)
        ?  pKitColors->GetColorChannelFromLS(pData->GetParameters().GetLsName())
        :  pKitColors->GetColorChannel(iu);
      if(pChannelColors != NULL)
      {
        nID = _ID(ID_GET_CHANNEL_ID_FROM_NR(iu));
        CGridLocusColumns::FORMAT_CHANNEL_DYE(&s,iu,(const wxChar *) pChannelColors->GetDyeName());
        pItem = new wxMenuItem(
          m_pMenuChannels,nID,s,"",wxITEM_CHECK);
#if COLOR_MENU_ITEMS
        pItem->SetBackgroundColour(pChannelColors->GetColorAnalyzed());
        pItem->SetTextColour(*wxWHITE);
#endif
        m_pMenuChannels->Append(pItem);
      }
    }
  }
  CMenuLabels::MENU_LABEL_TYPE nType = 
    m_bPreview
      ? CMenuLabels::MENU_TYPE_PREVIEW
      : CMenuLabels::MENU_TYPE_PLOT;
  m_pMenuLabels = new CMenuLabels(nType,m_nOffset);
  m_pMenuArtifact = new CMenuArtifact(m_nOffset);

  // now build the menu

  AppendSubMenu(m_pMenuData,"Data");
  AppendSubMenu(m_pMenuChannels,"Channel");
  if(!m_bPreview)
  {
    AppendCheckItem(
      _ID(IDmenuPlotSync),
      "Synchronize Axes");
  }
  AppendCheckItem(
    _ID(IDmenuPlotILS),
    "Show ILS");
  AppendCheckItem(
    _ID(IDmenuPlotRFU),
    "Show minimum RFU");
  
  if(m_bPreview)
  {
    AppendCheckItem(
      _ID(IDmenuPlotXBPS),
      "Show ILS BPS X-axis");
  }
  m_pMenuItemLabels = AppendSubMenu(m_pMenuLabels,"Labels");
  AppendSubMenu(m_pMenuArtifact,"Artifacts");
  AppendCheckItem(
    _ID(IDmenuPlotLadderLabels),
    "Show ladder labels");
  AppendCheckItem(
    _ID(IDmenuPlotLadderBins),
    "Show allele bins");
  if(!m_bPreview)
  {
    Append(_ID(IDmenuPlotResetAxes),
      "Reset Axes",
      "Reset axes to display all alleles");
    Append(_ID(IDmenuPlotResetAxesFull),
      "Show Primer Peaks",
      "Reset axes to include primer peaks");
    Append(_ID(IDmenuPlotMultiple),
      "Multiple Plots",
      "Show a plot for each channel");
    Append(_ID(IDmenuPlotAppend),
      "Append plot",
      "Append a new plot directly below this plot");
    Append(_ID(IDmenuPlotRemove),
      "Remove plot",
      "Remove this plot, keep all others");
    Append(_ID(IDmenuPlotRemoveOthers),
      "Remove other plots",
      "Remove all plots except this");
  }
  else
  {
    Append(IDMaxLadderLabels,
      LABEL_MAX_PEAK_LABELS,
      STATUS_MAX_PEAK_LABELS);
  }
}
const wxChar * const CMenuPlot::LABEL_MAX_PEAK_LABELS =
  wxS("Max. ladder peak labels...");
const wxChar * const CMenuPlot::STATUS_MAX_PEAK_LABELS =
  wxS("Set the maximum number of labels for ladder peaks");



void CMenuPlot::EnableLabelMenu(bool bEnable)
{
  m_pMenuItemLabels->Enable(bEnable);
}
bool CMenuPlot::IsLabelMenuEnabled()
{
  return m_pMenuItemLabels->IsEnabled();
}

bool CMenuPlot::AnalyzedValue()
{
  bool b = m_pMenuData->IsChecked(
    _ID(IDmenuPlotDataAnalyzed));
  return b;
}
bool CMenuPlot::RawValue()
{
  bool b = m_pMenuData->IsChecked(
    _ID(IDmenuPlotDataRaw));
  return b;
}
bool CMenuPlot::LadderValue()
{
  bool b = m_pMenuData->IsChecked(
    _ID(IDmenuPlotDataLadder));
  return b;
}

bool CMenuPlot::BaselineValue()
{
  bool b = m_pMenuData->IsChecked(
    _ID(IDmenuPlotDataBaseline));
  return b;
}
bool CMenuPlot::BaselineEnabled()
{
  bool b = m_pMenuData->IsEnabled(
    _ID(IDmenuPlotDataBaseline));
  return b;
}


//   data

void CMenuPlot::ShowAnalyzed(bool b)
{
  m_pMenuData->Check(_ID(IDmenuPlotDataAnalyzed),b);
}
void CMenuPlot::ShowRaw(bool b)
{
  m_pMenuData->Check(_ID(IDmenuPlotDataRaw),b);
}
void CMenuPlot::ShowLadder(bool b)
{
  m_pMenuData->Check(_ID(IDmenuPlotDataLadder),b);
}
void CMenuPlot::ShowBaseline(bool b)
{
  m_pMenuData->Check(_ID(IDmenuPlotDataBaseline),b);
}
void CMenuPlot::EnableBaseline(bool b)
{
  m_pMenuData->Enable(_ID(IDmenuPlotDataBaseline),b);
}

// channels

bool CMenuPlot::ChannelValue(unsigned int n)
{
  int nID = ID_GET_CHANNEL_ID_FROM_NR(n);
  return m_pMenuChannels->IsChecked(_ID(nID));
}
void CMenuPlot::ShowChannel(unsigned int n,bool b)
{
  int nID = ID_GET_CHANNEL_ID_FROM_NR(n);
  m_pMenuChannels->Check(_ID(nID),b);
}
int CMenuPlot::ChannelCount()
{
  return m_nChannelCount;
}

//  Sync, ILS, RFU

bool CMenuPlot::SyncValue()
{
  bool bRtn = false;
  if(!m_bPreview)
  {
    bRtn = IsChecked(_ID(IDmenuPlotSync));
  }
  return bRtn;
}
bool CMenuPlot::MinRfuValue()
{
  return IsChecked(_ID(IDmenuPlotRFU));
}

bool CMenuPlot::LadderLabels()
{
  return IsChecked(_ID(IDmenuPlotLadderLabels));
}   
bool CMenuPlot::LadderBins()
{
  return IsChecked(_ID(IDmenuPlotLadderBins));
}

bool CMenuPlot::ILSValue()
{
  return IsChecked(_ID(IDmenuPlotILS));
}
bool CMenuPlot::XBPSValue()
{
  return m_bPreview ? IsChecked(_ID(IDmenuPlotXBPS)) : m_bXBPS;
}

void CMenuPlot::SetXBPS(bool b)
{
  if(m_bPreview)
  {
    Check(_ID(IDmenuPlotXBPS),b);
  }
  else
  {
    m_bXBPS = b;
  }
}

void CMenuPlot::SetSync(bool b)
{
  if(!m_bPreview)
  {
    Check(_ID(IDmenuPlotSync),b);
  }
}
void CMenuPlot::ShowILS(bool b)
{
  Check(_ID(IDmenuPlotILS),b);
}
void CMenuPlot::ShowMinRfu(bool b)
{
  Check(_ID(IDmenuPlotRFU),b);
}
void CMenuPlot::ShowLadderLabels(bool b)
{
  Check(_ID(IDmenuPlotLadderLabels),b);
}
void CMenuPlot::ShowLadderBins(bool b)
{
  Check(_ID(IDmenuPlotLadderBins), b);
}
//  labels, artifacts

int CMenuPlot::ArtifactValue()
{
  return m_pMenuArtifact->GetIntValue();
}
#if 0
LABEL_PLOT_TYPE CMenuPlot::LabelType()
{
  LABEL_PLOT_TYPE n = 
    (LABEL_PLOT_TYPE)m_pMenuLabels->GetCheckedOffset();
  return n;
}
#endif
size_t CMenuPlot::GetLabelTypes(vector<unsigned int> *pan)
{
  size_t n = m_pMenuLabels->GetSelectionTypes(pan);
  return n;
}
void CMenuPlot::SetLabelTypes(const vector<unsigned int> &an)
{
  m_pMenuLabels->SetSelectionTypes(an);
}
void CMenuPlot::ClearLabelTypes()
{
  m_pMenuLabels->Clear();
}
void CMenuPlot::SetLabelType(LABEL_PLOT_TYPE n,LABEL_PLOT_TYPE nDefault)
{
  LABEL_PLOT_TYPE nType = CheckLabelType(n,nDefault);
  m_pMenuLabels->SelectByType(nType,true);
#if 0
  m_bUsedDefault = (nType != n);
  m_nLastType = nType;
#endif
}
void CMenuPlot::SetArtifactValue(int nLevel)
{
  m_pMenuArtifact->SetIntValue(nLevel);
}

//  enable/disable append, delete item

void CMenuPlot::EnableAppend(bool b)
{
  if(!m_bPreview)
  {
    Enable(_ID(IDmenuPlotAppend),b);
  }
}
bool CMenuPlot::IsAppendEnabled()
{
  bool bRtn = false;
  if(!m_bPreview)
  {
    bRtn = IsEnabled(_ID(IDmenuPlotAppend));
  }
  return bRtn;
}
void CMenuPlot::EnableDelete(bool b)
{
  if(!m_bPreview)
  {
    Enable(_ID(IDmenuPlotRemove),b);
    Enable(_ID(IDmenuPlotRemoveOthers),b);
  }
}
bool CMenuPlot::IsDeleteEnabled()
{
  bool bRtn = false;
  if(!m_bPreview)
  {
    bRtn = IsEnabled(_ID(IDmenuPlotRemove));
  }
  return bRtn;
}

void CMenuPlot::EnablePeakAreaLabel(bool b)
{
  m_pMenuLabels->EnablePeakAreaLabel(b);
}
bool CMenuPlot::PeakAreaLabelEnabled()
{
  return m_pMenuLabels->PeakAreaLabelEnabled();
}
bool CMenuPlot::SetStateFromEvent(wxCommandEvent &e)
{
  wxMenuItem *pItem = FindItem(e.GetId());
  bool bRtn = (pItem != NULL);
  if(bRtn && pItem->IsCheckable())
  {
    bool bCheck = e.IsChecked();
    if(pItem->IsChecked() != bCheck)
    {
      pItem->Check(bCheck);
    }
  }
  return bRtn;
}