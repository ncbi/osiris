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
#include "mainApp.h"
#include "CKitColors.h"
#include "CNotebookEditSample.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"
#include "nwx/nsstd.h"
#include "nwx/nwxString.h"
#include "nwx/vectorptr.h"
#include "nwx/nwxLog.h"
#include "CPanelUserID.h"
#include "CPanelLocusDetails.h"
#include "CLabels.h"
#include "CFrameSample.h"
#include "CPageEditSample.h"
#include "CEditSampleIcons.h"

#define SETUP_STAT_IMG \
  CEditSampleIcons *pIcons = nwxGET_GLOBAL(CEditSampleIcons); \
  pIcons->CheckForUpdates(); \
  CEditSampleIconsBatch CEditSampleIconsBatch_XXX; \
  int ndxNeedsAttn = pIcons->GetNeedsAttnImage(); \
  int ndxOK = pIcons->GetOKImage();

#define STAT_IMG(bNeedsAttn) ((bNeedsAttn) ? ndxNeedsAttn : ndxOK)

#define STAT_IMG_PAGE(pPage) STAT_IMG(pPage->NeedsAttn())

CNotebookEditSample::~CNotebookEditSample() 
{
  vectorptr<CPageEditSample>::cleanup(&m_vpPanels);
}
#define NOTEBOOK_OUT_OF_RANGE 999999

