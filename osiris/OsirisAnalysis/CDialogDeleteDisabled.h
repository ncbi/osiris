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
*  FileName: CDialogDeleteDisabled.h
*  Author:   Douglas Hoffman
*/
#ifndef __C_DIALOG_DELETE_DISABLED_H__
#define __C_DIALOG_DELETE_DISABLED_H__

#include <wx/dialog.h>
#include "nwx/stdb.h"
#include <vector>
#include "nwx/stde.h"
#include "nwx/nsstd.h"

class CPanelUserID;

class CDialogDeleteDisabled: public wxDialog
{
public:
  CDialogDeleteDisabled(
    wxWindow *parent,
    const std::vector<const wxString> &vsNames,
    bool bAllowUserOverride);
  virtual ~CDialogDeleteDisabled();
  const wxString &GetUserID();
private:
  CPanelUserID *m_pPanelUser;
};

#endif