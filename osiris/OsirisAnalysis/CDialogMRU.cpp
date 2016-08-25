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
*  FileName: CDialogMRU.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include "mainApp.h"
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "CDialogMRU.h"
#include "OsirisFileTypes.h"
#include "nwx/nwxListMRU.h"
#include "nwx/nwxXmlMRU.h"
#include "CMruListIcons.h"
#include "wxIDS.h"

IMPLEMENT_ABSTRACT_CLASS(CDialogMRU,wxDialog)

void CDialogMRU::_SetupImageList()
{
  long nImage;
#define __ARGS(x) (void *)x, sizeof(x) / sizeof(x[0])

  nImage = m_pListMRU->AddToImageList( __ARGS(iconBatch) );
  m_pListMRU->SetType(EXT_BATCH,DESCRIPTION_BATCH,nImage);

  nImage = m_pListMRU->AddToImageList( __ARGS(iconTable) );
  m_pListMRU->SetType(EXT_REPORT,DESCRIPTION_REPORT,nImage);
  m_pListMRU->SetType(EXT_REPORT_EDITED,DESCRIPTION_REPORT_EDITED,nImage);

  nImage = m_pListMRU->AddToImageList( __ARGS(iconPlot) );
  m_pListMRU->SetType(EXT_GRAPHIC, DESCRIPTION_GRAPHIC, nImage);

#undef __ARGS

}

