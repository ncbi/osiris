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
*  FileName: CGridAnalysis.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CGridAnalysis.h"
#include "CFrameAnalysis.h"
#include "COARfile.h"
#include "COARsampleSort.h"
#include "wxIDS.h"
#include "CLabSettings.h"
#include "nwx/nwxBatch.h"
#include "nwx/nwxGrid.h"
#include "Platform.h"

#if USE_WINGDINGS
wxFont CGridAnalysis::g_fontStatus(8,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,"wingdings");
#else
// X11 if ever implemented
wxFont CGridAnalysis::g_fontStatus;
#endif

const wxString CGridAnalysis::g_sSampleLevelNeedsAttention("*");
const wxString CGridAnalysis::g_sStatusNeedsAttention(CHAR_ATTN);
const wxString CGridAnalysis::g_sStatusOK(CHAR_OK);

wxPen CGridAnalysis::g_LIGHT_GREY(wxColour(216,216,216),1,wxSOLID);

IMPLEMENT_ABSTRACT_CLASS(CGridAnalysis,wxGrid)

CGridAnalysis::~CGridAnalysis()
{;}

CGridAnalysis::CGridAnalysis(wxWindow *pParent) :
  wxGrid(pParent,IDgrid),
  m_bgBold(255,255,160),
  m_bgNormal(255,255,255),
  m_bgRowHasAlert(255,255,218),
  m_fgBold(192,0,0),
  m_fgNormal(0,0,0),
  m_pDC(NULL),
  m_nLabelSize(0),
  m_nLabelType(-1),
  m_nXScroll(0),
  m_nYScroll(0),
  m_nIDmenuSort(IDmenuSortFileName)
{
  CreateGrid(1,1);
  SetRowLabelValue(0,wxEmptyString);
  SetColLabelValue(0,wxEmptyString);
  SetReadOnly(0,0,true);
  SetRowLabelAlignment(wxALIGN_LEFT,wxALIGN_CENTRE);
  m_fontNormal = GetDefaultCellFont();
  m_fontItalic = m_fontNormal;
  m_fontBold = m_fontNormal;
  m_fontBold.SetWeight(wxFONTWEIGHT_BOLD);
  m_fontBoldItalic = m_fontBold;
  m_fontBoldItalic.SetStyle(wxFONTSTYLE_ITALIC);
  m_fontItalic.SetStyle(wxFONTSTYLE_ITALIC);
#if USE_WINGDINGS
  g_fontStatus.SetPointSize(m_fontNormal.GetPointSize());
#else
  // X11 if ever implemented
  g_fontStatus = m_fontBold;
#endif

}
  /*
void CGridAnalysis::SetCellStyle(
  int nRow, int nCol, bool bBold, bool bItal,bool bHasAlert)
{
  wxFont *pFont(NULL);
  if(bBold)
  {
    pFont = bItal ? &m_fontBoldItalic : &m_fontBold;
    SetCellFont(nRow,nCol,*pFont);
    SetCellBackgroundColour(nRow,nCol,m_bgBold);
    SetCellTextColour(nRow,nCol,m_fgBold);
  }
  else
  {
    wxColour *pcolor = bHasAlert ? &m_bgRowHasAlert : &m_bgNormal;
    pFont = bItal ? &m_fontItalic : &m_fontNormal;
    SetCellFont(nRow,nCol,*pFont);
    SetCellBackgroundColour(nRow,nCol,*pcolor);
    SetCellTextColour(nRow,nCol,m_fgNormal);
  }
}
*/
void CGridAnalysis::SetCellStyle(
  int nRow, int nCol, GRID_FLAG nFlag)
{
  const int BOLD = 1;
  const int ITALIC = 2;
  const int BOLD_ITALIC = 3;
  int n = 0;
  wxFont *pFont = NULL;
  CParmOsiris *pParm(CParmOsiris::GetGlobal());
  const wxColour &cFG(pParm->GetForeground(nFlag));
  const wxColour &cBG(pParm->GetBackground(nFlag));
  const wxColour *pcFG(&cFG);
  const wxColour *pcBG(&cBG);
  if(pParm->IsGridBold(nFlag))
  {
    n |= BOLD;
  }
  if(pParm->IsGridItalic(nFlag))
  {
    n |= ITALIC;
  }
  switch(n)
  {
  case BOLD:
    pFont = &m_fontBold;
    break;
  case ITALIC:
    pFont = &m_fontItalic;
    break;
  case BOLD_ITALIC:
    pFont = &m_fontBoldItalic;
    break;
  default:
    pFont = &m_fontNormal;
    break;
  }
  SetCellFont(nRow,nCol,*pFont);
  if(pParm->IsGridReverse(nFlag))
  {
    pcFG = &cBG;
    pcBG = &cFG;
  }
  SetCellBackgroundColour(nRow,nCol,*pcBG);
  SetCellTextColour(nRow,nCol,*pcFG);
}
wxString CGridAnalysis::FormatRowLabel(int nRow, int nRowCount, const wxString &sLabel)
{
//  NUMBERED LABEL
  wxString sNumberedLabel;
  const wxChar *FMT = wxS("%d. ");

  // the following if/else is probably faster than a log10
  if(nRowCount >= 1000)
  {
    FMT = wxS("%4d. ");
  }
  else if(nRowCount >= 100)
  {
    FMT = wxS("%3d. ");
  }
  else if(nRowCount >= 10)
  {
    FMT = wxS("%2d. ");
  }
  sNumberedLabel.Alloc(sLabel.Len() + 8);
  sNumberedLabel.Printf(FMT,nRow );
  sNumberedLabel.Append(sLabel);
  return sNumberedLabel;
}
void CGridAnalysis::SetRowLabel(int nRow, const wxString &sLabel)
{
  wxString sNumberedLabel = FormatRowLabel(nRow + 1,GetNumberRows(),sLabel);
  SetRowLabelValue(nRow,sNumberedLabel);
  if(m_pDC != NULL)
  {
    wxSize sz = m_pDC->GetTextExtent(sNumberedLabel);
    int nSize = sz.GetWidth();
    if(nSize > m_nLabelSize)
    {
      m_nLabelSize = nSize;
    }
  }
}
void CGridAnalysis::UpdateLabelSize()
{
  if(m_nLabelSize)
  {
    SetRowLabelSize(m_nLabelSize + 6);
  }
}

