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

*  FileName: CComboLabels.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CComboLabels.h"
#include "CPanelPlotToolbar.h"
#include <wx/arrstr.h>
#include "wxIDS.h"
#include "Platform.h"

//  static constants for default choices, must be in sync
// default selection for combo box
const wxString CComboLabels::g_sAlleles("Alleles");
const wxString CComboLabels::g_sPeakArea("Peak Area");
// default selection for menu
const int CMenuLabels::g_nDefault(IDmenuDisplayAlleles);

//*******************************************************************
//
//                                           CComboLabels
//


void CComboLabels::GetChoices(
  wxArrayString *pas,  // array of display strings
  nwxArrayInt *panID,  // array of menu IDs
  bool bPlot,
  bool bAllowPeakArea,
  int nBase)
{

  // the order of pas must be consistent with
  // enum LABEL_PLOT_TYPE and LABEL_CELL_TYPE 
  // in LABEL_TYPES.h

  size_t nAlloc = 5;
  if(pas != NULL)
  {
    pas->Alloc(nAlloc);
    if(bPlot)
    {
      pas->Add("None");
    }
    pas->Add(g_sAlleles);
    pas->Add("BPS");
    pas->Add("RFU");
    pas->Add("Time");
    if(bAllowPeakArea || !bPlot)
    {
      pas->Add(g_sPeakArea);
    }
  }
  if(panID != NULL)
  {
    panID->Alloc(nAlloc);
    if(bPlot)
    {
      panID->Add(IDmenuDisplayNone + nBase);
    }
    panID->Add(IDmenuDisplayAlleles + nBase);
    panID->Add(IDmenuDisplayBPS + nBase);
    panID->Add(IDmenuDisplayRFU + nBase);
    panID->Add(IDmenuDisplayTime + nBase);
    if(bAllowPeakArea || !bPlot)
    {
      panID->Add(IDmenuDisplayPeakArea + nBase);
    }
  }
}


CComboLabels::CComboLabels(wxWindow *parent, wxWindowID id, bool bPlot) 
    : wxComboBox(), m_bPlot(bPlot)
{
  wxArrayString asChoices;
  GetChoices(&asChoices,NULL,bPlot, true, 0);

  if(!Create(parent,id,g_sAlleles,wxDefaultPosition,wxDefaultSize,
    asChoices,wxCB_DROPDOWN | wxCB_READONLY))
  {
    ; // error
  }
  else if(bPlot)
  {
    SetToolTip("Select type of label to display." PLOT_TOOLBAR_SHIFT_ALL);
  }
  else
  {
    SetToolTip("Select information to display in table");
  }
}
CComboLabels::~CComboLabels() {;}
void CComboLabels::CheckSetSelection(wxComboBox *p,int n)
{
  if( (n >= 0) && 
      (n < (int)p->GetCount()) && 
      (n != p->GetSelection())
    )
  {
    // check if already selected to 
    // prevent unnecessary events
    // -- according to wx documentation
    //  this should not trigger an event 
    //  but still checking anyway
    p->SetSelection(n);
  }
}
void CComboLabels::SelectByMenu(CMenuLabels *pMenu)
{
  int n = pMenu->GetCheckedOffset();
  CheckSetSelection(n);
}
bool CComboLabels::PeakAreaLabelEnabled()
{
  int n = (int)GetCount();
  bool bRtn = (n == (int)LABEL_PLOT_TYPE_COUNT);
  return bRtn;
}
void CComboLabels::EnablePeakAreaLabel(bool b)
{
  bool bCurrent = PeakAreaLabelEnabled();
  if(b != bCurrent)
  {
    if(b)
    {
      Append(g_sPeakArea);
    }
    else
    {
      Delete(GetCount() - 1);
    }
  }
}
CComboLabels *CComboLabels::Clone()
{
  CComboLabels *pNew = new CComboLabels(GetParent(),GetId(),m_bPlot);
  pNew->Select(GetCurrentSelection());
  return pNew;
}
//*******************************************************************
//
//                                           CComboLabelsName
//
void CComboLabelsName::GetChoices(
  wxArrayString *pas, nwxArrayInt *panID, int nOffset)
{
  pas->Empty();
  pas->Alloc(2);
  // names must be in numeric order by IDs
  // because the selection offset is used to determine value
  pas->Add("File name");
  pas->Add("Sample name");
  if(panID != NULL)
  {
    panID->Empty();
    panID->Alloc(2);
    panID->Add(IDmenuDisplayNameFile + nOffset);
    panID->Add(IDmenuDisplayNameSample + nOffset);
  }
}
CComboLabelsName::~CComboLabelsName() {;}
CComboLabelsName::CComboLabelsName(wxWindow *parent, wxWindowID id) 
    : wxComboBox()
{
  wxArrayString asChoices;
  GetChoices(&asChoices);

  if(!Create(parent,id,asChoices.Item(0),
    wxDefaultPosition,wxDefaultSize,
    asChoices,wxCB_DROPDOWN | wxCB_READONLY))
  {
    ; // error
  }
  else 
  {
    SetToolTip(
      "Select file names or sample names\n"
      "for the row labels in the table below");
  }
}
void CComboLabelsName::SelectByMenu(CMenuLabels *pMenu)
{
  int n = pMenu->GetCheckedOffset(true);
  CheckSetSelection(n);
}


