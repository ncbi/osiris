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
*  FileName: CNotebookEditSample.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_NOTEBOOK_EDIT_SAMPLE_H__
#define __C_NOTEBOOK_EDIT_SAMPLE_H__

#include <wx/datetime.h>
#include <wx/splitter.h>
#include <wx/treebook.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include "CLabSettings.h"
#include "CGridAlerts.h"
#include "COARfile.h"
#include "CHistoryTime.h"
#include "ISetDateTime.h"
#include "IEditPanel.h"
#include "CPanelSampleAlertDetails.h"

class CPanelUserID;
class CNotebookEditSample;

#ifndef __WINDOW_ARRAY_INDEX__
#define __WINDOW_ARRAY_INDEX__
typedef enum
{
  SA_NDX_DIR,
  SA_NDX_SAMPLE,
  SA_NDX_ILS,
  SA_NDX_CHANNEL,
  SA_WINDOW_COUNT
} ALERT_WINDOW_TYPE; // used in arrays below

#endif

// probably doesn't need to be in .h file
/*
class CPageEditSample : public wxPanel
{
  //  a notebook page for CNotebookEditSample
  //  the parameter, pEditPanel, creates a 
  //  panel for editing which will be a child 
  //  window of this
public:
  CPageEditSample(CNotebookEditSample *pParentPanel, IEditPanel *pEditPanel);
  bool NeedsApply();
  void DoApply();
  void RepaintData();
  const wxString &GetUserID();
private:
  IEditPanel *m_pEditPanel;
  CNotebookEditSample *m_pParent;
};
*/

class CEditAlertsBase : public IEditPanel
{
public:
  CEditAlertsBase(CNotebookEditSample *pParentPanel, COARfile *pFile, int nCountType, bool bReadOnly = false) :
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
  virtual ~CEditAlertsBase();
  bool IsReadOnly()
  {
    return m_bReadOnly;
  }
  virtual bool NeedsApply();
  virtual operator wxWindow *()
  {
    return GetPanel();
  }
  virtual void DoReview();
  virtual void DoAccept();
  wxWindow *GetParentWindow();
protected:
  virtual wxWindow *GetPanel() = 0;
  virtual IAppendReview *GetReviewReceiver() = 0;
  virtual IAppendReview *GetAcceptReceiver() = 0;

  COARmessages m_Msg;
  COARfile *m_pFile;
  CNotebookEditSample *m_pParent;
  CPanelSampleAlertDetails *m_pPanel;
  int m_nReviewerCount;
  int m_nAcceptanceCount;
private:
  IAppendReview *m_pReview;
  IAppendReview *m_pAccept;
  bool m_bReadOnly;
};
class CEditAlertsDir : public CEditAlertsBase
{
public:
  CEditAlertsDir(
    CNotebookEditSample *pParentPanel, 
    COARfile *pFile, 
    bool bReadOnly = false) :
      CEditAlertsBase(
        pParentPanel,
        pFile,
        CLabReview::REVIEW_DIR,
        bReadOnly)
  {}
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual void DoApply();
protected:
  virtual wxWindow *GetPanel();
  virtual IAppendReview *GetReviewReceiver();
  virtual IAppendReview *GetAcceptReceiver();
};

class CEditAlertsSample : public CEditAlertsBase
{
public:
  CEditAlertsSample(    
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    bool bReadOnly = false) :
      CEditAlertsBase(
        pParentPanel,
        pSample->GetFile(),
        CLabReview::REVIEW_SAMPLE,
        bReadOnly),
          m_pSample(pSample)
  {}
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual void DoApply();
  virtual void DoReview();
  virtual void DoAccept();
protected:
  virtual wxWindow *GetPanel();
private:
  COARsample *m_pSample;
};

class CEditAlertsILS : public CEditAlertsBase
{
public:
  CEditAlertsILS(
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    bool bReadOnly = false) :
      CEditAlertsBase(
        pParentPanel,
        pSample->GetFile(),
        CLabReview::REVIEW_ILS,
        bReadOnly),
          m_pSample(pSample)
  {}
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
  virtual void DoApply();
  virtual void DoReview();
  virtual void DoAccept();
protected:
  virtual wxWindow *GetPanel();
private:
  COARsample *m_pSample;
};

class CEditAlertsChannel : public CEditAlertsBase
{
public:
  CEditAlertsChannel(    
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    bool bReadOnly = false) :
      CEditAlertsBase(
        pParentPanel,
        pSample->GetFile(),
        CLabReview::REVIEW_CHANNEL,
        bReadOnly),
          m_pSample(pSample)
  {}
  virtual bool NeedsAcceptance();
  virtual bool NeedsReview();
  virtual bool HasHistory();
protected:
  virtual wxWindow *GetPanel();
private:
  COARsample *m_pSample;
};


