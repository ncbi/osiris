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
*  FileName: CDialogPlotMessage.cpp
*  Author:   Douglas Hoffman
*  Purpose:  When showing history or editing from a plot window,
*    an analysis file must be used.  This 
*     
*/
#include <wx/artprov.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "CDialogPlotMessage.h"

#ifdef __WXMAC__
#define PLF " "
#else
#define PLF "\n"
#endif
const wxString CDialogPlotMessageFind::MSG_HISTORY(
"This plot was not initiated from an analysis window and therefore" PLF
"historic data are not yet available.  Do you wish to search for an" PLF
"associated OSIRIS Analysis File now?"
);

const wxString CDialogPlotMessageFind::MSG_EDIT(
"Data edited from a plot window is saved in its corresponding" PLF
"OSIRIS Analysis file and the OSIRIS Plot file is not modified. " PLF
"The analysis file has not yet been identified because this plot" PLF
"was not initiated from an analysis window.  Would you like to" PLF
"search for an associated OSIRIS Analysis File now?"
);

const wxString CDialogPlotMessageFind::MSG_BINS(
  "This plot was not initiated from an analysis window and ladder" PLF
  "allele bin data are not available.  Do you wish to search for an" PLF
  "associated OSIRIS Analysis File now?"
);

const wxString CDialogPlotMessageFind::MSG_TABLE(
"This plot was not initiated from an analysis window and therefore" PLF
"the data are not yet available.  Do you wish to search for an" PLF
"associated OSIRIS Analysis File now?"
);

const wxString CDialogPlotMessageFind::MSG_PRINT(
  "This plot was not initiated from an analysis window and therefore" PLF
  "not all data needed for printing are available.  Do you wish" PLF
  "to search for an associated OSIRIS Analysis File now?"
);

CDialogPlotMessage::CDialogPlotMessage(wxWindow *parent, const wxString &sFileName) 
  : wxDialog(parent,wxID_ANY,wxString("Please Note..."))
{
  wxString sMessage(
    "Data edited from a plot window is saved to\n"
    "its associated OSIRIS Analysis Report File"
    );
  if(!sFileName.IsEmpty())
  {
    sMessage.Append("\n");
    sMessage.Append(sFileName);
  }
  wxBitmap bitMap = wxArtProvider::GetBitmap(wxART_INFORMATION);
  wxStaticBitmap *pIcon = new wxStaticBitmap(
    this,wxID_ANY,bitMap);
  wxStaticText *pText = new wxStaticText(
    this,wxID_ANY,sMessage);
  m_pCheckBox = new wxCheckBox(this, wxID_ANY,
    "Do not show this message again.");

  wxBoxSizer *pAcross = new wxBoxSizer(wxHORIZONTAL);
  pAcross->Add(pIcon,0,wxALIGN_CENTRE | wxALL, 11);
  pAcross->Add(pText,0,
    wxALIGN_CENTRE_VERTICAL | wxALIGN_LEFT | wxALL,8);

  wxBoxSizer *pDown = new wxBoxSizer(wxVERTICAL);
  pDown->Add(pAcross,0,wxALIGN_LEFT);
  pDown->Add(m_pCheckBox,0,wxALIGN_LEFT | wxLEFT, 11);
  pDown->Add(CreateButtonSizer(wxOK),
    0,wxALL | wxALIGN_CENTRE, 11);
  SetSizer(pDown);
  Layout();
}

