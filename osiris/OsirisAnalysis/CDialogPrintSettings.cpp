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
*  FileName: CDialogPrintSettings.cpp
*  Author:   Douglas Hoffman
*
*  Dialog for printout.  Select curves, samples, zoom, labels, and page heading.
*
*/

#include "CDialogPrintSettings.h"
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/valnum.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/checklst.h>
#include <wx/hyperlink.h>
#include "mainApp.h"
#include "COARfile.h"
#include "CPrintOut.h"
#include "CPrintParameters.h"
#include "CPrintOutAnalysis.h"
#include "CFrameAnalysis.h"
#include "CPrintPreview.h"
#include "CPanelPlot.h"
#include "nwx/nwxTextCtrl.h"

// widget labels

#define S_WINDOW_TITLE "Print Settings"
#define S_WINDOW_OMIT_TITLE "Omit samples"

#define S_SECTION_PEAKS "Peaks"
#define S_SECTION_SAMPLES "Samples"
#define S_SECTION_PEAK_LABELS "Peak labels"
#define S_SECTION_ARTIFACT_LABELS "Artifact labels"
#define S_SECTION_PAGE_HEADING "Page heading"
#define S_SECTION_CHANNELS_PER_PAGE "Channels per page"
#define S_SECTION_X_UNITS "X-axis units"
#define S_SECTION_SCALE_X_AXIS "Scale X-axis - Primer Peaks"
#define S_SECTION_SCALE_Y_AXIS "Scale Y-axis"

#define S_ANALYZED "Analyzed"
#define S_RAW "Raw"
#define S_LADDERS "Ladders"
#define S_LADDER_LABELS "Ladder labels"
#define S_LADDER_BINS "Ladder bins"
#define S_BASELINE "Baseline"
#define S_ILS_LINES "ILS vertical lines"
#define S_MIN_RFU_THRESHOLD "Minimum RFU threshold"
#define S_POS_CTRL "Positive controls"
#define S_NEG_CTRL "Negative controls"
#define S_DISABLED "Disabled"
#define S_OMIT_SAMPLES "Omit samples..."
#define S_ALLELES "Alleles"
#define S_BASE_PAIRS "Base pairs"
#define S_RFU "RFU"
#define S_TIME "Time"
#define S_ILS_REF_BPS "ILS reference base pairs"
#define S_PEAK_AREA "Peak area"
#define S_INCLUDE_DISABLED "Include disabled Alleles"
#define S_ALL "All"
#define S_NONE "None"
#define S_CRITICAL "Critical"
#define S_FILE_NAME "File name"
#define S_SAMPLE_NAME "Sample name"
#define S_EXCLUDE_PRIMER_PEAKS "Exclude"
#define S_INCLUDE_PRIMER_PEAKS "Include"
#define S_NEG_CTRL_PRIMER_PEAKS "Include neg. control only"
#define S_SPECIFY "Specify..."
#define S_SCALE_CHANNEL "Scale each channel"
#define S_SCALE_SAMPLE "Scale to entire sample"
#define S_SCALE_NEG_CTRL_PEAK "Scale to peak data"
#define S_SCALE_NEG_CTRL_RFU "Include minimum RFU"
#define S_SCALE_NEG_CTRL_ILS "Scale to ILS"
#define S_PAGE_TITLE "Page title"
#define S_PAGE_NOTES "Notes"
#define S_SAMPLES "Samples"
#define S_OMIT_ILS "Omit ILS channel"
#define S_TO " to "
#define S_RANGE_SECONDS "seconds"
#define S_RANGE_BPS "bps"
#define S_PLOTS_PER_PAGE "Plots per page"
#define S_TEXT_WIDTH "99999999"
#define LABEL(s) new wxStaticText(this, wxID_ANY, wxT(s))

#define LABEL_ARGS_TOP 0, wxALIGN_LEFT | wxBOTTOM, ID_BORDER
#define LABEL_ARGS 0, wxALIGN_LEFT | wxBOTTOM | wxTOP, ID_BORDER
#define ITEM_ARGS 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, ID_BORDER
#define ITEM_ARGS_EXPAND 0, wxLEFT | wxBOTTOM | wxEXPAND, ID_BORDER


#define __OUT_OF_RANGE 0x7fffffff

// CDialogPrintSettingsOmit - declaration

class CDialogPrintSettingsOmit : public wxDialog
{
public:
  CDialogPrintSettingsOmit(
    wxWindow *parent,
    bool bShowFileName,
    std::vector<const COARsample *> *pSamples,
    std::set<const COARsample *> *pSamplesOmit);
  virtual ~CDialogPrintSettingsOmit();
  virtual bool TransferDataToWindow();
  virtual bool TransferDataFromWindow();
private:
  void _OnClearLink(wxHyperlinkEvent &);
  void _OnInvertLink(wxHyperlinkEvent &);
  std::vector<const COARsample *> *m_pSamples;
  std::set<const COARsample *> *m_pSamplesOmit;
  wxCheckListBox *m_pListBox;
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_EVENT_TABLE()
};

// CDialogPrintSettings


CDialogPrintSettings::CDialogPrintSettings(
      CPrintPreviewFrame *parent,
      CFrameAnalysis *pFrame) :
  wxDialog(parent, wxID_ANY, wxT(S_WINDOW_TITLE),
    GET_PERSISTENT_POSITION(CDialogPrintSettings),
    wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP),
  m_pFile(pFrame->GetOARfile()),
  m_pFrameAnalysis(pFrame),
  m_pParent(parent)
{
  m_nRadioGroup = -1;
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsCreate"));
  _Build();
}
CDialogPrintSettings::~CDialogPrintSettings()
{
}

const int CDialogPrintSettings::NO_RADIO_INT = -32768;

nwxTextCtrlInteger *CDialogPrintSettings::_CreateNumericTextCtrl(
  wxWindow *parent, int nMin, int nMax, int nInit, int nID)
{
  nwxTextCtrlInteger *pRtn = new 
    nwxTextCtrlInteger(parent, nID, nMin, nMax, nInit, 6, 0L);
  pRtn->Enable(false);
  return pRtn;
}


int CDialogPrintSettings::_GetRadioInt(wxRadioButton *p, int nDefault)
{
  int nRtn = nDefault;
  std::map<wxRadioButton *, int>::iterator itr = m_mapRadioButtons.find(p);
  if (itr != m_mapRadioButtons.end())
  {
    nRtn = itr->second;
  }
  return nRtn;
}
int CDialogPrintSettings::_GetRadioValue(int nRadioGroup, int nDefault)
{
  int rtn = nDefault;
  std::pair< std::multimap<int, wxRadioButton *>::iterator, std::multimap<int, wxRadioButton *>::iterator> iPair =
    m_mapRadioGroups.equal_range(nRadioGroup);
  for (std::multimap<int, wxRadioButton *>::iterator itr = iPair.first; itr != iPair.second; ++itr)
  {
    if (itr->second->GetValue())
    {
      rtn = _GetRadioInt(itr->second, nDefault);
      break;
    }
  }
  return rtn;
}