CNotebookEditSample::CNotebookEditSample(
  COARfile *pFile,
  COARsample *pSample,
  wxWindow *parent,
  wxWindowID id,
  const map<int,wxString> *pmapChannelNames) :
    wxPanel(parent,id),
    m_pSample(pSample),
    m_pFile(pFile),
    m_pFrame(NULL),
    m_pNotebook(NULL),
    m_nSelectPage(0),
    m_nTimerCount(1), // delay setting notebook page
               // because of rendering problem
               // on the 'Notices' pages
               // see OnTimer() for details
    m_nBatch(0),
    m_bReadOnly(false)
{
  BeginBatch();
  vector<wxString> vsLocus;
  nwxLog_I_WAS_HERE;

  wxBoxSizer *pSizer;
  SETUP_STAT_IMG

  int i;
#ifdef __WXDEBUG__
  m_bTreeDumped = false;
#endif
  m_pNotebook = new nwxTreebook(this,true);
  m_pNotebook->SetImageList(pIcons->GetList());
  CPageEditSample *pPage;
  m_vpPanels.reserve(30);
  for(i = 0; i < SA_WINDOW_COUNT; i++)
  {
    switch(i)
    {
    case SA_NDX_DIR:
      pPage = new CEditAlertsDir(this,pFile);
      break;
    case SA_NDX_SAMPLE:
      pPage = new CEditAlertsSample(this,pSample);
      break;
    case SA_NDX_ILS:
      pPage = new CEditAlertsILS(this,pSample);
      break;
    case SA_NDX_CHANNEL:
      pPage = new CEditAlertsChannel(this,pSample,pmapChannelNames);
      break;
    default:
      pPage = NULL;
    }
    m_vpPanels.push_back(pPage);
    wxWindow *pWin = pPage->GetPanelPage();
    if(pWin == NULL)
    {
      wxString sErr =
        wxString::Format("Cannot create page for type %d",i);
      wxASSERT_MSG(0, sErr);
    }
    m_pNotebook->AddPage(pWin,pPage->GetTreePageLabel(),
      false,STAT_IMG_PAGE(pPage));
  }
  
  //****************************************************************
  // done with sample alert details, now set up loci
  //****************************************************************
  const wxColour *pColour;
  const wxString &sKitName = m_pFile->GetKitName();
  CKitColors *pKitColors = mainApp::GetKitColors();
  const CSingleKitColors *pSingleKit = pKitColors->GetKitColors(sKitName);
  const CChannelColors *pCC;


  size_t nChannelCount = m_pFile->GetChannelCount();
  size_t j,k,nLocusCount;
  const COARchannel *pChannel;
//  vector<size_t> vnExpand;
//  vector<size_t>::iterator itrExpand;
//  vnExpand.reserve(nChannelCount);
  for(k = 1; k <= nChannelCount; ++k)
  {
    // STOP HERE pChanel->GetChannelName() returns nothing
    pCC = pSingleKit->GetColorChannel(k);
    pColour = pCC->GetColorAnalyzedPtr();
    wxString sDyeName = pCC->GetDyeName();
    bool bEmpty = sDyeName.IsEmpty(); // for test/debug
    if(bEmpty)
    {
      sDyeName.Format("Channel %d",(int) k);
    }
    else
    {
      sDyeName.Append(wxString::Format(" - %d",(int)k));
    }
    pChannel = m_pFile->GetChannelByNr(k);
    m_pNotebook->AddPage(NULL,sDyeName,false,pIcons->GetSolidImage(*pColour));
//    vnExpand.push_back(m_vpPanels.size());
    m_vpPanels.push_back(NULL);
    m_asLocus.push_back(wxEmptyString);

    nLocusCount = pChannel->GetLocusCount();
    for(j = 0; j < nLocusCount; ++j)
    {
      const wxString &sLocusName = pChannel->GetLocusName(j);
      m_asLocus.push_back(sLocusName);
      pPage = new CPageEditLocus(this,pSample,sLocusName,(int)k);
      wxWindow *pWin = pPage->GetPanelPage();
      if(pWin == NULL)
      {
        wxString sErr(wxT("Cannot create page for "));
        sErr.Append(sLocusName);
        wxASSERT_MSG(0, sErr);
      }
      m_vpPanels.push_back(pPage);
      m_pNotebook->AddSubPage(pWin,pPage->GetTreePageLabel(),false,
        STAT_IMG_PAGE(pPage));
    }
  }
  // layout this
  m_pNotebook->GetTreeCtrl()->ExpandAll();
  m_pNotebook->SetSelection(m_pNotebook->GetPageCount() - 1);
  _SetupFrame();
  pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pNotebook,1,wxEXPAND);
  SetSizer(pSizer);
  Layout();
  // OnTimer() was called before constructor was done during testing
  EndBatch();
}
void CNotebookEditSample::_SetupFrame()
{
  if(m_pFrame == NULL)
  {
    wxWindow *pWin = GetParent();
    while(pWin != NULL)
    {
      m_pFrame = wxDynamicCast(pWin,CFrameSample);
      pWin = (m_pFrame == NULL) ? pWin->GetParent() : NULL;
    }
  }
  return;
}
const wxString &CNotebookEditSample::GetCurrentLocus()
{
  int nSelect = m_pNotebook->GetSelection() - SA_WINDOW_COUNT;
  const wxString *pRtn = NULL;
  if((nSelect < 0) ||
    (nSelect >= (int)m_asLocus.size())
    )
  {
    pRtn = &mainApp::EMPTY_STRING;
  }
  else
  {
    pRtn = &(m_asLocus.at((size_t)nSelect));
  }
  return *pRtn;
}

bool CNotebookEditSample::Validate()
{
  bool bRtn = true;
  if(!m_bReadOnly)
  {
    wxString sMessage;
    const wxChar *LF(wxS("\n"));
    int nCount = 0;
    int nPage = -1;
    CPanelListIterator itr;
    CPageEditSample *pPage;
    int i = 0;
    for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
    {
      pPage = *itr;
      if( (pPage != NULL) && pPage->NeedsApply() && pPage->IsNewNotesEmpty() )
      {
        sMessage += "You haven't entered notes for ";
        sMessage += pPage->GetPageLabel();
        sMessage += LF;
        nCount++;
        if(nPage < 0)
        {
          nPage = i;
        }
      }
      ++i;
    }
    if(nPage >= 0)
    {
      if(nCount > 1)
      {
        sMessage += LF;
      }
      sMessage += "Do you wish to continue?";
      bRtn = mainApp::Confirm(this,sMessage);
      if(!bRtn)
      {
        Select(nPage);
      }
    }
  }
  return bRtn;
}


