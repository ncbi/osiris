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
*  FileName: CDialogArchiveCreate.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CArchiveCollection.h"
#include "CDialogArchiveCreate.h"
#include "mainApp.h"
#include <wx/stattext.h>
#include <wx/sizer.h>


CDialogArchiveCreate::~CDialogArchiveCreate () {}

CDialogArchiveCreate::CDialogArchiveCreate(
  CArchiveCollection *par,
  wxWindow *parent) :
    wxDialog(parent,wxID_ANY,wxT("Create Analysis Archive"),
      wxDefaultPosition, wxDefaultSize, mainApp::DIALOG_STYLE),
    m_pCBIncludeInput(NULL)
{
  wxString sMessage;
  wxString s;
  const wxString &sFileType = par->GetInputFileType();
  size_t n;
  if(!par->HasInputFiles())
  {
    sMessage += wxString::Format(wxT("Cannot find input (.%s) files\n"),
      sFileType);
  }
  else 
  {
    if((n = par->SamplesMissingInputFiles()) > 0)
    {
      if(n == 1)
      {
        s = wxString::Format(wxT("One input (.%s) file is missing\n"),
          sFileType);
      }
      else
      {
        s = wxString::Format(wxT("%d input (.%s) files are missing\n"),
          (int)n,sFileType);
      }
      sMessage += s;
    }
    s = wxString::Format(wxT("Include input (.%s) files"),sFileType);
    m_pCBIncludeInput = new wxCheckBox(this,wxID_ANY,s);
    m_pCBIncludeInput->SetValue(true);
  }
  if((n = par->SamplesMissingPlotFiles()) > 0)
  {
    if(n == 1)
    {
      s = wxT("One plot file is missing\n");
    }
    else
    {
      s = wxString::Format(wxT("%d plot files are missing\n"), (int)n);
    }
    sMessage += s;
  }
  if(par->MissingLadderInfo())
  {
    sMessage += wxT("Ladder info file is missing\n");
  }
  if(par->MissingMessageBook())
  {
    sMessage += wxT("Message book file is missing\n");
  }
  if(par->MissingOAR())
  {
    sMessage += wxString::Format(wxT("Originial file, %s, is missing\n"),
      par->GetOARfileName());
  }
  n = sMessage.Len();
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  if(n > 0)
  {
    sMessage.Truncate(n-1);
    wxStaticText *pLabel = new wxStaticText(this,wxID_ANY,sMessage);
    pSizer->Add(pLabel,0,wxALL ^ wxBOTTOM,ID_BORDER);
  }
  if(m_pCBIncludeInput != NULL)
  {
    pSizer->Add(m_pCBIncludeInput,0,wxALL ^ wxBOTTOM,ID_BORDER);
  }
  m_pCBlocation = new wxCheckBox(this, wxID_ANY, wxT("Show file location"));
  m_pCBlocation->SetValue(true);
  pSizer->Add(m_pCBlocation,0,wxALL ^ wxBOTTOM, ID_BORDER);
  pSizer->Add(CreateButtonSizer(wxOK | wxCANCEL),0,wxALL | wxALIGN_CENTRE, ID_BORDER);
  SetSizer(pSizer);
  pSizer->Layout();
  Fit();
}