void CDialogPrintSettings::_SetRadioValue(int nRadioGroup, int nValue)
{
  std::pair< std::multimap<int, wxRadioButton *>::iterator, std::multimap<int, wxRadioButton *>::iterator> iPair =
    m_mapRadioGroups.equal_range(nRadioGroup);
  for (std::multimap<int, wxRadioButton *>::iterator itr = iPair.first; itr != iPair.second; ++itr)
  {
    if (_GetRadioInt(itr->second) == nValue)
    {
      itr->second->SetValue(true);
      break;
    }
  }
}

wxRadioButton *CDialogPrintSettings::_CreateRadioButton(
  const wxString &sLabel,
  int nValue,
  bool bNewGroup,
  int id)
{
  int nStyle = 0;
  if (bNewGroup)
  {
    m_nRadioGroup++;
    nStyle = wxRB_GROUP;
  }
  wxRadioButton *pRtn = new wxRadioButton(this, id, sLabel,
    wxDefaultPosition,
    wxDefaultSize, nStyle);
  m_mapRadioButtons.insert(std::map<wxRadioButton *, int>::value_type(pRtn, nValue));
  m_mapRadioGroups.insert(std::multimap<int, wxRadioButton *>::value_type(m_nRadioGroup, pRtn));
  return pRtn;
}


