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
*  FileName: CMenuLabels.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CMenuLabels.h"
#include "CComboLabels.h"
#include "wxIDS.h"
#include "LABEL_TYPES.h"



//*******************************************************************
//
//                                           CMenuLabels
//

CMenuLabels::CMenuLabels(MENU_LABEL_TYPE nType, int nPlotNrOrOffset) : 
  wxMenu(""), m_nBase(IDmenuDisplayBEGIN), m_nType(nType)
{
  wxArrayString asChoices;

  switch(m_nType)
  {
  case MENU_TYPE_PREVIEW:
  case MENU_TYPE_PLOT:
    if(nPlotNrOrOffset < IDmenuPlotBase_0)
    {
      m_nBase = ID_GET_PLOT_BASE(nPlotNrOrOffset) + IDmenuPlotLabels_BEGIN;
    }
    else
    {
      // plot number is actually the menu offset
      m_nBase = nPlotNrOrOffset + IDmenuPlotLabels_BEGIN;
    }
    break;
  case MENU_TYPE_TABLE:
    break;
  default:
    {
      wxString sErr;
      sErr.Printf("CMenuLabels::CMenuLabels, invalid menu type %d",(int)nType);
      wxASSERT_MSG(0,sErr);
    }
  }

  if( (m_nType != MENU_TYPE_TABLE) && 
      ( (m_nBase < IDmenuPlotBase_0) || 
        (m_nBase > IDmenuPlotBase_MAX) )
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
    CComboLabels::GetChoices(&asChoices,&m_anChoices,(m_nType == MENU_TYPE_PLOT), true, m_nBase);
    size_t nSize = asChoices.GetCount();
    size_t i;
    if(m_nType == MENU_TYPE_PLOT)
    {
      int nItem;
      for(i = 0; i < nSize; ++i)
      {
        nItem = m_anChoices.Item(i) - m_nBase;
        if(nItem == IDmenuDisplayNone)
        {
          pMenuItem = Append(m_anChoices.Item(i),asChoices.Item(i));
        }
        else
        {
          pMenuItem = AppendCheckItem(m_anChoices.Item(i), asChoices.Item(i));
        }
      }
    }
    else
    {
      for(i = 0; i < nSize; ++i)
      {
        pMenuItem = AppendRadioItem(m_anChoices.Item(i), asChoices.Item(i));
      }
    }
    m_pMenuArea = pMenuItem; // last item
    m_pMenuArea->Enable(m_nType == MENU_TYPE_TABLE);
    if(m_nType != MENU_TYPE_PLOT)
    {
      Check(g_nDefault + m_nBase,true); 
    }
      // must be equivalent to
      // the default selection of CComboBox 
      // in its constructor
    if(m_nType == MENU_TYPE_TABLE)
    {
      AppendSeparator();
      CComboLabelsName::GetChoices(&asChoices,&m_anChoicesName,IDmenuDisplayNameBASE);
      nSize = asChoices.GetCount();
      for(i = 0; i < nSize; i++)
      {
        AppendRadioItem(m_anChoicesName.Item(i),asChoices.Item(i));
      }
    }
  }
}

CMenuLabels::~CMenuLabels() {;}

bool CMenuLabels::SelectByType(int nType, bool bChecked)
{
  bool bRtn = false;
  wxMenuItem *pItem = FindItem(nType + m_nBase);
  if(pItem == NULL)
  {}
  else if( pItem->IsCheckable() )
  {
    bRtn = true;
    if(pItem->IsChecked() != bChecked)
    {
      pItem->Check(bChecked);
    }
  }
  else if(nType == LABEL_NONE)
  {
    Clear();
  }
  return bRtn;
}

size_t CMenuLabels::GetSelectionTypes(vector<unsigned int> *pan)
{
  size_t nCount = GetMenuItemCount();
  size_t n;
  unsigned int nBase = m_nBase;
  unsigned int nItem;
  wxMenuItem *pItem;
  pan->clear();
  pan->reserve(nCount);
  for(n = 0; n < nCount; n++)
  {
    pItem = FindItemByPosition(n);
    if( (pItem != NULL) && pItem->IsCheckable() && pItem->IsChecked() )
    {
      nItem = (unsigned int) pItem->GetId() - nBase;
      pan->push_back(nItem);
    }
  }
  n = pan->size();
  return n;
}
void CMenuLabels::Clear()
{
  if(this->m_nType == MENU_TYPE_PLOT)
  {
    size_t nCount = GetMenuItemCount();
    size_t n;
    wxMenuItem *pItem;
    for(n = 0; n < nCount; n++)
    {
      pItem = FindItemByPosition(n);
      if( (pItem != NULL) && pItem->IsCheckable() && pItem->IsChecked() )
      {
        pItem->Check(false);
      }
    }
  }
}
bool CMenuLabels::SelectByOffset(int nType, bool bName)
{
  bool bRtn = false;
  const nwxArrayInt &anChoices = GetArray(bName);
  if(m_nType == MENU_TYPE_PLOT)
  {
    wxASSERT_MSG(0,wxS("CMenuLabels::SelectByComboBox() called for plot"));
  }

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
void CMenuLabels::SelectByMenu(CMenuLabels *pMenuFrom)
{
  size_t nCount = pMenuFrom->GetMenuItemCount();
  wxMenuItem *pItemFrom;
  size_t nBaseFrom = pMenuFrom->GetBase();
  for(size_t i = 0; i < nCount; i++)
  {
    pItemFrom = pMenuFrom->FindItemByPosition(i);
    if(pItemFrom->IsCheckable())
    {
      Check(pItemFrom->GetId() - nBaseFrom + m_nBase,pItemFrom->IsChecked());
    }
  }
}

void CMenuLabels::SelectByComboBox(wxComboBox *pCombo,bool bName)
{
  SelectByOffset(pCombo->GetCurrentSelection(),bName);
}
