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

*  FileName: CDialogLabSettings.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_DIALOG_LAB_SETTINGS_H__
#define __C_DIALOG_LAB_SETTINGS_H__

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/gbsizer.h>
#include <wx/spinctrl.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nwxGrid.h"
#include "nwx/nwxSyncCheckbox.h"
#include "CAllLoci.h"
#include "CXMLmessageBook.h"

#ifdef __WXMSW__

#pragma warning(push)
#pragma warning( disable : 4267 )
#include <wx/choicebk.h>
#pragma warning(pop)

#else

#include <wx/choicebk.h>

#endif

#include <wx/textctrl.h>
#include <wx/sizer.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"

#include "nwx/nsstd.h"
#include "CLabSettings.h"

//********************************************************************
//
//    INameValidator
//
class INameValidator
{
public:
  virtual ~INameValidator(){;}
  virtual bool IsNameUsed(const wxString &s) = 0;
};
//********************************************************************
//
//    ISetReadOnly
//
class ISetReadOnly
{
public:
  virtual void SetReadOnly(bool b = true) = 0;
protected:
  ISetReadOnly() {;}
  virtual ~ISetReadOnly() {;}
};
//********************************************************************
//
//    nwxPanel
//
class nwxPanel : public wxPanel, public ISetReadOnly
{
public:
  nwxPanel(wxWindow *parent, wxWindowID id = wxID_ANY) :
      wxPanel(parent,id) {;}
};

//********************************************************************
//
//    nwxChoicebook
//
class nwxChoicebook : public wxChoicebook, public ISetReadOnly, public INameValidator
{
public:
  nwxChoicebook(wxWindow *parent) : wxChoicebook(
    parent,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    wxCHB_TOP)
  {
    m_pButtonAdd = NULL;
    m_pButtonRemove = NULL;
    m_pButtonRename = NULL;
    m_bReadOnly = false;
  }
  void SetupButtons(const wxString &sItemName = wxEmptyString, bool bRename = false);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true)
  {
    m_bReadOnly = b;
    if(m_pButtonAdd != NULL)
    {
      m_pButtonAdd->Enable(!b);
    }
    if(m_pButtonRemove != NULL)
    {
      m_pButtonRemove->Enable(!b);
    }
    size_t n = GetPageCount();
    for(size_t i = 0; i < n; i++)
    {
      nwxPanel *pr = (nwxPanel *)GetPage(i);
      pr->SetReadOnly(b);
    }
  }
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
  bool InsertPageSorted(nwxPanel *page,const wxString &text, bool select = false, int imageId = -1)
  {
    size_t n = FindInsertPosition(text);
    page->SetReadOnly(m_bReadOnly);
    bool bRtn = InsertPage(n,page,text,select,imageId);
    return bRtn;
  }
  virtual bool IsNameUsed(const wxString &sName);
  bool RenamePage(const wxString &sNewName, int nSelection = -1);
private:
  size_t FindInsertPosition(const wxString &s);
  wxButton *m_pButtonAdd;
  wxButton *m_pButtonRemove;
  wxButton *m_pButtonRename;
  bool m_bReadOnly;
};


//********************************************************************
//
//    CPanelLabNameStrings
//
class CPanelLabNameStrings : public nwxPanel
{
public:
  CPanelLabNameStrings(wxWindow *parent, CLabNameStrings *pData);
  virtual ~CPanelLabNameStrings();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void OnChangeCombo(wxChoicebookEvent &);
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  nwxChoicebook *m_pBook;
  wxStaticText *m_pLabelStdCtrlName;
  wxTextCtrl *m_pTextLadder;
  wxTextCtrl *m_pTextPos;
  wxTextCtrl *m_pTextNeg;
  wxTextCtrl *m_pTextStdCtrlName;
  vector<wxTextCtrl *> m_pvTextSpecimenTypes;
  CLabNameStrings *m_pData;
  wxBoxSizer *m_pSizer;
  int m_nPosCtrlNdx;
  DECLARE_EVENT_TABLE()
};

//********************************************************************
//
//    CPanelLabThresholds, CPanelRFU, CPanelOtherThresholds
//
class CPanelRFU : public nwxPanel
{
public:
  CPanelRFU(
    wxWindow *parent,
    wxWindowID id,
    CLabRFU *pData,
    bool *pbAllowOverride);
  virtual ~CPanelRFU();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true)
  {
    m_pTextMinRFU->SetEditable(!b);
    m_pTextMinRFUinterlocus->SetEditable(!b);
    m_pTextMaxRFU->SetEditable(!b);
    m_pTextFractionOfMaxPeak->SetEditable(!b);
    m_pTextStutterThreshold->SetEditable(!b);
    m_pTextAdenylationThreshold->SetEditable(!b);
    m_pCheckAllowOverride->Enable(!b);
    if(m_pTextPullupFractionFilter != NULL)
    {
      m_pTextPullupFractionFilter->SetEditable(!b);
    }
  }
  wxCheckBox *GetOverrideCheckbox()
  {
    return m_pCheckAllowOverride;
  }
