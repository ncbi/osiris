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

*  FileName: CPanelLabSettings.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_SETTINGS_H__
#define __C_PANEL_LAB_SETTINGS_H__

#include "CPanelLabSampleThresholds.h"
#include "CPanelLabLocusThresholds.h"
#include "CPanelLabReviewAcceptance.h"
#include "CPanelLabSettingsUtils.h"
#include "CPanelLabMarkerSet.h"
#include "CPanelLabGeneral.h"
#include "CPanelLabNameStrings.h"
class wxButton;
class wxNotebook;
class CVolume;


//********************************************************************
//
//    CPanelLabSettings
//
class CPanelLabSettings : public nwxPanel
{
public:
  CPanelLabSettings(wxWindow *parent, 
    bool bAlwaysReadOnly = false,
    bool bShowCancel = true);
  virtual ~CPanelLabSettings();
  static const wxSize g_SIZE_NUMBER;
  static const wxSize g_SIZE_MULTILINE_TEXT;
  static const wxString g_sFileNameStrPrompt;
  static const wxString g_sSampleNameStrPrompt;

  static const wxSize &GetDefaultFrameSize();
  static const wxString &NameStrPrompt(bool bSample)
  {
    return (bSample ? g_sSampleNameStrPrompt : g_sFileNameStrPrompt);
  }
  static wxTextCtrl *CreateTextCtrl(wxWindow *parent, const wxValidator &v = wxDefaultValidator)
  {
    return new wxTextCtrl(
      parent,wxID_ANY,wxEmptyString,wxDefaultPosition,
      CPanelLabSettings::g_SIZE_NUMBER,
      wxTE_LEFT,v);
  }
  static double Str2Double(const wxString &_s, double dDefault = -1.0);
  static int Str2Int(const wxString &_s, int nDefault = -1);
  static wxString Number2Str(double d, double dDefaultBlank = -1.0);
  static wxString Number2Str(int n, int nDefaultBlank = -1);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();


  bool UpdateAfterSave()
  {
    return m_pGeneral->TransferDataToWindow();
  }
  virtual void SetReadOnly(bool b = true);
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
  void OnPageChange(wxNotebookEvent &);
  void OnNext(wxCommandEvent &);
  void OnBack(wxCommandEvent &);
  bool SetData(
    CLabSettings *pData, 
    const CXMLmessageBook *pBook,
    bool bReadOnly = false);
  bool SetupAll(CVolume *pVolume);
  bool UpdateILSDyeName()
  {
    return m_pLocusThresholds->SetILSDyeName(_GetILSDyeName());
  }

private:
  static wxSize g_SIZE_FRAME;
  wxString _GetILSDyeName();
  CLabSettings *m_pData;
  const CXMLmessageBook *m_pMessageBook;
  wxButton *m_pbBack;
  wxButton *m_pbNext;
  wxButton *m_pbLock;
  wxButton *m_pbApply;
  wxNotebook *m_pNotebook;

  CPanelLabGeneral *m_pGeneral;                    //  label - General
  CPanelLabSampleThresholds *m_pSampleThresholds;  //    Sample Limits
  CPanelLabLocusThresholds *m_pLocusThresholds;    //    Thresholds
  CPanelLabNameStrings *m_pFilenames;              //    File/Sample Names
  CPanelLabMarkerSet *m_pMarkerSet;                //    Assignments
  CPanelLabReviewAcceptance *m_pReview;            //    Accept/Review
  size_t m_nLocusThresholds; // panel index for CPanelLabLocusThresholds
  bool m_bReadOnly;
  bool m_bAlwaysReadOnly;
//  const set<wxString> *m_pSetKits;
  DECLARE_EVENT_TABLE()
};
#endif
