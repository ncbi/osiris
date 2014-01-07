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
*  FileName: CSplitterCellHistory.h
*  Author:   Douglas Hoffman
*
*/

#include <wx/splitter.h>
#include "nwx/stdb.h"
#include <set>
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"
#include "nwx/nwxSyncNotebook.h"
#include "nwx/nwxSyncSplitter.h"
#include "nwx/nwxSyncScrollwin.h"

#include "COARfile.h"
#include "ISetDateTime.h"
#include "CPanelLocusDetails.h"

class wxStaticText;
class wxBoxSizer;
class CPanelHistory;

class CSplitterCellHistory: public wxSplitterWindow
{
public:

  //  locus/allele cell history

  CSplitterCellHistory(
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    wxWindow *parent, 
    wxWindowID id);

  //  locus/allele/artifact cell history (OAR file v1.2 or later)

  CSplitterCellHistory(
    const COARsample &Sample,
    int nChannel,
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime,
    wxWindow *parent, 
    wxWindowID id);

  //  Channel, ILS, and Sample alert history,
  //  uses channel # in alerts grid

  CSplitterCellHistory(
    int nSelect,
    bool bSingle,
    const COARfile &File,
    const COARsample &Sample,
    const map<int,wxString> &mapChannelNames,
    wxWindow *parent,
    wxWindowID id);

  virtual ~CSplitterCellHistory();
  void OnNotebookChange(wxNotebookEvent &);
  void OnHistoryChange(wxCommandEvent &);
  void OnSplitterChange(wxSplitterEvent &);
  void OnSplitterChanging(wxSplitterEvent &);
  void OnScroll(wxScrollWinEvent &e);
  void Split();
private:
  static bool _CheckMessageUpdate(
    const COARmessages &Msg,
    const vector<int> *pvn,
    const wxDateTime &dtLast, 
    const wxDateTime *pdt);


  void _SetupSetDateTime(
    const COARfile &File,
    const COARsample &Sample,
    set<wxDateTime> *psetKeep,
    int nSelect = -1);

  void _SetupSetDateTime(
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime, 
    set<wxDateTime> *psetKeep);

  void _SetupSetDateTime(
    const COARsample &Sample,
    int nChannel,
    const COARlocus &Locus,
    const COARmessages &Msg,
    const set<wxDateTime> &setDateTime, 
    set<wxDateTime> *psetKeep);

  static wxBoxSizer *_MakeSplitPanelSizer(
    wxWindow *parent, 
    wxWindow *topLabel,
    wxWindow *bottomData);

  void _LayoutWindow(
    wxWindow *topPanelData, wxWindow *bottomPanelData);

  void _BuildWindowSkeleton(const set<wxDateTime> &setDateTime);

  void _BuildWindow(
    const COARfile &File,
    const COARsample &Sample,
    const set<wxDateTime> &setDateTime,
    int nSelect,
    bool bShowOne);
  void _BuildWindow(
    const COARlocus *pLocus,
    const COARmessages *pMsg,
    const set<wxDateTime> &setDateTime,
    const COARsample *pSample = NULL, 
          // not NULL if OAR file v1.2 or later
    int nChannel = -1
    );

  void _OnScroll(wxCommandEvent &e);
  nwxSyncSplitter m_syncSplitter;
  nwxSyncNotebook m_syncNotebook;
#if !PANEL_LOCUS_NOTEBOOK
  nwxSyncSplitter m_syncSplitter2;
#endif
  vector<nwxSyncScrollwin *> m_apSyncScroll;
  ISetDateTime *m_pDateTime;

  CPanelHistory *m_pPanelHistory;
  wxStaticText *m_pLabelCurrent;
  wxPanel *m_pPanelTop;
  wxPanel *m_pPanelBottom;
  wxSplitterWindow *m_pSplitter;

  DECLARE_EVENT_TABLE()

};

