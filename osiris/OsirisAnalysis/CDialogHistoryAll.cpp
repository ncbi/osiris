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
*  FileName: CDialogHistoryAll.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include <wx/sizer.h>
#include "CDialogHistoryAll.h"
#include "COARfile.h"
#include "wxIDS.h"

#define g_SIZE wxSize(180,240)
/*
#define g_STYLE \
      wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | \
        wxTHICK_FRAME | wxRESIZE_BORDER
*/
#define g_TITLE "Select Date and Time"

CDialogHistoryAll::CDialogHistoryAll(
  const wxDateTime *pCurrentSelection,
  COARfile *pFile,
  wxWindow *parent,
  wxWindowID id) :
    wxDialog( parent,
              id,
              g_TITLE,
              wxDefaultPosition, 
              g_SIZE,
              mainApp::DIALOG_STYLE),
    m_timeSelected((time_t)1),
    m_pTimeSelected(NULL)
{
  const set<wxDateTime> *psetHistory = pFile->GetHistory();
  wxArrayString asTime;
  int nSelect = 0;
  if(pCurrentSelection != NULL)
  {
    m_timeSelected = *pCurrentSelection;
    m_pTimeSelected = &m_timeSelected;
  }

  asTime.Alloc(psetHistory->size() + 3);
  asTime.Add("Current");
  m_vTime.reserve(psetHistory->size() + 1);
  for(set<wxDateTime>::const_reverse_iterator itr = psetHistory->rbegin();
    itr != psetHistory->rend();
    ++itr)
  {
    if(_AppendTime(&asTime,*itr))
    {
      nSelect = (int) m_vTime.size();
    }
  }
  const wxDateTime &dtBegin(pFile->GetCreationTime());
  size_t nSize = m_vTime.size();
  if(nSize && (dtBegin < m_vTime.at(nSize - 1)))
  {
    if(_AppendTime(&asTime,dtBegin))
    {
      nSelect = (int) m_vTime.size();
    }
  }
  if(nSize)
  {
    wxDateTime dt0((time_t)0);
    if(_AppendTime(&asTime,dt0))
    {
      nSelect = (int) m_vTime.size();
    }
  }


  // now build it and they will look

  m_pList = new wxListBox(this,wxID_ANY,
    wxDefaultPosition, wxDefaultSize,
    asTime,wxLB_SINGLE | wxLB_NEEDED_SB);
  m_pList->Select(nSelect);
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(m_pList,1,wxALL | wxEXPAND, ID_BORDER);
  pSizer->Add(
    CreateButtonSizer(wxOK | wxCANCEL),
    0,(wxALL ^ wxTOP) | wxALIGN_CENTRE,ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
}

CDialogHistoryAll::~CDialogHistoryAll() {;}

bool CDialogHistoryAll::TransferDataFromWindow()
{
  int n = m_pList->GetSelection();
  bool bRtn = (n != wxNOT_FOUND);
  if(!bRtn)
  {
    wxString s("Please select a historic date/time when this data was edited.");
    mainApp::ShowError(s,this);
    m_pTimeSelected = NULL;
  }
  else if(n > 0)
  {
    m_timeSelected = m_vTime.at((size_t)n - 1);
    m_pTimeSelected = &m_timeSelected;
  }
  else
  {
    // the first item, 'Current', was selected
    m_pTimeSelected = NULL;
  }
  return bRtn;
}

void CDialogHistoryAll::OnListDoubleClick(wxCommandEvent &)
{
  if(TransferDataFromWindow())
  {
    EndModal(wxID_OK);
  }
}

BEGIN_EVENT_TABLE(CDialogHistoryAll,wxDialog)
EVT_LISTBOX_DCLICK(wxID_ANY,CDialogHistoryAll::OnListDoubleClick)
END_EVENT_TABLE()
