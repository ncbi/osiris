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

*  FileName: CDialogAbout.cpp
*  Author:   Douglas Hoffman
*
*/
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/colour.h>
#include "CImageAbout.h"
#include "CDialogAbout.h"
#include "nwx/nwxStaticBitmap.h"
#include "nwx/nwxColorUtil.h"
#include "mainApp.h"
#include "wxIDS.h"
#include "OsirisVersion.h"

const wxString CDialogAbout::g_sReadMe(_T(
"This software is a \"United States Government Work\" under terms "
"of the United States Copyright Act.  It was written as part of the "
"authors’ official duties as Government employees and thus cannot "
"be copyrighted. This software is freely available to the public "
"for use without a copyright notice.  Restrictions cannot be "
"placed on its present or future use.\n\n"
"Although all reasonable efforts have been taken to ensure the "
"accuracy and reliability of the software, the National Library "
"of Medicine (NLM) and the U.S. Government does not and cannot "
"warrant the performance or results that may be obtained by "
"using this software.  The NLM and the U.S. Government disclaims "
"all warranties as to performance, merchantability or fitness for "
"any particular purpose. In any work or product derived from this "
"material, proper attribution to the authors as the source of the "
"software would be appreciated."));



CPanelAbout::CPanelAbout(wxWindow *parent) : 
    wxPanel(parent,wxID_ANY,wxDefaultPosition, wxDefaultSize,wxBORDER_SIMPLE)
{
  void *buffer = CImageAbout::GetBuffer();
  size_t nSize = CImageAbout::GetBufferSize();
  nwxStaticBitmap *pBitmap = new nwxStaticBitmap(
    this,wxID_ANY,buffer, 
    nSize,
    wxBITMAP_TYPE_PNG);
  wxBoxSizer *pSizer(new wxBoxSizer(wxVERTICAL));
  wxBoxSizer *pSizerTop(new wxBoxSizer(wxVERTICAL));
  pSizer->Add(pBitmap,0,wxALL, ID_BORDER << 2);
  SetBackgroundColour(wxColour(255,255,255));
  pSizerTop->Add(pSizer,1,wxEXPAND);
  SetSizer(pSizerTop);
  Layout();
  Fit();
}

CDialogAbout::CDialogAbout(wxWindow *parent) : 
  wxDialog(parent,wxID_ANY, _T("About OSIRIS " OSIRIS_VERSION "..."),
    wxDefaultPosition, wxDefaultSize,
//    wxDEFAULT_DIALOG_STYLE)
    mainApp::DIALOG_STYLE)
{
  wxBoxSizer *pSizer(new wxBoxSizer(wxHORIZONTAL));
  wxBoxSizer *pSizerRight(new wxBoxSizer(wxVERTICAL));
  wxTextCtrl *pReadMe(new wxTextCtrl(this,wxID_ANY,
    g_sReadMe,wxDefaultPosition,wxSize(200,-1),
    wxTE_READONLY | wxTE_MULTILINE | wxTE_WORDWRAP));
  nwxColorUtil::BackgroundParent(pReadMe,true);
  wxFont fnt(pReadMe->GetFont());
  fnt.SetPointSize(10);
  pReadMe->SetFont(fnt);
  //pReadMe->SetBackgroundColour(*wxWHITE);
  pSizerRight->Add(pReadMe,1,wxEXPAND | wxALL, ID_BORDER);
  pSizerRight->Add(CreateButtonSizer(wxOK),0,wxBOTTOM | wxALIGN_CENTER,ID_BORDER);
  pSizer->Add(new CPanelAbout(this),0,wxEXPAND);
  pSizer->Add(pSizerRight,1,wxEXPAND);
  SetSizer(pSizer);
  Layout();
  Fit();
  Centre();
}
