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
*
*  FileName: CILSselection.h
*  Author:   Douglas Hoffman
*
*  container for two Combo Boxes, one for ILS family
*  and one for ILS.  The one for ILS family is shown
*  only when a kit is selected that utilized this
*  feature.  This class provides a wxSizer object
*  for the parent window to show the combo boxes 
*  and labels.
*
*/
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include "CILSselection.h"
#include "CKitList.h"
#include "CILSLadderInfo.h"
#include "mainApp.h"
#include "wxIDS.h"

CILSselection::CILSselection(wxWindow *pParent)
{
  m_pParent = pParent;
  m_pSizer = new wxBoxSizer(wxHORIZONTAL);
  m_pLabelFamily = new wxStaticText(m_pParent,wxID_ANY,wxT("ILS Family"));
  wxArrayString as;
  m_pComboFamily = new wxChoice(m_pParent,ID_ILS_FAMILY);
  m_pComboLS = new wxChoice(m_pParent,wxID_ANY);
}
CILSselection::~CILSselection() {}

void CILSselection::FamilyChange()
{
  wxString sFamily = m_pComboFamily->GetStringSelection();
  if(sFamily != m_sFamily)
  {
    // set up combo box
    CPersistKitList *pKitList = mainApp::GetKitList();
    const CILSLadderInfo *pILS = pKitList->GetILSLadderInfo();
    const CILSfamily *pFam = pILS->GetFamily(sFamily);
    bool bOK = false;
    if(pFam != NULL)
    {
      const std::vector<CILSname *> &vNames = pFam->GetNames();
      m_pComboLS->Clear();
      for(std::vector<CILSname *>::const_iterator itr = vNames.begin();
        itr != vNames.end();
        ++itr)
      {
        m_pComboLS->Append((*itr)->GetDisplayName());
        bOK = true;
      }
    }
    if(bOK)
    {
      m_pComboLS->SetSelection(0);
      m_sFamily = sFamily;
      m_pParent->Layout();
      //m_pParent->Refresh();
    }
    else
    {
      wxString s(wxT("Cannot change ILS family to "));
      s.append(sFamily);
      mainApp::LogMessage(s);
    }
  }
}
void CILSselection::SetKit(const wxString &sKitName)
{
  const int SIZER_COUNT_FAMILY = 4;
    // number of sizer items for ILS family and name
  const int SIZER_COUNT_ILS = 1;
  // number of sizer items for ILS only
  if(sKitName != m_sKit)
  {
    CPersistKitList *pKitList = mainApp::GetKitList();
    const wxArrayString *paILS = pKitList->GetIlsArray(sKitName);
    size_t nCount = m_pSizer->GetChildren().GetCount();
    if((paILS != NULL) && (paILS->Count() > 0))
    {
      if(nCount != SIZER_COUNT_FAMILY)
      {
        // build or rebuild sizer
        // with ILS family and ILS list
        int nFlag = wxALIGN_CENTER_VERTICAL |
            wxALIGN_LEFT | wxRIGHT;
        m_pSizer->Clear();
        m_pSizer->Add(m_pLabelFamily,0,nFlag,ID_BORDER);
        m_pSizer->Add(m_pComboFamily,0,nFlag,ID_BORDER + ID_BORDER);
        m_pSizer->Add(m_pLabelLS,0,nFlag,ID_BORDER);
        m_pSizer->Add(m_pComboLS,0,nFlag,0);
        m_sKit = sKitName;
        m_sFamily.Clear();
        FamilyChange();
        m_pLabelFamily->Show(true);
        m_pComboFamily->Show(true);
        m_pLabelLS->Show(true);
      }
      // rebuild Family combo box
      m_pComboFamily->Clear();
      m_pComboFamily->Append(*paILS);
      m_pComboFamily->SetSelection(0);
    }
    else
    {
      paILS = pKitList->GetLsArray(sKitName);
      if((paILS != NULL) && (paILS->Count() > 0))
      {
        // set up single ILS list
        if(nCount != SIZER_COUNT_ILS)
        {
          m_pSizer->Clear();
          m_pSizer->Add(m_pComboLS,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT,0);
          m_pLabelFamily->Show(false);
          m_pComboFamily->Show(false);
          m_pLabelLS->Show(false);
        }
        m_pComboFamily->Clear();
        m_pComboLS->Clear();
        m_pComboLS->Append(*paILS); // showing LSName, not display name
        m_pParent->Layout();
        //m_pParent->Refresh();
      }
    }
  }
}