void CDialogPrintSettings::_Build()
{
  // curves
  wxSize szText = GetTextExtent(wxT(S_TEXT_WIDTH));
  szText.SetHeight(-1);

  m_pCheckCurveAnalyzed = new wxCheckBox(this, wxID_ANY, wxT(S_ANALYZED));
  m_pCheckCurveRaw = new wxCheckBox(this, wxID_ANY, wxT(S_RAW));
  m_pCheckCurveLadder = new wxCheckBox(this, IDprintCurveLadders, wxT(S_LADDERS));
  m_pCheckCurveLadderLabels = new wxCheckBox(this, wxID_ANY, wxT(S_LADDER_LABELS));
  m_pCheckCurveLadderBins = new wxCheckBox(this, wxID_ANY, wxT(S_LADDER_BINS));
  m_pCheckCurveBaseline = new wxCheckBox(this, wxID_ANY, wxT(S_BASELINE));
  m_pCheckCurveILSvertical = new wxCheckBox(this, wxID_ANY, wxT(S_ILS_LINES));
  m_pCheckCurveMinRFU = new wxCheckBox(this, wxID_ANY, wxT(S_MIN_RFU_THRESHOLD));

  // samples


  m_pCheckSampleLadders = new wxCheckBox(this, wxID_ANY, wxT(S_LADDERS));
  m_pCheckSamplePosCtrl = new wxCheckBox(this, wxID_ANY, wxT(S_POS_CTRL));
  m_pCheckSampleNegCtrl = new wxCheckBox(this, wxID_ANY, wxT(S_NEG_CTRL));
  m_pCheckSampleDisabled = new wxCheckBox(this, wxID_ANY, wxT(S_DISABLED));
  m_pButtonSampleOmit = new wxButton(this, IDprintSampleOmit, wxT(S_OMIT_SAMPLES));

  // Peak labels


  m_pCheckLabelAllele = new wxCheckBox(this, wxID_ANY, wxT(S_ALLELES));
  m_pCheckLabelBPS = new wxCheckBox(this, wxID_ANY, wxT(S_BASE_PAIRS));
  m_pCheckLabelRFU = new wxCheckBox(this, wxID_ANY, wxT(S_RFU));
  m_pCheckLabelTime = new wxCheckBox(this, wxID_ANY, wxT(S_TIME));
  m_pCheckLabelILSBPS = new wxCheckBox(this, wxID_ANY, wxT(S_ILS_REF_BPS));
  m_pCheckLabelPeakArea = new wxCheckBox(this, wxID_ANY, wxT(S_PEAK_AREA));
  m_pCheckLabelIncludeDisabled = new wxCheckBox(this, wxID_ANY, wxT(S_INCLUDE_DISABLED));
  wxFont f(m_pCheckLabelIncludeDisabled->GetFont());
  f.MakeItalic();
  m_pCheckLabelIncludeDisabled->SetFont(f);

  // Artifact Labels

  m_pRadioArtifactNone = _CreateRadioButton(wxT(S_NONE), PRINT_LABEL_ARTIFACT_NONE, true);
  m_pRadioArtifactAll = _CreateRadioButton(wxT(S_ALL), PRINT_LABEL_ARTIFACT_ALL);
  m_pRadioArtifactCritical = _CreateRadioButton(wxT(S_CRITICAL), PRINT_LABEL_ARTIFACT_CRITICAL);
  m_nGroupArtifact = _GetCurrentGroup();


  // page heading

  m_pRadioHeadingFile = _CreateRadioButton(wxT(S_FILE_NAME), PRINT_TITLE_FILE_NAME, true);
  m_pRadioHeadingSample = _CreateRadioButton(wxT(S_SAMPLE_NAME), PRINT_TITLE_SAMPLE_NAME);
  m_nGroupHeading = _GetCurrentGroup();
  m_pTextHeadingNotes = new wxTextCtrl(this, wxID_ANY);

  // Channels Per Page
  m_pTextChannelsSamples = new  wxSpinCtrl(this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, szText, wxSP_ARROW_KEYS,
    1, CHANNEL_MAX, CHANNEL_MAX);
  m_pTextChannelsLadders = new  wxSpinCtrl(this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, szText, wxSP_ARROW_KEYS,
    1, CHANNEL_MAX, CHANNEL_MAX);
  m_pTextChannelsNegCtrl = new  wxSpinCtrl(this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, szText, wxSP_ARROW_KEYS,
    1, CHANNEL_MAX, CHANNEL_MAX);
  m_pCheckChannelsOmitILS = new wxCheckBox(this, wxID_ANY, wxT(S_OMIT_ILS));

  // X-Axis
  m_pRadioXaxisTime = _CreateRadioButton(wxT(S_TIME), PRINT_X_TIME, true, IDprintXaxisILSBPS);
  m_pRadioXaxisILSBPS = _CreateRadioButton(wxT(S_ILS_REF_BPS), PRINT_X_BPS, false, IDprintXaxisILSBPS);
  m_nGroupXaxisUnits = _GetCurrentGroup();


  // Scale X-Axis
  m_pRadioXscaleExcludePrimer = _CreateRadioButton(wxT(S_EXCLUDE_PRIMER_PEAKS), PRINT_X_SCALE_NO_PRIMER_PEAK, true, IDprintXscaleSpecify);
  m_pRadioXscaleIncludePrimer = _CreateRadioButton(wxT(S_INCLUDE_PRIMER_PEAKS), PRINT_X_SCALE_ALL_PRIMER_PEAK, false, IDprintXscaleSpecify);
  m_pRadioXscaleIncludePrimerNegCtrl = _CreateRadioButton(wxT(S_NEG_CTRL_PRIMER_PEAKS), PRINT_X_SCALE_CTRL_PRIMER_PEAK, false, IDprintXscaleSpecify);
  m_pRadioXscaleSpecify = _CreateRadioButton(wxT(S_SPECIFY), PRINT_X_SCALE_USER, false, IDprintXscaleSpecify);
  m_nGroupXscale = _GetCurrentGroup();
  m_pTextXscaleMin = _CreateNumericTextCtrl(this, -999, 99999, __OUT_OF_RANGE);
  m_pTextXscaleMax = _CreateNumericTextCtrl(this, -999, 99999, __OUT_OF_RANGE);
  m_pTextXscaleMin->Enable(false);
  m_pTextXscaleMax->Enable(false);

  // scale Y-Axis

  m_pRadioYscaleChannel = _CreateRadioButton(wxT(S_SCALE_CHANNEL), PRINT_Y_SCALE_CHANNEL, true, IDprintYscaleSpecify);
  m_pRadioYscaleSample = _CreateRadioButton(wxT(S_SCALE_SAMPLE), PRINT_Y_SCALE_SAMPLE, false, IDprintYscaleSpecify);
  m_pRadioYscaleSpecify = _CreateRadioButton(wxT(S_SPECIFY), PRINT_Y_SCALE_USER, false, IDprintYscaleSpecify);
  m_nGroupYscale = _GetCurrentGroup();

  m_pTextYscaleMin = _CreateNumericTextCtrl(this, -50000, 50000, __OUT_OF_RANGE);
  m_pTextYscaleMax = _CreateNumericTextCtrl(this, -50000, 50000, __OUT_OF_RANGE);
  m_pTextYscaleMin->Enable(false);
  m_pTextYscaleMax->Enable(false);
  m_pRadioYscaleNegCtrlData = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_PEAK), PRINT_Y_SCALE_NEG_PEAKS, true);
  m_pRadioYscaleNegCtrlRFU = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_RFU), PRINT_Y_SCALE_NEG_INCLUDE_RFU);
  m_pRadioYscaleNegCtrlILS = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_ILS), PRINT_Y_SCALE_NEG_ILS);
  m_nGroupYscaleNegCtrl = _GetCurrentGroup();

  // build the panel

  wxBoxSizer *pSizerLeft = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerRight = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerItems = pSizerLeft;
  wxBoxSizer *pSizerTemp;
  wxStaticText *pText;

  // Peaks
  pText = LABEL(S_SECTION_PEAKS);
  wxFont fontLabelBold = pText->GetFont();
  fontLabelBold.MakeBold();
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS_TOP);
  pSizerItems->Add(m_pCheckCurveAnalyzed, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveRaw, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveLadder, ITEM_ARGS);
  pSizerTemp = new wxBoxSizer(wxHORIZONTAL);
  pSizerTemp->AddSpacer(ID_BORDER << 1);
  pSizerTemp->Add(m_pCheckCurveLadderLabels, 0, 0, 0);
  pSizerItems->Add(pSizerTemp, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveLadderBins, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveBaseline, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveILSvertical, ITEM_ARGS);
  pSizerItems->Add(m_pCheckCurveMinRFU, ITEM_ARGS);

  // Samples
  pText = LABEL(S_SECTION_SAMPLES);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pCheckSampleLadders, ITEM_ARGS);
  pSizerItems->Add(m_pCheckSamplePosCtrl, ITEM_ARGS);
  pSizerItems->Add(m_pCheckSampleNegCtrl, ITEM_ARGS);
  pSizerItems->Add(m_pCheckSampleDisabled, ITEM_ARGS);
  pSizerTemp = new wxBoxSizer(wxHORIZONTAL);
  m_pTextOmitCount = LABEL("          ");
  pSizerTemp->Add(m_pButtonSampleOmit, 0,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 0);
  pSizerTemp->Add(m_pTextOmitCount, 0,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT | wxRIGHT, ID_BORDER);
  pSizerItems->Add(pSizerTemp, ITEM_ARGS);

  // Peak Labels
  pText = LABEL(S_SECTION_PEAK_LABELS);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pCheckLabelAllele, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelBPS, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelRFU, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelTime, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelILSBPS, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelPeakArea, ITEM_ARGS);
  pSizerItems->Add(m_pCheckLabelIncludeDisabled, ITEM_ARGS);


  // Artifact Labels
  pText =LABEL(S_SECTION_ARTIFACT_LABELS);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pRadioArtifactNone, ITEM_ARGS);
  pSizerItems->Add(m_pRadioArtifactAll, ITEM_ARGS);
  pSizerItems->Add(m_pRadioArtifactCritical, ITEM_ARGS);

  // X-Axis Units
  pText = LABEL(S_SECTION_X_UNITS);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pRadioXaxisTime, ITEM_ARGS);
  pSizerItems->Add(m_pRadioXaxisILSBPS, ITEM_ARGS);


  // Begin right column
  pSizerItems = pSizerRight;

  // Page Heading
  pText = LABEL(S_SECTION_PAGE_HEADING);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS_TOP);
  pSizerItems->Add(LABEL(S_PAGE_TITLE), ITEM_ARGS);
  pSizerItems->Add(m_pRadioHeadingFile, ITEM_ARGS);
  pSizerItems->Add(m_pRadioHeadingSample, ITEM_ARGS);
  pSizerItems->Add(LABEL(S_PAGE_NOTES), ITEM_ARGS);
  pSizerItems->Add(m_pTextHeadingNotes, ITEM_ARGS_EXPAND);

  // Channels per page
  pText = LABEL(S_SECTION_CHANNELS_PER_PAGE);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  wxFlexGridSizer *pGrid = new wxFlexGridSizer(3, 2, ID_BORDER, ID_BORDER);
  pGrid->Add(m_pTextChannelsSamples, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGrid->Add(LABEL(S_SAMPLES), 1, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGrid->Add(m_pTextChannelsLadders, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGrid->Add(LABEL(S_LADDERS), 1, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGrid->Add(m_pTextChannelsNegCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pGrid->Add(LABEL(S_NEG_CTRL), 1, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  pSizerItems->Add(pGrid, ITEM_ARGS);
  pSizerItems->Add(m_pCheckChannelsOmitILS, ITEM_ARGS);

  // Scale X-Axis Primer Peaks
  pSizerTemp = new wxBoxSizer(wxHORIZONTAL);
  pSizerTemp->AddSpacer(ID_BORDER << 1);
  pSizerTemp->Add(m_pTextXscaleMin, 0, wxALIGN_CENTER_VERTICAL);
  m_pLabelXscaleUnitsTo = LABEL(S_TO);
  pSizerTemp->Add(m_pLabelXscaleUnitsTo, 0, wxALIGN_CENTER_VERTICAL);
  pSizerTemp->Add(m_pTextXscaleMax, 0, wxALIGN_CENTER_VERTICAL);
  pSizerTemp->AddSpacer(ID_BORDER);
  m_pLabelXscaleUnits = LABEL(S_RANGE_SECONDS);
  pSizerTemp->Add(m_pLabelXscaleUnits, 0, wxALIGN_CENTER_VERTICAL);

  pText = LABEL(S_SECTION_SCALE_X_AXIS);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pRadioXscaleExcludePrimer, ITEM_ARGS);
  pSizerItems->Add(m_pRadioXscaleIncludePrimer, ITEM_ARGS);
  pSizerItems->Add(m_pRadioXscaleIncludePrimerNegCtrl, ITEM_ARGS);
  pSizerItems->Add(m_pRadioXscaleSpecify, ITEM_ARGS);
  pSizerItems->Add(pSizerTemp, ITEM_ARGS);

  // Scale Y-Axis
  pSizerTemp = new wxBoxSizer(wxHORIZONTAL);
  pSizerTemp->AddSpacer(ID_BORDER << 1);
  pSizerTemp->Add(m_pTextYscaleMin, 0, wxALIGN_CENTER_VERTICAL);
  m_pLabelYscaleTo = LABEL(S_TO);
  pSizerTemp->Add(m_pLabelYscaleTo, 0, wxALIGN_CENTER_VERTICAL);
  pSizerTemp->Add(m_pTextYscaleMax, 0, wxALIGN_CENTER_VERTICAL);
  pSizerTemp->AddSpacer(ID_BORDER);
  m_pLabelYRFU = LABEL(S_RFU);
  pSizerTemp->Add(m_pLabelYRFU, 0, wxALIGN_CENTER_VERTICAL);

  pText = LABEL(S_SECTION_SCALE_Y_AXIS);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pRadioYscaleChannel, ITEM_ARGS);
  pSizerItems->Add(m_pRadioYscaleSample, ITEM_ARGS);
  pSizerItems->Add(m_pRadioYscaleSpecify, ITEM_ARGS);
  pSizerItems->Add(pSizerTemp, ITEM_ARGS);

  // Negative Control
  pText = LABEL(S_NEG_CTRL);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS);
  pSizerItems->Add(m_pRadioYscaleNegCtrlData, ITEM_ARGS);
  pSizerItems->Add(m_pRadioYscaleNegCtrlRFU, ITEM_ARGS);
  pSizerItems->Add(m_pRadioYscaleNegCtrlILS, ITEM_ARGS);

  wxBoxSizer *pSizerFull = new wxBoxSizer(wxVERTICAL); // two items main panel and button panel
  wxBoxSizer *pSizerPanel = new wxBoxSizer(wxHORIZONTAL);
  pSizerPanel->Add(pSizerLeft, 0, wxALIGN_TOP | wxLEFT | wxRIGHT | wxTOP, ID_BORDER << 1);
  pSizerPanel->Add(pSizerRight, 0, wxALIGN_TOP | wxLEFT | wxRIGHT | wxTOP, ID_BORDER << 1);

  pSizerFull->Add(pSizerPanel, 1, wxEXPAND, 0);
  pSizerFull->Add(CreateButtonSizer(wxOK | wxCANCEL | wxAPPLY), 0, wxALL | wxALIGN_RIGHT, ID_BORDER << 1);
  SetSizer(pSizerFull);
  Layout();
  Fit();
}
void CDialogPrintSettings::_SaveTextIntValue(wxTextCtrl *pText, int *pn, bool bOnlyIfEnabled)
{
  if ((!bOnlyIfEnabled) || pText->IsEnabled())
  {
    wxString s = pText->GetValue();
    if (!s.IsEmpty())
    {
      *pn = atoi(s.c_str());
    }
  }
}

void CDialogPrintSettings::_SetXScaleUserValues()
{
  bool bEnabled = m_pTextXscaleMin->IsEnabled();
  int n1 = __OUT_OF_RANGE, n2 = __OUT_OF_RANGE;
  if (!bEnabled)
  {
  }
  else if (m_pRadioXaxisILSBPS->GetValue())
  {
    n1 = m_nXscaleMinBPS;
    n2 = m_nXscaleMaxBPS;
  }
  else
  {
    n1 = m_nXscaleMin;
    n2 = m_nXscaleMax;
  }
  m_pTextXscaleMin->SetIntValue(n1);
  m_pTextXscaleMax->SetIntValue(n2);
}
void CDialogPrintSettings::_EnableXScaleUser(bool bEnable)
{
  if (m_pTextXscaleMin->IsEnabled() != bEnable)
  {
    m_pTextXscaleMin->Enable(bEnable);
    m_pTextXscaleMax->Enable(bEnable);
    m_pLabelXscaleUnits->Enable(bEnable);
    m_pLabelXscaleUnitsTo->Enable(bEnable);
    _SetXScaleUserValues();
  }
}
void CDialogPrintSettings::_SetupLadderLabels()
{
  m_pCheckCurveLadderLabels->Enable(m_pCheckCurveLadder->GetValue());
}
void CDialogPrintSettings::_SetupXScaleUserUnits()
{
  const char *ps;
  bool bTime = m_pRadioXaxisTime->GetValue();
  ps =  bTime ? S_RANGE_SECONDS : S_RANGE_BPS;
  if (m_pLabelXscaleUnits->GetLabel() != ps)
  {
    m_pLabelXscaleUnits->SetLabel(ps);
  }
  if (m_pRadioXscaleSpecify->GetValue())
  {
    _SetXScaleUserValues();
  }
}
void CDialogPrintSettings::_EnableYScaleUser(bool bEnable)
{
  if (m_pTextYscaleMin->IsEnabled() != bEnable)
  {
    wxString s1, s2;
    if (bEnable)
    {
      s1 = nwxString::FormatNumber(m_nYscaleMin);
      s2 = nwxString::FormatNumber(m_nYscaleMax);
    }
    else
    {
      _SaveTextIntValue(m_pTextYscaleMin, &m_nYscaleMin);
      _SaveTextIntValue(m_pTextYscaleMax, &m_nYscaleMax);
    }
    m_pTextYscaleMin->SetValue(s1);
    m_pTextYscaleMax->SetValue(s2);
    m_pTextYscaleMin->Enable(bEnable);
    m_pTextYscaleMax->Enable(bEnable);
    m_pLabelYscaleTo->Enable(bEnable);
    m_pLabelYRFU->Enable(bEnable);
  }
}
void CDialogPrintSettings::_SetupXScaleUser()
{
  _EnableXScaleUser(m_pRadioXscaleSpecify->GetValue());
}
void CDialogPrintSettings::_SetupYScaleUser()
{
  _EnableYScaleUser(m_pRadioYscaleSpecify->GetValue());
}

bool CDialogPrintSettings::TransferDataToWindow()
{
  m_pCheckCurveAnalyzed->SetValue(m_pParms->GetPrintCurveAnalyzed());
  m_pCheckCurveLadder->SetValue(m_pParms->GetPrintCurveLadder());;
  m_pCheckCurveLadderLabels->SetValue(m_pParms->GetPrintCurveLadderLabels());
  m_pCheckCurveLadderBins->SetValue(m_pParms->GetPrintCurveRaw());
  m_pCheckCurveBaseline->SetValue(m_pParms->GetPrintCurveBaseline());
  m_pCheckCurveRaw->SetValue(m_pParms->GetPrintCurveRaw());
  m_pCheckCurveILSvertical->SetValue(m_pParms->GetPrintCurveILSvertical());
  m_pCheckCurveMinRFU->SetValue(m_pParms->GetPrintCurveMinRFU());

  // samples
  m_pCheckSampleLadders->SetValue(m_pParms->GetPrintSamplesLadders());
  m_pCheckSamplePosCtrl->SetValue(m_pParms->GetPrintSamplesPosCtrl());
  m_pCheckSampleNegCtrl->SetValue(m_pParms->GetPrintSamplesNegCtrl());
  m_pCheckSampleDisabled->SetValue(m_pParms->GetPrintSamplesDisabled());
  //m_pButtonSampleOmit;

  // peak labels

  _SetLabelSelection(m_pParms->GetPrintLabelsPeak());
  m_pCheckLabelIncludeDisabled->SetValue(m_pParms->GetPrintCurveDisabledAlleles());

  // artifact labels
  _SetRadioValue(m_nGroupArtifact, m_pParms->GetPrintArtifact());

  // page heading
  _SetRadioValue(m_nGroupHeading, m_pParms->GetPrintHeading());
  m_pTextHeadingNotes->SetValue(m_pParms->GetPrintHeadingNotes());

  // channels per page
  m_pTextChannelsSamples->SetValue(m_pParms->GetPrintChannelsSamples());
  m_pTextChannelsLadders->SetValue(m_pParms->GetPrintChannelsLadders());
  m_pTextChannelsNegCtrl->SetValue(m_pParms->GetPrintChannelsNegCtrl());
  m_pCheckChannelsOmitILS->SetValue(m_pParms->GetPrintChannelsOmitILS());

  // X Axis
  bool bXBPS = m_pParms->GetPrintXaxisILSBPS();
  _SetRadioValue(m_nGroupXaxisUnits, bXBPS ? PRINT_X_BPS : PRINT_X_TIME);

  // X Axis Scale Primer

  int nTemp = m_pParms->GetPrintXscale();
  _SetRadioValue(m_nGroupXscale, nTemp);
  m_nXscaleMin = m_pParms->GetPrintXscaleMin();
  m_nXscaleMax = m_pParms->GetPrintXscaleMax();
  m_nXscaleMinBPS = m_pParms->GetPrintXscaleMinBPS();
  m_nXscaleMaxBPS = m_pParms->GetPrintXscaleMaxBPS();
  _EnableXScaleUser(nTemp == PRINT_X_SCALE_USER);

  // Y Axis Scale

  nTemp = m_pParms->GetPrintYscale();
  _SetRadioValue(m_nGroupYscale, nTemp);
  m_nYscaleMin = m_pParms->GetPrintYscaleMin();
  m_nYscaleMax = m_pParms->GetPrintYscaleMax();
  _EnableYScaleUser(nTemp == PRINT_Y_SCALE_USER);

  // negative control Y Axis scale
  _SetRadioValue(m_nGroupYscaleNegCtrl, m_pParms->GetPrintYcaleNegCtrl());

  // context sensitive settings
  _SetupLadderLabels();
  _SetupXScaleUserUnits();
  return true;
}

bool CDialogPrintSettings::TransferDataFromWindow()
{
  int nValue = 0;
  CPrintOutAnalysis::SetOmittedSamples(m_setSamplesOmitDialog);

  // curve check boxes
  m_pParms->SetPrintCurveAnalyzed(m_pCheckCurveAnalyzed->GetValue());
  m_pParms->SetPrintCurveRaw(m_pCheckCurveRaw->GetValue());
  m_pParms->SetPrintCurveLadder(m_pCheckCurveLadder->GetValue());
  m_pParms->SetPrintCurveLadderLabels(m_pCheckCurveLadderLabels->GetValue());
  m_pParms->SetPrintCurveLadderBins(m_pCheckCurveLadderBins->GetValue());
  m_pParms->SetPrintCurveBaseline(m_pCheckCurveBaseline->GetValue());
  m_pParms->SetPrintCurveILSvertical(m_pCheckCurveILSvertical->GetValue());
  m_pParms->SetPrintCurveMinRFU(m_pCheckCurveMinRFU->GetValue());

  //samples
  m_pParms->SetPrintSamplesLadders(m_pCheckSampleLadders->GetValue());
  m_pParms->SetPrintSamplesPosCtrl(m_pCheckSamplePosCtrl->GetValue());
  m_pParms->SetPrintSamplesNegCtrl(m_pCheckSampleNegCtrl->GetValue());
  m_pParms->SetPrintSamplesDisabled(m_pCheckSampleDisabled->GetValue());

  // peak labels
  std::vector<unsigned int> an;
  _GetLabelSelection(&an);
  m_pParms->SetPrintLabelsPeak(an);
  m_pParms->SetPrintCurveDisabledAlleles(m_pCheckLabelIncludeDisabled->GetValue());

  // artifact labels
  m_pParms->SetPrintArtifact(_GetRadioValue(m_nGroupArtifact));

  // page heading
  m_pParms->SetPrintHeading(_GetRadioValue(m_nGroupHeading));
  m_pParms->SetPrintHeadingNotes(m_pTextHeadingNotes->GetValue());

  // channels per page
  m_pParms->SetPrintChannelsSamples(m_pTextChannelsSamples->GetValue());
  m_pParms->SetPrintChannelsLadders(m_pTextChannelsLadders->GetValue());
  m_pParms->SetPrintChannelsNegCtrl(m_pTextChannelsNegCtrl->GetValue());
  m_pParms->SetPrintChannelsOmitILS(m_pCheckChannelsOmitILS->GetValue());

  // X Axis
  bool bXBPS = m_pRadioXaxisILSBPS->GetValue();
  m_pParms->SetPrintXaxisILSBPS(bXBPS);

  // X Axis Scale Primer
  nValue = _GetRadioValue(m_nGroupXscale);
  m_pParms->SetPrintXscale(nValue);
  if (nValue != PRINT_X_SCALE_USER)
  {
  }
  else if (bXBPS)
  {
    _SaveTextIntValue(m_pTextXscaleMin, &m_nXscaleMinBPS);
    _SaveTextIntValue(m_pTextXscaleMax, &m_nXscaleMaxBPS);
    CPanelPlot::CheckRange(&m_nXscaleMinBPS, &m_nXscaleMaxBPS);
    m_pParms->SetPrintXscaleMinBPS(m_nXscaleMinBPS);
    m_pParms->SetPrintXscaleMaxBPS(m_nXscaleMaxBPS);

  }
  else
  {
    _SaveTextIntValue(m_pTextXscaleMin, &m_nXscaleMin);
    _SaveTextIntValue(m_pTextXscaleMax, &m_nXscaleMax);
    CPanelPlot::CheckRange(&m_nXscaleMin, &m_nXscaleMax);
    m_pParms->SetPrintXscaleMin(m_nXscaleMin);
    m_pParms->SetPrintXscaleMax(m_nXscaleMax);
  }

  // Y-Axis scale
  nValue = _GetRadioValue(m_nGroupYscale);
  m_pParms->SetPrintYscale(nValue);
  if(nValue == PRINT_Y_SCALE_USER)
  {
    _SaveTextIntValue(m_pTextYscaleMin, &m_nYscaleMin);
    _SaveTextIntValue(m_pTextYscaleMax, &m_nYscaleMax);
    CPanelPlot::CheckRange(&m_nYscaleMin, &m_nYscaleMax);
    m_pParms->SetPrintYscaleMin(m_nYscaleMin);
    m_pParms->SetPrintYscaleMax(m_nYscaleMax);
  }

  // negative control Y Axis scale
  m_pParms->SetPrintYcaleNegCtrl(_GetRadioValue(m_nGroupYscaleNegCtrl));
  return true;
}

void CDialogPrintSettings::_OnButtonOmit(wxCommandEvent &)
{
  std::vector<const COARsample *> vpSamples;
  int nTypes = 0;
  struct
  {
    wxCheckBox *pCheckBox;
    int nType;
  }
  TYPES[] =
  {
    {m_pCheckSampleDisabled, CFrameAnalysis::INCLUDE_DISABLED},
    {m_pCheckSampleLadders, CFrameAnalysis::INCLUDE_LADDER},
    {m_pCheckSamplePosCtrl, CFrameAnalysis::INCLUDE_POS_CTRL},
    {m_pCheckSampleNegCtrl, CFrameAnalysis::INCLUDE_NEG_CTRL}
  };
  size_t TYPE_COUNT = sizeof(TYPES) / sizeof(TYPES[0]);
  for (size_t i = 0; i < TYPE_COUNT; ++i)
  {
    if (TYPES[i].pCheckBox->GetValue())
    {
      nTypes |= TYPES[i].nType;
    }
  }
  m_pFrameAnalysis->GetSamplesByRow(&vpSamples, nTypes);
  CDialogPrintSettingsOmit omit(this, m_pRadioHeadingFile->GetValue(), &vpSamples, &m_setSamplesOmitDialog);
  int n = omit.ShowModal();
  if (n == wxID_OK)
  {
    wxString s;
    int nCount = (int)m_setSamplesOmitDialog.size();
    if (nCount)
    {
      s = wxString::Format(wxT("(%d}"), nCount);
    }
    m_pTextOmitCount->SetLabel(s);
    CPrintOutAnalysis::InvalidateAll(); // redo page setup
  }
}
void CDialogPrintSettings::_OnCurveLadders(wxCommandEvent &)
{
  _SetupLadderLabels();
}
void CDialogPrintSettings::_OnYScale(wxCommandEvent &)
{
  _SetupYScaleUser();
}
void CDialogPrintSettings::_OnXScale(wxCommandEvent &)
{
  _SetupXScaleUser();
}
void CDialogPrintSettings::_OnXUnits(wxCommandEvent &)
{
  _SetupXScaleUserUnits();
}
void CDialogPrintSettings::_Apply()
{
  // copy omitted samples
  TransferDataFromWindow();
  m_pParent->UpdateSettings();
}
void CDialogPrintSettings::_OnOK(wxCommandEvent &)
{
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsOK"));
  _Apply();
  Close();
}
void CDialogPrintSettings::_OnApply(wxCommandEvent &)
{
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsAPPLY"));
  _Apply();
}

void CDialogPrintSettings::_BuildLabelSelection()
{
  if (m_mapDisplayLabels.empty())
  {
    struct
    {
      wxCheckBox *pCheck;
      int n;
    }
    LABEL_LIST[] =
    {
      {m_pCheckLabelAllele, IDmenuDisplayAlleles},
      {m_pCheckLabelBPS, IDmenuDisplayBPS},
      {m_pCheckLabelRFU, IDmenuDisplayRFU},
      {m_pCheckLabelTime, IDmenuDisplayTime},
      {m_pCheckLabelILSBPS, IDmenuDisplayILS_BPS},
      {m_pCheckLabelPeakArea, IDmenuDisplayPeakArea},
      {NULL, -1}
    }, *pLabel;

    for (pLabel = LABEL_LIST; pLabel->pCheck != NULL; ++pLabel)
    {
      m_mapDisplayLabels.insert(std::map<int, wxCheckBox *>::value_type(pLabel->n, pLabel->pCheck));
    }
  }
}
void CDialogPrintSettings::_SetLabelSelection(const std::vector<unsigned int> &an)
{
  std::set<unsigned int> sn;
  _BuildLabelSelection();
  for (std::vector<unsigned int>::const_iterator itrs = an.begin(); itrs != an.end(); ++itrs)
  {
    sn.insert(*itrs);
  }
  bool bCheck;
  for (std::map<unsigned int, wxCheckBox *>::iterator itr = m_mapDisplayLabels.begin(); itr != m_mapDisplayLabels.end(); ++itr)
  {
    bCheck = sn.find(itr->first) != sn.end();
    itr->second->SetValue(bCheck);
  }
}
void CDialogPrintSettings::_GetLabelSelection(std::vector<unsigned int> *pan)
{
  pan->clear();
  for (std::map<unsigned int, wxCheckBox *>::iterator itr = m_mapDisplayLabels.begin(); itr != m_mapDisplayLabels.end(); ++itr)
  {
    if (itr->second->GetValue())
    {
      pan->push_back(itr->first);
    }
  }
}



IMPLEMENT_PERSISTENT_POSITION(CDialogPrintSettings)

BEGIN_EVENT_TABLE(CDialogPrintSettings, wxDialog)
EVT_CHECKBOX(IDprintCurveLadders, CDialogPrintSettings::_OnCurveLadders)
EVT_BUTTON(IDprintSampleOmit, CDialogPrintSettings::_OnButtonOmit)
EVT_BUTTON(wxID_APPLY, CDialogPrintSettings::_OnApply)
EVT_BUTTON(wxID_OK, CDialogPrintSettings::_OnOK)

EVT_RADIOBUTTON(IDprintXaxisILSBPS, CDialogPrintSettings::_OnXUnits)
EVT_RADIOBUTTON(IDprintXscaleSpecify, CDialogPrintSettings::_OnXScale)
EVT_RADIOBUTTON(IDprintYscaleSpecify, CDialogPrintSettings::_OnYScale)
EVT_PERSISTENT_POSITION(CDialogPrintSettings)
END_EVENT_TABLE()


// CDialogPrintSettingsOmit - implementation

CDialogPrintSettingsOmit::~CDialogPrintSettingsOmit() {}

CDialogPrintSettingsOmit::CDialogPrintSettingsOmit(
  wxWindow *parent,
  bool bShowFileName,
  std::vector<const COARsample *> *pSamples,
  std::set<const COARsample *> *pSamplesOmit) :
  wxDialog(parent, wxID_ANY, wxT(S_WINDOW_OMIT_TITLE),
    GET_PERSISTENT_POSITION(CDialogPrintSettingsOmit),
    GET_PERSISTENT_SIZE(CDialogPrintSettingsOmit), mainApp::DIALOG_STYLE)
  , m_pSamples(pSamples)
  , m_pSamplesOmit(pSamplesOmit)
{
  wxArrayString asChoices;
  asChoices.Alloc(pSamples->size());
  std::vector<const COARsample *>::iterator itr;
  if (bShowFileName)
  {
    for (itr = pSamples->begin(); itr != pSamples->end(); ++itr)
    {
      asChoices.Add((*itr)->GetName());
    }
  }
  else
  {
    for (itr = pSamples->begin(); itr != pSamples->end(); ++itr)
    {
      asChoices.Add((*itr)->GetSampleName());
    }
  }
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerLinks = new wxBoxSizer(wxHORIZONTAL);
  wxHyperlinkCtrl *pListClear = new wxHyperlinkCtrl(this, IDbuttonClear, "Clear", wxEmptyString);
  wxHyperlinkCtrl *pListInvert = new wxHyperlinkCtrl(this, IDbuttonInvert, "Invert", wxEmptyString);
  wxStaticText *pLabel = LABEL("Select samples to omit:");
  wxFont f = pLabel->GetFont();
  f.MakeBold();
  pLabel->SetFont(f);
  pSizer->Add(pLabel, 0, wxLEFT | wxRIGHT | wxTOP | wxALIGN_LEFT, ID_BORDER);
  pSizerLinks->Add(pListClear, 0, 0);
  pSizerLinks->Add(pListInvert, 0, wxLEFT, ID_BORDER);
  pSizer->Add(pSizerLinks, 0, wxLEFT | wxRIGHT | wxTOP | wxALIGN_LEFT, ID_BORDER);
  m_pListBox = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200), asChoices);
  pSizer->Add(m_pListBox, 1, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, ID_BORDER);
  pSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, ID_BORDER);
  SetSizer(pSizer);
  Layout();
}


