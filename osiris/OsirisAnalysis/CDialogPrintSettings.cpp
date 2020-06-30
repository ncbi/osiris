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

#define LABEL(s) new wxStaticText(this, wxID_ANY, wxT(s))

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
  bool m_bShowFileName;
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
    wxDEFAULT_DIALOG_STYLE),
  m_pFrameAnalysis(pFrame),
  m_pParent(parent),
  m_pFile(pFrame->GetOARfile())
{
  m_nRadioGroup = -1;
  mainApp::Ping(PING_EVENT, wxT("PrintSettingsCreate"));
  _Build();
}
CDialogPrintSettings::~CDialogPrintSettings()
{
}

const int CDialogPrintSettings::NO_RADIO_INT = -32768;

wxTextCtrl *CDialogPrintSettings::_CreateNumericTextCtrl(
  wxWindow *parent, const wxSize &sz, int nMin, int nMax, int nInit, int nID)
{
  wxIntegerValidator<int> val(NULL, wxNUM_VAL_ZERO_AS_BLANK);
  val.SetRange(nMin, nMax);
  wxTextCtrl *pRtn = new wxTextCtrl(parent, nID, nwxString::FormatNumber(nInit), wxDefaultPosition, sz, 0, val);
  pRtn->SetMaxLength(6);
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
  wxSize szText = GetTextExtent("99999999");
  szText.SetHeight(-1);

  m_pCheckCurveAnalyzed = new wxCheckBox(this, wxID_ANY, wxT(S_ANALYZED));
  m_pCheckCurveRaw = new wxCheckBox(this, wxID_ANY, wxT(S_RAW));
  m_pCheckCurveLadder = new wxCheckBox(this, IDprintCurveLadders, wxT(S_LADDERS));
  m_pCheckCurveLadderLabels = new wxCheckBox(this, wxID_ANY, wxT(S_LADDER_LABELS));
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
  m_pTextXscaleMin = _CreateNumericTextCtrl(this, szText, -999, 99999, 0);
  m_pTextXscaleMax = _CreateNumericTextCtrl(this, szText, -999, 99999, 12000);

  // scale Y-Axis

  m_pRadioYscaleChannel = _CreateRadioButton(wxT(S_SCALE_CHANNEL), PRINT_Y_SCALE_CHANNEL, true, IDprintYscaleSpecify);
  m_pRadioYscaleSample = _CreateRadioButton(wxT(S_SCALE_SAMPLE), PRINT_Y_SCALE_SAMPLE, false, IDprintYscaleSpecify);
  m_pRadioYscaleSpecify = _CreateRadioButton(wxT(S_SPECIFY), PRINT_Y_SCALE_USER, false, IDprintYscaleSpecify);
  m_nGroupYscale = _GetCurrentGroup();

  m_pTextYscaleMin = _CreateNumericTextCtrl(this, szText, -50000, 50000, -100);
  m_pTextYscaleMax = _CreateNumericTextCtrl(this, szText, -50000, 50000, 32000);

  m_pRadioYscaleNegCtrlData = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_PEAK), PRINT_Y_SCALE_NEG_PEAKS, true);
  m_pRadioYscaleNegCtrlRFU = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_RFU), PRINT_Y_SCALE_NEG_INCLUDE_RFU);
  m_pRadioYscaleNegCtrlILS = _CreateRadioButton(wxT(S_SCALE_NEG_CTRL_ILS), PRINT_Y_SCALE_NEG_ILS);
  m_nGroupYscaleNegCtrl = _GetCurrentGroup();

  // build the panel

