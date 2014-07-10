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
*  FileName: CDialogEditGridColours.cpp
*  Author:   Douglas Hoffman
*
*/

#include "mainApp.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/dcmemory.h>
#include <wx/brush.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/settings.h>

#include <wx/bitmap.h>
#include "CDialogEditGridColours.h"
#include "CGridAnalysis.h"
#include "CParmOsiris.h"
#include "mainFrame.h"
#include "wxIDS.h"
#include "nwx/nwxGrid.h"

typedef enum
{
  ROW_NORMAL = 0,
  ROW_CELL_ALERT,
  ROW_SAMPLE_ALERT,
  ROW_CELL_EDITED,
  ROW_SAMPLE_EDITED,
  ROW_CELL_ALERT_EDITED,
  ROW_SAMPLE_ALERT_EDITED,
  ROW_DISABLED,
  ROW_ATTN, // sample needs attention
  ROW_OK,
  ROW_COUNT
} EDIT_COLOUR_ROW_NUMBER;
typedef enum
{
  ROW_NEEDS_ATTENTION = 0, // attributes for cell needs attention
  ROW_EDITED,  // attributes for cell has been edited
  ROW_ATTR_COUNT
} EDIT_ATTR_ROW_NUMBER;

#define ROW_COLOR(n) (n <= ROW_OK)

typedef struct
{
  int nRow;
  const wxChar *psLabel;
  int nFlag;
} ROW_INFO;

static const ROW_INFO aROWS_ATTR[] =
{
  {
    ROW_NEEDS_ATTENTION,
    wxS(" Attributes when approval or review is needed"),
    GRID_ALERT_CELL
  },
  {
    ROW_EDITED,
    wxS(" Attributes when edited"),
    GRID_EDITED_CELL | GRID_REVIEWED
  }
};
static const ROW_INFO aROWS[] = 
{
  {
    ROW_NORMAL,
    wxS(" No alerts nor editing in sample"),
    GRID_NORMAL         
  },
  {
    ROW_CELL_ALERT,
    wxS(" Table cell has alerts"),
    GRID_ALERT_CELL | GRID_ACCEPTED
  },
  {
    ROW_SAMPLE_ALERT,
    wxS(" Sample has alerts"),
    GRID_ALERT_SAMPLE
  },
  {
    ROW_CELL_EDITED,
    wxS(" Table cell has been edited"),
    GRID_EDITED_CELL | GRID_REVIEWED
  },
  {
    ROW_SAMPLE_EDITED,
    wxS(" Sample has been edited"),
    GRID_EDITED_SAMPLE
  },
  {
    ROW_CELL_ALERT_EDITED,
    wxS(" Table cell has been edited and has alerts"),
    GRID_EDITED_ALERT_CELL | GRID_REVIEWED
  },
  {
    ROW_SAMPLE_ALERT_EDITED,
    wxS(" Sample has been edited and has alerts"),
    GRID_EDITED_ALERT_SAMPLE
  },
  {
    ROW_DISABLED,
    wxS(" Sample is disabled"),
    GRID_DISABLED
  }
};

//********************************************************* class CGridColorGrid

class CGridColorGrid : public wxGrid
{
public:
  CGridColorGrid(wxWindow *parent, wxWindowID id);
  virtual ~CGridColorGrid();
  void SetupGridColoursAttr(CParmOsiris *pParm);
};

CGridColorGrid::~CGridColorGrid() {}

