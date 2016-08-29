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
*  FileName: CDialogEditPeak.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_DIALOG_EDIT_PEAK_H__
#define __C_DIALOG_EDIT_PEAK_H__

#include <wx/dialog.h>
#include "nwx/vectorptr.h"
#include "nwx/nwxPointLabel.h"
#include "COARpeak.h"
#include "COARsample.h"
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
class wxRadioButton;
class wxComboBox;
class wxCheckBox;
class wxStaticText;
class COARfile;
class CFrameAnalysis;

class CDialogEditPeak : public wxDialog
{
public:
  CDialogEditPeak(
    wxWindow *parent,
    const COARsample *pSample,
    const COARpeakAny *pPeak);
  virtual ~CDialogEditPeak();
  virtual bool TransferDataToWindow();
  bool IsOK()
  {
    return m_bOK;
  }
  const COARpeakAny *GetEditedPeak()
  {
    return m_pCurrent;
  }
  bool CanAcceptHere()
  {
    return m_bCanAcceptHere;
  }
  bool UpdateFile(
    COARfile *pFile,
    CFrameAnalysis *pFrame, 
    wxWindow *parent = NULL);
  const wxString GetLocusName();
  bool IsModified();
  static const int ID_ACCEPT;
  static const int ID_EDIT;
private:
  void OnAccept(wxCommandEvent &);
  void OnEdit(wxCommandEvent &);
  void OnWidget(wxCommandEvent &);
  void _Cleanup();
  bool _Do_Accept(
    COARfile *pFile, CFrameAnalysis *pFrame, wxWindow *parent);
  bool _Do_UpdatePeak(
    COARfile *pFile, CFrameAnalysis *pFrame);
  void _EnableOffLadder(bool b = true);
  void _TransferCurrentToOffLadder();
  void _TransferOffLadderToCurrent();
  void _TransferArtifactToCurrent();
  void _UpdateCurrent();
  void _SetupWidgets();
  void _SetupCanAccept();
  void _BuildNoEdit();
  void _BuildAllele();
  void _BuildArtifactLabel();
  size_t _SetupAlleles();
  const wxString &_GetOffLadderValue();

  void _BuildComboLabel();
  size_t _GetSelectedAllele();
  wxString m_sDefaultArtifactLabel;
  wxString m_sOriginalLabel;
  std::vector<wxRadioButton *> m_vpRadioAllele;

  const COARsample *m_pSample; // do not delete 
  const COARpeakAny *m_pPeak;  // do not delete
  std::vector<COARpeakAny *> m_vpAlleles;   // cleanup in destructor
  COARpeakAny *m_pOriginal; // delete in destructor
  COARpeakAny *m_pCurrent;  // delete in destructor
  size_t m_nCurrentAllele;


  // widgets except for m_cpRadioAllele

  wxComboBox *m_pComboLabel;
  wxStaticText *m_pLabelLabel;
  wxCheckBox *m_pCheckEnabled;
  wxCheckBox *m_pCheckCritical;
  wxCheckBox *m_pCheckDontShowAgain;
  wxCheckBox *m_pCheckBoxLadderAccept;
  wxButton *m_pButtonAccept;
  wxButton *m_pButtonEditLocus;

  bool m_bCanAcceptHere;
  bool m_bLocusNeedsAcceptance;
  bool m_bOK;
  enum
  {
    _junk = wxID_HIGHEST,
    m_ID_ALLELE1,
    m_ID_ALLELE2,
    m_ID_ALLELE_DISABLED,
    m_ID_OFF_LADDER_ACCEPTED,
    m_ID_ARTIFACT_ENABLED,
    m_ID_ARTIFACT_CRITICAL,
    m_ID_ACCEPT,
    m_ID_EDIT
  };
  DECLARE_EVENT_TABLE();

};


#endif