class CEditLocus : public CEditAlertsBase
{
public:
  CEditLocus(
    CNotebookEditSample *pParentPanel, 
    COARsample *pSample, 
    const wxString &sLocusName,
    bool bReadOnly = false) :
      CEditAlertsBase(
          pParentPanel,
          pSample->GetFile(),
          CLabReview::REVIEW_LOCUS,
          bReadOnly),
      m_sLocusName(sLocusName)
  {};
private:
  wxString m_sLocusName;
  
};

class CNotebookEditSample : public wxPanel, public ISetDateTime
{
public:
  CNotebookEditSample(
    const COARfile *pFile, // dont' need pFile, get it from pSample
    const COARsample *pSample, 
    wxWindow *parent,
    wxWindowID id = wxID_ANY,
    const map<int,wxString> *pmapChannelNames = NULL
    );
  virtual ~CNotebookEditSample();
  virtual bool TransferDataToWindow();
  virtual bool Validate();
  bool IsModified(int n);
  bool IsModified();
  bool IsNotesModified(int n);

  void Select(int n)
  {
    // select notebook page, Sample, ILS, or Channel
    m_pNotebook->ChangeSelection((size_t)n);
  }
  int GetSelection()
  {
    return m_pNotebook->GetSelection();
  }
  CPanelSampleAlertDetails **GetSplitterWindows()
  {
    return &m_pSplitter[0];
  }
  CPanelSampleAlertDetails *GetSplitterWindow(int n)
  {
    return m_pSplitter[n];
  }
  wxTreebook *GetNotebookWindow()
  {
    return m_pNotebook;
  }
  virtual bool SetDateTime(const wxDateTime *pTime);
  const COARmessages &GetDirMessages()
  {
    return m_MsgDir;
  }
  const COARmessages &GetSampleMessages()
  {
    return m_MsgSample;
  }
  const COARmessages &GetILSMessages()
  {
    return m_MsgILS;
  }
  const COARmessages &GetChannelMessages()
  {
    return m_MsgChannel;
  }
  wxString GetNewSampleNotes()
  {
    return _GetNewNotes(SA_NDX_SAMPLE);
  }
  wxString GetNewILSNotes()
  {
    return _GetNewNotes(SA_NDX_ILS);
  }
  wxString GetNewChannelNotes()
  {
    return _GetNewNotes(SA_NDX_CHANNEL);
  }
  wxString GetNewDirNotes()
  {
    return _GetNewNotes(SA_NDX_DIR);
  }

  bool IsAlertsModified(int n);

  bool IsIndexUsed(int n)
  {
    return (m_pSplitter[n] != NULL);
  }
  void SetPanelUserID(CPanelUserID *p)
  {
    m_pPanelUser = p;
  }
  const wxString &GetCurrentLocus();
  const wxString &GetUserID();
  void RepaintData();
  void InitiateRepaintData();
private:
  bool _ProcessEvent();
  const COARnotes *_GetNotes(int n, const wxDateTime *pdt = NULL);
  wxString _GetReviewAcceptance(int n, const wxDateTime *pdt = NULL);
  const wxString &_GetNotesText(int n, const wxDateTime *pdt = NULL)
  {
    return COARnotes::GetText(_GetNotes(n,pdt));
  }
  wxString _GetNewNotes(int n)
  {
    wxString sRtn;
    CPanelSampleAlertDetails *psd = m_pSplitter[n];

    if(psd != NULL)
    {
      sRtn = psd->GetNewNotesValue();
    }
    return sRtn;
  }

  std::vector<const wxString> m_asLocus;
  wxString m_sUserID;
  CHistoryTime m_HistTime;
  COARmessages m_MsgDir;
  COARmessages m_MsgSample;
  COARmessages m_MsgILS;
  COARmessages m_MsgChannel;

  const COARsample *m_pSample;
  const COARfile *m_pFile;

  wxTreebook *m_pNotebook;
  CPanelUserID *m_pPanelUser;
  CPanelSampleAlertDetails *m_pSplitter[SA_WINDOW_COUNT];
  bool m_bReadOnly;


  static const wxString g_sLabelDirNotices;
  static const wxString g_sLabelSampleNotices;
  static const wxString g_sLabelILSNotices;
  static const wxString g_sLabelChannelNotices;
  static const wxString * const g_psLabels[SA_WINDOW_COUNT];
  static const wxString * const g_psNotesLabels[SA_WINDOW_COUNT];
public:
  static const wxString g_sLabelDir;
  static const wxString g_sLabelSample;
  static const wxString g_sLabelILS;
  static const wxString g_sLabelChannel;

  static const wxString &GetLabel(int ndx)
  {
    const wxString *psRtn = g_psLabels[ndx];
    return *psRtn;
  }
public:
  void OnCellChange(wxGridEvent &e);
  void OnNotesChange(wxCommandEvent &e);
  DECLARE_EVENT_TABLE();
};


#endif
