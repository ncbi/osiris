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
*  FileName: CPlotController.h
*  Author:   Douglas Hoffman
*
*  Base classes for creating consistent menubars and toolbar for plots
*/
#ifndef __C_PLOT_CONTROLLER_H__
#define __C_PLOT_CONTROLLER_H__

#include "LABEL_TYPES.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"


//****************************************** CArtifactDisplayList

class CArtifactDisplayList
{
public:
  enum
  {
    ARTIFACT_DISPLAY_COUNT = 3
  };
  static const int nArtifactLabelAll;
  static const int nArtifactLabelCritical;
  static const int nArtifactLabelNone;
  static const int nArtifactDefault;
  static const int DEFAULT_NDX;
  static const char * const
    apsDisplay[ARTIFACT_DISPLAY_COUNT];
  static const int
    anValue[ARTIFACT_DISPLAY_COUNT];
  static const int
    anMenuID[ARTIFACT_DISPLAY_COUNT];
private:
  CArtifactDisplayList() {;} // prevent instantiation
};

//****************************************** IArtifactDisplayController

class IArtifactDisplayController
{
public:
  virtual ~IArtifactDisplayController(){;}
  virtual void SetIntValue(int n) = 0;
  virtual int GetIntValue() = 0;
  virtual void SetDefault() = 0;
};

//***************************************************** CPlotController

class CPlotController
{
protected:
  CPlotController() {;}
public:
  virtual ~CPlotController(){;}
  // data:
  virtual bool AnalyzedValue() = 0;
  virtual bool RawValue() = 0;
  virtual bool LadderValue() = 0;
  virtual bool BaselineValue() = 0;
  virtual bool BaselineEnabled() = 0;

  virtual void ShowAnalyzed(bool b = true) = 0;
  virtual void ShowRaw(bool b = true) = 0;
  virtual void ShowLadder(bool b = true) = 0;
  virtual void ShowBaseline(bool b = true) = 0;
  virtual void EnableBaseline(bool b = true) = 0;

  virtual int GetPlotTypes();

  // channels

  virtual bool ChannelValue(unsigned int n) = 0;
  virtual void ShowChannel(unsigned int n,bool b = true) = 0;
  virtual int ChannelCount() = 0;
  virtual void ShowOneChannel(unsigned int n);

  //  Sync, ILS, RFU, X Axis
  virtual bool SyncValue() = 0;
  virtual bool MinRfuValue() = 0;
  virtual bool LadderLabels() = 0;
  virtual bool ILSValue() = 0;
  virtual bool XBPSValue() = 0;

  virtual void SetSync(bool b = true) = 0;
  virtual void ShowILS(bool b = true) = 0;
  virtual void ShowMinRfu(bool b = true) = 0;
  virtual void ShowLadderLabels(bool b = true) = 0;
  virtual void SetXBPS(bool b = true) = 0;

  // labels, artifacts

#if 0
  virtual LABEL_PLOT_TYPE LabelType() = 0;
#endif
  virtual void SetLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault = LABEL_NONE) = 0;
  virtual size_t GetLabelTypes(vector<unsigned int> *pan) = 0;
  virtual void SetLabelTypes(const vector<unsigned int> &an) = 0;
  virtual size_t LabelTypeCount()
  {
    vector<unsigned int> an;
    size_t nRtn = GetLabelTypes(&an);
    return nRtn;
  }
  virtual void CopyLabelTypes(CPlotController *pc)
  {
    vector<unsigned int> an;
    pc->GetLabelTypes(&an);
    SetLabelTypes(an);
  }
  virtual int ArtifactValue() = 0;

  LABEL_PLOT_TYPE CheckLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault)
  {
    LABEL_PLOT_TYPE nRtn = ((n == LABEL_PEAK_AREA) && !PeakAreaLabelEnabled()) ? nDefault : n;
    return nRtn;
  }
  virtual void SetArtifactValue(int nLevel) = 0;

  // enable/disable Label menu
  virtual void EnableLabelMenu(bool bEnable = true) = 0;
  virtual bool IsLabelMenuEnabled() = 0;

  //  enable, disable append, delete buttons

  virtual void EnableAppend(bool b = true) = 0;
  virtual bool IsAppendEnabled() = 0;

  virtual void EnableDelete(bool b = true) = 0;
  virtual bool IsDeleteEnabled() = 0;

  // enable peak area label
  virtual void EnablePeakAreaLabel(bool b) = 0;
  virtual bool PeakAreaLabelEnabled() = 0;

  // copy, reset
  virtual void CopySettings(CPlotController &w);
  virtual void ResetDefaults();
};

#endif