//*******************************************************************
//
//                                           CMenuLabels
//

CMenuLabels::CMenuLabels(bool bPlot, int nPlotNrOrOffset) : 
  wxMenu(""), m_bPlot(bPlot)
{
  wxArrayString asChoices;
  int nBase = IDmenuDisplayBEGIN;

  if(!bPlot)  
  {
    nBase = IDmenuDisplayBEGIN;
  }
  else if(nPlotNrOrOffset < IDmenuPlotBase_0)
  {
    nBase = ID_GET_PLOT_BASE(nPlotNrOrOffset) + IDmenuPlotLabels_BEGIN;
  }
  else
  {
    // plot number is actually the menu offset
    nBase = nPlotNrOrOffset + IDmenuPlotLabels_BEGIN;
  }

  if( (bPlot) && 
      ( (nBase < IDmenuPlotBase_0) || 
        (nBase > IDmenuPlotBase_MAX) )
    )
  {
    wxString sErr;
    sErr.Printf(
      "CMenuLabels::CMenuLabels, plot number, %d, is out of range",
      nPlotNrOrOffset);
    wxASSERT_MSG(0,sErr);
  }
  else
  {
    wxMenuItem *pMenuItem(NULL);
    CComboLabels::GetChoices(&asChoices,&m_anChoices,bPlot, true, nBase);
    size_t nSize = asChoices.GetCount();
    size_t i;
    for(i = 0; i < nSize; ++i)
    {
      pMenuItem = AppendRadioItem(m_anChoices.Item(i), asChoices.Item(i));
    }
    m_pMenuArea = pMenuItem; // last item
    m_pMenuArea->Enable(!bPlot);
    Check(g_nDefault + nBase,true); 
      // must be equivalent to
      // the default selection of CComboBox 
      // in its constructor
    if(!m_bPlot)
    {
      AppendSeparator();
      CComboLabelsName::GetChoices(&asChoices,&m_anChoicesName,IDmenuDisplayNameBASE);
      size_t nSize = asChoices.GetCount();
      for(i = 0; i < nSize; i++)
      {
        AppendRadioItem(m_anChoicesName.Item(i),asChoices.Item(i));
      }
    }
  }
}

CMenuLabels::~CMenuLabels() {;}

bool CMenuLabels::SelectByOffset(int nType, bool bName)
{
  bool bRtn = false;
  const nwxArrayInt &anChoices = GetArray(bName);
  if( (nType >= 0) && (nType < (int)anChoices.GetCount()) )
  {
    int nSelect = anChoices.Item(nType);
    if(!IsChecked(nSelect))
    {
      // only check if not already checked to prevent
      // unnecessary events
      Check(nSelect,true);
      bRtn = true;
    }
  }
  return bRtn;
}
int CMenuLabels::GetOffsetById(int nID,bool bName)
{
  int nRtn = -1;
  int n;
  const nwxArrayInt &anChoices = GetArray(bName);
  size_t nSize = anChoices.GetCount();
  for(size_t i = 0; i < nSize; ++i)
  {
    n = anChoices.Item(i);
    if(n == nID)
    {
      nRtn = (int)i;
      i = nSize; // loop exit
    }
  }
  return nRtn;
}
int CMenuLabels::GetCheckedOffset(bool bName)
{
  int nRtn = -1;
  int n;
  const nwxArrayInt &anChoices = GetArray(bName);
  size_t nSize = anChoices.GetCount();
  for(size_t i = 0; i < nSize; ++i)
  {
    n = anChoices.Item(i);
    if(IsChecked(n))
    {
      nRtn = (int)i;
      i = nSize; // loop exit
    }
  }
  return nRtn;
}