bool CNotebookEditSample::TransferDataToWindow()
{

  CPanelListIterator itr;
  CPageEditSample *pPage;
  bool bRtn = true;
  int nPos = 0;
  SETUP_STAT_IMG

  for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
  {
    pPage = *itr;
    if(pPage == NULL)
    {}
    else if( !pPage->TransferDataToPage() )
    {
      bRtn = false;
    }
    else
    {
      m_pNotebook->SetPageImage(nPos,STAT_IMG_PAGE(pPage));
      m_pNotebook->SetPageText(nPos,pPage->GetTreePageLabel());
    }
    nPos++;
  }
  return bRtn;
}

bool CNotebookEditSample::IsModified()
{
  bool bRtn = false;
  if(m_pSample->IsEnabled())
  {
    CPanelListIterator itr;
    CPageEditSample *pPage;
    for(itr = m_vpPanels.begin(); itr != m_vpPanels.end(); ++itr)
    {
      pPage = *itr;
      if( (pPage != NULL) && pPage->NeedsApply() )
      {
        bRtn = true;
        break;
      }
    }
  }
  return bRtn;
}
/*

bool CNotebookEditSample::SetDateTime(const wxDateTime *pTime)
{
  bool bRtn = m_HistTime.SetDateTime(pTime);
  TransferDataToWindow();
  return bRtn;
}
*/


void CNotebookEditSample::OnEnter(wxCommandEvent &)
{
  if(!m_bReadOnly)
  {
    _UpdateMenu();
  }
}
void CNotebookEditSample::OnCellChange(wxGridEvent &)
{
  if(!m_bReadOnly)
  {
    _UpdateMenu();
  }
}
void CNotebookEditSample::_UpdateMenu()
{
  if(m_pFrame != NULL)
  {
    m_pFrame->SetupMenuItems();
    CPageEditSample *pPage = GetCurrentPanel();
    if(pPage != NULL)
    {
      m_pNotebook->SetPageText(m_pNotebook->GetSelection(),pPage->GetTreePageLabel());
    }
  }
}

const wxString &CNotebookEditSample::GetUserID()
{
  return m_pFrame->GetUserID();
}
void CNotebookEditSample::InitiateRepaintData()
{
  if(m_pFrame != NULL)
  {
    m_pFrame->InitiateRepaintData();
  }
}
void CNotebookEditSample::OnChanged(wxBookCtrlEvent &e)
{
//  GetParent()->Refresh();
  _UpdateMenu();
  e.Skip();
}

void CNotebookEditSample::OnChanging(wxBookCtrlEvent &e)
{
  int nPage = e.GetSelection();
  if( nPage != wxNOT_FOUND )
  {
    //bool bUP  = wxGetKeyState(WXK_UP);
    //bool bDown = wxGetKeyState(WXK_DOWN);
    wxWindow *pWin = m_pNotebook->GetPage((size_t) nPage);
    if(pWin == NULL)
    {
      int nPrev = e.GetOldSelection();
      nwxTreebook *pBook = wxDynamicCast(e.GetEventObject(),nwxTreebook);
      if(
        (pBook != m_pNotebook) ||
        (nPrev == wxNOT_FOUND) || 
        (nPrev == nPage)
        )
      {}
      else if(nPrev < nPage)
      {
        m_nSelectPage = (size_t)nPage + 1;
      }
      else if(nPage > 0)
      {
        m_nSelectPage = (size_t)nPage - 1;
      }
      //e.Veto();
    }
  }
}
void CNotebookEditSample::RepaintData()
{
  // STOP HERE - need to implement
  TransferDataToWindow();
}

