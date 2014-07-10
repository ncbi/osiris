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
*  FileName: CDialogMaxLadderLabel.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CDialogMaxLadderLabel.h"
#include "nwx/nwxIntValidator.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include "wxIDS.h"
#include "CParmOsiris.h"

CDialogMaxLadderLabel::CDialogMaxLadderLabel(wxWindow *parent) :
  wxDialog(parent,wxID_ANY,wxString("Ladder Labels"))
{
  CParmOsirisGlobal parm;
  nwxIntValidator valid(&m_n,1,9999,-1,true);
  m_n = parm->GetMaxLadderLabels();
  m_pText = new wxTextCtrl(this,wxID_ANY,wxEmptyString,
    wxDefaultPosition, wxSize(100,-1),0,valid);
  wxBoxSizer *pSizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText *pLabel = new wxStaticText(this,wxID_ANY,
    "Enter the maximum number of peak labels to\n"
       "display on the ladder when viewed with a sample.");
  pSizer->Add(pLabel,0,wxALL, ID_BORDER);
  pSizer->Add(m_pText,0,(wxALL ^ wxTOP) | wxALIGN_LEFT,ID_BORDER);
  pSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, (
    wxALL ^ wxTOP) | wxALIGN_CENTRE, ID_BORDER);
  SetSizer(pSizer);
  Fit();
  CentreOnParent();
}

bool CDialogMaxLadderLabel::TransferDataToWindow()
{
  wxValidator *pValid = m_pText->GetValidator();
  bool bRtn = (pValid != NULL) 
    ? pValid->TransferToWindow()
    : false;
  return bRtn;
}

bool CDialogMaxLadderLabel::TransferDataFromWindow()
{
  wxValidator *pValid = m_pText->GetValidator();
  bool bRtn = (pValid != NULL) 
    ? pValid->TransferFromWindow()
    : false;
  if(bRtn)
  {
    CParmOsirisGlobal parm;
    parm->SetMaxLadderLabels(m_n);
  }
  return bRtn;
}

bool CDialogMaxLadderLabel::EditValue()
{
  int nValue = m_n;
  int n = ShowModal();
  bool bRtn = (n == wxID_OK || n == wxID_YES)
    && (nValue != m_n);
  return bRtn;
}