bool CGridAnalysis::_SetGridSize(int nRowCount, int nColCount)
{
  // return true if OK, false if error
  int nRowCurrent = GetNumberRows();
  int nColCurrent = GetNumberCols();
  bool bError = false;

  ClearSelection();
  ClearGrid();

  if((!nRowCount) || (nColCount < 1))
  {
    bError = true;
    nRowCount = 1;
    nColCount = 1;
  }
  if(nColCurrent > nColCount)
  {
    DeleteCols(nColCount,nColCurrent - nColCount);
  }
  else if(nColCurrent < nColCount)
  {
    AppendCols(nColCount - nColCurrent);
  }
  if(nRowCurrent > nRowCount)
  {
    DeleteRows(nRowCount,nRowCurrent - nRowCount);
  }
  else if(nRowCurrent < nRowCount)
  {
    AppendRows(nRowCount - nRowCurrent);
  }
  if(bError)
  {
    SetColLabelValue(0,wxEmptyString);
    SetRowLabelValue(0,wxEmptyString);
    SetCellStyle(0,0,GRID_NORMAL);
    SetCellValue(0,0,wxEmptyString);
  }
  return !bError;
}

void CGridAnalysis::_SetupCellFlag(
  GRID_FLAG flagBase,
  bool bSampleDisabled, bool bEdited, bool bAlert,
  int nAcceptCount, int nAcceptNeeded,
  int nReviewCount, int nReviewNeeded,
  GRID_FLAG *pFlag, bool *pbNeedsAttention)
{
  GRID_FLAG nFlag = flagBase;
  if(!bSampleDisabled)
  {
    if(bAlert)
    {
      nFlag |= GRID_ALERT_CELL;
      if(bEdited) {} // handle edited cell below
      else if(nAcceptCount >= nAcceptNeeded)
      {
        nFlag |= GRID_ACCEPTED;
      }
      else if(nReviewCount >= nReviewNeeded)
      {
        nFlag |= GRID_REVIEWED;
      }
      else
      {
        *pbNeedsAttention = true;
      }
    }
    if(bEdited)
    {
      nFlag |= GRID_EDITED_CELL;
      if(nReviewCount >= nReviewNeeded)
      {
        nFlag |= GRID_REVIEWED;
      }
      else
      {
        *pbNeedsAttention = true;
      }
    }
  }
  *pFlag = nFlag;
}