private:
  void AddLine(const char *ps,wxWindow *pw);
  wxTextCtrl *CreateTextCtrl(const wxValidator &v);

  wxGBPosition m_gbPos;
  wxTextCtrl *m_pTextMinRFU;
  wxTextCtrl *m_pTextMinRFUinterlocus;
  wxTextCtrl *m_pTextMaxRFU;
  wxTextCtrl *m_pTextFractionOfMaxPeak;
  wxTextCtrl *m_pTextPullupFractionFilter;
  wxTextCtrl *m_pTextStutterThreshold;
  wxTextCtrl *m_pTextAdenylationThreshold;
  wxCheckBox *m_pCheckAllowOverride;

  wxGridBagSizer *m_pSizer;
  CLabRFU *m_pData;
  bool *m_pbAllowOverride;
  unsigned int m_n;
  double m_d;
};

class CPanelOtherThresholds : public nwxPanel
{
public:
  CPanelOtherThresholds(wxWindow *parent, wxWindowID id, CLabThresholds *pData);
  virtual void SetReadOnly(bool b = true)
  {
    m_pChoiceHomozygoteUnits->Enable(!b);
    m_pTextHetImbalance->SetEditable(!b);
    m_pTextMinHomozygote->SetEditable(!b);
    m_pTextMaxPullup->SetEditable(!b);
    m_pTextMaxStutter->SetEditable(!b);
//    m_pTextMaxSpike->SetEditable(!b);
    m_pTextMaxAdenyl->SetEditable(!b);
    m_pTextMaxOffLadder->SetEditable(!b);
    m_pTextMaxResidual->SetEditable(!b);
    m_pTextMaxExcessiveResidual->SetEditable(!b);
    m_pTextMinBPSForArtifacts->SetEditable(!b);
    m_pTextMinBPSForArtifacts->SetEditable(!b);
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
private:
  static void _AddLine(
    wxWindow *parent,
    wxSizer *pSizer,
    const char *psLabel,
    wxWindow *pwText,
    const char *psSuffix);
  static void _AddLine(
    wxWindow *parent,
    wxSizer *pSizer,
    const char *psLabel,
    wxWindow *pwText,
    wxWindow *pWin = NULL);

  CXMLmessageBook m_msgBook;
  vector<wxTextCtrl *> m_pvTextMsgBookThresholds;
  vector<const CXMLmessageBookSM *> m_pvMsgBookSM;

  wxChoice *m_pChoiceHomozygoteUnits;
  wxTextCtrl *m_pTextHetImbalance;
  wxTextCtrl *m_pTextMinHomozygote;
//  wxTextCtrl *m_pTextMaxSpike;

  // the following 5 may be replaced with message book
  // parameters in the future

  wxTextCtrl *m_pTextMaxPullup;
  wxTextCtrl *m_pTextMaxStutter;
  wxTextCtrl *m_pTextMaxAdenyl;
  wxTextCtrl *m_pTextMaxOffLadder;
  wxTextCtrl *m_pTextMaxExcessiveResidual;

  wxTextCtrl *m_pTextMaxResidual;
  wxTextCtrl *m_pTextMinBPSForArtifacts;

  CLabThresholds *m_pData;


};

class CPanelLabThresholds : public nwxPanel
{
public:
  CPanelLabThresholds(
    wxWindow *parent,
    wxWindowID id,
    CLabThresholds *pData);
  virtual ~CPanelLabThresholds();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  nwxSyncCheckbox m_sync;

