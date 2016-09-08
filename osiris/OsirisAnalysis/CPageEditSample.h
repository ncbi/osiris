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
*  FileName: CPageEditSample.h
*  Author:   Douglas Hoffman
*
*  Interface class for any panel used for editing part of a sample
*
*/
#ifndef __C_PAGE_EDIT_SAMPLE_H__
#define __C_PAGE_EDIT_SAMPLE_H__

#include <wx/datetime.h>
#include <wx/window.h>
#include "CLabSettings.h"
#include "COARfile.h"
class CNotebookEditSample;
class CPanelSampleAlertDetails;
class CPanelLocusDetails;

class CPageEditSample
{
public:
  CPageEditSample(CNotebookEditSample *pParentPanel, COARfile *pFile, COARsample *pSample, int nCountType, bool bReadOnly = false) :
      m_pFile(pFile),
      m_pParentNotebook(pParentPanel),
      m_pPanel(NULL),
      m_nReviewerCount(-1),
      m_nAcceptanceCount(-1),
      m_pReview(NULL),
      m_pAccept(NULL),
      m_pDisable(NULL),
      m_pSample(pSample),
      m_bReadOnly(bReadOnly)
      {
        m_pFile->GetReviewerCounts(
          &m_nReviewerCount, 
          &m_nAcceptanceCount,
          nCountType);      
      };
  virtual ~CPageEditSample();
  virtual bool NeedsApply() = 0;
  virtual bool NeedsAcceptance() = 0;
  virtual bool NeedsReview() = 0;
  virtual bool HasHistory() = 0;
  virtual bool DoApply() = 0;
  virtual bool TransferDataToPage() = 0;
  virtual void SetReadOnly(bool bReadOnly) = 0;
  virtual void CheckReadOnly()
  {
    COARsample *pSample = GetSample();
    bool b = IsReadOnly();
    if( (pSample != NULL) && (pSample->IsDisabled() != b) )
    {
      SetReadOnly(!b);
    }
  }
  virtual const wxString &GetPageLabel() = 0;
  virtual const wxString &GetNewNotes() = 0;
  virtual const wxString &GetTreePageLabel();
  bool NeedsAttn()
  {
    return NeedsAcceptance() || NeedsReview();
  }
  COARsample *GetSample()
  {
    return m_pSample;
  }
  COARfile *GetFile()
  {
    return m_pFile;
  }
  virtual bool DoReview();
  virtual bool DoAccept();
  virtual bool DoToggleEnabled();
  virtual wxWindow *GetPanel();
  virtual wxWindow *GetPanelPage();
  wxWindow *GetParentWindow();
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
  bool IsNewNotesEmpty()
  {
    return GetNewNotes().IsEmpty();
  }
protected:
  const wxString &GetUserID();

  virtual wxWindow *CreatePanel(wxWindow *parent) = 0;
  virtual IAppendReview *CreateReviewReceiver() = 0;
  virtual IAppendReview *CreateAcceptReceiver() = 0;
  virtual IAppendReview *CreateDisableReceiver();
  void _SetReadOnlyRaw(bool b)
  {
    m_bReadOnly = b;
  }

private:
  wxString m_sUserID;
protected:
  wxString m_sTreePageLabelModified;
  wxString m_sTreePageLabelPlain;
  COARfile *m_pFile;
  CNotebookEditSample *m_pParentNotebook;
  wxWindow *m_pPanel;
  //wxWindow *m_pPanelPage;
  int m_nReviewerCount;
  int m_nAcceptanceCount;
  IAppendReview *m_pReview;
  IAppendReview *m_pAccept;
  IAppendReview *m_pDisable;
private:
  COARsample *m_pSample;
  bool m_bReadOnly;
};

class CPageEditSampleAlerts : public CPageEditSample
{
public:
    CPageEditSampleAlerts(
      CNotebookEditSample *pParentPanel,
      COARfile *pFile,
      COARsample *pSample,
      int nCountType,
      bool bReadOnly = false) :
        CPageEditSample(
          pParentPanel,
          pFile,
          pSample,
          nCountType,
          bReadOnly)
      {}
  virtual bool DoApply();
  virtual bool TransferDataToPage();
  virtual bool NeedsApply();
  virtual const wxString &GetPageLabel()
  {
    return m_sLabel;
  }
  virtual const wxString &GetNewNotes();
  virtual void SetReadOnly(bool bReadOnly);
protected:
  const wxString &GetLabelPrefix()
  {
    return m_sLabelShort;
  }
  void _SETUP_LABELS(const wxChar *ps)
  {
    m_sLabelShort = ps;
    m_sLabelShort.Append(wxT(" "));
    m_sLabel = m_sLabelShort;
    m_sLabel.Append("Notices");
  }
  virtual wxWindow *CreatePanel(wxWindow *parent);
  virtual CPanelSampleAlertDetails *CreateSubPanel(wxWindow *parent) = 0;
  CPanelSampleAlertDetails *GetSubPanel();
  virtual wxString GetReviewAcceptance() = 0;
  virtual void UpdateNotes() = 0;
  virtual const COARnotes *GetNotes() = 0;
  virtual void UpdateMessages() = 0;
  const wxString &GetNotesText()
  {
    return COARnotes::GetText(GetNotes());
  }
  COARmessages m_Msg;
private:
  wxString m_sLabel;
  wxString m_sLabelShort;
  wxString m_sNotes;
};