void CGridAnalysis::SetupRowLabel(
  COARsample *pSample,
  int iRow,
  int nLabelTypeName,
  bool bEdited,
  bool bNeedsAttention)
{
  wxString sName;
  sName = 
    (nLabelTypeName == IDmenuDisplayNameSample)
    ? pSample->GetSampleName()
    : pSample->GetName();
  if(bEdited)
  {
    sName.Append(COARsample::g_sCellChannelEdited);
  }
  if(bNeedsAttention)
  {
    sName.Append(g_sSampleLevelNeedsAttention);
  }
  SetRowLabel(iRow,sName);
}


void CGridAnalysis::UpdateGridRowLabels(
  COARsampleSort *pSort, int nLabelTypeName, wxDC *pDC)
{
  wxString s;
  COARsample *pSample;
  size_t nRowCount = pSort->GetCount();
  size_t i;
  bool bEdited;
  bool bNeedsAttention;
  wxDC *pDCuse = (pDC != NULL) ? pDC : GetDC();
  DCholder xx(this,pDCuse);

  m_nLabelType = nLabelTypeName;
  m_nLabelSize = 0;
  for(i = 0; i < nRowCount; i++)
  {
    pSample = pSort->GetSample((size_t)i);
    s = GetRowLabelValue(i).Right(2);
    bNeedsAttention = s.Contains(g_sSampleLevelNeedsAttention);
    bEdited = s.Contains(COARsample::g_sCellChannelEdited);
    SetupRowLabel(pSample,i,nLabelTypeName,bEdited,bNeedsAttention);
  }
  UpdateLabelSize();
}

