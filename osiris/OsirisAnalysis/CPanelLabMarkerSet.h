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
*  FileName: CPanelLabMarkerSet.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_LAB_MARKER_SET_H__
#define __C_PANEL_LAB_MARKER_SET_H__

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

#include "CGridAllele.h"
#include "CGridLocusColumns.h"
#include "CPanelLabSettingsUtils.h"


class nwxChoicebook;

//********************************************************************
//
//    CPanelLabLocusCollection
//
class CPanelLabLocusCollection : public nwxPanel
{
public:
  static const int GRIDROWS;
  CPanelLabLocusCollection(
    wxWindow *parent,
    int nGridRows = CPanelLabLocusCollection::GRIDROWS,
    bool bTriallele = false,
    bool bSkipAmel = false);
  virtual ~CPanelLabLocusCollection();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void SetData(CLabLocusCollection *pData,
    const wxString &sKitName)
  {
    m_pGrid->SetData(pData,sKitName);
  }
  CLabLocusCollection *GetData()
  {
    return m_pGrid->GetData();
  }
  virtual void SetReadOnly(bool b = true)
  {
    CGridLocusColumns::SetReadOnly(m_pGrid,b);
  }
private:
  CGridAllele *m_pGrid;
};

//********************************************************************
//
//    CPanelPosCtrlMarkerSet
//
class CPanelPosCtrlMarkerSet : public nwxPanel
{
public:
  CPanelPosCtrlMarkerSet(wxWindow *parent);
  virtual ~CPanelPosCtrlMarkerSet() {;}
  virtual bool TransferDataFromWindow()
  {
    bool bRtn = m_pGrid->TransferDataFromWindow();
    return bRtn;
  }
  virtual bool TransferDataToWindow()
  {
    bool bRtn = m_pGrid->TransferDataToWindow();
    return bRtn;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pGrid->SetGridReadOnly(b);
  }
  void SetData(CLabPositiveControls *pData,
    const wxString &sKitName)
  {
    m_pGrid->SetData(pData,sKitName);
  }
private:
  CGridAllelePosCtrl *m_pGrid;
};


//********************************************************************
//
//    CPanelLabMarkerSet
//
class CPanelLabMarkerSet : public nwxPanel
{
public:
  CPanelLabMarkerSet(
    wxWindow *parent);
  virtual ~CPanelLabMarkerSet();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  CLabMarkerSetCollection *GetData()
  {
    return m_pData;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pChoiceBook->SetReadOnly(b);
  }
  void SetData(
    CLabMarkerSetCollection *pData)
  {
    const wxString &sKitName(pData->GetMarkerSetName());
    m_pPanelOffLadder->SetData(
      pData->GetOffLadderLoci(),sKitName);
    m_pPanelSampleTrialleles->SetData(
      pData->GetSampleTriallleles(),sKitName);
    m_pPanelCtrlTrialleles->SetData(
      pData->GetPosCtrlTrialleles(),sKitName);
    m_pPanelPosCtrl->SetData(
      pData->GetPositiveControls(),sKitName);
  }
private:
  CLabMarkerSetCollection *m_pData; // <LabMarkerSetCollection>

  nwxChoicebook *m_pChoiceBook;
  CPanelLabLocusCollection *m_pPanelOffLadder;
  CPanelLabLocusCollection *m_pPanelSampleTrialleles;
  CPanelLabLocusCollection *m_pPanelCtrlTrialleles;
  CPanelPosCtrlMarkerSet *m_pPanelPosCtrl;
};

#endif
