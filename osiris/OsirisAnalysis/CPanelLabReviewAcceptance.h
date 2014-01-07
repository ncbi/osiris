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
*  FileName: CPanelLabReviewAcceptance.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_REVIEW_ACCEPTANCE__
#define __C_PANEL_REVIEW_ACCEPTANCE__

#include "CPanelLabSettingsUtils.h"
#include "nwx/nwxGrid.h"
#include <wx/checkbox.h>

//********************************************************************
//
//    CPanelLabReviewAcceptance
//
class CPanelLabReviewAcceptance : public nwxPanel
{
public:
  CPanelLabReviewAcceptance(wxWindow *parent);
  virtual ~CPanelLabReviewAcceptance();

  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true);
  void SetData(CLabReview *pData)
  {
    m_pReview = pData;
  }
private:
  void _SetCellValue(int nRow, int nCol, int nValue);
  int _GetCellValue(int nRow, int nCol);
  static const int ROW_REVIEW;
  static const int ROW_ACCEPT;
  CLabReview *m_pReview;
  wxGrid *m_pGrid;
  wxCheckBox *m_pCheckAllowUserOverride;
  wxCheckBox *m_pCheckExportIfNeedsAttn;
};

#endif
