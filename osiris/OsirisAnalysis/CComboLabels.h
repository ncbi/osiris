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
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class CMenuLabels;

//  class for creating a combo box for labels,
//  must be the same labels as CMenuLabels below
class CComboLabels : public wxComboBox
{
public:
  CComboLabels(wxWindow *parent, wxWindowID id);
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
  static const wxString g_sBPS;
  static const wxString g_sRFU;
  static const wxString g_sTime;
  static const wxString g_sILS_BPS;
  static const wxString g_sPeakArea;
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



#endif