void CGridAnalysis::UpdateGrid(
  COARfile *pFile, COARsampleSort *pSort,
  int nLabelType, 
  int nLabelTypeName,
  const wxDateTime *pHistory, 
  wxDC *pDC)
{
  nwxGridBatch xBatch(this);
  wxDC *pDCuse = (pDC != NULL) ? pDC : GetDC();
  DCholder xx(this,pDCuse);

  wxString sChannel;
  wxString sCell;
  int iRow;
  int nCol;
  int nReviewCount;
  int nAcceptCount;
  int nReviewNeeded;
  int nAcceptNeeded;
  size_t nRowCount = pSort->GetCount();
  size_t nAlleleColCount = pFile->GetLocusCount();
  size_t i;
  COARlocus *pLocus;
  COARsample *pSample;
  const COARmessages *pMsgs = pFile->GetMessages();
  GRID_FLAG flagBase;
  GRID_FLAG flag;
  bool bAlert;
  bool bEdited;
  bool bSampleHasAlert;
  bool bSampleEdited;
  bool bNeedsAttention;
  bool bSampleDisabled;
  m_nLabelSize = 0;
  m_nLabelType = nLabelTypeName;

  

#if 0
  // moved to next loop
  // Set up row labels
  for(iRow = 0; iRow < (int)nRowCount; ++iRow)
  {
    pSample = pSort->GetSample((size_t)iRow);
    sName = pSample->GetName();
    if(pSample->IsSampleLevelEdited(pFile->GetMessages(),pHistory))
    {
      sName.Append(COARsample::g_sCellChannelEdited);
    }
    SetRowLabel(iRow,sName);
  }
#endif

  // set up cells
  pFile->GetReviewerCounts(&nReviewNeeded,&nAcceptNeeded,CLabReview::REVIEW_SAMPLE);

  for(iRow = 0; iRow < (int)nRowCount; ++iRow)
  {
    // setup sample info
    flagBase = GRID_NORMAL;
    bNeedsAttention = false;
    pSample = pSort->GetSample((size_t)iRow);
    bSampleHasAlert = pSample->HasAnyAlerts(pMsgs,pHistory);
    bSampleEdited = pSample->IsEdited(pMsgs,pHistory);
    bSampleDisabled = pSample->IsDisabled(pHistory);
    const COARsampleReviewAccept &accept(pSample->GetAcceptance());
    const COARsampleReviewAccept &review(pSample->GetReviews());

    if(bSampleDisabled)
    {
      flagBase |= GRID_DISABLED;
    }
    else 
    {
      if(bSampleHasAlert)
      {
        flagBase |= GRID_ALERT_SAMPLE;
      }
      if(bSampleEdited)
      {
        flagBase |= GRID_EDITED_SAMPLE;
      }
    }
    //  Setup Row Label for sample
    bEdited = pSample->IsSampleLevelEdited(pMsgs,pHistory);
    bAlert = pSample->HasSampleAlert(pMsgs,pHistory);
    nAcceptCount = accept.GetSampleCount(pHistory);
    nReviewCount = review.GetSampleCount(pHistory);
    _SetupCellFlag(
      flagBase,
      bSampleDisabled,bEdited,bAlert,
      nAcceptCount, nAcceptNeeded,
      nReviewCount, nReviewNeeded,
      &flag,&bNeedsAttention);
    SetupRowLabel(pSample,iRow,nLabelTypeName,bEdited,bNeedsAttention);


    // ILS alert column

    bAlert = bSampleHasAlert && pSample->HasILSAlert(pMsgs,pHistory);
    bEdited = pSample->IsCellILSEdited(pMsgs,pHistory);
    nAcceptCount = accept.GetILSCount(pHistory);
    nReviewCount = review.GetILSCount(pHistory);
    pFile->GetReviewerCounts(&nReviewNeeded,&nAcceptNeeded,CLabReview::REVIEW_ILS);
    _SetupCellFlag(
      flagBase,
      bSampleDisabled,bEdited,bAlert,
      nAcceptCount, nAcceptNeeded,
      nReviewCount, nReviewNeeded,
      &flag,&bNeedsAttention);
    SetCellValue(iRow,CFrameAnalysis::ILS_COLUMN,pSample->GetCellILS(pMsgs,pHistory));
    SetCellStyle(
      iRow,CFrameAnalysis::ILS_COLUMN,flag);

    // channel alert column

    sChannel = pSample->GetCellChannel(pMsgs,pHistory);
    bAlert = bSampleHasAlert && COARsample::HasChannelAlert(sChannel);
    bEdited = bSampleEdited && pSample->IsCellChannelEdited(pMsgs,pHistory);
    nAcceptCount = accept.GetChannelCount(pHistory);
    nReviewCount = review.GetChannelCount(pHistory);
    pFile->GetReviewerCounts(&nReviewNeeded,&nAcceptNeeded,CLabReview::REVIEW_CHANNEL);

    _SetupCellFlag(
      flagBase,
      bSampleDisabled,bEdited,bAlert,
      nAcceptCount, nAcceptNeeded,
      nReviewCount, nReviewNeeded,
      &flag,&bNeedsAttention);
    SetCellValue(iRow,CFrameAnalysis::CHANNEL_ALERT_COLUMN,sChannel);
    SetCellStyle(iRow,CFrameAnalysis::CHANNEL_ALERT_COLUMN,flag);

    SetReadOnly(iRow,CFrameAnalysis::ILS_COLUMN,true);
    SetReadOnly(iRow,CFrameAnalysis::CHANNEL_ALERT_COLUMN,true);
    SetReadOnly(iRow,CFrameAnalysis::STATUS_COLUMN,true);

    pFile->GetReviewerCounts(&nReviewNeeded,&nAcceptNeeded,CLabReview::REVIEW_LOCUS);
    for(i = 0, nCol = CFrameAnalysis::FIRST_LOCUS_COLUMN;
        i < nAlleleColCount;
        ++i, ++nCol)
    {
      SetReadOnly(iRow,nCol,true);
      pLocus = pSample->FindLocus(pFile->GetLocusName(i));
      if(pLocus != NULL)
      {
        sCell = pLocus->GetCell(nLabelType,pHistory);
        bAlert = bSampleHasAlert && pLocus->HasAlerts(pMsgs,pHistory);
        const COARchannel *pChannel =
          pFile->GetChannelFromLocus(pLocus->GetName());
        int nChannel = pChannel->GetChannelNr();
        nAcceptCount = pLocus->GetAcceptanceCount(pHistory);
        nReviewCount = pLocus->GetReviewCount(pHistory);
        bEdited = pLocus->HasBeenEdited(
          pMsgs,pSample,nChannel,pHistory);
        _SetupCellFlag(flagBase,
          bSampleDisabled,bEdited,bAlert,
          nAcceptCount,nAcceptNeeded,
          nReviewCount,nReviewNeeded,
          &flag,&bNeedsAttention);
        if(sCell.IsEmpty())
        {
          if(bAlert)
          {
            sCell = "?";
          }
          else if(bEdited)
          {
            sCell = COARsample::g_sCellChannelEdited;
          }
        }
        SetCellValue(iRow,nCol,sCell);
        SetCellStyle(iRow,nCol,flag);
      }
      else
      {
        SetCellValue(iRow,nCol,wxEmptyString);
        SetCellStyle(iRow,nCol,flagBase);
      }
    }
    // setup control cell

    SetReadOnly(iRow,nCol,true);
    SetCellValue(iRow,nCol,pSample->GetPositiveControl());
    SetCellStyle(iRow,nCol,flagBase);

    // setup status cell
    SetReadOnly(iRow,CFrameAnalysis::STATUS_COLUMN,true);
    if(bSampleDisabled)
    {
      SetCellValue(iRow,CFrameAnalysis::STATUS_COLUMN,"");
      SetCellStyle(iRow,CFrameAnalysis::STATUS_COLUMN,flagBase);
    }
    else 
    {
      CParmOsirisGlobal parm;
      const wxColour &clFg(parm->GetStatusForeground(bNeedsAttention));
      const wxColour &clBg(parm->GetStatusBackground(bNeedsAttention));
      const wxString &s(
        bNeedsAttention ? g_sStatusNeedsAttention : g_sStatusOK);
      SetCellValue(iRow,CFrameAnalysis::STATUS_COLUMN,s);
      SetCellFont(iRow,CFrameAnalysis::STATUS_COLUMN,g_fontStatus);
      SetCellBackgroundColour(iRow,CFrameAnalysis::STATUS_COLUMN,clBg);
      SetCellTextColour(iRow,CFrameAnalysis::STATUS_COLUMN,clFg);
      SetCellAlignment(
        iRow,CFrameAnalysis::STATUS_COLUMN,
        wxALIGN_CENTRE,wxALIGN_CENTRE);
      SetCellAlignment(
        iRow,CFrameAnalysis::ILS_COLUMN,
        wxALIGN_CENTRE,wxALIGN_CENTRE);
    }
  }
  AutoSize();
  UpdateLabelSize();
}

