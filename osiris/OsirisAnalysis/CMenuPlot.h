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
*  FileName: CMenuPlot.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_PLOT_H__
#define __C_MENU_PLOT_H__

#include <wx/menu.h>
#include "CPlotController.h"
#include "CMenuLabels.h"
#include "LABEL_TYPES.h"
#include "wxIDS.h"

class CMenuLabels;
class CKitColors;
class CPlotData;

class CMenuArtifact : public wxMenu, IArtifactDisplayController
{
public:
  CMenuArtifact(int nOffset);
  virtual void SetIntValue(int n);
  virtual int GetIntValue();
  virtual void SetDefault();
private:
  int _Ndx2MenuID(int ndx);
  int _ID(int id)
  {
    return id + m_nOffset;
  }
  int m_nOffset;
};

class CMenuPlot : public wxMenu, public CPlotController
{
public:
  static const wxChar * const LABEL_MAX_PEAK_LABELS;
  static const wxChar * const STATUS_MAX_PEAK_LABELS;

  CMenuPlot(
    int m_nOffset,
    CPlotData *pData,
    CKitColors *pColors);
  CMenuPlot(
    CPlotData *pData,
    CKitColors *pColors);
      // lite menu used for plot preview in CFrameAnalysis
  virtual ~CMenuPlot();
  CMenuPlot *Clone();
  bool HasID(int nID)
  {
    bool bRtn = (nID >= m_nOffset) &&
      (nID < (m_nOffset + IDmenuPlot_MAX));
    return bRtn;
  }
  // enable/disable Label menu

  virtual void EnableLabelMenu(bool bEnable = true);
  virtual bool IsLabelMenuEnabled();

  // data:

  virtual bool AnalyzedValue();
  virtual bool RawValue();
  virtual bool LadderValue();
  virtual bool BaselineValue();
  virtual bool BaselineEnabled();
  virtual bool XBPSValue();

  virtual void ShowAnalyzed(bool b = true);
  virtual void ShowRaw(bool b = true);
  virtual void ShowLadder(bool b = true);
  virtual void ShowBaseline(bool b = true);
  virtual void EnableBaseline(bool b = true);
  virtual void SetXBPS(bool b = true);

  // channels

  virtual bool ChannelValue(unsigned int n);
  virtual void ShowChannel(unsigned int n,bool b = true);
  virtual int ChannelCount();

  //  Sync, ILS, RFU
  virtual bool SyncValue();
  virtual bool MinRfuValue();
  virtual bool LadderLabels();
  virtual bool LadderBins();
  virtual bool DisabledAlleles();
  virtual bool ILSValue();

  virtual void SetSync(bool b = true);
  virtual void ShowILS(bool b = true);
  virtual void ShowMinRfu(bool b = true);
  virtual void ShowLadderLabels(bool b = true);
  virtual void ShowLadderBins(bool b = true);
  virtual void ShowDisabledAlleles(bool b = true);

  // labels, artifacts
  virtual size_t GetLabelTypes(vector<unsigned int> *pan);
  virtual void SetLabelTypes(const vector<unsigned int> &an);
  virtual void SetLabelType(LABEL_PLOT_TYPE n, LABEL_PLOT_TYPE nDefault = LABEL_NONE);
  virtual int ArtifactValue();
  virtual void SetArtifactValue(int nLevel);


  //  enable, disable append, delete items

  virtual void EnableAppend(bool b = true);
  virtual bool IsAppendEnabled();

  virtual void EnableDelete(bool b = true);
  virtual bool IsDeleteEnabled();

  virtual void EnablePeakAreaLabel(bool b);
  virtual bool PeakAreaLabelEnabled();

  int GetLabelType()
  {
    vector<unsigned int> an;
    size_t nCount = GetLabelTypes(&an);
    int nRtn = 0;
    nRtn = (nCount >= 1) ? (int)an[0] : 0;
    return nRtn;
  }
  void ClearLabelTypes();
  bool SetStateFromEvent(wxCommandEvent &e);
  void SetToolbarLabel(bool bShowing);
  void SetScrollbarLabel(bool bShowing);
private:
  void _Build(CPlotData *pData, CKitColors *pColors);
  int _ID(int n)
  {
    return n + m_nOffset;
  }
  int m_nOffset;
  unsigned int m_nChannelCount;
  CKitColors *m_pKitColors;
  CPlotData *m_pData;
  wxMenuItem *m_pMenuItemLabels;
  wxMenu *m_pMenuData;
  wxMenu *m_pMenuChannels;
  CMenuArtifact *m_pMenuArtifact;
  CMenuLabels *m_pMenuLabels;
  bool m_bXBPS;
  bool m_bPreview;

  DECLARE_ABSTRACT_CLASS(CMenuPlot)
};


#endif