class CEditAlertsDir : public CPageEditSampleAlerts
{
private:
  class CDirPanelSet
  {
  public:
    static void Add(CEditAlertsDir *pPanel);
    static void Remove(CEditAlertsDir *pPanel);
    static void SyncAllTo(CEditAlertsDir *pPanel);
  private:
    CDirPanelSet() {} // private, prevent instantiation
    typedef std::set<CEditAlertsDir *> SET_PANEL;
    typedef std::map<COARfile *, SET_PANEL *> MAP_ALL;
    typedef MAP_ALL::iterator MAP_ALL_ITERATOR;
    static MAP_ALL g_mapAll;
    static MAP_ALL_ITERATOR _Find(CEditAlertsDir *pPanel)
    {
      MAP_ALL_ITERATOR rtn = g_mapAll.find(pPanel->GetFile());
      return rtn;
    }
  };


public:
  CEditAlertsDir(
    CNotebookEditSample *pParentPanel, 
    COARfile *pFile, 
    bool bReadOnly = false) :
      CPageEditSampleAlerts(
        pParentPanel,
        pFile,
        (COARsample *)NULL,
        CLabReview::REVIEW_DIR,
        bReadOnly)
  {
    _SETUP_LABELS(wxT("Directory"));
    CDirPanelSet::Add(this);
  }
  virtual ~CEditAlertsDir()
  {
    CDirPanelSet::Remove(this);
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual bool DoApply()
  {
    bool bRtn = CPageEditSampleAlerts::DoApply();
    SyncAll();
    return bRtn;
  }
  virtual bool DoReview()
  {
    bool bRtn = CPageEditSampleAlerts::DoReview();
    SyncAll();
    return bRtn;
  }
  virtual bool DoAccept()
  {
    bool bRtn = CPageEditSampleAlerts::DoAccept();
    SyncAll();
    return bRtn;
  }
  void SyncAll() 
  {
    CDirPanelSet::SyncAllTo(this);
  }
  void SyncTo(CEditAlertsDir *pFromThis);
protected:
  virtual void UpdateMessages();
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual CPanelSampleAlertDetails *CreateSubPanel(wxWindow *parent);
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
private:
  std::vector<wxString> m_vsLocus;
};




class CEditAlertsSample : public CPageEditSampleAlerts
{
public:
  CEditAlertsSample(    
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    bool bReadOnly = false) :
      CPageEditSampleAlerts(
        pParentPanel,
        pSample->GetFile(),
        pSample,
        CLabReview::REVIEW_SAMPLE,
        bReadOnly)
  {
    _SETUP_LABELS(wxT("Sample"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateMessages();
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual CPanelSampleAlertDetails *CreateSubPanel(wxWindow *pParent);
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
};


class CEditAlertsILS : public CPageEditSampleAlerts
{
public:
  CEditAlertsILS(
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    bool bReadOnly = false) :
      CPageEditSampleAlerts(
        pParentPanel,
        pSample->GetFile(),
        pSample,
        CLabReview::REVIEW_ILS,
        bReadOnly)
  {
    _SETUP_LABELS(wxT("ILS"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateMessages();
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual CPanelSampleAlertDetails *CreateSubPanel(wxWindow *pParent);
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
};


class CEditAlertsChannel : public CPageEditSampleAlerts
{
public:
  CEditAlertsChannel(    
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    const map<int,wxString> *pmapChannelNames,
    bool bReadOnly = false) :
      CPageEditSampleAlerts(
        pParentPanel,
        pSample->GetFile(),
        pSample,
        CLabReview::REVIEW_CHANNEL,
        bReadOnly),
          m_pmapChannelNames(pmapChannelNames)
  {
    _SETUP_LABELS(wxT("Channel"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateMessages();
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual CPanelSampleAlertDetails *CreateSubPanel(wxWindow *pParent);
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
private:
  const map<int,wxString> *m_pmapChannelNames;
};


class CPageEditLocus : public CPageEditSample
{
public:
  CPageEditLocus(
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    const wxString &sLocusName,
    int nChannelNr,
    bool bReadOnly = false) :
      CPageEditSample(
          pParentPanel,
          pSample->GetFile(),
          pSample,
          CLabReview::REVIEW_LOCUS,
          bReadOnly),
      m_sLocusName(sLocusName),
      m_nChannel(nChannelNr),
      m_pLocus(NULL)
  {
    m_pLocus = pSample->FindLocus(m_sLocusName);
  };
  virtual bool NeedsApply(); 
      // CPageEditSample::NeedsApply is not appropriate
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual bool DoApply();
  virtual bool TransferDataToPage();
  virtual const wxString &GetPageLabel()
  {
    return m_sLocusName;
  }
  virtual const wxString &GetNewNotes();
  virtual void SetReadOnly(bool bReadOnly);
protected:
  CPanelLocusDetails *GetLocusPanel();
  virtual wxWindow *CreatePanel(wxWindow *parent);
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
private:
  wxString m_sLocusName;
  wxString m_sNotes;
  int m_nChannel;
  COARlocus *m_pLocus;
  
};

#endif