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
*  FileName: CPanelLabNameStrings.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_NAME_STRINGS_H__
#define __C_PANEL_LAB_NAME_STRINGS_H__

#include <wx/choicebk.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "CPanelLabSettingsUtils.h"


//********************************************************************
//
//    CPanelLabNameStrings
//
class CPanelLabNameStrings : public nwxPanel
{
public:
  CPanelLabNameStrings(wxWindow *parent);
  virtual ~CPanelLabNameStrings();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetData(CLabNameStrings *pData)
  {
    m_pData = pData;
  }
  void OnChangeCombo(wxChoicebookEvent &);
  void OnChangeRadio(wxCommandEvent &);
  virtual void SetReadOnly(bool b = true)
  {
    m_pChoiceBook->SetReadOnly(b);
    if(m_pTextStdCtrlName != NULL)
    {
      m_pTextStdCtrlName->SetEditable(!b);
    }
    m_pRadioNameType->Enable(!b);
  }
  void SyncPrompt();

private:
  wxRadioBox *m_pRadioNameType;
  nwxChoicebook *m_pChoiceBook;
  wxStaticText *m_pLabelStdCtrlName;
  wxStaticText *m_pLabelNameString;

  //  file or sample name search strings that are NOT used for CODIS CMF
  wxTextCtrl *m_pTextLadder;
  wxTextCtrl *m_pTextPos;
  wxTextCtrl *m_pTextNeg;
  wxTextCtrl *m_pTextPossibleMixture;
  wxTextCtrl *m_pTextSingleSource;

  wxTextCtrl *m_pTextStdCtrlName;
  vector<wxTextCtrl *> m_pvTextSpecimenTypes;
  CLabNameStrings *m_pData;
  wxBoxSizer *m_pSizer;
  int m_nPosCtrlNdx;
  DECLARE_EVENT_TABLE()
};



#endif