#define LABEL_ARGS_TOP 0, wxALIGN_LEFT | wxBOTTOM, ID_BORDER
#define LABEL_ARGS 0, wxALIGN_LEFT | wxBOTTOM | wxTOP, ID_BORDER
#define ITEM_ARGS 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, ID_BORDER
#define ITEM_ARGS_EXPAND 0, wxLEFT | wxBOTTOM | wxEXPAND, ID_BORDER

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
void CDialogPrintSettings::_EnableXScaleUser(bool bEnable)
{
  if (m_pTextXscaleMin->IsEnabled() != bEnable)
  {
    wxString s1, s2;
    if (bEnable)
    {
      s1 = nwxString::FormatNumber(m_nXscaleMin);
      s2 = nwxString::FormatNumber(m_nXscaleMax);
    }
    else
    {
      _SaveTextIntValue(m_pTextXscaleMin, &m_nXscaleMin);
      _SaveTextIntValue(m_pTextXscaleMax, &m_nXscaleMax);
    }
    m_pTextXscaleMin->SetValue(s1);
    m_pTextXscaleMax->SetValue(s2);

    m_pTextXscaleMin->Enable(bEnable);
    m_pTextXscaleMax->Enable(bEnable);
    m_pLabelXscaleUnits->Enable(bEnable);
    m_pLabelXscaleUnitsTo->Enable(bEnable);
  }
}
void CDialogPrintSettings::_SetupLadderLabels()
{
  m_pCheckCurveLadderLabels->Enable(m_pCheckCurveLadder->GetValue());
}

void CDialogPrintSettings::_SetupXScaleUserUnits()
{
  const char *ps;
  ps = m_pRadioXaxisTime->GetValue() ? S_RANGE_SECONDS : S_RANGE_BPS;
  m_pLabelXscaleUnits->SetLabel(ps);
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
  CParmOsirisGlobal pParm;
  m_pCheckCurveAnalyzed->SetValue(pParm->GetPrintCurveAnalyzed());
  m_pCheckCurveLadder->SetValue(pParm->GetPrintCurveLadder());;
  m_pCheckCurveLadderLabels->SetValue(pParm->GetPrintCurveLadderLabels());
  m_pCheckCurveBaseline->SetValue(pParm->GetPrintCurveBaseline());
  m_pCheckCurveRaw->SetValue(pParm->GetPrintCurveRaw());
  m_pCheckCurveILSvertical->SetValue(pParm->GetPrintCurveILSvertical());
  m_pCheckCurveMinRFU->SetValue(pParm->GetPrintCurveMinRFU());

  // samples
  m_pCheckSampleLadders->SetValue(pParm->GetPrintSamplesLadders());
  m_pCheckSamplePosCtrl->SetValue(pParm->GetPrintSamplesPosCtrl());
  m_pCheckSampleNegCtrl->SetValue(pParm->GetPrintSamplesNegCtrl());
  m_pCheckSampleDisabled->SetValue(pParm->GetPrintSamplesDisabled());
  //m_pButtonSampleOmit;

  // peak labels

  _SetLabelSelection(pParm->GetPrintLabelsPeak());

  // artifact labels
  _SetRadioValue(m_nGroupArtifact, pParm->GetPrintArtifact());

  // page heading
  _SetRadioValue(m_nGroupHeading, pParm->GetPrintHeading());
  m_pTextHeadingNotes->SetValue(pParm->GetPrintHeadingNotes());

  // channels per page
  m_pTextChannelsSamples->SetValue(pParm->GetPrintChannelsSamples());
  m_pTextChannelsLadders->SetValue(pParm->GetPrintChannelsLadders());
  m_pTextChannelsNegCtrl->SetValue(pParm->GetPrintChannelsNegCtrl());
  m_pCheckChannelsOmitILS->SetValue(pParm->GetPrintChannelsOmitILS());

  // X Axis
  _SetRadioValue(m_nGroupXaxisUnits, pParm->GetPrintXaxisILSBPS() ? PRINT_X_BPS : PRINT_X_TIME);

  // X Axis Scale Primer

  int nTemp = pParm->GetPrintXscale();
  _SetRadioValue(m_nGroupXscale, nTemp);
  m_nXscaleMin = pParm->GetPrintXscaleMin();
  m_nXscaleMax = pParm->GetPrintXscaleMax();
  _EnableXScaleUser(nTemp == PRINT_X_SCALE_USER);

  // Y Axis Scale

  nTemp = pParm->GetPrintYscale();
  _SetRadioValue(m_nGroupYscale, nTemp);
  m_nYscaleMin = pParm->GetPrintYscaleMin();
  m_nYscaleMax = pParm->GetPrintYscaleMax();
  _EnableYScaleUser(nTemp == PRINT_Y_SCALE_USER);

  // negative control Y Axis scale
  _SetRadioValue(m_nGroupYscaleNegCtrl, pParm->GetPrintYcaleNegCtrl());

  // context sensitive settings
  _SetupLadderLabels();
  _SetupXScaleUserUnits();
  return true;
}