CDialogMRU::CDialogMRU(
      nwxXmlMRU *pMRU, mainFrame *parent, const wxSize &szDefault,int nStyle) : 
  wxDialog(parent->DialogParent(),wxID_ANY,"Recently Viewed Files",
    GET_PERSISTENT_POSITION(CDialogMRU),GET_PERSISTENT_SIZE_DEFAULT(CDialogMRU,szDefault),
    mainApp::DIALOG_STYLE),
  m_pCBstartup(NULL),
  m_pButtonCleanup(NULL),
  m_pXmlMRU(pMRU),
  m_pParent(parent),
  m_nMAX(10),
  m_tMessage(0)
{
  m_pListMRU = new nwxListMRU(m_pXmlMRU,this);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *pSizerBottom = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *pSizerTop = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText *pLabelSearch(new wxStaticText(
    this,wxID_ANY,"Search: "));
  m_pTextSearch = new wxTextCtrl(
    this,IDtextSearchMRU,"",wxDefaultPosition, wxDefaultSize,0);
  wxButton *pButtonClear = new wxButton(
    this,IDbuttonClearSearchMRU,"Clear",
    wxDefaultPosition, wxDefaultSize,
    wxBU_EXACTFIT);
  pSizerTop->Add(pLabelSearch,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT,0);
  pSizerTop->Add(m_pTextSearch,1,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT | wxEXPAND | wxLEFT,ID_BORDER);
  pSizerTop->Add(pButtonClear,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT | wxLEFT,ID_BORDER);


  pSizer->Add(pSizerTop,0,wxALL | wxEXPAND,ID_BORDER);
  pSizer->Add(m_pListMRU,1,(wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);

  if(nStyle & MRU_STYLE_OPEN_STARTUP)
  {
    CParmOsirisGlobal x;
    m_pCBstartup = new wxCheckBox(
      this,wxID_ANY," Show at startup");
    m_pCBstartup->SetValue(x->GetStartupMRU());
    pSizerBottom->Add(m_pCBstartup,1,
      wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL);
  }
  else
  {
    pSizerBottom->AddStretchSpacer(1);
  }
  pSizerBottom->Add(CreateButtonSizer(wxOK | wxCANCEL),0,wxALIGN_CENTRE);
  if(nStyle & MRU_STYLE_CLEANUP)
  {
    m_pButtonCleanup = new wxButton(
      this,IDbuttonCleanMRU,"Cleanup List",
      wxDefaultPosition, wxDefaultSize,
      wxBU_EXACTFIT);
    m_pButtonCleanup->SetToolTip(
      "Remove deleted files from the list and\n"
      "update the modification time of the\n"
      "remaining files if needed");
    if(!m_pXmlMRU->GetCount())
    {
      m_pButtonCleanup->Enable(false);
    }
    wxBoxSizer *pSizerCleanup = new wxBoxSizer(wxHORIZONTAL);
    pSizerCleanup->AddStretchSpacer(1);
    pSizerCleanup->Add(
      m_pButtonCleanup,0,wxALIGN_CENTRE_VERTICAL | wxALIGN_RIGHT);
    pSizerBottom->Add(
      pSizerCleanup,1,wxALIGN_CENTRE_VERTICAL);
  }
  else
  {
    pSizerBottom->AddStretchSpacer(1);
  }

  pSizer->Add(pSizerBottom,0,
    (wxALL ^ wxTOP) | wxEXPAND, ID_BORDER);
  SetSizer(pSizer);

  pSizer->Layout();
  SetMinSize(wxDefaultSize);
  //CentreOnParent();
  _SetupImageList();
}

CDialogMRU::~CDialogMRU() 
{
  UpdateStartup();
}
void CDialogMRU::UpdateStartup()
{
  if(m_pCBstartup != NULL)
  {
    CParmOsirisGlobal x;
    x->SetStartupMRU(m_pCBstartup->GetValue());
  }
}

bool CDialogMRU::TransferDataToWindow()
{
  bool bRtn = m_pListMRU->Load();
  if(m_pButtonCleanup != NULL)
  {
    m_pButtonCleanup->Enable(!!m_pXmlMRU->GetCount());
  }
  return bRtn;
}

bool CDialogMRU::Validate()
{
  size_t n = (size_t) m_pListMRU->GetSelectedItemCount();
  bool bRtn = true;
  if(n > m_nMAX)
  {
    bool bShow = true;
    bRtn = false;
    if(m_tMessage)
    {
      time_t t;
      time(&t);
      if( (t - m_tMessage) < 3 )
      {
        bShow = false;
      }
      m_tMessage = t;
    }
    if(bShow)
    {
      wxString s;
      s.Printf(
        "The number of files selected, %d,\n"
        "exceeds the maximum of %d",
        (int)n, (int)m_nMAX );
      mainApp::ShowError(s,this);
      time(&m_tMessage);
    }
  }
  else
  {
    m_vsFiles.reserve(n);
    m_tMessage = 0;
  }
  return bRtn;
}
bool CDialogMRU::TransferDataFromWindow()
{
  bool bRtn = Validate();
  m_vsFiles.clear();
  if(bRtn)
  {
    m_pListMRU->GetAllSelected(&m_vsFiles);
  }
  return bRtn;
}

void CDialogMRU::ClearSelection()
{
  m_pListMRU->ClearSelection();
}

bool CDialogMRU::LoadFiles()
{
  //
  //  Lock MRU file
  //  if lock is successful
  //    load all selected files and return true
  //  else
  //    return false
  //
  wxString sFileName;
  wxString sList;
  bool bRtn = true;
  int nNotFound = 0;
  wxArrayString asFiles;
  asFiles.Alloc(m_vsFiles.size());
  if(!m_vsFiles.empty())
  {
    nwxMRUBatch b(m_pXmlMRU); // lock MRU file so last access time can be updated
    for(vector<wxString>::iterator itr = m_vsFiles.begin();
      itr != m_vsFiles.end();
      ++itr)
    {
      if(wxFileName::IsFileReadable(*itr))
      {
        asFiles.Add(*itr);
      }
      else
      {
        sList.Append("\n");
        sList.Append(*itr);
        nNotFound++;
      }
    }
    ClearSelection();
    if(nNotFound)
    {
      wxString sError;
      sError.Printf(
          "The following %s missing or not accessible:",
          (nNotFound == 1) ? "file is" : "files are"
          );
      sError.Append(sList);
      mainApp::ShowError(sError,m_pParent->DialogParent());
    }
    if(!asFiles.IsEmpty())
    {
      m_pParent->OpenFiles(asFiles);
    }
  }
  return bRtn;
}

void CDialogMRU::OnActivate(wxListEvent &e)
{
  m_vsFiles.clear();
  long n = e.GetIndex();
  m_pListMRU->GetItem(n,&m_vsFiles);
  EndModal(wxID_OK);
}

void CDialogMRU::OnSelect(wxListEvent &e)
{
  if(!Validate())
  {
    m_pListMRU->SetSelected(e.GetIndex(),false);
  }
}
void CDialogMRU::OnCleanup(wxCommandEvent &)
{
  int nCount = m_pXmlMRU->GetCount();
  if(nCount > 0)
  {
    {
    CProgressDialogMRU x(this,nCount);
    m_pXmlMRU->UpdateModTimeAll(&x,true);
    }
    TransferDataToWindow();
    nCount = m_pXmlMRU->GetCount();
    if(!nCount)
    {
      m_pButtonCleanup->Enable(false);
    }
  }
}

void CDialogMRU::OnClearSearch(wxCommandEvent &)
{
  m_pTextSearch->Clear();
}
void CDialogMRU::OnSearch(wxCommandEvent &)
{
  m_pListMRU->SetFilter(m_pTextSearch->GetValue());
}

/*
void CDialogMRU::OnColumnClick(wxListEvent &e)
{
  int n = e.GetColumn();
  if(n >= 0) {m_pListMRU->SortBy(n);}
}
*/
IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogMRU)

BEGIN_EVENT_TABLE(CDialogMRU,wxDialog)
EVT_PERSISTENT_SIZE_POSITION(CDialogMRU)
EVT_BUTTON(IDbuttonClearSearchMRU, CDialogMRU::OnClearSearch)
EVT_BUTTON(IDbuttonCleanMRU,CDialogMRU::OnCleanup)
EVT_TEXT(IDtextSearchMRU,CDialogMRU::OnSearch)
EVT_LIST_ITEM_SELECTED(wxID_ANY,CDialogMRU::OnSelect)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY,CDialogMRU::OnActivate)
// EVT_LIST_COL_CLICK(wxID_ANY,CDialogMRU::OnColumnClick)
END_EVENT_TABLE()
