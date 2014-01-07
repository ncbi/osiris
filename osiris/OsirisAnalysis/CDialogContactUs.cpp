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

*  FileName: CDialogContactUs.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CDialogContactUs.h"
#include <wx/hyperlink.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"

CDialogContactUs::CDialogContactUs(wxWindow *parent) :
  wxDialog(parent,wxID_ANY,_T("Contact Us"),
    wxDefaultPosition, wxDefaultSize,wxCAPTION | wxCLOSE_BOX)
{
  const char *URL_EMAIL = "mailto:forensics@ncbi.nlm.nih.gov";
  const char *EMAIL = URL_EMAIL + 7;
  wxSizer *pButtonSizer = CreateButtonSizer(wxOK);
  wxHyperlinkCtrl *pLink = new wxHyperlinkCtrl(
    this,wxID_ANY,_T(EMAIL),_T(URL_EMAIL),
    wxDefaultPosition,
    wxDefaultSize,
    wxNO_BORDER | wxHL_ALIGN_LEFT | wxHL_CONTEXTMENU);
//  wxStaticText *pText1 = new wxStaticText(this,wxID_ANY,
//    _T("Online help is not currently available."));
  wxStaticText *pText2 = new wxStaticText(this,wxID_ANY,
       _T("For assistance, contact us at "));
  wxBoxSizer *pContactSizer = new wxBoxSizer(wxHORIZONTAL);
//  wxBoxSizer *pMessageSizer = new wxBoxSizer(wxVERTICAL);
  pContactSizer->Add(pText2,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
  pContactSizer->Add(pLink,0,wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
//  pMessageSizer->Add(pText1,0,wxALIGN_LEFT);
//  pMessageSizer->Add(pContactSizer,0,wxALIGN_LEFT);
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(pContactSizer,1,wxALL | wxALIGN_CENTRE,ID_BORDER << 2);
  pSizerAll->Add(pButtonSizer,0,wxBOTTOM | wxALIGN_CENTRE, ID_BORDER);
  SetSizer(pSizerAll);
  Layout();
  Fit();
}
