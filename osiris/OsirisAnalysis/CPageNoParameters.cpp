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
*  FileName: CPageNoParameters.cpp
*  Author:   Douglas Hoffman
*
*/
#include "CPageNoParameters.h"
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "wxIDS.h"

CPageNoParameters::CPageNoParameters(
  wxWizard *parent,wxWizardPage *prev) :
    wxWizardPageSimple(parent,prev)
{
  wxStaticText *pLabel1 = new wxStaticText(
    this,wxID_ANY,
    _T("The selected XSLT file has no external parameters."));
  wxStaticText *pLabel2 = new wxStaticText(
    this,wxID_ANY,_T("Select \"Next\" to continue."));
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->AddStretchSpacer(1);
  pSizer->Add(pLabel1,0,wxALIGN_CENTER);
  pSizer->Add(pLabel2,0,wxALIGN_CENTER);
  pSizer->AddStretchSpacer(1);
  SetSizer(pSizer);
  Layout();
}

IMPLEMENT_ABSTRACT_CLASS(CPageNoParameters, wxWizardPageSimple);
