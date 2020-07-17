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
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*  FileName: CDialogPrintSettings.h
*  Author:   Douglas Hoffman
*
*  Dialog for printout.  Select curves, samples, zoom, labels, and page heading.
*
*/

#ifndef __C_DIALOG_PRINT_SETTINGS_H__
#define __C_DIALOG_PRINT_SETTINGS_H__

#include <wx/dialog.h>
#include "nwx/stdb.h"
#include <set>
#include <map>
#include "nwx/stde.h"
#include "nwx/PersistentSize.h"
#include "CParmOsiris.h"

class wxCheckBox;
class wxRadioButton;
class wxTextCtrl;
class wxButton;
class CFrameAnalysis;
class CPrintPreviewFrame;
class COARfile;
class COARsample;
class wxStaticText;
class wxSpinCtrl;

class CDialogPrintSettingsSample : public wxDialog
{
public:
  CDialogPrintSettingsSample(CPrintPreviewFrame *parent, const COARsample *pSample);
  virtual ~CDialogPrintSettingsSample();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();

  static int GetSampleType(const COARsample *pSample);
  static int GetPlotsPerPage(int nSampleType);
  static int GetPlotsPerPage(const COARsample *pSample)
  {
    return GetPlotsPerPage(GetSampleType(pSample));
  }

private:
  void _Build();

  typedef enum
  {
    SAMPLE_REGULAR,
    SAMPLE_LADDER,
    SAMPLE_NEGATIVE_CONTROL
  } SAMPLE_TYPE;
  CParmOsirisGlobal m_pParms;
  int m_nSampleType;
  const wxChar *m_psPlotsPerPageLabel;
  wxSpinCtrl *m_pTextPlotsPerPage;
  wxTextCtrl *m_pTextHeadingNotes;
  wxRadioButton *m_pRadioHeadingFile;
  wxRadioButton *m_pRadioHeadingSample;
  CPrintPreviewFrame *m_pParent;

  void _OnApply(wxCommandEvent &);
  void _OnOK(wxCommandEvent &);
  void _Apply();

  DECLARE_PERSISTENT_POSITION
  DECLARE_EVENT_TABLE()
};

class CDialogPrintSettings : public wxDialog
{
public:
  CDialogPrintSettings(CPrintPreviewFrame *parent, CFrameAnalysis *pFrame);
  virtual ~CDialogPrintSettings();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
private:
  static const int NO_RADIO_INT;
  static wxTextCtrl *_CreateNumericTextCtrl(
    wxWindow *parent, int nMin, int nMax, int nInit, int nID = wxID_ANY);
  void _EnableXScaleUser(bool b);
  void _EnableYScaleUser(bool b);
  void _SetupXScaleUser();
  void _SetupYScaleUser();
  void _SetupXScaleUserUnits();
  void _SetupLadderLabels();
  void _Build();
  int _GetRadioInt(wxRadioButton *p, int nDefault = NO_RADIO_INT);
  int _GetRadioValue(int nRadioGroup, int nDefault = NO_RADIO_INT);
  void _SetRadioValue(int nRadioGroup, int nValue);

  wxRadioButton *_CreateRadioButton(
    const wxString &label,
    int nValue,
    bool bNewGroup = false,
    int id = wxID_ANY);

  static void _SaveTextIntValue(wxTextCtrl *pText, int *pn, bool bOnlyIfEnabled = false);
  int _GetCurrentGroup()
  {
    return m_nRadioGroup;
  }

  // events

  void _OnButtonOmit(wxCommandEvent &);
  void _OnCurveLadders(wxCommandEvent &);
  void _OnYScale(wxCommandEvent &);
  void _OnXScale(wxCommandEvent &);
  void _OnXUnits(wxCommandEvent &);
  void _OnApply(wxCommandEvent &);
  void _OnOK(wxCommandEvent &);
  void _Apply();

  CParmOsirisGlobal m_pParms;
  int m_nRadioGroup;
  COARfile *m_pFile;
  CFrameAnalysis *m_pFrameAnalysis;
  CPrintPreviewFrame *m_pParent;

