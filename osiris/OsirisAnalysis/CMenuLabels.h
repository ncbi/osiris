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
*  FileName: CMenuLabels.h
*  Author:   Douglas Hoffman
*
*  Purpose: Let the programmer know that the CMenuLabels class is in
*    CComboLabels.cpp/h
*
*/

#include <wx/menu.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxArrayInt.h"

class CMenuLabels;
class wxComboBox;

//     class for creating a sub-menu for labels,
//     must be the same labels as CComboLabels

class CMenuLabels : public wxMenu
{
public:
  typedef enum
  {
    MENU_TYPE_TABLE,
    MENU_TYPE_PREVIEW,
    MENU_TYPE_PLOT
  }
  MENU_LABEL_TYPE;

  CMenuLabels(MENU_LABEL_TYPE nType = MENU_TYPE_TABLE, int nPlotNrOrOffset = 0);
  virtual ~CMenuLabels();
  bool SelectByOffset(int nType, bool bName = false);
  bool SelectByType(int nType, bool bCheck = true);
  size_t GetSelectionTypes(vector<unsigned int> *pan);
  void SetSelectionTypes(const vector<unsigned int> &an)
  {
    vector<unsigned int>::const_iterator itr;
    Clear();
    for(itr = an.begin(); itr != an.end(); ++itr)
    {
      SelectByType(*itr,true);
    }
  }
  void Clear();
  int GetCheckedOffset(bool bName = false);
  int GetOffsetById(int nID,bool bName = false);
  void EnablePeakAreaLabel(bool b)
  {
    m_pMenuArea->Enable(b);
  }
  bool PeakAreaLabelEnabled()
  {
    return m_pMenuArea->IsEnabled();
  }
  void SelectByMenu(CMenuLabels *pMenuFrom);
  void SelectByComboBox(wxComboBox *pCombo,bool bName = false);
  int GetCount()
  {
    return (int)m_anChoices.GetCount();
  }

private:
  const nwxArrayInt &GetArray(bool bName = false)
  {
    return bName ? m_anChoicesName : m_anChoices;
  }
  void SetBase(size_t n)
  {
    m_nBase = n;
  }
  size_t GetBase() const
  {
    return m_nBase;
  }
  static const int g_nDefault;
  wxMenuItem *m_pMenuArea;
  size_t m_nBase;
  MENU_LABEL_TYPE m_nType;
  nwxArrayInt m_anChoices;
  nwxArrayInt m_anChoicesName;
};