bool CDialogPrintSettingsOmit::TransferDataToWindow()
{
  if (!m_pSamplesOmit->empty())
  {
    unsigned int i = 0;
    std::vector<const COARsample *>::iterator itr;
    for (itr = m_pSamples->begin(); itr != m_pSamples->end(); ++itr)
    {
      if (m_pSamplesOmit->find(*itr) != m_pSamplesOmit->end())
      {
        m_pListBox->Check(i, true);
      }
      ++i;
    }
  }
  return true;
}
bool CDialogPrintSettingsOmit::TransferDataFromWindow()
{
  wxArrayInt anChecked;
  m_pSamplesOmit->clear();
  unsigned int nCount = m_pListBox->GetCheckedItems(anChecked);
  int nItem;
  for (unsigned int i = 0; i < nCount; ++i)
  {
    nItem = anChecked.Item(i);
    m_pSamplesOmit->insert(m_pSamples->at(nItem));
  }
  return true;
}

void CDialogPrintSettingsOmit::_OnClearLink(wxHyperlinkEvent &)
{
  unsigned int nCount = m_pListBox->GetCount();
  for (unsigned int i = 0; i < nCount; ++i)
  {
    m_pListBox->Check(i, false);
  }
}
void CDialogPrintSettingsOmit::_OnInvertLink(wxHyperlinkEvent &)
{
  unsigned int nCount = m_pListBox->GetCount();
  for (unsigned int i = 0; i < nCount; ++i)
  {
    m_pListBox->Check(i, !m_pListBox->IsChecked(i));
  }
}

IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogPrintSettingsOmit)

BEGIN_EVENT_TABLE(CDialogPrintSettingsOmit, wxDialog)
EVT_HYPERLINK(IDbuttonClear, CDialogPrintSettingsOmit::_OnClearLink)
EVT_HYPERLINK(IDbuttonInvert, CDialogPrintSettingsOmit::_OnInvertLink)
EVT_PERSISTENT_SIZE_POSITION(CDialogPrintSettingsOmit)
END_EVENT_TABLE()


// CDialogPrintSettingsSample - print settings for a single sample
//   when printing from plot window

int CDialogPrintSettingsSample::GetPlotsPerPage(int nSampleType)
{
  int nSamplesPerPage = CHANNEL_MAX;
  CParmOsirisGlobal pParms;
  switch (nSampleType)
  {
  case SAMPLE_LADDER:
    nSamplesPerPage = pParms->GetPrintChannelsLadders();
    break;
  case SAMPLE_NEGATIVE_CONTROL:
    nSamplesPerPage = pParms->GetPrintChannelsNegCtrl();
    break;
  case SAMPLE_REGULAR:
  default:
    nSamplesPerPage = pParms->GetPrintChannelsSamples();
  }
  return nSamplesPerPage;
}
int CDialogPrintSettingsSample::GetSampleType(const COARsample *pSample)
{
  int nRtn = SAMPLE_REGULAR;
  if (pSample->IsLadderType())
  {
    nRtn = SAMPLE_LADDER;
  }
  else if (pSample->IsNegControl())
  {
    nRtn = SAMPLE_NEGATIVE_CONTROL;
  }
  return nRtn;
}

