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
*  FileName: CPanelStatus.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __C_PANEL_STATUS_H__
#define __C_PANEL_STATUS_H__

#include <wx/scrolwin.h>
#include <wx/string.h>
#include <wx/hyperlink.h>
#include "nwx/vectorptr.h"
#include "nwx/nsstd.h"

class COARsample;
class COARfile;
class COARlocus;
class CFrameAnalysis;
class CStatusInfo;
class wxBoxSizer;

class CPanelStatus : public wxScrolledWindow
{
public:
  CPanelStatus(
    COARfile *pFile,
    COARsample *pSample,
    CFrameAnalysis *parent,
    wxWindowID id = wxID_ANY);
  virtual ~CPanelStatus();
  void OnClick(wxHyperlinkEvent &e);
  bool CheckSample(COARsample *pSample)
  {
    return(pSample == m_pSample);
  }
private:
  void _AddLink(
    const wxString &s, 
    int nID, 
    COARlocus *pLocus, 
    wxBoxSizer *pSizer);
  void _Build();
  vector<CStatusInfo *> m_vpStatus;
  COARfile *m_pFile;
  COARsample *m_pSample;
  CFrameAnalysis *m_pParent;
  DECLARE_EVENT_TABLE();
};

#endif

