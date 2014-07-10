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
*  FileName: CMenuAnalysis.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_MENU_ANALYSIS_H__
#define __C_MENU_ANALYSIS_H__

#include <wx/menu.h>
#include "CMenuEdit.h" // contained IMenuEdit
#include "Platform.h"

class CMenuLabels;
class CMenuAccept;
class CMenuReview;
class CMenuHistory;
class CComboLabels;
class CComboLabelsName;
class CMenuSort;
class COARfile;
class CFrameAnalysis;

class CMenuAnalysis : public CBaseMenuEdit
{
public:
  CMenuAnalysis(COARfile *pFile);
  virtual ~CMenuAnalysis();
  CMenuAnalysis *Clone();
  void EnablePreview(bool bEnable);
  void SetPreviewTextShow(bool bShow);
  wxMenu *GetMenuSortPopup();
  CMenuHistory *GetMenuHistoryPopup();
  wxMenu *GetPopup();
  wxMenu *GetMenuEdit();
  void UpdateChildren();

  void EnableHistoryView(bool b);
  void UpdateHistory(bool bForce = false);
  bool SelectTime(const wxDateTime *pTime);
  void SetHistoryCellLabel(const wxString &s);
  void SetControlsOnTop(bool b);
  int GetCellTypeOffset();
  int GetNameTypeOffset();
  int GetCellTypeOffsetById(int nID);
  int GetNameTypeOffsetById(int nID);

  virtual void EnableAccept(
    bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir);
  virtual void EnableReview(
    bool bLocus, bool bSample, bool bILS, bool bChannels, bool bDir);
  virtual void SetSampleEnabled(bool bEnable,bool bAllowDisable = true);
  virtual void SetLocus(const wxString &s, bool bEnabled = true);
  virtual void EnableEditLocus(bool bEnable);
  virtual void EnableEditDirectory(bool bEnable);

  bool SetCellTypeOffset(int n);
  bool SetNameTypeOffset(int n);
  void SetNameTypeFromId(int n);
  void SetCellTypeFromComboBox(CComboLabels *);
  void SetNameTypeFromComboBox(CComboLabelsName *);
  //void SetCellTypeToComboBox(CComboLabels *);
  static const wxChar *PREVIEW_HELP_TEXT;
  static const wxChar *PARAMETERS_TOOL_TIP;

private:
  void CopyState(CMenuAnalysis *pTo);

#if !REUSE_MENUS
  wxMenu *_GetPopup();
  CMenuHistory *_GetHistoryPanelPopup();
  wxMenu *_GetMenuSortPopup();
  void _UpdateHistoryButton();
  void _UpdatePopup();

  CMenuAnalysis *m_pMenuPopup;
  CMenuHistory *m_pMenuHistoryButton;
  CMenuSort *m_pMenuSortPopup;
#endif
  CMenuEdit *m_pMenuEdit;
  CMenuHistory *m_pMenuHistory;
  CMenuSort *m_pMenuSort;
  CMenuLabels *m_pMenuCellType;
  wxMenuItem *m_pMenuItemToolbar;
  wxMenuItem *m_pMenuItemPreview;
  wxMenuItem *m_pMenuItemDisable;
  wxMenuItem *m_pMenuItemDisableMulti;
  wxMenuItem *m_pMenuItemReAnalyze;
  COARfile *m_pFile;

};

#endif