bool CGridAnalysis::TransferDataToGrid(
  COARfile *pFile, 
  COARsampleSort *pSort, 
  int nLabelType, 
  int nLabelTypeName, 
  wxDC *pdc)
{

  int nAMEL = -1;
  bool bError = false;

  // set up row/col count
  size_t nRowCount = pSort->GetCount();
  size_t nAlleleColCount = pFile->GetLocusCount();
  size_t nColCount = nAlleleColCount + CFrameAnalysis::FIRST_LOCUS_COLUMN + 1;
  DCholder xxx(this,pdc);
  m_nLabelSize = 0;
  m_setColChannelChange.clear();
  nwxGridBatch xBatch(this);

  bError = (!nRowCount) || (!nAlleleColCount) ||
    (!_SetGridSize((int) nRowCount,(int)nColCount));
  if(!bError)
  {
    // set up column headers
    wxString sLabel;
    wxString sLocus;
    wxString sChannel;
    SetColLabelValue(CFrameAnalysis::STATUS_COLUMN,"");
    SetColLabelValue(CFrameAnalysis::ILS_COLUMN,"ILS");
    SetColLabelValue(CFrameAnalysis::CHANNEL_ALERT_COLUMN,"Channels");
    int nPrevChannel = 0;
    int nChannel;
    int nCol;
    size_t i;
    const COARdirectoryAlerts *pDirAlerts = pFile->GetDirectoryAlerts();

    // set up column labels

    for(i = 0, nCol = CFrameAnalysis::FIRST_LOCUS_COLUMN; i < nAlleleColCount; ++i,++nCol)
    {
      sLocus = pFile->GetLocusName(i);
      nChannel = pFile->GetChannelNr(i);
      sLabel = wxString::Format("%ls-%d",sLocus.wc_str(),nChannel);
      if(pDirAlerts->GetBaseLocusAlertsByLocus(sLocus) != NULL)
      {
        sLabel.Append(" *");
      }
      if(nChannel != nPrevChannel)
      {
        m_setColChannelChange.insert(nCol - 1);
        nPrevChannel = nChannel;
      }
      SetColLabelValue(nCol,sLabel);
      if(!strncmp(sLabel.MakeUpper().utf8_str(),"AMEL",4))
      {
        nAMEL = nCol;
      }
    }
    nCol = (int)nColCount - 1;
    SetColLabelValue(nCol,"+Ctrl");
    m_setColChannelChange.insert(nCol - 1);
    m_setColChannelChange.insert(nCol);

    UpdateGrid(pFile,pSort,nLabelType,nLabelTypeName);
    UpdateLabelSize();
    SetGridCursor(0,CFrameAnalysis::ILS_COLUMN);
    SetColLabelSize(GetRowSize(0) + 2);
  }
  return !bError;
}