CGridColorGrid::CGridColorGrid(
  wxWindow *parent, wxWindowID id) : 
    wxGrid(parent, id, wxDefaultPosition, wxDefaultSize, 
      wxBORDER_SIMPLE | wxWANTS_CHARS)
{
  wxFont fnStatus = CGridAnalysis::GetFontStatus();
  int nRow;
  size_t i;
  const size_t N = sizeof(aROWS) / sizeof(aROWS[0]);
  CreateGrid((int)ROW_COUNT,1);

  SetRowLabelSize(1);
  SetColLabelSize(1);
  DisableDragColSize();
  DisableDragRowSize();
  nwxGrid::SetAllReadOnly(this,true);

  fnStatus.SetPointSize(GetDefaultCellFont().GetPointSize());
  SetColLabelValue(0,wxEmptyString);

  const ROW_INFO *pRow = aROWS;
  for(i = 0; i < N; i++)
  {
    nRow = (int) pRow->nRow;
    SetRowLabelValue(nRow,wxEmptyString);
    SetCellValue(nRow,0,pRow->psLabel);
    pRow++;
  }

  SetRowLabelValue((int)ROW_ATTN,wxEmptyString);
  SetCellFont((int) ROW_ATTN,0,fnStatus);
  SetCellValue((int) ROW_ATTN,0,CGridAnalysis::g_sStatusNeedsAttention);
  SetCellAlignment(wxALIGN_CENTRE,(int)ROW_ATTN,0);

  SetRowLabelValue((int)ROW_OK,wxEmptyString);
  SetCellFont((int) ROW_OK,0,fnStatus);
  SetCellValue((int) ROW_OK,0,CGridAnalysis::g_sStatusOK);
  SetCellAlignment(wxALIGN_CENTRE,(int)ROW_OK,0);

}


void CGridColorGrid::SetupGridColoursAttr(CParmOsiris *pParm)
{
  wxFont fn = GetDefaultCellFont();
  wxFontWeight fw = wxFONTWEIGHT_NORMAL;
  int fs = wxFONTSTYLE_NORMAL;
  const size_t N = sizeof(aROWS) / sizeof(aROWS[0]);
  const ROW_INFO *pRow = aROWS;
  size_t i;
  int nRow;
  int nFlag;
  wxColour cFG;
  wxColour cBG;
  for(i = 0; i < N; i++)
  {
    nRow = (int) pRow->nRow;
    nFlag = pRow->nFlag;
    cFG = pParm->GetForeground(nFlag);
    cBG = pParm->GetBackground(nFlag);
    fw = pParm->IsGridBold(nFlag) ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL;
    fs = pParm->IsGridItalic(nFlag) ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL;
    fn.SetWeight(fw);
    fn.SetStyle(fs);
    SetCellFont(nRow,0,fn);
    SetCellTextColour(nRow,0,cFG);
    SetCellBackgroundColour(nRow,0,cBG);
    pRow++;
  }

  SetCellTextColour(
    (int)ROW_ATTN,0,pParm->GetStatusForeground(true));
  SetCellBackgroundColour(
    (int)ROW_ATTN,0,pParm->GetStatusBackground(true));
  SetCellTextColour(
    (int)ROW_OK,0,pParm->GetStatusForeground(false));
  SetCellBackgroundColour(
    (int)ROW_OK,0,pParm->GetStatusBackground(false));
}

//********************************************************* class CGridAttrGrid

class CGridAttrGrid : public wxGrid
{
public:
  CGridAttrGrid(wxWindow *parent, wxWindowID id);
  virtual ~CGridAttrGrid();
  void SetupGridColoursAttr(CParmOsiris *pParm);
};

CGridAttrGrid::~CGridAttrGrid() {}

CGridAttrGrid::CGridAttrGrid(wxWindow *parent, wxWindowID id) :
  wxGrid(parent,id,wxDefaultPosition,wxDefaultSize, 
      wxBORDER_SIMPLE | wxWANTS_CHARS)
{
  CreateGrid((int) ROW_ATTR_COUNT,1);
  SetRowLabelSize(1);
  SetColLabelSize(1);
  DisableDragColSize();
  DisableDragRowSize();
  nwxGrid::SetAllReadOnly(this,true);
  SetColLabelValue(0,wxEmptyString);
  const ROW_INFO *pRow = aROWS_ATTR;
  const size_t N = sizeof(aROWS_ATTR) / sizeof(aROWS_ATTR[0]);
  size_t i;
  int nRow;

  for(i = 0; i < N; i++)
  {
    nRow = (int) pRow->nRow;
    SetRowLabelValue(nRow,wxEmptyString);
    SetCellValue(nRow,0,pRow->psLabel);
    pRow++;
  }
}