CDialogPrintSettingsSample::CDialogPrintSettingsSample(
  CPrintPreviewFrame *parent, const COARsample *pSample) :
  wxDialog(parent, wxID_ANY, wxT(S_WINDOW_TITLE),
    GET_PERSISTENT_POSITION(CDialogPrintSettingsSample),
    wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP),
  m_pParent(parent)
{
  m_nSampleType = GetSampleType(pSample);
  switch (m_nSampleType)
  {
  case SAMPLE_LADDER:
    m_psPlotsPerPageLabel = wxT(S_LADDERS);
    break;
  case SAMPLE_NEGATIVE_CONTROL:
    m_psPlotsPerPageLabel = wxT(S_NEG_CTRL);
    break;
  case SAMPLE_REGULAR:
  default:
    m_psPlotsPerPageLabel = wxT(S_SAMPLES);
  }
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsSampleCreate"));
  _Build();
}
void CDialogPrintSettingsSample::_Build()
{
  wxString sLabelCount(wxT(S_PLOTS_PER_PAGE));
  wxSize szText = GetTextExtent(wxT(S_TEXT_WIDTH));
  wxBoxSizer *pSizerItems = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerPlotsPerPage = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *pText;
  szText.SetHeight(-1);

  m_pRadioHeadingFile = new wxRadioButton(this, wxID_ANY, wxT(S_FILE_NAME), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  m_pRadioHeadingSample = new wxRadioButton(this, wxID_ANY, wxT(S_SAMPLE_NAME));
  m_pTextHeadingNotes = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, szText);
  m_pTextPlotsPerPage = new  wxSpinCtrl(this, wxID_ANY, wxEmptyString,
    wxDefaultPosition, szText, wxSP_ARROW_KEYS, 1, CHANNEL_MAX, CHANNEL_MAX);

  // Page Heading
  pText = LABEL(S_SECTION_PAGE_HEADING);
  wxFont fontLabelBold(pText->GetFont());
  fontLabelBold.MakeBold();
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS_TOP);
  pSizerItems->Add(LABEL(S_PAGE_TITLE), ITEM_ARGS);
  pSizerItems->Add(m_pRadioHeadingFile, ITEM_ARGS);
  pSizerItems->Add(m_pRadioHeadingSample, ITEM_ARGS);
  pSizerItems->Add(LABEL(S_PAGE_NOTES), ITEM_ARGS);
  pSizerItems->Add(m_pTextHeadingNotes, ITEM_ARGS_EXPAND);
  pText = LABEL(S_PLOTS_PER_PAGE);
  pText->SetFont(fontLabelBold);
  pSizerItems->Add(pText, LABEL_ARGS_TOP);

  pSizerPlotsPerPage->Add(m_pTextPlotsPerPage, 0, wxALIGN_CENTER_VERTICAL, 0);
  pSizerPlotsPerPage->Add(
    new wxStaticText(this, wxID_ANY, m_psPlotsPerPageLabel),
    0, wxALIGN_CENTER_VERTICAL | wxLEFT, ID_BORDER);
  pSizerItems->Add(pSizerPlotsPerPage, ITEM_ARGS);

  wxBoxSizer *pSizerFull = new wxBoxSizer(wxVERTICAL);
  pSizerFull->Add(pSizerItems, 0, (wxALL ^ wxBOTTOM) | wxALIGN_CENTER, ID_BORDER << 1);
  pSizerFull->Add(CreateButtonSizer(wxOK | wxCANCEL | wxAPPLY), 0, wxALL | wxALIGN_RIGHT, ID_BORDER << 1);
  SetSizer(pSizerFull);
  Layout();
  Fit();
}

