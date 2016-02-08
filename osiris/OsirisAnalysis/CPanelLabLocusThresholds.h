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
*  FileName: CPanelLabLocusThresholds.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_LOCUS_THRESHOLDS_H__
#define __C_PANEL_LAB_LOCUS_THRESHOLDS_H__

#include "CPanelLabSettingsUtils.h"
#include "CGridLabThresholds.h"
#include "CGridRFULimits.h"
#include <wx/checkbox.h>


//********************************************************************
//
//    CPanelLabLocusThresholds (including CChoiceHomozygote)
//
class CChoiceHomozygote : public wxChoice
{
public:
  CChoiceHomozygote(wxWindow *parent, wxWindowID id);
  virtual ~CChoiceHomozygote();
  void SetSelectionByValue(wxChar x);
  wxChar GetSelectionByValue();
  static const wxChar VALUE_RFU;
  static const wxChar VALUE_PCT;
};

class CPanelLabLocusThresholds: public nwxPanel
{
public:
  CPanelLabLocusThresholds(wxWindow *parent, wxWindowID id);
  virtual ~CPanelLabLocusThresholds();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true);
  bool SetILSDyeName(const wxString &sDyeName)
  {
    return m_pGridLadder->SetILSDyeName(sDyeName);
  }
  bool SetData(CLabThresholds *pData,const wxString &sKitName)
  {
    m_pData = pData;
    bool b1 = m_pGridSample->SetData(pData,sKitName);
    bool b2 = m_pGridLadder->SetData(pData,sKitName);
    m_pGridRFU->SetData(pData,sKitName);
    return b1 && b2;
  }
  
private:
  CLabThresholds *m_pData;
  CGridRFULimits *m_pGridRFU;
  wxCheckBox *m_pAllowOverride;
  CGridLabThresholdsSample *m_pGridSample;
  CGridLabThresholdsLadder *m_pGridLadder;
  CChoiceHomozygote *m_pChoiceHomozygoteUnits;
};

#endif