void CNotebookEditSample::OnTimer(wxTimerEvent &)
{
  if(InBatch() || (m_pNotebook == NULL)) {}
  else if(m_nTimerCount > 0)
  {
    // UGLY HACK!
    //
    // upon creation of this window, (MS Windows)
    // there are rendering problem with the
    // notebook page until the page is changed
    // m_nTimerCount delays changing
    // the displayed page by one timer 'tick'
    // (about 1/4 second)
    //
    //  8/17/2016 djh 
    if(m_pNotebook->IsShown())
    {
      m_nTimerCount--;
    }
  }
  else if(
    (m_nSelectPage < NOTEBOOK_OUT_OF_RANGE) && 
    (m_nSelectPage < m_pNotebook->GetPageCount()) &&
    ((int)m_nSelectPage != m_pNotebook->GetSelection())
    )
  {
    if(m_pNotebook->IsShown())
    {
      m_pNotebook->SetSelection(m_nSelectPage);
      m_nSelectPage = NOTEBOOK_OUT_OF_RANGE;
      m_pFrame->UpdateSizeHack(true);
#ifdef __WXDEBUG__
      if(!m_bTreeDumped) _DumpTree();
#endif
    }
  }
  else
  {
    m_nSelectPage = NOTEBOOK_OUT_OF_RANGE;
  }
}

void CNotebookEditSample::SelectLocus(const wxString &sLocus)
{
  if(!sLocus.IsEmpty())
  {
    size_t i = SA_WINDOW_COUNT;
    size_t BIG = 100000;
    for(std::vector<const wxString>::iterator itr = m_asLocus.begin();
      (itr != m_asLocus.end()) && (i < BIG);
      ++itr)
    {
      if((*itr) == sLocus)
      {
        _SetSelection(i);
        i = BIG;
      }
      i++;
    }
  }
}
void CNotebookEditSample::SelectAlerts(int nAlertType)
{
  if(nAlertType >= 0 && nAlertType < SA_WINDOW_COUNT)
  {
    _SetSelection((size_t)nAlertType);
  }
}


#ifdef __WXDEBUG__

void CNotebookEditSample::_DumpItems(
  std::vector<wxString> *pvs,
  wxTreeCtrl *pTree,
  const wxTreeItemId &id,
  int nLevel)
{
  if(id.IsOk())
  {
    wxString s;
    wxTreeItemId idN;
    if(nLevel >= 0)
    {
      s.Append(wxT("\n"));
      for(int i = 1; i < nLevel; ++i)
      {
        s.Append(wxT("--"));
      }
      s.Append(wxT(" "));
      s.Append(nwxString::FormatNumber(pvs->size()));
      s.Append(wxT(" "));
      s.Append(pTree->GetItemText(id));
      pvs->push_back(s);
    }
    wxTreeItemIdValue cookie;
    for(idN = pTree->GetFirstChild(id,cookie);
      idN.IsOk();
      idN = pTree->GetNextChild(id,cookie))
    {
      _DumpItems(pvs,pTree,idN,nLevel+1);
    }
  }
}
void CNotebookEditSample::_DumpTree()
{
  std::vector<wxString> vs;
  vs.reserve(32);
  wxTreeCtrl *pTree = m_pNotebook->GetTreeCtrl();
  long nStyle1 = pTree->GetWindowStyleFlag();
  long nStyle2 = pTree->GetWindowStyle();
  bool bHideRoot = !!((nStyle1 | nStyle2) & wxTR_HIDE_ROOT);
  wxTreeItemId idRoot = pTree->GetRootItem();
  _DumpItems(&vs,pTree,idRoot,bHideRoot ? -1 : 0);
  wxString s;
  nwxString::Join(vs,&s,"");
  nwxLog_I_WAS_HERE;
  nwxLog::LogMessage(s);
  m_bTreeDumped = true;
}

#endif

BEGIN_EVENT_TABLE(CNotebookEditSample,wxPanel)
EVT_TEXT(wxID_ANY,CNotebookEditSample::OnEnter)
EVT_COMMAND_ENTER(wxID_ANY, CNotebookEditSample::OnEnter)
EVT_GRID_CMD_CELL_CHANGED(wxID_ANY,CNotebookEditSample::OnCellChange)
C_EVT_CHANGING(wxID_ANY, CNotebookEditSample::OnChanging)
C_EVT_CHANGED(wxID_ANY, CNotebookEditSample::OnChanged)
END_EVENT_TABLE()
