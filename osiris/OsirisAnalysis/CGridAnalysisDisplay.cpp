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
*  FileName: CGridAnalysisDisplay.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CGridAnalysisDisplay.h"
#include <wx/event.h>
#include "wxIDS.h"
#include "CParmOsiris.h"


const int CGridAnalysisDisplay::SORT_DISPLAYED_NAME(IDmenuSortDisplayedName - IDmenuSortBASE);
const int CGridAnalysisDisplay::SORT_FILE_NAME(IDmenuSortFileName  - IDmenuSortBASE);
const int CGridAnalysisDisplay::SORT_SAMPLE_NAME(IDmenuSortSampleName - IDmenuSortBASE);
const int CGridAnalysisDisplay::SORT_SEVERITY(IDmenuSortSeverity  - IDmenuSortBASE);
const int CGridAnalysisDisplay::SORT_RUN_TIME(IDmenuSortRunTime - IDmenuSortBASE);




int CGridAnalysisDisplay::GetSortType(bool bForDisplay)
{
  // bForDisplay
  //   true - for sorting a table
  //   false - value from parameter file
  //     (for menu checkbox in a future version)
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  int nSort = pParm->GetTableSortBy();
  if(bForDisplay)
  {
    int nDisplay = pParm->GetTableShowSampleDisplayNames();
    if(nSort == SORT_DISPLAYED_NAME)
    {
      nSort = (nDisplay == IDmenuDisplayNameSample) ? SORT_SAMPLE_NAME : SORT_FILE_NAME;
    }
    else if(
        (nSort == SORT_FILE_NAME) ||
        (nSort == SORT_SAMPLE_NAME) ||
        (nSort == SORT_SEVERITY) ||
        (nSort == SORT_RUN_TIME)
        )
    {}
    else
    {
      nSort = SORT_FILE_NAME;
    }
  }
  return nSort;
}

void CGridAnalysisDisplay::SetSortTypeFromEvent(int nID)
{
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  pParm->SetTableSortBy(nID - IDmenuSortBASE);
}
void CGridAnalysisDisplay::SetDisplayType(int n)
{
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  pParm->SetTableShowSampleDisplayNames(n);
}
void CGridAnalysisDisplay::SetDisplayTypeFromEvent(int nID)
{
  SetDisplayType(nID - IDmenuDisplayNameBASE);
}
int CGridAnalysisDisplay::GetDisplayType()
{
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  return pParm->GetTableShowSampleDisplayNames();
}
bool CGridAnalysisDisplay::GetControlsOnTop()
{
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  return pParm->GetTableControlsAtTop();
}
void CGridAnalysisDisplay::SetControlsOnTop(bool b)
{
  CParmOsiris *pParm = CParmOsiris::GetGlobal();
  return pParm->SetTableControlsAtTop(b);
}