  CLabThresholds *m_pData;
  nwxChoicebook *m_pBook;
  CPanelRFU *m_pPanelLadder;
  CPanelRFU *m_pPanelLS;
  CPanelRFU *m_pPanelSample;
  CPanelOtherThresholds *m_pPanelMisc;
  CPanelRFU *CreateRFUPanel(CLabRFU &Data)
  {
    CPanelRFU *pRtn = new CPanelRFU(
      m_pBook,
      wxID_ANY,
      &Data,&m_pData->m_bAllowMinRFUoverride);
    wxCheckBox *pcb = pRtn->GetOverrideCheckbox();
    m_sync.Add(pcb);
    return pRtn;
  }
public:
  void OnCheckbox(wxCommandEvent &e);
  DECLARE_EVENT_TABLE()
};
//********************************************************************
//
//    CPanelLabLocusCollection
//
class CPanelLabLocusCollection : public nwxPanel, public INameValidator
{
public:
  static const int GRIDROWS;
  CPanelLabLocusCollection(
    wxWindow *parent,
    CLabLocusCollection *pData,
    const set<wxString> *pSetLoci,
    int nGridRows = CPanelLabLocusCollection::GRIDROWS,
    bool bTriallele = false,
    bool bSkipAmel = false);
  virtual ~CPanelLabLocusCollection();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void OnAddLocus(wxCommandEvent &);
  void OnRemoveLocus(wxCommandEvent &);
  virtual bool IsNameUsed(const wxString &s);
  CLabLocusCollection *GetData()
  {
    return m_pData;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  nwxChoicebook *m_pBook;
  CLabLocusCollection *m_pData;
  const set<wxString> *m_pSetLoci;
  int m_nGridRows;
  bool m_bTriallele;
  bool m_bSkipAmel;
  DECLARE_EVENT_TABLE()
};
//********************************************************************
//
//    CPanelPosCtrlLoci
//
class CPanelPosCtrlLoci : public nwxPanel
{
public:
  CPanelPosCtrlLoci(
    wxWindow *parent,
    CLabPositiveControl *pPosCtrl,
    const set<wxString> *pSetLoci);
  virtual ~CPanelPosCtrlLoci();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true)
  {
    m_pPanelLoci->SetReadOnly(b);
    m_pTextFileStr->SetEditable(!b);
  }

private:
  CLabPositiveControl *m_pPosCtrl;
  CPanelLabLocusCollection *m_pPanelLoci;
  wxTextCtrl *m_pTextFileStr;
  static const int ROWS;

};


//********************************************************************
//
//    CPanelAlleleGrid
//
class CPanelAlleleGrid : public nwxPanel
{
public:
  CPanelAlleleGrid(
    wxWindow *parent, 
    CLabLocus *pLocus, 
    int nGridRows = CPanelLabLocusCollection::GRIDROWS);
  virtual ~CPanelAlleleGrid();
  wxGrid *GetGrid()
  {
    return m_pGrid;
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  CLabLocus *GetLocusData()
  {
    return m_pLocus;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pGrid->SetAllReadOnly(b);
  }
private:
  void CreateAlleleGrid(bool bIsAmel);
  nwxGrid *m_pGrid;
  CLabLocus *m_pLocus;
  int m_nGridRows;
};

//********************************************************************
//
//    CPanelTrialleleGrid
//
class CPanelTrialleleGrid : public nwxPanel
{
public:
  CPanelTrialleleGrid(
    wxWindow *parent,
    CLabLocusCollection *pLocus,
    const wxString &sLocusName,
    int nGridRows = CPanelLabLocusCollection::GRIDROWS);
  virtual ~CPanelTrialleleGrid();
  wxGrid *GetGrid()
  {
    return m_pGrid;
  }
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  CLabLocusCollection *GetLocusData()
  {
    return m_pLocus;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pGrid->SetAllReadOnly(b);
  }
private:
  void CreateAlleleGrid(bool bIsAmel);
  nwxGrid *m_pGrid;
  CLabLocusCollection *m_pLocus;
  wxString m_sLocusName;
  int m_nGridRows;
};



//********************************************************************
//
//    CDialogName
//
//     choose a name for a new item
//
class CDialogName : public wxDialog
{
public:
  CDialogName(
    wxWindow *pParent,
    INameValidator *pv,
    const wxString &sPrompt,
    const wxString &sTitle,
    const set<wxString> *pSetValues = NULL,
    bool bSkipAmel = false,
    const wxString *psCurrent = NULL);
  virtual ~CDialogName();
  wxString GetName();
  virtual bool TransferDataFromWindow();
private:
  const wxString m_sPrompt;
  wxString m_sCurrent;
  wxTextCtrl *m_pText;
  wxComboBox *m_pCombo;
  INameValidator *m_pv;
};

//********************************************************************
//
//    CPanelPosCtrlMarkerSet
//
class CPanelPosCtrlMarkerSet : public nwxPanel, public INameValidator
{
public:
  CPanelPosCtrlMarkerSet(wxWindow *parent, CLabPositiveControls *pData, const set<wxString> *pSetLoci);
  virtual ~CPanelPosCtrlMarkerSet();
  void OnAdd(wxCommandEvent &);
  void OnRemove(wxCommandEvent &);
  void OnRename(wxCommandEvent &);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual bool IsNameUsed(const wxString &s);
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  CLabPositiveControls *m_pData;
  nwxChoicebook *m_pBook;
  const set<wxString> *m_pSetLoci;
  DECLARE_EVENT_TABLE();

};

//********************************************************************
//
//    CPanelLabMarkerSet
//
class CPanelLabMarkerSet : public nwxPanel
{
public:
  CPanelLabMarkerSet(wxWindow *parent,CLabMarkerSetCollection *pData, const set<wxString> *pSetLoci);
  virtual ~CPanelLabMarkerSet();
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  CLabMarkerSetCollection *GetData()
  {
    return m_pData;
  }
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  CLabMarkerSetCollection *m_pData; // <LabMarkerSetCollection>

