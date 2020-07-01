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
*  FileName: CDialogPrintColor.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include "CDialogPrintColor.h"
#include "nwx/nwxTextCtrl.h"
#include "CParmOsiris.h"
#include "mainApp.h"
#include "CKitColors.h"
#include "CPrintOutTestPattern.h"
#include "nwx/nwxPlotCtrl.h" // grid color

CDialogPrintColor::~CDialogPrintColor() {}

CDialogPrintColor::CDialogPrintColor(wxWindow *parent) :
  wxDialog(parent, wxID_ANY, wxT("Adjust Print Colors"),
    GET_PERSISTENT_POSITION(CDialogPrintColor),
    wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE)
{
#define TEXT0 "Instructions:"
#define TEXT1 "To adjust colors for your printer, print the " 
#define TEXT1_LINK "test pattern"
#define TEXT2 "and for each color, select the desired brightness, (0-100)"

  CParmOsirisGlobal pParm;
  CKitColors *pKitColors = mainApp::GetKitColors();
  wxSizerFlags flagText(0), flagColor(0);
  flagText.Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
  flagColor.Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL).Expand();
  // vertical sizer for 3 rows of text
  // horizontal sizer for text with adjacent hyperlink
  wxBoxSizer *pSizerText = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerLink = new wxBoxSizer(wxHORIZONTAL);
  wxStaticText *pText;
  pText = new wxStaticText(this, wxID_ANY, TEXT0);
  wxFont fontBold = pText->GetFont();
  fontBold.MakeBold();
  pText->SetFont(fontBold);
  pSizerText->Add(pText, flagText);
  pText = new wxStaticText(this, wxID_ANY, TEXT1);
  pSizerLink->Add(pText, flagText);
  wxHyperlinkCtrl *pLink = new wxHyperlinkCtrl(this, wxID_ANY, TEXT1_LINK, wxEmptyString);
  pSizerLink->Add(pLink, flagText);
  pSizerLink->AddStretchSpacer(1);
  pSizerText->Add(pSizerLink, flagText);
  pText = new wxStaticText(this, wxID_ANY, TEXT2);
  pSizerText->Add(pText, flagText);

  // BEGIN build controls
  wxFlexGridSizer *pSizerControls = new wxFlexGridSizer(2,wxSize(2,2));
  struct
  {
    const char *pLabel;
    nwxTextCtrlInteger **ppText;
    int nValue;
  }
  COLOR_DATA[] =
  {
    {"Blue", &pTextBlue, pParm->GetPrintColorBlue()},
    {"Green", &pTextGreen, pParm->GetPrintColorGreen()},
    {"Yellow", &pTextYellow, pParm->GetPrintColorYellow()},
    {"Red", &pTextRed, pParm->GetPrintColorRed()},
    {"Orange", &pTextOrange, pParm->GetPrintColorOrange()},
    {"Purple", &pTextPurple, pParm->GetPrintColorPurple()},
    {NULL, NULL, 0},
  },
  ColorRowGray = { "Gray", &pTextGray, pParm->GetPrintColorGray() },
  *pColorRow;
  
  for (pColorRow = &COLOR_DATA[0];
    pColorRow->pLabel != NULL;
    ++pColorRow)
  {
    wxString sColor(pColorRow->pLabel);
    _ColorRow(
      pSizerControls,
      sColor,
      pColorRow->nValue,
      pKitColors->GetColorByName(sColor),
      pColorRow->ppText);
  }
  _ColorRow(pSizerControls, ColorRowGray.pLabel, ColorRowGray.nValue, nwxPlotCtrl::GridColor(), ColorRowGray.ppText);
  // END build controls

  // build button bar
  wxSizer *pSizerButtons = CreateButtonSizer(wxOK | wxCANCEL);
  wxButton *pButtonPrint = new wxButton(this, IDprintPrint, wxT("Print Test Page..."));
  pSizerButtons->InsertStretchSpacer(0, 1);
  pSizerButtons->Insert(0, pButtonPrint, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);

  // build window sizer
  wxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(pSizerText, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_LEFT, ID_BORDER);
  pSizerAll->Add(pSizerControls, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, ID_BORDER);
  pSizerAll->Add(pSizerButtons, 0, wxALL | wxEXPAND, ID_BORDER);
  SetSizer(pSizerAll);
  Layout();
  Fit();
}
bool CDialogPrintColor::TransferDataToWindow()
{
  return true;
}
bool CDialogPrintColor::TransferDataFromWindow()
{
  CParmOsirisGlobal pParm;
  pParm->SetPrintColorRed(pTextRed->GetIntValue());
  pParm->SetPrintColorGreen(pTextGreen->GetIntValue());
  pParm->SetPrintColorBlue(pTextBlue->GetIntValue());
  pParm->SetPrintColorOrange(pTextOrange->GetIntValue());
  pParm->SetPrintColorYellow(pTextYellow->GetIntValue());
  pParm->SetPrintColorPurple(pTextPurple->GetIntValue());
  pParm->SetPrintColorGray(pTextGray->GetIntValue());
  return true;
}



void CDialogPrintColor::_ColorRow(
  wxSizer *pSizer,
  const wxString &sLabel,
  int nValue,
  const wxColour &color,
  nwxTextCtrlInteger **ppCtrl)
{
  wxPanel *pPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
  wxStaticText *pText = new wxStaticText(pPanel, wxID_ANY, sLabel);
  wxBoxSizer *pPanelSizer = new wxBoxSizer(wxVERTICAL);

  pPanelSizer->AddStretchSpacer(1);
  pPanelSizer->Add(pText, 0, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, ID_BORDER);
  pPanelSizer->AddStretchSpacer(1);
  pPanel->SetSizer(pPanelSizer);
  pText->SetFont(pText->GetFont().MakeBold());
  pText->SetBackgroundColour(color);
  pPanel->SetBackgroundColour(color);
  if ((int(color.Red()) + int(color.Blue()) + int(color.Green())) < 384)
  {
    // average of RGB < 128
    pText->SetForegroundColour(*wxWHITE);
  }
  *ppCtrl = new nwxTextCtrlInteger(this, wxID_ANY, 0, 100, nValue, 3);
  pSizer->Add(pPanel, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
  pSizer->Add(*ppCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
}
void CDialogPrintColor::_PrintTestPage()
{
  CPrintOutTestPattern::Print();
}
void CDialogPrintColor::_OnPrintLink(wxHyperlinkEvent &)
{
  _PrintTestPage();
}
void CDialogPrintColor::_OnPrintButton(wxCommandEvent &)
{
  _PrintTestPage();
}


IMPLEMENT_PERSISTENT_POSITION(CDialogPrintColor)
BEGIN_EVENT_TABLE(CDialogPrintColor, wxDialog)
EVT_HYPERLINK(wxID_ANY, CDialogPrintColor::_OnPrintLink)
EVT_BUTTON(IDprintPrint, CDialogPrintColor::_OnPrintButton)
EVT_PERSISTENT_POSITION(CDialogPrintColor)
END_EVENT_TABLE()