void CGridAttrGrid::SetupGridColoursAttr(CParmOsiris *pParm)
{
  wxFont fn = GetDefaultCellFont();
  wxFontWeight fw = wxFONTWEIGHT_NORMAL;
  int fs = wxFONTSTYLE_NORMAL;
  const size_t N = sizeof(aROWS_ATTR) / sizeof(aROWS_ATTR[0]);
  const ROW_INFO *pRow = aROWS_ATTR;
  size_t i;
  int nRow;
  int nFlag;
  wxColour cFG;
  wxColour cBG;
  wxColour c;
  for(i = 0; i < N; i++)
  {
    nRow = (int) pRow->nRow;
    nFlag = pRow->nFlag;
    cFG = pParm->GetForeground(nFlag);
    cBG = pParm->GetBackground(nFlag);
    if( pParm->IsGridReverse(nFlag) )
    {
        c = cFG;
        cFG = cBG;
        cBG = c;
    }
    fw = pParm->IsGridBold(nFlag) ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL;
    fs = pParm->IsGridItalic(nFlag) ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL;
    fn.SetWeight(fw);
    fn.SetStyle(fs);
    SetCellFont(nRow,0,fn);
    SetCellTextColour(nRow,0,cFG);
    SetCellBackgroundColour(nRow,0,cBG);
    pRow++;
  }
}



//********************************************************* class CDialogEditGridColours

CDialogEditGridColours::~CDialogEditGridColours() {}

CDialogEditGridColours::CDialogEditGridColours(mainFrame *parent) : 
  wxDialog(parent, wxID_ANY, wxS("Edit Grid Colors"),
    wxDefaultPosition, wxDefaultSize,
    mainApp::DIALOG_STYLE & ~wxRESIZE_BORDER),
  m_nRowColor(-1),
  m_nRowAttr(-1),
  m_nInSelect(0),
  m_bModified(false)
{
  wxFont fnt;
  wxSize sz;
  wxSize sz2;
  int nw;
  int nw2;

  wxStaticText *pTextTop;
  wxStaticText *pTextAttributes;
  wxButton *pButtonReset;

  wxBoxSizer *pSizerAll;
  wxBoxSizer *pSizerColors;
  wxBoxSizer *pSizerAttributes;
  wxBoxSizer *pSizerWin;
  wxFlexGridSizer *pSizer2x2;
  wxSizer *pSizerButtons;

  pTextTop = new wxStaticText(
    this,wxID_ANY,
    "Select a line in the lists below and use the\n"
    "buttons to modify its colors or attributes\n\nColors");
  m_pGridColor = new CGridColorGrid(this,wxID_ANY);

    
  m_pTextFG = new wxStaticText(this,wxID_ANY,"Foreground Color: ");
  m_pButtonFG = _CreateButton(this,IDbuttonEditFGcolor);
  m_pTextBG = new wxStaticText(this,wxID_ANY,"Background Color: ");
  m_pButtonBG = _CreateButton(this,IDbuttonEditBGcolor);
  m_pButtonReverse = new wxButton(this,IDbuttonReverse,"Reverse");

  pSizer2x2 = new wxFlexGridSizer(2,2,ID_BORDER,ID_BORDER);
  pSizer2x2->Add(m_pTextFG,0,wxALIGN_CENTER_VERTICAL);
  pSizer2x2->Add(m_pButtonFG,0,wxALIGN_CENTER_VERTICAL);
  pSizer2x2->Add(m_pTextBG,0,wxALIGN_CENTER_VERTICAL);
  pSizer2x2->Add(m_pButtonBG,0,wxALIGN_CENTER_VERTICAL);

  pSizerColors = new wxBoxSizer(wxHORIZONTAL);
  pSizerColors->Add(pSizer2x2,0,wxALIGN_CENTER_VERTICAL);
  pSizerColors->Add(m_pButtonReverse,0,
    wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxLEFT, ID_BORDER);

  pTextAttributes = new wxStaticText(this,wxID_ANY,"Attributes");
  m_pGridAttr = new CGridAttrGrid(this,wxID_ANY);


  m_pToggleBold = new wxToggleButton(this,wxID_ANY,"Bold");
  m_pToggleItalic = new wxToggleButton(this,wxID_ANY,"Italic");
  m_pToggleReverse = new wxToggleButton(this,wxID_ANY,"Reverse");
  fnt = m_pToggleItalic->GetFont();
  fnt.SetStyle(wxFONTSTYLE_ITALIC);
  m_pToggleItalic->SetFont(fnt);
  fnt.SetStyle(wxFONTSTYLE_NORMAL);
  fnt.SetWeight(wxFONTWEIGHT_BOLD);
  m_pToggleBold->SetFont(fnt);

  pButtonReset = new wxButton(
    this,IDbuttonResetDefault, "Reset Defaults");

  {
    CParmOsirisGlobal pParm;
    _SetupGridColoursAttr(pParm.Get());
  }

  m_nRowColor = -1;
  m_nRowAttr = -1;
  _UpdateButtons(0,0);

  m_pGridColor->SetGridCursor(0,0);
  m_pGridAttr->SetGridCursor(0,0);
  m_pGridColor->AutoSizeColumns(true);
  m_pGridAttr->AutoSizeColumns(true);
  m_pGridColor->AutoSizeRows(true);
  m_pGridAttr->AutoSizeRows(true);


  // do full layout

  sz =  _ComputeGridSize(m_pGridColor);
  sz2 = _ComputeGridSize(m_pGridAttr);
  nw = sz.GetWidth();
  nw2 = sz2.GetWidth();
  if(nw2 > nw)
  {
    sz.SetWidth(nw2);
    m_pGridColor->SetColSize(0,m_pGridAttr->GetColSize(0));
  }
  else
  {
    sz2.SetWidth(nw);
    m_pGridAttr->SetColSize(0,m_pGridColor->GetColSize(0));
  }
  m_pGridColor->SetSize(sz);
  m_pGridAttr->SetSize(sz2);

  pSizerAttributes = new wxBoxSizer(wxHORIZONTAL);
  pSizerAttributes->Add(m_pToggleBold,0,0,0);
  pSizerAttributes->Add(m_pToggleItalic,0,wxLEFT, ID_BORDER);
  pSizerAttributes->Add(m_pToggleReverse,0,wxLEFT, ID_BORDER);

  pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(pTextTop,0,wxALL | wxALIGN_LEFT, ID_BORDER);
  pSizerAll->Add(m_pGridColor,0,wxEXPAND | (wxALL ^ wxTOP),ID_BORDER);
  pSizerAll->Add(pSizerColors,0,
    wxALIGN_CENTER_HORIZONTAL | (wxALL ^ wxTOP), ID_BORDER);
  pSizerAll->Add(pTextAttributes,0,wxEXPAND | wxALL , ID_BORDER);
  pSizerAll->Add(m_pGridAttr,0,wxEXPAND | (wxALL ^ wxTOP), ID_BORDER);
  pSizerAll->Add(pSizerAttributes,0,wxALIGN_CENTER_HORIZONTAL | wxALL, ID_BORDER);
  pSizerAll->Add(pButtonReset,0,wxALIGN_CENTER_HORIZONTAL | (wxALL ^ wxBOTTOM),ID_BORDER);


  pSizerButtons = CreateButtonSizer(wxOK | wxCANCEL);
  pSizerWin = new wxBoxSizer(wxVERTICAL);
  pSizerWin->Add(pSizerAll,1,wxEXPAND,0);
  pSizerWin->Add(pSizerButtons,0,wxALIGN_CENTER_HORIZONTAL | wxALL, ID_BORDER);
  SetSizer(pSizerWin);
  pSizerWin->Layout();
  Fit();
  CentreOnParent();
  m_pGridColor->SetFocus();
}

