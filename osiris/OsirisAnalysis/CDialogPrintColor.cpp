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
    wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP)
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
  wxPanel *pPanel = new wxPanel(this);
  pText = new wxStaticText(pPanel, wxID_ANY, TEXT0);
  wxFont fontBold = pText->GetFont();
  fontBold.MakeBold();
  pText->SetFont(fontBold);
  pSizerText->Add(pText, flagText);
  pText = new wxStaticText(pPanel, wxID_ANY, TEXT1);
  pSizerLink->Add(pText, flagText);
  wxHyperlinkCtrl *pLink = new wxHyperlinkCtrl(pPanel, wxID_ANY, TEXT1_LINK, wxEmptyString);
  pSizerLink->Add(pLink, flagText);
  pSizerLink->AddStretchSpacer(1);
  pSizerText->Add(pSizerLink, flagText);
  pText = new wxStaticText(pPanel, wxID_ANY, TEXT2);
  pSizerText->Add(pText, flagText);

  // BEGIN build controls
  wxFlexGridSizer *pSizerControls = new wxFlexGridSizer(2,wxSize(2,4));
  struct
  {
    const char *pLabel;
    nwxTextCtrlInteger **ppText;
    int nValue;
  }
  COLOR_DATA[] =
  {
    {"Blue", &m_pTextBlue, pParm->GetPrintColorBlue()},
    {"Green", &m_pTextGreen, pParm->GetPrintColorGreen()},
    {"Yellow", &m_pTextYellow, pParm->GetPrintColorYellow()},
    {"Red", &m_pTextRed, pParm->GetPrintColorRed()},
    {"Orange", &m_pTextOrange, pParm->GetPrintColorOrange()},
    {"Purple", &m_pTextPurple, pParm->GetPrintColorPurple()},
    {NULL, NULL, 0},
  },
  ColorRowGray = { "Gray", &m_pTextGray, pParm->GetPrintColorGray() },
  *pColorRow;

  wxWindow *pPrev = pLink;
  for (pColorRow = &COLOR_DATA[0];
    pColorRow->pLabel != NULL;
    ++pColorRow)
  {
    wxString sColor(pColorRow->pLabel);
    _ColorRow(
      pPanel,
      pSizerControls,
      sColor,
      pColorRow->nValue,
      pKitColors->GetColorByName(sColor),
      pColorRow->ppText);
    (*pColorRow->ppText)->MoveAfterInTabOrder(pPrev);
    pPrev = *pColorRow->ppText;
  }
  _ColorRow(pPanel, pSizerControls, 
    ColorRowGray.pLabel, ColorRowGray.nValue, 
    nwxPlotCtrl::GridColor(), ColorRowGray.ppText);
  (*ColorRowGray.ppText)->MoveAfterInTabOrder(pPrev);
  // END build controls

  // build button bar
  wxButton *pButtonPrint = new wxButton(this, IDprintPrint, wxT("Print Test Page..."));
  wxSizer *pSizerButtons = CreateButtonSizer(wxOK | wxCANCEL);
  pSizerButtons->InsertStretchSpacer(0, 1);
  pSizerButtons->Insert(0, pButtonPrint, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);

  // build window sizer
  wxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  wxSizer *pSizerPanel = new wxBoxSizer(wxVERTICAL);
  pSizerPanel->Add(pSizerText, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_LEFT, ID_BORDER);
  pSizerPanel->Add(pSizerControls, 0, wxTOP | wxLEFT | wxRIGHT | wxALIGN_CENTER, ID_BORDER);
  pPanel->SetSizer(pSizerPanel);
  pSizerAll->Add(pPanel, 0, wxEXPAND, 0);
  pSizerAll->Add(pSizerButtons, 0, wxALL | wxEXPAND, ID_BORDER);
  SetSizer(pSizerAll);
  Layout();
  Fit();
}
bool CDialogPrintColor::TransferDataToWindow()
{
  m_pTextBlue->SetFocus();
  m_pTextBlue->SelectAll();
  return true;
}
bool CDialogPrintColor::TransferDataFromWindow()
{
  CParmOsirisGlobal pParm;
  pParm->SetPrintColorRed(m_pTextRed->GetIntValue());
  pParm->SetPrintColorGreen(m_pTextGreen->GetIntValue());
  pParm->SetPrintColorBlue(m_pTextBlue->GetIntValue());
  pParm->SetPrintColorOrange(m_pTextOrange->GetIntValue());
  pParm->SetPrintColorYellow(m_pTextYellow->GetIntValue());
  pParm->SetPrintColorPurple(m_pTextPurple->GetIntValue());
  pParm->SetPrintColorGray(m_pTextGray->GetIntValue());
  return true;
}



void CDialogPrintColor::_ColorRow(
  wxPanel *pTopPanel,
  wxSizer *pSizer,
  const wxString &sLabel,
  int nValue,
  const wxColour &color,
  nwxTextCtrlInteger **ppCtrl)
{
  wxPanel *pPanel = new wxPanel(pTopPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);
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
  *ppCtrl = new nwxTextCtrlInteger(pTopPanel, wxID_ANY, 0, 100, nValue, 3);
  pSizer->Add(pPanel, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxEXPAND, 0);
  pSizer->Add(*ppCtrl, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
}
void CDialogPrintColor::_PrintTestPage()
{
  CPrintOutTestPattern::Print(this);
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
