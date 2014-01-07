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

*  FileName: CComboLabels.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_COMBO_LABELS_H__
#define __C_COMBO_LABELS_H__

#include <wx/window.h>
#include <wx/combobox.h>
#include <wx/menu.h>
#include "nwx/nwxArrayInt.h"

class CMenuLabels;

//  class for creating a combo box for labels,
//  must be the same labels as CMenuLabels below
class CComboLabels : public wxComboBox
{
public:
  CComboLabels(wxWindow *parent, wxWindowID id, bool bPlot = false);
  virtual ~CComboLabels();
  static void GetChoices(
    wxArrayString *pas,
    nwxArrayInt *panID,
    bool bPlot,
    bool bAllowPeakArea,
    int nOffset);
  void SelectByMenu(CMenuLabels *pMenu);
  void EnablePeakAreaLabel(bool b);
  bool PeakAreaLabelEnabled();
  static void CheckSetSelection(wxComboBox *p,int n);
  void CheckSetSelection(int n)
  {
    CheckSetSelection(this,n);
  }
  CComboLabels *Clone();
private:
  static const wxString g_sAlleles;
  static const wxString g_sPeakArea;
  bool m_bPlot;
};

class CComboLabelsName : public wxComboBox
{
public:
  CComboLabelsName(wxWindow *parent, wxWindowID id = wxID_ANY);
  virtual ~CComboLabelsName();
  static void GetChoices(
    wxArrayString *pas,
    nwxArrayInt *panID = NULL,
    int nOffset = 0);
  void SelectByMenu(CMenuLabels *pMenu);
  void CheckSetSelection(int n)
  {
    CComboLabels::CheckSetSelection(this,n);
  }
};

//     class for creating a sub-menu for labels,
//     must be the same labels as CComboLabels

class CMenuLabels : public wxMenu
{
public:
  CMenuLabels(bool bPlot = false, int nPlotNrOrOffset = 0);
  virtual ~CMenuLabels();
  bool SelectByOffset(int nType, bool bName = false);
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
  void SelectByComboBox(wxComboBox *pCombo,bool bName = false)
  {
    SelectByOffset(pCombo->GetCurrentSelection(),bName);
  }
  int GetCount()
  {
    return (int)m_anChoices.GetCount();
  }

private:
  const nwxArrayInt &GetArray(bool bName = false)
  {
    return bName ? m_anChoicesName : m_anChoices;
  }
  static const int g_nDefault;
  wxMenuItem *m_pMenuArea;
  int m_nBase;
  bool m_bPlot;
  nwxArrayInt m_anChoices;
  nwxArrayInt m_anChoicesName;
};


#endif