void CDialogEditGridColours::_SetupGridColoursAttr(CParmOsiris *pParm)
{
  m_nAttrNeedsAttn = pParm->GetAttrNeedsAttn();
  m_nAttrEdited = pParm->GetAttrEdited();
  m_pGridColor->SetupGridColoursAttr(pParm);
  m_pGridAttr->SetupGridColoursAttr(pParm);
}
wxSize CDialogEditGridColours::_ComputeGridSize(wxGrid *pGrid)
{
  int nRow = pGrid->GetNumberRows() - 1;
  int nCol = pGrid->GetNumberCols() - 1;
  wxRect r = pGrid->CellToRect(nRow,nCol);
  wxPoint pt = (r.GetBottomRight());
  int nScrollWidth = wxSystemSettings::GetMetric(wxSYS_VSCROLL_X);
  int nScrollHeight = wxSystemSettings::GetMetric(wxSYS_HSCROLL_Y);
  wxSize sz(pt.x + nScrollWidth,pt.y + nScrollHeight);
  return sz;
}
bool CDialogEditGridColours::TransferDataToWindow()
{
  return true;
}
bool CDialogEditGridColours::TransferDataFromWindow()
{
  wxColour cFG;
  wxColour cBG;
  CParmOsirisGlobal pParm;
  int nRow;
  size_t i;
  const size_t N = sizeof(aROWS) / sizeof(aROWS[0]);
  const ROW_INFO *pRow = aROWS;
  GRID_FLAG nFlag;
  for(i = 0; i < N; i++)
  {
    nRow = (int) pRow->nRow;
    nFlag = (GRID_FLAG)pRow->nFlag;
    cFG = m_pGridColor->GetCellTextColour(nRow,0);
    cBG = m_pGridColor->GetCellBackgroundColour(nRow,0);
    pParm->SetForeground(nFlag,cFG);
    pParm->SetBackground(nFlag,cBG);
    pRow++;
  }
  cFG = m_pGridColor->GetCellTextColour((int)ROW_ATTN,0);
  cBG = m_pGridColor->GetCellBackgroundColour((int)ROW_ATTN,0);
  pParm->SetStatusForeground(cFG,true);
  pParm->SetStatusBackground(cBG,true);
  cFG = m_pGridColor->GetCellTextColour((int)ROW_OK,0);
  cBG = m_pGridColor->GetCellBackgroundColour((int)ROW_OK,0);
  pParm->SetStatusForeground(cFG,false);
  pParm->SetStatusBackground(cBG,false);
  pParm->SetAttrEdited(m_nAttrEdited);
  pParm->SetAttrNeedsAttn(m_nAttrNeedsAttn);
  m_bModified = pParm->IsModified();
  return true;
}
ColorButton *CDialogEditGridColours::_CreateButton(wxWindow *parent,wxWindowID id)
{
  wxBitmap bm(60,16,-1);
  wxBitmapButton *pRtn = new wxBitmapButton(parent,id,bm);
  pRtn->SetToolTip("Click here to select a different color.");
  return pRtn;
}