int CGridAnalysis::GetChannelNumber(const wxString &s)
{
  int nRtn = 0;
  int nFind = s.Find(wxChar('-'),true);
  if(nFind != wxNOT_FOUND)
  {
    nFind++;
    nRtn = atoi(((const char *)s.utf8_str()) + nFind);
  }
  return nRtn;
}
wxPen CGridAnalysis::GetDefaultGridLinePen()
{
  return g_LIGHT_GREY;
}
wxPen CGridAnalysis::GetColGridLinePen(int nCol)
{
  if(m_setColChannelChange.find(nCol) == m_setColChannelChange.end())
  {
    return GetDefaultGridLinePen();
  }
  return *wxBLACK_PEN;
}
wxPen CGridAnalysis::GetRowGridLinePen(int nRow)
{
  nRow++;
  if(nRow == GetNumberRows())
  {
    return *wxBLACK_PEN;
  }
  return GetDefaultGridLinePen();
}


void CGridAnalysis::OnKey(wxKeyEvent &e)
{
  int n = e.GetKeyCode();
  if(n == KEY_CODE_COPY)
  {
    nwxGrid::CopyToClipboard(this,true);
  }
  else
  {
    e.Skip();
  }
}

BEGIN_EVENT_TABLE(CGridAnalysis, wxGrid)
EVT_CHAR(CGridAnalysis::OnKey)
END_EVENT_TABLE()
