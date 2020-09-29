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
*  FileName: CDialogExportPlot.cpp
*  Author:   Douglas Hoffman
*
*/
#include <math.h>
#include <stdlib.h>
#include <wx/utils.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/radiobox.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/dcclient.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include "mainApp.h"
#include "CDialogExportPlot.h"
#include "COARfile.h"
#include "CParmOsiris.h"
#include "wxIDS.h"
#include "nwx/nwxStaticBitmap.h"
#include "nwx/nwxString.h"
#include "nwx/nwxRound.h"
#include "nwx/nwxFileUtil.h"
#include "nwx/nwxDialogBitmap.h"
#include "nwx/PersistentSize.h"
#include "OsirisFileTypes.h"
#include "CFramePlot.h"


CDialogExportPlot::~CDialogExportPlot()
{
  if(m_pDialogFile != NULL)
  {
    delete m_pDialogFile;
  }
  _CleanupBitmap();
}
CDialogExportPlot::CDialogExportPlot(
  CFramePlot *parent,
  wxWindowID id,
  const wxString &sPlotFileName) :
    wxDialog(
        parent,
        id,
        "Export PNG Graphic File",
        wxDefaultPosition,
        wxDefaultSize,
        mainApp::DIALOG_STYLE),
    m_sPlotFile(sPlotFileName),
    m_pTextTitle(NULL),
    m_pTextWidth(NULL),
    m_pTextHeight(NULL),
    m_pDialogFile(NULL),
    m_pBitmap(NULL),
    m_pParent(parent)
{
  CParmOsirisGlobal parm;
  double dWidth = parm->GetPNGwidth();
  double dHeight = parm->GetPNGheight();
  m_nUsage = parm->GetPNGusage();
  m_nUnits = parm->GetPNGunits();
  if(dWidth < 1.0 || dHeight < 1.0)
  {

    m_nUnits = UNIT_PIXELS;
    m_nUsage = USAGE_SCREEN;
    dWidth = 1024.0;
    dHeight = 768.0;
  }
  else
  {
    _ValidateUU();
  }
  _SetType();
  _SetRange();
  m_valid.SetAllowEmpty(false);

  m_pTextTitle = new wxTextCtrl(this,IDExportGraphic);
  m_pTextTitle->SetMaxLength(255);
  wxArrayString as;
  as.Alloc(3);
  as.Add("&Screen");
  as.Add("&Print");
  m_pUsage = new wxRadioBox(
    this,
    IDpngUsage,
    "Usage",
    wxDefaultPosition,
    wxDefaultSize,
    as,
    1,
    wxRA_SPECIFY_ROWS);

  m_pUsage->SetSelection(m_nUsage);
  as.Empty();
  as.Add("Pixels");
  as.Add("Inches");
  as.Add("Centimeters");
  wxSize szText(50,-1);
  wxStaticText *pText1 = new wxStaticText(this,wxID_ANY,
    "Width x Height  ");

  wxStaticText *pLabelTitle = new wxStaticText(
    this,wxID_ANY,"Title");
  m_pTextWidth = new wxTextCtrl(
    this,IDExportGraphic,"",
    wxDefaultPosition,
    szText,0,m_valid);

  wxStaticText *pText2 = new wxStaticText(this,wxID_ANY,
    " X ");

  m_pTextHeight = new wxTextCtrl(
    this,IDExportGraphic,"",
    wxDefaultPosition,
    szText,0,m_valid);
  m_pUnits = new wxComboBox(
    this,
    IDpngUnits,
    as.Item((size_t)m_nUnits),
    wxDefaultPosition,
    wxDefaultSize,
    as,
    wxCB_READONLY | wxCB_DROPDOWN);
  m_pTextFileName = new wxTextCtrl(this,wxID_ANY);
  wxButton *pButtonBrowse = new wxButton(
    this,IDbrowseOutput, "&Browse...",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  m_pCheckViewLocation = new wxCheckBox(this,wxID_ANY,
    CParmOsiris::LABEL_VIEW_LOCATION);
  m_pCheckViewLocation->SetValue(parm->GetPNGviewLocation());

  wxStaticText *pText3 = new wxStaticText(this,wxID_ANY,
    "File Name");
  wxBoxSizer *pSizerHW = new wxBoxSizer(wxHORIZONTAL);
  pSizerHW->Add(pText1,0,wxALIGN_CENTRE_VERTICAL);
  pSizerHW->Add(m_pTextWidth,0,wxALIGN_CENTRE_VERTICAL);
  pSizerHW->Add(pText2,0,wxALIGN_CENTRE_VERTICAL);
  pSizerHW->Add(m_pTextHeight,0,wxALIGN_CENTRE_VERTICAL);
  pSizerHW->Add(m_pUnits,0,wxLEFT | wxALIGN_CENTRE_VERTICAL, ID_BORDER);

  wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);
  pSizerTop->Add(m_pUsage,0,wxALIGN_BOTTOM | wxRIGHT,ID_BORDER);
  pSizerTop->Add(pSizerHW,0,wxALIGN_BOTTOM | wxBOTTOM, ID_BORDER);

  wxBoxSizer *pSizerFileName = new wxBoxSizer(wxHORIZONTAL);
  pSizerFileName->Add(
    m_pTextFileName,1,wxEXPAND | wxALIGN_CENTRE_VERTICAL);
  pSizerFileName->Add(
    pButtonBrowse,0,wxLEFT | wxALIGN_CENTRE_VERTICAL, ID_BORDER);

  wxSizer *pButtonSizer = CreateButtonSizer(wxOK | wxCANCEL);
  wxSizerItem *pItem = pButtonSizer->GetItem(1);
  pButtonSizer->Add(
    new wxButton(this,IDpreview,"Previe&w"),0,
    pItem->GetFlag(),pItem->GetBorder()
    );

  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
//  pSizer->Add(m_pUsage,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(pLabelTitle,0,wxALL | wxALIGN_LEFT,ID_BORDER);
  pSizer->Add(m_pTextTitle,0,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  pSizer->Add(pSizerTop,0,(wxALL ^ wxTOP) | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(pText3,0,wxLEFT | wxALIGN_LEFT, ID_BORDER);
  pSizer->Add(pSizerFileName,0,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  pSizer->Add(m_pCheckViewLocation,0,(wxALL ^ wxTOP),ID_BORDER);

  pSizer->Add(pButtonSizer,0,(wxALL ^ wxTOP) | wxALIGN_CENTRE,ID_BORDER);
  _TransferWidthHeightToWindow(dWidth,dHeight);

  SetSizer(pSizer);
  pSizer->Fit(this);
  pSizer->Layout();

//  SetupTextCtrlWidth(m_pTextFileName);
  Layout();


}
bool CDialogExportPlot::Show(bool show)
{
  _CleanupBitmap();
  return wxDialog::Show(show);
}

bool CDialogExportPlot::TransferDataToWindow()
{
  wxString sFile = m_pTextFileName->GetValue();
  if(sFile.IsEmpty() || wxFileName::FileExists(sFile))
  {
    wxFileName fn(m_sPlotFile);
    wxString sTitle = fn.GetName();
    m_pTextTitle->ChangeValue(sTitle);
    m_sDefaultFile = nwxFileUtil::SetupFileName(m_sPlotFile,EXT_PNG);
    m_pTextFileName->ChangeValue(m_sDefaultFile);
    m_pTextFileName->SetInsertionPointEnd();
  }
  return true;
}
/*
void CDialogExportPlot::SetupTextCtrlWidth(wxTextCtrl *p, int nMin)
{
  wxClientDC dc(p);
  dc.SetFont(p->GetFont());
  wxSize sz = dc.GetTextExtent(p->GetValue());
  wxSize szCurrent = p->GetSize();
  sz.SetWidth(sz.GetWidth() + 8);
  if(sz.GetWidth() < nMin)
  {
    sz.SetWidth(nMin);
  }
  sz.SetHeight(szCurrent.GetHeight());
  p->SetSize(sz);
}
*/
void CDialogExportPlot::_TransferWidthHeightToWindow(double dWidth, double dHeight)
{
  wxString sWidth;
  wxString sHeight;
  if(m_nUnits == UNIT_PIXELS)
  {
    sWidth = nwxString::FormatNumber(nwxRound::Round(dWidth));
    sHeight = nwxString::FormatNumber(nwxRound::Round(dHeight));
  }
  else
  {
    sWidth = nwxString::FormatNumber(dWidth);
    sHeight = nwxString::FormatNumber(dHeight);
  }
  m_pTextWidth->ChangeValue(sWidth);
  m_pTextHeight->ChangeValue(sHeight);
  _SetType();
}
void CDialogExportPlot::_CleanupBitmap()
{
  if(m_pBitmap != NULL)
  {
    delete m_pBitmap;
    m_pBitmap = NULL;
  }
}
void CDialogExportPlot::_BuildBitmap()
{
  if(m_pBitmap == NULL)
  {
    wxString sTitle = m_pTextTitle->GetValue();
    double dWidth = _GetWidth();
    double dHeight = _GetHeight();
    double dMult;
    int nWidth;
    int nHeight;
    int nDPI = _GetDPI();

    nwxString::Trim(&sTitle);
    switch(m_nUnits)
    {
    case UNIT_IN:
      nWidth = nwxRound::Round(dWidth * nDPI);
      nHeight = nwxRound::Round(dHeight * nDPI);
      break;
    case UNIT_CM:
      dMult = (1.0 / 2.54) * nDPI;
      nWidth = nwxRound::Round(dWidth * dMult);
      nHeight = nwxRound::Round(dHeight * dMult);
      break;
    case UNIT_PIXELS:
    default:
      nWidth = nwxRound(dWidth);
      nHeight = nwxRound(dHeight);
      break;
    };
    m_pBitmap = m_pParent->CreateBitmap(nWidth,nHeight,nDPI,sTitle);
  }
}
bool CDialogExportPlot::TransferDataFromWindow()
{
  wxBusyCursor cr;
  wxString sError;
  bool bRtn = true;
  bool bView = m_pCheckViewLocation->GetValue();
  _BuildBitmap();
  if(m_pBitmap == NULL)
  {
    sError = "ERROR: Could not create image.";
    wxASSERT_MSG(0,sError);
    mainApp::LogMessage(sError);
  }
  else
  {
    wxString sFileName = m_pTextFileName->GetValue();
    wxImage img = m_pBitmap->ConvertToImage();
    nwxStaticBitmap::AddPngHandler();
    if(!img.SaveFile(sFileName,wxBITMAP_TYPE_PNG))
    {
      sError = "ERROR: Could not save file: \n  ";
      sError.Append(sFileName);
    }
    else if(bView)
    {
      if(!nwxFileUtil::ShowFileFolder(sFileName))
      {
        sError =
          "WARNING: Could not view file folder\n"
             "but the file was successfully saved";
      }
    }
  }
  if(!sError.IsEmpty())
  {
    mainApp::ShowError(sError,this);
    mainApp::LogMessage(sError);
    bRtn = false;
  }
  else
  {
    CParmOsirisGlobal parm;
    double dH = _GetHeight();
    double dW = _GetWidth();
    parm->SetPNGheight(dH);
    parm->SetPNGwidth(dW);
    parm->SetPNGunits(m_nUnits);
    parm->SetPNGusage(m_nUsage);
    parm->SetPNGviewLocation(bView);
  }
  return bRtn;
}

const int CDialogExportPlot::nDPI_PRINT = 300;
const int CDialogExportPlot::nDPI_SCREEN = 72;
const double CDialogExportPlot::dPRINT_TO_SCREEN = (72.0 / 300.0);
const double CDialogExportPlot::dSCREEN_TO_PRINT = (300.0 / 72.0);

void CDialogExportPlot::_UpdateValidator()
{
  if(m_pTextWidth != NULL)
  {
    m_pTextWidth->SetValidator(m_valid);
    m_pTextHeight->SetValidator(m_valid);
  }
}
void CDialogExportPlot::_SetRange()
{
  double dMin = 320.0;
  double dMax = 6000.0;
  bool bLen = false;
  switch(m_nUnits)
  {
  case UNIT_IN:
    dMin = 1.0;
    dMax = 20.0;
    bLen = true;
    break;
  case UNIT_CM:
    dMin = 2.54;
    dMax = 50.8;
    bLen = true;
    break;
  default:
    break;
  };
  if(bLen && (m_nUsage == USAGE_SCREEN))
  {
    dMin *= 4.0;
  }
  m_valid.SetRange(dMin,dMax);
  _UpdateValidator();
}

void CDialogExportPlot::_SetTypeInt(bool bInteger)
{
  if(m_valid.GetTypeInt() != bInteger)
  {
    m_valid.SetTypeInt(bInteger);
    _UpdateValidator();
  }
}


double CDialogExportPlot::_GetDValue(wxTextCtrl *p)
{
  wxString s = p->GetValue();
  double dRtn = atof(s.utf8_str());
  return dRtn;
}

void CDialogExportPlot::OnUnits(wxCommandEvent &)
{
  int n = m_pUnits->GetSelection();
  if(n != m_nUnits)
  {
    double dHeight;
    double dWidth;
    double dMult = 1.0;
    double dDPI = (m_nUsage == USAGE_PRINT) ? nDPI_PRINT : nDPI_SCREEN;
    bool bInt = true;

    switch(m_nUnits)
    {
      // set multiplier to change to pixels
    case UNIT_IN:
      dMult = dDPI;
      break;
    case UNIT_CM:
      dMult = (1.0 / 2.54) * dDPI;
      break;
    case UNIT_PIXELS:
    default:
      break;

    }
    switch(n)
    {
      // set multiplier to change from pixels
    case UNIT_IN:
      dMult /= dDPI;
      bInt = false;
      break;
    case UNIT_CM:
      dMult /= ((1.0 / 2.54) * dDPI);
      bInt = false;
      break;
    case UNIT_PIXELS:
    default:
      break;
    }
    dWidth = _GetWidth();
    dHeight = _GetHeight();
    dWidth *= dMult;
    dHeight *= dMult;
    m_nUnits = n;
    _SetType();
    _SetRange();
    _TransferWidthHeightToWindow(dWidth,dHeight);
  }
  _CleanupBitmap();
}


void CDialogExportPlot::OnUsage(wxCommandEvent &)
{
  m_nUsage = m_pUsage->GetSelection();
  if(m_nUnits == UNIT_PIXELS)
  {
    double dWidth = _GetWidth();
    double dHeight = _GetHeight();
    double dMult =
      (m_nUsage == USAGE_SCREEN)
      ? dPRINT_TO_SCREEN
      : dSCREEN_TO_PRINT;
    dHeight *= dMult;
    dWidth *= dMult;
    _TransferWidthHeightToWindow(dWidth,dHeight);
  }
  _SetRange();
  _CleanupBitmap();
}
void CDialogExportPlot::OnChangeText(wxCommandEvent &)
{
  _CleanupBitmap();
}
void CDialogExportPlot::OnButtonBrowse(wxCommandEvent &)
{
  wxString sFileName;
  if(m_pDialogFile == NULL)
  {
    sFileName = m_pTextFileName->GetValue();
    m_pDialogFile = nwxFileUtil::CreateSaveDialog(
      this,"Save File",FILE_TYPE_PNG,sFileName,m_sDefaultFile);
  }
  if(m_pDialogFile->ShowModal() == wxID_OK)
  {
    sFileName = m_pDialogFile->GetPath();
    m_pTextFileName->ChangeValue(sFileName);
  }
}


class CDialogExportPlotPreview : public nwxDialogBitmap
{
public:
  CDialogExportPlotPreview(
    wxBitmap *pBitmap,
    wxWindow *parent,
    const wxPoint &pos,
    const wxSize &sz) :
    nwxDialogBitmap(
      pBitmap, parent, wxID_ANY, "Preview Image",
      GET_PERSISTENT_POSITION_DEFAULT(CDialogExportPlotPreview, pos),
      GET_PERSISTENT_SIZE_DEFAULT(CDialogExportPlotPreview, sz), 
      mainApp::DIALOG_STYLE | wxMAXIMIZE_BOX)
  {}
  virtual ~CDialogExportPlotPreview() {}
  DECLARE_PERSISTENT_SIZE_POSITION
  DECLARE_EVENT_TABLE()
};

IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogExportPlotPreview)

BEGIN_EVENT_TABLE(CDialogExportPlotPreview, nwxDialogBitmap)
EVT_PERSISTENT_SIZE_POSITION(CDialogExportPlotPreview)
END_EVENT_TABLE()


void CDialogExportPlot::OnButtonPreview(wxCommandEvent &)
{
  if(Validate())
  {
	_BuildBitmap();
    if(m_pBitmap == NULL)
    {
      wxString sError = "ERROR: Could not create image.";
      wxASSERT_MSG(0,sError);
      mainApp::LogMessage(sError);
    }
    else
    {
      wxRect r = wxGetClientDisplayRect();

      wxSize sz(r.GetWidth(),r.GetHeight());  //wxGetDisplaySize();
      int nx = sz.GetWidth() / 20;
      int ny = sz.GetHeight() / 20;
      wxPoint pt(nx + r.GetX(), ny + r.GetY());
      sz.SetWidth(sz.GetWidth() - nx - nx);
      sz.SetHeight(sz.GetHeight() - ny - ny);

      CDialogExportPlotPreview dlg(
        m_pBitmap,this,pt,sz);
      dlg.SetFitToWindow();
      dlg.ShowModal();
    }
  }
}

BEGIN_EVENT_TABLE(CDialogExportPlot,wxDialog)
EVT_RADIOBOX(IDpngUsage,CDialogExportPlot::OnUsage)
EVT_COMBOBOX(IDpngUnits,CDialogExportPlot::OnUnits)
EVT_BUTTON(IDpreview,CDialogExportPlot::OnButtonPreview)
EVT_BUTTON(IDbrowseOutput,CDialogExportPlot::OnButtonBrowse)
EVT_TEXT(IDExportGraphic,CDialogExportPlot::OnChangeText)
END_EVENT_TABLE()