void CDialogEditGridColours::_SetButtonColour(ColorButton *p, const wxColour &c)
{
  wxBitmap bitmap(p->GetBitmapLabel());
  wxMemoryDC dc(bitmap);
  wxBrush brush(c);
  dc.SetBackgroundMode(wxSOLID);
  dc.SetBackground(brush);
  dc.Clear();
  p->SetBitmapFocus(bitmap);
  p->SetBitmapHover(bitmap);
  p->SetBitmapLabel(bitmap);
  p->SetBitmapSelected(bitmap);
  p->SetBitmapDisabled(bitmap);
  p->Refresh();
}
void CDialogEditGridColours::_UpdateButtons(int nRowColor, int nRowAttr)
{
  if(nRowColor != m_nRowColor)
  {
    if(nRowColor < 0)
    {
      nRowColor = m_nRowColor;
    }
    if(nRowAttr < 0)
    {
      nRowAttr = m_nRowAttr;
    }
    wxColour cFG = m_pGridColor->GetCellTextColour(nRowColor,0);
    wxColour cBG = m_pGridColor->GetCellBackgroundColour(nRowColor,0);

    _SetButtonColour(m_pButtonFG,cFG);
    _SetButtonColour(m_pButtonBG,cBG);
    m_nRowColor = nRowColor;
  }
  if(nRowAttr != m_nRowAttr)
  {
    bool bBold = false;
    bool bItalic = false;
    bool bReverse = false;
    bool bEnableReverse = false;
    int n;
    if(nRowAttr == ROW_NEEDS_ATTENTION)
    {
      bEnableReverse = true;
      n = m_nAttrNeedsAttn;
      bReverse = CParmOsiris::IsAttrReverse(n);
    }
    else
    {
      n = m_nAttrEdited;
    }
    bBold = CParmOsiris::IsAttrBold(n);
    bItalic = CParmOsiris::IsAttrItalic(n);

    m_pToggleReverse->Enable(bEnableReverse);
    m_pToggleBold->SetValue(bBold);
    m_pToggleItalic->SetValue(bItalic);
    m_pToggleReverse->SetValue(bReverse);

    m_nRowAttr = nRowAttr;
  }
}
bool CDialogEditGridColours::_ShowColourDialog(wxColour *pColour)
{
  mainFrame *p = (mainFrame *) GetParent();
  bool bRtn = p->ShowColourDialog(pColour);
  return bRtn;
}
void CDialogEditGridColours::OnButtonReverse(wxCommandEvent &)
{
  wxColour cBG = m_pGridColor->GetCellBackgroundColour(m_nRowColor,0);
  wxColour cFG = m_pGridColor->GetCellTextColour(m_nRowColor,0);
  m_pGridColor->SetCellBackgroundColour(m_nRowColor,0,cFG);
  m_pGridColor->SetCellTextColour(m_nRowColor,0,cBG);
  _UpdateColorsEdited();
  m_pGridColor->Refresh();
  _UpdateButtons(-1,m_nRowAttr);
  m_pGridColor->SetFocus();
}
void CDialogEditGridColours::OnButtonBG(wxCommandEvent &)
{
  wxColour c = m_pGridColor->GetCellBackgroundColour(m_nRowColor,0);
  if(_ShowColourDialog(&c))
  {
    m_pGridColor->SetCellBackgroundColour(m_nRowColor,0,c);
    _SetButtonColour(m_pButtonBG,c);
    m_pGridColor->Refresh();
    _UpdateColorsEdited();
  }
  m_pGridColor->SetFocus();
}
void CDialogEditGridColours::OnButtonFG(wxCommandEvent &)
{
  wxColour c = m_pGridColor->GetCellTextColour(m_nRowColor,0);
  if(_ShowColourDialog(&c))
  {
    m_pGridColor->SetCellTextColour(m_nRowColor,0,c);
    _SetButtonColour(m_pButtonFG,c);
    m_pGridColor->Refresh();
    _UpdateColorsEdited();
  }
  m_pGridColor->SetFocus();
}
void CDialogEditGridColours::OnButtonDefault(wxCommandEvent &)
{
  CParmOsirisGlobal pParm;
  CParmOsiris Parm(*pParm.Get());
  Parm.SetDefaultColours();
  _SetupGridColoursAttr(&Parm);
  _UpdateButtons(-1,-1);
  m_pGridColor->Refresh();
  m_pGridAttr->Refresh();
  m_pGridColor->SetFocus();
}
void CDialogEditGridColours::OnAttrButton(wxCommandEvent &e)
{
  wxFontWeight fw = wxFONTWEIGHT_NORMAL;
  int fs = wxFONTSTYLE_NORMAL;
  int nAttr = 0;
  if(m_pToggleBold->GetValue())
  {
    nAttr |= (int)GRID_BOLD;
    fw = wxFONTWEIGHT_BOLD;
  }
  if(m_pToggleItalic->GetValue())
  {
    nAttr |= (int)GRID_ITALIC;
    fs = wxFONTSTYLE_ITALIC;
  }
  if(m_nRowAttr == ROW_EDITED)
  {
    m_nAttrEdited = nAttr;
  }
  else if(m_nRowAttr == ROW_NEEDS_ATTENTION)
  {
    if(m_pToggleReverse->GetValue())
    {
      nAttr |= (int)GRID_REVERSE;
    }
    m_nAttrNeedsAttn = nAttr;
  }
  if(e.GetEventObject() == (wxObject *)m_pToggleReverse)
  {
    wxColour cFG = m_pGridAttr->GetCellTextColour(m_nRowAttr,0);
    wxColour cBG = m_pGridAttr->GetCellBackgroundColour(m_nRowAttr,0);
    m_pGridAttr->SetCellTextColour(m_nRowAttr,0,cBG);
    m_pGridAttr->SetCellBackgroundColour(m_nRowAttr,0,cFG);
  }
  else
  {
    wxFont fn = m_pGridColor->GetCellFont(m_nRowAttr,0);
    fn.SetStyle(fs);
    fn.SetWeight(fw);
    m_pGridAttr->SetCellFont(m_nRowAttr,0,fn);
    _UpdateAttributesEdited();
  }
  m_pGridAttr->Refresh();
  m_pGridAttr->SetFocus();
}
void CDialogEditGridColours::OnGrid(wxGridEvent &e)
{
  int nRowColor = m_nRowColor;
  int nRowAttr = m_nRowAttr;
  int nRow = e.GetRow();
  wxGrid *pGrid = (wxGrid *)e.GetEventObject();
  if(pGrid == m_pGridAttr)
  {
    nRowAttr = nRow;
  }
  else if(pGrid == m_pGridColor)
  {
    nRowColor = nRow;
  }
  else
  {
    nRowColor = -1;
    nRowAttr = -1;
    wxASSERT_MSG(0,"CDialogEditGridColours::OnGrid() unknown grid");
  }
  _UpdateButtons(nRowColor,nRowAttr);
  e.Skip(true);
}
void CDialogEditGridColours::OnGridSelect(wxGridRangeSelectEvent &e)
{
  bool bSkip = true;
  if(!m_nInSelect)
  {
    CIncrementer xxx(m_nInSelect);
    if(e.Selecting())
    {
      wxGrid *pGrid = (wxGrid *) e.GetEventObject();
      int nRow = pGrid->GetGridCursorRow();
      pGrid->ClearSelection();
      pGrid->SetGridCursor(nRow,0);
/*
      int nTop = e.GetTopRow();
      int nBot = e.GetBottomRow();
      if(nTop != nBot)
      {
        int nRow = pGrid->GetGridCursorRow();
        pGrid->ClearSelection();
        pGrid->SetGridCursor(nRow,0);
      }
*/
    }
  }
  e.Skip(bSkip);
}
void CDialogEditGridColours::_UpdateAttributesEdited()
{
  if(m_nRowAttr == ROW_EDITED)
  {
    wxFont fn = m_pGridColor->GetCellFont(ROW_CELL_EDITED,0);
    wxFontWeight fw = wxFONTWEIGHT_NORMAL;
    int fs = wxFONTSTYLE_NORMAL;
    if(CParmOsiris::IsAttrBold(m_nAttrEdited))
    {
      fw = wxFONTWEIGHT_BOLD;
    }
    if(CParmOsiris::IsAttrItalic(m_nAttrEdited))
    {
      fs = wxFONTSTYLE_ITALIC;
    }

    fn.SetStyle(fs);
    fn.SetWeight(fw);
    m_pGridColor->SetCellFont(ROW_CELL_EDITED,0,fn);
    m_pGridColor->SetCellFont(ROW_CELL_ALERT_EDITED,0,fn);
    m_pGridColor->Refresh();
  }
}
void CDialogEditGridColours::_UpdateColorsEdited()
{
  int nRow = -1;
  bool bReverse = false;
  if(m_nRowColor == ROW_CELL_EDITED)
  {
    nRow = ROW_EDITED;
    bReverse = CParmOsiris::IsAttrReverse(m_nAttrEdited);
  }
  else if(m_nRowColor == ROW_CELL_ALERT)
  {
    nRow = ROW_NEEDS_ATTENTION;
    bReverse = CParmOsiris::IsAttrReverse(m_nAttrNeedsAttn);
  }
  if(nRow >= 0)
  {
    wxColour cFG = m_pGridColor->GetCellTextColour(m_nRowColor,0);
    wxColour cBG = m_pGridColor->GetCellBackgroundColour(m_nRowColor,0);
    if(bReverse)
    {
      wxColour c = cFG;
      cFG = cBG;
      cBG = c;
    }
    m_pGridAttr->SetCellTextColour(nRow,0,cFG);
    m_pGridAttr->SetCellBackgroundColour(nRow,0,cBG);
    m_pGridAttr->Refresh();
  }
}

BEGIN_EVENT_TABLE(CDialogEditGridColours,wxDialog)
EVT_BUTTON(IDbuttonEditFGcolor,CDialogEditGridColours::OnButtonFG)
EVT_BUTTON(IDbuttonEditBGcolor,CDialogEditGridColours::OnButtonBG)
EVT_BUTTON(IDbuttonResetDefault,CDialogEditGridColours::OnButtonDefault)
EVT_BUTTON(IDbuttonReverse,CDialogEditGridColours::OnButtonReverse)
EVT_TOGGLEBUTTON(wxID_ANY,CDialogEditGridColours::OnAttrButton)
EVT_GRID_CMD_SELECT_CELL(wxID_ANY,CDialogEditGridColours::OnGrid)
EVT_GRID_CMD_RANGE_SELECT(wxID_ANY,CDialogEditGridColours::OnGridSelect)
END_EVENT_TABLE()

