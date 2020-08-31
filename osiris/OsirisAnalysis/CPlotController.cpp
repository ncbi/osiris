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
*  FileName: CPlotController.cpp
*  Author:   Douglas Hoffman
*
*  Base classes for creating consistent menubars and toolbar for plots
*/


#include "CPlotController.h"
#include "CPlotData.h"
#include "wxIDS.h"
//**********************************************  CArtifactDisplayList



const int CArtifactDisplayList::nArtifactLabelAll = ARTIFACT_ALL;
const int CArtifactDisplayList::nArtifactLabelNone = ARTIFACT_NONE;
const int CArtifactDisplayList::nArtifactLabelCritical = ARTIFACT_CRITICAL;

const int CArtifactDisplayList::DEFAULT_NDX = 2;
    // array index of 'critical' in the arrays

const char * const
  CArtifactDisplayList::apsDisplay
      [CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT] =
{
  "None",
  "All",
  "Critical"
};

const int
  CArtifactDisplayList::anValue
      [CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT] =
{
  CArtifactDisplayList::nArtifactLabelNone,
  CArtifactDisplayList::nArtifactLabelAll,
  CArtifactDisplayList::nArtifactLabelCritical
};

const int
  CArtifactDisplayList::anMenuID
      [CArtifactDisplayList::ARTIFACT_DISPLAY_COUNT] =
{
  IDmenuPlotArtifactsNone,
  IDmenuPlotArtifactsAll,
  IDmenuPlotArtifactsCritical
};

//**********************************************  CPlotController



int CPlotController::GetPlotTypes()
{
  int nRtn = 0;
  if(AnalyzedValue()) { nRtn |= CPlotData::TYPE_ANALYZED; }
  if(RawValue())      { nRtn |= CPlotData::TYPE_RAW; }
  if(LadderValue())   { nRtn |= CPlotData::TYPE_LADDER; }
  if(BaselineValue()) { nRtn |= CPlotData::TYPE_BASELINE; }
  return nRtn;
}

void CPlotController::CopySettings(CPlotController &w)
{
  int nChannels = ChannelCount();
  int nChannelw = w.ChannelCount();
  int n = (nChannels < nChannelw) ? nChannels : nChannelw;
  int i;

  // Analyzed, Raw, Ladder

  ShowAnalyzed(w.AnalyzedValue());
  ShowRaw(w.RawValue());
  ShowLadder(w.LadderValue());
  ShowBaseline(w.BaselineValue());
  EnableBaseline(w.BaselineEnabled());

  // channels

  //  n is the minimum number of channels in
  //  *this and w, copy settings from w

  for(i = 1; i <= n; i++)
  {
    ShowChannel(i,w.ChannelValue(i));
  }

  // if *this has more channels than w, (should never happen)
  // hide the remaining channels

  for(i = n + 1; i < nChannels; i++)
  {
    ShowChannel(i,false);
  }

  // labels, artifacts

//  SetLabelType(w.LabelType());
  vector<unsigned int> anTypes;
  w.GetLabelTypes(&anTypes);
  SetLabelTypes(anTypes);

  SetArtifactValue(w.ArtifactValue());

  // sync, ILS, RFU, time vs XBPS

  SetSync(w.SyncValue());
  ShowILS(w.ILSValue());
  ShowMinRfu(w.MinRfuValue());
  ShowLadderLabels(w.LadderLabels());
  ShowLadderBins(w.LadderBins());
  SetXBPS(w.XBPSValue());

  // append, delete

  EnableAppend(w.IsAppendEnabled());
  EnableDelete(w.IsDeleteEnabled());

  // enable peak area label
  EnablePeakAreaLabel(w.PeakAreaLabelEnabled());

  // enable label menu
  EnableLabelMenu(w.IsLabelMenuEnabled());
}

void CPlotController::ResetDefaults()
{
  ShowAnalyzed(true);
  ShowRaw(false);
  ShowLadder(false);
  ShowBaseline(false);

  SetSync(true);
  ShowILS(false);
  ShowMinRfu(false);
  ShowLadderLabels(false);
  ShowLadderBins(false);
  SetXBPS(false);

  SetArtifactValue(CArtifactDisplayList::nArtifactLabelCritical);
  SetLabelType(LABEL_ALLELE);
  EnableLabelMenu(true);
}

void CPlotController::ShowOneChannel(unsigned int n)
{
  int nChannels = ChannelCount();
  int i;
  for(i = 1; i <= nChannels; i++)
  {
    ShowChannel(i, i == (int)n);
  }
}