  nwxChoicebook *m_pBook;
  CPanelLabLocusCollection *m_pPanelOffLadder;
  CPanelLabLocusCollection *m_pPanelSampleTrialleles;
//  CPanelLabLocusCollection *m_pPanelPosCtrlTrialleles;
//  3/26/10 - Positive control tri-alleles are obsolete in lab settings
//    bacause of positive controls below
//    the corresponding data will be left in CLabSettings
//    for backward compatibility but should be removed in v 2.0
//
  CPanelPosCtrlMarkerSet *m_pPanelPosCtrl;
  const set<wxString> *m_pSetLoci;
};


//********************************************************************
//
//    CPanelLabAllMarkerSets
//
class CPanelLabAllMarkerSets : public nwxPanel, public INameValidator
{
public:
  CPanelLabAllMarkerSets(wxWindow *parent,
    CLabMarkerSetSpecifications *pData,
    const set<wxString> *pLoci,
    const set<wxString> *pKits);
  virtual ~CPanelLabAllMarkerSets();

  void OnAddMarker(wxCommandEvent &);
  void OnRemoveMarker(wxCommandEvent &);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual bool IsNameUsed(const wxString &s);
  virtual void SetReadOnly(bool b = true)
  {
    m_pBook->SetReadOnly(b);
  }
private:
  nwxChoicebook *m_pBook;
  CLabMarkerSetSpecifications *m_pData;
  const set<wxString> *m_pSetLoci;
  const set<wxString> *m_pSetKits;
  DECLARE_EVENT_TABLE()
};
//********************************************************************
//
//    CPanelReviewAcceptance
//
class CPanelReviewAcceptance : public nwxPanel
{
public:
  CPanelReviewAcceptance(wxWindow *parent,
     CLabReview *pData);
  virtual ~CPanelReviewAcceptance();

  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true);
private:
  CLabReview *m_pReview;
  wxSpinCtrl *m_pSpinReview;
  wxSpinCtrl *m_pSpinAccept;
};


//********************************************************************
//
//    CDialogLabSettings
//
class CDialogLabSettings : public wxDialog, public ISetReadOnly
{
public:
  CDialogLabSettings(
    wxWindow *parent, CLabSettings *pSettings, CAllLoci *pLoci = NULL);
  virtual ~CDialogLabSettings();
  static const wxSize g_SIZE_NUMBER;
  static const wxSize g_SIZE_MULTILINE_TEXT;
  static const wxString g_sFileNameStrPrompt;

  static wxTextCtrl *CreateTextCtrl(wxWindow *parent, const wxValidator &v = wxDefaultValidator)
  {
    return new wxTextCtrl(
      parent,wxID_ANY,wxEmptyString,wxDefaultPosition,
      CDialogLabSettings::g_SIZE_NUMBER,
      wxTE_LEFT,v);
  }
  static double Str2Double(const wxString &_s, double dDefault = -1.0);
  static int Str2Int(const wxString &_s, int nDefault = -1);
  static wxString Number2Str(double d, double dDefaultBlank = -1.0);
  static wxString Number2Str(int n, int nDefaultBlank = -1);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  virtual void SetReadOnly(bool b = true)
  {
    m_pThresholds->SetReadOnly(b);
    m_pFilenames->SetReadOnly(b);
    m_pMarkerSets->SetReadOnly(b);
  }
  void OnPageChange(wxNotebookEvent &);
  void OnNext(wxCommandEvent &);
  void OnBack(wxCommandEvent &);
private:
  wxButton *m_pbBack;
  wxButton *m_pbNext;
  wxNotebook *m_pNotebook;
  CPanelLabThresholds *m_pThresholds;
  CPanelLabNameStrings *m_pFilenames;
  CPanelLabAllMarkerSets *m_pMarkerSets;
  CPanelReviewAcceptance *m_pReview;
  const set<wxString> *m_pSetLoci;
  const set<wxString> *m_pSetKits;
  DECLARE_EVENT_TABLE()
};

inline wxTextCtrl *CPanelRFU::CreateTextCtrl(const wxValidator &v)
{
  return CDialogLabSettings::CreateTextCtrl(this,v);
}


#endif
