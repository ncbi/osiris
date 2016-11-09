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
*  FileName: CDialogShowLabSettings.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainApp.h"
#include "mainFrame.h"
#include "wxIDS.h"
#include "CPanelLabSettings.h"
#include "CDialogShowLabSettings.h"
#include <wx/sizer.h>

CDialogShowLabSettings::CDialogShowLabSettings(
  wxWindow *parent, wxWindowID id,
  CLabSettings *pLab,
  const CXMLmessageBook *pBook) :
    wxDialog(parent,id,"Lab Settings",
         GET_PERSISTENT_POSITION(CDialogShowLabSettings),
         GET_PERSISTENT_SIZE(CDialogShowLabSettings),
         mainApp::DIALOG_STYLE)
{
  m_pPanel = new CPanelLabSettings(this,true,false);
  m_pPanel->SetData(pLab,pBook,true);
  m_pPanel->TransferDataToWindow();
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  pSizer->Add(m_pPanel,1,wxEXPAND,0);
//  2016-11-03 commented out, don't remember why it's there, annoying
//#ifndef __NO_MDI__
//  SetMaxSize(mainFrame::Size80());
//#endif
  SetSizer(pSizer);
  Layout();
//  Fit();
//  CentreOnParent();
}
CDialogShowLabSettings::~CDialogShowLabSettings() {}

bool CDialogShowLabSettings::TransferDataToWindow()
{
  return true;
}

IMPLEMENT_PERSISTENT_SIZE_POSITION(CDialogShowLabSettings)

BEGIN_EVENT_TABLE(CDialogShowLabSettings,wxDialog)
EVT_PERSISTENT_SIZE_POSITION(CDialogShowLabSettings)
END_EVENT_TABLE()