bool CDialogPrintSettings::TransferDataFromWindow()
{
  CParmOsirisGlobal pParm;
  int nValue = 0;
  CPrintOutAnalysis::SetOmittedSamples(m_setSamplesOmitDialog);

  // curve check boxes
  pParm->SetPrintCurveAnalyzed(m_pCheckCurveAnalyzed->GetValue());
  pParm->SetPrintCurveRaw(m_pCheckCurveRaw->GetValue());
  pParm->SetPrintCurveLadder(m_pCheckCurveLadder->GetValue());
  pParm->SetPrintCurveLadderLabels(m_pCheckCurveLadderLabels->GetValue());
  pParm->SetPrintCurveBaseline(m_pCheckCurveBaseline->GetValue());
  pParm->SetPrintCurveILSvertical(m_pCheckCurveILSvertical->GetValue());
  pParm->SetPrintCurveMinRFU(m_pCheckCurveMinRFU->GetValue());

  //samples
  pParm->SetPrintSamplesLadders(m_pCheckSampleLadders->GetValue());
  pParm->SetPrintSamplesPosCtrl(m_pCheckSamplePosCtrl->GetValue());
  pParm->SetPrintSamplesNegCtrl(m_pCheckSampleNegCtrl->GetValue());
  pParm->SetPrintSamplesDisabled(m_pCheckSampleDisabled->GetValue());

  // peak labels
  std::vector<unsigned int> an;
  _GetLabelSelection(&an);
  pParm->SetPrintLabelsPeak(an);

  // artifact labels
  pParm->SetPrintArtifact(_GetRadioValue(m_nGroupArtifact));

  // page heading
  pParm->SetPrintHeading(_GetRadioValue(m_nGroupHeading));
  pParm->SetPrintHeadingNotes(m_pTextHeadingNotes->GetValue());

  // channels per page
  pParm->SetPrintChannelsSamples(m_pTextChannelsSamples->GetValue());
  pParm->SetPrintChannelsLadders(m_pTextChannelsLadders->GetValue());
  pParm->SetPrintChannelsNegCtrl(m_pTextChannelsNegCtrl->GetValue());
  pParm->SetPrintChannelsOmitILS(m_pCheckChannelsOmitILS->GetValue());

  // X Axis
  pParm->SetPrintXaxisILSBPS(m_pRadioXaxisILSBPS->GetValue());

  // X Axis Scale Primer
  nValue = _GetRadioValue(m_nGroupXscale);
  pParm->SetPrintXscale(nValue);
  if (nValue == PRINT_X_SCALE_USER)
  {
    _SaveTextIntValue(m_pTextXscaleMin, &m_nXscaleMin);
    _SaveTextIntValue(m_pTextXscaleMax, &m_nXscaleMax);
    CPanelPlot::CheckRange(&m_nXscaleMin, &m_nXscaleMax);
    pParm->SetPrintXscaleMin(m_nXscaleMin);
    pParm->SetPrintXscaleMax(m_nXscaleMax);
  }

  // Y-Axis scale
  nValue = _GetRadioValue(m_nGroupYscale);
  pParm->SetPrintYscale(nValue);
  if(nValue == PRINT_Y_SCALE_USER)
  {
    _SaveTextIntValue(m_pTextYscaleMin, &m_nYscaleMin);
    _SaveTextIntValue(m_pTextYscaleMax, &m_nYscaleMax);
    CPanelPlot::CheckRange(&m_nYscaleMin, &m_nYscaleMax);
    pParm->SetPrintYscaleMin(m_nYscaleMin);
    pParm->SetPrintYscaleMax(m_nYscaleMax);
  }

  // negative control Y Axis scale
  pParm->SetPrintYcaleNegCtrl(_GetRadioValue(m_nGroupYscaleNegCtrl));
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
  , m_bShowFileName(bShowFileName)
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