  // curve check boxes
  wxCheckBox *m_pCheckCurveAnalyzed;
  wxCheckBox *m_pCheckCurveRaw;
  wxCheckBox *m_pCheckCurveLadder;
  wxCheckBox *m_pCheckCurveLadderLabels;
  wxCheckBox *m_pCheckCurveBaseline;
  wxCheckBox *m_pCheckCurveILSvertical;
  wxCheckBox *m_pCheckCurveMinRFU;

  // samples
  wxCheckBox *m_pCheckSampleLadders;
  wxCheckBox *m_pCheckSamplePosCtrl;
  wxCheckBox *m_pCheckSampleNegCtrl;
  wxCheckBox *m_pCheckSampleDisabled;
  wxButton *m_pButtonSampleOmit;
  wxStaticText *m_pTextOmitCount;
  std::set<const COARsample*> m_setSamplesOmitDialog;

  // peak labels
  wxCheckBox *m_pCheckLabelAllele;
  wxCheckBox *m_pCheckLabelBPS;
  wxCheckBox *m_pCheckLabelRFU;
  wxCheckBox *m_pCheckLabelTime;
  wxCheckBox *m_pCheckLabelILSBPS;
  wxCheckBox *m_pCheckLabelPeakArea;

  // artifact labels

  int m_nGroupArtifact;
  wxRadioButton *m_pRadioArtifactAll;
  wxRadioButton *m_pRadioArtifactCritical;
  wxRadioButton *m_pRadioArtifactNone;

  // page heading

  int m_nGroupHeading;
  wxRadioButton *m_pRadioHeadingFile;
  wxRadioButton *m_pRadioHeadingSample;
  wxTextCtrl *m_pTextHeadingNotes;

  // channels per page
  wxSpinCtrl *m_pTextChannelsSamples;
  wxSpinCtrl *m_pTextChannelsLadders;
  wxSpinCtrl *m_pTextChannelsNegCtrl;
  wxCheckBox *m_pCheckChannelsOmitILS;

  // X Axis
  int m_nGroupXaxisUnits;
  wxRadioButton *m_pRadioXaxisTime;
  wxRadioButton *m_pRadioXaxisILSBPS;

  // X Axis Scale Primer

  int m_nGroupXscale;
  wxRadioButton *m_pRadioXscaleExcludePrimer;
  wxRadioButton *m_pRadioXscaleIncludePrimer;
  wxRadioButton *m_pRadioXscaleIncludePrimerNegCtrl;
  wxRadioButton *m_pRadioXscaleSpecify;
  wxTextCtrl *m_pTextXscaleMin;
  wxTextCtrl *m_pTextXscaleMax;
  wxStaticText *m_pLabelXscaleUnits;
  wxStaticText *m_pLabelXscaleUnitsTo;
  int m_nXscaleMin;
  int m_nXscaleMax;

  // Y Axis Scale

  int m_nGroupYscale;
  wxRadioButton *m_pRadioYscaleChannel;
  wxRadioButton *m_pRadioYscaleSample;
  wxRadioButton *m_pRadioYscaleSpecify;
  wxTextCtrl *m_pTextYscaleMin;
  wxTextCtrl *m_pTextYscaleMax;
  wxStaticText *m_pLabelYscaleTo;
  wxStaticText *m_pLabelYRFU;
  int m_nYscaleMin;
  int m_nYscaleMax;

  // negative control Y Axis scale
  int m_nGroupYscaleNegCtrl;
  wxRadioButton *m_pRadioYscaleNegCtrlData;
  wxRadioButton *m_pRadioYscaleNegCtrlRFU;
  wxRadioButton *m_pRadioYscaleNegCtrlILS;

  std::map<wxRadioButton *, int> m_mapRadioButtons;
  std::multimap<int, wxRadioButton *> m_mapRadioGroups;
  std::map<unsigned int, wxCheckBox *> m_mapDisplayLabels;

  void _SetLabelSelection(const std::vector<unsigned int> &an);
  void _GetLabelSelection(std::vector<unsigned int> *pan);
  void _BuildLabelSelection();

  DECLARE_PERSISTENT_POSITION
  DECLARE_EVENT_TABLE()
};

#endif