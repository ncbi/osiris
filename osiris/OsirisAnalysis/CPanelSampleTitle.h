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
*  FileName: CPanelSampleTitle.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_SAMPLE_TITLE_H__
#define __C_PANEL_SAMPLE_TITLE_H__

#include <wx/panel.h>
#include <wx/string.h>
#include <wx/dialog.h>
#include <wx/hyperlink.h>

class COARsample;

class CPanelSampleTitle : public wxPanel
{
public:
  CPanelSampleTitle(
    wxWindow *parent,
    const COARsample *pSample, 
    const wxString &sUserID,
    bool bReadOnly);
  virtual ~CPanelSampleTitle() {}
  const wxString &GetUserID()
  {
    return m_sUser;
  }
  void OnClickUser(wxHyperlinkEvent &);
private:
  wxString m_sUser;
  wxHyperlinkCtrl *m_pwUser;
  DECLARE_EVENT_TABLE()
  DECLARE_ABSTRACT_CLASS(CPanelSampleTitle)

};

#endif
