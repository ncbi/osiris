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
*  FileName: CPanelLabGeneral.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_GENERAL_H__
#define __C_PANEL_LAB_GENERAL_H__

#include "CPanelLabSettingsUtils.h"
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/gbsizer.h>

//********************************************************************
//
//    CPanelLabGeneral
//
class CPanelLabGeneral : public nwxPanel
{
public:
  CPanelLabGeneral(wxWindow *parent);
  virtual ~CPanelLabGeneral();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true);
  void SetData(
    CLabSettingsInfo *pInfo, 
    CLabMarkerSetCollection *pMarker)
  {
    m_pInfo = pInfo;
    m_pMarker = pMarker;
    m_bTransferred = false;
  }
  wxString GetILS()
  {
    wxString s;
    if(m_bTransferred)
    {
      s = m_pChoiceILS->GetStringSelection();
    }
    else if(m_pMarker != NULL)
    {
      s = m_pMarker->GetILSName();
    }
    return s;
  }
private:
  void _AddLine(
    const wxChar *psLabel,wxObject *pobj,
    wxGridBagSizer *pSizer,
    wxGBPosition *pPos
  );

  CLabSettingsInfo *m_pInfo;
  CLabMarkerSetCollection *m_pMarker;

  wxStaticText *m_pLabelVolumeName;
  wxStaticText *m_pLabelKitName;
  wxStaticText *m_pLabelLastUpdateUser;
  wxChoice *m_pChoiceILS;
  wxCheckBox *m_pCheckUserOverride;

  wxRadioButton *m_pRadioRaw;
  wxRadioButton *m_pRadioAnalyzed;
  vector<wxRadioButton *> m_vpRadioFileType;

  wxTextCtrl *m_pTextProtocol;
  wxTextCtrl *m_pTextLot;
  wxTextCtrl *m_pTextNotes;
  bool m_bTransferred; // set to true after TransferDataToWindow
                       // set to false after SetData
                       // determines how to get selected ILS name
};

#endif