CDialogPrintSettingsSample::~CDialogPrintSettingsSample() {}
bool CDialogPrintSettingsSample::TransferDataToWindow()
{
  int nPlotsPerPage = GetPlotsPerPage(m_nSampleType);
  m_pTextPlotsPerPage->SetValue(nPlotsPerPage);
  if (m_pParms->GetPrintHeading() == PRINT_TITLE_FILE_NAME)
  {
    m_pRadioHeadingFile->SetValue(true);
  }
  else
  {
    m_pRadioHeadingSample->SetValue(true);
  }
  m_pTextHeadingNotes->SetValue(m_pParms->GetPrintHeadingNotes());
  return true;
}
bool CDialogPrintSettingsSample::TransferDataFromWindow()
{
  int n = m_pTextPlotsPerPage->GetValue();
  switch (m_nSampleType)
  {
  case SAMPLE_LADDER:
    m_pParms->SetPrintChannelsLadders(n);
    break;
  case SAMPLE_NEGATIVE_CONTROL:
    m_pParms->SetPrintChannelsNegCtrl(n);
    break;
  case SAMPLE_REGULAR:
  default:
    m_pParms->SetPrintChannelsSamples(n);
  }
  n = m_pRadioHeadingFile->GetValue() ? PRINT_TITLE_FILE_NAME : PRINT_TITLE_SAMPLE_NAME;
  m_pParms->SetPrintHeading(n);
  m_pParms->SetPrintHeadingNotes(m_pTextHeadingNotes->GetValue());
  return true;
}
void CDialogPrintSettingsSample::_Apply()
{
  TransferDataFromWindow();
  m_pParent->UpdateSettings();
}
void CDialogPrintSettingsSample::_OnOK(wxCommandEvent &)
{
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsSampleOK"));
  _Apply();
  Close();
}
void CDialogPrintSettingsSample::_OnApply(wxCommandEvent &)
{
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsSampleAPPLY"));
  _Apply();
}


IMPLEMENT_PERSISTENT_POSITION(CDialogPrintSettingsSample)

BEGIN_EVENT_TABLE(CDialogPrintSettingsSample, wxDialog)
EVT_BUTTON(wxID_APPLY, CDialogPrintSettingsSample::_OnApply)
EVT_BUTTON(wxID_OK, CDialogPrintSettingsSample::_OnOK)
EVT_PERSISTENT_POSITION(CDialogPrintSettingsSample)
END_EVENT_TABLE()

