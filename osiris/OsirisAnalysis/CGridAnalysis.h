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

*  FileName: CGridAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_GRID_ANALYSIS__
#define __C_GRID_ANALYSIS__
#include "CParmGridAttributes.h"
#include <wx/event.h>
#include <wx/dc.h>
#include <wx/grid.h>
#include <wx/pen.h>
#include <wx/colour.h>
#include <wx/font.h>
#include "nwx/nsstd.h"
#include "nwx/stdb.h"
#include <vector>
#include <set>
#include "nwx/stde.h"

class COARsampleSort;
class COARsample;
class COARfile;

//**************************************************** CGridAnalysis
class CGridAnalysis : public wxGrid
{
private:
  class DCholder
  {
  public:
    DCholder(CGridAnalysis *pGrid,wxDC *pDC)
    {
      m_pGrid = pGrid;
      m_pDC = pDC;
      m_pOLD = m_pGrid->m_pDC;
      if(m_pOLD != m_pDC)
      {
        m_pGrid->SetDC(m_pDC);
      }
    }
    ~DCholder()
    {
      if(m_pOLD != m_pDC)
      {
        m_pGrid->SetDC(m_pOLD);
      }
    }
  private:
    wxDC *m_pOLD;
    wxDC *m_pDC;
    CGridAnalysis *m_pGrid;
  };
public:
  friend class DCholder;
  virtual ~CGridAnalysis();
  CGridAnalysis(wxWindow *pParent);
/*
  void SetCellStyle(
    int nRow, int nCol, bool bBold, 
    bool bItal = false, bool bHasAlerts = false);
*/
  static wxString FormatRowLabel(int nRow, int nRowCount,const wxString &sLabel);
  void SetCellStyle(int nRow, int nCol,GRID_FLAG nFlag);
  void SetRowLabel(int nRow, const wxString &sLabel);
  void UpdateLabelSize();
  void ResetLabelSize()
  {
    m_nLabelSize = 0;
  }

  void UpdateGridRowLabels(
    COARsampleSort *pSort, 
    int nLabelTypeName,
    wxDC *pDC);

  int GetCurrentLabelType()
  {
    return m_nLabelType;
  }

  bool TransferDataToGrid(
    COARfile *pFile, COARsampleSort *pSort, 
    int nLabelType, 
    int nLabelTypeName,
    wxDC *pdc);
  virtual wxPen GetRowGridLinePen(int nRow);
  virtual wxPen GetColGridLinePen(int nCol);
  virtual wxPen GetDefaultGridLinePen();
  void UpdateGrid(
    COARfile *pFile, COARsampleSort *pSort, 
    int nLabelType = 0, 
    int nLabelTypeName = 0,
    const wxDateTime *pHistory = NULL, wxDC *pDC = NULL);
  void SaveScrollPosition()
  {
    GetViewStart(&m_nXScroll,&m_nYScroll);
  }
  void RestoreScrollPosition()
  {
    Scroll(m_nXScroll,m_nYScroll);
    int nRow = GetGridCursorRow();
    int nCol = GetGridCursorCol();
    if(nRow >= 0 && nCol >= 0 && !IsVisible(nRow,nCol,true))
    {
      MakeCellVisible(nRow,nCol);
    }
  }

  static const wxString g_sSampleLevelNeedsAttention;
  static const wxString g_sStatusNeedsAttention;
  static const wxString g_sStatusOK;
  static const wxFont &GetFontStatus()
  {
    return g_fontStatus;
  }
private:
  void SetupRowLabel(
    COARsample *pSample,
    int iRow,
    int nLabelTypeName,
    bool bEdited,
    bool bNeedsAttention);

  void _SetupCellFlag(
    GRID_FLAG flagBase,
    bool bSampleDisabled,bool bEdited, bool bAlert,
    int nAcceptCount, int nAcceptNeeded,
    int nReviewCount, int nReviewNeeded,
    GRID_FLAG *pFlag, bool *pbNeedsAttention);
  bool _SetGridSize(int nRowsCount, int nColCount);
  wxDC *GetDC()
  {
    return m_pDC;
  }
  void SetDC(wxDC *pdc)
  {
    m_pDC = pdc;
    if(m_pDC != NULL)
    {
      m_pDC->SetFont(m_fontBold);
    }
  }
  static int GetChannelNumber(const wxString &s);

  vector<COARsample *> m_vpSamples;
  set<int> m_setColChannelChange;


  wxColour m_bgBold;
  wxColour m_bgNormal;
  wxColour m_bgRowHasAlert;
  wxColour m_fgBold;
  wxColour m_fgNormal;
  wxFont m_fontBold;
  wxFont m_fontNormal;
  wxFont m_fontBoldItalic;
  wxFont m_fontItalic;
  wxDC *m_pDC;
  int m_nLabelSize;
  int m_nLabelType;
  int m_nXScroll;
  int m_nYScroll;

  static wxPen g_LIGHT_GREY;
  static wxFont g_fontStatus;

public:
  void OnKey(wxKeyEvent &e);
  DECLARE_EVENT_TABLE();
  DECLARE_ABSTRACT_CLASS(CGridAnalysis)
};

#endif
