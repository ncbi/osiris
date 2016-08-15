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
#include "IPageEditSample.h"
#include "CLabSettings.h"
#include "COARfile.h"
class CNotebookEditSample;

class CPageEditSample : public IPageEditSample
{
public:
  CPageEditSample(CNotebookEditSample *pParentPanel, COARfile *pFile, int nCountType, bool bReadOnly = false) :
      m_pFile(pFile),
      m_pParent(pParentPanel),
      m_pPanel(NULL),
      m_nReviewerCount(-1),
      m_nAcceptanceCount(-1),
      m_pReview(NULL),
      m_pAccept(NULL),
      m_bReadOnly(bReadOnly)
      {
        m_pFile->GetReviewerCounts(
          &m_nReviewerCount, 
          &m_nAcceptanceCount,
          nCountType);      
      };
  virtual ~CPageEditSample();
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
//  virtual operator wxWindow *()
//  {
//    return GetPanel();
//  }
  virtual bool DoReview();
  virtual bool DoAccept();
  wxWindow *GetParentWindow();
  virtual const wxString &GetPageLabel() = 0;
  virtual const wxString &GetNewNotes() = 0;
  bool IsNewNotesEmpty()
  {
    return GetNewNotes().IsEmpty();
  }
  virtual wxWindow *GetPanel();
protected:
  const wxString &GetUserID();

  virtual wxWindow *CreatePanel() = 0;
  virtual IAppendReview *CreateReviewReceiver() = 0;
  virtual IAppendReview *CreateAcceptReceiver() = 0;

private:
  wxString m_sUserID;
protected:
  COARfile *m_pFile;
  CNotebookEditSample *m_pParent;
  wxWindow *m_pPanel;
  int m_nReviewerCount;
  int m_nAcceptanceCount;
  IAppendReview *m_pReview;
  IAppendReview *m_pAccept;
private:
  bool m_bReadOnly;
};

class CPageEditSampleAlerts : public CPageEditSample
{
public:
    CPageEditSampleAlerts(
      CNotebookEditSample *pParentPanel,
      COARfile *pFile,
      int nCountType,
      bool bReadOnly = false) :
        CPageEditSample(
          pParentPanel,
          pFile,
          nCountType,
          bReadOnly)
      {}
  virtual void DoApply();
  virtual bool TransferDataToPage();
  virtual bool NeedsApply();
  virtual const wxString &GetPageLabel()
  {
    return m_sLabel;
  }
  virtual const wxString &GetNewNotes();
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
  virtual wxString GetReviewAcceptance() = 0;
  virtual void UpdateNotes() = 0;
  virtual const COARnotes *GetNotes() = 0;
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
public:
  CEditAlertsDir(
    CNotebookEditSample *pParentPanel, 
    COARfile *pFile, 
    bool bReadOnly = false) :
      CPageEditSampleAlerts(
        pParentPanel,
        pFile,
        CLabReview::REVIEW_DIR,
        bReadOnly)
  {
    _SETUP_LABELS(wxT("Directory"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual wxWindow *CreatePanel();
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
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
        CLabReview::REVIEW_SAMPLE,
        bReadOnly),
          m_pSample(pSample)
  {
    _SETUP_LABELS(wxT("Sample"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual wxWindow *CreatePanel();
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
private:
  COARsample *m_pSample;
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
        CLabReview::REVIEW_ILS,
        bReadOnly),
          m_pSample(pSample)
  {
    _SETUP_LABELS(wxT("ILS"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual wxWindow *CreatePanel();
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
private:
  COARsample *m_pSample;
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
        CLabReview::REVIEW_CHANNEL,
        bReadOnly),
          m_pSample(pSample),
          m_pmapChannelNames(pmapChannelNames)
  {
    _SETUP_LABELS(wxT("Channel"));
  }
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual void UpdateNotes();
  virtual const COARnotes *GetNotes();
  virtual wxWindow *CreatePanel();
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
  virtual wxString GetReviewAcceptance();
private:
  COARsample *m_pSample;
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
          CLabReview::REVIEW_LOCUS,
          bReadOnly),
      m_sLocusName(sLocusName),
      m_nChannel(nChannelNr),
      m_pSample(pSample),
      m_pLocus(NULL)
  {
    m_pLocus = m_pSample->FindLocus(m_sLocusName);
  };
  virtual bool NeedsApply(); 
      // CPageEditSample::NeedsApply is not appropriate
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual void DoApply();
  virtual bool TransferDataToPage();
  virtual const wxString &GetPageLabel()
  {
    return m_sLocusName;
  }
  virtual const wxString &GetNewNotes();
protected:
  virtual wxWindow *CreatePanel();
  virtual IAppendReview *CreateReviewReceiver();
  virtual IAppendReview *CreateAcceptReceiver();
private:
  wxString m_sLocusName;
  wxString m_sNotes;
  int m_nChannel;
  COARsample *m_pSample;
  COARlocus *m_pLocus;
  
};

#endif